/***************************************************************************
                          DIA_flyEraser.cpp  -  configuration dialog for
						   Eraser filter
                              -------------------
                         Chris MacGregor, December 2007
                         chris-avidemux@bouncingdog.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "default.h"

#include "ADM_colorspace/ADM_rgb.h"

#include "ADM_image.h"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_videoFilter/ADM_vidEraser.h"
#include "DIA_flyDialog.h"
#include "DIA_flyEraser.h"

/************* COMMON PART *********************/

uint8_t flyEraser::sliderChanged ()
{
    return ADM_flyDialog::sliderChanged();
}

uint8_t  flyEraser::update ()
{
    download();
    process();
    copyYuvFinalToRgb();
    display();
    return 1;
}

uint8_t flyEraser::process ()
{
    uint8_t ret = ADMVideoEraser::doEraser (_yuvBuffer, _yuvBufferOut,
                                            _in, sliderGet(), eraserp,
                                            &param, _w, _h);
    return ret;
}

/************* COMMON PART *********************/
