/****************************************************************************
 copyright            : (C) 2001 by mean
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


#ifndef GUI_RENDER_H
#define GUI_RENDER_H


#include "ADM_image.h"

class AVDMGenericVideoStream;
typedef struct
{
    void *display;
    int  window;
	int x;
	int y;
	int width;
	int height;
} GUI_WindowInfo;

typedef enum 
{
    ADM_PREVIEW_NONE, 
    ADM_PREVIEW_OUTPUT,
    ADM_PREVIEW_SIDE,
    ADM_PREVIEW_TOP,
    ADM_PREVIEW_SEPARATE
}ADM_PREVIEW_MODE;

typedef enum 
{
        ZOOM_1_4,
        ZOOM_1_2,
        ZOOM_1_1,
        ZOOM_2,
        ZOOM_4,
        ZOOM_INVALID
}renderZoom;


ADM_PREVIEW_MODE getPreviewMode(void);
void             setPreviewMode(ADM_PREVIEW_MODE preview);
void             changePreviewZoom(renderZoom nzoom);

class admPreview
{
  public:
      static uint8_t update(uint32_t framenum);
      static void start(void);
      static void stop(void);
      static void setMainDimension(uint32_t, uint32_t );
      static void updateFilters(AVDMGenericVideoStream *first,AVDMGenericVideoStream *last);
      static void deferDisplay(uint32_t onoff,uint32_t startat);
      static void displayNow(uint32_t framenum);
      static void cleanUp(void);  
      static ADMImage *getBuffer(void);
};

uint8_t renderInit( void );
void renderDestroy(void);
uint8_t renderResize(uint32_t w, uint32_t h,uint32_t phyW,uint32_t phyH);
uint8_t renderRefresh(void);
uint8_t renderExpose(void);
uint8_t renderUpdateImage(uint8_t *ptr,renderZoom zoom);
uint8_t renderUpdateImageBlit(uint8_t *ptr,uint32_t startx, uint32_t starty, uint32_t w, uint32_t,uint32_t primary);

uint8_t renderStartPlaying( void );
uint8_t renderStopPlaying( void );
uint8_t renderHasAccelZoom(void);

uint8_t renderLock(void);
uint8_t renderUnlock(void);

void *UI_getDrawWidget(void);
void UI_rgbDraw(void *widg,uint32_t w, uint32_t h,uint8_t *ptr);
void UI_updateDrawWindowSize(void *win,uint32_t w,uint32_t h);
void UI_getWindowInfo(void *draw, GUI_WindowInfo *xinfo);

typedef enum 
{
        RENDER_GTK=0,
#ifdef USE_XV
        RENDER_XV=1,
#endif
#ifdef USE_SDL
        RENDER_SDL=2,

#ifdef ADM_WIN32
		RENDER_DIRECTX=3,
#endif
#endif
        RENDER_LAST       


}ADM_RENDER_TYPE;

#endif
