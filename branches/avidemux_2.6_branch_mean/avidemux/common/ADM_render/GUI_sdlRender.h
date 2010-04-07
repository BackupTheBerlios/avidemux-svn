
/***************************************************************************
    copyright            : (C) 2006 by mean
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
#include "GUI_render.h"
#include "GUI_accelRender.h"

#ifndef TSDLRENDER_H
#define TSDLRENDER_H
/**
    \class sdlRender
*/
class sdlRender: public VideoRenderBase
{
  protected:
              bool     useYV12;
              uint8_t *decoded;
  public:
                             sdlRender( void ) ;
              virtual	bool init( GUI_WindowInfo *  window, uint32_t w, uint32_t h,renderZoom zoom);
              virtual	bool stop(void);				
              virtual   bool displayImage(ADMImage *pic);
};

void initSdl(int videoDevice);
void quitSdl(void);
#endif
