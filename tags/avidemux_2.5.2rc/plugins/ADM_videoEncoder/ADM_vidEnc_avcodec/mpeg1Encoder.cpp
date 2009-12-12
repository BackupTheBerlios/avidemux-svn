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
#include "mpegMatrix.h"

extern int _uiType;
static bool changedConfig(const char* fileName, ConfigMenuType configType);
static char *serializeConfig(void);
static Mpeg1Encoder *encoder = NULL;

#ifdef __WIN32
extern void convertPathToAnsi(const char *path, char **ansiPath);
#endif

Mpeg1Encoder::Mpeg1Encoder(void)
{
	encoder = this;

	init(CODEC_ID_MPEG1VIDEO, ADM_CSP_YV12);

	_encodeOptions.structSize = sizeof(vidEncOptions);
	_encodeOptions.encodeMode = DEFAULT_ENCODE_MODE;
	_encodeOptions.encodeModeParameter = DEFAULT_ENCODE_MODE_PARAMETER;

	_bitrateParam.capabilities = ADM_ENC_CAP_CQ | ADM_ENC_CAP_2PASS | ADM_ENC_CAP_2PASS_BR;
	_bitrateParam.qz = DEFAULT_ENCODE_MODE_PARAMETER;
	_bitrateParam.avg_bitrate = 1000;
	_bitrateParam.finalsize = 700;

	_statFile = NULL;
}

int Mpeg1Encoder::initContext(const char* logFileName)
{
	AvcodecEncoder::initContext(logFileName);
	int ret = ADM_VIDENC_ERR_SUCCESS;

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

	switch (_options.getMatrix())
	{
		case MATRIX_TMPGENC:
			printf("using custom matrix: Tmpg\n");
			_context->intra_matrix = tmpgenc_intra;
			_context->inter_matrix = tmpgenc_inter;
			break;
		case MATRIX_ANIME:
			printf("using custom matrix: anim\n");
			_context->intra_matrix = anime_intra;
			_context->inter_matrix = anime_inter;

			break;
		case MATRIX_KVCD:
			printf("using custom matrix: kvcd\n");
			_context->intra_matrix = kvcd_intra;
			_context->inter_matrix = kvcd_inter;
			break;
	}

	switch (_options.getInterlaced())
	{
		case INTERLACED_TFF:
			_frame.top_field_first = true;
		case INTERLACED_BFF:
			_frame.interlaced_frame = true;
			break;
	}

	_context->max_b_frames = 2;
	_context->luma_elim_threshold = -2;
	_context->chroma_elim_threshold = -5;
	_context->me_range = 255;
	_context->mb_decision = FF_MB_DECISION_RD;
	_context->scenechange_threshold = 0xfffffff;
	_context->rc_max_rate_header = _options.getMaxBitrate() * 1000;
	_context->rc_buffer_size_header = _options.getBufferSize() * 8 * 1024;
	_context->dark_masking = 0.01;
	_context->rc_qsquish = 1.0;

	if (_currentPass == 1)
	{
		_context->bit_rate = 0;
		_context->bit_rate_tolerance = 1024 * 8 * 1000;
		_context->flags |= CODEC_FLAG_QSCALE;

		if (_passCount > 1)
			_context->flags |= CODEC_FLAG_PASS1;
	}
	else 
	{
		if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_SIZE)
			_context->bit_rate = calculateBitrate(_fpsNum, _fpsDen, _frameCount, _encodeOptions.encodeModeParameter);
		else
			_context->bit_rate = _encodeOptions.encodeModeParameter * 1000;

		if (_context->bit_rate > _options.getMaxBitrate() * 1000)
			_context->bit_rate = _options.getMaxBitrate() * 1000;

		_context->bit_rate_tolerance = 8000000;
		_context->flags |= CODEC_FLAG_PASS2;
	}

	if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_SIZE || _encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_ABR)
	{
		char *log = NULL;

#ifdef __WIN32
		convertPathToAnsi(logFileName, &log);
#else
		log = new char[strlen(logFileName) + 1];
		strcpy(log, logFileName);
#endif

		if (_currentPass == 1)
		{
			_statFile = fopen(log, "wb");

			if (!_statFile)
				ret = ADM_VIDENC_ERR_FAILED;
		}
		else
		{
			FILE *statFile = fopen(log, "rb");

			if (statFile)
			{
				fseek(statFile, 0, SEEK_END);

				long statSize = ftello(statFile);

				fseek(statFile, 0, SEEK_SET);
				_context->stats_in = new char[statSize + 1];
				_context->stats_in[statSize] = 0;

				fread(_context->stats_in, statSize, 1, statFile);
				fclose(statFile);
			}
			else
				ret = ADM_VIDENC_ERR_FAILED;
		}
	}

	if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_CQP || _currentPass == 2)
	{
		_context->rc_max_rate = _context->rc_max_rate_header;
		_context->rc_buffer_size = _context->rc_buffer_size_header;
	}

	return ret;
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
	loadSettings(&_encodeOptions, &_options);

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
	diaElemUInteger ctlMaxb(&_maxBitrate, "Ma_x. bitrate:", 100, 9000);
	diaElemUInteger ctlMinb(&_minBitrate, "Mi_n. bitrate:", 0, 9000);
	diaElemToggle ctlXvid(&_useXvidRateControl, "_Use Xvid rate control");
	diaElemUInteger ctlVbv(&_bufferSize, "_Buffer size:", 1, 1024);
	diaElemMenu ctlWidescreen(&_widescreen, "Aspect _ratio:", 2, wideM);
	diaElemMenu ctlMatrix(&_userMatrix, "_Matrices:", 4, matrixM);
	diaElemUInteger ctlGop(&_gopSize, "_GOP size:", 1, 30);
	diaElemMenu ctlInterW(&_interlaced, "_Interlacing:", 3, interM);
	diaElem *elmGeneral[9]= {&ctlBitrate, &ctlMinb, &ctlMaxb, &ctlXvid, &ctlVbv, &ctlWidescreen, &ctlInterW, &ctlMatrix, &ctlGop};

	diaElemConfigMenu ctlConfigMenu(configName, &configType, _options.getUserConfigDirectory(), _options.getSystemConfigDirectory(),
		changedConfig, serializeConfig, elmGeneral, 9);
	diaElem *elmHeader[1] = {&ctlConfigMenu};

	diaElemTabs tabGeneral("User Interface", 9, elmGeneral);
	diaElemTabs *tabs[] = {&tabGeneral};

	if (diaFactoryRunTabs("avcodec MPEG-1 Configuration", 1, elmHeader, 1, tabs))
	{
		saveSettings(&_encodeOptions, &_options);
		updateEncodeProperties(&_encodeOptions);

		return 1;
	}

	return 0;
}

