/***************************************************************************
                          ADM_vidStabilize.cpp  -  description
                             -------------------
    begin                : Mon Oct 7 2002
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <assert.h>
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
#include "ADM_video/ADM_vidStabilize.h"

extern uint8_t distMatrix[256][256];
extern uint32_t fixMul[16];

BUILD_CREATE(stabilize_create,ADMVideoStabilize);



 char 								*ADMVideoStabilize::printConf(void)
 {
	  	static char buf[50];

 				sprintf((char *)buf," Stabilize :%ld",*_param);
        return buf;
	}
uint8_t  GUI_getIntegerValue(int *valye, int min, int max, char *title);
uint8_t ADMVideoStabilize::configure(AVDMGenericVideoStream *instream)
{
int i;
			_in=instream;
			i=(int)*_param;
			if(GUI_getIntegerValue(&i,1,255,"Stabilize Threshold"))
			{
					*_param=(uint32_t)i;
					return 1;
			}
			return 0;
		    
	
}
//--------------------------------------------------------	
ADMVideoStabilize::ADMVideoStabilize(AVDMGenericVideoStream *in,CONFcouple *couples)
                  :ADMVideoCached(in,couples)
{
  //uint32_t frame;
  _uncompressed=NULL;
   
 
  _info.encoding=1;
  if(couples)
  {
			_param=NEW(uint32_t);
			assert(couples->getCouple((char *)"param",_param));
	}
	else
	{
			_param=NEW( uint32_t);
			*_param=30;
	}
 
}


uint8_t	ADMVideoStabilize::getCoupledConf( CONFcouple **couples)
{

			assert(_param);
			*couples=new CONFcouple(1);
 			(*couples)->setCouple((char *)"param",*_param);

			return 1;

}
//                     1
//		Get in range in 121 + coeff matrix
//                     1
//
// If the value is too far away we ignore it
// else we blend

uint8_t ADMVideoStabilize::getFrameNumberNoAlloc(uint32_t frame,
																	uint32_t *len,
   																	uint8_t *data,
   																	uint32_t *flags)
{
UNUSED_ARG(flags);
uint32_t uvlen;
uint32_t dlen,dflags;

uint8_t										*_next;
uint8_t										*_previous;  	
uint8_t										*_current;
	//		printf("\n Stabilize : %lu\n",frame);

		
					
			
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
				assert(frame<_info.nb_frames);   
			// read uncompressed frame
			switch(fillCache(frame))
			{
					case 0: return 0;
				  case 1: memcpy(data,_buffer[index_c],*len);return 1;break;
				  case 2: break;
				  default: assert(0); break;				
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
              
             
              uint8_t *nl,*pl,*nc,*pc;
              
              uint16_t c,coeff;
              uint32_t x;
              for(uint32_t y= _info.height-1;y>1;y--)
              	{
			nl=incur+_info.width;
			pl=incur-_info.width;
			nc=incur+1;
			pc=incur-1;		   
								   
		        for(x= _info.width-1;x>1;x--)
        		{
	               		c=*incur*4;;
	                    	coeff=4;
#define PONDERATE(x,p) if(distMatrix[*incur][x]<*_param)	                      \
{ c+=x;coeff++;}
																PONDERATE(*innext,1);
				PONDERATE(*inprev,1);
				PONDERATE(*(pc),1);
				PONDERATE(*(nc),1);
				PONDERATE(*(nl),1);
				PONDERATE(*(pl),1);
													//*zout=(uint8_t)floor(0.49+(c/coeff));
																assert(coeff);
				assert(coeff<16);
				*zout=(c*fixMul[coeff])>>16;
				zout++;
				incur++;
				innext++;
				inprev++;	
				nl++;pl++;nc++;pc++;	  
			}
			zout+=2;
			incur+=2;
			innext+=2;
			inprev+=2;
		}
		return 1;	
}
ADMVideoStabilize::~ADMVideoStabilize()
{
 
 	DELETE(_param);
}


#endif
