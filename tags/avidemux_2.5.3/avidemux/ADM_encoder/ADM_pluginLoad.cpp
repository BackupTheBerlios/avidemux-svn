/***************************************************************************
                             ADM_pluginLoad.cpp

    begin                : Mon Apr 14 2008
    copyright            : (C) 2008 by gruntster/mean
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "config.h"
#include "ADM_default.h"
#include <list>
#include <locale>

#include "DIA_fileSel.h"
#include "ADM_pluginLoad.h"
#include "ADM_vidEncode.hxx"

struct COMPRES_PARAMS *AllVideoCodec = NULL;
int AllVideoCodecCount = 0;

extern COMPRES_PARAMS *internalVideoCodec[];
extern int getInternalVideoCodecCount();

#if 1
#define aprintf printf
#else
#define aprintf(...) {}
#endif

ADM_vidEnc_plugin::ADM_vidEnc_plugin(const char *file) : ADM_LibWrapper()
{
	initialised = (loadLibrary(file) && getSymbols(21,
		&getEncoders, "vidEncGetEncoders",
		&getEncoderName, "vidEncGetEncoderName",
		&getEncoderType, "vidEncGetEncoderType",
		&getEncoderDescription, "vidEncGetEncoderDescription",
		&getFourCC, "vidEncGetFourCC",
		&getEncoderRequirements, "vidEncGetEncoderRequirements",
		&getEncoderApiVersion, "vidEncGetEncoderApiVersion",
		&getEncoderVersion, "vidEncGetEncoderVersion",
		&getEncoderGuid, "vidEncGetEncoderGuid",
		&isConfigurable, "vidEncIsConfigurable",
		&configure, "vidEncConfigure",
		&getOptions, "vidEncGetOptions",
		&setOptions, "vidEncSetOptions",
		&getPassCount, "vidEncGetPassCount",
		&getCurrentPass, "vidEncGetCurrentPass",
		&getEncoderGuid, "vidEncGetEncoderGuid",
		&open, "vidEncOpen",
		&beginPass, "vidEncBeginPass",
		&encodeFrame, "vidEncEncodeFrame",
		&finishPass, "vidEncFinishPass",
		&close, "vidEncClose"));
}

std::list<ADM_vidEnc_plugin *> ADM_videoEncoderPlugins;
/**
    \fn ADM_ve_getNbEncoders
    \brief get the number of encoder plugin loaded
    @return the number of encoder plugins
*/
uint32_t ADM_ve_getNbEncoders(void)
{
    return ADM_videoEncoderPlugins.size();

}
/**
     \fn ADM_ve_getEncoderInfo
     \brief Get info about an encoder plugin
     @param filter [in] Encoder index, between 0 and ADM_ve_getNbEncoders-1 included
     @param name [out] Name + info of the encoder
     @param major,minor,patch [out] Version number of the encoder
     @return true
*/
bool     ADM_ve_getEncoderInfo(int filter, const char **name, uint32_t *major,uint32_t *minor,uint32_t *patch)
{
	ADM_assert(filter >= 0 && filter < ADM_videoEncoderPlugins.size());

	ADM_vidEnc_plugin *plugin = getVideoEncoderPlugin(filter);
	int ma, mi, pa;

	plugin->getEncoderVersion(0, &ma, &mi, &pa);

	*name = plugin->getEncoderDescription(plugin->encoderId);
	*major = (uint32_t)ma;
	*minor = (uint32_t)mi;
	*patch = (uint32_t)pa;

	return true;
}

static int loadVideoEncoderPlugin(std::list<ADM_vidEnc_plugin*>* pluginList, int uiType, const char *file)
{
	ADM_vidEnc_plugin *plugin = new ADM_vidEnc_plugin(file);
	int* encoderIds;
	int encoderCount;
	bool success = false;

	if (plugin->isAvailable())
	{
		// Retrieve video encoders
		encoderCount = plugin->getEncoders(uiType, &encoderIds);

		for (int encoderIndex = 0; encoderIndex < encoderCount; encoderIndex++)
		{
			if (!plugin)
				plugin = new ADM_vidEnc_plugin(file);

			int encoderId = encoderIds[encoderIndex];
			int apiVersion = plugin->getEncoderApiVersion(encoderId);

			if (apiVersion == ADM_VIDENC_API_VERSION)
			{
				plugin->encoderId = encoderId;
				plugin->fileName = ADM_GetFileName(file);

				int major, minor, patch;

				plugin->getEncoderVersion(encoderId, &major, &minor, &patch);

				printf("[ADM_vidEnc_plugin] Plugin loaded version %d.%d.%d, filename %s, desc: %s\n", major, minor, patch, plugin->fileName, plugin->getEncoderDescription(encoderId));

				pluginList->push_back(plugin);

				plugin = NULL;
				success = true;
			}
			else
			{
				printf("[ADM_vidEnc_plugin] File %s has an outdated API version (%d vs %d)\n", ADM_GetFileName(file), apiVersion, ADM_VIDENC_API_VERSION);
				delete plugin;
			}
		}		
	}
	else
	{
		printf("[ADM_vidEnc_plugin] Unable to load %s\n", ADM_GetFileName(file));
		delete plugin;
	}

	return success;
}
/**
 *      \fn loadPlugins
 *      \brief load plugin
 */