void Mpeg1Encoder::loadSettings(vidEncOptions *encodeOptions, Mpeg1EncoderOptions *options)
{
	char *configurationName;

	options->getPresetConfiguration(&configurationName, (PluginConfigType*)&configType);

	if (configurationName)
	{
		strcpy(this->configName, configurationName);
		delete [] configurationName;
	}

	if (encodeOptions)
	{
		_minBitrate = options->getMinBitrate();
		_maxBitrate = options->getMaxBitrate();
		_useXvidRateControl = options->getXvidRateControl();
		_bufferSize = options->getBufferSize();
		_widescreen = options->getWidescreen();
		_interlaced = options->getInterlaced();
		_userMatrix = options->getMatrix();
		_gopSize = options->getGopSize();

		updateEncodeProperties(encodeOptions);
	}
}

void Mpeg1Encoder::saveSettings(vidEncOptions *encodeOptions, Mpeg1EncoderOptions *options)
{
	options->setPresetConfiguration(&configName[0], (PluginConfigType)configType);

	switch (_bitrateParam.mode)
	{
		case COMPRESS_CQ:
			encodeOptions->encodeMode = ADM_VIDENC_MODE_CQP;
			encodeOptions->encodeModeParameter = _bitrateParam.qz;

			break;
		case COMPRESS_2PASS:
			encodeOptions->encodeMode = ADM_VIDENC_MODE_2PASS_SIZE;
			encodeOptions->encodeModeParameter = _bitrateParam.finalsize;

			break;
		case COMPRESS_2PASS_BITRATE:
			encodeOptions->encodeMode = ADM_VIDENC_MODE_2PASS_ABR;
			encodeOptions->encodeModeParameter = _bitrateParam.avg_bitrate;

			break;
	}

	options->setMinBitrate(_minBitrate);
	options->setMaxBitrate(_maxBitrate);
	options->setXvidRateControl(_useXvidRateControl);
	options->setBufferSize(_bufferSize);
	options->setWidescreen(_widescreen);
	options->setInterlaced((InterlacedMode)_interlaced);
	options->setMatrix((MatrixMode)_userMatrix);
	options->setGopSize(_gopSize);
}

