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
#include <math.h>
#include <libxml/tree.h>

#include "encoder.h"
#include "mpeg1Encoder.h"
#include "mpeg2Encoder.h"
#include "ADM_inttype.h"
#include "mpeg2enc.h"

int uiType;

static Mpeg1Encoder mpeg1;
static Mpeg2Encoder mpeg2;

static int encoderIds[] = { 0, 1 };
static Mpeg2encEncoder* encoders[] = {&mpeg1, &mpeg2};
static int supportedCsps[] = { ADM_CSP_YV12 };

extern int mpegenc_encode(char *in, char *out, int *size, int *flags, int *quant);
extern int mpegenc_end(void);
extern int mpegenc_init(mpeg2parm *incoming,int width, int height, int fps1000);
extern int mpegenc_setQuantizer(int q);

#ifdef __WIN32
extern void convertPathToAnsi(const char *path, char **ansiPath);
#endif

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
	return ADM_VIDENC_REQ_NULL_FLUSH;
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
	_bufferSize = (properties->width * properties->height) + 2 * (((properties->width + 1) >> 1) * ((properties->height + 1) >> 1));
	_buffer = new uint8_t[_bufferSize];
	_xvidRc = NULL;

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

	int encodeModeParameter, maxBitrate, vbv;

	_openPass = true;
	_currentPass++;

	memset(&_param, 0, sizeof(mpeg2parm));
	_param.setDefault();
	_param.searchrad = 16; // speed up

	initParameters(&encodeModeParameter, &maxBitrate, &vbv);

	if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_SIZE || _encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_ABR)
	{
		char *log = NULL;

#ifdef __WIN32
		convertPathToAnsi(passParameters->logFileName, &log);
#else
		log = new char[strlen(passParameters->logFileName) + 1];
		strcpy(log, passParameters->logFileName);
#endif
		_xvidRc = new ADM_newXvidRcVBV((_fpsNum * 1000) / _fpsDen, log);
		delete [] log;

		_param.quant = 2;

		if (_currentPass == 1)
		{
			_xvidRc->startPass1();
			_param.ignore_constraints = 1;
			_param.bitrate = 50000000;
		}
		else
		{
			uint32_t bitrate, size;

			if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_SIZE)
			{
				size = encodeModeParameter;
				bitrate = calculateBitrate(_fpsNum, _fpsDen, _frameCount, encodeModeParameter);
			}
			else
			{
				double d = _frameCount;

				d *= 1000.;
				d /= (_fpsNum * 1000) / _fpsDen;   // D is a duration in second
				d *= encodeModeParameter;   // * bitrate = total bits
				d /= 8;   // Byte
				d /= 1024 * 1024;   // MB

				size = (uint32_t)d;
				bitrate = encodeModeParameter * 1000;
			}

			if (bitrate > maxBitrate * 1000)
				bitrate = maxBitrate * 1000;

			_xvidRc->setVBVInfo(maxBitrate, 0, vbv);
			_xvidRc->startPass2(size, _frameCount);
			_param.bitrate = bitrate;
		}
	}
	else if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_CQP)
	{
		_param.quant = encodeModeParameter;
		_param.bitrate = maxBitrate * 1000;
	}
	else if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_CBR)
	{
		_param.quant = 0;
		_param.bitrate = encodeModeParameter * 1000;
	}

	if (!mpegenc_init(&_param, _width, _height, (_fpsNum * 1000) / _fpsDen))
		return ADM_VIDENC_ERR_FAILED;

	return ADM_VIDENC_ERR_SUCCESS;
}

int Mpeg2encEncoder::encodeFrame(vidEncEncodeParameters *encodeParams)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	ADM_rframe rf;
	int flags, size, qz;

	if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_CQP)
	{
		mpegenc_setQuantizer(_encodeOptions.encodeModeParameter);
	}
	else if (_currentPass == 2)
	{
		uint32_t qz_in;

		_xvidRc->getQz(&qz_in, &rf);

		if (qz_in < 2)
			qz_in = 2;
		else if (qz_in > 28)
			qz_in = 28;

		mpegenc_setQuantizer(qz_in);
	}

	char *input;

	if (encodeParams->frameData[0] == NULL)
		input = new char[1];
	else
		input = (char*)encodeParams->frameData[0];

	if (!mpegenc_encode(input, (char*)_buffer, &size, &flags, &qz))
		return ADM_VIDENC_ERR_FAILED;

	if (encodeParams->frameData[0] == NULL)
		delete [] input;

	encodeParams->frameType = getFrameType(flags);
	encodeParams->encodedDataSize = size;
	encodeParams->encodedData = _buffer;
	encodeParams->ptsFrame = 0;
	encodeParams->quantiser = qz;

	switch (encodeParams->frameType)
	{
		case ADM_VIDENC_FRAMETYPE_IDR:
			rf = RF_I;
			break;
		case ADM_VIDENC_FRAMETYPE_B:
			rf = RF_B;
			break;
		case ADM_VIDENC_FRAMETYPE_P:
			rf = RF_P;
			break;
	}

	if (encodeParams->encodedDataSize > 0 && 
		(_encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_SIZE || _encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_ABR))
	{
		if (_currentPass == 1)
			_xvidRc->logPass1(encodeParams->quantiser, rf, encodeParams->encodedDataSize);
		else
			_xvidRc->logPass2(encodeParams->quantiser, rf, encodeParams->encodedDataSize);
	}

	return ADM_VIDENC_ERR_SUCCESS;
}

int Mpeg2encEncoder::finishPass(void)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	mpegenc_end();

	if (_openPass)
		_openPass = false;

	if (_xvidRc)
	{
		delete _xvidRc;
		_xvidRc = NULL;
	}

	return ADM_VIDENC_ERR_SUCCESS;
}

int Mpeg2encEncoder::close(void)
{
	if (_openPass)
		finishPass();

	if (_buffer)
	{
		delete [] _buffer;
		_buffer = NULL;
	}

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

unsigned int Mpeg2encEncoder::calculateBitrate(unsigned int fpsNum, unsigned int fpsDen, unsigned int frameCount, unsigned int sizeInMb)
{
	double db, ti;

	db = sizeInMb;
	db = db * 1024. * 1024. * 8.;
	// now db is in bits

	// compute duration
	ti = frameCount;
	ti *= fpsDen;
	ti /= fpsNum;	// nb sec
	db = db / ti;

	return (unsigned int)floor(db);
}
