/**
        \fn ADM_ad_plugin.h
        \brief Interface for dynamically loaded audio decoder
*/
#ifndef ADM_ad_plugin_h
#define ADM_ad_plugin_h
#include "ADM_default."
#include "ADM_audiocodec.h"
#define AD_API_VERSION 1
/* These are the 4 functions exported by each plugin ...*/
typedef (ADM_AudioCodec*)ADM_ad_CreateFunction(uint32_t fourcc, WAVHeader *info,uint32_t extraLength,uint8_t *extraData);
typedef (void *)         ADM_ad_DeleteFunction(ADM_AudioCodec *codec);
typedef bool             ADM_ad_SupportedFormat(uint32_t audioFourcc);
typedef uint32_t         ADM_ad_GetApiVersion(void);
typedef uint32_t         ADM_ad_GetDecoderVersion(uint32_t *major, uint32_t *minor, uint32_t *patch);
typedef const char       *ADM_ADM_ad_GetInfo(void);
/* */

#endif
