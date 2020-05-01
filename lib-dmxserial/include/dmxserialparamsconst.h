/**
 * @file dmxserialparamsconst.h
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

#ifndef DMXSERIALPARAMSCONST_H_
#define DMXSERIALPARAMSCONST_H_

#include <stdint.h>

class DmxSerialParamsConst {
public:
	alignas(uint32_t) static const char FILE_NAME[];

	alignas(uint32_t) static const char TYPE[];

	alignas(uint32_t) static const char UART_BAUD[];
	alignas(uint32_t) static const char UART_BITS[];
	alignas(uint32_t) static const char UART_PARITY[];
	alignas(uint32_t) static const char UART_STOPBITS[];

	alignas(uint32_t) static const char SPI_SPEED_HZ[];
	alignas(uint32_t) static const char SPI_MODE[];

	alignas(uint32_t) static const char I2C_ADDRESS[];
	alignas(uint32_t) static const char I2C_SPEED_MODE[];
};

#endif /* DMXSERIALPARAMSCONST_H_ */
