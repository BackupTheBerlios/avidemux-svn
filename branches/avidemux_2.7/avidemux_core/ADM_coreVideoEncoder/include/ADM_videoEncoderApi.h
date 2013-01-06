/***************************************************************************
                          \fn ADM_videoEncoderApi.h
                          \brief Api to deal with video encoder
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
#ifndef ADM_VIDEO_ENCODER_API_H
#define ADM_VIDEO_ENCODER_API_H

#include "ADM_coreVideoEncoder6_export.h"
#include "ADM_confCouple.h"

class ADM_coreVideoEncoder;
class ADM_coreVideoFilter;

ADM_COREVIDEOENCODER6_EXPORT uint8_t ADM_ve6_loadPlugins(const char *path, ADM_UI_TYPE uiType);
ADM_COREVIDEOENCODER6_EXPORT void ADM_ve6_cleanup(void);
ADM_COREVIDEOENCODER6_EXPORT bool ADM_ve6_getEncoderInfo(int filter, const char **name, uint32_t *major,uint32_t *minor,uint32_t *patch);
ADM_COREVIDEOENCODER6_EXPORT const char *ADM_ve6_getMenuName(uint32_t i);
ADM_COREVIDEOENCODER6_EXPORT uint32_t ADM_ve6_getNbEncoders(void);
ADM_COREVIDEOENCODER6_EXPORT bool                  videoEncoder6SelectByName(const char *name);
ADM_COREVIDEOENCODER6_EXPORT bool                  videoEncoder6Configure(void);
ADM_COREVIDEOENCODER6_EXPORT int                   videoEncoder6_GetIndexFromName(const char *name);
ADM_COREVIDEOENCODER6_EXPORT bool                  videoEncoder6_SetCurrentEncoder(uint32_t index);
ADM_COREVIDEOENCODER6_EXPORT const char            *videoEncoder6_GetCurrentEncoderName(void);
ADM_COREVIDEOENCODER6_EXPORT bool                  videoEncoder6_SetConfiguration(CONFcouple *ci,bool full=true);
bool                  videoEncoder6_GetConfiguration(CONFcouple **c);
ADM_COREVIDEOENCODER6_EXPORT ADM_coreVideoEncoder *createVideoEncoderFromIndex(ADM_coreVideoFilter *chain,int index,bool globalHeader);

#endif
