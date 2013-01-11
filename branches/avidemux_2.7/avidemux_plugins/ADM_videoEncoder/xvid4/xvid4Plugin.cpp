/***************************************************************************
                          \fn     xvid4Plugin
                          \brief  Plugin for xvid4 dummy encoder
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
#include "ADM_xvid4.h"
#include "ADM_coreVideoEncoderInternal.h"
#include "xvid4_encoder_desc.cpp"
extern bool         xvid4Configure(void);
extern xvid4_encoder xvid4Settings;

void resetConfigurationData()
{
	xvid4_encoder defaultConf = XVID_DEFAULT_CONF;

	memcpy(&xvid4Settings, &defaultConf, sizeof(xvid4_encoder));
}

ADM_DECLARE_VIDEO_ENCODER_PREAMBLE(xvid4Encoder);
ADM_DECLARE_VIDEO_ENCODER_MAIN("xvid4",
                               "MPEG-4 ASP",
                               "Xvid4 based mpeg4 Encoder (c) 2010 Mean/Gruntster",
							   "Xvid",
                                xvid4Configure, // No configuration
                                ADM_UI_ALL,
                                1,0,0,
                                xvid4_encoder_param, // conf template
                                &xvid4Settings // conf var
);

static char* version = NULL;

extern "C" {
	ADM_VIDEOENCODER_PLUGIN_EXPORT const char* getUnderlyingLibraryVersion()
	{
		if (version == NULL)
		{
			version = new char[10];
			snprintf(
				version, 10, "%d.%d.%d", XVID_VERSION_MAJOR(XVID_VERSION), 
				XVID_VERSION_MINOR(XVID_VERSION), XVID_VERSION_PATCH(XVID_VERSION));
		}

		return version;
	}
}