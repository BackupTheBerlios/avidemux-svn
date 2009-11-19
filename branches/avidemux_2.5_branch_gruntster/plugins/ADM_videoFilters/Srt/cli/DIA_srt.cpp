/***************************************************************************
                          ADM_guiSRT.cpp  -  description
                             -------------------
    begin                : Wed Dec 18 2002
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
#include "DIA_flyDialog.h"


#include "DIA_fileSel.h"

#include "ADM_videoFilter.h"

class ADMfont;
#include "ADM_vidSRT.h"
#include "DIA_flySrtPos.h"
#include "ADM_colorspace.h"

/**
      \fn DIA_srtPos
      \brief Dialog that handles subtitle size and position
*/
int DIA_srtPos(AVDMGenericVideoStream *in,uint32_t *size,uint32_t *position)
{
	return 1;
}

uint8_t    flySrtPos::upload(void)
{
        return 1;
}
uint8_t    flySrtPos::download(void)
{
        return 1;
}
