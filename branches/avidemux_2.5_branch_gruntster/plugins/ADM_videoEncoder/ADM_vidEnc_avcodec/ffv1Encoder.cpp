/***************************************************************************
                               ffv1Encoder.cpp

    begin                : Thu Jul 2 2009
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

#include "ADM_inttype.h"
#include "ffv1Encoder.h"

FFV1Encoder::FFV1Encoder(void)
{
	init(CODEC_ID_FFV1, ADM_CSP_YV12);
}

const char* FFV1Encoder::getEncoderType(void)
{
	return "FFV1";
}

const char* FFV1Encoder::getEncoderDescription(void)
{
	return "FFV1 video encoder plugin for Avidemux (c) Mean/Gruntster";
}

const char* FFV1Encoder::getFourCC(void)
{
	return "FFV1";
}

const char* FFV1Encoder::getEncoderGuid(void)
{
	return "4828D06D-B555-4d72-94EC-34F04F97E501";
}

int FFV1Encoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
{
	if (encodeOptions)
	{
		encodeOptions->encodeMode = ADM_VIDENC_MODE_CQP;
		encodeOptions->encodeModeParameter = 1000;
	}

	return 0;
}

int FFV1Encoder::setOptions(vidEncOptions *encodeOptions, const char *pluginOptions)
{
	return 0;
}
