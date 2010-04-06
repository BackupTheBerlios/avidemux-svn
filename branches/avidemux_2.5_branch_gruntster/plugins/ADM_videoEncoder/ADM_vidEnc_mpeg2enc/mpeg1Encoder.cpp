 /***************************************************************************
                               mpeg1Encoder.cpp

    begin                : Mon Apr 5 2010
    copyright            : (C) 2010 by gruntster
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
#include "mpeg1Encoder.h"
#include "format_codes.h"

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

	_passCount = 1;
	_encodeOptions.encodeMode = MPEG1_DEFAULT_ENCODE_MODE;
	_encodeOptions.encodeModeParameter = MPEG1_DEFAULT_ENCODE_MODE_PARAMETER;
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
	return "056FE919-C1D3-4450-A812-A767EAB07786";
}

int Mpeg1Encoder::isConfigurable(void)
{
	return (_uiType == ADM_UI_GTK || _uiType == ADM_UI_QT4);
}

int Mpeg1Encoder::configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	loadSettings(NULL, &_options);

	diaElemUInteger ctlSplitFile(&_splitFile, "New sequence every (MB):", 400, 4096);
	diaElem *elmGeneral[1] = {&ctlSplitFile};

	diaElemConfigMenu ctlConfigMenu(configName, &configType, _options.getUserConfigDirectory(), _options.getSystemConfigDirectory(),
		changedConfig, serializeConfig, elmGeneral, 1);
	diaElem *elmHeader[1] = {&ctlConfigMenu};

	diaElemTabs tabGeneral("Settings", 1, elmGeneral);
	diaElemTabs *tabs[] = {&tabGeneral};

	if (diaFactoryRunTabs("mpeg2enc Configuration", 1, elmHeader, 1, tabs))
	{
		saveSettings(NULL, &_options);

		return 1;
	}

	return 0;
}

void Mpeg1Encoder::loadSettings(vidEncOptions *encodeOptions, Mpeg1Options *options)
{
	char *configurationName;

	options->getPresetConfiguration(&configurationName, (PluginConfigType*)&configType);

	if (configurationName)
	{
		strcpy(this->configName, configurationName);
		delete [] configurationName;
	}

	if ((intptr_t)encodeOptions != -1)
		_splitFile = options->getFileSplit();
}

void Mpeg1Encoder::saveSettings(vidEncOptions *encodeOptions, Mpeg1Options *options)
{
	options->setPresetConfiguration(&configName[0], (PluginConfigType)configType);
	options->setFileSplit(_splitFile);
}

bool changedConfig(const char* configName, ConfigMenuType configType)
{
	if (configType == CONFIG_MENU_DEFAULT)
	{
		Mpeg1Options defaultOptions;

		encoder->loadSettings(NULL, &defaultOptions);
	}
	else
	{
		Mpeg1Options options;

		options.setPresetConfiguration(configName, (PluginConfigType)configType);

		if (configType == CONFIG_MENU_CUSTOM)
			encoder->loadSettings((vidEncOptions*)-1, &options);
		else
			encoder->loadSettings(NULL, &options);
	}

	return true;
}

char *serializeConfig(void)
{
	Mpeg1Options options;

	encoder->saveSettings(NULL, &options);

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

int Mpeg1Encoder::setOptions(vidEncOptions *encodeOptions, const char *pluginOptions)
{
	if (_opened)
		return ADM_VIDENC_ERR_ALREADY_OPEN;

	bool success = true;

	if (pluginOptions)
	{
		success = _options.fromXml(pluginOptions, PLUGIN_XML_INTERNAL);

		_options.loadPresetConfiguration();
	}

	if (success)
		return ADM_VIDENC_ERR_SUCCESS;
	else
		return ADM_VIDENC_ERR_FAILED;
}

void Mpeg1Encoder::initParameters(int *encodeModeParameter, int *maxBitrate, int *vbv)
{
	_param.fieldenc = 0;
	_param.format = MPEG_FORMAT_VCD;
	_param.aspect_ratio = 2;
	_param.min_GOP_size = _param.max_GOP_size = 18;
	_param.seq_length_limit = _options.getFileSplit();

	*encodeModeParameter = 1000;
	*maxBitrate = 9216000;
	*vbv = 0;
}
