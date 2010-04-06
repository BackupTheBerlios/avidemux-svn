 /***************************************************************************
                               flv1EncoderOptions.cpp

	These settings are intended for scripting and can contain a Preset 
	Configuration block.  If this block exists then "internal" settings are
	ignored and an external configuration file should be read instead, 
	e.g. PlayStation Portable profile.  However, if the external file is 
	missing, internal settings have to be used and will reflect a snapshot
	of the external configuration file at the time the script was generated.

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

#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <sstream>
#include <string>
#include "ADM_default.h"

#include "flv1EncoderOptions.h"

FLV1EncoderOptions::FLV1EncoderOptions(void) : PluginOptions(FLV1_PLUGIN_CONFIG_DIR, "Flv1", "avcodec/Flv1Param.xsd", FLV1_DEFAULT_ENCODE_MODE, FLV1_DEFAULT_ENCODE_MODE_PARAMETER)
{
	reset();
}

void FLV1EncoderOptions::reset(void)
{
	PluginOptions::reset();

	setGopSize(100);
}

unsigned int FLV1EncoderOptions::getGopSize(void)
{
	return _gopSize;
}

void FLV1EncoderOptions::setGopSize(unsigned int gopSize)
{
	if (gopSize >= 1 && gopSize <= 250)
		_gopSize = gopSize;
}

void FLV1EncoderOptions::addOptionsToXml(xmlNodePtr xmlNodeRoot)
{
	const int bufferSize = 100;
	xmlChar xmlBuffer[bufferSize + 1];

	xmlNodeRoot = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)getOptionsTagRoot(), NULL);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"gopSize", number2String(xmlBuffer, bufferSize, getGopSize()));
}

void FLV1EncoderOptions::parseOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "gopSize") == 0)
				setGopSize(atoi(content));

			xmlFree(content);
		}
	}
}
