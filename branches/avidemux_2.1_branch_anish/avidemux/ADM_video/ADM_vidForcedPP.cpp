//
// C++ Implementation: ADM_vidForcedPP
//
// Description: 
//
//	Force postprocessing assuming constant quant & image type
//	Uselefull on some badly authored DVD for example
//
// Author: mean <fixounet@free.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ADM_assert.h>
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



#include "ADM_vidForcedPP.h"
#include "ADM_dialog/DIA_enter.h"
#include "ADM_filter/video_filters.h"


static FILTER_PARAM ppParam={3,{"postProcType","postProcStrength","forcedQuant"}};


SCRIPT_CREATE(forcedpp_script,ADMVideoForcedPP,ppParam);
BUILD_CREATE(forcedpp_create,ADMVideoForcedPP);

extern int DIA_getMPParams( uint32_t *pplevel, uint32_t *ppstrength,uint32_t *swap);

uint8_t ADMVideoForcedPP::configure( AVDMGenericVideoStream *instream)
{
	_in=instream;
	uint32_t postproc, strength,uv;
	int forced;
		postproc=_param->postProcType;
		strength=_param->postProcStrength;
		uv=0;
		if( DIA_getMPParams( &postproc,&strength,&uv))
		{
				_param->postProcType=postproc;
				_param->postProcStrength=strength;
                                
				forced=_param->forcedQuant;
				if( DIA_GetIntegerValue(&forced,2,31, "Quant Value","Enter forced Q:"))
				{
					_param->forcedQuant=forced;
				}

				updatePostProc(&_postproc );				

		}
	return 1;	
 	
}
uint8_t	ADMVideoForcedPP::getCoupledConf( CONFcouple **couples)
{

			*couples=new CONFcouple(3);

			CSET(postProcType);
			CSET(postProcStrength);
			CSET(forcedQuant);

		return 1;	
}
char *ADMVideoForcedPP::printConf( void )
{
 	static char buf[50];
 	
 	sprintf((char *)buf," Forced pp : %lu %lu",_param->postProcType,_param->postProcStrength);
        return buf;
}

ADMVideoForcedPP::~ADMVideoForcedPP()
{
	if(_uncompressed)
 		delete _uncompressed;
	if(_postproc.ppContext)
		deletePostProc(&_postproc);

 	_uncompressed=NULL;
}


 ADMVideoForcedPP::ADMVideoForcedPP( AVDMGenericVideoStream *in,CONFcouple *couples)
{
		printf("Adding forced PP filter\n");
		_uncompressed=NULL;
		_in=in;
		ADM_assert(in);
		if(!couples)
		{
			
			_param=NEW(PP_CONF);
	    		_param->postProcType=5; // Bff=0 / 1=tff
	    		_param->postProcStrength=5;
			_param->forcedQuant=10;
			
	    	}
		else
		{
			_param=NEW(PP_CONF);
			GET(postProcType);
			GET(postProcStrength);
			GET(forcedQuant);
			
						
		}
		ADM_assert(in);
		
		memcpy(&_info,_in->getInfo(),sizeof(_info));	
		
	    	initPostProc(&_postproc,_info.width,_info.height);
		_postproc.postProcType=_param->postProcType;
		_postproc.postProcStrength=_param->postProcStrength;
		_postproc.forcedQuant=_param->forcedQuant;
		updatePostProc(&_postproc);	    	
	    	
	    	//_uncompressed=new uint8_t[3*_info.width*_info.height];	
		_uncompressed=new ADMImage(_info.width,_info.height);	
		

}
uint8_t ADMVideoForcedPP::getFrameNumberNoAlloc(uint32_t frame,
							uint32_t *len,
							ADMImage *data,
							uint32_t *flags)
{
		uint32_t page=_info.width*_info.height;
		
		*len=(page*3)>>1;
		if(frame>=_info.nb_frames) return 0;		
		
		if(!(_postproc.postProcType && _postproc.postProcStrength) )
		{
			// disabled
			if(!_in->getFrameNumberNoAlloc(frame, len,data,flags)) return 0;
				
			return 1;
		
		}
			// we do postproc !
			// keep
			uint8_t *iBuff[3],*oBuff[3];
			int strideTab[3],strideTab2[3];			
				
				
			if(!_in->getFrameNumberNoAlloc(frame, len,_uncompressed,flags))
			 		return 0;

			oBuff[0]=YPLANE(data);
                        oBuff[1]=VPLANE(data);
                        oBuff[2]=UPLANE(data);

				
			iBuff[0]=YPLANE(_uncompressed);
		 	iBuff[1]=VPLANE(_uncompressed);
 		 	iBuff[2]=UPLANE(_uncompressed);
				
				
		        strideTab[0]=strideTab2[0]=_info.width;
			strideTab[1]=strideTab2[1]=_info.width>>1;
			strideTab[2]=strideTab2[2]=_info.width>>1;
	
			int type;
			if(_uncompressed->flags&AVI_KEY_FRAME)
					type=1;
			else if(_uncompressed->flags & AVI_B_FRAME)
					type=3;
				else
					type=2;
		 	pp_postprocess(
		      		iBuff,
		        	strideTab,
		        	oBuff,
		        	strideTab2,
				_info.width,
		        	_info.height,
		        	NULL,
		          	0,
		         	_postproc.ppMode,
		          	_postproc.ppContext,
		          	type); // I ?
			  	
			
		
			data->copyInfo(_uncompressed);	
			//printf("Type:%d\n",type);
	return 1;
}

