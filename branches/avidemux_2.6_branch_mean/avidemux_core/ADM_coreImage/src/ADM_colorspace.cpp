/***************************************************************************
    copyright            : (C) 2007 by mean
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


#include "ADM_default.h"
#include "ADM_colorspace.h"


extern "C" {
#include "ADM_ffmpeg/libavcodec/avcodec.h"
#include "ADM_ffmpeg/libavutil/avutil.h"
#include "ADM_ffmpeg/libswscale/swscale.h"
}

#include "ADM_rgb.h" 
#include "ADM_colorspace.h"

#ifdef ADM_CPU_X86
		#define ADD(x,y) if( CpuCaps::has##x()) flags|=SWS_CPU_CAPS_##y;
#define FLAGS()		ADD(MMX,MMX);				ADD(3DNOW,3DNOW);		ADD(MMXEXT,MMX2);
#else
#ifdef ADM_CPU_ALTIVEC
#define FLAGS() flags|=SWS_CPU_CAPS_ALTIVEC;
#else
#define FLAGS()
#endif
#endif

#define CONTEXT (SwsContext *)context

/**
    \fn ADMColor2LAVColor
    \brief Convert ADM colorspace type swscale/lavcodec colorspace name

*/
static PixelFormat ADMColor2LAVColor(ADM_colorspace fromColor)
{
  switch(fromColor)
  {
    case ADM_COLOR_YV12: return PIX_FMT_YUV420P;
    case ADM_COLOR_YUV422P: return PIX_FMT_YUV422P;
    case ADM_COLOR_RGB32A: return PIX_FMT_RGB32;
    case ADM_COLOR_BGR32A: return PIX_FMT_RGB32; // Faster that way...PIX_FMT_BGR32;
    case ADM_COLOR_RGB24: return PIX_FMT_RGB24;
    default : ADM_assert(0); 
  }
  return PIX_FMT_YUV420P;
}
/**
      \fn getStrideAndPointers
      \param dst=1 -> destination, =0 source
      \brief Fill in strides etc.. needed by libswscale
*/
uint8_t ADMColorScalerFull::getStrideAndPointers(bool dst,
        uint8_t  *from,ADM_colorspace fromColor,
        uint8_t **srcData,int *srcStride)
{
    uint32_t width,height;
    if(!dst)
    {
        width=srcWidth;
        height=srcHeight;
    }else
    {
        width=dstWidth;
        height=dstHeight;
    }
  switch(fromColor)
  {
    case ADM_COLOR_RGB24:
            srcData[0]=from;
            srcData[1]=NULL;
            srcData[2]=NULL;
            srcStride[0]=width*3;
            srcStride[1]=0;
            srcStride[2]=0;
            break;
    case  ADM_COLOR_YV12:
            srcData[0]=from;
            srcData[1]=from+width*height;
            srcData[2]=from+((5*width*height)>>2);
            srcStride[0]=width;
            srcStride[1]=width>>1;
            srcStride[2]=width>>1;
            break;
    case  ADM_COLOR_YUV422P:
            srcData[0]=from;
            srcData[1]=from+width*height;
            srcData[2]=from+((3*width*height)>>1);
            srcStride[0]=width;
            srcStride[1]=width>>1;
            srcStride[2]=width>>1;
            break;
    case ADM_COLOR_RGB32A:
    case ADM_COLOR_BGR32A:
            srcData[0]=from;
            srcData[1]=NULL;
            srcData[2]=NULL;
            srcStride[0]=width*4;
            srcStride[1]=0;
            srcStride[2]=0;
            break;
    
    default:
        ADM_assert(0);
  }
  return 1;
}
/**
    \fn  convert
    \brief Do the color conversion
  @param from Source image
  @param to Target image
*/
#define swap16(x) x=((x>>8)&0xff)+(x<<8)
bool ADMColorScalerFull::convert(uint8_t  *from, uint8_t *to)
{
  uint8_t *srcData[3];
  uint8_t *dstData[3];
  int srcStride[3];
  int dstStride[3];
  
  getStrideAndPointers(false,from,fromColor,srcData,srcStride);
  getStrideAndPointers(true,to,toColor,dstData,dstStride);
  sws_scale(CONTEXT,srcData,srcStride,0,srcHeight,dstData,dstStride);
  if(toColor==ADM_COLOR_BGR32A)
  {
    // RGB32->BG32
    uint32_t l=dstWidth*dstHeight;
    uint8_t *d=(uint8_t *)to;
    int r,b,g,a;
    while(l--)
    {
        uint8_t s=d[0];
        d[0]=d[2];
        d[2]=s;
        d+=4;
        
    }
  }
  return true;
  
}
/**
    \fn convertPlanes
    \brief Same as convert but the 3 planes are given separately
*/
bool            ADMColorScalerFull::convertPlanes(uint32_t  sourceStride[3],uint32_t destStride[3],     
                                  uint8_t   *sourceData[3], uint8_t *destData[3])
{
    int xs[3]={sourceStride[0],sourceStride[1],sourceStride[2]};
    int xd[3]={destStride[0],destStride[1],destStride[2]};
     sws_scale(CONTEXT,sourceData,xs,0,srcHeight,destData,xd);
     return true;
}
/**
    \fn  ADMColorScaler
    \brief Constructor
  @param w width
  @param h height
  @param from colorspace to convert from
  @param to colorspace to concert to
*/

