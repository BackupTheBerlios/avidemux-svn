/***************************************************************************
                          ADM_vidDropOut.cpp  -  description
                             -------------------
    begin                : Mon Oct 7 2002
    copyright            : (C) 2002 by Ron Reithoffer 
    email                : <ron.reithoffer@chello.at>
 ***************************************************************************/
#if 0
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
#include <math.h>

#include "config.h"
#include "fourcc.h"
#include "avio.hxx"
#include "config.h"
#include "avi_vars.h"
#ifdef HAVE_ENCODER


#include "ADM_toolkit/toolkit.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_video/ADM_vidDropOut.h"
#include "ADM_filter/video_filters.h"


static FILTER_PARAM dropParam={1,{"threshold"}};


SCRIPT_CREATE(dropout_script,ADMVideoDropOut,dropParam);


//extern uint8_t distMatrix[256][256];
extern uint32_t fixMul[16];

BUILD_CREATE(dropout_create,ADMVideoDropOut);
//_______________________________________________________________
 

 char 								*ADMVideoDropOut::printConf(void)
 {
	  	static char buf[50];

 				sprintf((char *)buf," DropOut :%ld",*_param);
        return buf;
	}
uint8_t  GUI_getIntegerValue(int *valye, int min, int max, char *title);	
uint8_t ADMVideoDropOut::configure(AVDMGenericVideoStream *instream)
{
int i;

			_in=instream;
			i=(int)*_param;
			if(GUI_getIntegerValue(&i,1,255,"DropOut Threshold"))
			{
					*_param=(uint32_t)i;
					return 1;
			}
			return 0;
		    
	
}
//--------------------------------------------------------	
ADMVideoDropOut::ADMVideoDropOut(
									AVDMGenericVideoStream *in,CONFcouple *couples)
                  :ADMVideoCached(in,couples)
{
  //uint32_t frame;
  _uncompressed=NULL;

  _info.encoding=1;
  if(couples)
  {
  			_param=NEW( uint32_t);
			couples->getCouple((char *)"threshold",(uint32_t *)_param);
	}
	else
	{
			_param=NEW( uint32_t);
			*_param=30;
	}
 
}

uint8_t	ADMVideoDropOut::getCoupledConf( CONFcouple **couples)
{

			ADM_assert(_param);
			*couples=new CONFcouple(1);
			(*couples)->setCouple((char *)"threshold",(*_param));
			return 1;

}

//                     1
//		Get in range in 121 + coeff matrix
//                     1
//
// If the value is too far away we ignore it
// else we blend

uint8_t ADMVideoDropOut::getFrameNumberNoAlloc(uint32_t frame,
				uint32_t *len,
   				ADMImage *data,
				uint32_t *flags)
{
UNUSED_ARG(flags);

uint32_t uvlen;
uint32_t dlen,dflags;

uint8_t										*_next;
uint8_t										*_previous;
uint8_t										*_current;
	//		printf("\n DropOut : %lu\n",frame);




			uvlen=    _info.width*_info.height;
			*len=uvlen+(uvlen>>1);
			  if(frame> _info.nb_frames-1) return 0;
			if(!frame || (frame==_info.nb_frames-1))
			{
				 if(!_in->getFrameNumberNoAlloc(frame, &dlen,_buffer[0],&dflags))
				 {
					 	return 0;
					}
				 _bufnum[0]=frame;
					memcpy(data,_buffer[0],*len);
					return 1;
			}
				ADM_assert(frame<_info.nb_frames);
			// read uncompressed frame
			switch(fillCache(frame))
			{
				  case 0: return 0;
				  case 1: memcpy(data,_buffer[index_c],*len);return 1;break;
				  case 2: break;
				  default: ADM_assert(0); break;
			}
   		_previous=_buffer[index_p];
   		_current=_buffer[index_c];
   		_next=_buffer[index_n];
           // for u & v , no action -> copy it as is
           memcpy(data+uvlen,_current+uvlen,uvlen>>1);

             uint8_t *inprev,*innext,*incur,*zout;

              inprev=_previous+1+_info.width;
              innext=_next+1+_info.width;
              incur =_current+1+_info.width;

              zout=data+_info.width+1;

              int32_t c0,c1,c2,c3; //,_nextPix,_currPix,_prevPix,cc;

              for(uint32_t y= _info.height-2;y>2;y--)
              	{
		  c0=0;
		  c1=0;
		  c2=0;
		  c3=0;

	  	inprev=_previous	+1+y*_info.width;
              	innext=_next		+1+y*_info.width;;
              	incur =_current	+1+y*_info.width;;

			// look if the field is more different temporarily than spacially

			    for(uint32_t x= _info.width-1;x>1;x--)
        		      	{

						c0+=(abs(((*inprev))-( *incur    ))^2);
						c1+=(abs(((*inprev))-( *innext   ))^2)<<1;
						c0+=(abs(((*incur ))-( *innext   ))^2);


						c2+=(abs(((    *(incur-_info.width*2) ))-( *(incur            )   ))^2)   ;
						c3+=(abs(((    *(incur-_info.width*2) ))-( *(incur+_info.width*2)   ))^2)<<1;
						c2+=(abs(((    *(incur            ) ))-( *(incur+_info.width*2)   ))^2)   ;


						incur++;
						innext++;
						inprev++;
				}

		// If yes, replace the line by an average of next/previous image
		inprev=_previous	+y*_info.width;
              	innext=_next		+y*_info.width;;
              	incur =_current	+y*_info.width;;
		zout=data		+y*_info.width;

		if (c1<c0 &&c3<c2)
		{
		    for(uint32_t x= _info.width;x>0;x--)
       			      	{
					*zout= ((*(inprev))+(*(innext)))>>1 ;
					zout++;
					innext++;
					inprev++;
				}
		}
		else
			memcpy(zout,incur,_info.width);
	}
return 1;
}
ADMVideoDropOut::~ADMVideoDropOut()
{
	DELETE(_param);
}


#endif
#endif