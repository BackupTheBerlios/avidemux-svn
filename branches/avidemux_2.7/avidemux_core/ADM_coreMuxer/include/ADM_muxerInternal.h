/***************************************************************************
                          ADM_videoInternal.h  -  description
    begin                : Thu Apr 18 2008
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

#ifndef  ADM_muxerInternal_H
#define  ADM_muxerInternal_H

#define ADM_MUXER_API_VERSION 8

#include "ADM_mx_plugin_export.h"
#include "ADM_confCouple.h"
#include "ADM_paramList.h"

#define ADM_MUXER_BEGIN( Ext,Class,maj,mn,pat,name,desc,displayName,configureFunc,confTemplate,confVar,confSize) \
extern "C" {\
	static void *defaultConfig = NULL; \
\
	static void snapshotDefaultConfiguration() \
	{ \
		if (confVar != NULL && defaultConfig == NULL) \
		{ \
			defaultConfig = ADM_alloc(confSize); \
			memcpy(defaultConfig, confVar, confSize); \
		} \
	} \
\
ADM_MUXER_PLUGIN_EXPORT ADM_muxer   *create(void){ return new Class; } \
ADM_MUXER_PLUGIN_EXPORT void         destroy(ADM_muxer *h){ Class *z=(Class *)h;delete z;} \
ADM_MUXER_PLUGIN_EXPORT uint8_t      getVersion(uint32_t *major,uint32_t *minor,uint32_t *patch) {*major=maj;*minor=mn;*patch=pat;return 1;} \
ADM_MUXER_PLUGIN_EXPORT uint32_t     getApiVersion(void) {return ADM_MUXER_API_VERSION;} \
ADM_MUXER_PLUGIN_EXPORT const char  *getName(void) {return name;} \
ADM_MUXER_PLUGIN_EXPORT const char  *getDescriptor(void) {return desc;} \
ADM_MUXER_PLUGIN_EXPORT const char  *getDisplayName(void) { return displayName;} \
ADM_MUXER_PLUGIN_EXPORT const char  *getDefaultExtension(void) { return Ext;} \
ADM_MUXER_PLUGIN_EXPORT bool        getConfiguration(CONFcouple **conf) \
{\
         if(confTemplate==NULL) {*conf=NULL;return true;} \
         return ADM_paramSave(conf,confTemplate,confVar); \
}\
ADM_MUXER_PLUGIN_EXPORT bool        setConfiguration(CONFcouple *conf)\
{\
	snapshotDefaultConfiguration(); \
                  return ADM_paramLoad(conf,confTemplate,confVar); \
} \
ADM_MUXER_PLUGIN_EXPORT bool        resetConfiguration() \
{ \
	snapshotDefaultConfiguration(); \
	if (defaultConfig != NULL) memcpy(confVar, defaultConfig, confSize); \
	return true; \
} \
ADM_MUXER_PLUGIN_EXPORT bool  configure(void) \
{ \
	snapshotDefaultConfiguration(); \
 if(configureFunc==NULL) return true;\
 return configureFunc();}\
}

#endif
