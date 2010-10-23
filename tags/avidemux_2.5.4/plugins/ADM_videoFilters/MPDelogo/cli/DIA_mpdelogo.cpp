/***************************************************************************
                              DIA_mpdelogo.cpp
                              ----------------

                        GUI for MPlayer Delogo filter

    begin                : Fri May 3 2002
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

#include <math.h>
#include "ADM_default.h"
#include "ADM_image.h"
#include "ADM_videoFilter.h"
#include "ADM_colorspace.h"
#include "ADM_vidMPdelogo.h"
#include "DIA_flyDialog.h"
#include "DIA_flyMpDelogo.h"

uint8_t DIA_getMPdelogo(MPDELOGO_PARAM *param, AVDMGenericVideoStream *in)
{
	return 1;
}

uint8_t flyMpDelogo::download(void)
{
	return 1;
}

uint8_t flyMpDelogo::upload(void)
{
	return 1;
}

