/***************************************************************************
                          ADM_vidContrast.cpp  -  description
                             -------------------
    begin                : Sun Sep 22 2002
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
#include <ADM_assert.h>
#include <math.h>

#include "config.h"
#include "fourcc.h"
#include "avio.hxx"
#include "config.h"
#include "avi_vars.h"


#include "ADM_toolkit/toolkit.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_videoFilter/ADM_vidContrast.h"
#include "ADM_filter/video_filters.h"



static FILTER_PARAM contrastParam =
  { 5, {"offset", "coef", "doLuma", "doChromaU", "doChromaV"} };


SCRIPT_CREATE (contrast_script, ADMVideoContrast, contrastParam);

BUILD_CREATE (contrast_create, ADMVideoContrast);

char *
ADMVideoContrast::printConf (void)
{
  static char buf[50];

  ADM_assert (_param);
  sprintf ((char *) buf, " contrast : %1.2f %d", _param->coef,
	   _param->offset);
  return buf;
}

//_______________________________________________________________

ADMVideoContrast::ADMVideoContrast (AVDMGenericVideoStream * in,
				    CONFcouple * couples)
{


  _in = in;
  memcpy (&_info, _in->getInfo (), sizeof (_info));
  _info.encoding = 1;
  //   _uncompressed=new uint8_t [3*_in->getInfo()->width*_in->getInfo()->height];
  _uncompressed =
    new ADMImage (_in->getInfo ()->width, _in->getInfo ()->height);
  ADM_assert (_uncompressed);
  _param = NULL;
  if (couples)
    {
      _param = NEW (CONTRAST_PARAM);
      GET (offset);
      GET (coef);
      GET (doLuma);
      GET (doChromaU);
      GET (doChromaV);
    }
  else
    {
      _param = NEW (CONTRAST_PARAM);
      _param->offset = 0;
      _param->coef = 1.0f;
      _param->doLuma = 1;
      _param->doChromaU = 1;
      _param->doChromaV = 1;

    }
  buildContrastTable (_param->coef, _param->offset, _tableFlat, _tableNZ);


}


uint8_t ADMVideoContrast::getCoupledConf (CONFcouple ** couples)
{

  ADM_assert (_param);
  *couples = new CONFcouple (5);

#define CSET(x)  (*couples)->setCouple((char *)#x,(_param->x))
  CSET (offset);
  CSET (coef);
  CSET (doLuma);
  CSET (doChromaU);
  CSET (doChromaV);
  return 1;

}
ADMVideoContrast::~ADMVideoContrast ()
{
  if (_uncompressed)
    delete _uncompressed;
  _uncompressed = 0;
  DELETE (_param);
}

//
//      Remove y and v just keep U and expand it
//
uint8_t
  ADMVideoContrast::getFrameNumberNoAlloc (uint32_t frame,
					   uint32_t * len,
					   ADMImage * data, uint32_t * flags)
{
  //uint32_t x,w;

  ADM_assert (_param);
 if(frame>= _info.nb_frames) return 0;


  if (!_in->getFrameNumberNoAlloc (frame, len, _uncompressed, flags))
    return 0;
  *len = _info.width * _info.height + (_info.width * _info.height >> 1);

  uint32_t sz = _info.width * _info.height;
  // luma 
  if (_param->doLuma)
    {
      if (!doContrast (_uncompressed->data, data->data, _tableFlat,
		       _info.width, _info.height))
	return 0;
    }
  else
    {
      memcpy (data->data, _uncompressed->data, sz);
    }
  // ______u_____________         

  if (_param->doChromaU)
    {
      if (!doContrast (_uncompressed->data + sz, data->data + sz, _tableNZ,
		       _info.width >> 1, _info.height >> 1))
	return 0;
    }
  else
    {
      memcpy (data->data + sz, _uncompressed->data + sz, sz >> 2);
    }

  // ______v_____________         
  if (_param->doChromaV)
    {

      if (!doContrast
	  (_uncompressed->data + sz + (sz >> 2), data->data + sz + (sz >> 2),
	   _tableNZ, _info.width >> 1, _info.height >> 1))
	return 0;
    }
  else
    {
      memcpy (data->data + sz + (sz >> 2),
	      _uncompressed->data + sz + (sz >> 2), sz >> 2);
    }




  return 1;
}

uint8_t
buildContrastTable (float coef, int8_t off,
		    uint8_t * tableFlat, uint8_t * tableNZ)
{
  float f;

  for (uint32_t i = 0; i < 256; i++)
    {
      f = i;
      f *= coef;
//                                      f= (f-128)*coef+128;
      f += off;
      if (f < 0.)
	f = 0.;
      if (f > 255.)
	f = 255.;
      *(tableFlat + i) = (uint8_t) floor (f);

      f = i;
      f -= 128;
      f *= coef;
//                                      f= (f-128)*coef+128;

//                                      f+=off;
      if (f < -127.)
	f = -127.;
      if (f > 127.)
	f = 127.;
      f += 128.;
      *(tableNZ + i) = (uint8_t) floor (f);
    }
  return 0;
}

uint8_t
doContrast (uint8_t * in, uint8_t * out, uint8_t * table,
	    uint32_t w, uint32_t h)
{

  for (uint32_t y = h * w; y > 0; y--)
    {
      *out++ = table[*in++];
    }

  return 1;

}
uint8_t DIA_contrast(AVDMGenericVideoStream *astream,CONTRAST_PARAM *param);
uint8_t
ADMVideoContrast::configure (AVDMGenericVideoStream * instream)
{
  if(DIA_contrast(_in,_param))
  {
    buildContrastTable (_param->coef, _param->offset, _tableFlat, _tableNZ);
    return 1;
  }
  return 0;
}

// EOF