bool changedConfig(const char* configName, ConfigMenuType configType)
{
	bool failure = false;

	if (configType == CONFIG_MENU_DEFAULT)
	{
		Mpeg1EncoderOptions defaultOptions;
		vidEncOptions *defaultEncodeOptions = defaultOptions.getEncodeOptions();

		encoder->loadSettings(defaultEncodeOptions, &defaultOptions);

		delete defaultEncodeOptions;
	}
	else
	{
		Mpeg1EncoderOptions options;

		options.setPresetConfiguration(configName, (PluginConfigType)configType);

		if (configType == CONFIG_MENU_CUSTOM)
			encoder->loadSettings(NULL, &options);
		else
		{
			vidEncOptions *encodeOptions;

			if (options.loadPresetConfiguration())
			{
				encodeOptions = options.getEncodeOptions();

				encoder->loadSettings(encodeOptions, &options);

				delete encodeOptions;
			}
			else
			{
				failure = true;
			}
		}
	}

	return configType == CONFIG_MENU_CUSTOM | !failure;
}

char *serializeConfig(void)
{
	vidEncOptions encodeOptions;
	Mpeg1EncoderOptions options;

	encoder->saveSettings(&encodeOptions, &options);
	options.setEncodeOptions(&encodeOptions);

	return options.toXml(PLUGIN_XML_EXTERNAL);
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
		updateEncodeProperties(encodeOptions);
	}

	if (success)
		return ADM_VIDENC_ERR_SUCCESS;
	else
		return ADM_VIDENC_ERR_FAILED;
}

int Mpeg1Encoder::beginPass(vidEncPassParameters *passParameters)
{
	int qz = 0;
	int ret = AvcodecEncoder::beginPass(passParameters);

	if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_CQP)
		qz = _encodeOptions.encodeModeParameter;
	else if (_currentPass == 1)
		qz = 2;

	if (qz)
		_frame.quality = (int)floor(FF_QP2LAMBDA * qz + 0.5);

	return ret;
}

int Mpeg1Encoder::finishPass(void)
{
	int ret = AvcodecEncoder::finishPass();

	if (_statFile)
	{
		fclose(_statFile);
		_statFile = NULL;
	}

	if (_context && _context->stats_in)
	{
		delete [] _context->stats_in;
		_context->stats_in = NULL;
	}

	return ret;
}

int Mpeg1Encoder::encodeFrame(vidEncEncodeParameters *encodeParams)
{
	int ret = AvcodecEncoder::encodeFrame(encodeParams);

	if (_context->stats_out)
		fprintf (_statFile, "%s", _context->stats_out);

	return ret;
}

void Mpeg1Encoder::updateEncodeProperties(vidEncOptions *encodeOptions)
{
	switch (encodeOptions->encodeMode)
	{
		case ADM_VIDENC_MODE_CQP:
			_passCount = 1;

			_bitrateParam.mode = COMPRESS_CQ;
			_bitrateParam.qz = encodeOptions->encodeModeParameter;

			break;
		case ADM_VIDENC_MODE_2PASS_SIZE:
			_passCount = 2;

			_bitrateParam.mode = COMPRESS_2PASS;
			_bitrateParam.finalsize = encodeOptions->encodeModeParameter;

			break;
		case ADM_VIDENC_MODE_2PASS_ABR:
			_passCount = 2;

			_bitrateParam.mode = COMPRESS_2PASS_BITRATE;
			_bitrateParam.avg_bitrate = encodeOptions->encodeModeParameter;

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
