 /***************************************************************************
                               mpeg1Encoder.cpp

    begin                : Mon Apr 5 2010
    copyright            : (C) 2010 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <math.h>
#include <libxml/tree.h>

#include "ADM_inttype.h"
#include "mpeg1Encoder.h"
#include "format_codes.h"

extern int _uiType;
static bool changedConfig(const char* fileName, ConfigMenuType configType);

#ifdef __WIN32
extern void convertPathToAnsi(const char *path, char **ansiPath);
#endif

Mpeg1Encoder::Mpeg1Encoder(void)
{
	_passCount = 1;
	_encodeOptions.encodeMode == ADM_VIDENC_MODE_CBR;
}

const char* Mpeg1Encoder::getEncoderType(void)
{
	return "MPEG-1";
}

const char* Mpeg1Encoder::getEncoderDescription(void)
{
	return "MPEG-1 video encoder plugin for Avidemux (c) Mean/Gruntster";
}

const char* Mpeg1Encoder::getFourCC(void)
{
	return "mpg1";
}

const char* Mpeg1Encoder::getEncoderGuid(void)
{
	return "056FE919-C1D3-4450-A812-A767EAB07786";
}

int Mpeg1Encoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
{
	if (encodeOptions)
	{
		encodeOptions->encodeMode = ADM_VIDENC_MODE_CBR;
		encodeOptions->encodeModeParameter = 1000;
	}

	return 0;
}

int Mpeg1Encoder::setOptions(vidEncOptions *encodeOptions, const char *pluginOptions)
{
	return ADM_VIDENC_ERR_SUCCESS;
}

int Mpeg1Encoder::initParameters(int *encodeModeParameter, int *maxBitrate, int *vbv)
{
	_param.fieldenc = 0;
	_param.format = MPEG_FORMAT_VCD;
	_param.aspect_ratio = 2;
	_param.min_GOP_size = _param.max_GOP_size = 18;
	*encodeModeParameter = 1000;
	*maxBitrate = 9216000;
	*vbv = 0;
}
