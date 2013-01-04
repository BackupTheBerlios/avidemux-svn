/***************************************************************************
    copyright            : (C) 2007 by mean
    email                : fixounet@free.fr

      See lavformat/flv[dec/env].c for detail
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
#include "ADM_muxerInternal.h"
#include "muxerffPS.h"
#include "ps_muxer_desc.cpp"
#include "fourcc.h"
 bool ffPSConfigure(void);

ADM_MUXER_BEGIN( "mpg",muxerffPS,
                    1,0,0,
                    "ffPS",    // Internal name
                    "FFmpeg PS muxer plugin (c) Mean 2009",
                    "MPEG-PS", // DIsplay name
					"FFmpeg",
                    ffPSConfigure,
                    ps_muxer_param, //template
                    &psMuxerConfig, //config
                    sizeof(ps_muxer)
                );

static char* version = NULL;

extern "C" {
	ADM_MUXER_PLUGIN_EXPORT const char* getUnderlyingLibraryVersion()
	{
		if (version == NULL)
		{
			version = new char[10];
			snprintf(version, 10, "%d", avformat_version());
		}

		return version;
	}
}