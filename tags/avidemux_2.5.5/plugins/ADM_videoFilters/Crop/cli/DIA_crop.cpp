/***************************************************************************
                          DIA_crop.cpp  -  description
                             -------------------

			    GUI for cropping including autocrop
			    +Revisted the Gtk2 way
			     +Autocrop now in RGB space (more accurate)

    begin                : Fri May 3 2002
    copyright            : (C) 2002/2007 by mean
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


#include "ADM_image.h"
#include "ADM_videoFilter.h"

#include "DIA_flyDialog.h"
#include "DIA_flyCrop.h"
#include "ADM_vidCrop_param.h"

int DIA_getCropParams(const char *name, CROP_PARAMS *param, AVDMGenericVideoStream *in)
{
	return 0;
}

//____________________________________
uint8_t flyCrop::upload(void)
{
        return 1;
}
uint8_t flyCrop::download(void)
{
	return 1;
}
	
