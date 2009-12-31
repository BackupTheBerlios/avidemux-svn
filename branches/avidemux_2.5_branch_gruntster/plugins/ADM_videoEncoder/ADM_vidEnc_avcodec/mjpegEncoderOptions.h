/***************************************************************************
                           mjpeg1EncoderOptions.h

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

#ifndef MjpegEncoderOptions_h
#define MjpegEncoderOptions_h

#include <vector>
#include "PluginOptions.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

#define MJPEG_DEFAULT_ENCODE_MODE ADM_VIDENC_MODE_CQP
#define MJPEG_DEFAULT_ENCODE_MODE_PARAMETER 4

class MjpegEncoderOptions : public PluginOptions
{
protected:
	void addOptionsToXml(xmlNodePtr xmlNodeRoot);
	void parseOptions(xmlNode *node);

public:
	MjpegEncoderOptions(void);
};

#endif	// MjpegEncoderOptions_h
