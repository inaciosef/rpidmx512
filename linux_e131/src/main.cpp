/**
 * @file main.cpp
 *
 */
/* Copyright (C) 2017-2020 by Arjan van Vught mailto:info@raspberrypi-dmx.nl
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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "hardware.h"
#include "networklinux.h"
#include "ledblink.h"

#include "e131bridge.h"
#include "e131params.h"
#include "storee131.h"

#include "dmxmonitor.h"
#include "dmxmonitorparams.h"
#include "storemonitor.h"

#include "spiflashstore.h"

#include "remoteconfig.h"
#include "remoteconfigparams.h"
#include "storeremoteconfig.h"

#include "firmwareversion.h"
#include "software_version.h"

int main(int argc, char **argv) {
	Hardware hw;
	NetworkLinux nw;
	LedBlink lb;
	FirmwareVersion fw(SOFTWARE_VERSION, __DATE__, __TIME__);

	if (argc < 2) {
		printf("Usage: %s ip_address|interface_name\n", argv[0]);
		return -1;
	}

	fw.Print();

	puts("sACN E1.31 Real-time DMX Monitor {4 Universes}");

	if (nw.Init(argv[1]) < 0) {
		fprintf(stderr, "Not able to start the network\n");
		return -1;
	}

	SpiFlashStore spiFlashStore;

	E131Params e131Params(new StoreE131);
	E131Bridge bridge;

	if (e131Params.Load()) {
		e131Params.Dump();
		e131Params.Set(&bridge);
	}

	if (fopen("direct.update", "r") != NULL) {
		bridge.SetDirectUpdate(true);
	}

	DMXMonitor monitor;
	DMXMonitorParams monitorParams(new StoreMonitor);

	if (monitorParams.Load()) {
		monitorParams.Dump();
		monitorParams.Set(&monitor);
	}

	bridge.SetOutput(&monitor);

	uint16_t nUniverse;
	bool bIsSetIndividual = false;
	bool bIsSet;

	for (uint32_t i = 0; i < E131_PARAMS_MAX_PORTS; i++) {
		nUniverse = e131Params.GetUniverse(i, bIsSet);

		if (bIsSet) {
			bridge.SetUniverse(i, E131_OUTPUT_PORT, nUniverse);
			bIsSetIndividual = true;
		}
	}

	if (!bIsSetIndividual) {
		bridge.SetUniverse(0, E131_OUTPUT_PORT, 0 + e131Params.GetUniverse());
		bridge.SetUniverse(1, E131_OUTPUT_PORT, 1 + e131Params.GetUniverse());
		bridge.SetUniverse(2, E131_OUTPUT_PORT, 2 + e131Params.GetUniverse());
		bridge.SetUniverse(3, E131_OUTPUT_PORT, 3 + e131Params.GetUniverse());
	}

	nw.Print();
	bridge.Print();

	RemoteConfig remoteConfig(REMOTE_CONFIG_E131, REMOTE_CONFIG_MODE_MONITOR, bridge.GetActiveOutputPorts());

	StoreRemoteConfig storeRemoteConfig;
	RemoteConfigParams remoteConfigParams(&storeRemoteConfig);

	if(remoteConfigParams.Load()) {
		remoteConfigParams.Set(&remoteConfig);
		remoteConfigParams.Dump();
	}

	while (spiFlashStore.Flash())
		;

	bridge.Start();

	for (;;) {
		bridge.Run();
		remoteConfig.Run();
		spiFlashStore.Flash();
	}

	return 0;
}
