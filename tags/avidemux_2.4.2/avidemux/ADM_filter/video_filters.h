/***************************************************************************
                          video_filters.h  -  description
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
 #ifndef __VIDEO_FILTERS__
 #define  __VIDEO_FILTERS__

#include "ADM_script/adm_scanner.h"
typedef enum
{
                VF_INVALID=0,
                VF_RESIZE=1,
                VF_CROP,
                VF_TELECIDE,
                VF_BLACKEN,
                VF_SHARPEN,
                VF_MEAN,
                VF_MEDIAN,
                VF_GAUSSIAN,
                VF_SMOOTHCLEAN,
                VF_DEINTERLACE,
                VF_ADDBORDER,
                VF_PALSHIFT,
                VF_SWAPUV,
                VF_CONTRAST,
                VF_LUMA,
                VF_CHROMAU,
                VF_CHROMAV,
                VF_STABILIZE,
                VF_FLIP,
                VF_DENOISE,
                VF_SUBTILE,
                VF_LARGEMEDIAN,
                VF_FLUXSMOOTH,
                VF_SALT,
                VF_PARTIAL          ,
                VF_VLADSMOOTH,
                VF_ROTATE,
                VF_DROPOUT,
                VF_SWAPFIELDS,
                VF_KEEPODD,
                VF_KEEPEVEN,
                VF_SEPARATEFIELDS,
                VF_MERGEFIELDS,
                VF_CHROMASHIFT,
                VF_SMARTSWAPFIELDS,
                VF_MPLAYERRESIZE,
                VF_IVTC,
                VF_PULLDOWN,
                VF_MPLHQD3D,
                VF_MPLLQD3D,
                VF_LAVDEINT,
                VF_MPDETC,
                VF_KRNDEINT,
                VF_FORCEDPP,
                VF_SOFTEN,
                VF_DECOMB,
                VF_DECIMATE,
                VF_MSMOOTH,
                VF_STACKFIELD,
                VF_HZSTACKFIELD,
                VF_WHIRL,
                VF_EQUALIZER,
                VF_VOBSUB,
                VF_CHFPS,
                VF_RESAMPLE_FPS,
                VF_LAVPP_DEINT,
                VF_UNSTACK_FIELD,
                VF_HUE,
                VF_EQ2,
                VF_DGBOB,
                VF_MSHARPEN,
                VF_ASHARP,
                VF_CNR2,
                VF_DELTA,
                VF_UNBLEND,
                VF_HARDIVTC,
                VF_MOSAIC,
                VF_BLENDREMOVAL,
                VF_TDEINT,
                VF_MPDELOGO,
                VF_COLOR_YUV,
                VF_ANIMATED,
                VF_MCDEINT,
                VF_REVERSE,
                VF_FADE,
                VF_ASS,
                VF_YADIF,
                VF_DVBSUB,
                VF_LOGO,
                VF_COMPUTEAVERAGE,
                VF_ERASER,
                VF_SWISSARMYKNIFE,
                VF_THRESHOLD,
                VF_PARTICLELIST,
                VF_FRAMENUM,
                VF_DUMMY,
                VF_COUNT,
                MAX_FILTER = VF_COUNT - 1,
                VF_EXTERNAL_START=0xF0000000
          }VF_FILTERS;
#define ADM_CONF_STRING_SIZE 256
#define ADM_FILTER_DECLARE_CONF(fmt,...) {static char confString[ADM_CONF_STRING_SIZE];\
                                    snprintf(confString,ADM_CONF_STRING_SIZE-1,fmt,## __VA_ARGS__);\
                                    return confString;} 

typedef AVDMGenericVideoStream *(ADM_createT) (AVDMGenericVideoStream *in, CONFcouple *);
typedef AVDMGenericVideoStream *(ADM_create_from_scriptT) (AVDMGenericVideoStream *in, int n,Arg *args);

         
   struct FILTER_ENTRY
   {
        const char        *name;
        ADM_createT       *create;
        void              (*destroy)( AVDMGenericVideoStream *old); /* Maybe needed ...*/
        VF_FILTERS        tag;
        uint8_t           viewable;
        const char              *filtername;
        const char              *description;
        ADM_create_from_scriptT *create_from_script;

       FILTER_ENTRY (const char * name, ADM_createT * create, VF_FILTERS tag,
                     uint8_t viewable, const char * filtername,
                     const char * description = 0,
                     ADM_create_from_scriptT * create_from_script = 0)
           : name (name),
             create (create),
             destroy (0),
             tag (tag),
             viewable (viewable),
             filtername (filtername),
             description (description),
             create_from_script (create_from_script)
       {
       }

       FILTER_ENTRY ()
           : name (0),
             create (0),
             destroy (0),
             tag (VF_INVALID),
             viewable (0),
             filtername (0),
             description (0),
             create_from_script (0)
       {
       }
   };
 	
   typedef struct
   {
          VF_FILTERS              tag;
          AVDMGenericVideoStream *filter;
          CONFcouple             *conf;
   }FILTER;

   typedef struct
   {
            uint32_t    nb;
            const char  *param[25];
   }FILTER_PARAM;
   
AVDMGenericVideoStream *getLastVideoFilter( uint32_t frameStart, uint32_t nbFrame);
AVDMGenericVideoStream *getLastVideoFilter( void );
AVDMGenericVideoStream *getFirstVideoFilter( uint32_t frameStart, uint32_t nbFrame);
AVDMGenericVideoStream *getFirstVideoFilter( void);
AVDMGenericVideoStream *getFirstCurrentVideoFilter( void);
FILTER * getCurrentVideoFilterList (uint32_t * count);
const FILTER_ENTRY * filterGetEntryFromTag (VF_FILTERS tag);
const char * filterGetNameFromTag(VF_FILTERS tag);

  void	filterSetPostProc( void );
  
#ifdef USE_LIBXML2
void filterSaveXml(const char *name);
void filterSaveXml(const char *name,uint8_t silent);
void filterLoadXml(const char *name);
int  filterLoadXml(const char *name,uint8_t silent);
#endif
 void filterSave(const char *name);
 void filterLoad(const char *name);
 int filterLoad(const char *name,uint8_t silent);
 void filterSave(const char *name,uint8_t silent);
CONFcouple *filterBuildCouple(FILTER_PARAM *param,uint32_t n,Arg *args);
void filterSaveScriptJS(FILE *f);

void registerFilter(const char *name,VF_FILTERS tag,uint8_t viewable,
                    ADM_createT  *create,
                    char *fname);
                    
void registerFilterEx(const char *name,VF_FILTERS tag,uint8_t viewable,
                ADM_createT  *create,
		const char *filtername,
                ADM_create_from_scriptT *create_from_script,
                const char *description);
                
void 		filterListAll( void );
VF_FILTERS 	filterGetTagFromName(const char *inname);
uint8_t 	filterAddScript(VF_FILTERS tags,uint32_t n,Arg *args);
//AVDMGenericVideoStream *filterCreateFromTag(VF_FILTERS tag,uint8_t *conf, AVDMGenericVideoStream *in) ;

 #endif
