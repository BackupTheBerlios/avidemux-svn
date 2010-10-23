/***************************************************************************
                               flv1Encoder.cpp

    begin                : Tue Dec 29 2009
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
#include "ADM_plugin_translate.h"
#include "flv1Encoder.h"

extern int _uiType;
static bool changedConfig(const char* fileName, ConfigMenuType configType);
static char *serializeConfig(void);
static FLV1Encoder *encoder = NULL;

#ifdef __WIN32
extern void convertPathToAnsi(const char *path, char **ansiPath);
#endif

FLV1Encoder::FLV1Encoder(void)
{
	encoder = this;

	init(CODEC_ID_FLV1, ADM_CSP_YV12);

	_encodeOptions.structSize = sizeof(vidEncOptions);
	_encodeOptions.encodeMode = FLV1_DEFAULT_ENCODE_MODE;
	_encodeOptions.encodeModeParameter = FLV1_DEFAULT_ENCODE_MODE_PARAMETER;
}

int FLV1Encoder::initContext(const char* logFileName)
{
	AvcodecEncoder::initContext(logFileName);

	_context->gop_size = _options.getGopSize();
	_context->bit_rate = _encodeOptions.encodeModeParameter * 1000;
	_context->bit_rate_tolerance = 8000000;
	_context->luma_elim_threshold = -2;
	_context->chroma_elim_threshold = -5;
	_context->lumi_masking = 0.05;
	_context->mb_decision = FF_MB_DECISION_RD;
	_context->rc_qsquish = 1.0;
	_context->i_quant_factor = 0.8;
	_context->dark_masking = 0.01;

	return ADM_VIDENC_ERR_SUCCESS;
}

const char* FLV1Encoder::getEncoderType(void)
{
	return "Sorenson Spark";
}

const char* FLV1Encoder::getEncoderDescription(void)
{
	return "Sorenson Spark video encoder plugin for Avidemux (c) Mean/Gruntster";
}

const char* FLV1Encoder::getFourCC(void)
{
	return "FLV1";
}

const char* FLV1Encoder::getEncoderGuid(void)
{
	return "134AA23B-A1FE-4d7b-AC99-85E440BA4595";
}

int FLV1Encoder::isConfigurable(void)
{
	return (_uiType == ADM_UI_GTK || _uiType == ADM_UI_QT4);
}

int FLV1Encoder::configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	loadSettings(&_encodeOptions, &_options);

	diaElemUInteger ctlBitrate(&_bitrate, QT_TR_NOOP("_Bitrate (kb/s):"), 100, 9000);
	diaElemUInteger ctlGop(&_gopSize, QT_TR_NOOP("_GOP size:"), 1, 250);
	diaElem *elmGeneral[2] = {&ctlBitrate, &ctlGop};

	diaElemConfigMenu ctlConfigMenu(configName, &configType, _options.getUserConfigDirectory(), _options.getSystemConfigDirectory(),
		changedConfig, serializeConfig, elmGeneral, 2);
	diaElem *elmHeader[1] = {&ctlConfigMenu};

	diaElemTabs tabGeneral(QT_TR_NOOP("Settings"), 2, elmGeneral);
	diaElemTabs *tabs[] = {&tabGeneral};

	if (diaFactoryRunTabs(QT_TR_NOOP("avcodec Sorenson Spark Configuration"), 1, elmHeader, 1, tabs))
	{
		saveSettings(&_encodeOptions, &_options);
		updateEncodeProperties(&_encodeOptions);

		return 1;
	}

	return 0;
}

void FLV1Encoder::loadSettings(vidEncOptions *encodeOptions, FLV1EncoderOptions *options)
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
		_gopSize = options->getGopSize();

		updateEncodeProperties(encodeOptions);
	}
}

void FLV1Encoder::saveSettings(vidEncOptions *encodeOptions, FLV1EncoderOptions *options)
{
	options->setPresetConfiguration(&configName[0], (PluginConfigType)configType);

	encodeOptions->encodeMode = ADM_VIDENC_MODE_CBR;
	encodeOptions->encodeModeParameter = _bitrate;

	options->setGopSize(_gopSize);
}

bool changedConfig(const char* configName, ConfigMenuType configType)
{
	bool failure = false;

	if (configType == CONFIG_MENU_DEFAULT)
	{
		FLV1EncoderOptions defaultOptions;
		vidEncOptions *defaultEncodeOptions = defaultOptions.getEncodeOptions();

		encoder->loadSettings(defaultEncodeOptions, &defaultOptions);

		delete defaultEncodeOptions;
	}
	else
	{
		FLV1EncoderOptions options;

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

	return (configType == CONFIG_MENU_CUSTOM) | !failure;
}

char *serializeConfig(void)
{
	vidEncOptions encodeOptions;
	FLV1EncoderOptions options;

	encoder->saveSettings(&encodeOptions, &options);
	options.setEncodeOptions(&encodeOptions);

	return options.toXml(PLUGIN_XML_EXTERNAL);
}

int FLV1Encoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
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

int FLV1Encoder::setOptions(vidEncOptions *encodeOptions, const char *pluginOptions)
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

void FLV1Encoder::updateEncodeProperties(vidEncOptions *encodeOptions)
{
	_passCount = 1;
	_bitrate = encodeOptions->encodeModeParameter;
}
