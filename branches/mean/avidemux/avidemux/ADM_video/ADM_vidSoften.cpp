//
// C++ Implementation: Soften
//
// Description: 
//
// See .h file
//
// Author: mean <fixounet@free.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

//
//	This is the very unoptimized version
//	could use some stuff and some mmx too
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <assert.h>
#include <math.h>
#include <math.h>
#include <iconv.h>
#include "config.h"


#include "fourcc.h"
#include "avio.hxx"

#include "avi_vars.h"

#include "ADM_toolkit/toolkit.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"

#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_FILTER
#include "ADM_toolkit/ADM_debug.h"



#include "ADM_vidSoften.h"
#include "ADM_dialog/DIA_enter.h"

BUILD_CREATE(soften_create,ADMVideoMaskedSoften);

extern uint8_t distMatrix[256][256];
extern uint32_t fixMul[16];


uint8_t ADMVideoMaskedSoften::configure( AVDMGenericVideoStream *instream)
{
	_in=instream;
	int radius;
	
		radius=_param->radius;
		if(DIA_GetIntegerValue(&radius,1,60, "Radius","Radius:"))
			_param->radius=radius;
			
		radius=_param->luma;
		if(DIA_GetIntegerValue(&radius,0,255, "Luma Threshold","Luma Threshold:"))
			_param->luma=radius;
		
		radius=_param->chroma;
		if(DIA_GetIntegerValue(&radius,0,255, "Chroma Threshold","Chroma Threshold:"))
			_param->chroma=radius;
					
		
	return 1;	
 	
}
uint8_t	ADMVideoMaskedSoften::getCoupledConf( CONFcouple **couples)
{

			*couples=new CONFcouple(3);

			CSET(radius);
			CSET(luma);
			CSET(chroma);

		return 1;	
}
char *ADMVideoMaskedSoften::printConf( void )
{
 	static char buf[50];
 	
 	sprintf((char *)buf," Soften : radius: %lu l:%lu c:%lu", 
		_param->radius,_param->luma, _param->chroma);
        return buf;
}

