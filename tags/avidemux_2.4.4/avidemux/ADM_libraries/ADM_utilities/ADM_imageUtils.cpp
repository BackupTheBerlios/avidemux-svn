/***************************************************************************
    copyright            : (C) 2003-2005 by mean
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ADM_assert.h"
#include "default.h"

#include "ADM_image.h"
#include "ADM_toolkit/bitmap.h"
#include "avifmt.h"
#include "ADM_utilities/avifmt2.h"
#include "ADM_utilities/ADM_bitstream.h"
#include "ADM_osSupport/ADM_cpuCap.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_colorspace/colorspace.h"
#include "ADM_encoder/ADM_vidEncode.hxx"

#include "ADM_codecs/ADM_codec.h"
#include "ADM_lavcodec.h"
#include "ADM_codecs/ADM_ffmpeg.h"
#include "admmangle.h"
static uint8_t tinyAverage(uint8_t *dst, uint8_t *src1, uint8_t *src2,uint32_t l)
{

uint8_t *s1,*s2,*d1;
int a1,a2;
        s1=src1;
        s2=src2;
        
        d1=dst;
          for(int y=0;y<l;y++)
                {
                        a1=*s1;
                        a2=*s2;
                        a1=a1+a2;
                        a1>>=1;
                        if(a1<0) a1=0;
                        if(a1>255) a1=255;
                        *d1=a1;                         

                        s1++;
                        s2++;
                        d1++;
                }
        
        return 1;
}
#if defined( ARCH_X86 ) || defined (ARCH_X86_64)
static uint8_t tinyAverageMMX(uint8_t *dst, uint8_t *src1, uint8_t *src2,uint32_t l)
{
int delta;
uint32_t ww,rr;
uint8_t *s1,*s2,*d1;
int a1,a2;
        s1=src1;
        s2=src2;
        
        d1=dst;
        ww=l>>2;
        rr=l&3;


#ifdef GCC_2_95_X
         __asm__(
                         "pxor %mm7,%mm7"
                ::
                 );
#else
         __asm__(
                         "pxor %%mm7,%%mm7"
                ::
                 );
#endif

          for(int y=0;y<ww;y++)
          {
                __asm__(
                        "movd           (%0),%%mm0 \n"
                        "movd           (%1),%%mm1 \n"
                        "punpcklbw      %%mm7,%%mm0 \n"
                        "punpcklbw      %%mm7,%%mm1 \n"
                        "paddw           %%mm1,%%mm0 \n"
                        "psrlw          $1,%%mm0 \n"
                        "packuswb       %%mm0,  %%mm0\n"
                        "movd           %%mm0,(%2) \n"

                : : "r" (s1),"r" (s2),"r"(d1)
                );
                        s1+=4;
                        s2+=4;
                        d1+=4;
                }
        __asm__(
                        "emms\n"
                ::
                );
        if(rr) tinyAverage(d1, s1, s2,rr);
        return 1;
}


#endif

uint8_t ADMImage::merge(ADMImage *src1,ADMImage *src2)
{
#if 1 && defined( ARCH_X86 ) || defined (ARCH_X86_64)
        if(CpuCaps::hasMMX())
        {
                tinyAverageMMX(YPLANE(this),YPLANE(src1),YPLANE(src2),(_width*_height*3)>>1);
                return 1;
        }
#endif
        tinyAverage(YPLANE(this),YPLANE(src1),YPLANE(src2),(_width*_height*3)>>1);
        return 1;


}
/* 3000 * 3000 max size, using uint32_t is safe... */
static uint32_t computeDiff(uint8_t  *s1,uint8_t *s2,uint32_t noise,uint32_t l)
{
uint32_t df=0;
uint32_t delta;
uint32_t ww,hh;


        for(int x=0;x<l;x++)
        {
                delta=abs(*s1-*s2);
                if(delta>noise)
                        df+=delta;
                s1++;
                s2++;

        }
        return df;
}
#if defined( ARCH_X86 ) || defined (ARCH_X86_64)
static uint64_t noise64 asm(MANGLE(noise64));
static uint32_t computeDiffMMX(uint8_t  *s1,uint8_t *s2,uint32_t noise,uint32_t l)
{
uint32_t df=0;
uint32_t delta;
uint32_t ll,rr;
uint64_t noise2=(uint64_t )noise;

uint32_t result=0;
        noise64=noise2+(noise2<<16)+(noise2<<32)+(noise2<<48);
        ll=l>>2;
        rr=l&3;

#ifdef GCC_2_95_X
         __asm__(
                         "pxor %mm7,%mm7\n"
                         "pxor %mm3,%mm3\n"
                         "movq "Mangle(noise64)", %mm6\n"
                :: 
                 );
#else
         __asm__(
                         "pxor %%mm7,%%mm7\n"
                         "pxor %%mm3,%%mm3\n"
                         "movq "Mangle(noise64)", %%mm6\n"
                :: 
                 );
#endif

          for(int y=0;y<ll;y++)
          {
                __asm__(
                        "movd           (%0),  %%mm0 \n"
                        "movd           (%1),  %%mm1 \n"
                        "punpcklbw      %%mm7, %%mm0 \n"
                        "punpcklbw      %%mm7, %%mm1 \n"
                        
                        "movq           %%mm0, %%mm2 \n"
                        "psubusw        %%mm1, %%mm2 \n"
                        "psubusw        %%mm0, %%mm1 \n"
                        "por            %%mm1, %%mm2 \n" // SAD  
                                           
                        "movq           %%mm2, %%mm0 \n"
                        "pcmpgtw        %%mm6, %%mm2 \n" // Threshold against noise
                        "pand           %%mm2, %%mm0 \n" // MM0 is the 4 diff, time to pack

                        "movq           %%mm0, %%mm1 \n" // MM0 is a b c d and we want
                        "psrlq          $16,  %%mm1 \n"  // mm3+=a+b+c+d

                        "movq           %%mm0, %%mm2 \n"
                        "psrlq          $32,  %%mm2 \n"

                        "movq           %%mm0, %%mm4 \n"
                        "psrlq          $48,  %%mm4 \n"

                        "paddw          %%mm1, %%mm0 \n"
                        "paddw          %%mm2, %%mm4 \n"
                        "paddw          %%mm4, %%mm0 \n"

                        "psllq          $48,  %%mm0 \n"
                        "psrlq          $48,  %%mm0 \n"

                        "paddw          %%mm0, %%mm3 \n" /* PADDQ is SSE2 */

                : : "r" (s1),"r" (s2)
                );
                        s1+=4;
                        s2+=4;
                        
         }
        // Pack result
#if 1        
                __asm__(
                       
                        "movd           %%mm3,(%0)\n"
                        "emms\n"
                :: "r"(&result)
                );
#endif                
        if(rr) result+=computeDiff(s1, s2, noise,rr);
        return result;
}


