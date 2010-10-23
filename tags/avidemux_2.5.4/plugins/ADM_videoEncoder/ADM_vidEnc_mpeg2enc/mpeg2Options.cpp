 /***************************************************************************
                               Mpeg2Options.cpp

	These settings are intended for scripting and can contain a Preset 
	Configuration block.  If this block exists then "internal" settings are
	ignored and an external configuration file should be read instead, 
	e.g. PlayStation Portable profile.  However, if the external file is 
	missing, internal settings have to be used and will reflect a snapshot
	of the external configuration file at the time the script was generated.

    begin                : Thu Dec 31 2009
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

#include <math.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <sstream>
#include <string>
#include "ADM_default.h"
#include "ADM_files.h"

#include "mpeg2Options.h"

Mpeg2Options::Mpeg2Options(void) : PluginOptions(MPEG2_PLUGIN_CONFIG_DIR, "Mpeg2", "mpeg2enc/Mpeg2Param.xsd", MPEG2_DEFAULT_ENCODE_MODE, MPEG2_DEFAULT_ENCODE_MODE_PARAMETER)
{
	reset();
}

void Mpeg2Options::reset(void)
{
	PluginOptions::reset();

	setMaxBitrate(9000);
	setFileSplit(4096);
	setStreamType(MPEG2_STREAMTYPE_DVD);
	setWidescreen(false);
	setInterlaced(MPEG2_INTERLACED_NONE);
	setMatrix(MPEG2_MATRIX_DEFAULT);
}

unsigned int Mpeg2Options::getMaxBitrate(void)
{
	return _maxBitrate;
}

void Mpeg2Options::setMaxBitrate(unsigned int maxBitrate)
{
	if (maxBitrate >= 100 && maxBitrate <= 9000)
		_maxBitrate = maxBitrate;
}

unsigned int Mpeg2Options::getFileSplit(void)
{
	return _fileSplit;
}

void Mpeg2Options::setFileSplit(unsigned int mb)
{
	if (mb >= 400 && mb <= 4096)
		_fileSplit = mb;
}

bool Mpeg2Options::getWidescreen(void)
{
	return _widescreen;
}

void Mpeg2Options::setWidescreen(bool widescreen)
{
	_widescreen = widescreen;
}

Mpeg2InterlacedMode Mpeg2Options::getInterlaced(void)
{
	return _interlaced;
}

void Mpeg2Options::setInterlaced(Mpeg2InterlacedMode interlaced)
{
	if (interlaced == MPEG2_INTERLACED_NONE || interlaced == MPEG2_INTERLACED_BFF || interlaced == MPEG2_INTERLACED_TFF)
		_interlaced = interlaced;
}

Mpeg2MatrixMode Mpeg2Options::getMatrix(void)
{
	return _matrix;
}

void Mpeg2Options::setMatrix(Mpeg2MatrixMode matrix)
{
	if (matrix == MPEG2_MATRIX_DEFAULT || matrix == MPEG2_MATRIX_TMPGENC || matrix == MPEG2_MATRIX_KVCD)
		_matrix = matrix;
}

Mpeg2StreamTypeMode Mpeg2Options::getStreamType(void)
{
	return _streamType;
}

void Mpeg2Options::setStreamType(Mpeg2StreamTypeMode streamType)
{
	if (streamType == MPEG2_STREAMTYPE_DVD || streamType == MPEG2_STREAMTYPE_SVCD)
		_streamType = streamType;
}

void Mpeg2Options::addOptionsToXml(xmlNodePtr xmlNodeRoot)
{
	const int bufferSize = 100;
	xmlChar xmlBuffer[bufferSize + 1];

	xmlNodeRoot = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)getOptionsTagRoot(), NULL);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"maxBitrate", number2String(xmlBuffer, bufferSize, getMaxBitrate()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"fileSplit", number2String(xmlBuffer, bufferSize, getFileSplit()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"widescreen", boolean2String(xmlBuffer, bufferSize, getWidescreen()));

	switch (getStreamType())
	{
		case MPEG2_STREAMTYPE_SVCD:
			strcpy((char*)xmlBuffer, "svcd");
			break;
		default:
			strcpy((char*)xmlBuffer, "dvd");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"streamType", xmlBuffer);

	switch (getInterlaced())
	{
		case MPEG2_INTERLACED_BFF:
			strcpy((char*)xmlBuffer, "bff");
			break;
		case MPEG2_INTERLACED_TFF:
			strcpy((char*)xmlBuffer, "tff");
			break;
		default:
			strcpy((char*)xmlBuffer, "none");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"interlaced", xmlBuffer);

	switch (getMatrix())
	{
		case MPEG2_MATRIX_TMPGENC:
			strcpy((char*)xmlBuffer, "tmpgenc");
			break;
		case MPEG2_MATRIX_KVCD:
			strcpy((char*)xmlBuffer, "kvcd");
			break;
		default:
			strcpy((char*)xmlBuffer, "default");
			break;
	}
}

void Mpeg2Options::parseOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "maxBitrate") == 0)
				setMaxBitrate(atoi(content));
			else if (strcmp((char*)xmlChild->name, "fileSplit") == 0)
				setFileSplit(atoi(content));
			else if (strcmp((char*)xmlChild->name, "widescreen") == 0)
				setWidescreen(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "streamType") == 0)
			{
				Mpeg2StreamTypeMode mode = MPEG2_STREAMTYPE_DVD;

				if (strcmp(content, "svcd") == 0)
					mode = MPEG2_STREAMTYPE_SVCD;

				setStreamType(mode);
			}
			else if (strcmp((char*)xmlChild->name, "interlaced") == 0)
			{
				Mpeg2InterlacedMode mode = MPEG2_INTERLACED_NONE;

				if (strcmp(content, "bff") == 0)
					mode = MPEG2_INTERLACED_BFF;
				else if (strcmp(content, "tff") == 0)
					mode = MPEG2_INTERLACED_TFF;

				setInterlaced(mode);
			}
			else if (strcmp((char*)xmlChild->name, "matrix") == 0)
			{
				Mpeg2MatrixMode mode = MPEG2_MATRIX_DEFAULT;

				if (strcmp(content, "tmpgenc") == 0)
					mode = MPEG2_MATRIX_TMPGENC;
				else if (strcmp(content, "kvcd") == 0)
					mode = MPEG2_MATRIX_KVCD;

				setMatrix(mode);
			}

			xmlFree(content);
		}
	}
}
