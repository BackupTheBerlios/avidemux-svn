/***************************************************************************
                            mpeg1EncoderOptions.h

    begin                : Sat Jul 4 2009
    copyright            : (C) 2009 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef mpeg1EncoderOptions_h
#define mpeg1EncoderOptions_h

#include <vector>
#include "PluginOptions.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

#define MPEG1_DEFAULT_ENCODE_MODE ADM_VIDENC_MODE_CQP
#define MPEG1_DEFAULT_ENCODE_MODE_PARAMETER 4

typedef enum
{
	MPEG1_INTERLACED_NONE,
	MPEG1_INTERLACED_BFF,
	MPEG1_INTERLACED_TFF
} Mpeg1InterlacedMode;

typedef enum
{
	MPEG1_MATRIX_DEFAULT,
	MPEG1_MATRIX_TMPGENC,
	MPEG1_MATRIX_ANIME,
	MPEG1_MATRIX_KVCD
} Mpeg1MatrixMode;

class Mpeg1EncoderOptions : public PluginOptions
{
protected:
	unsigned int _minBitrate, _maxBitrate, _bufferSize, _gopSize;
	bool _xvidRateControl, _widescreen;
	Mpeg1InterlacedMode _interlaced;
	Mpeg1MatrixMode _matrix;

	void addOptionsToXml(xmlNodePtr xmlNodeRoot);
	void parseOptions(xmlNode *node);

public:
	Mpeg1EncoderOptions(void);
	void reset(void);

	unsigned int getMinBitrate(void);
	void setMinBitrate(unsigned int minBitrate);

	unsigned int getMaxBitrate(void);
	void setMaxBitrate(unsigned int maxBitrate);

	bool getXvidRateControl(void);
	void setXvidRateControl(bool xvidRateControl);

	unsigned int getBufferSize(void);
	void setBufferSize(unsigned int bufferSize);

	bool getWidescreen(void);
	void setWidescreen(bool widescreen);

	Mpeg1InterlacedMode getInterlaced(void);
	void setInterlaced(Mpeg1InterlacedMode interlaced);

	Mpeg1MatrixMode getMatrix(void);
	void setMatrix(Mpeg1MatrixMode matrix);

	unsigned int getGopSize(void);
	void setGopSize(unsigned int gopSize);
};

#endif	// mpeg1EncoderOptions_h