ADMColorScalerFull::ADMColorScalerFull(ADMColorScaler_algo algo,
            uint32_t sw, uint32_t sh,
            uint32_t dw, uint32_t dh,
            ADM_colorspace from,ADM_colorspace to)
{
   context=NULL;
   reset(algo,sw,sh,dw,dh,from,to);

}
/**
    \fn  ~ADMColorScaler
    \brief Destructor
*/
ADMColorScalerFull::~ADMColorScalerFull()
{
  if(context)
  {
     sws_freeContext(CONTEXT);
     context=NULL;
  }
}
/**
    \fn reset
*/
bool  ADMColorScalerFull::reset(ADMColorScaler_algo algo, uint32_t sw, uint32_t sh, uint32_t dw,uint32_t dh,ADM_colorspace from,ADM_colorspace to)
{
    if(context) sws_freeContext(CONTEXT);
    context=NULL;
    this->algo=algo;
    int flags;
    switch(algo)
    {
#define SETAL(x) case ADM_CS_##x: flags=SWS_##x;break;

    SETAL(BILINEAR);
    SETAL(BICUBIC);
    SETAL(LANCZOS);
    SETAL(BICUBLIN);
    SETAL(GAUSS);
    SETAL(SINC);
    SETAL(SPLINE);
    default: ADM_assert(0);
    }

    FLAGS();
  
    srcWidth=sw;
    srcHeight=sh;

    dstWidth=dw;
    dstHeight=dh;

    fromColor=from;
    toColor=to;
    PixelFormat lavFrom=ADMColor2LAVColor(fromColor);
    PixelFormat lavTo=ADMColor2LAVColor(toColor);
    
    context=(void *)sws_getContext(
                      srcWidth,srcHeight,
                      lavFrom ,
                      dstWidth,dstHeight,
                      lavTo,
                      flags, NULL, NULL,NULL);
}
//------------------------------
bool            ADMColorScalerSimple::changeWidthHeight(uint32_t newWidth, uint32_t newHeight)
{
    if(newWidth==srcWidth && newHeight==srcHeight) return true; // no change
    
     return reset(algo, newWidth,newHeight, newWidth,newHeight,fromColor,toColor);

}


/**
    \fn ADM_ConvertRgb24ToYV12
*/
bool ADM_ConvertRgb24ToYV12(bool inverted,uint32_t w, uint32_t h, uint8_t *source, uint8_t *destination) 
{
    ADMColorScalerSimple  converter( w,h,ADM_COLOR_RGB24,ADM_COLOR_YV12);
    
    if(true==inverted)
    {
        uint8_t a,b,c;
        uint8_t *src=source;
        uint32_t len=w*h;
        while(len--)
        {
            a=src[0];b=src[1];c=src[2];
            src[0]=c;src[2]=a;//src[1]=b;
            src+=3;
        }
    }
    if(false==converter.convert(source,destination)) return false;
    return true;
}

//EOF
