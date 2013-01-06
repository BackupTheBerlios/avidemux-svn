/***************************************************************************
                             ADM_dynVideoEncoder.cpp

    begin                : Mon Apr 14 2008
    copyright            : (C) 2008 2009 by gruntster/mean
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
#include "DIA_fileSel.h"
#include "ADM_coreVideoEncoder.h"
#include "ADM_coreVideoEncoderInternal.h"
#include "DIA_uiTypes.h"
#include "ADM_videoEncoderApi.h"
#include "BVector.h"
#include "ADM_videoEncoder6.h"

static int currentVideoCodec=0;

ADM_videoEncoderDesc copyDesc={
        "Copy",
        "Copy",
        "Copy encoder",
        ADM_VIDEO_ENCODER_API_VERSION, //uint32_t     apiVersion;            // const

        NULL, //ADM_coreVideoEncoder *(*create)(ADM_coreVideoFilter *head);
        NULL, //void         (*destroy)(ADM_coreVideoEncoder *codec);
        NULL, //bool         (*configure)(void);                                // Call UI to set it up
        NULL, //bool         (*getConfigurationData)(uint32_t *l, uint8_t **d); // Get the encoder private conf
        NULL, //bool         (*setConfigurationData)(uint32_t l, uint8_t *d);   // Set the encoder private conf
        NULL,
        ADM_UI_ALL, //ADM_UI_TYPE  UIType;                // Type of UI
        1,0,0, //uint32_t     major,minor,patch;     // Version of the plugin
        NULL  //void         *opaque;               // Hide stuff in here
};

/**
        \fn ADM_ve6_getNbEncoders
        \brief Returns the number of demuxers plugins except one
*/
uint32_t ADM_ve6_getNbEncoders(void)
{
    return ListOfEncoders.size();
}
/**
    \fn     ADM_ve6_getEncoderInfo
    \brief  Get Infos about the demuxer #th plugin
*/
bool     ADM_ve6_getEncoderInfo(int filter, const char **name, uint32_t *major,uint32_t *minor,uint32_t *patch)
{
    ADM_assert(filter<ListOfEncoders.size());

	PluginVersion *pluginVersion = ListOfEncoders[filter]->version();

	*name = ListOfEncoders[filter]->name();
	*major = pluginVersion->majorVersion;
	*minor = pluginVersion->minorVersion;
	*patch = pluginVersion->patchVersion;

    return true;
}
/**
    \fn tryLoadingFilterPlugin
    \brief Try loading the file given as argument as an audio device plugin

*/
#define Fail(x) {printf("%s:"#x"\n",file);goto er;}
static bool tryLoadingEncoderPlugin(const char *file, ADM_UI_TYPE uiType)
{
	IVideoEncoderPlugin *dll = ADM_videoEncoder6::loadPlugin(file);
    if(dll == NULL) Fail(CannotLoad);
    if(dll->apiVersion() != ADM_VIDEO_ENCODER_API_VERSION) Fail(WrongApiVersion);
	if((dll->UIType() & uiType) != uiType) Fail(WrongUI);

    ListOfEncoders.append(dll); // Needed for cleanup. FIXME TODO Delete it.
    printf("[VideoEncoder6] Registered filter %s as  %s\n",file,dll->description());
    return true;
	// Fail!
er:
	delete dll;
	return false;

}
/**
 * 	\fn ADM_ve6_loadPlugins
 *  \brief load all audio device plugins
 */
