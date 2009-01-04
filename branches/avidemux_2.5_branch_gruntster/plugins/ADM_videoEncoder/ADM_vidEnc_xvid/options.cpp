 /***************************************************************************
                               XvidOptions.cpp

	These settings are intended for scripting and can contain a Preset 
	Configuration block.  If this block exists then "internal" settings are
	ignored and an external configuration file should be read instead, 
	e.g. PlayStation Portable profile.  However, if the external file is 
	missing, internal settings have to be used and will reflect a snapshot
	of the external configuration file at the time the script was generated.

    begin                : Fri Jun 13 2008
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

#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

#include "config.h"
#include "ADM_inttype.h"
#include "ADM_files.h"
#include "options.h"

XvidOptions::XvidOptions(void)
{
	_configurationName = NULL;

	reset();
}

XvidOptions::~XvidOptions(void)
{
	cleanUp();
}

void XvidOptions::cleanUp(void)
{
	if (_configurationName)
	{
		free(_configurationName);
		_configurationName = NULL;
	}
}

void XvidOptions::reset(void)
{
	cleanUp();

	memset(&xvid_enc_create, 0, sizeof(xvid_enc_create_t));
	memset(&xvid_enc_frame, 0, sizeof(xvid_enc_frame_t));

	xvid_enc_frame.vop_flags = XVID_VOP_HALFPEL | XVID_VOP_HQACPRED;

	setPar(1, 1);
	setMotionEstimation(ME_LOW);
	setRateDistortion(RD_DCT_ME);
	setMinQuantiser(2, 2, 2);
	setMaxQuantiser(31, 31, 31);

	_parAsInput = false;

	setPresetConfiguration("<default>", CONFIG_DEFAULT);
}

void XvidOptions::getPresetConfiguration(char** configurationName, ConfigType *configurationType)
{
	if (_configurationName)
		*configurationName = strdup(_configurationName);
	else
		*configurationName = NULL;

	*configurationType = _configurationType;
}

void XvidOptions::setPresetConfiguration(const char* configurationName, ConfigType configurationType)
{
	clearPresetConfiguration();

	_configurationName = strdup(configurationName);
	_configurationType = configurationType;
}

void XvidOptions::clearPresetConfiguration(void)
{
	if (_configurationName)
		free(_configurationName);

	_configurationName = strdup("<custom>");
	_configurationType = CONFIG_CUSTOM;
}

unsigned int XvidOptions::getThreads(void)
{
	return xvid_enc_create.num_threads;
}

void XvidOptions::setThreads(unsigned int threads)
{
	xvid_enc_create.num_threads = threads;
}

bool XvidOptions::getParAsInput(void)
{
	return _parAsInput;
}

void XvidOptions::setParAsInput(bool parAsInput)
{
	_parAsInput = parAsInput;
}

void XvidOptions::getPar(unsigned int *width, unsigned int *height)
{
	*width = xvid_enc_frame.par_width;
	*height = xvid_enc_frame.par_height;
}

void XvidOptions::setPar(unsigned int width, unsigned int height)
{
	if (width > 0)
		xvid_enc_frame.par_width = width;

	if (height > 0)
		xvid_enc_frame.par_height = height;

	  if (xvid_enc_frame.par_width == xvid_enc_frame.par_height)
		  xvid_enc_frame.par = XVID_PAR_11_VGA;
	  else
		  xvid_enc_frame.par = XVID_PAR_EXT;
}

MotionEstimationMode XvidOptions::getMotionEstimation(void)
{
	MotionEstimationMode motionEstimation = ME_NONE;

	if ((xvid_enc_frame.motion & ME_HIGH) == ME_HIGH)
		motionEstimation = ME_HIGH;
	else if ((xvid_enc_frame.motion & ME_MEDIUM) == ME_MEDIUM)
		motionEstimation = ME_MEDIUM;
	else if ((xvid_enc_frame.motion & ME_LOW) == ME_LOW)
		motionEstimation = ME_LOW;

	return motionEstimation;
}

void XvidOptions::setMotionEstimation(MotionEstimationMode motionEstimation)
{
	if (motionEstimation == ME_NONE || motionEstimation == ME_LOW || motionEstimation == ME_MEDIUM || motionEstimation == ME_HIGH)
	{
		xvid_enc_frame.motion &= ~ME_NONE;
		xvid_enc_frame.motion &= ~ME_LOW;
		xvid_enc_frame.motion &= ~ME_MEDIUM;
		xvid_enc_frame.motion &= ~ME_HIGH;
		xvid_enc_frame.motion |= motionEstimation;

		if (motionEstimation == ME_NONE)
			xvid_enc_frame.type = XVID_TYPE_IVOP;
		else
			xvid_enc_frame.type = XVID_TYPE_AUTO;
	}
}

RateDistortionMode XvidOptions::getRateDistortion(void)
{
	RateDistortionMode rateDistortion = RD_NONE;

	if ((xvid_enc_frame.motion & RD_SQUARE) == RD_SQUARE)
		rateDistortion = RD_SQUARE;
	else if ((xvid_enc_frame.motion & RD_HPEL_QPEL_8) == RD_HPEL_QPEL_8)
		rateDistortion = RD_HPEL_QPEL_8;
	else if ((xvid_enc_frame.motion & RD_HPEL_QPEL_16) == RD_HPEL_QPEL_16)
		rateDistortion = RD_HPEL_QPEL_16;
	else if ((xvid_enc_frame.vop_flags & XVID_VOP_MODEDECISION_RD) == XVID_VOP_MODEDECISION_RD)
		rateDistortion = RD_DCT_ME;

	return rateDistortion;
}

void XvidOptions::setRateDistortion(RateDistortionMode rateDistortion)
{
	if (rateDistortion == RD_NONE || rateDistortion == RD_DCT_ME || rateDistortion == RD_HPEL_QPEL_16 || 
		rateDistortion == RD_HPEL_QPEL_8 || rateDistortion == RD_SQUARE)
	{
		xvid_enc_frame.motion &= ~RD_HPEL_QPEL_16;
		xvid_enc_frame.motion &= ~RD_HPEL_QPEL_8;
		xvid_enc_frame.motion &= ~RD_SQUARE;

		if (rateDistortion == RD_NONE)
			xvid_enc_frame.vop_flags &= ~XVID_VOP_MODEDECISION_RD;
		else
		{
			if (rateDistortion != RD_DCT_ME)
				xvid_enc_frame.motion |= rateDistortion;

			xvid_enc_frame.vop_flags |= XVID_VOP_MODEDECISION_RD;
		}
	}
}

bool XvidOptions::getBframeRdo(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_RD_BVOP;
}

void XvidOptions::setBframeRdo(bool bFrameRdo)
{
	if (bFrameRdo)
		xvid_enc_frame.vop_flags |= XVID_VOP_RD_BVOP;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_RD_BVOP;
}

bool XvidOptions::getChromaMotionEstimation(void)
{
	return xvid_enc_frame.motion & CHROMA_ME;
}

void XvidOptions::setChromaMotionEstimation(bool chromaMotionEstimation)
{
	if (chromaMotionEstimation)
		xvid_enc_frame.motion |= CHROMA_ME;
	else
		xvid_enc_frame.motion &= ~CHROMA_ME;
}

bool XvidOptions::getQpel(void)
{
	return xvid_enc_frame.vol_flags & XVID_VOL_QUARTERPEL;
}

void XvidOptions::setQpel(bool qpel)
{
	if (qpel)
	{
		xvid_enc_frame.vol_flags |= XVID_VOL_QUARTERPEL;
		xvid_enc_frame.motion |= XVID_ME_QUARTERPELREFINE16;

		if (getInterMotionVector())
			xvid_enc_frame.motion |= XVID_ME_QUARTERPELREFINE8;
	}
	else
	{
		xvid_enc_frame.vol_flags &= ~XVID_VOL_QUARTERPEL;
		xvid_enc_frame.motion &= ~XVID_ME_QUARTERPELREFINE16;
		xvid_enc_frame.motion &= ~XVID_ME_QUARTERPELREFINE8;
	}
}

bool XvidOptions::getGmc(void)
{
	return xvid_enc_frame.vol_flags & XVID_VOL_GMC;
}

void XvidOptions::setGmc(bool gmc)
{
	if (gmc)
	{
		xvid_enc_frame.vol_flags |= XVID_VOL_GMC;
		xvid_enc_frame.motion |= XVID_ME_GME_REFINE;
	}
	else
	{
		xvid_enc_frame.vol_flags &= ~XVID_VOL_GMC;
		xvid_enc_frame.motion &= ~XVID_ME_GME_REFINE;
	}
}

bool XvidOptions::getTurboMode(void)
{
	return xvid_enc_frame.motion & TURBO_MODE;
}

void XvidOptions::setTurboMode(bool turboMode)
{
	if (turboMode)
	{
		xvid_enc_frame.motion |= TURBO_MODE;
	}
	else
	{
		xvid_enc_frame.motion &= ~TURBO_MODE;
	}
}

bool XvidOptions::getChromaOptimisation(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_CHROMAOPT;
}

void XvidOptions::setChromaOptimisation(bool chromaOptimisation)
{
	if (chromaOptimisation)
		xvid_enc_frame.vop_flags |= XVID_VOP_CHROMAOPT;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_CHROMAOPT;
}

bool XvidOptions::getInterMotionVector(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_INTER4V;
}

void XvidOptions::setInterMotionVector(bool interMotionVector)
{
	if (interMotionVector)
	{
		xvid_enc_frame.vop_flags |= XVID_VOP_INTER4V;

		if (getQpel())
			setQpel(true);
	}
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_INTER4V;
}

bool XvidOptions::getGreyscale(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_GREYSCALE;
}

void XvidOptions::setGreyscale(bool greyscale)
{
	if (greyscale)
		xvid_enc_frame.vop_flags |= XVID_VOP_GREYSCALE;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_GREYSCALE;
}

InterlacedMode XvidOptions::getInterlaced(void)
{
	InterlacedMode interlaced = INTERLACED_NONE;

	if (xvid_enc_frame.vop_flags & XVID_VOP_TOPFIELDFIRST)
		interlaced = INTERLACED_TFF;
	else if (xvid_enc_frame.vol_flags & XVID_VOL_INTERLACING)
		interlaced = INTERLACED_BFF;

	return interlaced;
}

void XvidOptions::setInterlaced(InterlacedMode interlaced)
{
	if (interlaced == INTERLACED_NONE)
	{
		xvid_enc_frame.vol_flags &= ~XVID_VOL_INTERLACING;
		xvid_enc_frame.vop_flags &= ~XVID_VOP_TOPFIELDFIRST;
	}
	else if (interlaced == INTERLACED_BFF || interlaced == INTERLACED_TFF)
	{
		xvid_enc_frame.vol_flags |= XVID_VOL_INTERLACING;

		if (interlaced == INTERLACED_TFF)
			xvid_enc_frame.vop_flags |= XVID_VOP_TOPFIELDFIRST;
		else
			xvid_enc_frame.vop_flags &= ~XVID_VOP_TOPFIELDFIRST;
	}
}

unsigned int XvidOptions::getFrameDropRatio(void)
{
	return xvid_enc_create.frame_drop_ratio;
}

void XvidOptions::setFrameDropRatio(unsigned int frameDropRatio)
{
	if (frameDropRatio <= 100)
		xvid_enc_create.frame_drop_ratio = frameDropRatio;
}

unsigned int XvidOptions::getMaxKeyInterval(void)
{
	return xvid_enc_create.max_key_interval;
}

void XvidOptions::setMaxKeyInterval(unsigned int maxKeyInterval)
{
	xvid_enc_create.max_key_interval = maxKeyInterval;
}

unsigned int XvidOptions::getMaxBFrames(void)
{
	return xvid_enc_create.max_bframes;
}

void XvidOptions::setMaxBFrames(unsigned int maxBFrames)
{
	if (maxBFrames <= 20)
		xvid_enc_create.max_bframes = maxBFrames;
}

bool XvidOptions::getPacked(void)
{
	return xvid_enc_create.global & XVID_GLOBAL_PACKED;
}

void XvidOptions::setPacked(bool packed)
{
	if (packed)
		xvid_enc_create.global |= XVID_GLOBAL_PACKED;
	else
		xvid_enc_create.global &= ~XVID_GLOBAL_PACKED;
}

bool XvidOptions::getMpegQuantisation(void)
{
	return xvid_enc_frame.vol_flags & XVID_VOL_MPEGQUANT;
}

void XvidOptions::setMpegQuantisation(bool mpegQuantisation)
{
	if (mpegQuantisation)
		xvid_enc_frame.vol_flags |= XVID_VOL_MPEGQUANT;
	else
		xvid_enc_frame.vol_flags &= ~XVID_VOL_MPEGQUANT;
}

bool XvidOptions::getTrellis(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_TRELLISQUANT;
}

void XvidOptions::setTrellis(bool trellis)
{
	if (trellis)
		xvid_enc_frame.vop_flags |= XVID_VOP_TRELLISQUANT;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_TRELLISQUANT;
}

bool XvidOptions::getCartoon(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_CARTOON;
}

void XvidOptions::setCartoon(bool cartoon)
{
	if (cartoon)
		xvid_enc_frame.vop_flags |= XVID_VOP_CARTOON;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_CARTOON;
}

void XvidOptions::getMinQuantiser(unsigned int *i, unsigned int *p, unsigned int *b)
{
	*i = xvid_enc_create.min_quant[0];
	*p = xvid_enc_create.min_quant[1];
	*b = xvid_enc_create.min_quant[2];
}

void XvidOptions::setMinQuantiser(unsigned int i, unsigned int p, unsigned int b)
{
	if (i > 0 && i <= 51)
		xvid_enc_create.min_quant[0] = i;

	if (p > 0 && p <= 51)
		xvid_enc_create.min_quant[1] = p;

	if (b > 0 && b <= 51)
		xvid_enc_create.min_quant[2] = b;
}

void XvidOptions::getMaxQuantiser(unsigned int *i, unsigned int *p, unsigned int *b)
{
	*i = xvid_enc_create.max_quant[0];
	*p = xvid_enc_create.max_quant[1];
	*b = xvid_enc_create.max_quant[2];
}

void XvidOptions::setMaxQuantiser(unsigned int i, unsigned int p, unsigned int b)
{
	if (i > 0 && i <= 51)
		xvid_enc_create.max_quant[0] = i;

	if (p > 0 && p <= 51)
		xvid_enc_create.max_quant[1] = p;

	if (b > 0 && b <= 51)
		xvid_enc_create.max_quant[2] = b;
}

bool XvidOptions::getClosedGop(void)
{
	return xvid_enc_create.global & XVID_GLOBAL_CLOSED_GOP;
}

void XvidOptions::setClosedGop(bool closedGop)
{
	if (closedGop)
		xvid_enc_create.global |= XVID_GLOBAL_CLOSED_GOP;
	else
		xvid_enc_create.global &= ~XVID_GLOBAL_CLOSED_GOP;
}

char* XvidOptions::toXml(void)
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

		if (!(xmlNodeRoot = xmlNewDocNode(xmlDoc, NULL, (xmlChar*)"XvidConfig", NULL)))
			break;

		xmlDocSetRootElement(xmlDoc, xmlNodeRoot);

		if (_configurationType != CONFIG_CUSTOM)
		{
			xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"presetConfiguration", NULL);

			xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"name", (xmlChar*)_configurationName);

			if (_configurationType == CONFIG_USER)
				strcpy((char*)xmlBuffer, "user");
			else if (_configurationType == CONFIG_SYSTEM)
				strcpy((char*)xmlBuffer, "system");
			else
				strcpy((char*)xmlBuffer, "default");

			xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"type", xmlBuffer);
		}

		addXvidOptionsToXml(xmlNodeRoot);
		xml = dumpXmlDocToMemory(xmlDoc);
		xmlFreeDoc(xmlDoc);

		break;
	}

	return xml;
}

char* XvidOptions::dumpXmlDocToMemory(xmlDocPtr xmlDoc)
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

void XvidOptions::addXvidOptionsToXml(xmlNodePtr xmlNodeRoot)
{
	const int bufferSize = 100;
	xmlChar xmlBuffer[bufferSize + 1];
	xmlNodePtr xmlNodeChild, xmlNodeChild2;

	xmlNodeRoot = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"XvidOptions", NULL);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"threads", number2String(xmlBuffer, bufferSize, getThreads()));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"vui", NULL);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"sarAsInput", boolean2String(xmlBuffer, bufferSize, getParAsInput()));

	unsigned int width, height;

	getPar(&width, &height);

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"sarHeight", number2String(xmlBuffer, bufferSize, height));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"sarWidth", number2String(xmlBuffer, bufferSize, width));

	switch (getMotionEstimation())
	{
		case ME_LOW:
			strcpy((char*)xmlBuffer, "low");
			break;
		case ME_MEDIUM:
			strcpy((char*)xmlBuffer, "medium");
			break;
		case ME_HIGH:
			strcpy((char*)xmlBuffer, "high");
			break;
		default:
			strcpy((char*)xmlBuffer, "none");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"motionEstimation", xmlBuffer);

	switch (getRateDistortion())
	{
		case RD_DCT_ME:
			strcpy((char*)xmlBuffer, "dct");
			break;
		case RD_HPEL_QPEL_16:
			strcpy((char*)xmlBuffer, "hpelQpel16");
			break;
		case RD_HPEL_QPEL_8:
			strcpy((char*)xmlBuffer, "hpelQpel8");
			break;
		case RD_SQUARE:
			strcpy((char*)xmlBuffer, "square");
			break;
		default:
			strcpy((char*)xmlBuffer, "none");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"rdo", xmlBuffer);

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"bFrameRdo", boolean2String(xmlBuffer, bufferSize, getBframeRdo()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"chromaMotionEstimation", boolean2String(xmlBuffer, bufferSize, getChromaMotionEstimation()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"qPel", boolean2String(xmlBuffer, bufferSize, getQpel()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"gmc", boolean2String(xmlBuffer, bufferSize, getGmc()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"turboMode", boolean2String(xmlBuffer, bufferSize, getTurboMode()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"chromaOptimiser", boolean2String(xmlBuffer, bufferSize, getChromaOptimisation()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"fourMv", boolean2String(xmlBuffer, bufferSize, getInterMotionVector()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"greyscale", boolean2String(xmlBuffer, bufferSize, getGreyscale()));

	switch (getInterlaced())
	{
		case INTERLACED_BFF:
			strcpy((char*)xmlBuffer, "bff");
			break;
		case INTERLACED_TFF:
			strcpy((char*)xmlBuffer, "tff");
			break;
		default:
			strcpy((char*)xmlBuffer, "none");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"interlaced", xmlBuffer);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"frameDropRatio", number2String(xmlBuffer, bufferSize, getFrameDropRatio()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"maxIframeInterval", number2String(xmlBuffer, bufferSize, getMaxKeyInterval()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"maxBframes", number2String(xmlBuffer, bufferSize, getMaxBFrames()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"packed", boolean2String(xmlBuffer, bufferSize, getPacked()));
}

bool XvidOptions::validateXml(xmlDocPtr doc)
{
	const char *schemaFile = "XvidParam.xsd";
	char *pluginDir = ADM_getPluginPath();
	char schemaPath[strlen(pluginDir) + strlen(PLUGIN_SUBDIR) + 1 + strlen(schemaFile) + 1];
	bool success = false;

	strcpy(schemaPath, pluginDir);
	strcat(schemaPath, PLUGIN_SUBDIR);
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

int XvidOptions::fromXml(const char *xml)
{
	bool success = false;

	clearPresetConfiguration();

	xmlDocPtr doc = xmlReadMemory(xml, strlen(xml), "Xvid.xml", NULL, 0);

	if (success = validateXml(doc))
	{
		xmlNode *xmlNodeRoot = xmlDocGetRootElement(doc);

		for (xmlNode *xmlChild = xmlNodeRoot->children; xmlChild; xmlChild = xmlChild->next)
		{
			if (xmlChild->type == XML_ELEMENT_NODE)
			{
				char *content = (char*)xmlNodeGetContent(xmlChild);

				if (strcmp((char*)xmlChild->name, "presetConfiguration") == 0)
					parsePresetConfiguration(xmlChild);
				else if (strcmp((char*)xmlChild->name, "XvidOptions") == 0)
					parseXvidOptions(xmlChild);

				xmlFree(content);
			}
		}
	}

	xmlFreeDoc(doc);

	return success;
}

void XvidOptions::parsePresetConfiguration(xmlNode *node)
{
	char* name = NULL;
	ConfigType type = CONFIG_CUSTOM;

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
					type = CONFIG_USER;
				else if (strcmp(content, "system") == 0)
					type = CONFIG_SYSTEM;
				else
					type = CONFIG_DEFAULT;
			}

			xmlFree(content);
		}
	}

	setPresetConfiguration(name, type);
	free(name);
}

void XvidOptions::parseXvidOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "threads") == 0)
				setThreads(atoi(content));
			else if (strcmp((char*)xmlChild->name, "vui") == 0)
				parseVuiOptions(xmlChild);
			else if (strcmp((char*)xmlChild->name, "motionEstimation") == 0)
			{
				MotionEstimationMode motionEstimation = ME_NONE;

				if (strcmp(content, "low") == 0)
					motionEstimation = ME_LOW;
				else if (strcmp(content, "medium") == 0)
					motionEstimation = ME_MEDIUM;
				else if (strcmp(content, "high") == 0)
					motionEstimation = ME_HIGH;

				setMotionEstimation(motionEstimation);
			}
			else if (strcmp((char*)xmlChild->name, "rdo") == 0)
			{
				RateDistortionMode rateDistortion = RD_NONE;

				if (strcmp(content, "dct") == 0)
					rateDistortion = RD_DCT_ME;
				else if (strcmp(content, "hpelQpel16") == 0)
					rateDistortion = RD_HPEL_QPEL_16;
				else if (strcmp(content, "hpelQpel8") == 0)
					rateDistortion = RD_HPEL_QPEL_8;
				else if (strcmp(content, "square") == 0)
					rateDistortion = RD_SQUARE;

				setRateDistortion(rateDistortion);
			}
			else if (strcmp((char*)xmlChild->name, "bFrameRdo") == 0)
				setBframeRdo(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "chromaMotionEstimation") == 0)
				setChromaMotionEstimation(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "qPel") == 0)
				setQpel(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "gmc") == 0)
				setGmc(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "turboMode") == 0)
				setTurboMode(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "chromaOptimiser") == 0)
				setChromaOptimisation(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "fourMv") == 0)
				setInterMotionVector(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "greyscale") == 0)
				setGreyscale(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "interlaced") == 0)
			{
				InterlacedMode interlaced = INTERLACED_NONE;

				if (strcmp(content, "bff") == 0)
					interlaced = INTERLACED_BFF;
				else if (strcmp(content, "tff") == 0)
					interlaced = INTERLACED_TFF;

				setInterlaced(interlaced);
			}
			else if (strcmp((char*)xmlChild->name, "frameDropRatio") == 0)
				setFrameDropRatio(atoi(content));
			else if (strcmp((char*)xmlChild->name, "maxIframeInterval") == 0)
				setMaxKeyInterval(atoi(content));
			else if (strcmp((char*)xmlChild->name, "maxBframes") == 0)
				setMaxBFrames(atoi(content));
			else if (strcmp((char*)xmlChild->name, "packed") == 0)
				setPacked(string2Boolean(content));

			xmlFree(content);
		}
	}
}

void XvidOptions::parseVuiOptions(xmlNode *node)
{
	unsigned int width = 0, height = 0;

	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "sarAsInput") == 0)
				setParAsInput(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "sarHeight") == 0)
				height = atoi(content);
			else if (strcmp((char*)xmlChild->name, "sarWidth") == 0)
				width = atoi(content);

			xmlFree(content);
		}
	}

	setPar(width, height);
}

xmlChar* XvidOptions::number2String(xmlChar *buffer, size_t size, int number)
{
	snprintf((char*)buffer, size, "%d", number);

	return buffer;
}

xmlChar* XvidOptions::number2String(xmlChar *buffer, size_t size, unsigned int number)
{
	snprintf((char*)buffer, size, "%d", number);

	return buffer;
}

xmlChar* XvidOptions::number2String(xmlChar *buffer, size_t size, float number)
{
	snprintf((char*)buffer, size, "%f", number);

	return buffer;
}

xmlChar* XvidOptions::boolean2String(xmlChar *buffer, size_t size, bool boolean)
{
	if (boolean)
		strcpy((char*)buffer, "true");
	else
		strcpy((char*)buffer, "false");

	return buffer;
}

bool XvidOptions::string2Boolean(char *buffer)
{
	return (strcmp(buffer, "true") == 0);
}
