/***************************************************************************
                                encoder.cpp

    begin                : Thu Apr 10 2009
    copyright            : (C) 2008 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "encoder.h"
#include "dvEncoder.h"
#include "ffv1Encoder.h"
#include "ffvhuffEncoder.h"
#include "HuffyuvEncoder.h"
#include "ADM_inttype.h"

int uiType;

static DVEncoder dv;
static FFV1Encoder ffv1;
static FFVHuffEncoder ffvhuff;
static HuffyuvEncoder huffyuv;

static AvcodecEncoder* encoders[] = { &dv, &ffv1, &ffvhuff, &huffyuv};

extern "C"
{
	void *avcodecEncoder_getPointers(int _uiType, int *count)
	{
		uiType = _uiType;
		*count = sizeof(encoders) / sizeof(AvcodecEncoder*);

		return &encoders;
	}

	const char* avcodecEncoder_getEncoderName(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->getEncoderName();
	}

	const char* avcodecEncoder_getEncoderType(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->getEncoderType();
	}

	const char* avcodecEncoder_getEncoderDescription(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->getEncoderDescription();
	}

	const char* avcodecEncoder_getFourCC(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->getFourCC();
	}

	int avcodecEncoder_getEncoderRequirements(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->getEncoderRequirements();
	}

	const char* avcodecEncoder_getEncoderGuid(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->getEncoderGuid();
	}

	int avcodecEncoder_isConfigurable(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->isConfigurable();
	}

	int avcodecEncoder_configure(int encoderId, vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->configure(configParameters, properties);
	}

	int avcodecEncoder_getOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->getOptions(encodeOptions, pluginOptions, bufferSize);
	}

	int avcodecEncoder_setOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->setOptions(encodeOptions, pluginOptions);
	}

	int avcodecEncoder_getPassCount(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->getPassCount();
	}

	int avcodecEncoder_getCurrentPass(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->getCurrentPass();
	}

	int avcodecEncoder_open(int encoderId, vidEncVideoProperties *properties)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->open(properties);
	}

	int avcodecEncoder_beginPass(int encoderId, vidEncPassParameters *passParameters)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->beginPass(passParameters);
	}

	int avcodecEncoder_encodeFrame(int encoderId, vidEncEncodeParameters *encodeParams)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->encodeFrame(encodeParams);
	}

	int avcodecEncoder_finishPass(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->finishPass();
	}

	int avcodecEncoder_close(int encoderId)
	{
		AvcodecEncoder *encoder = (AvcodecEncoder*)encoderId;
		return encoder->close();
	}
}

void AvcodecEncoder::init(enum CodecID id, enum PixelFormat targetPixelFormat)
{
	_codecId = id;
	_opened = false;

	_passCount = 1;
	_currentPass = 0;
	_openPass = false;

	_targetPixelFormat = targetPixelFormat;

	_swsContext = NULL;
	_resampleBuffer = NULL;
}

void AvcodecEncoder::initContext(vidEncVideoProperties *properties)
{
	_context->width = properties->width;
	_context->height = properties->height;
	_context->time_base = (AVRational){properties->fpsDen, properties->fpsNum};
	_context->pix_fmt = _targetPixelFormat;
}

AVCodec *AvcodecEncoder::getAvCodec(void)
{
	return avcodec_find_encoder(_codecId);
}

int AvcodecEncoder::getFrameType(void)
{
	if (_context->coded_frame->key_frame == 1)
		return ADM_VIDENC_FRAMETYPE_IDR;
	else if (_context->coded_frame->pict_type == FF_B_TYPE)
		return ADM_VIDENC_FRAMETYPE_B;

	return ADM_VIDENC_FRAMETYPE_P;
}

void AvcodecEncoder::updateEncodeParameters(vidEncEncodeParameters *encodeParams, uint8_t *buffer, int bufferSize)
{
	encodeParams->frameType = getFrameType();
	encodeParams->ptsFrame = _context->coded_frame->display_picture_number;
	encodeParams->encodedDataSize = bufferSize;
	encodeParams->encodedData = buffer;

	if (_context->coded_frame->quality)
		encodeParams->quantiser =(int)floor(_context->coded_frame->quality / (float)FF_QP2LAMBDA);
	else
		encodeParams->quantiser = (int)floor(_frame.quality / (float)FF_QP2LAMBDA);
}

AvcodecEncoder::~AvcodecEncoder(void)
{
	close();
}

const char* AvcodecEncoder::getEncoderName(void)
{
	return "avcodec";
}

int AvcodecEncoder::getEncoderRequirements(void)
{
	AVCodec *codec = getAvCodec();

	return (codec && (codec->capabilities & CODEC_CAP_DELAY) ? ADM_VIDENC_REQ_NULL_FLUSH : ADM_VIDENC_REQ_NONE);
}

int AvcodecEncoder::isConfigurable(void)
{
	return 0;
}

int AvcodecEncoder::configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	return 0;
}

int AvcodecEncoder::getCurrentPass(void)
{
	return _currentPass;
}

int AvcodecEncoder::getPassCount(void)
{
	return _passCount;
}

int AvcodecEncoder::open(vidEncVideoProperties *properties)
{
	if (_opened)
		return ADM_VIDENC_ERR_ALREADY_OPEN;

	_opened = true;
	_currentPass = 0;
	_context = avcodec_alloc_context();

	if (!_context)
		return ADM_VIDENC_ERR_FAILED;

	initContext(properties);

	AVCodec *codec = getAvCodec();

	if (!codec)
		return ADM_VIDENC_ERR_FAILED;

	if (avcodec_open(_context, codec) < 0)
	{
		close();

		return ADM_VIDENC_ERR_FAILED;
	}

	memset(&_frame, 0, sizeof(_frame));
	_frame.pts = AV_NOPTS_VALUE;

	_bufferSize = properties->width * properties->height * 3;
	_buffer = new uint8_t[_bufferSize];

	return ADM_VIDENC_ERR_SUCCESS;
}

int AvcodecEncoder::beginPass(vidEncPassParameters *passParameters)
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

	if (_context->pix_fmt != PIX_FMT_YUV420P)
	{
		_swsContext = sws_getContext(
			_context->width, _context->height, PIX_FMT_YUV420P,
			_context->width, _context->height, _context->pix_fmt,
			SWS_BICUBLIN, NULL, NULL, NULL);

		_resampleSize = _bufferSize;
		_resampleBuffer = new uint8_t[_resampleSize];
	}

	passParameters->structSize = sizeof(vidEncPassParameters);
	passParameters->extraData = _context->extradata;
	passParameters->extraDataSize = _context->extradata_size;

	return ADM_VIDENC_ERR_SUCCESS;
}

int AvcodecEncoder::encodeFrame(vidEncEncodeParameters *encodeParams)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	_frame.key_frame = 0;
	_frame.pict_type = 0;

	if (encodeParams->frameData)
	{
		AVPicture sourcePicture, *inputPicture;

		avpicture_fill(
			&sourcePicture, encodeParams->frameData, PIX_FMT_YUV420P, _context->width, _context->height);

		// Swap planes to make YV12 look like YUV420P
		uint8_t *tmpPlane = sourcePicture.data[1];

		sourcePicture.data[1] = sourcePicture.data[2];
		sourcePicture.data[2] = tmpPlane;

		if (_swsContext)
		{
			AVPicture resamplePicture;

			avpicture_fill(
				&resamplePicture, _resampleBuffer, _context->pix_fmt, _context->width, _context->height);

			inputPicture = &resamplePicture;

			sws_scale(
				_swsContext, sourcePicture.data, sourcePicture.linesize, 0,
				_context->height, inputPicture->data, inputPicture->linesize);
		}
		else
			inputPicture = &sourcePicture;

		memcpy(&_frame.linesize, inputPicture->linesize, sizeof(_frame.linesize));
		memcpy(&_frame.data, inputPicture->data, sizeof(_frame.data));

		int size = avcodec_encode_video(_context, _buffer, _bufferSize, &_frame);

		if (size < 0)
			return ADM_VIDENC_ERR_FAILED;

		updateEncodeParameters(encodeParams, _buffer, size);
	}

	return ADM_VIDENC_ERR_SUCCESS;
}

int AvcodecEncoder::finishPass(void)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	if (_openPass)
		_openPass = false;

	if (_swsContext)
	{
		sws_freeContext(_swsContext);
		_swsContext = NULL;
	}

	if (_resampleBuffer)
	{
		delete _resampleBuffer;
		_resampleBuffer = NULL;
	}

	return ADM_VIDENC_ERR_SUCCESS;
}

int AvcodecEncoder::close(void)
{
	if (_openPass)
		finishPass();

	_opened = false;
	_currentPass = 0;

	if (_context)
	{
		avcodec_close(_context);
		_context = NULL;
	}

	if (_buffer)
	{
		delete [] _buffer;
		_buffer = NULL;
	}

	return ADM_VIDENC_ERR_SUCCESS;
}
