/**
 * @file ltcdisplayparams.h
 *
 */
/* Copyright (C) 2019-2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#ifndef LTCDISPLAYPARAMS_H_
#define LTCDISPLAYPARAMS_H_

#include <stdint.h>
#include <stdbool.h>

#include "ltcdisplaymax7219.h"
#include "ltcdisplayws28xx.h"

#include "ws28xx.h"
#include "rgbmapping.h"

struct TLtcDisplayParams {
	uint32_t nSetList;
	uint8_t nMax7219Type;
	uint8_t nMax7219Intensity;
	uint8_t nLedType;
	uint8_t nGlobalBrightness;
	uint8_t nRgbMapping;
	uint8_t nWS28xxIntensity;
	uint8_t nWS28xxColonBlinkMode;
	uint32_t aWS28xxColour[LTCDISPLAYWS28XX_COLOUR_INDEX_LAST];
	uint8_t nWS28xxType;
} __attribute__((packed));

enum TLtcDisplayParamsMask {
	LTCDISPLAY_PARAMS_MASK_MAX7219_TYPE = (1 << 0),
	LTCDISPLAY_PARAMS_MASK_MAX7219_INTENSITY = (1 << 1),
	LTCDISPLAY_PARAMS_MASK_LED_TYPE = (1 << 2),
	LTCDISPLAY_PARAMS_MASK_GLOBAL_BRIGHTNESS = (1 << 3),
	LTCDISPLAY_PARAMS_MASK_RGB_MAPPING = (1 << 4),
	LTCDISPLAY_PARAMS_MASK_WS28XX_INTENSITY = (1 << 5),
	LTCDISPLAY_PARAMS_MASK_WS28XX_COLON_BLINK_MODE = (1 << 6),
	LTCDISPLAY_PARAMS_MASK_WS28XX_COLOUR_INDEX = (1 << 7),
	LTCDISPLAY_PARAMS_MASK_WS28XX_TYPE = (1 << 8)
};

class LtcDisplayParamsStore {
public:
	virtual ~LtcDisplayParamsStore(void) {}

	virtual void Update(const struct TLtcDisplayParams *ptLtcDisplayParams)=0;
	virtual void Copy(struct TLtcDisplayParams *ptLtcDisplayParams)=0;
};

class LtcDisplayParams {
public:
	LtcDisplayParams(LtcDisplayParamsStore *pLtcDisplayParamsStore = 0);
	~LtcDisplayParams(void);

	bool Load(void);
	void Load(const char *pBuffer, uint32_t nLength);

	void Builder(const struct TLtcDisplayParams *ptLtcDisplayParams, char *pBuffer, uint32_t nLength, uint32_t &nSize);
	void Save(char *pBuffer, uint32_t nLength, uint32_t &nSize);

	void Set(LtcDisplayWS28xx *pLtcDisplayWS28xx);

	void Dump(void);

	uint8_t GetGlobalBrightness(void) {
		return m_tLtcDisplayParams.nGlobalBrightness;
	}

	TLtcDisplayMax7219Types GetMax7219Type(void) {
		return static_cast<TLtcDisplayMax7219Types>(m_tLtcDisplayParams.nMax7219Type);
	}

	uint8_t GetMax7219Intensity(void) {
		return m_tLtcDisplayParams.nMax7219Intensity;
	}

	TWS28XXType GetLedType(void) {
		return static_cast<TWS28XXType>(m_tLtcDisplayParams.nLedType);
	}

	TLtcDisplayWS28xxTypes GetWS28xxType(void) {
		return static_cast<TLtcDisplayWS28xxTypes>(m_tLtcDisplayParams.nWS28xxType);
	}

    static void staticCallbackFunction(void *p, const char *s);

private:
    void callbackFunction(const char *pLine);
    bool isMaskSet(uint32_t nMask)  {
    	return (m_tLtcDisplayParams.nSetList & nMask) == nMask;
    }

private:
	LtcDisplayParamsStore *m_pLtcDisplayParamsStore;
	struct TLtcDisplayParams m_tLtcDisplayParams;
};

#endif /* LTCDISPLAYPARAMS_H_ */
