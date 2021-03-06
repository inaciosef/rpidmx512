/**
 * @file bw_spi_dimmer.c
 *
 */
/* Copyright (C) 2016-2019 by Arjan van Vught mailto:info@orangepi-dmx.nl
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
#include <stdbool.h>

#include "bob.h"

#include "bw.h"
#include "bw_spi.h"
#include "bw_dimmer.h"
#include "bw_spi_dimmer.h"

bool bw_spi_dimmer_start(device_info_t *device_info) {

	if (device_info->slave_address == 0) {
		device_info->slave_address = BW_DIMMER_DEFAULT_SLAVE_ADDRESS;
	}

	if (device_info->speed_hz == 0) {
		device_info->speed_hz = BW_DIMMER_SPI_SPEED_DEFAULT_HZ;
	} else if (device_info->speed_hz > (uint32_t) BW_DIMMER_SPI_SPEED_MAX_HZ) {
		device_info->speed_hz = BW_DIMMER_SPI_SPEED_MAX_HZ;
	}

	if (device_info->chip_select >= SPI_CS2) {
		device_info->chip_select = SPI_CS2;
		bcm2835_aux_spi_begin();
		device_info->internal.clk_div = bcm2835_aux_spi_CalcClockDivider(device_info->speed_hz);
	} else {
		FUNC_PREFIX(spi_begin());;
	}

	char id[BW_ID_STRING_LENGTH+1];
	bw_spi_read_id(device_info, id);

	if (memcmp(id, "spi_dimmer", 10) == 0) {
		return true;
	}

	return false;
}

void bw_spi_dimmer_output(const device_info_t *device_info, uint8_t value) {
	char cmd[3];

	cmd[0] = (char) device_info->slave_address;
	cmd[1] = (char) BW_PORT_WRITE_DIMMER;
	cmd[2] = (char) value;

	if (device_info->chip_select == SPI_CS2) {
		bcm2835_aux_spi_setClockDivider(device_info->internal.clk_div);
		bcm2835_aux_spi_writenb(cmd, sizeof(cmd) / sizeof(cmd[0]));
	} else {
		FUNC_PREFIX(spi_set_speed_hz(device_info->speed_hz));
		FUNC_PREFIX(spi_chipSelect(device_info->chip_select));
		FUNC_PREFIX(spi_setDataMode(SPI_MODE0));
		FUNC_PREFIX(spi_writenb(cmd, sizeof(cmd) / sizeof(cmd[0])));
	}
}
