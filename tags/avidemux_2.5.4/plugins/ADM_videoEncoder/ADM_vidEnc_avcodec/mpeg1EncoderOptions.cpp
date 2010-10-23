 /***************************************************************************
                               Mpeg1EncoderOptions.cpp

	These settings are intended for scripting and can contain a Preset 
	Configuration block.  If this block exists then "internal" settings are
	ignored and an external configuration file should be read instead, 
	e.g. PlayStation Portable profile.  However, if the external file is 
	missing, internal settings have to be used and will reflect a snapshot
	of the external configuration file at the time the script was generated.

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

#include <math.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <sstream>
#include <string>
#include "ADM_default.h"
#include "ADM_files.h"

#include "mpeg1EncoderOptions.h"

Mpeg1EncoderOptions::Mpeg1EncoderOptions(void) : PluginOptions(MPEG1_PLUGIN_CONFIG_DIR, "Mpeg1", "avcodec/Mpeg1Param.xsd", MPEG1_DEFAULT_ENCODE_MODE, MPEG1_DEFAULT_ENCODE_MODE_PARAMETER)
{
	reset();
}

void Mpeg1EncoderOptions::reset(void)
{
	PluginOptions::reset();

	setMinBitrate(600);
	setMaxBitrate(2200);
	setXvidRateControl(false);
	setBufferSize(40);
	setWidescreen(false);
	setInterlaced(MPEG1_INTERLACED_NONE);
	setMatrix(MPEG1_MATRIX_DEFAULT);
	setGopSize(12);
}

unsigned int Mpeg1EncoderOptions::getMinBitrate(void)
{
	return _minBitrate;
}

void Mpeg1EncoderOptions::setMinBitrate(unsigned int minBitrate)
{
	if (minBitrate <= 9000)
		_minBitrate = minBitrate;
}

unsigned int Mpeg1EncoderOptions::getMaxBitrate(void)
{
	return _maxBitrate;
}

void Mpeg1EncoderOptions::setMaxBitrate(unsigned int maxBitrate)
{
	if (maxBitrate >= 100 && maxBitrate <= 9000)
		_maxBitrate = maxBitrate;
}

bool Mpeg1EncoderOptions::getXvidRateControl(void)
{
	return _xvidRateControl;
}

void Mpeg1EncoderOptions::setXvidRateControl(bool xvidRateControl)
{
	_xvidRateControl = xvidRateControl;
}

unsigned int Mpeg1EncoderOptions::getBufferSize(void)
{
	return _bufferSize;
}

void Mpeg1EncoderOptions::setBufferSize(unsigned int bufferSize)
{
	if (bufferSize >= 1 && bufferSize <= 1024)
		_bufferSize = bufferSize;
}

bool Mpeg1EncoderOptions::getWidescreen(void)
{
	return _widescreen;
}

void Mpeg1EncoderOptions::setWidescreen(bool widescreen)
{
	_widescreen = widescreen;
}

Mpeg1InterlacedMode Mpeg1EncoderOptions::getInterlaced(void)
{
	return _interlaced;
}

void Mpeg1EncoderOptions::setInterlaced(Mpeg1InterlacedMode interlaced)
{
	if (interlaced == MPEG1_INTERLACED_NONE || interlaced == MPEG1_INTERLACED_BFF || interlaced == MPEG1_INTERLACED_TFF)
		_interlaced = interlaced;
}

Mpeg1MatrixMode Mpeg1EncoderOptions::getMatrix(void)
{
	return _matrix;
}

void Mpeg1EncoderOptions::setMatrix(Mpeg1MatrixMode matrix)
{
	if (matrix == MPEG1_MATRIX_DEFAULT || matrix == MPEG1_MATRIX_TMPGENC || matrix == MPEG1_MATRIX_ANIME || matrix == MPEG1_MATRIX_KVCD)
		_matrix = matrix;
}

unsigned int Mpeg1EncoderOptions::getGopSize(void)
{
	return _gopSize;
}

void Mpeg1EncoderOptions::setGopSize(unsigned int gopSize)
{
	if (gopSize >= 1 && gopSize <= 30)
		_gopSize = gopSize;
}

void Mpeg1EncoderOptions::addOptionsToXml(xmlNodePtr xmlNodeRoot)
{
	const int bufferSize = 100;
	xmlChar xmlBuffer[bufferSize + 1];

	xmlNodeRoot = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)getOptionsTagRoot(), NULL);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"minBitrate", number2String(xmlBuffer, bufferSize, getMinBitrate()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"maxBitrate", number2String(xmlBuffer, bufferSize, getMaxBitrate()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"xvidRateControl", boolean2String(xmlBuffer, bufferSize, getXvidRateControl()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"bufferSize", number2String(xmlBuffer, bufferSize, getBufferSize()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"widescreen", boolean2String(xmlBuffer, bufferSize, getWidescreen()));

	switch (getInterlaced())
	{
		case MPEG1_INTERLACED_BFF:
			strcpy((char*)xmlBuffer, "bff");
			break;
		case MPEG1_INTERLACED_TFF:
			strcpy((char*)xmlBuffer, "tff");
			break;
		default:
			strcpy((char*)xmlBuffer, "none");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"interlaced", xmlBuffer);

	switch (getMatrix())
	{
		case MPEG1_MATRIX_TMPGENC:
			strcpy((char*)xmlBuffer, "tmpgenc");
			break;
		case MPEG1_MATRIX_ANIME:
			strcpy((char*)xmlBuffer, "anime");
			break;
		case MPEG1_MATRIX_KVCD:
			strcpy((char*)xmlBuffer, "kvcd");
			break;
		default:
			strcpy((char*)xmlBuffer, "default");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"gopSize", number2String(xmlBuffer, bufferSize, getGopSize()));
}

void Mpeg1EncoderOptions::parseOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "minBitrate") == 0)
				setMinBitrate(atoi(content));
			else if (strcmp((char*)xmlChild->name, "maxBitrate") == 0)
				setMaxBitrate(atoi(content));
			else if (strcmp((char*)xmlChild->name, "xvidRateControl") == 0)
				setXvidRateControl(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "bufferSize") == 0)
				setBufferSize(atoi(content));
			else if (strcmp((char*)xmlChild->name, "widescreen") == 0)
				setWidescreen(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "interlaced") == 0)
			{
				Mpeg1InterlacedMode mode = MPEG1_INTERLACED_NONE;

				if (strcmp(content, "bff") == 0)
					mode = MPEG1_INTERLACED_BFF;
				else if (strcmp(content, "tff") == 0)
					mode = MPEG1_INTERLACED_TFF;

				setInterlaced(mode);
			}
			else if (strcmp((char*)xmlChild->name, "matrix") == 0)
			{
				Mpeg1MatrixMode mode = MPEG1_MATRIX_DEFAULT;

				if (strcmp(content, "tmpgenc") == 0)
					mode = MPEG1_MATRIX_TMPGENC;
				else if (strcmp(content, "anime") == 0)
					mode = MPEG1_MATRIX_ANIME;
				else if (strcmp(content, "kvcd") == 0)
					mode = MPEG1_MATRIX_KVCD;

				setMatrix(mode);
			}
			else if (strcmp((char*)xmlChild->name, "gopSize") == 0)
				setGopSize(atoi(content));

			xmlFree(content);
		}
	}
}
