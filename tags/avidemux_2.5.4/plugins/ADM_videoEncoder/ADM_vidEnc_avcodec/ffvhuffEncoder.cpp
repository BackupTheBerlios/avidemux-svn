/***************************************************************************
                             ffvhuffEncoder.cpp

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
#include "ffvhuffEncoder.h"

FFVHuffEncoder::FFVHuffEncoder(void)
{
	init(CODEC_ID_FFVHUFF, ADM_CSP_YV12);
}

const char* FFVHuffEncoder::getEncoderType(void)
{
	return "FFVHuff";
}

const char* FFVHuffEncoder::getEncoderDescription(void)
{
	return "FFVHuff video encoder plugin for Avidemux (c) Mean/Gruntster";
}

const char* FFVHuffEncoder::getFourCC(void)
{
	return "FFVH";
}

const char* FFVHuffEncoder::getEncoderGuid(void)
{
	return "E5D8EAC6-71C1-4f3e-A975-B655232271FB";
}

int FFVHuffEncoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
{
	if (encodeOptions)
	{
		encodeOptions->encodeMode = ADM_VIDENC_MODE_CQP;
		encodeOptions->encodeModeParameter = 1000;
	}

	return 0;
}

int FFVHuffEncoder::setOptions(vidEncOptions *encodeOptions, const char *pluginOptions)
{
	return 0;
}
