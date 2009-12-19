/***************************************************************************
                             PluginXmlOptions.h

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

#ifndef PluginXmlOptions_h
#define PluginXmlOptions_h


typedef enum
{
	PLUGIN_XML_INTERNAL,
	PLUGIN_XML_EXTERNAL,
} PluginXmlType;

class PluginXmlOptions
{
protected:
	xmlChar* number2String(xmlChar *buffer, size_t size, int number);
	xmlChar* number2String(xmlChar *buffer, size_t size, unsigned int number);
	xmlChar* number2String(xmlChar *buffer, size_t size, float number);
	xmlChar* boolean2String(xmlChar *buffer, size_t size, bool boolean);
	
	bool string2Boolean(const char *buffer);
	float string2Float(const char *buffer);

	virtual char* dumpXmlDocToMemory(xmlDocPtr xmlDoc);
	virtual bool validateXml(xmlDocPtr doc, const char* schemaFile);

public:
	virtual char* toXml(PluginXmlType xmlType) = 0;
	virtual int fromXml(const char *xml, PluginXmlType xmlType) = 0;
};

#endif	// PluginXmlOptions_h
