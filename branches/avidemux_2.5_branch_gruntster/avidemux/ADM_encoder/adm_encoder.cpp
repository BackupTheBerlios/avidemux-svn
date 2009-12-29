/***************************************************************************
                          adm_encoder.cpp  -  description
                             -------------------
    begin                : Sun Jul 14 2002
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

#include "ADM_assert.h"
#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_encoder/adm_encoder.h"

Encoder::~Encoder (void)
{
#define CLEAN(x) if(x) { delete [] x;x=NULL;}

  //CLEAN(_vbuffer);
  if (_vbuffer)
    delete _vbuffer;
  _vbuffer = NULL;
  CLEAN (entries);

}
