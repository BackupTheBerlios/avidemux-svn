/***************************************************************************
                          vidVCD.cpp  -  description
                             -------------------
    begin                : Wed Aug 21 2002
    copyright            : (C) 2002 by mean
    email                : fixounet@free.fr

	This is a meta filter that adds the needed filter to convert the stream to VCD resolution
   It computes the parmater and create
			- A resize filter
			- A Add border filter

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ADM_assert.h>

#include <ADM_assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "config.h"
#include "fourcc.h"
#include "avio.hxx"
#include "avi_vars.h"

#include "ADM_toolkit/toolkit.hxx"
#include "prototype.h"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_filter/video_filters.h"
#include "ADM_filter/vidVCD.h"
#include "prefs.h"
#include "ADM_dialog/DIA_resizeWiz.h"
//___________________________________________
extern FILTER_ENTRY allfilters[MAX_FILTER];
extern uint32_t nb_video_filter;
extern FILTER videofilters[MAX_FILTER];
extern uint32_t nb_active_filter;
extern ADM_Composer *video_body;

static uint8_t computeResize (void);


RESWIZ_FORMAT format;
RESWIZ_AR sourceAR, destinationAR;
static double aspectRatio[2][3]={
                                                                {1.,0.888888,1.19}, // NTSC 1:1 4:3 16:9
                                                                {1.,1.066667,1.43} // PAL  1:1 4:3 16:9
                                                        };

typedef struct targetFmt
{
        int x1,x2,y1,y2;
};


static targetFmt VCD={352, 352, 288, 240};
static targetFmt SVCD={480, 720, 576, 480};
static targetFmt DVD={720, 720, 576, 480};
static targetFmt DVDHD1={352, 720, 576, 480};
targetFmt *allFormats[4]={&VCD,&SVCD,&DVD,&DVDHD1};

#define ARME(x) format=RESWIZ_##x;


void setVCD (void)
{

  ARME (VCD);
  computeResize();
}
void setSVCD (void)
{

  ARME (SVCD);
  computeResize();
}
void setDVD (void)
{

  ARME (DVD);
  computeResize();
}
void setHalfD1 (void)
{

  ARME (DVD_HD1);
  computeResize();
}


extern AVDMGenericVideoStream *create_addBorder (AVDMGenericVideoStream * in,
						 uint32_t x, uint32_t x2,
						 uint32_t y, uint32_t y2);
extern AVDMGenericVideoStream *createResizeFromParam (AVDMGenericVideoStream *
						      in, uint32_t x,
						      uint32_t y);
//
//  Compute the filters, aspect is the reduction factor for h
//  =1 for VCD/DVD
//      =2/3 for SVCD
//------------------------------
/********************************************************************/
uint8_t
computeResize (void)
{

int targetx,targetxFinal,targetyPAL,targetyNTSC;

  ADV_Info *info;
  char *inputratio = NULL;


  uint8_t res = 0;		/* 1 Pal, 2 NSTC or film */
  uint32_t targety;

  // scale as if it was 1:1 -> 4:3
        prefs->get (FEATURE_SVCDRES_PREFEREDSOURCERATIO, &inputratio);
        if(inputratio)
        {
                if (inputratio && !strcmp (inputratio, "1:1")) 
                        sourceAR=RESWIZ_AR_1_1;
                if (inputratio && !strcmp (inputratio, "4:3")) 
                        sourceAR=RESWIZ_AR_4_3;
                if (inputratio && !strcmp (inputratio, "16:9")) 
                        sourceAR=RESWIZ_AR_16_9;
     
                ADM_dealloc (inputratio);
                inputratio = NULL;
        }
        
  
  if (!DIA_resizeWiz (&format, &sourceAR, &destinationAR))
    return 0;

 targetx=allFormats[format]->x1;
 targetxFinal=allFormats[format]->x2;
 targetyPAL=allFormats[format]->y1;
 targetyNTSC=allFormats[format]->y2;
        

  info = getLastVideoFilter (frameStart, frameEnd - frameStart)->getInfo ();


  res = identMovieType (info->fps1000);
  //printf("fps : %lu / %d \n",info->fps1000,res);
  switch (res)
    {
    case FRAME_PAL:
      res = 1;
      break;
    case FRAME_NTSC:
    case FRAME_FILM:
      res = 2;
      break;
    default:
      res = 0;
      return 0;
      break;
    }
  if (res == 2)
    targety = targetyNTSC;
  else
    targety = targetyPAL;


  double rx, ry;
  uint32_t newx, newy;
  uint32_t cropx, cropy;
  uint32_t original_x, original_y;


  original_x = info->width;
  original_y = info->height;

  
  // scale as if it was 1:1 -> 4:3
  rx = original_x;
  switch (res)
  {
      case 1:		//PAL
                rx = rx *aspectRatio[1][sourceAR]/aspectRatio[1][destinationAR];
                break;
       case 2:		// NTSC
                rx = rx * aspectRatio[0][sourceAR]/aspectRatio[0][destinationAR];
                break;
   }

  original_x = (uint32_t) floor (rx + 0.49);

  rx = original_x;
  rx = rx / targetxFinal;


  ry = original_y;
  ry = ry / targety;

  // which do do we compress less ?
  if (rx > ry)			// resize by X, add border afterward
    {
      newx = targetx;
      ry = original_y;
      ry = ry / rx;
      newy = (uint32_t) floor (ry + 0.49);
      printf (" resize by x\n");

    }
  else
    {
      newy = targety;
      rx = original_x;
      rx = rx / ry;
      rx *= targetx;
      rx /= targetxFinal;
      newx = (uint32_t) floor (rx + 0.49);
      printf (" resize by y\n");

    }

  printf ("\n New X x Y = %u x %u\n", newx, newy);

  // correct odd / even

  newx -= newx % 4;
  newy -= newy % 4;

  // Now correct crop


  cropx = targetx - newx;
  cropy = targety - newy;

  printf ("\n Resized to : %u x %u, add black border %u x %u",
	  newx, newy, cropx, cropy);

  // now build filter


// first resize (if needed)

  CONFcouple *couple;
  if (newx != info->width || newy != info->height)
    {
      videofilters[nb_active_filter].filter =
	createResizeFromParam (getLastVideoFilter (), newx, newy);


      videofilters[nb_active_filter].tag = VF_MPLAYERRESIZE;
      videofilters[nb_active_filter].filter->getCoupledConf (&couple);
      videofilters[nb_active_filter].conf = couple;;
      nb_active_filter++;
    }
// then add crop (if needed)

  if (cropx || cropy)
    {
      videofilters[nb_active_filter].filter =
	create_addBorder (videofilters[nb_active_filter - 1].filter,
			  cropx >> 1, cropx >> 1, cropy >> 1, cropy >> 1);

      videofilters[nb_active_filter].tag = VF_ADDBORDER;
      videofilters[nb_active_filter].filter->getCoupledConf (&couple);
      videofilters[nb_active_filter].conf = couple;;
      nb_active_filter++;
    }
  return 1;
}
