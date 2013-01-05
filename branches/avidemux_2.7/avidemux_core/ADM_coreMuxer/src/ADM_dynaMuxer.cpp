/***************************************************************************
                          ADM_coreMuxer.cpp  -  description
                             -------------------
    copyright            : (C) 2008 by mean
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

#include "BVector.h"
#include "ADM_default.h"
#include "ADM_dynMuxer.h"
#include "ADM_muxerInternal.h"
#include "ADM_muxerProto.h"

extern "C" {
#include "libavformat/url.h"
}

BVector <IMuxerPlugin*> ListOfMuxers;

/**
        \fn ADM_mx_getNbMuxers
        \brief Returns the number of muxers plugins except one
*/
int ADM_mx_getMuxerCount(void)
{
    return ListOfMuxers.size();
}

/**
    \fn tryLoadingMuxerPlugin
    \brief Try loading the file given as argument as a muxer

*/
#define Fail(x) {printf("%s:"#x"\n",file);goto er;}
static bool tryLoadingMuxerPlugin(const char *file)
{
	IMuxerPlugin *dll = ADM_dynMuxer::loadPlugin(file);

    if (dll == NULL) Fail(CannotLoad);
    if(dll->apiVersion() != ADM_MUXER_API_VERSION) Fail(WrongApiVersion);

    ListOfMuxers.append(dll); // Needed for cleanup. FIXME TODO Delete it.
    printf("[Muxers] Registered filter %s as  %s\n",file,dll->descriptor());
    return true;
	// Fail!
er:
	delete dll;
	return false;

}
/**
 * 	\fn ADM_mx_loadPlugins
 *  \brief load all audio device plugins
 */
bool ADM_mx_loadPlugins(const char *path)
{
#define MAX_EXTERNAL_FILTER 100
// FIXME Factorize

	char *files[MAX_EXTERNAL_FILTER];
	uint32_t nbFile;

	memset(files,0,sizeof(char *)*MAX_EXTERNAL_FILTER);
	printf("[ADM_mx_plugin] Scanning directory %s\n",path);

	if(!buildDirectoryContent(&nbFile, path, files, MAX_EXTERNAL_FILTER, SHARED_LIB_EXT))
	{
		printf("[ADM_av_plugin] Cannot parse plugin\n");
		return false;
	}

	for(uint32_t i=0;i<nbFile;i++)
		tryLoadingMuxerPlugin(files[i]);

	printf("[ADM_mx_plugin] Scanning done\n");
    // Sort muxers by displayName, bubble sort
    int nb=ListOfMuxers.size();
    for(int i=0;i<nb;i++)
        for(int j=i+1;j<nb;j++)
        {
             IMuxerPlugin *a,*b;
             a=ListOfMuxers[i];
             b=ListOfMuxers[j];
             if(strcmp(a->name(),b->name())>0)
             {
                ListOfMuxers[j]=a;
                ListOfMuxers[i]=b;
             }
        }
        clearDirectoryContent(nbFile,files);
	return true;
}
/**
    \fn ADM_mx_cleanup
*/
bool ADM_mx_cleanup(void) 
{
	int nb=ListOfMuxers.size();

	for(int i=0;i<nb;i++)
	{
		if(ListOfMuxers[i]) delete ListOfMuxers[i];
		ListOfMuxers[i]=NULL;
	}

    return true;
}

IMuxerPlugin* ADM_mx_getMuxerPlugin(int index)
{
	return ListOfMuxers[index];
}

IMuxerPlugin* ADM_mx_getMuxerPlugin(const char* id, int* index)
{
    for (int muxerIndex = 0; muxerIndex < ListOfMuxers.size(); muxerIndex++)
    {
        IMuxerPlugin *muxer = ListOfMuxers[muxerIndex];

        if (!strcasecmp(muxer->id(), id))
		{
			*index = muxerIndex;

			return muxer;
		}
    }

	*index = -1;

    return NULL;
}

IMuxerPlugin* ADM_mx_getMuxerPlugin(const char* id)
{
	int index;

	return ADM_mx_getMuxerPlugin(id, &index);
}

//___________________________________________
extern "C"
{
    #include "libavformat/avformat.h"
};

void ADM_lavFormatInit(void)
{
	av_register_all();

	// Make sure avformat is correctly configured
	const char* formats[] = {"mpegts", "dvd", "vcd", "svcd", "mp4", "psp", "flv", "matroska"};
	AVOutputFormat *avfmt;

	for (int i = 0; i < 8; i++)
	{
		avfmt = av_guess_format(formats[i], NULL, NULL);

		if (avfmt == NULL)
		{
			printf("Error: %s muxer isn't registered\n", formats[i]);
			ADM_assert(0);
		}
	}

	URLProtocol *up = ffurl_protocol_next(NULL);

	if (strcmp(up->name, "file") != 0)
	{
		printf("Error: file protocol isn't registered\n");
		ADM_assert(0);
	}
}
// EOF

