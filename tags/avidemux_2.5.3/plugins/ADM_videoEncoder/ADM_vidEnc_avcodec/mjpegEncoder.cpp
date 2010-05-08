/***************************************************************************
                              mjpegEncoder.cpp

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
#include "mjpegEncoder.h"

extern int _uiType;
static bool changedConfig(const char* fileName, ConfigMenuType configType);
static char *serializeConfig(void);
static MjpegEncoder *encoder = NULL;

#ifdef __WIN32
extern void convertPathToAnsi(const char *path, char **ansiPath);
#endif

MjpegEncoder::MjpegEncoder(void)
{
	encoder = this;

	init(CODEC_ID_MJPEG, ADM_CSP_YV12);

	_encodeOptions.structSize = sizeof(vidEncOptions);
	_encodeOptions.encodeMode = MJPEG_DEFAULT_ENCODE_MODE;
	_encodeOptions.encodeModeParameter = MJPEG_DEFAULT_ENCODE_MODE_PARAMETER;
}

int MjpegEncoder::initContext(const char* logFileName)
{
	AvcodecEncoder::initContext(logFileName);

	_context->pix_fmt = PIX_FMT_YUVJ420P;
	_context->flags = CODEC_FLAG_QSCALE;
	_context->bit_rate = 0;
	_context->bit_rate_tolerance = 1024 * 8 * 1000;
	_context->gop_size = 250;
	_context->rc_qsquish = 1.0;
	_context->i_quant_factor = 0.8;

	return ADM_VIDENC_ERR_SUCCESS;
}

const char* MjpegEncoder::getEncoderType(void)
{
	return "M-JPEG";
}

const char* MjpegEncoder::getEncoderDescription(void)
{
	return "M-JPEG video encoder plugin for Avidemux (c) Mean/Gruntster";
}

const char* MjpegEncoder::getFourCC(void)
{
	return "MJPG";
}

const char* MjpegEncoder::getEncoderGuid(void)
{
	return "075E8A4E-5B3D-47c6-9F70-853D6B855106";
}

int MjpegEncoder::isConfigurable(void)
{
	return (_uiType == ADM_UI_GTK || _uiType == ADM_UI_QT4);
}

int MjpegEncoder::configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	loadSettings(&_encodeOptions, &_options);

	diaElemUInteger ctlQuantiser(&_quantiser, "_Quantiser:", 2, 31);
	diaElem *elmGeneral[1] = {&ctlQuantiser};

	diaElemConfigMenu ctlConfigMenu(configName, &configType, _options.getUserConfigDirectory(), _options.getSystemConfigDirectory(),
		changedConfig, serializeConfig, elmGeneral, 1);
	diaElem *elmHeader[1] = {&ctlConfigMenu};

	diaElemTabs tabGeneral("Settings", 1, elmGeneral);
	diaElemTabs *tabs[] = {&tabGeneral};

	if (diaFactoryRunTabs("avcodec M-JPEG Configuration", 1, elmHeader, 1, tabs))
	{
		saveSettings(&_encodeOptions, &_options);
		updateEncodeProperties(&_encodeOptions);

		return 1;
	}

	return 0;
}

void MjpegEncoder::loadSettings(vidEncOptions *encodeOptions, MjpegEncoderOptions *options)
{
	char *configurationName;

	options->getPresetConfiguration(&configurationName, (PluginConfigType*)&configType);

	if (configurationName)
	{
		strcpy(this->configName, configurationName);
		delete [] configurationName;
	}

	if (encodeOptions)
		updateEncodeProperties(encodeOptions);
}

void MjpegEncoder::saveSettings(vidEncOptions *encodeOptions, MjpegEncoderOptions *options)
{
	options->setPresetConfiguration(&configName[0], (PluginConfigType)configType);

	encodeOptions->encodeMode = ADM_VIDENC_MODE_CQP;
	encodeOptions->encodeModeParameter = _quantiser;
}

bool changedConfig(const char* configName, ConfigMenuType configType)
{
	bool failure = false;

	if (configType == CONFIG_MENU_DEFAULT)
	{
		MjpegEncoderOptions defaultOptions;
		vidEncOptions *defaultEncodeOptions = defaultOptions.getEncodeOptions();

		encoder->loadSettings(defaultEncodeOptions, &defaultOptions);

		delete defaultEncodeOptions;
	}
	else
	{
		MjpegEncoderOptions options;

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
	MjpegEncoderOptions options;

	encoder->saveSettings(&encodeOptions, &options);
	options.setEncodeOptions(&encodeOptions);

	return options.toXml(PLUGIN_XML_EXTERNAL);
}

int MjpegEncoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
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

int MjpegEncoder::setOptions(vidEncOptions *encodeOptions, const char *pluginOptions)
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

int MjpegEncoder::beginPass(vidEncPassParameters *passParameters)
{
	int ret = AvcodecEncoder::beginPass(passParameters);

	_frame.quality = (int)floor(FF_QP2LAMBDA * _encodeOptions.encodeModeParameter + 0.5);

	return ret;
}

void MjpegEncoder::updateEncodeProperties(vidEncOptions *encodeOptions)
{
	_passCount = 1;
	_quantiser = encodeOptions->encodeModeParameter;
}
