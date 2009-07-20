/***************************************************************************
                               mpeg1Encoder.cpp

    begin                : Sat Jul 4 2009
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
#include "ADM_inttype.h"
#include "mpeg1Encoder.h"
#include "DIA_factory.h"

extern int _uiType;

Mpeg1Encoder::Mpeg1Encoder(void)
{
	init(CODEC_ID_MPEG1VIDEO, ADM_CSP_YV12);

	_encodeOptions.structSize = sizeof(vidEncOptions);
	_encodeOptions.encodeMode = DEFAULT_ENCODE_MODE;
	_encodeOptions.encodeModeParameter = DEFAULT_ENCODE_MODE_PARAMETER;

	_bitrateParam.capabilities = ADM_ENC_CAP_CQ | ADM_ENC_CAP_2PASS | ADM_ENC_CAP_2PASS_BR;
}

void Mpeg1Encoder::initContext(vidEncVideoProperties *properties)
{
	AvcodecEncoder::initContext(properties);

	_context->gop_size = _options.getGopSize();

	if (_options.getWidescreen())
	{
		_context->sample_aspect_ratio.num = 16;
		_context->sample_aspect_ratio.den = 9;
	}
	else
	{
		_context->sample_aspect_ratio.num = 4;
		_context->sample_aspect_ratio.den = 3;
	}
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
	return "85FC9CAC-CE6C-4aa6-9D5F-352D6349BA3E";
}

int Mpeg1Encoder::isConfigurable(void)
{
	return (_uiType == ADM_UI_GTK || _uiType == ADM_UI_QT4);
}

int Mpeg1Encoder::configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	unsigned int minBitrate = _options.getMinBitrate();
	unsigned int maxBitrate = _options.getMaxBitrate();
	unsigned int useXvidRateControl = _options.getXvidRateControl();
	unsigned int bufferSize = _options.getBufferSize();
	unsigned int widescreen = _options.getWidescreen();
	unsigned int interlaced = _options.getInterlaced();
	unsigned int userMatrix = _options.getMatrix();
	unsigned int gopSize = _options.getGopSize();

	_bitrateParam.avg_bitrate = 1000;
	_bitrateParam.finalsize = 700;

	updateEncodeProperties(properties);

	diaMenuEntry wideM[] = {
		{0, "4:3"},
		{1, "16:9"}};

	diaMenuEntry matrixM[]={
		{0, "Default"},
		{1, "TMPGEnc"},
		{2, "Anime"},
		{3, "KVCD"}
	};

	diaMenuEntry interM[]={
		{0, "Progressive"},
		{1, "Interlaced BFF"},
		{2, "Interlaced TFF"}
	};

	diaElemBitrate ctlBitrate(&_bitrateParam, NULL);
	diaElemUInteger ctlMaxb(&maxBitrate, "Ma_x. bitrate:", 100, 9000);
	diaElemUInteger ctlMinb(&minBitrate, "Mi_n. bitrate:", 0, 9000);
	diaElemToggle ctlXvid(&useXvidRateControl, "_Use Xvid rate control");
	diaElemUInteger ctlVbv(&bufferSize, "_Buffer size:", 1, 1024);
	diaElemMenu ctlWidescreen(&widescreen, "Aspect _ratio:", 2, wideM);
	diaElemMenu ctlMatrix(&userMatrix, "_Matrices:", 4, matrixM);
	diaElemUInteger ctlGop(&gopSize, "_GOP size:", 1, 30);
	diaElemMenu ctlInterW(&interlaced, "_Interlacing:", 3, interM);

	diaElem *elems[9]= {&ctlBitrate, &ctlMinb, &ctlMaxb, &ctlXvid, &ctlVbv, &ctlWidescreen, &ctlInterW, &ctlMatrix, &ctlGop};

	if (diaFactoryRun("avcodec MPEG-1 Configuration", 9, elems))
	{
		switch (_bitrateParam.mode)
		{
			case COMPRESS_CQ:
				_encodeOptions.encodeMode = ADM_VIDENC_MODE_CQP;
				_encodeOptions.encodeModeParameter = _bitrateParam.qz;

				break;
			case COMPRESS_2PASS:
				_encodeOptions.encodeMode = ADM_VIDENC_MODE_2PASS_SIZE;
				_encodeOptions.encodeModeParameter = _bitrateParam.finalsize;

				break;
			case COMPRESS_2PASS_BITRATE:
				_encodeOptions.encodeMode = COMPRESS_2PASS_BITRATE;
				_encodeOptions.encodeModeParameter = _bitrateParam.avg_bitrate;

				break;
		}

		_options.setMinBitrate(minBitrate);
		_options.setMaxBitrate(maxBitrate);
		_options.setXvidRateControl(useXvidRateControl);
		_options.setBufferSize(bufferSize);
		_options.setWidescreen(widescreen);
		_options.setInterlaced((InterlacedMode)interlaced);
		_options.setMatrix((MatrixMode)userMatrix);
		_options.setGopSize(gopSize);

		return 1;
	}

	return 0;
}

int Mpeg1Encoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
{
	char* xml = _options.toXml(PLUGIN_XML_INTERNAL);
	int xmlLength = strlen(xml);

	if (bufferSize >= xmlLength)
	{
		memcpy(pluginOptions, xml, xmlLength);
		memcpy(encodeOptions, &_encodeOptions, sizeof(vidEncOptions));
	}
	else if (bufferSize != 0)
		xmlLength = 0;

	delete [] xml;

	return xmlLength;
}

int Mpeg1Encoder::setOptions(vidEncOptions *encodeOptions, char *pluginOptions)
{
	if (_opened)
		return ADM_VIDENC_ERR_ALREADY_OPEN;

	bool success = true;

	if (pluginOptions)
	{
		success = _options.fromXml(pluginOptions, PLUGIN_XML_INTERNAL);

		_options.loadPresetConfiguration();
	}

	if (encodeOptions && success)
	{
		memcpy(&_encodeOptions, encodeOptions, sizeof(vidEncOptions));
		updateEncodeProperties(NULL);
	}

	if (success)
		return ADM_VIDENC_ERR_SUCCESS;
	else
		return ADM_VIDENC_ERR_FAILED;
}

int Mpeg1Encoder::beginPass(vidEncPassParameters *passParameters)
{
	return AvcodecEncoder::beginPass(passParameters);
}

int Mpeg1Encoder::encodeFrame(vidEncEncodeParameters *encodeParams)
{
	int ret = AvcodecEncoder::encodeFrame(encodeParams);

	if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_CQP)
		encodeParams->quantiser = _encodeOptions.encodeModeParameter;

	return ret;
}

void Mpeg1Encoder::updateEncodeProperties(vidEncVideoProperties *properties)
{
	switch (_encodeOptions.encodeMode)
	{
		case ADM_VIDENC_MODE_CQP:
			_passCount = 1;

			_bitrateParam.mode = COMPRESS_CQ;
			_bitrateParam.qz = _encodeOptions.encodeModeParameter;

			break;
		case ADM_VIDENC_MODE_2PASS_SIZE:
			_passCount = 2;

			_bitrateParam.mode = COMPRESS_2PASS;
			_bitrateParam.finalsize = _encodeOptions.encodeModeParameter;

			//if (properties)
				//bitrateParam->bitrate = calculateBitrate(properties->fpsNum, properties->fpsDen, properties->frameCount, _encodeOptions.encodeModeParameter) / 1000;					
			//else
				//bitrateParam->bitrate = 1500;

			break;
		case ADM_VIDENC_MODE_2PASS_ABR:
			_passCount = 2;

			_bitrateParam.mode = COMPRESS_2PASS_BITRATE;
			_bitrateParam.avg_bitrate = _encodeOptions.encodeModeParameter;

			break;
	}
}

unsigned int Mpeg1Encoder::calculateBitrate(unsigned int fpsNum, unsigned int fpsDen, unsigned int frameCount, unsigned int sizeInMb)
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
