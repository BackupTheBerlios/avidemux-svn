/***************************************************************************
                          filter.cpp  -  description
                             -------------------
    begin                : Wed Mar 27 2002
    copyright            : (C) 2002 by mean
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "ADM_assert.h"
#include "config.h"
#include "fourcc.h"
#include "avio.hxx"

#include "ADM_toolkit/toolkit.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_video/ADM_videoNull.h"
#include "ADM_filter/video_filters.h"
#include "ADM_video/ADM_vidPartial.h"
#include "ADM_userInterfaces/ADM_commonUI/GUI_render.h"

#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_PREVIEW
#include "ADM_osSupport/ADM_debug.h"
#include "ADM_osSupport/ADM_quota.h"
//
// exported vars
std::vector <FILTER_ENTRY> allfilters;
uint32_t nb_active_filter=0;
FILTER  videofilters[MAX_FILTER];

static uint32_t lastStart=0, lastNb=0;

extern ADM_Composer *video_body;
extern AVDMGenericVideoStream *filterCreateFromTag(VF_FILTERS tag,CONFcouple *conf, AVDMGenericVideoStream *in) ;
extern char *ADM_escape(const ADM_filename *incoming);
//

static  void updateVideoFilters(void);

// Ugly should be dynamically allocated
#warning HARDCODEC IMAGE SIZE

// dummy constructor used to register the filter
//____________________________________
AVDMVideo_FilterDec::AVDMVideo_FilterDec(char *name,
									AVDMGenericVideoStream *(*create) (AVDMGenericVideoStream *in, void *))
{
    UNUSED_ARG(name);
    UNUSED_ARG(create);
         //registerFilter(name, create);
	 //
}
//
// Delete everything that is pending
void filterCleanUp( void )
{
	for(uint32_t i=0;i<nb_active_filter;i++)
	{
		  	delete videofilters[i].filter;
			if(videofilters[i].conf) delete videofilters[i].conf;
             videofilters[i].conf=NULL;
             videofilters[i].filter=NULL;  		
	}
   nb_active_filter=0;
}
void filterListAll( void )
{
	printf("\nVideo filters\n");
	for(uint32_t i=0;i<allfilters.size();i++)
	{
		const char *name = allfilters[ i].filtername;
		if(allfilters[ i].viewable)
		{
			if(name)
				printf("\t%s\n",name);
		}
	}

}
VF_FILTERS filterGetTagFromName(const char *inname)
{
	VF_FILTERS filter=VF_DUMMY;
	for(uint32_t i=0;i<allfilters.size();i++)
	{
		const char *name=allfilters[ i].filtername;
		if(allfilters[ i].viewable || allfilters[i].tag==VF_PARTIAL)
		{
			if(name)
			{
				if(strlen(name))
				{
					if(!strcasecmp(name,inname))
						return allfilters[ i].tag; 
				}
			}
		}
	}
	return filter;
}
void registerFilter(const char *name,VF_FILTERS tag,uint8_t viewable
		,AVDMGenericVideoStream *(*create) (AVDMGenericVideoStream *in, CONFcouple *)
		,char *filtername)
{
//        printf ("*nb_video_filter() = %d, max %d, name = %s\n", nb_video_filter(), (MAX_FILTER-1), name);
        allfilters.push_back (FILTER_ENTRY (name, create, tag, viewable, filtername));

        if(viewable==1)
	{
        	aprintf("\nRegistered filter %lu: %s",allfilters.size(),name);
	}
}
void registerFilterEx(const char *name,VF_FILTERS tag,uint8_t viewable
		,AVDMGenericVideoStream *(*create) (AVDMGenericVideoStream *in, CONFcouple *)
		,const char *filtername,AVDMGenericVideoStream *(*create_from_script) (AVDMGenericVideoStream *in, int n,Arg *args), const char *desc)
{
//    printf ("nb_video_filter = %d, max %d, name = %s\n", nb_video_filter, (MAX_FILTER-1), name);
        allfilters.push_back (FILTER_ENTRY (name, create, tag, viewable,
                                            filtername, desc, create_from_script));

        if(viewable==1)
	{
        	aprintf("\nRegistered filter %lu: %s",allfilters.size(),name);
	}


}
/************************** Get the last video filter of the chain ***************************
The reference is 0 in that case !
************************************************************************************************/
AVDMGenericVideoStream *getLastVideoFilter(void)
{
    aviInfo info;

	video_body->getVideoInfo(&info);

	lastNb=info.nb_frames;
	lastStart=0;
	aprintf("GetLast : -- LasrNB %lu start %lu \n",lastNb,lastNb);
  // sanity check
  	if(nb_active_filter==0)
  		{
  		 		nb_active_filter=1;
				aprintf("--preview filter 0\n");
				return videofilters[  nb_active_filter-1].filter;
  		}
 	updateVideoFilters();
	return videofilters[  nb_active_filter-1].filter;
}

