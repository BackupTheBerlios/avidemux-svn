 /***************************************************************************
                            h263EncoderOptions.cpp

	These settings are intended for scripting and can contain a Preset 
	Configuration block.  If this block exists then "internal" settings are
	ignored and an external configuration file should be read instead, 
	e.g. PlayStation Portable profile.  However, if the external file is 
	missing, internal settings have to be used and will reflect a snapshot
	of the external configuration file at the time the script was generated.

    begin                : Wed Dec 30 2009
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

#include "h263EncoderOptions.h"

H263EncoderOptions::H263EncoderOptions(void) : PluginOptions(H263_PLUGIN_CONFIG_DIR, "H263", "avcodec/H263Param.xsd", H263_DEFAULT_ENCODE_MODE, H263_DEFAULT_ENCODE_MODE_PARAMETER)
{
	reset();
}

void H263EncoderOptions::reset(void)
{
	PluginOptions::reset();

	setMotionEstimationMethod(H263_MEM_EPZS);
	set4MotionVector(true);
	setMaxBFrames(0);
	setQuarterPixel(false);
	setGmc(false);
	setQuantisationType(H263_QUANT_H263);
	setMbDecisionMode(H263_MBDEC_RD);
	setMinQuantiser(2);
	setMaxQuantiser(31);
	setQuantiserDifference(3);
	setTrellis(false);
	setQuantiserCompression(0.5);
	setQuantiserBlur(0.5);
}

H263MotionEstimationMethod H263EncoderOptions::getMotionEstimationMethod(void)
{
	return _motionEst;
}

void H263EncoderOptions::setMotionEstimationMethod(H263MotionEstimationMethod motionEst)
{
	_motionEst = motionEst;
}

bool H263EncoderOptions::get4MotionVector(void)
{
	return _4MV;
}

void H263EncoderOptions::set4MotionVector(bool fourMv)
{
	_4MV = fourMv;
}

unsigned int H263EncoderOptions::getMaxBFrames(void)
{
	return _maxBFrames;
}

void H263EncoderOptions::setMaxBFrames(unsigned int maxBFrames)
{
	if (maxBFrames <= 32)
		_maxBFrames = maxBFrames;
}

bool H263EncoderOptions::getQuarterPixel(void)
{
	return _qpel;
}

void H263EncoderOptions::setQuarterPixel(bool qpel)
{
	_qpel = qpel;
}

bool H263EncoderOptions::getGmc(void)
{
	return _gmc;
}

void H263EncoderOptions::setGmc(bool gmc)
{
	_gmc = gmc;
}

H263QuantisationType H263EncoderOptions::getQuantisationType(void)
{
	return _quantType;
}

void H263EncoderOptions::setQuantisationType(H263QuantisationType quantType)
{
	_quantType = quantType;
}

H263MacroblockDecisionMode H263EncoderOptions::getMbDecisionMode(void)
{
	return _mbDecision;
}

void H263EncoderOptions::setMbDecisionMode(H263MacroblockDecisionMode mbDecision)
{
	_mbDecision = mbDecision;
}

unsigned int H263EncoderOptions::getMinQuantiser(void)
{
	return _minQuantiser;
}

void H263EncoderOptions::setMinQuantiser(unsigned int quantiser)
{
	if (quantiser > 0 && quantiser <= 31)
		_minQuantiser = quantiser;
}

unsigned int H263EncoderOptions::getMaxQuantiser(void)
{
	return _maxQuantiser;
}

void H263EncoderOptions::setMaxQuantiser(unsigned int quantiser)
{
	if (quantiser > 0 && quantiser <= 31)
		_maxQuantiser = quantiser;
}

unsigned int H263EncoderOptions::getQuantiserDifference(void)
{
	return _maxQuantiserDiff;
}

void H263EncoderOptions::setQuantiserDifference(unsigned int difference)
{
	if (difference > 0 && difference <= 31)
		_maxQuantiserDiff = difference;
}

bool H263EncoderOptions::getTrellis(void)
{
	return _trellis;
}

void H263EncoderOptions::setTrellis(bool trellis)
{
	_trellis = trellis;
}

float H263EncoderOptions::getQuantiserCompression(void)
{
	return _quantCompression;
}

void H263EncoderOptions::setQuantiserCompression(float compression)
{
	if (compression > 0. && compression <= 1.)
		_quantCompression = compression;
}

float H263EncoderOptions::getQuantiserBlur(void)
{
	return _quantBlur;
}

void H263EncoderOptions::setQuantiserBlur(float blur)
{
	if (blur > 0. && blur <= 1.)
		_quantBlur = blur;
}

void H263EncoderOptions::addOptionsToXml(xmlNodePtr xmlNodeRoot)
{
	const int bufferSize = 100;
	xmlChar xmlBuffer[bufferSize + 1];

	xmlNodeRoot = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)getOptionsTagRoot(), NULL);

	switch (getMotionEstimationMethod())
	{
		case H263_MEM_FULL:
			strcpy((char*)xmlBuffer, "full");
			break;
		case H263_MEM_LOG:
			strcpy((char*)xmlBuffer, "log");
			break;
		case H263_MEM_PHODS:
			strcpy((char*)xmlBuffer, "phods");
			break;
		case H263_MEM_EPZS:
			strcpy((char*)xmlBuffer, "epzs");
			break;
		default:
			strcpy((char*)xmlBuffer, "none");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"motionEstimationMethod", xmlBuffer);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"fourMotionVector", boolean2String(xmlBuffer, bufferSize, get4MotionVector()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"maximumBFrames", number2String(xmlBuffer, bufferSize, getMaxBFrames()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quarterPixel", boolean2String(xmlBuffer, bufferSize, getQuarterPixel()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"globalMotionCompensation", boolean2String(xmlBuffer, bufferSize, getGmc()));

	switch (getQuantisationType())
	{
		case H263_QUANT_MPEG:
			strcpy((char*)xmlBuffer, "mpeg");
			break;
		default:
			strcpy((char*)xmlBuffer, "h263");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantisationType", xmlBuffer);

	switch (getMbDecisionMode())
	{
		case H263_MBDEC_BITS:
			strcpy((char*)xmlBuffer, "fewestBits");
			break;
		case H263_MBDEC_RD:
			strcpy((char*)xmlBuffer, "rateDistortion");
			break;
		default:
			strcpy((char*)xmlBuffer, "sad");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"macroblockDecisionMode", xmlBuffer);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"minimumQuantiser", number2String(xmlBuffer, bufferSize, getMinQuantiser()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"maximumQuantiser", number2String(xmlBuffer, bufferSize, getMaxQuantiser()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantiserDifference", number2String(xmlBuffer, bufferSize, getQuantiserDifference()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"trellis", boolean2String(xmlBuffer, bufferSize, getTrellis()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantiserCompression", number2String(xmlBuffer, bufferSize, getQuantiserCompression()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantiserBlur", number2String(xmlBuffer, bufferSize, getQuantiserBlur()));
}

void H263EncoderOptions::parseOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "motionEstimationMethod") == 0)
			{
				H263MotionEstimationMethod method = H263_MEM_NONE;

				if (strcmp(content, "full") == 0)
					method = H263_MEM_FULL;
				else if (strcmp(content, "log") == 0)
					method = H263_MEM_LOG;
				else if (strcmp(content, "phods") == 0)
					method = H263_MEM_PHODS;
				else if (strcmp(content, "epzs") == 0)
					method = H263_MEM_EPZS;

				setMotionEstimationMethod(method);
			}
			if (strcmp((char*)xmlChild->name, "fourMotionVector") == 0)
				set4MotionVector(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "maximumBFrames") == 0)
				setMaxBFrames(atoi(content));
			else if (strcmp((char*)xmlChild->name, "quarterPixel") == 0)
				setQuarterPixel(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "globalMotionCompensation") == 0)
				setGmc(string2Boolean(content));
			if (strcmp((char*)xmlChild->name, "quantisationType") == 0)
			{
				H263QuantisationType type = H263_QUANT_H263;

				if (strcmp(content, "mpeg") == 0)
					type = H263_QUANT_MPEG;

				setQuantisationType(type);
			}			
			if (strcmp((char*)xmlChild->name, "macroblockDecisionMode") == 0)
			{
				H263MacroblockDecisionMode mode = H263_MBDEC_SAD;

				if (strcmp(content, "fewestBits") == 0)
					mode = H263_MBDEC_BITS;
				else if (strcmp(content, "rateDistortion") == 0)
					mode = H263_MBDEC_RD;

				setMbDecisionMode(mode);
			}
			else if (strcmp((char*)xmlChild->name, "minimumQuantiser") == 0)
				setMinQuantiser(atoi(content));
			else if (strcmp((char*)xmlChild->name, "maximumQuantiser") == 0)
				setMaxQuantiser(atoi(content));
			else if (strcmp((char*)xmlChild->name, "quantiserDifference") == 0)
				setQuantiserDifference(atoi(content));
			else if (strcmp((char*)xmlChild->name, "trellis") == 0)
				setTrellis(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "quantiserCompression") == 0)
				setQuantiserCompression(string2Float(content));
			else if (strcmp((char*)xmlChild->name, "quantiserBlur") == 0)
				setQuantiserBlur(string2Float(content));

			xmlFree(content);
		}
	}
}