#endif

uint32_t ADMImage::lumaDiff(ADMImage *src1,ADMImage *src2,uint32_t noise)
{

#if 1 && defined( ARCH_X86 ) || defined (ARCH_X86_64)
uint32_t r1,r2;
        if(CpuCaps::hasMMX())
        {
                return computeDiffMMX(YPLANE(src1),YPLANE(src2),noise,src1->_width*src1->_height);                
        }
#endif
        return computeDiff(YPLANE(src1),YPLANE(src2),noise,src1->_width*src1->_height);
}

//******************************************************************************
// so srcR=2*src-srcP
static uint8_t tinySubstract(uint8_t *dst, uint8_t *src1, uint8_t *src2,uint32_t l)
{
int delta;
uint32_t ww,hh;
uint8_t *s1,*s2,*d1;
int a1,a2;
        s1=src1;
        s2=src2;
        
        d1=dst;
       

          for(int y=0;y<l;y++)
               
                {
                        a1=*s1;
                        a2=*s2;
                        a1=2*a1-a2;
                        if(a1<0) a1=0;
                        if(a1>255) a1=255;
                        *d1=a1;                         

                        s1++;
                        s2++;
                        d1++;
                }
        return 1;
}
#if defined( ARCH_X86 ) || defined (ARCH_X86_64)
static uint8_t tinySubstractMMX(uint8_t *dst, uint8_t *src1, uint8_t *src2,uint32_t l)
{
int delta;
uint32_t ww,hh;
uint8_t *s1,*s2,*d1;
int ll,rr;
        ll=l>>2;
        rr=l&3;
        s1=src1;
        s2=src2;
        
        d1=dst;
      
#ifdef GCC_2_95_X
        __asm__(
                         "pxor %mm7,%mm7"
                ::
                 );
#else
        __asm__(
                         "pxor %%mm7,%%mm7"
                ::
                 );
#endif
        for(int x=0;x<ll;x++)
                {
                        __asm__(
                        "movd           (%0),%%mm0 \n"
                        "movd           (%1),%%mm1 \n"
                       
                        "punpcklbw      %%mm7,%%mm0 \n"
                        "punpcklbw      %%mm7,%%mm1 \n"
                      
                        
                        "paddw          %%mm0,%%mm0 \n"
                       
                        
                        "psubusw        %%mm1,%%mm0 \n" // mm1=sum                       
                        "packuswb       %%mm0,  %%mm0\n"
                        "movd           %%mm0,(%2) \n"                       
                        :: "r"(s1),"r"(s2),"r"(d1)
                        );
                        s1+=4;
                        s2+=4;
                        d1+=4;
                }
                 __asm__(                       
                        "emms\n"
                :: 
                );
        if(rr) tinySubstractMMX(d1, s1, s2,rr);
        return 1;
}
#endif

