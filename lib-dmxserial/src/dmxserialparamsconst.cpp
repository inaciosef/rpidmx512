/**
 * @file dmxserialparamsconst.cpp
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

#include "dmxserialparamsconst.h"

alignas(uint32_t) const char DmxSerialParamsConst::FILE_NAME[]  = "serial.txt";

alignas(uint32_t) const char  DmxSerialParamsConst::TYPE[] = "type";

alignas(uint32_t) const char DmxSerialParamsConst::UART_BAUD[]  = "uart_baud";
alignas(uint32_t) const char DmxSerialParamsConst::UART_BITS[]  = "uart_bits";
alignas(uint32_t) const char DmxSerialParamsConst::UART_PARITY[]  = "uart_parity";
alignas(uint32_t) const char DmxSerialParamsConst::UART_STOPBITS[]  = "uart_stopbits";

alignas(uint32_t) const char DmxSerialParamsConst::SPI_SPEED_HZ[]  = "spi_speed_hz";
alignas(uint32_t) const char DmxSerialParamsConst::SPI_MODE[]  = "spi_mode";

alignas(uint32_t) const char DmxSerialParamsConst::I2C_ADDRESS[]  = "i2c_address";
alignas(uint32_t) const char DmxSerialParamsConst::I2C_SPEED_MODE[]  = "i2c_speed_mode";