uint8_t ADM_ve6_loadPlugins(const char *path, ADM_UI_TYPE uiType)
{
#define MAX_EXTERNAL_FILTER 100
// FIXME Factorize

	char *files[MAX_EXTERNAL_FILTER];
	uint32_t nbFile;

	memset(files,0,sizeof(char *)*MAX_EXTERNAL_FILTER);
	printf("[ADM_ve6_plugin] Scanning directory %s\n",path);

	if(!buildDirectoryContent(&nbFile, path, files, MAX_EXTERNAL_FILTER, SHARED_LIB_EXT))
	{
		printf("[ADM_ve6_plugin] Cannot parse plugin\n");
		return 0;
	}
    // Add our copy encoder....
    ADM_videoEncoder6 *dll=new ADM_videoEncoder6(&copyDesc);
    ListOfEncoders.append(dll);

	for(int i=0;i<nbFile;i++)
		tryLoadingEncoderPlugin(files[i], uiType);

	printf("[ADM_ve6_plugin] Scanning done\n");
    int nb=ListOfEncoders.size();
    for(int i=1;i<nb;i++)
        for(int j=i+1;j<nb;j++)
        {
			 IVideoEncoderPlugin *a,*b;
             a=ListOfEncoders[i];
             b=ListOfEncoders[j];
			 if(strcmp(a->name(),b->name())>0)
             {
                ListOfEncoders[j]=a;
                ListOfEncoders[i]=b;
             }
        }
        clearDirectoryContent(nbFile,files);

	return 1;
}
/**
        \fn ADM_ve6_cleanup
        \brief Current device is no longer used, delete
*/
void ADM_ve6_cleanup(void)
{
        int nb=ListOfEncoders.size();
        for(int i=0;i<nb;i++)
                {
                        if(ListOfEncoders[i]) delete ListOfEncoders[i];
                        ListOfEncoders[i]=NULL;
                }
        ListOfEncoders.clear();
}

/**
    \fn ADM_ve6_getMenuName
    \brief IT starts from 0, ignoring the copy one
*/
const char *ADM_ve6_getMenuName(uint32_t i)
{
	 int nb=ListOfEncoders.size();

	ADM_assert(i < nb);

	return ListOfEncoders[i]->name();
}
/**
    \fn ADM_ve6_Changed
*/
void ADM_ve6_Changed(int newCodecIndex)
{
	int nb=ListOfEncoders.size();
	ADM_assert(newCodecIndex < nb);

	currentVideoCodec=newCodecIndex;
}


/**
    \fn createVideoEncoder
*/
ADM_coreVideoEncoder *createVideoEncoderFromIndex(ADM_coreVideoFilter *chain,int index,bool globalHeader)
{
    int nb=ListOfEncoders.size();
	ADM_assert(index < nb);
    ADM_assert(index); // 0 is for copy, should not get it through here
    IVideoEncoderPlugin *plugin=ListOfEncoders[index];

    ADM_coreVideoEncoder *enc=plugin->createEncoder(chain,globalHeader);
    return enc;
}
/**
    \fn videoEncoder6SelectByName
*/
bool videoEncoder6SelectByName(const char *name)
{
    int i=videoEncoder6_GetIndexFromName(name);
    if(i==-1) return false;

    currentVideoCodec=i;
    return true;
}
/**
    \fn videoEncoder6Configure

*/
bool                  videoEncoder6Configure(void)
{
    return ListOfEncoders[currentVideoCodec]->configure();
}
/**
    \fn videoEncoder6_SetCurrentEncoder

*/
bool                  videoEncoder6_SetCurrentEncoder(uint32_t index)
{
      int nb=ListOfEncoders.size();
      if(index>=nb) return false;
      currentVideoCodec=index;
      return true;
}
/**
    \fn videoEncoder6_GetCurrentEncoderName
*/
const char            *videoEncoder6_GetCurrentEncoderName(void)
{
    ADM_assert(currentVideoCodec<ListOfEncoders.size());
    
	return ListOfEncoders[currentVideoCodec]->id();
}
/**
    \fn videoEncoder6_SetPartialConfiguration
*/
bool                  videoEncoder6_SetConfiguration(CONFcouple *c,bool full)
{
    if(!c) return true;
    ADM_assert(currentVideoCodec<ListOfEncoders.size());
    
	return ListOfEncoders[currentVideoCodec]->setConfiguration(c, full);
}

/**
    \fn videoEncoder6_GetConfiguration
*/

bool                  videoEncoder6_GetConfiguration(CONFcouple **c)
{
    ADM_assert(currentVideoCodec<ListOfEncoders.size());
    
	return ListOfEncoders[currentVideoCodec]->getConfiguration(c);
}
/**
    \fn videoEncoder6_GetIndexFromName
*/
int                   videoEncoder6_GetIndexFromName(const char *name)
{
    int nb=ListOfEncoders.size();

    for(int i=0;i<nb;i++)
    {
        if(!strcasecmp(name,ListOfEncoders[i]->id()))
        {
            return i;
        }
    }
    return -1;
}
//EOF
