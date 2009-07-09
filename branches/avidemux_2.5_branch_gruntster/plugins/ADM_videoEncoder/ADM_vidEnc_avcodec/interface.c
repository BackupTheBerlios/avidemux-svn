/***************************************************************************
                                 interface.c

    begin                : Thu Apr 23 2009
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
#include "ADM_vidEnc_plugin.h"
#include "encoder.h"

int _uiType = 0;

int vidEncGetEncoders(int uiType, int** encoderIds)
{
	if (uiType == ADM_UI_CLI || uiType == ADM_UI_GTK || uiType == ADM_UI_QT4)
	{
		int count = 0;

		_uiType = uiType;
		*encoderIds = avcodecEncoder_getPointers(uiType, &count);

		return count;
	}
	else
	{
		*encoderIds = NULL;
		return 0;
	}
}

const char* vidEncGetEncoderName(int encoderId)
{
	return avcodecEncoder_getEncoderName(encoderId);
}

const char* vidEncGetEncoderType(int encoderId)
{
	return avcodecEncoder_getEncoderType(encoderId);
}

const char* vidEncGetEncoderDescription(int encoderId)
{
	return avcodecEncoder_getEncoderDescription(encoderId);
}

const char* vidEncGetFourCC(int encoderId)
{
	return avcodecEncoder_getFourCC(encoderId);
}

int vidEncGetEncoderRequirements(int encoderId)
{
	return avcodecEncoder_getEncoderRequirements(encoderId);
}

int vidEncGetEncoderApiVersion(int encoderId)
{
	return ADM_VIDENC_API_VERSION;
}

void vidEncGetEncoderVersion(int encoderId, int* major, int* minor, int* patch)
{
	*major = 1;
	*minor = 0;
	*patch = 0;
}

const char* vidEncGetEncoderGuid(int encoderId)
{
	return avcodecEncoder_getEncoderGuid(encoderId);
}

int vidEncIsConfigurable(int encoderId)
{
	return avcodecEncoder_isConfigurable(encoderId);
}

int vidEncConfigure(int encoderId, vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	return avcodecEncoder_configure(encoderId, configParameters, properties);
}

int vidEncGetOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
{
	return avcodecEncoder_getOptions(encoderId, encodeOptions, pluginOptions, bufferSize);
}

int vidEncSetOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions)
{
	return avcodecEncoder_setOptions(encoderId, encodeOptions, pluginOptions);
}

int vidEncGetPassCount(int encoderId)
{
	return avcodecEncoder_getPassCount(encoderId);
}

int vidEncGetCurrentPass(int encoderId)
{
	return avcodecEncoder_getCurrentPass(encoderId);
}

int vidEncOpen(int encoderId, vidEncVideoProperties *properties)
{
	return avcodecEncoder_open(encoderId, properties);
}

int vidEncBeginPass(int encoderId, vidEncPassParameters *passParameters)
{
	return avcodecEncoder_beginPass(encoderId, passParameters);
}

int vidEncEncodeFrame(int encoderId, vidEncEncodeParameters *encodeParams)
{
	return avcodecEncoder_encodeFrame(encoderId, encodeParams);
}

int vidEncFinishPass(int encoderId)
{
	return avcodecEncoder_finishPass(encoderId);
}

int vidEncClose(int encoderId)
{
	return avcodecEncoder_close(encoderId);
}
