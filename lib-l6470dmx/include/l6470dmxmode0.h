/**
 * @file l6470dmxmode0.h
 *
 */
/* Copyright (C) 2017-2019 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#ifndef L6470DMXMODE0_H_
#define L6470DMXMODE0_H_

#include "l6470dmxmode.h"
#include "l6470.h"

#include "motorparams.h"

class L6470DmxMode0: public L6470DmxMode {
public:
	L6470DmxMode0(L6470 *pL6470);
	~L6470DmxMode0(void);

	void Start(void);
	void Stop(void);

	void Data(const uint8_t*);

	static TL6470DmxModes GetMode(void) {
		return L6470DMXMODE0;
	}

	static uint8_t GetDmxFootPrint(void) {
		return 1;
	}

private:
	L6470 *m_pL6470;
	float m_fMinSpeed;
	float m_fMaxSpeed;
};

#endif /* L6470DMXMODE0_H_ */
