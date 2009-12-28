 /***************************************************************************
                              PluginOptions.cpp

    begin                : Mon Apr 21 2008
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

#include <sstream>
#include <string>

#include "ADM_inttype.h"
#include "ADM_files.h"
#include "PluginXmlOptions.cpp"
#include "PluginOptions.h"

#undef malloc
#undef free
#undef strdup
PluginOptions::PluginOptions(const char* configurationDirectory, const char* tagPrefix, const char* schemaFile, 
							 unsigned int defaultEncodeMode, int defaultEncodeModeParameter)
{
	_configurationDirectory = new char[strlen(configurationDirectory) + 1];
	strcpy(_configurationDirectory, configurationDirectory);

	_tagPrefix = new char[strlen(tagPrefix) + 1];
	strcpy(_tagPrefix, tagPrefix);

	_schemaFile = new char[strlen(schemaFile) + 1];
	strcpy(_schemaFile, schemaFile);

	_configTagRoot = new char[strlen(tagPrefix) + 7];
	strcpy(_configTagRoot, tagPrefix);
	strcat(_configTagRoot, "Config");

	_optionsTagRoot = new char[strlen(tagPrefix) + 8];
	strcpy(_optionsTagRoot, tagPrefix);
	strcat(_optionsTagRoot, "Options");

	_configurationName = NULL;
	_defaultEncodeMode = defaultEncodeMode;
	_defaultEncodeModeParameter = defaultEncodeModeParameter;
	setEncodeOptionsToDefaults();

	reset();
}

PluginOptions::~PluginOptions(void)
{
	cleanUp();

	if (_configurationDirectory)
	{
		delete [] _configurationDirectory;
		_configurationDirectory = NULL;
	}

	if (_tagPrefix)
	{
		delete [] _tagPrefix;
		_tagPrefix = NULL;
	}

	if (_schemaFile)
	{
		delete [] _schemaFile;
		_schemaFile = NULL;
	}

	if (_configTagRoot)
	{
		delete [] _configTagRoot;
		_configTagRoot = NULL;
	}

	if (_optionsTagRoot)
	{
		delete [] _optionsTagRoot;
		_optionsTagRoot = NULL;
	}
}

void PluginOptions::cleanUp(void)
{
	if (_configurationName)
	{
		free(_configurationName);
		_configurationName = NULL;
	}
}

void PluginOptions::reset(void)
{
	cleanUp();

	setPresetConfiguration("<default>", PLUGIN_CONFIG_DEFAULT);
}

const char* PluginOptions::getSchemaFile()
{
	return _schemaFile;
}

const char* PluginOptions::getConfigTagRoot()
{
	return _configTagRoot;
}

const char* PluginOptions::getOptionsTagRoot()
{
	return _optionsTagRoot;
}

void PluginOptions::getPresetConfiguration(char** configurationName, PluginConfigType *configurationType)
{
	if (_configurationName)
	{
		*configurationName = new char[strlen(_configurationName) + 1];
		strcpy(*configurationName, _configurationName);
	}
	else
		*configurationName = NULL;

	*configurationType = _configurationType;
}

void PluginOptions::setPresetConfiguration(const char* configurationName, PluginConfigType configurationType)
{
	clearPresetConfiguration();

	_configurationName = strdup(configurationName);
	_configurationType = configurationType;
}

void PluginOptions::clearPresetConfiguration(void)
{
	if (_configurationName)
		free(_configurationName);

	_configurationName = strdup("<custom>");
	_configurationType = PLUGIN_CONFIG_CUSTOM;
}

bool PluginOptions::loadPresetConfiguration(void)
{
	char *dir = NULL;
	bool success = false;
	PluginConfigType configurationType = _configurationType;
	char configurationName[strlen(_configurationName) + 1];

	strcpy(configurationName, _configurationName);

	if (configurationType == PLUGIN_CONFIG_USER)
		dir = getUserConfigDirectory();
	else if (configurationType == PLUGIN_CONFIG_SYSTEM)
		dir = getSystemConfigDirectory();

	if (dir)
	{
		char presetConfigFile[strlen(dir) + strlen(configurationName) + 6];

		strcpy(presetConfigFile, dir);
		strcat(presetConfigFile, "/");
		strcat(presetConfigFile, configurationName);
		strcat(presetConfigFile, ".xml");

		delete [] dir;
		FILE *file = fopen(presetConfigFile, "r");

		if (file)
		{
			fseek(file, 0, SEEK_END);

			long size = ftell(file);
			char presetXml[size];

			fseek(file, 0, SEEK_SET);
			size = fread(presetXml, 1, size, file);
			presetXml[size] = 0;
			fclose(file);

			success = fromXml(presetXml, PLUGIN_XML_EXTERNAL);
			setPresetConfiguration(configurationName, configurationType);
		}
		else
			printf("Error - Unable to open or read %s\n", presetConfigFile);
	}

	return success;
}

void PluginOptions::setEncodeOptionsToDefaults(void)
{
	_encodeOptions.encodeMode = _defaultEncodeMode;
	_encodeOptions.encodeModeParameter = _defaultEncodeModeParameter;
}

vidEncOptions* PluginOptions::getEncodeOptions(void)
{
	vidEncOptions *encodeOptions = new vidEncOptions;

	memcpy(encodeOptions, &_encodeOptions, sizeof(vidEncOptions));

	return encodeOptions;
}

void PluginOptions::setEncodeOptions(vidEncOptions* encodeOptions)
{
	memcpy(&_encodeOptions, encodeOptions, sizeof(vidEncOptions));
}

char* PluginOptions::toXml(PluginXmlType xmlType)
{
	xmlDocPtr xmlDoc = xmlNewDoc((const xmlChar*)"1.0");
	xmlNodePtr xmlNodeRoot, xmlNodeChild;
	const int bufferSize = 100;
	xmlChar xmlBuffer[bufferSize + 1];
	char *xml = NULL;

	while (true)
	{
		if (!xmlDoc)
			break;

		if (!(xmlNodeRoot = xmlNewDocNode(xmlDoc, NULL, (xmlChar*)getConfigTagRoot(), NULL)))
			break;

		xmlDocSetRootElement(xmlDoc, xmlNodeRoot);

		if (xmlType == PLUGIN_XML_INTERNAL)
		{
			if (_configurationType != PLUGIN_CONFIG_CUSTOM)
			{
				xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"presetConfiguration", NULL);

				xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"name", (xmlChar*)_configurationName);

				if (_configurationType == PLUGIN_CONFIG_USER)
					strcpy((char*)xmlBuffer, "user");
				else if (_configurationType == PLUGIN_CONFIG_SYSTEM)
					strcpy((char*)xmlBuffer, "system");
				else
					strcpy((char*)xmlBuffer, "default");

				xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"type", xmlBuffer);
			}
		}
		else
		{
			xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"encodeOptions", NULL);

			switch (_encodeOptions.encodeMode)
			{
			case ADM_VIDENC_MODE_CBR:
				strcpy((char*)xmlBuffer, "CBR");
				break;
			case ADM_VIDENC_MODE_CQP:
				strcpy((char*)xmlBuffer, "CQP");
				break;
			case ADM_VIDENC_MODE_AQP:
				strcpy((char*)xmlBuffer, "AQP");
				break;
			case ADM_VIDENC_MODE_2PASS_SIZE:
				strcpy((char*)xmlBuffer, "2PASS SIZE");
				break;
			case ADM_VIDENC_MODE_2PASS_ABR:
				strcpy((char*)xmlBuffer, "2PASS ABR");
				break;
			}

			xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"mode", xmlBuffer);
			xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"parameter", number2String(xmlBuffer, bufferSize, _encodeOptions.encodeModeParameter));
		}

		addOptionsToXml(xmlNodeRoot);
		xml = dumpXmlDocToMemory(xmlDoc);
		xmlFreeDoc(xmlDoc);

		break;
	}

	return xml;
}

int PluginOptions::fromXml(const char *xml, PluginXmlType xmlType)
{
	clearPresetConfiguration();

	bool success = false;

	xmlDocPtr doc = xmlReadMemory(xml, strlen(xml), "options.xml", NULL, 0);

	if (success = validateXml(doc, getSchemaFile()))
	{
		xmlNode *xmlNodeRoot = xmlDocGetRootElement(doc);

		for (xmlNode *xmlChild = xmlNodeRoot->children; xmlChild; xmlChild = xmlChild->next)
		{
			if (xmlChild->type == XML_ELEMENT_NODE)
			{
				char *content = (char*)xmlNodeGetContent(xmlChild);

				if (xmlType == PLUGIN_XML_EXTERNAL && strcmp((char*)xmlChild->name, "encodeOptions") == 0)
					parseEncodeOptions(xmlChild, &_encodeOptions);
				else if (xmlType == PLUGIN_XML_INTERNAL && strcmp((char*)xmlChild->name, "presetConfiguration") == 0)
					parsePresetConfiguration(xmlChild);
				else if (strcmp((char*)xmlChild->name, getOptionsTagRoot()) == 0)
					parseOptions(xmlChild);

				xmlFree(content);
			}
		}
	}

	xmlFreeDoc(doc);

	return success;
}

void PluginOptions::parseEncodeOptions(xmlNode *node, vidEncOptions *encodeOptions)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "mode") == 0)
			{
				if (strcmp(content, "CBR") == 0)
					encodeOptions->encodeMode = ADM_VIDENC_MODE_CBR;
				else if (strcmp(content, "CQP") == 0)
					encodeOptions->encodeMode = ADM_VIDENC_MODE_CQP;
				else if (strcmp(content, "AQP") == 0)
					encodeOptions->encodeMode = ADM_VIDENC_MODE_AQP;
				else if (strcmp(content, "2PASS SIZE") == 0)
					encodeOptions->encodeMode = ADM_VIDENC_MODE_2PASS_SIZE;
				else if (strcmp(content, "2PASS ABR") == 0)
					encodeOptions->encodeMode = ADM_VIDENC_MODE_2PASS_ABR;
			}
			else if (strcmp((char*)xmlChild->name, "parameter") == 0)
				encodeOptions->encodeModeParameter = atoi(content);

			xmlFree(content);
		}
	}
}

void PluginOptions::parsePresetConfiguration(xmlNode *node)
{
	char* name = NULL;
	PluginConfigType type = PLUGIN_CONFIG_CUSTOM;

	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "name") == 0)
				name = strdup((char*)content);
			else if (strcmp((char*)xmlChild->name, "type") == 0)
			{
				if (strcmp(content, "user") == 0)
					type = PLUGIN_CONFIG_USER;
				else if (strcmp(content, "system") == 0)
					type = PLUGIN_CONFIG_SYSTEM;
				else
					type = PLUGIN_CONFIG_DEFAULT;
			}

			xmlFree(content);
		}
	}

	setPresetConfiguration(name, type);
	free(name);
}

char* PluginOptions::getUserConfigDirectory(void)
{
	return ADM_getHomeRelativePath(_configurationDirectory);
}

char* PluginOptions::getSystemConfigDirectory(void)
{
	char* pluginPath = ADM_getPluginPath();
	char* systemConfigPath = new char[strlen(pluginPath) + strlen(_configurationDirectory) + 2];

	strcpy(systemConfigPath, pluginPath);
	strcat(systemConfigPath, "/");
	strcat(systemConfigPath, _configurationDirectory);

	delete [] pluginPath;

	return systemConfigPath;
}
