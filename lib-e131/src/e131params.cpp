/**
 * @file e131params.cpp
 *
 */
/* Copyright (C) 2016-2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#include <stdint.h>
#include <string.h>
#ifndef NDEBUG
 #include <stdio.h>
#endif
#include <assert.h>

#include "e131params.h"
#include "e131paramsconst.h"
#include "e131.h"

#include "readconfigfile.h"
#include "sscan.h"

#include "lightsetconst.h"

#include "debug.h"

#define BOOL2STRING(b)			(b) ? "Yes" : "No"
#define MERGEMODE2STRING(m)		(m == E131_MERGE_HTP) ? "HTP" : "LTP"

E131Params::E131Params(E131ParamsStore *pE131ParamsStore):m_pE131ParamsStore(pE131ParamsStore) {
	memset(&m_tE131Params, 0, sizeof(struct TE131Params));

	m_tE131Params.nUniverse = E131_UNIVERSE_DEFAULT;
	m_tE131Params.nNetworkTimeout = E131_NETWORK_DATA_LOSS_TIMEOUT_SECONDS;
	m_tE131Params.nDirection = E131_OUTPUT_PORT;
	m_tE131Params.nPriority = E131_PRIORITY_DEFAULT;
}

E131Params::~E131Params(void) {
}

bool E131Params::Load(void) {
	m_tE131Params.nSetList = 0;

	ReadConfigFile configfile(E131Params::staticCallbackFunction, this);

	if (configfile.Read(E131ParamsConst::FILE_NAME)) {
		// There is a configuration file
		if (m_pE131ParamsStore != 0) {
			m_pE131ParamsStore->Update(&m_tE131Params);
		}
	} else if (m_pE131ParamsStore != 0) {
		m_pE131ParamsStore->Copy(&m_tE131Params);
	} else {
		return false;
	}

	return true;
}

void E131Params::Load(const char* pBuffer, uint32_t nLength) {
	assert(pBuffer != 0);
	assert(nLength != 0);

	assert(m_pE131ParamsStore != 0);

	if (m_pE131ParamsStore == 0) {
		return;
	}

	m_tE131Params.nSetList = 0;

	ReadConfigFile config(E131Params::staticCallbackFunction, this);

	config.Read(pBuffer, nLength);

	m_pE131ParamsStore->Update(&m_tE131Params);
}

void E131Params::callbackFunction(const char *pLine) {
	assert(pLine != 0);

	char value[16];
	uint8_t len;
	uint8_t value8;
	uint16_t value16;
	float fValue;

	if (Sscan::Uint16(pLine, LightSetConst::PARAMS_UNIVERSE, &value16) == SSCAN_OK) {
		if ((value16 == 0) || (value16 > E131_UNIVERSE_MAX)) {
			m_tE131Params.nUniverse = E131_UNIVERSE_DEFAULT;
		} else {
			m_tE131Params.nUniverse = value16;
		}
		m_tE131Params.nSetList |= E131_PARAMS_MASK_UNIVERSE;
		return;
	}

	len = 3;
	if (Sscan::Char(pLine, E131ParamsConst::MERGE_MODE, value, &len) == SSCAN_OK) {
		if (memcmp(value, "ltp", 3) == 0) {
			m_tE131Params.nMergeMode = E131_MERGE_LTP;
			m_tE131Params.nSetList |= E131_PARAMS_MASK_MERGE_MODE;
		} else if (memcmp(value, "htp", 3) == 0) {
			m_tE131Params.nMergeMode = E131_MERGE_HTP;
			m_tE131Params.nSetList |= E131_PARAMS_MASK_MERGE_MODE;
		}
		return;
	}

	for (uint32_t i = 0; i < E131_PARAMS_MAX_PORTS; i++) {
		if (Sscan::Uint16(pLine, E131ParamsConst::UNIVERSE_PORT[i], &value16) == SSCAN_OK) {
			m_tE131Params.nUniversePort[i] = value16;
			m_tE131Params.nSetList |= (E131_PARAMS_MASK_UNIVERSE_A << i);
			return;
		}

		len = 3;
		if (Sscan::Char(pLine, E131ParamsConst::MERGE_MODE_PORT[i], value, &len) == SSCAN_OK) {
			if (memcmp(value, "ltp", 3) == 0) {
				m_tE131Params.nMergeModePort[i] = E131_MERGE_LTP;
				m_tE131Params.nSetList |= (E131_PARAMS_MASK_MERGE_MODE_A << i);
			} else if (memcmp(value, "htp", 3) == 0) {
				m_tE131Params.nMergeModePort[i] = E131_MERGE_HTP;
				m_tE131Params.nSetList |= (E131_PARAMS_MASK_MERGE_MODE_A << i);
			}
			return;
		}
	}

	if (Sscan::Float(pLine, E131ParamsConst::NETWORK_DATA_LOSS_TIMEOUT, &fValue) == SSCAN_OK) {
		m_tE131Params.nNetworkTimeout = fValue;
		m_tE131Params.nSetList |= E131_PARAMS_MASK_NETWORK_TIMEOUT;
		return;
	}

	if (Sscan::Uint8(pLine, E131ParamsConst::DISABLE_MERGE_TIMEOUT, &value8) == SSCAN_OK) {
		m_tE131Params.bDisableMergeTimeout = (value8 != 0);
		m_tE131Params.nSetList |= E131_PARAMS_MASK_MERGE_TIMEOUT;
		return;
	}

	if (Sscan::Uint8(pLine, LightSetConst::PARAMS_ENABLE_NO_CHANGE_UPDATE, &value8) == SSCAN_OK) {
		m_tE131Params.bEnableNoChangeUpdate = (value8 != 0);
		m_tE131Params.nSetList |= E131_PARAMS_MASK_ENABLE_NO_CHANGE_OUTPUT;
		return;
	}

	len = 5;
	if (Sscan::Char(pLine, E131ParamsConst::DIRECTION, value, &len) == SSCAN_OK) {
		if (memcmp(value, "input", 5) == 0) {
			m_tE131Params.nDirection = E131_INPUT_PORT;
			m_tE131Params.nSetList |= E131_PARAMS_MASK_DIRECTION;
		}
		return;
	}

	len = 6;
	if (Sscan::Char(pLine, E131ParamsConst::DIRECTION, value, &len) == SSCAN_OK) {
		if (memcmp(value, "output", 6) == 0) {
			m_tE131Params.nDirection = E131_OUTPUT_PORT;
			m_tE131Params.nSetList |= E131_PARAMS_MASK_DIRECTION;
		}
		return;
	}

	if (Sscan::Uint8(pLine, E131ParamsConst::PRIORITY, &value8) == SSCAN_OK) {
		if ((value8 >= E131_PRIORITY_LOWEST) && (value8 <= E131_PRIORITY_HIGHEST)) {
			m_tE131Params.nPriority = value8;
			m_tE131Params.nSetList |= E131_PARAMS_MASK_PRIORITY;
		}
		return;
	}

}

void E131Params::Dump(void) {
#ifndef NDEBUG
	printf("%s::%s \'%s\':\n", __FILE__, __FUNCTION__, E131ParamsConst::FILE_NAME);

	if (isMaskSet(E131_PARAMS_MASK_UNIVERSE)) {
		printf(" %s=%d\n", LightSetConst::PARAMS_UNIVERSE,
				static_cast<int>(m_tE131Params.nUniverse));
	}

	for (unsigned i = 0; i < E131_PARAMS_MAX_PORTS; i++) {
		if (isMaskSet(E131_PARAMS_MASK_UNIVERSE_A << i)) {
			printf(" %s=%d\n", E131ParamsConst::UNIVERSE_PORT[i], m_tE131Params.nUniversePort[i]);
		}
	}

	if (isMaskSet(E131_PARAMS_MASK_MERGE_MODE)) {
		printf(" %s=%s\n", E131ParamsConst::MERGE_MODE, MERGEMODE2STRING(m_tE131Params.nMergeMode));
	}

	for (unsigned i = 0; i < E131_PARAMS_MAX_PORTS; i++) {
		if (isMaskSet(E131_PARAMS_MASK_MERGE_MODE_A << i)) {
			printf(" %s=%s\n", E131ParamsConst::MERGE_MODE_PORT[i], MERGEMODE2STRING(m_tE131Params.nMergeModePort[i]));
		}
	}

	if (isMaskSet(E131_PARAMS_MASK_NETWORK_TIMEOUT)) {
		printf(" %s=%.1f [%s]\n", E131ParamsConst::NETWORK_DATA_LOSS_TIMEOUT, m_tE131Params.nNetworkTimeout, (m_tE131Params.nNetworkTimeout != 0) ? "" : "Disabled");
	}

	if(isMaskSet(E131_PARAMS_MASK_MERGE_TIMEOUT)) {
		printf(" %s=%d [%s]\n", E131ParamsConst::DISABLE_MERGE_TIMEOUT,
				static_cast<int>(m_tE131Params.bDisableMergeTimeout),
				BOOL2STRING(m_tE131Params.bDisableMergeTimeout));
	}

	if(isMaskSet(E131_PARAMS_MASK_ENABLE_NO_CHANGE_OUTPUT)) {
		printf(" %s=%d [%s]\n", LightSetConst::PARAMS_ENABLE_NO_CHANGE_UPDATE,
				static_cast<int>(m_tE131Params.bEnableNoChangeUpdate),
				BOOL2STRING(m_tE131Params.bEnableNoChangeUpdate));
	}

	if(isMaskSet(E131_PARAMS_MASK_DIRECTION)) {
		printf(" %s=%d [%s]\n", E131ParamsConst::DIRECTION,
				static_cast<int>(m_tE131Params.nDirection),
				m_tE131Params.nDirection == E131_INPUT_PORT ?
						"Input" : "Output");
	}

	if (isMaskSet(E131_PARAMS_MASK_PRIORITY)) {
		printf(" %s=%d\n", E131ParamsConst::PRIORITY, m_tE131Params.nPriority);
	}
#endif
}

uint16_t E131Params::GetUniverse(uint8_t nPort, bool &IsSet) {
	assert(nPort < E131_PARAMS_MAX_PORTS);

	IsSet = isMaskSet(E131_PARAMS_MASK_UNIVERSE_A << nPort);

	return m_tE131Params.nUniversePort[nPort];
}

void E131Params::staticCallbackFunction(void *p, const char *s) {
	assert(p != 0);
	assert(s != 0);

	(static_cast<E131Params*>(p))->callbackFunction(s);
}
