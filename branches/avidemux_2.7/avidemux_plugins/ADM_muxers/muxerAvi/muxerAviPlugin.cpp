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
#include "muxerAvi.h"
#include "fourcc.h"

#include <stddef.h>
#include "avi_muxer_desc.cpp"

 extern "C" bool AviConfigure(void);
extern avi_muxer muxerConfig;
ADM_MUXER_BEGIN( "avi",muxerAvi,
                    1,0,0,
                    "AVI",    // Internal name
                    "AVI muxer plugin (c) Mean 2008",
                    "AVI", // DIsplay name
					NULL,
                    AviConfigure,
                    avi_muxer_param, //template
                    &muxerConfig,sizeof(muxerConfig)
                );

extern "C" {
	ADM_MUXER_PLUGIN_EXPORT const char* getUnderlyingLibraryVersion()
	{
		return NULL;
	}
}