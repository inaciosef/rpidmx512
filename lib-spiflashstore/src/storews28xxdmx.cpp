/**
 * @file storews28xxdmx.cpp
 *
 */
/* Copyright (C) 2018-2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
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
#include <assert.h>

#include "storews28xxdmx.h"

#include "ws28xxdmxparams.h"
#include "ws28xxdmxstore.h"

#include "spiflashstore.h"

#include "debug.h"

StoreWS28xxDmx *StoreWS28xxDmx::s_pThis = 0;

StoreWS28xxDmx::StoreWS28xxDmx(void) {
	DEBUG_ENTRY

	s_pThis = this;

	DEBUG_PRINTF("%p", s_pThis);

	DEBUG_EXIT
}

StoreWS28xxDmx::~StoreWS28xxDmx(void) {
	DEBUG_ENTRY

	DEBUG_EXIT
}

void StoreWS28xxDmx::Update(const struct TWS28xxDmxParams *pWS28xxDmxParams) {
	DEBUG_ENTRY

	SpiFlashStore::Get()->Update(STORE_WS28XXDMX, pWS28xxDmxParams, sizeof(struct TWS28xxDmxParams));

	DEBUG_EXIT
}

void StoreWS28xxDmx::Copy(struct TWS28xxDmxParams *pWS28xxDmxParams) {
	DEBUG_ENTRY

	SpiFlashStore::Get()->Copy(STORE_WS28XXDMX, pWS28xxDmxParams, sizeof(struct TWS28xxDmxParams));

	DEBUG_EXIT
}

void StoreWS28xxDmx::SaveDmxStartAddress(uint16_t nDmxStartAddress) {
	DEBUG_ENTRY

	SpiFlashStore::Get()->Update(STORE_WS28XXDMX, __builtin_offsetof(struct TWS28xxDmxParams, nDmxStartAddress), &nDmxStartAddress, sizeof(uint32_t), WS28XXDMX_PARAMS_MASK_DMX_START_ADDRESS);

	DEBUG_EXIT
}
