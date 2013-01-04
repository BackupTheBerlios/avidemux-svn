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
#include "muxerMkv.h"
#include "mkv_muxer_desc.cpp"
#include "fourcc.h"
 bool mkvConfigure(void);

ADM_MUXER_BEGIN( "mkv",muxerMkv,
                    1,0,0,
                    "MKV",    // Internal name
                    "Matroska muxer plugin (c) Mean 2009",
                    "Matroska", // DIsplay name
					"FFmpeg",
                    mkvConfigure,
                    mkv_muxer_param, //template
                    &mkvMuxerConfig, //config
                    sizeof(mkv_muxer)
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