/**
    \file  ADM_coreVideoEncoderInternal.h
    \brief interface to video encoder plugins

*/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef VIDEOENCODERINTERNAL_H
#define VIDEOENCODERINTERNAL_H

#define ADM_VIDEO_ENCODER_API_VERSION 6

#include "ADM_ve_plugin_export.h"
#include "ADM_coreVideoEncoder6_export.h"
#include "BVector.h"
#include "ADM_coreVideoEncoder.h"
#include "ADM_dynamicLoading.h"
#include "DIA_uiTypes.h"
#include "ADM_paramList.h"
#include "IVideoEncoderPlugin.h"
#include "ADM_videoEncoder6.h"

// Macros to declare audio encoder
/**************************************************************************/
#define ADM_DECLARE_VIDEO_ENCODER_PREAMBLE(Class) \
static bool getConfigurationData (CONFcouple **c); \
static bool setConfigurationData (CONFcouple *c,bool full=true);\
\
static ADM_coreVideoEncoder * create (ADM_coreVideoFilter * head,bool globalHeader) \
{ \
  return new Class (head,globalHeader); \
} \
static void destroy (ADM_coreVideoEncoder * in) \
{\
  Class *z = (Class *) in; \
  delete z; \
}
//******************************************************

#define ADM_DECLARE_VIDEO_ENCODER_MAIN(name,menuName,desc,engineName,configure,uiType,maj,minV,patch,confTemplate,confVar) \
static ADM_videoEncoderDesc encoderDesc={\
    name,\
    menuName,\
    desc,\
    ADM_VIDEO_ENCODER_API_VERSION,\
    &create,\
    &destroy,\
    configure,\
    getConfigurationData,\
    setConfigurationData,\
    resetConfigurationData,\
    uiType,\
    maj,minV,patch,\
    NULL\
};\
bool getConfigurationData (CONFcouple **c)\
{\
         if(confTemplate==NULL) {*c=NULL;return true;} \
         return ADM_paramSave(c,confTemplate,confVar); \
}\
bool setConfigurationData (CONFcouple *c,bool full)\
{\
	if(full) return ADM_paramLoad(c,confTemplate,confVar); \
	return ADM_paramLoadPartial(c,confTemplate,confVar); \
} \
extern "C" ADM_VIDEOENCODER_PLUGIN_EXPORT ADM_videoEncoderDesc *getInfo (void) \
{ \
  return &encoderDesc; \
} \
extern "C" ADM_VIDEOENCODER_PLUGIN_EXPORT const char* getUnderlyingLibraryName(void) \
{ \
	return engineName; \
}

#ifndef QT_TR_NOOP
#define QT_TR_NOOP(x) x
#endif
#endif
//EOF
