/**
 * @file artnetcontroller.h
 *
 */
/**
 * Art-Net Designed by and Copyright Artistic Licence Holdings Ltd.
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
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "artnetcontroller.h"
#include "artnet.h"
#include "artnetconst.h"

#include "hardware.h"
#include "network.h"

#include "debug.h"

#define ARTNET_MIN_HEADER_SIZE		12

static uint16_t s_ActiveUniverses[ARTNET_POLL_TABLE_SIZE_UNIVERSES] __attribute__ ((aligned (4)));

ArtNetController *ArtNetController::s_pThis = 0;

ArtNetController::ArtNetController(void):
	m_bSynchronization(true),
	m_bUnicast(true),
	m_nHandle(-1),
	m_pArtNetTrigger(0),
	m_nLastPollMillis(0),
	m_bDoTableCleanup(true),
	m_bDmxHandled(false),
	m_nActiveUniverses(0),
	m_nMaster(DMX_MAX_VALUE)
{
	DEBUG_ENTRY

	assert(s_pThis == 0);
	s_pThis = this;

	m_pArtNetPacket = new struct TArtNetPacket;
	assert(m_pArtNetPacket != 0);

	memset(&m_ArtNetPoll, 0, sizeof(struct TArtPoll));
	memcpy(&m_ArtNetPoll, NODE_ID, 8);
	m_ArtNetPoll.OpCode = OP_POLL;
	m_ArtNetPoll.ProtVerLo = ARTNET_PROTOCOL_REVISION;
	m_ArtNetPoll.TalkToMe = TTM_SEND_ARTP_ON_CHANGE;

	m_pArtDmx = new struct TArtDmx;
	assert(m_pArtDmx != 0);

	memset(m_pArtDmx, 0, sizeof(struct TArtDmx));
	memcpy(m_pArtDmx, NODE_ID, 8);
	m_pArtDmx->OpCode = OP_DMX;
	m_pArtDmx->ProtVerLo = ARTNET_PROTOCOL_REVISION;

	m_pArtSync = new struct TArtSync;
	assert(m_pArtSync != 0);

	memset(m_pArtSync, 0, sizeof(struct TArtSync));
	memcpy(m_pArtSync, NODE_ID, 8);
	m_pArtSync->OpCode = OP_SYNC;
	m_pArtSync->ProtVerLo = ARTNET_PROTOCOL_REVISION;

	m_tArtNetController.Oem[0] = ArtNetConst::OEM_ID[0];
	m_tArtNetController.Oem[1] = ArtNetConst::OEM_ID[1];

	ActiveUniversesClear();

	DEBUG_EXIT
}

ArtNetController::~ArtNetController(void) {
	DEBUG_ENTRY

	delete m_pArtNetPacket;
	m_pArtNetPacket = 0;

	DEBUG_EXIT
}

void ArtNetController::Start(void) {
	DEBUG_ENTRY

	m_tArtNetController.nIPAddressLocal = Network::Get()->GetIp();
	m_tArtNetController.nIPAddressBroadcast = Network::Get()->GetBroadcastIp();

	m_nHandle = Network::Get()->Begin(ARTNET_UDP_PORT);
	assert(m_nHandle != -1);

	Network::Get()->SendTo(m_nHandle, &m_ArtNetPoll, sizeof(struct TArtPoll), m_tArtNetController.nIPAddressBroadcast, ARTNET_UDP_PORT);

	DEBUG_EXIT
}

void ArtNetController::Stop(void) {
	DEBUG_ENTRY

	//FIXME ArtNetController::Stop

	DEBUG_EXIT
}

void ArtNetController::HandleDmxOut(uint16_t nUniverse, const uint8_t *pDmxData, uint16_t nLength, uint8_t nPortIndex) {
	DEBUG_ENTRY

	ActiveUniversesAdd(nUniverse);

	m_pArtDmx->Physical = nPortIndex;
	m_pArtDmx->PortAddress = nUniverse;
	m_pArtDmx->LengthHi = (nLength & 0xFF00) >> 8;
	m_pArtDmx->Length = (nLength & 0xFF);

	// The sequence number is used to ensure that ArtDmx packets are used in the correct order.
	// This field is incremented in the range 0x01 to 0xff to allow the receiving node to resequence packets.
	m_pArtDmx->Sequence++;

	if (m_pArtDmx->Sequence == 0) {
		m_pArtDmx->Sequence = 1;
	}

	if (__builtin_expect((m_nMaster == DMX_MAX_VALUE), 1)) {
		memcpy(m_pArtDmx->Data, pDmxData, nLength);
	} else if (m_nMaster == 0) {
		memset(m_pArtDmx->Data, 0, nLength);
	} else {
		for (uint32_t i = 0; i < nLength; i++) {
			m_pArtDmx->Data[i] = (m_nMaster * static_cast<uint32_t>(pDmxData[i])) / DMX_MAX_VALUE;
		}
	}

	uint32_t nCount = 0;
	struct TArtNetPollTableUniverses *IpAddresses = const_cast<struct TArtNetPollTableUniverses*>(GetIpAddress(nUniverse));

	if (m_bUnicast) {
		if (IpAddresses != 0) {
			nCount = IpAddresses->nCount;
		} else {
			DEBUG_EXIT
			return;
		}
	}

	// If the number of universe subscribers exceeds 40 for a given universe, the transmitting device may broadcast.

	if (m_bUnicast && (nCount <= 40)) {
		for (uint32_t nIndex = 0; nIndex < nCount; nIndex++) {
			Network::Get()->SendTo(m_nHandle, m_pArtDmx, sizeof(struct TArtDmx), IpAddresses->pIpAddresses[nIndex], ARTNET_UDP_PORT);
		}

		m_bDmxHandled = true;

		DEBUG_EXIT
		return;
	}

	if (!m_bUnicast || (nCount > 40)) {
		Network::Get()->SendTo(m_nHandle, m_pArtDmx, sizeof(struct TArtDmx), m_tArtNetController.nIPAddressBroadcast, ARTNET_UDP_PORT);

		m_bDmxHandled = true;
	}

	DEBUG_EXIT
}

void ArtNetController::HandleSync(void) {
	if (m_bSynchronization && m_bDmxHandled) {
		m_bDmxHandled = false;
		Network::Get()->SendTo(m_nHandle, m_pArtSync, sizeof(struct TArtSync), m_tArtNetController.nIPAddressBroadcast, ARTNET_UDP_PORT);
	}
}

void ArtNetController::HandleBlackout(void) {
	m_pArtDmx->LengthHi = (512 & 0xFF00) >> 8;
	m_pArtDmx->Length = (512 & 0xFF);

	memset(m_pArtDmx->Data, 0, 512);

	for (uint32_t nIndex = 0; nIndex < m_nActiveUniverses; nIndex++) {
		m_pArtDmx->PortAddress = s_ActiveUniverses[nIndex];

		uint32_t nCount = 0;
		const struct TArtNetPollTableUniverses *IpAddresses = GetIpAddress(s_ActiveUniverses[nIndex]);

		if (m_bUnicast) {
			if (IpAddresses != 0) {
				nCount = IpAddresses->nCount;
			} else {
				continue;
			}
		}

		if (m_bUnicast && (nCount <= 40)) {
			// The sequence number is used to ensure that ArtDmx packets are used in the correct order.
			// This field is incremented in the range 0x01 to 0xff to allow the receiving node to resequence packets.
			m_pArtDmx->Sequence++;

			if (m_pArtDmx->Sequence == 0) {
				m_pArtDmx->Sequence = 1;
			}

			for (uint32_t nIndex = 0; nIndex < nCount; nIndex++) {
				Network::Get()->SendTo(m_nHandle, m_pArtDmx, sizeof(struct TArtDmx), IpAddresses->pIpAddresses[nIndex], ARTNET_UDP_PORT);
			}

			continue;
		}

		if (!m_bUnicast || (nCount > 40)) {
			// The sequence number is used to ensure that ArtDmx packets are used in the correct order.
			// This field is incremented in the range 0x01 to 0xff to allow the receiving node to resequence packets.
			m_pArtDmx->Sequence++;

			if (m_pArtDmx->Sequence == 0) {
				m_pArtDmx->Sequence = 1;
			}

			Network::Get()->SendTo(m_nHandle, m_pArtDmx, sizeof(struct TArtDmx), m_tArtNetController.nIPAddressBroadcast, ARTNET_UDP_PORT);
		}

	}

	m_bDmxHandled = true;
	HandleSync();
}

void ArtNetController::HandleTrigger(void) {
	DEBUG_ENTRY
	const TArtTrigger *pArtTrigger = &m_pArtNetPacket->ArtPacket.ArtTrigger;

	if ((pArtTrigger->OemCodeHi == 0xFF && pArtTrigger->OemCodeLo == 0xFF) || (pArtTrigger->OemCodeHi == m_tArtNetController.Oem[0] && pArtTrigger->OemCodeLo == m_tArtNetController.Oem[1])) {
		DEBUG_PRINTF("Key=%d, SubKey=%d, Data[0]=%d", pArtTrigger->Key, pArtTrigger->SubKey, pArtTrigger->Data[0]);

		m_pArtNetTrigger->Handler(reinterpret_cast<const struct TArtNetTrigger*>(&pArtTrigger->Key));
	}

	DEBUG_EXIT
}

void ArtNetController::HandlePoll(void) {
	const uint32_t nCurrentMillis = Hardware::Get()->Millis();

	if (__builtin_expect((nCurrentMillis - m_nLastPollMillis > ARTNET_POLL_INTERVAL_MILLIS), 0)) {
#ifndef NDEBUG
		time_t ltime = time(0);
		struct tm tm = *localtime(&ltime);

		DEBUG_PRINTF("SendPoll - %.2d:%.2d:%.2d", tm.tm_hour, tm.tm_min, tm.tm_sec);
#endif

		Network::Get()->SendTo(m_nHandle, &m_ArtNetPoll, sizeof(struct TArtPoll), m_tArtNetController.nIPAddressBroadcast, ARTNET_UDP_PORT);
		m_nLastPollMillis= nCurrentMillis;

#ifndef NDEBUG
		Dump();
		DumpTableUniverses();
#endif
	}

	if (m_bDoTableCleanup && (__builtin_expect((nCurrentMillis - m_nLastPollMillis > ARTNET_POLL_INTERVAL_MILLIS/4), 0))) {
		Clean();
	}
}

void ArtNetController::HandlePollReply(void) {
	DEBUG_ENTRY

#ifndef NDEBUG
	time_t ltime = time(0);
	struct tm tm = *localtime(&ltime);

	printf("ArtPollReply - %.2d:%.2d:%.2d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
#endif

	Add(&m_pArtNetPacket->ArtPacket.ArtPollReply);

	DEBUG_EXIT
}

void ArtNetController::Run(void) {
	char *pArtPacket = reinterpret_cast<char*>(&m_pArtNetPacket->ArtPacket);
	uint16_t nForeignPort;

	if (m_bUnicast) {
		HandlePoll();
	}

	const int nBytesReceived = Network::Get()->RecvFrom(m_nHandle, pArtPacket, sizeof(struct TArtNetPacket), &m_pArtNetPacket->IPAddressFrom, &nForeignPort) ;

	if (__builtin_expect((nBytesReceived < ARTNET_MIN_HEADER_SIZE), 1)) {
		return;
	}

	if (memcmp(pArtPacket, "Art-Net\0", 8) != 0) {
		return;
	}

	const TOpCodes OpCode = static_cast<TOpCodes>(((pArtPacket[9] << 8)) + pArtPacket[8]);

	switch (OpCode) {
	case OP_POLLREPLY:
		HandlePollReply();
		break;
	case OP_TRIGGER:
		if (m_pArtNetTrigger != 0) {
			HandleTrigger();
		}
		break;
	default:
		break;
	}
}

void ArtNetController::ActiveUniversesClear(void) {
	memset(s_ActiveUniverses, 0, sizeof(s_ActiveUniverses));
	m_nActiveUniverses = 0;
}

void ArtNetController::ActiveUniversesAdd(uint16_t nUniverse) {
	DEBUG_ENTRY
	DEBUG_PRINTF("nUniverse=%d", static_cast<int>(nUniverse));

	int32_t nLow = 0;
	int32_t nMid = 0;
	int32_t nHigh = m_nActiveUniverses;

	if (m_nActiveUniverses == (sizeof(s_ActiveUniverses) / sizeof(s_ActiveUniverses[0]))) {
		assert(0);
		return;
	}

	while (nLow <= nHigh) {

		nMid = nLow + ((nHigh - nLow) / 2);
		const uint32_t nMidValue = s_ActiveUniverses[nMid];

		if (nMidValue < nUniverse) {
			nLow = nMid + 1;
		} else if (nMidValue > nUniverse) {
			nHigh = nMid - 1;
		} else {
			DEBUG_EXIT
			return;
		}
	}

	DEBUG_PRINTF("nLow=%d, nMid=%d, nHigh=%d", nLow, nMid, nHigh);

	if ((nHigh != -1) && (m_nActiveUniverses != static_cast<uint32_t>(nHigh))) {

		uint16_t *p16 = reinterpret_cast<uint16_t*>(s_ActiveUniverses);

		assert(nLow >= 0);

		//TODO Can we make i unsigned ?
		for (uint32_t i = m_nActiveUniverses; i >= static_cast<uint32_t>(nLow); i--) {
			p16[i + 1] = p16[i];
		}

		s_ActiveUniverses[nLow] = nUniverse;

		DEBUG_PRINTF(">m< nUniverse=%u, nLow=%d", nUniverse, nLow);
	} else {
		s_ActiveUniverses[m_nActiveUniverses] = nUniverse;

		DEBUG_PRINTF(">a< nUniverse=%u, nMid=%d", nUniverse, nMid);
	}

	m_nActiveUniverses++;

	DEBUG_EXIT
}

void ArtNetController::Print(void) {
	printf("Art-Net Controller\n");
	printf(" Max Node's    : %u\n", ARTNET_POLL_TABLE_SIZE_ENRIES);
	printf(" Max Universes : %u\n", ARTNET_POLL_TABLE_SIZE_UNIVERSES);
	if (!m_bUnicast) {
		puts(" Unicast is disabled");
	}
	if (!m_bSynchronization) {
		puts(" Synchronization is disabled");
	}
}