uint8_t ADMImage::substract(ADMImage *src1,ADMImage *src2)
{

#if 1 && defined( ARCH_X86 ) || defined (ARCH_X86_64)
uint32_t r1,r2;
        if(CpuCaps::hasMMX())
        {
                 return tinySubstractMMX(YPLANE(this),YPLANE(src1),YPLANE(src2),src1->_width*src1->_height);                
        }
#endif
        return tinySubstract(YPLANE(this),YPLANE(src1),YPLANE(src2),src1->_width*src1->_height);
}
 BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmph;
  FILE *fd;
  uint32_t sz;
  uint16_t s16;
  uint32_t s32;
  

/**
    \fn saveAsBmp
    \brief save current image into filename, into bmp format
*/
uint8_t  ADMImage::saveAsBmp(const char *filename)
{
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmph;
  FILE *fd;
  uint32_t sz;
  uint16_t s16;
  uint32_t s32;
  
  sz = _width* _height * 3;

  bmfh.xHotspot = bmfh.yHotspot = 0;
  bmfh.offsetToBits = sizeof (bmfh) + sizeof (bmph);
//_________________________________________
  bmph.biSize = sizeof (bmph);
  bmph.biWidth = _width;
  bmph.biHeight = _height;
  bmph.biPlanes = 1;
  bmph.biBitCount = 24;
  bmph.biCompression = 0;	// COMPRESSION NONE
  bmph.biSizeImage = sz;
  bmph.biXPelsPerMeter = 0;
  bmph.biYPelsPerMeter = 0;
  bmph.biClrUsed = 0;
  bmph.biClrImportant = 0;
/*
	bmph.resolutionUnits=0;
	bmph.origin=0;
	bmph.colorEncoding=0;
*/

  ADMImage image(_width,_height);
  

  printf ("\n %u x %u=%u\n", bmph.biWidth, bmph.biHeight, sz);

  uint8_t *out;

        out=(uint8_t *)ADM_alloc(sz);
        if(!out)
        {
            GUI_Error_HIG(QT_TR_NOOP("Memory error"), NULL);
//            ADM_dealloc(out);
            return 0;
        }

        if(!COL_yv12rgbBMP(bmph.biWidth, bmph.biHeight,data, out))
        {
              GUI_Error_HIG(QT_TR_NOOP("Error converting to BMP"), NULL);
              ADM_dealloc(out);
              return 0;
        }
        fd = fopen (filename, "wb");
        if (!fd)
        {
                GUI_Error_HIG (QT_TR_NOOP("Something bad happened"), NULL);
                ADM_dealloc(out);
                return 0;
        }

	// Bitmpap file header, not using tructure due to gcc padding it
#ifdef ADM_BIG_ENDIAN
	s16 = 0x424D;
#else	
  	s16 = 0x4D42;
#endif	
  	s32 = 14 + sizeof (bmph) + sz;
#ifdef ADM_BIG_ENDIAN	
	#define SWAP32(x) x=R32(x)	
#else
	#define SWAP32(x) ; 
#endif
        SWAP32(s32);	
        fwrite (&s16, 2, 1, fd);
        fwrite (&s32, 4, 1, fd);
        s32 = 0;
        fwrite (&s32, 4, 1, fd);
        s32 = 14 + sizeof (bmph);
        SWAP32(s32);
        fwrite (&s32, 4, 1, fd);
#ifdef ADM_BIG_ENDIAN
	Endian_BitMapInfo(&bmph);
#endif
        fwrite (&bmph, sizeof (bmph), 1, fd);
        fwrite (out, sz, 1, fd);
  
        fclose(fd);
        ADM_dealloc(out);
        return 1;
}
/**
    \fn saveAsJpg
    \brief save current image into filename, into jpg format
*/
uint8_t  ADMImage::saveAsJpg(const char *filename)
{
 ffmpegEncoderFFMjpeg *codec=NULL;
  FILE *fd;
  uint8_t *buffer=NULL;
  uint32_t sz;
  

        sz = _width*_height*3;
        ADMBitstream bitstream(sz);
        buffer=new uint8_t[sz];
        bitstream.data=buffer;
        codec=new  ffmpegEncoderFFMjpeg(_width,_height,FF_MJPEG)  ;
        codec->init( 95,25000);
        if(!codec->encode(this,&bitstream))
        {
                GUI_Error_HIG(QT_TR_NOOP("Cannot encode the frame"), NULL);
                delete [] buffer;
                delete codec;
                return 0;
        }
        delete codec;
        fd=fopen(filename,"wb");
        if(!fd)
        {
                GUI_Error_HIG(QT_TR_NOOP("File error"),QT_TR_NOOP( "Cannot open \"%s\" for writing."), filename);
                delete [] buffer;
                return 0;
        }
        fwrite (buffer, bitstream.len, 1, fd);
        fclose(fd);
        delete [] buffer;
        return 1;
}
/**
      \fn LumaReduceBy2
      \brief Very simple reduce by 2 both hz & vz for luma plane only.
*/
 uint8_t   ADMImage::LumaReduceBy2(void)
{
  uint8_t *ptr1,*ptr2,*outptr;
  for(int y=0;y<_height>>1;y++)
  {
    
    ptr1=data+y*_width*2;
    ptr2=ptr1+_width;
    outptr=data+(_width>>1)*y;
    for(int x=0;x<_width>>1;x++)
    {
      uint32_t a,b,c,d;  
          a=ptr1[0]+ptr1[1];
          b=ptr2[0]+ptr2[1];
          ptr1+=2;
          ptr2+=2;
          a=(a+b)>>2;
          *outptr++=a;
    }
  }
  return 1;
  
}
 /**
  *		\fn  copyLeftSideTo
  * 	\brief Copy half the image (left part) to dest
  * 	@param dest : Image to copy to 
  */
 uint8_t ADMImage::copyLeftSideTo(ADMImage *dest)
 {
		uint8_t *src,*dst;
		uint32_t stride;
 
			ADM_assert(_width==dest->_width);
			ADM_assert(_height==dest->_height);
		
	 		dst=YPLANE(dest);
		    src=YPLANE(this);
		    stride=_width;
		    for(uint32_t y=0;y<_height;y++)   // We do both u & v!
		    {
		        memcpy(dst,src,stride>>1);
		        dst+=stride;
		        src+=stride;
		    }
		        // U
		    dst=UPLANE(dest);
		    src=UPLANE(this);
		    stride=_width>>1;
		    uint32_t h2=_height>>1;
		    for(uint32_t y=0;y<h2;y++)   // We do both u & v!
		    {
		        memcpy(dst,src,stride>>1);
		        dst+=stride;
		        src+=stride;
		    }
		        // V
		    dst=VPLANE(dest);
		    src=VPLANE(this);
		    for(uint32_t y=0;y<h2;y++)   // We do both u & v!
		    {
		        memcpy(dst,src,stride>>1);
		        dst+=stride;
		        src+=stride;
		    }
		    return 1;
 }
//EOF
