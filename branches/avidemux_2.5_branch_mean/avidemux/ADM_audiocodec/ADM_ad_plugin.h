/**
        \fn ADM_ad_plugin.h
        \brief Interface for dynamically loaded audio decoder
*/
#ifndef ADM_ad_plugin_h
#define ADM_ad_plugin_h
#include "ADM_default.h"
#include "../ADM_audio/ADM_audiodef.h"
#include "ADM_audiocodec.h"
#include "ADM_audio/aviaudio.hxx"
#include "ADM_audiocodec/ADM_audiocodec.h"

#define AD_API_VERSION 1
/* These are the 6 functions exported by each plugin ...*/
typedef ADM_Audiocodec  *(ADM_ad_CreateFunction)(uint32_t fourcc, 
								WAVHeader *info,uint32_t extraLength,uint8_t *extraData);
typedef void             (ADM_ad_DeleteFunction)(ADM_Audiocodec *codec);
typedef bool             (ADM_ad_SupportedFormat)(uint32_t audioFourcc);
typedef uint32_t         (ADM_ad_GetApiVersion)(void);
typedef bool            (ADM_ad_GetDecoderVersion)(uint32_t *major, uint32_t *minor, uint32_t *patch);
typedef const char       *(ADM_ADM_ad_GetInfo)(void);
/* */

#endif
