/**
 * @file dmxgpioparamsconst.h
 *
 */
/* Copyright (C) 2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#ifndef DMXGPIOPARAMSCONST_H_
#define DMXGPIOPARAMSCONST_H_

#include "dmx.h"

enum {
	DMXGPIOPARAMS_DATA_DIRECTION_OUT_MAX_LENGTH = 21
};

class DmxGpioParamsConst {
public:

	static const char FILE_NAME[];

	static const char DATA_DIRECTION[];
	static const char DATA_DIRECTION_OUT[DMX_MAX_OUT][DMXGPIOPARAMS_DATA_DIRECTION_OUT_MAX_LENGTH];
};

#endif /* DMXGPIOPARAMSCONST_H_ */
