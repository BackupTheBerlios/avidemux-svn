/***************************************************************************
                            mpeg2EncoderOptions.h

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

#ifndef mpeg2EncoderOptions_h
#define mpeg2EncoderOptions_h

#include <vector>
#include "PluginOptions.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

#define MPEG2_DEFAULT_ENCODE_MODE ADM_VIDENC_MODE_CQP
#define MPEG2_DEFAULT_ENCODE_MODE_PARAMETER 4

typedef enum
{
	MPEG2_INTERLACED_NONE,
	MPEG2_INTERLACED_BFF,
	MPEG2_INTERLACED_TFF
} Mpeg2InterlacedMode;

typedef enum
{
	MPEG2_MATRIX_DEFAULT,
	MPEG2_MATRIX_TMPGENC,
	MPEG2_MATRIX_ANIME,
	MPEG2_MATRIX_KVCD
} Mpeg2MatrixMode;

class Mpeg2EncoderOptions : public PluginOptions
{
protected:
	unsigned int _minBitrate, _maxBitrate, _bufferSize, _gopSize;
	bool _xvidRateControl, _widescreen;
	Mpeg2InterlacedMode _interlaced;
	Mpeg2MatrixMode _matrix;

	void addOptionsToXml(xmlNodePtr xmlNodeRoot);
	void parseOptions(xmlNode *node);

public:
	Mpeg2EncoderOptions(void);
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

	Mpeg2InterlacedMode getInterlaced(void);
	void setInterlaced(Mpeg2InterlacedMode interlaced);

	Mpeg2MatrixMode getMatrix(void);
	void setMatrix(Mpeg2MatrixMode matrix);

	unsigned int getGopSize(void);
	void setGopSize(unsigned int gopSize);
};

#endif	// mpeg2EncoderOptions_h
