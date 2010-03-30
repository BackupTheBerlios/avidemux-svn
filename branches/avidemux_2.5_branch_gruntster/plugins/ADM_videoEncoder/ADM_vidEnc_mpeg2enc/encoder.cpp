/***************************************************************************
                                encoder.cpp

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
#include <libxml/tree.h>

#include "encoder.h"
#include "mpeg2Encoder.h"
#include "ADM_inttype.h"
#include "mpeg2enc.h"

int uiType;

static Mpeg2Encoder mpeg2;

static int encoderIds[] = { 0 };
static Mpeg2encEncoder* encoders[] = {&mpeg2};
static int supportedCsps[] = { ADM_CSP_YV12 };

extern int mpegenc_init(mpeg2parm *incoming,int width, int height, int fps1000);
extern int mpegenc_encode(char *in, char *out, int *size, int *flags, int *quant);
extern int mpegenc_setQuantizer(int q);

extern "C"
{
	void *mpeg2encEncoder_getPointers(int _uiType, int *count)
	{
		uiType = _uiType;
		*count = sizeof(encoderIds) / sizeof(int);

		return &encoderIds;
	}

	const char* mpeg2encEncoder_getEncoderName(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->getEncoderName();
	}

	const char* mpeg2encEncoder_getEncoderType(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->getEncoderType();
	}

	const char* mpeg2encEncoder_getEncoderDescription(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->getEncoderDescription();
	}

	const char* mpeg2encEncoder_getFourCC(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->getFourCC();
	}

	int mpeg2encEncoder_getEncoderRequirements(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->getEncoderRequirements();
	}

	const char* mpeg2encEncoder_getEncoderGuid(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->getEncoderGuid();
	}

	int mpeg2encEncoder_isConfigurable(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->isConfigurable();
	}

	int mpeg2encEncoder_configure(int encoderId, vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->configure(configParameters, properties);
	}

	int mpeg2encEncoder_getOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->getOptions(encodeOptions, pluginOptions, bufferSize);
	}

	int mpeg2encEncoder_setOptions(int encoderId, vidEncOptions *encodeOptions, const char *pluginOptions)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->setOptions(encodeOptions, pluginOptions);
	}

	int mpeg2encEncoder_getPassCount(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->getPassCount();
	}

	int mpeg2encEncoder_getCurrentPass(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->getCurrentPass();
	}

	int mpeg2encEncoder_open(int encoderId, vidEncVideoProperties *properties)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->open(properties);
	}

	int mpeg2encEncoder_beginPass(int encoderId, vidEncPassParameters *passParameters)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->beginPass(passParameters);
	}

	int mpeg2encEncoder_encodeFrame(int encoderId, vidEncEncodeParameters *encodeParams)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->encodeFrame(encodeParams);
	}

	int mpeg2encEncoder_finishPass(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->finishPass();
	}

	int mpeg2encEncoder_close(int encoderId)
	{
		Mpeg2encEncoder *encoder = encoders[encoderId];
		return encoder->close();
	}
}

Mpeg2encEncoder::~Mpeg2encEncoder(void)
{
	close();
}

const char* Mpeg2encEncoder::getEncoderName(void)
{
	return "mpeg2enc";
}

int Mpeg2encEncoder::getEncoderRequirements(void)
{
	return ADM_VIDENC_REQ_NONE;
}

int Mpeg2encEncoder::isConfigurable(void)
{
	return 0;
}

int Mpeg2encEncoder::configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	return 0;
}

int Mpeg2encEncoder::getCurrentPass(void)
{
	return _currentPass;
}

int Mpeg2encEncoder::getPassCount(void)
{
	return _passCount;
}

int Mpeg2encEncoder::open(vidEncVideoProperties *properties)
{
	if (_opened)
		return ADM_VIDENC_ERR_ALREADY_OPEN;

	_opened = true;
	_currentPass = 0;

	_width = properties->width;
	_height = properties->height;

	_fpsNum = properties->fpsNum;
	_fpsDen = properties->fpsDen;

	_frameCount = properties->frameCount;
	_bufferSize = (properties->width * properties->height) + 2 * ((properties->width + 1 >> 1) * (properties->height + 1 >> 1));
	_buffer = new uint8_t[_bufferSize];

	memset(&_param, 0, sizeof(mpeg2parm));
	_param.setDefault();
	_param.searchrad = 16; // speed up

	properties->supportedCspsCount = 1;
	properties->supportedCsps = supportedCsps;

	return ADM_VIDENC_ERR_SUCCESS;
}

int Mpeg2encEncoder::beginPass(vidEncPassParameters *passParameters)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	if (_openPass)
		return ADM_VIDENC_ERR_PASS_ALREADY_OPEN;

	if (_currentPass == _passCount)
		return ADM_VIDENC_ERR_PASS_COUNT_REACHED;

	if (_passCount > 1 && _currentPass == 0 && passParameters->useExistingLogFile)
	{
		_currentPass++;
		return ADM_VIDENC_ERR_PASS_SKIP;
	}

	_openPass = true;
	_currentPass++;

	if (!mpegenc_init(&_param, _width, _height, (_fpsNum * 1000) / _fpsDen))
		return ADM_VIDENC_ERR_FAILED;

	return ADM_VIDENC_ERR_SUCCESS;
}

int Mpeg2encEncoder::encodeFrame(vidEncEncodeParameters *encodeParams)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	int flags, size, qz;

	if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_CQP)
		mpegenc_setQuantizer(_encodeOptions.encodeModeParameter);

	if (!mpegenc_encode((char*)encodeParams->frameData, (char*)_buffer, &size, &flags, &qz))
		return ADM_VIDENC_ERR_FAILED;

	encodeParams->frameType = getFrameType(flags);
	encodeParams->encodedDataSize = size;
	encodeParams->encodedData = _buffer;
	encodeParams->quantiser = qz;

	return ADM_VIDENC_ERR_SUCCESS;
}

int Mpeg2encEncoder::finishPass(void)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	if (_openPass)
		_openPass = false;

	if (_buffer)
	{
		delete [] _buffer;
		_buffer = NULL;
	}

	return ADM_VIDENC_ERR_SUCCESS;
}

int Mpeg2encEncoder::close(void)
{
	if (_openPass)
		finishPass();

	_opened = false;
	_currentPass = 0;

	return ADM_VIDENC_ERR_SUCCESS;
}

int Mpeg2encEncoder::getFrameType(int flags)
{
	switch (flags)
	{
		case I_TYPE:
			return ADM_VIDENC_FRAMETYPE_IDR;
		case B_TYPE:
			return ADM_VIDENC_FRAMETYPE_B;
		default:
			return ADM_VIDENC_FRAMETYPE_P;
	}
}