extern ADM_UI_TYPE UI_GetCurrentUI(void); //FIXME
void loadPlugins(void)
{
        char *pluginDir = ADM_getPluginPath();

        loadVideoEncoderPlugins(UI_GetCurrentUI(), pluginDir);
        delete [] pluginDir;
}

static bool comparePlugins(ADM_vidEnc_plugin* plugin1, ADM_vidEnc_plugin* plugin2)
{
	int firstLen = strlen(plugin1->getEncoderType(plugin1->encoderId)) + strlen(plugin1->getEncoderName(plugin1->encoderId));
	int secondLen = strlen(plugin2->getEncoderType(plugin2->encoderId)) + strlen(plugin2->getEncoderName(plugin2->encoderId));
	char first[firstLen];
	char second[secondLen];
	int i = 0;

	strcpy(first, plugin1->getEncoderType(plugin1->encoderId));
	strcat(first, plugin1->getEncoderName(plugin1->encoderId));
	strcpy(second, plugin2->getEncoderType(plugin2->encoderId));
	strcat(second, plugin2->getEncoderName(plugin2->encoderId));

	while ((i < firstLen) && (i < secondLen))
	{
		if (tolower(first[i]) < tolower(second[i]))
			return true;
		else if (tolower(first[i]) > tolower(second[i]))
			return false;

		i++;
	}

	if (firstLen < secondLen)
		return true;
	else
		return false;
}

/**
 * 	\fn ADM_vidEnc_loadPlugins
 *  \brief load all audio plugins
 */
int loadVideoEncoderPlugins(int uiType, const char *path)
{
#define MAX_EXTERNAL_FILTER 50

#ifdef __WIN32
#define SHARED_LIB_EXT "dll"
#elif defined(__APPLE__)
#define SHARED_LIB_EXT "dylib"
#else
#define SHARED_LIB_EXT "so"
#endif

	char *files[MAX_EXTERNAL_FILTER];
	uint32_t nbFile = 0;

	memset(files, 0, sizeof(char *)*MAX_EXTERNAL_FILTER);
	printf("[ADM_vidEnc_plugin] Scanning directory %s\n", path);

	if (!buildDirectoryContent(&nbFile, path, files, MAX_EXTERNAL_FILTER, SHARED_LIB_EXT))
		printf("[ADM_vidEnc_plugin] Cannot parse plugin\n");

	for (int i = 0; i < nbFile; i++)
	{
		loadVideoEncoderPlugin(&ADM_videoEncoderPlugins, uiType, files[i]);
		ADM_dealloc(files[i]);
	}

	printf("[ADM_vidEnc_plugin] Scanning done, found %d codec\n", ADM_videoEncoderPlugins.size());

	AllVideoCodecCount = ADM_videoEncoderPlugins.size() + getInternalVideoCodecCount();
	AllVideoCodec = new COMPRES_PARAMS[AllVideoCodecCount];

	// Copy over internal codecs
	int internalCodecCount = getInternalVideoCodecCount();

	for (int i = 0; i < internalCodecCount; i++)
		memcpy(&AllVideoCodec[i], internalVideoCodec[i], sizeof(COMPRES_PARAMS));

	// Add external codecs
	int counter = 0;

	ADM_videoEncoderPlugins.sort(comparePlugins);

	for (std::list<ADM_vidEnc_plugin*>::iterator it = ADM_videoEncoderPlugins.begin(); it != ADM_videoEncoderPlugins.end(); it++)
	{
		ADM_vidEnc_plugin *plugin = *it;
		ADM_assert(plugin);

		const char* codecName = plugin->getEncoderName(plugin->encoderId);
		const char* codecType = plugin->getEncoderType(plugin->encoderId);
		char* displayName;
		const char *prevType, *nextType;

		if (it == ADM_videoEncoderPlugins.begin())
			prevType = NULL;
		else
		{
			*it--;
			prevType = (*it)->getEncoderType((*it)->encoderId);
			*it++;
		}

		*it++;

		if (it == ADM_videoEncoderPlugins.end())
			nextType = NULL;
		else
			nextType = (*it)->getEncoderType((*it)->encoderId);

		*it--;

		if ((prevType != NULL && strcmp(prevType, codecType) == 0) ||
			(nextType != NULL && strcmp(nextType, codecType) == 0))
		{
			displayName = new char[strlen(codecName) + strlen(codecType) + 4];
			sprintf(displayName, "%s (%s)", codecType, codecName);
		}
		else
		{
			displayName = new char[strlen(codecType) + 1];
			sprintf(displayName, "%s", codecType);
		}

		COMPRES_PARAMS *param = &AllVideoCodec[internalCodecCount + counter];

		param->codec = CodecExternal;
		param->menuName = displayName;
		param->tagName = codecName;
		param->extra_param = counter;
		param->extraSettings = NULL;
		param->extraSettingsLen = 0;

		int length = plugin->getOptions(plugin->encoderId, NULL, NULL, 0);
		char *pluginOptions = new char[length + 1];
		vidEncOptions encodeOptions;

		plugin->getOptions(plugin->encoderId, &encodeOptions, pluginOptions, length);
		pluginOptions[length] = 0;

		updateCompressionParameters(param, encodeOptions.encodeMode, encodeOptions.encodeModeParameter, pluginOptions, length);
		counter++;
	}

	return 1;
}

