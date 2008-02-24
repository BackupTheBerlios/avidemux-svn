/***************************************************************************
                          ADM_codecs.cpp  -  description
                             -------------------
    begin                : Fri Apr 12 2002
    copyright            : (C) 2002 by mean
    email                : fixounet@free.fr

    see here : http://www.webartz.com/fourcc/

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
#include <math.h>



#ifdef USE_FFMPEG
extern "C"
{
#include "ADM_lavcodec.h"
};
#endif
#include "ADM_default.h"
#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif


#include "ADM_colorspace/colorspace.h"
#ifdef USE_XX_XVID
#include "xvid.h"
#endif


#include "ADM_codecs/ADM_codec.h"
#include "ADM_codecs/ADM_mjpeg.h"
#include "ADM_codecs/ADM_codecNull.h"
#include "ADM_codecs/ADM_rgb16.h"
#include "ADM_codecs/ADM_uyvy.h"
#include "ADM_codecs/ADM_xvideco.h"

#include "ADM_utilities/fourcc.h"

#ifdef USE_FFMPEG
#include "ADM_codecs/ADM_ffmp43.h"
#endif

#ifdef USE_DIVX
#include "ADM_codecs/ADM_divx4.h"
#endif

#ifdef USE_THEORA
#include "ADM_codecs/ADM_theora_dec.h"
#endif
#include "ADM_codecs/ADM_mpeg.h"
#include "ADM_codecs/ADM_png.h"
#include "ADM_toolkit/toolkit.hxx"
#include <ADM_assert.h>
#include "prefs.h"

extern uint8_t GUI_Question (char *);
extern uint8_t use_fast_ffmpeg;
uint8_t isMpeg12Compatible (uint32_t fourcc);
uint8_t
isMpeg4Compatible (uint32_t fourcc)
{
#define CHECK(x) if(fourCC::check(fourcc,(uint8_t *)x)) \
						{divx4=1; }

  uint8_t divx4 = 0;

  CHECK ("FMP4");
  CHECK ("fmp4");
  CHECK ("DIVX");
  CHECK ("divx");
  CHECK ("DX50");
  CHECK ("xvid");
  CHECK ("XVID");
  CHECK ("BLZ0");
  CHECK ("M4S2");
  CHECK ("3IV2");

  return divx4;

#undef CHECK
}
#ifdef ADM_BIG_ENDIAN
#define SWAP32(x) x=R32(x)
#else
#define SWAP32(x) ;
#endif

uint8_t
isMpeg12Compatible (uint32_t fourcc)
{
#define CHECK(x) if(fourCC::check(fourcc,(uint8_t *)x)) \
						{mpeg=1; }

  uint8_t mpeg = 0;
  CHECK ("MPEG");
  CHECK ("mpg1");
  CHECK ("mpg2");
  SWAP32 (fourcc);
  if (fourcc == 0x10000002 || fourcc==0x10000001) //Mplayer fourcc
    mpeg = 1;
  return mpeg;
#undef CHECK
}
uint8_t
isH264Compatible (uint32_t fourcc)
{
#define CHECK(x) if(fourCC::check(fourcc,(uint8_t *)x)) \
                                                {h264=1; }

  uint8_t h264 = 0;

  CHECK ("X264");
  CHECK ("x264");
  CHECK ("h264");
  CHECK ("H264");
  CHECK ("AVC1");
  CHECK ("avc1");
  return h264;

#undef CHECK
}

uint8_t
isMSMpeg4Compatible (uint32_t fourcc)
{
#define CHECK(x) if(fourCC::check(fourcc,(uint8_t *)x)) \
						{divx3=1; }

  uint8_t divx3 = 0;

  CHECK ("MP43");
  CHECK ("mp43");
  CHECK ("div3");
  CHECK ("DIV3");
  CHECK ("DIV4");
  CHECK ("div4");
  CHECK ("COL1");

  return divx3;

#undef CHECK
}
uint8_t
isDVCompatible (uint32_t fourcc)
{
#define CHECK(x) if(fourCC::check(fourcc,(uint8_t *)x)) \
						{dv=1; }

  uint8_t dv = 0;

  CHECK ("dvsd");
  CHECK ("DVDS");
  CHECK ("dvpp");

  return dv;

#undef CHECK
}


uint8_t
  decoders::uncompress (ADMCompressedImage * in, ADMImage * out)
{
  UNUSED_ARG (in);
  UNUSED_ARG (out);
  return 0;
}
decoders *
getDecoderVopPacked (uint32_t fcc, uint32_t w, uint32_t h, uint32_t extraLen,
		     uint8_t * extraData)
{
  UNUSED_ARG (fcc);
  UNUSED_ARG (extraLen);
  UNUSED_ARG (extraData);
  return (decoders *) (new decoderFFMpeg4VopPacked (w, h));

}
decoders *getDecoderH264noLogic (uint32_t fcc, uint32_t w, uint32_t h, uint32_t extraLen,
		     uint8_t * extraData)
{
  UNUSED_ARG (fcc);
  UNUSED_ARG (extraLen);
  UNUSED_ARG (extraData);
  return (decoders *) (new decoderFFH264 (w, h, extraLen, extraData,0));

}
decoders *
getDecoder (uint32_t fcc, uint32_t w, uint32_t h, uint32_t extraLen,
	    uint8_t * extraData,uint32_t bpp)
{
  printf("\nSearching decoder (%d x %d, extradataSize:%d)...\n",w,h,extraLen);
  if (isMSMpeg4Compatible (fcc) == 1)
    {
      // For div3, no problem we take ffmpeg

      return (decoders *) (new decoderFFDiv3 (w, h));
    }

#ifdef USE_FFMPEG
  if (isDVCompatible(fcc))//"CDVC"))
    {

      return (decoders *) (new decoderFFDV (w, h, extraLen, extraData));
    }
#endif
#ifdef USE_FFMPEG
  if (fourCC::check (fcc, (uint8_t *) "MP42"))
    {

      return (decoders *) (new decoderFFMP42 (w, h));
    }
#endif
    if (fourCC::check (fcc, (uint8_t *) "FLV1"))
    {
      return (decoders *) (new decoderFFFLV1 (w, h,extraLen, extraData));
    }
  
    
#ifdef USE_FFMPEG
  if (fourCC::check (fcc, (uint8_t *) "H263"))
    {

      return (decoders *) (new decoderFFH263 (w, h));
    }
  if (fourCC::check (fcc, (uint8_t *) "HFYU"))
    {

      return (decoders *) (new decoderFFhuff (w, h, extraLen, extraData,bpp));
    }
#ifdef USE_PNG
  if (fourCC::check (fcc, (uint8_t *) "PNG "))
    {

      return (decoders *) (new decoderPng (w, h));
    }
#endif
 if (fourCC::check (fcc, (uint8_t *) "cvid"))
    {

      return (decoders *) (new decoderFFCinepak (w, h, extraLen, extraData));
    }
  if (fourCC::check (fcc, (uint8_t *) "FFVH"))
    {

      return (decoders *) (new decoderFF_ffhuff (w, h, extraLen, extraData,bpp));
    }
  if (fourCC::check (fcc, (uint8_t *) "SVQ3"))
    {

      return (decoders *) (new decoderFFSVQ3 (w, h, extraLen, extraData));
    }
  if (fourCC::check (fcc, (uint8_t *) "tscc"))
    {

      return (decoders *) (new decoderCamtasia (w, h, bpp));
    }

     if (fourCC::check (fcc, (uint8_t *) "CRAM"))
    {

      return (decoders *) (new decoderFFCRAM (w, h, extraLen, extraData));
    }
  if (fourCC::check (fcc, (uint8_t *) "WMV2"))
    {

      return (decoders *) (new decoderFFWMV2 (w, h, extraLen, extraData));
    }
    if (fourCC::check (fcc, (uint8_t *) "WMV1"))
    {

      return (decoders *) (new decoderFFWMV1 (w, h, extraLen, extraData));
    }
  
  if (fourCC::check (fcc, (uint8_t *) "WMV3"))
    {

      return (decoders *) (new decoderFFWMV3 (w, h, extraLen, extraData));
    }

    if (fourCC::check (fcc, (uint8_t *) "WVC1"))
    {

      return (decoders *) (new decoderFFVC1 (w, h, extraLen, extraData));
    }

if (fourCC::check (fcc, (uint8_t *) "FFV1"))
    {

      return (decoders *) (new decoderFFV1 (w, h));
    }
  if (fourCC::check (fcc, (uint8_t *) "SNOW"))
    {

      return (decoders *) (new decoderSnow (w, h));
    }
  if (isH264Compatible (fcc))
    {

      return (decoders *) (new decoderFFH264 (w, h, extraLen, extraData,1));
    }
#endif

/*
	Could be either divx5 packed crap or xvid or ffmpeg
	For now we return FFmpeg and later will switch to divx5 if available
		(ugly hack for ugly hack....)
*/

  if (isMpeg4Compatible (fcc) == 1)
    {
      return (decoders *) (new decoderFFMpeg4 (w, h, fcc,extraLen, extraData));
      //    return(decoders *)( new decoderXvid(w,h));
      //    return(decoders *)( new decoderDIVX(w,h));
    }

  if (fourCC::check (fcc, (uint8_t *) "MJPB"))
    {
      printf ("\n using FF mjpeg codec\n");
      return (decoders *) (new decoderFFMjpegB (w, h,extraLen,extraData));
    }
