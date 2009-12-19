/***************************************************************************
                            PluginXmlOptions.cpp

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
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

#include "PluginXmlOptions.h"
#include "ADM_files.h"

xmlChar* PluginXmlOptions::number2String(xmlChar *buffer, size_t size, int number)
{
	std::ostringstream stream;

	stream.imbue(std::locale::classic());
	stream << number;
	std::string string = stream.str();

	strncpy((char*)buffer, string.c_str(), size);

	return buffer;
}

xmlChar* PluginXmlOptions::number2String(xmlChar *buffer, size_t size, unsigned int number)
{
	std::ostringstream stream;

	stream.imbue(std::locale::classic());
	stream << number;
	std::string string = stream.str();

	strncpy((char*)buffer, string.c_str(), size);

	return buffer;
}

xmlChar* PluginXmlOptions::number2String(xmlChar *buffer, size_t size, float number)
{
	std::ostringstream stream;

	stream.imbue(std::locale::classic());
	stream << number;
	std::string string = stream.str();

	strncpy((char*)buffer, string.c_str(), size);

	return buffer;
}

xmlChar* PluginXmlOptions::boolean2String(xmlChar *buffer, size_t size, bool boolean)
{
	if (boolean)
		strncpy((char*)buffer, "true", size);
	else
		strncpy((char*)buffer, "false", size);

	return buffer;
}

bool PluginXmlOptions::string2Boolean(const char *buffer)
{
	return (strcmp(buffer, "true") == 0);
}

float PluginXmlOptions::string2Float(const char *buffer)
{
	std::istringstream stream(buffer);
	float value;

	stream >> value;

	return value;
}

char* PluginXmlOptions::dumpXmlDocToMemory(xmlDocPtr xmlDoc)
{
	xmlChar *tempBuffer;
	int tempBufferSize;
	char *xml = NULL;

	xmlDocDumpMemory(xmlDoc, &tempBuffer, &tempBufferSize);

	// remove carriage returns (even though libxml was instructed not to format the XML)
	xmlChar* bufferChar = tempBuffer;
	int bufferCharIndex = 0;

	while (*bufferChar != '\0')
	{
		if (*bufferChar == '\n')
		{
			memmove(bufferChar, bufferChar + 1, tempBufferSize - bufferCharIndex);
			tempBufferSize--;
		}
		else if (*bufferChar == '\"')
			*bufferChar = '\'';

		bufferChar++;
		bufferCharIndex++;
	}

	xml = new char[tempBufferSize + 1];
	memcpy(xml, tempBuffer, tempBufferSize);
	xml[tempBufferSize] = 0;

	return xml;
}

bool PluginXmlOptions::validateXml(xmlDocPtr doc, const char *schemaFile)
{
	char *pluginDir = ADM_getPluginPath();
	char schemaPath[strlen(pluginDir) + strlen(PLUGIN_SCHEMA_DIR) + 1 + strlen(schemaFile) + 1];
	bool success = false;

	strcpy(schemaPath, pluginDir);
	strcat(schemaPath, PLUGIN_SCHEMA_DIR);
	strcat(schemaPath, "/");
	strcat(schemaPath, schemaFile);
	delete [] pluginDir;

	xmlSchemaParserCtxtPtr xmlSchemaParserCtxt = xmlSchemaNewParserCtxt(schemaPath);
	xmlSchemaPtr xmlSchema = xmlSchemaParse(xmlSchemaParserCtxt);

 	xmlSchemaFreeParserCtxt(xmlSchemaParserCtxt);

 	xmlSchemaValidCtxtPtr xmlSchemaValidCtxt = xmlSchemaNewValidCtxt(xmlSchema);

 	if (xmlSchemaValidCtxt)
	{
 		success = !xmlSchemaValidateDoc(xmlSchemaValidCtxt, doc);
	 	xmlSchemaFree(xmlSchema);
		xmlSchemaFreeValidCtxt(xmlSchemaValidCtxt);
 	}
	else
 		xmlSchemaFree(xmlSchema);

	return success;
}
