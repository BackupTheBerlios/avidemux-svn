/***************************************************************************
                              PluginOptions.h

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

#ifndef PluginOptions_h
#define PluginOptions_h


extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

#include "PluginXmlOptions.h"

typedef enum
{
	PLUGIN_CONFIG_CUSTOM,
	PLUGIN_CONFIG_DEFAULT,
	PLUGIN_CONFIG_USER,
	PLUGIN_CONFIG_SYSTEM
} PluginConfigType;

class PluginOptions : public PluginXmlOptions
{
protected:
	vidEncOptions _encodeOptions;
	unsigned int _defaultEncodeMode;
	int _defaultEncodeModeParameter;

	char *_tagPrefix, *_configTagRoot, *_optionsTagRoot;
	char *_configurationDirectory, *_schemaFile;
	char *_configurationName;
	PluginConfigType _configurationType;

	virtual void cleanUp(void);
	virtual void setEncodeOptionsToDefaults(void);
	virtual void parseEncodeOptions(xmlNode *node, vidEncOptions *encodeOptions);
	virtual void parsePresetConfiguration(xmlNode *node);

public:
	PluginOptions(const char* configurationDirectory, const char* tagPrefix, const char* schemaFile, unsigned int defaultEncodeMode, int defaultEncodeModeParameter);
	~PluginOptions(void);

	virtual vidEncOptions* getEncodeOptions(void);
	virtual void setEncodeOptions(vidEncOptions* encodeOptions);

	virtual void reset(void);
	virtual const char* getSchemaFile();
	virtual const char* getConfigTagRoot();
	virtual const char* getOptionsTagRoot();

	virtual void getPresetConfiguration(char** configurationName, PluginConfigType *configurationType);
	virtual void setPresetConfiguration(const char* configurationName, PluginConfigType configurationType);
	virtual void clearPresetConfiguration(void);
	virtual bool loadPresetConfiguration(void);

	virtual void addOptionsToXml(xmlNodePtr xmlNodeRoot) = 0;
	virtual void parseOptions(xmlNode *node) = 0;

	virtual char* toXml(PluginXmlType xmlType);
	virtual int fromXml(const char *xml, PluginXmlType xmlType);

	virtual char* getUserConfigDirectory(void);
	virtual char* getSystemConfigDirectory(void);
};

#endif	// options_h
