 /***************************************************************************
                               mpeg2Encoder.cpp

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

#include "ADM_inttype.h"
#include "mpeg2Encoder.h"

extern int _uiType;
static bool changedConfig(const char* fileName, ConfigMenuType configType);
static char *serializeConfig(void);
static Mpeg2Encoder *encoder = NULL;

#ifdef __WIN32
extern void convertPathToAnsi(const char *path, char **ansiPath);
#endif

Mpeg2Encoder::Mpeg2Encoder(void)
{
	encoder = this;

	_encodeOptions.structSize = sizeof(vidEncOptions);
	_encodeOptions.encodeMode = MPEG2_DEFAULT_ENCODE_MODE;
	_encodeOptions.encodeModeParameter = MPEG2_DEFAULT_ENCODE_MODE_PARAMETER;

	_bitrateParam.capabilities = ADM_ENC_CAP_CBR | ADM_ENC_CAP_CQ | ADM_ENC_CAP_2PASS | ADM_ENC_CAP_2PASS_BR;
	_bitrateParam.qz = MPEG2_DEFAULT_ENCODE_MODE_PARAMETER;
	_bitrateParam.avg_bitrate = 1000;
	_bitrateParam.finalsize = 700;
	_bitrateParam.bitrate = 1500;
}

const char* Mpeg2Encoder::getEncoderType(void)
{
	return "MPEG-2";
}

const char* Mpeg2Encoder::getEncoderDescription(void)
{
	return "mpeg2enc video encoder plugin for Avidemux (c) Mean/Gruntster";
}

const char* Mpeg2Encoder::getFourCC(void)
{
	return "mpg2";
}

const char* Mpeg2Encoder::getEncoderGuid(void)
{
	return "C16E9CCE-D9B3-4fbe-B0C5-8B1BEBF2178E";
}

int Mpeg2Encoder::isConfigurable(void)
{
	return (_uiType == ADM_UI_GTK || _uiType == ADM_UI_QT4);
}

int Mpeg2Encoder::configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
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
	diaElemMenu ctlWidescreen(&_widescreen, "Aspect _ratio:", 2, wideM);
	diaElemMenu ctlMatrix(&_userMatrix, "_Matrices:", 4, matrixM);
	diaElemUInteger ctlGop(&_gopSize, "_GOP size:", 1, 30);
	diaElemMenu ctlInterW(&_interlaced, "_Interlacing:", 3, interM);
	diaElem *elmGeneral[6]= {&ctlBitrate, &ctlMaxb, &ctlWidescreen, &ctlInterW, &ctlMatrix, &ctlGop};

	diaElemConfigMenu ctlConfigMenu(configName, &configType, _options.getUserConfigDirectory(), _options.getSystemConfigDirectory(),
		changedConfig, serializeConfig, elmGeneral, 6);
	diaElem *elmHeader[1] = {&ctlConfigMenu};

	diaElemTabs tabGeneral("Settings", 6, elmGeneral);
	diaElemTabs *tabs[] = {&tabGeneral};

	if (diaFactoryRunTabs("mpeg2enc Configuration", 1, elmHeader, 1, tabs))
	{
		saveSettings(&_encodeOptions, &_options);
		updateEncodeProperties(&_encodeOptions);

		return 1;
	}

	return 0;
}

void Mpeg2Encoder::loadSettings(vidEncOptions *encodeOptions, Mpeg2Options *options)
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
		_maxBitrate = options->getMaxBitrate();
		_widescreen = options->getWidescreen();
		_interlaced = options->getInterlaced();
		_userMatrix = options->getMatrix();
		_gopSize = options->getGopSize();

		updateEncodeProperties(encodeOptions);
	}
}

void Mpeg2Encoder::saveSettings(vidEncOptions *encodeOptions, Mpeg2Options *options)
{
	options->setPresetConfiguration(&configName[0], (PluginConfigType)configType);

	switch (_bitrateParam.mode)
	{
		case COMPRESS_CQ:
			encodeOptions->encodeMode = ADM_VIDENC_MODE_CQP;
			encodeOptions->encodeModeParameter = _bitrateParam.qz;

			break;
		case COMPRESS_CBR:
			encodeOptions->encodeMode = ADM_VIDENC_MODE_CBR;
			encodeOptions->encodeModeParameter = _bitrateParam.bitrate;

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

	options->setMaxBitrate(_maxBitrate);
	options->setWidescreen(_widescreen);
	options->setInterlaced((Mpeg2InterlacedMode)_interlaced);
	options->setMatrix((Mpeg2MatrixMode)_userMatrix);
	options->setGopSize(_gopSize);
}

bool changedConfig(const char* configName, ConfigMenuType configType)
{
	bool failure = false;

	if (configType == CONFIG_MENU_DEFAULT)
	{
		Mpeg2Options defaultOptions;
		vidEncOptions *defaultEncodeOptions = defaultOptions.getEncodeOptions();

		encoder->loadSettings(defaultEncodeOptions, &defaultOptions);

		delete defaultEncodeOptions;
	}
	else
	{
		Mpeg2Options options;

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
	Mpeg2Options options;

	encoder->saveSettings(&encodeOptions, &options);
	options.setEncodeOptions(&encodeOptions);

	return options.toXml(PLUGIN_XML_EXTERNAL);
}

int Mpeg2Encoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
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

int Mpeg2Encoder::setOptions(vidEncOptions *encodeOptions, const char *pluginOptions)
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

int Mpeg2Encoder::beginPass(vidEncPassParameters *passParameters)
{

}

int Mpeg2Encoder::encodeFrame(vidEncEncodeParameters *encodeParams)
{

}

int Mpeg2Encoder::finishPass(void)
{

}

void Mpeg2Encoder::updateEncodeProperties(vidEncOptions *encodeOptions)
{
	switch (encodeOptions->encodeMode)
	{
		case ADM_VIDENC_MODE_CQP:
			_passCount = 1;

			_bitrateParam.mode = COMPRESS_CQ;
			_bitrateParam.qz = encodeOptions->encodeModeParameter;

			break;
		case ADM_VIDENC_MODE_CBR:
			_passCount = 1;

			_bitrateParam.mode = COMPRESS_CBR;
			_bitrateParam.bitrate = encodeOptions->encodeModeParameter;

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

unsigned int Mpeg2Encoder::calculateBitrate(unsigned int fpsNum, unsigned int fpsDen, unsigned int frameCount, unsigned int sizeInMb)
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