/*
	Return last video filter and rebuild a chain with only the selected frame

*/
AVDMGenericVideoStream *getLastVideoFilter( uint32_t start, uint32_t nb)
{
  // sanity check
  	lastNb=nb;
	lastStart=start;
	aprintf("GetLast full : -- LasrNB %lu start %lu \n",lastNb,lastNb);
  	if(nb_active_filter==0)
  		{
  		 		nb_active_filter=1;
  		 		videofilters[0].filter=  new AVDMVideoStreamNull(video_body,start,nb);
  		}
	updateVideoFilters();
       return videofilters[  nb_active_filter-1].filter;
}

/********* Returns the first video filters starting from frame xx with nb frames ***************/
AVDMGenericVideoStream *getFirstVideoFilter( uint32_t start, uint32_t nbFrame)
{
  // sanity check
  	lastNb=nbFrame;
	lastStart=start;
	aprintf("GetFirst full : -- LasrNB %lu start %lu \n",lastNb,lastNb);
	updateVideoFilters();
       return videofilters[  0].filter;
}
AVDMGenericVideoStream *getFirstCurrentVideoFilter( void)
{
	ADM_assert(nb_active_filter);
 	return videofilters[  0].filter;
}
AVDMGenericVideoStream *getFirstVideoFilter( void)
{
  // sanity check
  aviInfo info;

	video_body->getVideoInfo(&info);
	lastNb=info.nb_frames;
	lastStart=0;
	aprintf("GetFirst : -- LasrNB %lu start %lu \n",lastNb,lastNb);
	updateVideoFilters();
       return videofilters[  0].filter;

}

FILTER * getCurrentVideoFilterList (uint32_t * count)
{
    if (count)
        *count = nb_active_filter;
    return videofilters;
}

//
//	Parse the list of active filters, delete them et recreate them with (new ?) configuration
//
void updateVideoFilters(void )
{
		if(nb_active_filter!=0)
		{
			delete videofilters[0].filter;
			videofilters[0].filter=NULL;

		}
  		 videofilters[0].filter=  new AVDMVideoStreamNull(video_body,lastStart,lastNb);

  		if(nb_active_filter<=1)
  		{
  		 	nb_active_filter=1;
			aprintf("--preview filter %d\n",nb_active_filter-1);
                        admPreview::updateFilters(videofilters[0].filter,videofilters[nb_active_filter-1].filter);
  		 	return;
  		}
  		// Rebuild other filters
                for(uint32_t i=1;i<nb_active_filter;i++)
                {
                    VF_FILTERS tag;
                    AVDMGenericVideoStream *old;
                            old= videofilters[i].filter;
                            tag=videofilters[i].tag;
  
                            videofilters[i].filter=filterCreateFromTag(tag,
                                                videofilters[i].conf,
                                                videofilters[i-1].filter);
                          delete old;
                }
		aprintf("--preview filter %d\n",nb_active_filter-1);
                ADM_assert(nb_active_filter);
                admPreview::updateFilters(videofilters[0].filter,videofilters[nb_active_filter-1].filter);
}
//
//	Create a filter from : its tag, its config and an input stream
//

AVDMGenericVideoStream *filterCreateFromTag(VF_FILTERS tag,CONFcouple *couple, AVDMGenericVideoStream *in)
{
	 AVDMGenericVideoStream *filter;

			ADM_assert(tag!=VF_INVALID);
                        if(tag>=VF_EXTERNAL_START)
                        {
                          // start from the end, it is an external filter 
                          for(uint32_t i=allfilters.size()-1;i>=0;i--)
                          {
                              if(tag==allfilters[i].tag)
                                {
                                        filter=allfilters[i].create( in, couple);
                                        return filter;
                                }
                          }
                        } // else search forward
                        else
                        {
                          for(unsigned int i=0;i<allfilters.size();i++)
                                  {
                                          if(tag==allfilters[i].tag)
                                                  {
                                                          filter=allfilters[i].create( in, couple);
                                                          return filter;
                                                  }
                                  }
                        }
			ADM_assert(0);
			return NULL;                      
}
/*


*/
const FILTER_ENTRY * filterGetEntryFromTag (VF_FILTERS tag)
{
    ADM_assert(tag!=VF_INVALID);
    for(unsigned int i=0;i<allfilters.size();i++)
    {
        if(tag==allfilters[i].tag)
        {
            return &(allfilters[i]);
        }
    }
    ADM_assert(0);
    return NULL;                      
}

const char  *filterGetNameFromTag(VF_FILTERS tag)
{
    const FILTER_ENTRY * entry = filterGetEntryFromTag (tag);
    ADM_assert(entry);
    return entry->name;
}
/*____________________________________
	Save and load current set of filters
	
	We save/load the tag that identifies the filter
	and a raw hex dump of its config
		+ comment and name in clear text
	____________________________________ 
*/

/*---------------------------------------*/	 


