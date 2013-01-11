/***************************************************************************
                          \fn     jpegPlugin
                          \brief  Plugin for jpeg dummy encoder
                             -------------------

    copyright            : (C) 2002/2009 by mean
    email                : fixounet@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ADM_default.h"
#include "ADM_ffMpeg4.h"
#include "ADM_coreVideoEncoderInternal.h"
#include "../src/FFcodecSettings_desc.cpp"
extern bool         ffMpeg4Configure(void);
extern FFcodecSettings Mp4Settings;

void resetConfigurationData()
{
	FFcodecSettings defaultConf = MPEG4_CONF_DEFAULT;

	memcpy(&Mp4Settings, &defaultConf, sizeof(FFcodecSettings));
}

ADM_DECLARE_VIDEO_ENCODER_PREAMBLE(ADM_ffMpeg4Encoder);
ADM_DECLARE_VIDEO_ENCODER_MAIN("ffMpeg4",
                               "MPEG-4 ASP",
                               "Simple FFmpeg based MPEG-4 ASP Encoder (c) 2009 Mean",
							   "FFmpeg",
                                ffMpeg4Configure, // No configuration
                                ADM_UI_ALL,
                                1,0,0,
                                FFcodecSettings_param, // conf template
                                &Mp4Settings // conf var
);

static char* version = NULL;

extern "C" {
	ADM_VIDEOENCODER_PLUGIN_EXPORT const char* getUnderlyingLibraryVersion()
	{
		if (version == NULL)
		{
			version = new char[10];
			snprintf(version, 10, "%d", avcodec_version());
		}

		return version;
	}
}