ADMVideoMaskedSoften::~ADMVideoMaskedSoften()
{
	if(_uncompressed)
 		delete []_uncompressed;	
 	_uncompressed=NULL;
}


 ADMVideoMaskedSoften::ADMVideoMaskedSoften( AVDMGenericVideoStream *in,CONFcouple *couples)
{
		
		_uncompressed=NULL;
		_in=in;
		assert(in);
		if(!couples)
		{
			
			_param=NEW(MaskedSoften_CONF);
	    		_param->radius=2; 
	    		_param->luma=5;
			_param->chroma=5;
			
	    	}
		else
		{
			_param=NEW(MaskedSoften_CONF);
			GET(radius);
			GET(luma);
			GET(chroma);
			
						
		}
		assert(in);
		
		memcpy(&_info,_in->getInfo(),sizeof(_info));	
			    	
	    	_uncompressed=new uint8_t[3*_info.width*_info.height];	
		

}
uint8_t ADMVideoMaskedSoften::getFrameNumberNoAlloc(uint32_t frame,
							uint32_t *len,
							uint8_t *data,
							uint32_t *flags)
{
		uint32_t page=_info.width*_info.height;
		int32_t radius=_param->radius;
		uint32_t offset;
		
		int blockx,blocky;
		
		*len=(page*3)>>1;
		if(frame>=_info.nb_frames) return 0;		
		
				
		if(!_in->getFrameNumberNoAlloc(frame, len,_uncompressed,flags))
				 		return 0;

		// do luma only ATM
		// copy chroma
		memcpy(data+page,_uncompressed+page,page>>1);
		
		// for luma, the radius first lines /last lines are unchanged
		memcpy(data,_uncompressed,radius*_info.width);
		
		offset=page-_info.width*radius-1;
		
		memcpy(data+offset,
			_uncompressed+offset,
			radius*_info.width);

		uint8_t *src,*dst;
		uint32_t val,cur,coef;
		
		// optimized one
		if(radius==2) return radius5(_uncompressed,data);
		if(radius==1) return radius3(_uncompressed,data);
		
		for(uint32_t y=radius;y<_info.height-radius;y++)
		{
			src=_uncompressed+y*_info.width;
			dst=data+y*_info.width;
			
			memcpy(dst,src,radius);
			src+=radius;
			dst+=radius;
			
			for(uint32_t x=radius;x<_info.width-radius;x++)
			{
				coef=0;
				val=0;
			
				for( blocky=-radius;blocky<=radius;blocky++)
				{
					for( blockx=-radius;blockx<=radius;blockx++)
					{
						cur=*(src+blockx+blocky*_info.width);
						
						if( distMatrix[cur][*src]<=_param->luma)
						{
							coef++;
							val+=cur;
						}
					
					
					}
				}
				assert(coef);
				if(coef!=1)
					val=(val+(coef>>1)-1)/coef;
				*dst++=val;
				src++;
				
				//*dst++=*src++;
			}
			memcpy(dst,src,radius);
		
		}	
	return 1;
}
uint8_t ADMVideoMaskedSoften::radius5(uint8_t *_uncompressed, uint8_t *data) 
{
int blockx,blocky;
uint32_t val,coef;

uint8_t *src,*dst;
uint8_t cur;

uint8_t *c0,*c1,*c2,*c3,*c4,ref;



	for(uint32_t y=2;y<_info.height-2;y++)
		{
			src=_uncompressed+y*_info.width;
			dst=data+y*_info.width;
			
			*dst++=*src++;
			*dst++=*src++;
			
			for(uint32_t x=2;x<_info.width-2;x++)
			{
				coef=0;
				val=0;
				c0=src-2-2*_info.width;
				c1=c0+_info.width;
				c2=c1+_info.width;
				c3=c2+_info.width;
				c4=c3+_info.width;
				
				ref=*src;
					
#define CHECK(x) cur=*x;if(distMatrix[cur][ref]<=_param->luma) {coef++;val+=cur;}x++;					
				for( blockx=5;blockx>0;blockx--)
				{
					CHECK(c0);
					CHECK(c1);
					CHECK(c2);
					CHECK(c3);
					CHECK(c4);
				}
				assert(coef);
				if(coef!=1)
					val=(val+(coef>>1)-1)/coef;

				*dst++=val;
				src++;
			}
			*dst++=*src++;
			*dst++=*src++;		
		}	
}
uint8_t ADMVideoMaskedSoften::radius3(uint8_t *_uncompressed, uint8_t *data) 
{
int blockx,blocky;
uint32_t val,coef;

uint8_t *src,*dst;
uint8_t cur;

uint8_t *c0,*c1,*c2,ref;

uint8_t *dist;

	for(uint32_t y=1;y<_info.height-1;y++)
		{
			src=_uncompressed+y*_info.width;
			dst=data+y*_info.width;
			
			*dst++=*src++;
			
			
			for(uint32_t x=1;x<_info.width-1;x++)
			{
				coef=0;
				val=0;
				c0=src-1-_info.width;
				c1=c0+_info.width;
				c2=c1+_info.width;
				
				ref=*src;
				dist=distMatrix[ref];
#define CHECK(x) cur=*x;if(dist[cur]<=_param->luma) {coef++;val+=cur;}x++;					
					CHECK(c0);
					CHECK(c0);
					CHECK(c0);
					
					CHECK(c1);
					CHECK(c1);					
					CHECK(c1);
					
					CHECK(c2);
					CHECK(c2);					
					CHECK(c2);
				
					
				assert(coef);
				if(coef!=1)
					val=(val+(coef>>1)-1)/coef;
				*dst++=val;
				src++;
			}
			*dst++=*src++;
			
		}	
}






