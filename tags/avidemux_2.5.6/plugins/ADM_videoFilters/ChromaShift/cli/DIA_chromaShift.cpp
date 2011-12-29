/***************************************************************************
                          ADM_guiChromaShift.cpp  -  description
                             -------------------
    begin                : Sun Aug 24 2003
    copyright            : (C) 2002-2003 by mean
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


#include "ADM_image.h"
#include "ADM_videoFilter.h"

#include "ADM_assert.h"
#include "DIA_flyDialog.h"
#include "../ADM_vidChromaShift_param.h"
#include "DIA_flyChromaShift.h"
uint8_t DIA_getChromaShift( AVDMGenericVideoStream *instream,CHROMASHIFT_PARAM    *param );
uint8_t DIA_getChromaShift( AVDMGenericVideoStream *in,CHROMASHIFT_PARAM    *param )
{
        return 1;
}
uint8_t    flyChromaShift::upload(void)
{
  return 1;
}
uint8_t    flyChromaShift::download(void)
{
  
  return 1;
}