//______________________________________________________
// Check and build a confCouple from the args
// the filter_param arg is a template for that filter
//______________________________________________________
CONFcouple *filterBuildCouple(FILTER_PARAM *param,uint32_t nb,Arg *args)
{
int found,l;
int trans[MAXPARAM];
	if(param->nb>VARIABLE_PARAMS)
	{
		// Variable # of parameters
		// We check we have at least what is required by the template
		for(int i=0;i<(param->nb-VARIABLE_PARAMS);i++)
				{
					l=strlen(param->param[i]);
					ADM_assert(l);
					found=-1;
					for(int j=0;j<nb;j++)
					{
						if(!strncasecmp(param->param[i],args[j].arg.string,l))
						{
							if(strlen(args[j].arg.string)>l && args[j].arg.string[l]=='=')
							{
								found=j;
								break;
							}
						}
					}
					if(found==-1)
					{	
						printf("Param : %s not found or incorrect\n",param->param[i]);
						 return NULL;
					}
				}
				// if we get here, it means we found each param, time to build the couples
				CONFcouple *couple;
				couple=new CONFcouple(nb);
				for(int i=0;i<nb;i++)
				{
					char *copy=ADM_strdup(args[i].arg.string);
					// Search "="
					char *where=strstr(copy,"=");
					if(!where) ADM_assert(0);
					*where=0;
					if(!couple->setCouple(copy,where+1))
						{
							printf("Set couple failed\n");
							delete couple;
							return NULL;
						}
					ADM_dealloc(copy);
				}
				return couple;
	}
	//********************* Constant # of parameters #################
	else
	{
		if(nb!=param->nb )
		{
			printf("# of parameters mismatch: expected %d, got %d\n",nb,param->nb);
			return NULL;
		}
		// For each param check we are ok
		// the generic form is name=value
		// name should be the same as in the param array
		for(int i=0;i<nb;i++)
		{
			l=strlen(param->param[i]);
			ADM_assert(l);
			found=-1;
			for(int j=0;j<nb;j++)
			{
				if(!strncasecmp(param->param[i],args[j].arg.string,l))
				{
					if(strlen(args[j].arg.string)>l && args[j].arg.string[l]=='=')
					{
						found=j;
						trans[i]=j;
						break;
					}
				}
			}
			if(found==-1)
			{	
				printf("Param : %s not found or incorrect\n",param->param[i]);
				 return NULL;
			}
		}
		// if we get here, it means we found each param, time to build the couples
		CONFcouple *couple;
		couple=new CONFcouple(nb);
		for(int i=0;i<nb;i++)
		{
			l=strlen(param->param[i]);
			if(!couple->setCouple(param->param[i],args[ trans[i]].arg.string+l+1))
				{
					printf("Set couple failed\n");
					delete couple;
					return NULL;
				}
		}
		return couple;
	}
	
}
uint8_t 	filterAddScript(VF_FILTERS tag,uint32_t n,Arg *args)
{
	// 1- search filter
	int found=-1;
	aviInfo info;

	video_body->getVideoInfo(&info);

	for(unsigned int i=0;i<allfilters.size();i++)
	{
		if(tag==allfilters[i].tag)
		{
			if(nb_active_filter<1)
			{
  		 		nb_active_filter=1;
  		 		videofilters[0].filter=  new AVDMVideoStreamNull(video_body,0,info.nb_frames);	
			}
			AVDMGenericVideoStream *filter=NULL;
			CONFcouple *setup=NULL;
			if(!allfilters[i].create_from_script)
			{
				printf("That filter cannot be created from script\n");
				return 0;
			}
			filter=allfilters[i].create_from_script(videofilters[nb_active_filter-1].filter,n-1,&(args[1]));
			if(!filter) return 0;
			videofilters[nb_active_filter].filter=filter;
			videofilters[nb_active_filter].tag=tag;						
			filter->getCoupledConf(&setup);
			videofilters[nb_active_filter].conf=setup;
			nb_active_filter++;
			return 1;
		}
	}
	printf("Tag not found:%d\n",tag);
	return 0;
}

void filterSaveScriptJS(FILE *f)
{
                for(int i=1;i<nb_active_filter;i++)
                {
                        VF_FILTERS tag=videofilters[i].tag;
                        qfprintf(f,"app.video.addFilter(");
                        for(unsigned int j=0;j<allfilters.size();j++)
                                {
                                        if(tag==allfilters[j].tag)
                                        {       
                                                qfprintf(f,"\"%s\"",allfilters[j].filtername);
                                                break;  
                                        }
                                }
                        // get args
                        CONFcouple *couple;
                        char *arg,*value,*filtered=NULL;
                        if(videofilters[i].filter->getCoupledConf( &couple))
                        {
                                for(int j=0;j<couple->getNumber();j++)
                                {
                                         couple->getEntry(j, &arg,&value);
                                        // Filter out backslash
                                         filtered=ADM_escape((ADM_filename *)value);
                                         qfprintf(f,",\"%s=%s\"",arg,filtered);
                                         if(filtered) delete [] filtered ;
                                         filtered=NULL;
                                }
                                delete couple;
                        }
                        qfprintf(f,");\n");                      
                
                }
                
}               	
// EOF
