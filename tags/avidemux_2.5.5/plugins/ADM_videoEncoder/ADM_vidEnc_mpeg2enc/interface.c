/***************************************************************************
                                 interface.c

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
		*encoderIds = mpeg2encEncoder_getPointers(uiType, &count);

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
	return mpeg2encEncoder_getEncoderName(encoderId);
}

const char* vidEncGetEncoderType(int encoderId)
{
	return mpeg2encEncoder_getEncoderType(encoderId);
}

const char* vidEncGetEncoderDescription(int encoderId)
{
	return mpeg2encEncoder_getEncoderDescription(encoderId);
}

const char* vidEncGetFourCC(int encoderId)
{
	return mpeg2encEncoder_getFourCC(encoderId);
}

int vidEncGetEncoderRequirements(int encoderId)
{
	return mpeg2encEncoder_getEncoderRequirements(encoderId);
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
	return mpeg2encEncoder_getEncoderGuid(encoderId);
}

int vidEncIsConfigurable(int encoderId)
{
	return mpeg2encEncoder_isConfigurable(encoderId);
}

int vidEncConfigure(int encoderId, vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	return mpeg2encEncoder_configure(encoderId, configParameters, properties);
}

int vidEncGetOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
{
	return mpeg2encEncoder_getOptions(encoderId, encodeOptions, pluginOptions, bufferSize);
}

int vidEncSetOptions(int encoderId, vidEncOptions *encodeOptions, const char *pluginOptions)
{
	return mpeg2encEncoder_setOptions(encoderId, encodeOptions, pluginOptions);
}

int vidEncGetPassCount(int encoderId)
{
	return mpeg2encEncoder_getPassCount(encoderId);
}

int vidEncGetCurrentPass(int encoderId)
{
	return mpeg2encEncoder_getCurrentPass(encoderId);
}

int vidEncOpen(int encoderId, vidEncVideoProperties *properties)
{
	return mpeg2encEncoder_open(encoderId, properties);
}

int vidEncBeginPass(int encoderId, vidEncPassParameters *passParameters)
{
	return mpeg2encEncoder_beginPass(encoderId, passParameters);
}

int vidEncEncodeFrame(int encoderId, vidEncEncodeParameters *encodeParams)
{
	return mpeg2encEncoder_encodeFrame(encoderId, encodeParams);
}

int vidEncFinishPass(int encoderId)
{
	return mpeg2encEncoder_finishPass(encoderId);
}

int vidEncClose(int encoderId)
{
	return mpeg2encEncoder_close(encoderId);
}
