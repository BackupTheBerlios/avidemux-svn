/**/
/***************************************************************************
                          DIA_hue
                             -------------------

			   Ui for hue & sat

    begin                : 08 Apr 2005
    copyright            : (C) 2004/7 by mean
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
#include "ADM_image.h"
#include "ADM_videoFilter.h"
#include "ADM_vidHue.h"

#include "DIA_flyHue.h"
/**
      \fn DIA_getHue
      \brief Handle Hue (fly)Dialog
*/
uint8_t DIA_getHue(Hue_Param *param, AVDMGenericVideoStream *in)
{
        return 1;
}
/**************************************/
uint8_t flyHue::upload(void)
{
        return 1;
}
uint8_t flyHue::download (void)
{
        return 1;
}




