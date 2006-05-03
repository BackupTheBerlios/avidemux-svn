
/***************************************************************************
    (c)Mean 2006 fixounet@free.fr
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ADM_assert.h>

#include "config.h"
#include "fourcc.h"
#include "avio.hxx"
#include "avi_vars.h"
#include "math.h"

#include "ADM_toolkit/toolkit.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"


#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_FILTER
#include "ADM_toolkit/ADM_debug.h"
#include "ADM_filter/video_filters.h"
#include"ADM_video/ADM_vidAnimated.h"

static FILTER_PARAM animated_template={10,
    {"tc0","tc1","tc2",
    "tc3","tc4","tc5",
    "isNTSC","backgroundImg","vignetteW","vignetteH"
    }};
BUILD_CREATE(animated_create,ADMVideoAnimated);
SCRIPT_CREATE(animated_script,ADMVideoAnimated,animated_template);

extern uint8_t DIA_animated(ANIMATED_PARAM *param);

uint8_t ADMVideoAnimated::configure(AVDMGenericVideoStream *in)
{
    DIA_animated(_param);
   setup();
   return 1;
}

char *ADMVideoAnimated::printConf( void )
{
   static char buf[50];
   sprintf((char *)buf," Animated Menu ");
   return buf;
}
uint8_t ADMVideoAnimated::setup( void)
{
    cleanup();
    for(int i=0;i<MAX_VIGNETTE;i++) _caches[i]=new VideoCache(18,_in);  // 18 is good for mpeg2
    _resizer=new ADMImageResizer(_in->getInfo()->width,_in->getInfo()->height,
            _param->vignetteW,_param->vignetteH);
    _image=new ADMImage(_param->vignetteW,_param->vignetteH);

}
uint8_t ADMVideoAnimated::cleanup( void)
{
   
    for(int i=0;i<MAX_VIGNETTE;i++) 
    {
        if(_caches[i]) delete _caches[i];
        _caches[i]=NULL;
    }
    if(_resizer) delete _resizer;
    if(_image) delete _image;
    _resizer=NULL;
    _image=NULL;

}
ADMVideoAnimated::ADMVideoAnimated(AVDMGenericVideoStream *in,CONFcouple *couples) 
{
   _resizer=NULL;
   _image=NULL;
    for(int i=0;i<MAX_VIGNETTE;i++) _caches[i]=NULL;

   _in=in;
   memcpy(&_info,_in->getInfo(),sizeof(_info));    
   _info.encoding=1;
   _uncompressed=NULL;
   _param=NEW(ANIMATED_PARAM);
   if(couples)
   {
        GET(isNTSC);
        GET(backgroundImg);
        GET(vignetteW);
        GET(vignetteH);
#undef GET
#define GET(x)  couples->getCouple((char *)"tc"#x,&(_param->timecode[x]))
        GET(0);
        GET(1);
        GET(2);
        GET(3);
        GET(4);
        GET(5);
   }
   else // Default
   {
#define MKP(x,y) _param->x=y;
            MKP(isNTSC,0);
            MKP(vignetteW,160);
            MKP(vignetteH,120);
            MKP(backgroundImg,(ADM_filename *)"taist.jpg");
#undef MKP
#define MKP(x,y) _param->timecode[x]=y
        MKP(0,0);
        MKP(1,100);
        MKP(2,200);
        MKP(3,300);
        MKP(4,500);
        MKP(5,600);
   }
   if(_param->isNTSC) _info.height=480;
        else _info.height=576;
    _info.width=720;
    setup();
}
//____________________________________________________________________
ADMVideoAnimated::~ADMVideoAnimated()
{
   delete _param;
   _param=NULL;
   _uncompressed=NULL;
   cleanup();
}
#define BYTE uint8_t 
#ifndef MAX
#define MAX(x,y) ((x)>(y) ?(x):(y))
#endif
#ifndef MIN
#define MIN(x,y) ((x)<(y) ?(x):(y))
#endif
//_____________________________________________________________
// It is slow as we should do the resize in place (one 
//   bunch of memcpy saved)
// Since it is killing the editor cache anyway, speed it not an issue
//______________________________________________________________
uint8_t ADMVideoAnimated::getFrameNumberNoAlloc(uint32_t frame,
  uint32_t *len,
  ADMImage *data,
  uint32_t *flags)
  {
    ADMImage *in;
    uint32_t offset,pool;

    // Clean the image
    data->blacken();
 const int x_coordinate[3]={LEFT_MARGIN,360-(_param->vignetteW)/2,720-(_param->vignetteW)-LEFT_MARGIN};
 const int y_coordinate[2]={TOP_MARGIN,TOP_MARGIN+(_param->vignetteH)+TOP_MARGIN};

     // Do 3 top
     for(int line=0;line<2;line++)
        for(int i=0;i<3;i++)
        {
            pool=i+(3*line);
            if(_param->timecode[pool]+frame<_in->getInfo()->nb_frames) 
            {
                in=_caches[pool]->getImage(_param->timecode[pool]+frame);
                if(in) 
                {
                     _resizer->resize(in,_image);
                    _caches[pool]->unlockAll();
                }else
                {   
                    _image->blacken();
                }
            }else
            {   // Blacken
                 _image->blacken();
            }
            // Blit

            _image->copyTo(data,x_coordinate[i],y_coordinate[line]);
        }

	return 1;

}


uint8_t	ADMVideoAnimated::getCoupledConf( CONFcouple **couples)
{
   
      ADM_assert(_param);
      *couples=new CONFcouple(10);
#define CSET(x)  (*couples)->setCouple((char *)#x,(_param->x))
       CSET(isNTSC);
       CSET(backgroundImg);
        CSET(vignetteW);
        CSET(vignetteH);
#undef CSET
#define CSET(x)  (*couples)->setCouple((char *)"tc"#x,(_param->timecode[x]))
        CSET(0);
        CSET(1);
        CSET(2);
        CSET(3);
        CSET(4);
        CSET(5);

      return 1;
}



// EOF
