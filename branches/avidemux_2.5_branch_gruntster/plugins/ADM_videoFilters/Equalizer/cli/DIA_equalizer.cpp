
//
/***************************************************************************
                          DIA_Equalizer
                             -------------------

			   Ui for equalizer, ugly

    begin                : 30 Dec 2004
    copyright            : (C) 2004/5 by mean
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
#include <math.h>


#include "ADM_image.h"
#include "ADM_videoFilter.h"
#include "ADM_vidEqualizer.h"
#include "ADM_colorspace.h"
//
//	Video is in YV12 Colorspace
//
//
uint8_t DIA_getEqualizer(EqualizerParam *param, AVDMGenericVideoStream *in)
{
	return 1;

}

