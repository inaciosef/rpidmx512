/**
 * @file tftpfileserver.cpp
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

#if (__GNUC__ > 8) || defined(__clang__)
 #pragma GCC diagnostic ignored "-Wunused-private-field"
#endif

#include "tftpfileserver.h"

#include "debug.h"

TFTPFileServer::TFTPFileServer(uint8_t *pBuffer, uint32_t nSize):
		m_pBuffer(pBuffer),
		m_nSize(nSize),
		m_nFileSize(0),
		m_bDone(false)
{
	DEBUG_ENTRY
	DEBUG_EXIT
}

TFTPFileServer::~TFTPFileServer(void) {
	DEBUG_ENTRY
	DEBUG_EXIT
}

void TFTPFileServer::Exit(void) {
	DEBUG_ENTRY
	DEBUG_EXIT
}


bool TFTPFileServer::FileOpen(const char* pFileName, TTFTPMode tMode) {
	DEBUG_ENTRY
	DEBUG_EXIT
	return false;
}

bool TFTPFileServer::FileCreate(const char* pFileName, TTFTPMode tMode) {
	DEBUG_ENTRY
	DEBUG_EXIT
	return false;
}

bool TFTPFileServer::FileClose(void) {
	DEBUG_ENTRY
	DEBUG_EXIT
	return false;
}

int TFTPFileServer::FileRead(void* pBuffer, unsigned nCount, unsigned nBlockNumber) {
	DEBUG_ENTRY
	DEBUG_EXIT
	return -1;
}

int TFTPFileServer::FileWrite(const void *pBuffer, unsigned nCount, unsigned nBlockNumber) {
	DEBUG_ENTRY
	DEBUG_EXIT
	return -1;
}
