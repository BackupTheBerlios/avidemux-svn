/***************************************************************************
    \file ADM_muxerProto.h
    \brief Prototypes for muxers plugins functions
    \author Mean (C) 2009, fixounet@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ADM_MUXER_PROTO_H
#define ADM_MUXER_PROTO_H

#include "ADM_coreMuxer6_export.h"
#include "IMuxerPlugin.h"

ADM_COREMUXER6_EXPORT void ADM_lavFormatInit(void);
ADM_COREMUXER6_EXPORT bool ADM_mx_cleanup(void);
ADM_COREMUXER6_EXPORT int ADM_mx_getMuxerCount(void);
ADM_COREMUXER6_EXPORT bool ADM_mx_loadPlugins(const char *path);
ADM_COREMUXER6_EXPORT IMuxerPlugin* ADM_mx_getMuxerPlugin(int index);
ADM_COREMUXER6_EXPORT IMuxerPlugin* ADM_mx_getMuxerPlugin(const char* id);
ADM_COREMUXER6_EXPORT IMuxerPlugin* ADM_mx_getMuxerPlugin(const char* id, int* index);
#endif
