/***************************************************************************
                             huffyuvEncoder.cpp

    begin                : Mon Jun 29 2009
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
#include "huffyuvEncoder.h"

HuffyuvEncoder::HuffyuvEncoder(void)
{
	init(CODEC_ID_HUFFYUV, ADM_CSP_I422);
}

const char* HuffyuvEncoder::getEncoderType(void)
{
	return "Huffyuv";
}

const char* HuffyuvEncoder::getEncoderDescription(void)
{
	return "Huffyuv video encoder plugin for Avidemux (c) Mean/Gruntster";
}

const char* HuffyuvEncoder::getFourCC(void)
{
	return "HFYU";
}

const char* HuffyuvEncoder::getEncoderGuid(void)
{
	return "970CB80B-5713-445c-A187-5C6F4A76FA76";
}

int HuffyuvEncoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
{
	if (encodeOptions)
	{
		encodeOptions->encodeMode = ADM_VIDENC_MODE_CQP;
		encodeOptions->encodeModeParameter = 1000;
	}

	return 0;
}

int HuffyuvEncoder::setOptions(vidEncOptions *encodeOptions, const char *pluginOptions)
{
	return 0;
}
