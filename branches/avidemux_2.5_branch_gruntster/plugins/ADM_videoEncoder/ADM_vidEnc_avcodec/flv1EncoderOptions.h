/***************************************************************************
                            flv1EncoderOptions.h

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

#ifndef FLV1EncoderOptions_h
#define FLV1EncoderOptions_h

#include <vector>
#include "PluginOptions.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

#define FLV1_DEFAULT_ENCODE_MODE ADM_VIDENC_MODE_CBR
#define FLV1_DEFAULT_ENCODE_MODE_PARAMETER 1500

class FLV1EncoderOptions : public PluginOptions
{
protected:
	unsigned int _bitrate, _gopSize;

	void addOptionsToXml(xmlNodePtr xmlNodeRoot);
	void parseOptions(xmlNode *node);

public:
	FLV1EncoderOptions(void);
	void reset(void);

	unsigned int getGopSize(void);
	void setGopSize(unsigned int gopSize);
};

#endif	// FLV1EncoderOptions_h
