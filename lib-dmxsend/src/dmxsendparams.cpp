/**
 * @file dmxsendparams.cpp
 *
 */
/* Copyright (C) 2017-2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#if !defined(__clang__)	// Needed for compiling on MacOS
 #pragma GCC push_options
 #pragma GCC optimize ("Os")
#endif

#include <stdint.h>
#include <string.h>
#ifndef NDEBUG
 #include <stdio.h>
#endif
#include <assert.h>

#include "dmxparams.h"
#include "dmxsendconst.h"

#include "readconfigfile.h"
#include "sscan.h"

#define DMX_PARAMS_MIN_BREAK_TIME		9
#define DMX_PARAMS_DEFAULT_BREAK_TIME	9
#define DMX_PARAMS_MAX_BREAK_TIME		127

#define DMX_PARAMS_MIN_MAB_TIME			1
#define DMX_PARAMS_DEFAULT_MAB_TIME		1
#define DMX_PARAMS_MAX_MAB_TIME			127

#define DMX_PARAMS_DEFAULT_REFRESH_RATE	40

DMXParams::DMXParams(DMXParamsStore *pDMXParamsStore) : m_pDMXParamsStore(pDMXParamsStore) {
	m_tDMXParams.nSetList = 0;
	m_tDMXParams.nBreakTime = DMX_PARAMS_DEFAULT_BREAK_TIME;
	m_tDMXParams.nMabTime = DMX_PARAMS_DEFAULT_MAB_TIME;
	m_tDMXParams.nRefreshRate = DMX_PARAMS_DEFAULT_REFRESH_RATE;
}

DMXParams::~DMXParams(void) {
	m_tDMXParams.nSetList = 0;
}

bool DMXParams::Load(void) {
	m_tDMXParams.nSetList = 0;

	ReadConfigFile configfile(DMXParams::staticCallbackFunction, this);

	if (configfile.Read(DMXSendConst::PARAMS_FILE_NAME)) {
		// There is a configuration file
		if (m_pDMXParamsStore != 0) {
			m_pDMXParamsStore->Update(&m_tDMXParams);
		}
	} else if (m_pDMXParamsStore != 0) {
		m_pDMXParamsStore->Copy(&m_tDMXParams);
	} else {
		return false;
	}

	return true;
}

void DMXParams::Load(const char* pBuffer, uint32_t nLength) {
	assert(pBuffer != 0);
	assert(nLength != 0);
	assert(m_pDMXParamsStore != 0);

	if (m_pDMXParamsStore == 0) {
		return;
	}

	m_tDMXParams.nSetList = 0;

	ReadConfigFile config(DMXParams::staticCallbackFunction, this);

	config.Read(pBuffer, nLength);

	m_pDMXParamsStore->Update(&m_tDMXParams);
}

void DMXParams::callbackFunction(const char *pLine) {
	assert(pLine != 0);

	uint8_t nValue8;

	if (Sscan::Uint8(pLine, DMXSendConst::PARAMS_BREAK_TIME, &nValue8) == SSCAN_OK) {
		if ((nValue8 >= DMX_PARAMS_MIN_BREAK_TIME) && (nValue8 <= DMX_PARAMS_MAX_BREAK_TIME)) {
			m_tDMXParams.nBreakTime = nValue8;
			m_tDMXParams.nSetList |= DMX_SEND_PARAMS_MASK_BREAK_TIME;
		}
	} else if (Sscan::Uint8(pLine, DMXSendConst::PARAMS_MAB_TIME, &nValue8) == SSCAN_OK) {
		if ((nValue8 >= DMX_PARAMS_MIN_MAB_TIME) && (nValue8 <= DMX_PARAMS_MAX_MAB_TIME)) {
			m_tDMXParams.nMabTime = nValue8;
			m_tDMXParams.nSetList |= DMX_SEND_PARAMS_MASK_MAB_TIME;
		}
	} else if (Sscan::Uint8(pLine, DMXSendConst::PARAMS_REFRESH_RATE, &nValue8) == SSCAN_OK) {
		m_tDMXParams.nRefreshRate = nValue8;
		m_tDMXParams.nSetList |= DMX_SEND_PARAMS_MASK_REFRESH_RATE;
	}
}

void DMXParams::Dump(void) {
#ifndef NDEBUG
	if (m_tDMXParams.nSetList == 0) {
		return;
	}

	printf("%s::%s \'%s\':\n", __FILE__, __FUNCTION__, DMXSendConst::PARAMS_FILE_NAME);

	if (isMaskSet(DMX_SEND_PARAMS_MASK_BREAK_TIME)) {
		printf(" %s=%d\n", DMXSendConst::PARAMS_BREAK_TIME, static_cast<int>(m_tDMXParams.nBreakTime));
	}

	if (isMaskSet(DMX_SEND_PARAMS_MASK_MAB_TIME)) {
		printf(" %s=%d\n", DMXSendConst::PARAMS_MAB_TIME, static_cast<int>(m_tDMXParams.nMabTime));
	}

	if (isMaskSet(DMX_SEND_PARAMS_MASK_REFRESH_RATE)) {
		printf(" %s=%d\n", DMXSendConst::PARAMS_REFRESH_RATE, static_cast<int>(m_tDMXParams.nRefreshRate));
	}
#endif
}

void DMXParams::staticCallbackFunction(void *p, const char *s) {
	assert(p != 0);
	assert(s != 0);

	(static_cast<DMXParams*>(p))->callbackFunction(s);
}
