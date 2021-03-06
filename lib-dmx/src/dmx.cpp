/**
 * @file dmx.cpp
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

#include <stdint.h>
#ifndef NDEBUG
 #include <stdio.h>
#endif
#include <assert.h>

#include "dmx.h"

Dmx::Dmx(uint8_t nGpioPin, bool DoInit): m_IsInitDone(DoInit) {
#ifndef NDEBUG
	printf("Dmx::Dmx nGpioPin=%d, DoInit=%d\n", nGpioPin, DoInit);
#endif
	dmx_init_set_gpiopin(nGpioPin);

	if (DoInit) {
		dmx_init();
	}
}

Dmx::~Dmx(void) {
}

void Dmx::Init(void) {
	assert(!m_IsInitDone);

	if (!m_IsInitDone) {
		dmx_init();
		m_IsInitDone = true;
	}
}

