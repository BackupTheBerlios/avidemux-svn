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
***************************************************************************///
//  The pitch stride has to be changed as there could be some padding depending on the OS
//
//
#include "config.h"
#if defined(USE_SDL)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>



extern "C" {
#include "SDL/SDL.h"
#include "SDL/SDL_syswm.h"


}

#include "default.h"

#include "ADM_colorspace/colorspace.h"
#include "ADM_colorspace/ADM_rgb.h"
#include "ADM_commonUI//GUI_render.h"

#include "GUI_accelRender.h"
#include "GUI_sdlRender.h"
#include "ADM_assert.h"

//******************************************
static uint8_t sdl_running=0;
static SDL_Overlay *sdl_overlay=NULL;
static SDL_Surface *sdl_display=NULL;
static SDL_Rect disp;
#ifdef ADM_WIN32
HWND sdlWin32;
#endif
static ColBase *color=NULL;

sdlAccelRender::sdlAccelRender( void)
{
        useYV12=1;
        decoded=NULL;
}
uint8_t sdlAccelRender::end( void)
{
        if(sdl_overlay)
        {
                SDL_FreeYUVOverlay(sdl_overlay);
        }
        if(sdl_display)
        {
        SDL_UnlockSurface(sdl_display);
                SDL_FreeSurface(sdl_display);
        }
        if(sdl_running)
        {
                SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }
        if(decoded)
        {
        delete [] decoded;
        decoded=NULL;	
        }
        sdl_running=0;
        sdl_overlay=NULL;
        sdl_display=NULL;
        printf("[SDL] System closed and destroyed\n");
        
        
}
uint8_t sdlAccelRender::init( GUI_WindowInfo * window, uint32_t w, uint32_t h)
{
	printf("[SDL] Initialising video subsystem\n");

	int bpp;
	int flags;

    // Ask for the position of the drawing window at start
    disp.w=w;
    disp.h=h;
    disp.x=0;
    disp.y=0;

    if(!useYV12)
    {
		color=new ColBase(720,480);
		decoded=new uint8_t[w*h*2];
    }

    // Hack to get SDL to use GTK window, ugly but works
#if !defined(CONFIG_DARWIN) && !defined(ADM_WIN32)
	char SDL_windowhack[32];
    int winid=(int)window->window;

    sprintf(SDL_windowhack,"[SDL] SDL_WINDOWID=%ld",winid);
    putenv(SDL_windowhack);
#endif

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) 
    {
		printf("[SDL] FAILED initialising video subsystem\n");
		printf("[SDL] ERROR: %s\n", SDL_GetError());

        return 0;
    }

    sdl_running=1;
    flags = SDL_ANYFORMAT | SDL_HWPALETTE | SDL_HWSURFACE | SDL_NOFRAME;
    bpp= SDL_VideoModeOK( w, h,  16, flags );
    sdl_display= SDL_SetVideoMode( w, h,  bpp, flags );

    if (!sdl_display)
    {
        end();
        printf("[SDL] Cannot create surface\n");
		printf("[SDL] ERROR: %s\n", SDL_GetError());
        return 0;
    }

    SDL_LockSurface(sdl_display);

#ifdef ADM_WIN32
	struct SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);

	if (-1 != SDL_GetWMInfo(&wmInfo))
	{
		sdlWin32 = wmInfo.window;
		SetParent(sdlWin32,(HWND) window->display);
		MoveWindow(sdlWin32,0,0,w,h,0);
	}
	else
	{
		printf("[SDL] Reparenting failed\n");
	}
#endif

        int cspace;
        
        if(useYV12) cspace=SDL_YV12_OVERLAY;
            else    cspace=SDL_YUY2_OVERLAY;
        //_______________________________________________________
        sdl_overlay=SDL_CreateYUVOverlay((w),(h), cspace, sdl_display);

		// DirectX may fail but overlay still created.
		// Not a showstopper but log failure.
		if (strlen(SDL_GetError()))
		{
			printf("[SDL] ERROR: %s\n", SDL_GetError());
		}

        if(!sdl_overlay)
        {
			end();
			printf("[SDL] Cannot create SDL overlay\n");
			printf("[SDL] ERROR: %s\n", SDL_GetError());

			return 0;
        }
        
        printf("[SDL] Overlay created; type: %d, planes: %d, pitch: %d\n", sdl_overlay->hw_overlay, sdl_overlay->planes, sdl_overlay->pitches[0]);

        if(!sdl_overlay->hw_overlay)
        {
            printf("[SDL] Hardware acceleration disabled\n");
        #ifdef CONFIG_DARWIN
            printf("[SDL] ** Darwin**\n");
        #endif
        }

        if(!useYV12) color->reset(w,h);

		printf("[SDL] Video subsystem initalised successfully\n");

        return 1;
}

static void interleave(uint8_t *dst,uint8_t *src,int width, int stride, int lines)
{
    for(int y=0;y<lines;y++)
    {
        memcpy(dst,src,width);
        src+=width;
        dst+=stride;          
    }   
}
uint8_t sdlAccelRender::display(uint8_t *ptr, uint32_t w, uint32_t h,renderZoom zoom)
{
int pitch;
int page=w*h;
        ADM_assert(sdl_overlay);
        SDL_LockYUVOverlay(sdl_overlay);	
        pitch=sdl_overlay->pitches[0];
//	printf("SDL: new pitch :%d\n",pitch);
        if(useYV12)
        {
        if(pitch==w)
            memcpy(sdl_overlay->pixels[0],ptr,w*h);
        else
            interleave(sdl_overlay->pixels[0],ptr,w,pitch,h);
            
        pitch=sdl_overlay->pitches[1];
        if(pitch==(w>>1))
            memcpy(sdl_overlay->pixels[1],ptr+page,(w*h)>>2);
        else
            interleave(sdl_overlay->pixels[1],ptr+page,w>>1,pitch,h>>1);
      
        pitch=sdl_overlay->pitches[2];
        if(pitch==(w>>1))
            memcpy(sdl_overlay->pixels[2],ptr+(page*5)/4,(w*h)>>2);
        else
            interleave(sdl_overlay->pixels[2],ptr+(page*5)/4,w>>1,pitch,h>>1);  	
        }else
        {
        color->reset(w,h);
        if(pitch==2*w)
        {
            color->scale(ptr,sdl_overlay->pixels[0]);
        }
        else
        {
            color->scale(ptr,decoded);
            interleave(sdl_overlay->pixels[0],decoded,2*w,pitch,h);
        }
    }	
        
        disp.w=w;
        disp.h=h;
        disp.x=0;
        disp.y=0;
        
        SDL_UnlockYUVOverlay(sdl_overlay);
        SDL_DisplayYUVOverlay(sdl_overlay,&disp);
        
        return 1;
}

#endif
