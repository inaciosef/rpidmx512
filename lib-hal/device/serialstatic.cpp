/**
 * @file serialstatic.cpp
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

#include <stdint.h>
#include <string.h>

#include "serial.h"

#include "debug.h"

constexpr char aType[SERIAL_TYPE_UNDEFINED][5] = { "uart", "spi", "i2c" };
constexpr char aUartParity[SERIAL_UART_PARITY_UNDEFINED][5] = { "none", "odd", "even" };
constexpr char aI2cSpeed[SERIAL_I2C_SPEED_MODE_UNDEFINED][9] = { "standard", "fast" };

const char *Serial::GetType(enum TSerialTypes tType) {
	if (tType < SERIAL_TYPE_UNDEFINED) {
		return aType[tType];
	}

	return "Undefined";
}

enum TSerialTypes Serial::GetType(const char *pType) {
	for (uint32_t i = 0; i < sizeof(aType) / sizeof(aType[0]); i++) {
		if (strcasecmp(aType[i], pType) == 0) {
			return static_cast<TSerialTypes>(i);
		}
	}

	return SERIAL_TYPE_UART;
}

const char *Serial::GetUartParity(enum TSerialUartParity tParity) {
	if (tParity < SERIAL_UART_PARITY_UNDEFINED) {
		return aUartParity[tParity];
	}

	return "Undefined";
}

enum TSerialUartParity Serial::GetUartParity(const char *pParity) {
	for (uint32_t i = 0; i < sizeof(aUartParity) / sizeof(aUartParity[0]); i++) {
		if (strcasecmp(aUartParity[i], pParity) == 0) {
			return static_cast<TSerialUartParity>(i);
		}
	}

	return SERIAL_UART_PARITY_NONE;
}

const char *Serial::GetI2cSpeed(enum TSerialI2cSpeedModes tSpeed) {
	if (tSpeed < SERIAL_I2C_SPEED_MODE_UNDEFINED) {
		return aI2cSpeed[tSpeed];
	}

	return "Undefined";

}
enum TSerialI2cSpeedModes Serial::GetI2cSpeed(const char *pSpeed) {
	for (uint32_t i = 0; i < sizeof(aI2cSpeed) / sizeof(aI2cSpeed[0]); i++) {
		if (strcasecmp(aI2cSpeed[i], pSpeed) == 0) {
			return static_cast<TSerialI2cSpeedModes>(i);
		}
	}

	return SERIAL_I2C_SPEED_MODE_FAST;
}