void updateCompressionParameters(COMPRES_PARAMS *params, int encodeMode, int encodeModeParameter, char *extraSettings, int extraSettingsLength)
{
	COMPRESSION_MODE compressMode = getCompressionMode(encodeMode);

	params->mode = compressMode;

	switch (compressMode)
	{
		case COMPRESS_CBR:
			params->bitrate = encodeModeParameter;
			break;
		case COMPRESS_AQ:
			params->qz = encodeModeParameter;
			break;
		case COMPRESS_CQ:
			params->qz = encodeModeParameter;
			break;
		case COMPRESS_2PASS:
			params->finalsize = encodeModeParameter;
			break;
		case COMPRESS_2PASS_BITRATE:
			params->avg_bitrate = encodeModeParameter;
			break;
		default:
			ADM_assert(0);
	}

	if (params->extraSettings)
		delete [] (char*)params->extraSettings;

	params->extraSettings = extraSettings;
	params->extraSettingsLen = extraSettingsLength;
}

COMPRESSION_MODE getCompressionMode(int encodeMode)
{
	COMPRESSION_MODE mode;

	switch (encodeMode)
	{
		case ADM_VIDENC_MODE_AQP:
			mode = COMPRESS_AQ;
			break;
		case ADM_VIDENC_MODE_CQP:
			mode = COMPRESS_CQ;
			break;
		case ADM_VIDENC_MODE_CBR:
			mode = COMPRESS_CBR;
			break;
		case ADM_VIDENC_MODE_2PASS_SIZE:
			mode = COMPRESS_2PASS;
			break;
		case ADM_VIDENC_MODE_2PASS_ABR:
			mode = COMPRESS_2PASS_BITRATE;
			break;
		default:
			ADM_assert(0);
	}

	return mode;
}

int getVideoEncodePluginMode(COMPRESSION_MODE mode)
{
	int encodeMode;

	switch (mode)
	{
		case COMPRESS_AQ:
			encodeMode = ADM_VIDENC_MODE_AQP;
			break;
		case COMPRESS_CQ:
			encodeMode = ADM_VIDENC_MODE_CQP;
			break;
		case COMPRESS_CBR:
			encodeMode = ADM_VIDENC_MODE_CBR;
			break;
		case COMPRESS_2PASS:
			encodeMode = ADM_VIDENC_MODE_2PASS_SIZE;
			break;
		case COMPRESS_2PASS_BITRATE:
			encodeMode = ADM_VIDENC_MODE_2PASS_ABR;
			break;
		default:
			ADM_assert(0);
	}

	return encodeMode;
}

ADM_vidEnc_plugin* getVideoEncoderPlugin(int index)
{
	ADM_assert(index < ADM_videoEncoderPlugins.size());

	std::list<ADM_vidEnc_plugin*>::iterator it = ADM_videoEncoderPlugins.begin();

	if (index > 0)
		advance(it, index);

	return *it;
}
