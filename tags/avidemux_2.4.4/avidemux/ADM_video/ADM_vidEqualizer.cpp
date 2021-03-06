
/***************************************************************************
                        Simple equalizer or linear luma/chroma filter
    

   (C) 2004/2005 Mean fixounet@free.fr
   GPL version 2

 ***************************************************************************/
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ADM_assert.h>

#include "config.h"
#include "fourcc.h"
#include "avio.hxx"
#include "avi_vars.h"
#include "math.h"

#include "ADM_toolkit/toolkit.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"

#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_FILTER
#include "ADM_osSupport/ADM_debug.h"



#include "ADM_vidEqualizer.h"
#include "ADM_filter/video_filters.h"

static FILTER_PARAM equalizer_template={8,{"0","1","2","3"
					,"4","5","6","7"}};
BUILD_CREATE(equalizer_create,vidEqualizer);
SCRIPT_CREATE(equalizer_script,vidEqualizer,equalizer_template);

extern uint8_t DIA_getEqualizer(EqualizerParam *param, AVDMGenericVideoStream *incoming);

uint8_t vidEqualizer::configure(AVDMGenericVideoStream *in)
{
ADMImage *video1;
uint32_t l,f;
uint8_t r;

	_in=in;		
	r= DIA_getEqualizer(_param,in);
	return r;
	
}

char *vidEqualizer::printConf( void )
{
 ADM_FILTER_DECLARE_CONF(" Equalizer");
        
}

vidEqualizer::vidEqualizer(AVDMGenericVideoStream *in,CONFcouple *couples) 
{
		_in=in;		
   		memcpy(&_info,_in->getInfo(),sizeof(_info));    
  		_info.encoding=1;
		_uncompressed=NULL;
		
  		_info.encoding=1;
		_uncompressed=new ADMImage(_info.width,_info.height);
		
		
		_param=NEW(EqualizerParam);
		if(couples)
		{
		        char dummy[10];
                        for(int i=0;i<256;i++)  
                        {
                                sprintf(dummy,"x%03d",i);
                                couples->getCouple((char *)dummy,&(_param->_scaler[i]));
                        }
		}
		else // Default
  		{
                                for(int i=0;i<256;i++)
                                        _param->_scaler[i]=i;
		}
}
//____________________________________________________________________
vidEqualizer::~vidEqualizer()
{
		
	delete _uncompressed;
	delete _param;
	_param=NULL;
	_uncompressed=NULL;
		
		
}

//______________________________________________________________
uint8_t vidEqualizer::getFrameNumberNoAlloc(uint32_t frame,
				uint32_t *len,
   				ADMImage *data,
				uint32_t *flags)
{

        if(frame>= _info.nb_frames) return 0;
	if(!_in->getFrameNumberNoAlloc(frame,len,_uncompressed,flags)) return 0;
	
	uint8_t *src,*dst;
	src=_uncompressed->data;
	dst=data->data;
	for(uint32_t y=_info.height;y>0;y--)
	for(uint32_t x=_info.width;x>0;x--)
		*(dst++)=_param->_scaler[*(src++)];

	uint32_t square=_info.width*_info.height;
	square>>=2;
	// copy u & v too
	memcpy(data->data+4*square,_uncompressed->data+4*square,2*square);
	return 1;
}


uint8_t	vidEqualizer::getCoupledConf( CONFcouple **couples)
{
char dummy[10];
			ADM_assert(_param);
			*couples=new CONFcouple(256);

        for(int i=0;i<256;i++)  
        {
                sprintf(dummy,"x%03d",i);
                (*couples)->setCouple(dummy,(_param->_scaler[i]));
        }
	return 1;
}

// EOF
