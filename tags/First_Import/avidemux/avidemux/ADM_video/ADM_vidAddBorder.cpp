/***************************************************************************
                          ADM_vidAddBorder.cpp  -  description
                             -------------------
    begin                : Sun Aug 11 2002
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

#include "config.h"
#include "fourcc.h"
#include "avio.hxx"
#include "config.h"
#include "avi_vars.h"
#ifdef HAVE_ENCODER


#include "ADM_toolkit/toolkit.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_video/ADM_vidAddBorder.h"
#include "ADM_video/ADM_vidCommonFilter.h"

BUILD_CREATE(addBorder_create,AVDMVideoAddBorder);

char *AVDMVideoAddBorder::printConf( void )
{
 	static char buf[50];

 	sprintf((char *)buf," Add Borders %lu x %lu --> %lu x %lu",
 				_in->getInfo()->width,
 				_in->getInfo()->height,
 				_info.width,
 				_info.height);
        return buf;
}

AVDMVideoAddBorder::AVDMVideoAddBorder(  AVDMGenericVideoStream *in,CONFcouple *couples)
{

 	_in=in;		
   	memcpy(&_info,_in->getInfo(),sizeof(_info));  		
	
		if(couples)
		{

			 _param=NEW(CROP_PARAMS);

				GET(cropx);
				GET(cropx2);
				GET(cropy);
				GET(cropy2);
				_info.width+=_param->cropx2+_param->cropx;
				_info.height+=_param->cropy2+_param->cropy;
		}	
			else 			
		{	// default parameter	
				_param=NEW(CROP_PARAMS);
				_param->cropx=_param->cropy=
				_param->cropx2=_param->cropy2=0;
		}				
					
 	//_uncompressed=(uint8_t *)malloc(3*_in->getInfo()->width*_in->getInfo()->height);
 	_uncompressed=new uint8_t [3*_in->getInfo()->width*_in->getInfo()->height];
  assert(_uncompressed);
  _info.encoding=1;

  	  	
}
AVDMGenericVideoStream *create_addBorder(AVDMGenericVideoStream *in,uint32_t x,uint32_t x2,uint32_t y,uint32_t y2)
{
	return new AVDMVideoAddBorder(in,x,x2,y,y2);
}
AVDMVideoAddBorder::AVDMVideoAddBorder(  AVDMGenericVideoStream *in,uint32_t x,uint32_t x2,uint32_t y,uint32_t y2)
{

 	_in=in;		
   	memcpy(&_info,_in->getInfo(),sizeof(_info));  		

				_param=NEW(CROP_PARAMS);
				_param->cropx=x;
				_param->cropy=y;
				_param->cropx2=x2;
				_param->cropy2=y2;
	_info.width+=_param->cropx2+_param->cropx;
	_info.height+=_param->cropy2+_param->cropy;
 	//_uncompressed=(uint8_t *)malloc(3*_in->getInfo()->width*_in->getInfo()->height);
 	_uncompressed=new uint8_t [3*_in->getInfo()->width*_in->getInfo()->height];
  assert(_uncompressed);
  _info.encoding=1;


}

uint8_t	AVDMVideoAddBorder::getCoupledConf( CONFcouple **couples)
{

			assert(_param);
			*couples=new CONFcouple(4);


	CSET(cropx);
	CSET(cropx2);
	CSET(cropy);
	CSET(cropy2);
			return 1;

}
AVDMVideoAddBorder::~AVDMVideoAddBorder()
{
 	delete []_uncompressed;
	DELETE(_param);
 	
}
uint8_t AVDMVideoAddBorder::getFrameNumberNoAlloc(uint32_t frame,
																	uint32_t *len,
   																	uint8_t *data,
   																	uint32_t *flags)
{

			assert(frame<_info.nb_frames);
			assert(_param);									
								
			// read uncompressed frame
       		if(!_in->getFrameNumberNoAlloc(frame, len,_uncompressed,flags)) return 0;
       		
				// blacken screen
				memset(data,0,_info.width*_info.height);
				memset(data+_info.width*_info.height,128,_info.width*_info.height>>1);


				// do luma
				uint8_t *src,*dest;
       		uint32_t y,x,line,lineout;
       		
       		y=_in->getInfo()->height;
       		x=_in->getInfo()->width;
       		line=x;
				lineout=_info.width;
       		src=_uncompressed;
       		dest=data+_param->cropx+_info.width*_param->cropy;
       		
       		for(uint32_t k=y;k>0;k--)
       			{
       			 	    memcpy(dest,src,line);
       			 	    src+=line;
       			 	    dest+=lineout;
       			}
       		 // Crop U  & V
					uint8_t *src_u,*src_v;
					uint8_t *dst_u,*dst_v;

       		 	src_u=_uncompressed+y*x;
       		 	src_v=src_u+(x*y>>2);
       		 	line>>=1;
       		 	lineout>>=1;       		       		 	
					dst_u= data+_info.width*_info.height+(_info.width*_param->cropy>>2)+(_param->cropx>>1);;
					dst_v= dst_u+(_info.width*_info.height>>2);

       		 		for(uint32_t k=y>>1;k>0;k--)
       		 		{
       		 	    	  	memcpy(dst_u,src_u,line);
       		 	    	  	memcpy(dst_v,src_v,line);

       			 	    	src_u+=line;
       			 	    	src_v+=line;

       			 	    	dst_u+=lineout;
       			 	    	dst_v+=lineout;

       		 		}
       		  *len= _info.width*_info.height+(_info.width*_info.height>>1);       			
      return 1;
}



#endif
