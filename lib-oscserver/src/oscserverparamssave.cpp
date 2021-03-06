/**
 * @file oscserverparamssave.cpp
 *
 */
/* Copyright (C) 2019-2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
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
#include <assert.h>

#include "oscserverparms.h"
#include "oscserverconst.h"
#include "oscconst.h"

#include "lightsetconst.h"

#include "propertiesbuilder.h"

#include "debug.h"

void OSCServerParams::Builder(const struct TOSCServerParams *ptOSCServerParams, char *pBuffer, uint32_t nLength, uint32_t& nSize) {
	DEBUG_ENTRY

	assert(pBuffer != 0);

	if (ptOSCServerParams != 0) {
		memcpy(&m_tOSCServerParams, ptOSCServerParams, sizeof(struct TOSCServerParams));
	} else {
		m_pOSCServerParamsStore->Copy(&m_tOSCServerParams);
	}

	PropertiesBuilder builder(OSCServerConst::PARAMS_FILE_NAME, pBuffer, nLength);

	builder.Add(OscConst::PARAMS_INCOMING_PORT, m_tOSCServerParams.nIncomingPort, isMaskSet(OSCSERVER_PARAMS_MASK_INCOMING_PORT));
	builder.Add(OscConst::PARAMS_OUTGOING_PORT, m_tOSCServerParams.nOutgoingPort, isMaskSet(OSCSERVER_PARAMS_MASK_OUTGOING_PORT));
	builder.Add(OSCServerConst::PARAMS_PATH, m_tOSCServerParams.aPath, isMaskSet(OSCSERVER_PARAMS_MASK_PATH));
	builder.Add(OSCServerConst::PARAMS_PATH_INFO, m_tOSCServerParams.aPathInfo, isMaskSet(OSCSERVER_PARAMS_MASK_PATH_INFO));
	builder.Add(OSCServerConst::PARAMS_PATH_BLACKOUT, m_tOSCServerParams.aPathBlackOut, isMaskSet(OSCSERVER_PARAMS_MASK_PATH_BLACKOUT));
	builder.Add(OSCServerConst::PARAMS_TRANSMISSION, m_tOSCServerParams.bPartialTransmission, isMaskSet(OSCSERVER_PARAMS_MASK_TRANSMISSION));

	builder.Add(LightSetConst::PARAMS_ENABLE_NO_CHANGE_UPDATE, m_tOSCServerParams.bEnableNoChangeUpdate, isMaskSet(OSCSERVER_PARAMS_MASK_ENABLE_NO_CHANGE_OUTPUT));

	nSize = builder.GetSize();

	DEBUG_EXIT
	return;
}

void OSCServerParams::Save(char *pBuffer, uint32_t nLength, uint32_t &nSize) {
	DEBUG_ENTRY

	if (m_pOSCServerParamsStore == 0) {
		nSize = 0;
		DEBUG_EXIT
		return;
	}

	Builder(0, pBuffer, nLength, nSize);

	return;
}