if (fourCC::check (fcc, (uint8_t *) "MJPG")
      || fourCC::check (fcc, (uint8_t *) "mjpa"))
    {
#if  0
      //#ifdef USE_MJPEG
      printf ("\n using mjpeg codec\n");
      return (decoders *) (new decoderMjpeg (w, h));
#else
      printf ("\n using FF mjpeg codec\n");
      return (decoders *) (new decoderFFMJPEG (w, h));
#endif

    }
  if (fourCC::check (fcc, (uint8_t *) "YV12")
      || fourCC::check (fcc, (uint8_t *) "I420"))
    {
      printf ("\n using null codec\n");
      return (decoders *) (new decoderNull (w, h));
    }
  if (fourCC::check (fcc, (uint8_t *) "UYVY"))
    {
      printf ("\n using uyvy codec\n");
      return (decoders *) (new decoderUYVY (w, h));
    }
  if (fourCC::check (fcc, (uint8_t *) "YUY2"))
    {
      printf ("\n using YUY2 codec\n");
      return (decoders *) (new decoderYUY2 (w, h));
    }
  if (fourCC::check (fcc, (uint8_t *) "AMV "))
    {
      printf ("\n using AMV codec\n");
      return (decoders *) (new decoderFFAMV (w, h,extraLen,extraData));
    }

    
  if (fourCC::check (fcc, (uint8_t *) "VP6F"))
    {
      printf ("\n using VP6F codec\n");
      return (decoders *) (new decoderFFVP6F (w, h,extraLen,extraData));
    }


  if ((fcc == 0) || fourCC::check (fcc, (uint8_t *) "RGB "))
    {
      // RGB 16 Codecs
      printf ("\n using RGB codec\n");
      return (decoders *) (new decoderRGB16 (w, h, 0, bpp));

    }
 if ((fcc == 0) || fourCC::check (fcc, (uint8_t *) "DIB "))
    {
      // RGB 16 Codecs
      printf ("\n using RGB-DIB codec\n");
      return (decoders *) (new decoderRGB16 (w, h, 1, bpp));

    }
  if (isMpeg12Compatible (fcc))
    {
      uint32_t lavcodec_mpeg = 0;
      printf ("\n using Mpeg1/2 codec (libmpeg2)\n");
      if (!prefs->get (FEATURE_USE_LAVCODEC_MPEG, &lavcodec_mpeg))
	{
	  lavcodec_mpeg = 0;
	}
      if (lavcodec_mpeg)
	{
	  return (decoders *) (new
			       decoderFFMpeg12 (w, h, extraLen, extraData));
	}
      else
	{
	  return (decoders *) (new decoderMpeg (w, h, extraLen, extraData));
	}
      //  
    }

  // default : null decoder
  printf ("\n using invalid codec for \n");
  fourCC::print (fcc);

  return (decoders *) (new decoderEmpty (w, h));



}

uint8_t coders::compress (ADMImage * in, ADMBitstream * out)
{
  UNUSED_ARG (in);
  UNUSED_ARG (out);

  return 0;
}
