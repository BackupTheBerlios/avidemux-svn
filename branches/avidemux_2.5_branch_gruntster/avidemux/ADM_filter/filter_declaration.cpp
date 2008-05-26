/***************************************************************************
                          filter_declaration.cpp  -  description

                          Declare filters that are statically linked to avidemux.
                          Usually it is to be avoided and better to use dynamic filter.

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

#include "config.h"
#include "ADM_default.h"



#include "ADM_videoFilter.h"
#include "ADM_videoFilter_internal.h"

#define FILTERDEC_create(x) extern   AVDMGenericVideoStream *x(AVDMGenericVideoStream *in, CONFcouple *param)
#define FILTERDEC_createFromScript(x) extern   AVDMGenericVideoStream *x(AVDMGenericVideoStream *in, int n,Arg *args)

#define REGISTERX(z,a,b,c,d,e,f,g) {\
			FILTERDEC_create(f);\
			FILTERDEC_createFromScript(g);\
			registerFilterEx((const char*)a,(const char *)b,z,f,g,(const char *)c);\
}

void registerVideoFilters( void )
{
printf("\n Registering Filters\n");
printf(  "*********************\n");

//ko -- ko -- ko -- ko ko --ko -ko
	
  
REGISTERX(VF_TRANSFORM, "rotate",QT_TR_NOOP("Rotate"),QT_TR_NOOP("Rotate the picture by 90, 180 or 270 degrees."),VF_ROTATE,1,rotate_create,rotate_script);
REGISTERX(VF_INTERLACING, "tdeint",QT_TR_NOOP("TDeint"),QT_TR_NOOP("Motion adaptative deinterlacer by Tritical."),VF_TDEINT,1,tdeint_create,tdeint_script);


REGISTERX(VF_MISC, "animatedmenu",QT_TR_NOOP("Animated Menu"), QT_TR_NOOP("Create a video made of 6 mini windows, very useful to do DVD menus."),
        VF_ANIMATED,1,animated_create,animated_script);
    
#if 0  
#ifdef USE_FREETYPE   
	REGISTERX(VF_SUBTITLE, "subtitle",QT_TR_NOOP("Subtitler"),QT_TR_NOOP("Add srt/sub subtitles to the picture."),VF_SUBTILE,1,subtitle_create,subtitle_script);
// Ass can be moved & linked to libass	
	REGISTERX(VF_SUBTITLE, "ass",QT_TR_NOOP("ASS"),QT_TR_NOOP("Add ASS/SSA subtitles to the picture."),VF_ASS,1,ass_create,ass_script);
#endif
#endif	

        REGISTERX(VF_SUBTITLE,"vobsub",QT_TR_NOOP("VobSub"),QT_TR_NOOP("Embed VobSub subtitles into picture."),VF_VOBSUB,1,vobsub_create,vobsub_script);
        REGISTERX(VF_SUBTITLE,"dvbsub",QT_TR_NOOP("DVB sub"),QT_TR_NOOP("Embed DVB-T subtitle."),VF_DVBSUB,1,dvbsub_create,dvbsub_script);
        //*********************
#if 0        
	REGISTERX(VF_MISC, "computeaverage",QT_TR_NOOP("Compute Average"),QT_TR_NOOP("Compute average luma value for each pixel throughout all frames, and save to file"),VF_COMPUTEAVERAGE,1,computeaverage_create,computeaverage_script);
	REGISTERX(VF_MISC, "threshold",QT_TR_NOOP("Threshold"),QT_TR_NOOP("Force too-bright/too-dim pixels to 0 and the rest to 255 or vice-versa"),VF_THRESHOLD,1,threshold_create,threshold_script);
	//REGISTERX("particlelist",QT_TR_NOOP("Particle List"),QT_TR_NOOP("Identify 'particles' (groups of pixels)"),VF_PARTICLELIST,1,particle_create,particle_script);
	//REGISTERX("eraser",QT_TR_NOOP("Eraser"),QT_TR_NOOP("\"Erase\" arbitrary areas of each frame"),VF_ERASER,1,eraser_create,eraser_script);
	//REGISTERX("swissarmyknife",QT_TR_NOOP("Swiss Army Knife"),QT_TR_NOOP("Apply operation like P'=P*A, P-A, P+A, etc. using a convolution, rolling average, image from file, or constant"),VF_SWISSARMYKNIFE,1,swissarmyknife_create,swissarmyknife_script);
#endif
// Misc filter, internal only        
	REGISTERX(VF_MISC,"partial",QT_TR_NOOP("Partial"),"",VF_PARTIAL,0,partial_create,partial_script);
 printf("\n");
}		


// EOF
