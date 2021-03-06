/**
        \fn ADM_vf_plugin.h
        \brief Interface for dynamically loaded video filter
*/
#ifndef ADM_vf_plugin_h
#define ADM_vf_plugin_h
#include <stddef.h>

#include "ADM_vf_plugin_export.h"
#include "DIA_uiTypes.h"
#include "ADM_filterCategory.h"
#include "ADM_paramList.h"
#include "ADM_coreUtils.h"
#include "ADM_dynamicLoading.h"

class ADM_coreVideoFilter;

#define VF_API_VERSION 4

/**
    \struct admVideoFilterInfo
*/
typedef struct
{
        const char                  *internalName;
        const char                  *displayName;
        const char                  *desc;
        VF_CATEGORY                 category;
}admVideoFilterInfo;

/* These are the 6 functions exported by each plugin ...*/
typedef ADM_coreVideoFilter  *(ADM_vf_CreateFunction)(ADM_coreVideoFilter *previous,CONFcouple *conf);
typedef void              (ADM_vf_DeleteFunction)(ADM_coreVideoFilter *codec);
typedef int               (ADM_vf_SupportedUI)(void); //  QT4/GTK / ALL
typedef uint32_t          (ADM_vf_GetApiVersion)(void);
typedef bool              (ADM_vf_GetPluginVersion)(uint32_t *major, uint32_t *minor, uint32_t *patch);
typedef const char       *(ADM_vf_GetString)(void);
typedef VF_CATEGORY       (ADM_vf_getCategory)(void);
typedef void              (ADM_vf_getDefaultConfiguration)(CONFcouple **c);

/**
 *  \class ADM_vf_plugin
 *  \brief Base class for video filter loader
 */
class ADM_vf_plugin : public ADM_LibWrapper
{
	public:
		ADM_vf_CreateFunction		*create;
		ADM_vf_DeleteFunction		*destroy;
		ADM_vf_SupportedUI		    *supportedUI;

		ADM_vf_GetApiVersion		*getApiVersion;
		ADM_vf_GetPluginVersion	    *getFilterVersion;
		ADM_vf_GetString    	    *getDesc;
        ADM_vf_GetString    	    *getInternalName;
        ADM_vf_GetString    	    *getDisplayName;
        ADM_vf_getCategory          *getCategory;

        const char 					*nameOfLibrary;
	    VF_FILTERS                  tag;
        admVideoFilterInfo          info;

		ADM_vf_plugin(const char *file);
};

#define DECLARE_VIDEO_FILTER(Class,Major,Minor,Patch,UI,category,internalName,displayName,Desc) \
	extern "C" { \
	ADM_VIDEOFILTER_PLUGIN_EXPORT ADM_coreVideoFilter *create(ADM_coreVideoFilter *previous,CONFcouple *conf)\
	{ \
		return new Class(previous,conf);\
	} \
	ADM_VIDEOFILTER_PLUGIN_EXPORT void *destroy(ADM_coreVideoFilter *codec) \
	{ \
		Class *a=(Class *)codec;\
		delete a;\
        return NULL;\
	}\
	ADM_VIDEOFILTER_PLUGIN_EXPORT int supportedUI(void) \
	{ \
		return UI; \
	} \
	ADM_VIDEOFILTER_PLUGIN_EXPORT uint32_t getApiVersion(void)\
	{\
			return VF_API_VERSION;\
	}\
	ADM_VIDEOFILTER_PLUGIN_EXPORT bool getFilterVersion(uint32_t *major,uint32_t *minor, uint32_t *patch)\
	{\
		*major=Major;\
		*minor=Minor;\
		*patch=Patch;\
		return true;\
	}\
	ADM_VIDEOFILTER_PLUGIN_EXPORT const char *getDesc(void)\
	{\
		return Desc; \
	}\
	ADM_VIDEOFILTER_PLUGIN_EXPORT const char *getInternalName(void)\
	{\
		return internalName; \
	}\
	ADM_VIDEOFILTER_PLUGIN_EXPORT const char *getDisplayName(void)\
	{\
		return displayName; \
	}\
    ADM_VIDEOFILTER_PLUGIN_EXPORT VF_CATEGORY getCategory(void) \
    { \
        return category;\
    }\
	}

#endif
