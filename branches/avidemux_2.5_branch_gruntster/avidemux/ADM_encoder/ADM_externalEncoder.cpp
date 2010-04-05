/***************************************************************************
                          ADM_externalEncoder.cpp

    begin                : Tue Apr 15 2008
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

#define __STDC_LIMIT_MACROS

#include "ADM_default.h"
#include "ADM_externalEncoder.h"
#include "ADM_plugin/ADM_vidEnc_plugin.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}

static const int colourSpaceMapping[][2] = {{ADM_CSP_YV12, PIX_FMT_YUV420P}, {ADM_CSP_I422, PIX_FMT_YUV422P}};

externalEncoder::externalEncoder(COMPRES_PARAMS *params, bool globalHeader)
{
	_plugin = getVideoEncoderPlugin(params->extra_param);
	_openPass = false;
	_useExistingLogFile = false;
	_logFileName = NULL;
	_globalHeader = globalHeader;

	_swsContext = NULL;
	_resampleSize = 0;
	_resampleBuffer = NULL;
}

externalEncoder::~externalEncoder()
{
	stop();

	if (_logFileName)
		delete [] _logFileName;

	if (_resampleBuffer)
		delete [] _resampleBuffer;

	if (_swsContext)
		sws_freeContext(_swsContext);
}

int externalEncoder::getColourSpace(enum PixelFormat pixFmt)
{
	int colourSpace = 0;

	for (int i = 0; i < (sizeof(colourSpaceMapping) / sizeof(int) / 2); i++)
	{
		if (colourSpaceMapping[i][1] == pixFmt)
		{
			colourSpace = colourSpaceMapping[i][0];
			break;
		}
	}

	return colourSpace;
}

enum PixelFormat externalEncoder::getAvCodecColourspace(int colourSpace)
{
	enum PixelFormat pixFmt = PIX_FMT_NONE;

	for (int i = 0; i < (sizeof(colourSpaceMapping) / sizeof(int) / 2); i++)
	{
		if (colourSpaceMapping[i][0] == colourSpace)
		{
			pixFmt = (enum PixelFormat)colourSpaceMapping[i][1];
			break;
		}
	}

	return pixFmt;
}

uint8_t externalEncoder::configure(AVDMGenericVideoStream *instream, int useExistingLogFile)
{
	ADV_Info *info;

	info = instream->getInfo();
	_w = info->width;
	_h = info->height;

	_vbuffer = new ADMImage (_w, _h);
	ADM_assert(_vbuffer);

	_in = instream;
	_useExistingLogFile = useExistingLogFile;

	vidEncVideoProperties properties;

	memset(&properties, 0, sizeof(vidEncVideoProperties));

	properties.structSize = sizeof(vidEncVideoProperties);
	properties.width = _w;
	properties.height = _h;
	properties.parWidth = instream->getPARWidth();
	properties.parHeight = instream->getPARHeight();
	properties.frameCount = info->nb_frames;
	properties.fpsNum = info->fps1000;
	properties.fpsDen = 1000;

	if (_globalHeader)
		properties.flags |= ADM_VIDENC_FLAG_GLOBAL_HEADER;

	if (_plugin->open(_plugin->encoderId, &properties))
	{
		int64_t pixFmtMask = 0;

		for (int i = 0; i < properties.supportedCspsCount; i++)
			pixFmtMask |= (1 << getAvCodecColourspace(properties.supportedCsps[i]));

		_pixFmt = avcodec_find_best_pix_fmt(pixFmtMask, PIX_FMT_YUV420P, 0, NULL);

		if (_pixFmt != PIX_FMT_YUV420P)
		{
			AVPicture resamplePicture;

			_swsContext = sws_getContext(
				properties.width, properties.height, PIX_FMT_YUV420P,
				properties.width, properties.height, _pixFmt,
				SWS_SPLINE, NULL, NULL, NULL);

			_resampleSize = avpicture_fill(&resamplePicture, NULL, _pixFmt, properties.width, properties.height);
			_resampleBuffer = new uint8_t[_resampleSize];
		}

		printf("[externalEncoder] Target colourspace: %s\n", _pixFmt == PIX_FMT_YUV420P ? "yv12" : avcodec_get_pix_fmt_name(_pixFmt));

		return (startPass() == ADM_VIDENC_ERR_SUCCESS);
	}
	else
		return 0;
}

uint8_t externalEncoder::encode(uint32_t frame, ADMBitstream *out)
{
	uint32_t l, f;
	vidEncEncodeParameters params;

	if (frame != UINT32_MAX && !_in->getFrameNumberNoAlloc(frame, &l, _vbuffer, &f))
	{
		printf ("[externalEncoder] Error reading incoming frame\n");
		return 0;
	}

	params.structSize = sizeof(vidEncEncodeParameters);

	if (frame == UINT32_MAX)
	{
		memset(&params.frameData, 0, sizeof(params.frameData));
		memset(&params.frameLineSize, 0, sizeof(params.frameLineSize));

		params.frameDataSize = 0;
	}
	else
	{
		AVPicture sourcePicture, resamplePicture, *inputPicture;

		params.frameDataSize = avpicture_fill(&sourcePicture, _vbuffer->data, PIX_FMT_YUV420P, _w, _h);

		if (_swsContext)
		{
			params.frameDataSize = avpicture_fill(&resamplePicture, _resampleBuffer, _pixFmt, _w, _h);

			// Swap planes since input is YV12 (not YUV420P)
			uint8_t *tmpPlane = sourcePicture.data[1];

			sourcePicture.data[1] = sourcePicture.data[2];
			sourcePicture.data[2] = tmpPlane;

			sws_scale(
				_swsContext, (const uint8_t**)sourcePicture.data, sourcePicture.linesize, 0,
				_h, resamplePicture.data, resamplePicture.linesize);

			inputPicture = &resamplePicture;
		}
		else
			inputPicture = &sourcePicture;

		memcpy(&params.frameLineSize, inputPicture->linesize, sizeof(params.frameLineSize));
		memcpy(&params.frameData, inputPicture->data, sizeof(params.frameData));
	}

	params.encodedData = NULL;
	params.encodedDataSize = 0;

	if (_plugin->encodeFrame(_plugin->encoderId, &params))
	{
		memcpy(out->data, params.encodedData, params.encodedDataSize);

		out->len = params.encodedDataSize;
		out->ptsFrame = params.ptsFrame;
		out->out_quantizer = params.quantiser;

		switch (params.frameType)
		{
			case ADM_VIDENC_FRAMETYPE_NULL:
				break;
			case ADM_VIDENC_FRAMETYPE_IDR:
				out->flags = AVI_KEY_FRAME;
				break;
			case ADM_VIDENC_FRAMETYPE_B:
				out->flags = AVI_B_FRAME;
				break;
			case ADM_VIDENC_FRAMETYPE_P:
				out->flags = AVI_P_FRAME;
				break;
			default:
				assert(0);
				break;
		}

		return 1;
	}
	else
		return 0;
}

const char* externalEncoder::getCodecName(void)
{
	return _plugin->getFourCC(_plugin->encoderId);
}

const char* externalEncoder::getDisplayName(void)
{
	return _plugin->getEncoderName(_plugin->encoderId);
}

const char* externalEncoder::getFCCHandler(void)
{
	return _plugin->getFourCC(_plugin->encoderId);
}

int externalEncoder::getRequirements(void)
{
	int req = _plugin->getEncoderRequirements(_plugin->encoderId);

	if (req == ADM_VIDENC_REQ_NULL_FLUSH)
		return ADM_ENC_REQ_NULL_FLUSH;
}

uint8_t externalEncoder::isDualPass(void)
{
	return _plugin->getPassCount(_plugin->encoderId) == 2;
}

uint8_t externalEncoder::startPass(void)
{
	vidEncPassParameters passParameters;
	int ret;

	memset(&passParameters, 0, sizeof(vidEncPassParameters));
	passParameters.structSize = sizeof(vidEncPassParameters);
	passParameters.logFileName = _logFileName;
	passParameters.useExistingLogFile = _useExistingLogFile;
	passParameters.csp = getColourSpace(_pixFmt);

	ret = _plugin->beginPass(_plugin->encoderId, &passParameters);

	if (ret == ADM_VIDENC_ERR_PASS_SKIP)
	{
		printf("[externalEncoder] skipping pass\n");

		return 1;
	}

	if (ret != ADM_VIDENC_ERR_SUCCESS)
		printf("[externalEncoder] begin pass failed: %d\n", ret);

	_openPass = (ret == ADM_VIDENC_ERR_SUCCESS);

	if (_openPass)
	{
		_extraData = passParameters.extraData;
		_extraDataSize = passParameters.extraDataSize;
	}
	else
	{
		_extraData = NULL;
		_extraDataSize = 0;
	}

	return _openPass;
}

uint8_t externalEncoder::startPass1(void)
{
	return 1;
}

uint8_t externalEncoder::startPass2(void)
{
	if (_openPass)
		_plugin->finishPass(_plugin->encoderId);

	return startPass();
}

uint8_t externalEncoder::hasExtraHeaderData(uint32_t *l, uint8_t **data)
{
	printf ("[externalEncoder] %d extra header bytes\n", _extraDataSize);

	if (_extraDataSize > 0)
	{
		*data = _extraData;
		*l = _extraDataSize;

		return 1;
	}
	else
		return 0;
}

uint8_t externalEncoder::stop(void)
{
	if (_openPass)
		_plugin->finishPass(_plugin->encoderId);

	_plugin->close(_plugin->encoderId);

	return 1;
}

uint8_t externalEncoder::setLogFile(const char *p, uint32_t fr)
{
	if (_logFileName)
		delete [] _logFileName;

	_logFileName = new char[strlen(p) + 1];
	strcpy(_logFileName, p);

	return 1;
}
