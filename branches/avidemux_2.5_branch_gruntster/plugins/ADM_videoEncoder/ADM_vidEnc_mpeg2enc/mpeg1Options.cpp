 /***************************************************************************
                               Mpeg1Options.cpp

	These settings are intended for scripting and can contain a Preset 
	Configuration block.  If this block exists then "internal" settings are
	ignored and an external configuration file should be read instead, 
	e.g. PlayStation Portable profile.  However, if the external file is 
	missing, internal settings have to be used and will reflect a snapshot
	of the external configuration file at the time the script was generated.

    begin                : Tue Apr 6 2010
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
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <sstream>
#include <string>
#include "ADM_default.h"
#include "ADM_files.h"

#include "mpeg1Options.h"

Mpeg1Options::Mpeg1Options(void) : PluginOptions(MPEG1_PLUGIN_CONFIG_DIR, "Mpeg1", "mpeg2enc/Mpeg1Param.xsd", MPEG1_DEFAULT_ENCODE_MODE, MPEG1_DEFAULT_ENCODE_MODE_PARAMETER)
{
	reset();
}

void Mpeg1Options::reset(void)
{
	PluginOptions::reset();

	setFileSplit(790);
}

unsigned int Mpeg1Options::getFileSplit(void)
{
	return _fileSplit;
}

void Mpeg1Options::setFileSplit(unsigned int mb)
{
	if (mb >= 400 && mb <= 4096)
		_fileSplit = mb;
}

void Mpeg1Options::addOptionsToXml(xmlNodePtr xmlNodeRoot)
{
	const int bufferSize = 100;
	xmlChar xmlBuffer[bufferSize + 1];

	xmlNodeRoot = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)getOptionsTagRoot(), NULL);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"fileSplit", number2String(xmlBuffer, bufferSize, getFileSplit()));
}

void Mpeg1Options::parseOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "fileSplit") == 0)
				setFileSplit(atoi(content));

			xmlFree(content);
		}
	}
}
