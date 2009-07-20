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
#include "../common/PluginOptions.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

#define DEFAULT_ENCODE_MODE ADM_VIDENC_MODE_CQP
#define DEFAULT_ENCODE_MODE_PARAMETER 4

typedef enum
{
	INTERLACED_NONE,
	INTERLACED_BFF,
	INTERLACED_TFF
} InterlacedMode;

typedef enum
{
	MATRIX_DEFAULT,
	MATRIX_TMPGENC,
	MATRIX_ANIME,
	MATRIX_KVCD
} MatrixMode;

class Mpeg1EncoderOptions : public PluginOptions
{
protected:
	unsigned int _minBitrate, _maxBitrate, _bufferSize, _gopSize;
	bool _xvidRateControl, _widescreen;
	InterlacedMode _interlaced;
	MatrixMode _matrix;

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

	InterlacedMode getInterlaced(void);
	void setInterlaced(InterlacedMode interlaced);

	MatrixMode getMatrix(void);
	void setMatrix(MatrixMode matrix);

	unsigned int getGopSize(void);
	void setGopSize(unsigned int gopSize);
};

#endif	// mpeg1EncoderOptions_h
