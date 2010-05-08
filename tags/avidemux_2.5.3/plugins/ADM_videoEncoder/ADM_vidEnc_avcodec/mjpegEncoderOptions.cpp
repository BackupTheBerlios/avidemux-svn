 /***************************************************************************
                           mjpegEncoderOptions.cpp

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

#include "mjpegEncoderOptions.h"

MjpegEncoderOptions::MjpegEncoderOptions(void) : PluginOptions(MJPEG_PLUGIN_CONFIG_DIR, "Mjpeg", "avcodec/MjpegParam.xsd", MJPEG_DEFAULT_ENCODE_MODE, MJPEG_DEFAULT_ENCODE_MODE_PARAMETER) { }

void MjpegEncoderOptions::addOptionsToXml(xmlNodePtr xmlNodeRoot)
{ 
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)getOptionsTagRoot(), NULL);
}

void MjpegEncoderOptions::parseOptions(xmlNode *node) { }
