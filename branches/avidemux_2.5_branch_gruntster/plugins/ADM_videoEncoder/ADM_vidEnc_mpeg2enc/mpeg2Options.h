 /***************************************************************************
                               mpeg2EncOptions.h

    begin                : Thu Dec 31 2009
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

#ifndef mpeg2Options_h
#define mpeg2Options_h

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
	MPEG2_MATRIX_KVCD
} Mpeg2MatrixMode;

typedef enum
{
	MPEG2_STREAMTYPE_DVD,
	MPEG2_STREAMTYPE_SVCD
} Mpeg2StreamTypeMode;

class Mpeg2Options : public PluginOptions
{
protected:
	unsigned int _maxBitrate, _fileSplit;
	bool _widescreen;
	Mpeg2InterlacedMode _interlaced;
	Mpeg2MatrixMode _matrix;
	Mpeg2StreamTypeMode _streamType;

	void addOptionsToXml(xmlNodePtr xmlNodeRoot);
	void parseOptions(xmlNode *node);

public:
	Mpeg2Options(void);
	void reset(void);

	unsigned int getMaxBitrate(void);
	void setMaxBitrate(unsigned int maxBitrate);

	unsigned int getFileSplit(void);
	void setFileSplit(unsigned int mb);

	bool getWidescreen(void);
	void setWidescreen(bool widescreen);

	Mpeg2InterlacedMode getInterlaced(void);
	void setInterlaced(Mpeg2InterlacedMode interlaced);

	Mpeg2MatrixMode getMatrix(void);
	void setMatrix(Mpeg2MatrixMode matrix);

	Mpeg2StreamTypeMode getStreamType(void);
	void setStreamType(Mpeg2StreamTypeMode streamType);
};

#endif	// mpeg2Options_h
