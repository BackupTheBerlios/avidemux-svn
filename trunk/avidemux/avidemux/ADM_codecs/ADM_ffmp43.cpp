/***************************************************************************
                          ADM_ffmp43.cpp  -  description
                             -------------------
                             
	Decoder for Divx3/4/..., using ffmpeg
                             
    begin                : Wed Sep 25 2002
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
#include <ADM_assert.h>
#include <string.h>
#include <math.h>
#include "config.h"
#ifdef USE_FFMPEG

#include "ADM_lavcodec.h"
#include "ADM_library/default.h"

#include "ADM_colorspace/colorspace.h"

#include "ADM_codecs/ADM_codec.h"

#include "ADM_codecs/ADM_ffmp43.h"
//#include "ADM_gui/GUI_MPP.h"

#include "ADM_toolkit/toolkit.hxx"

#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME  MODULE_CODEC
#include "ADM_toolkit/ADM_debug.h"


extern "C"
{
int av_is_voppacked(AVCodecContext *avctx);
};
uint8_t  decoderFF::isDivxPacked( void )
{
	return av_is_voppacked(_context);
}


//________________________________________________
void decoderFF::setParam( void )
{
		if(GUI_Question("Invert u & v ?"))
			_swapUV=1;
		else
			_swapUV=0;

        return; // no param for ffmpeg
}

//-------------------------------
decoderFF::decoderFF(uint32_t w,uint32_t h) :decoders(w,h)
{

//				memset(&_context,0,sizeof(_context));
				_allowNull=0;
				_context=NULL;
#if LIBAVCODEC_BUILD >= 4624
   	_context=avcodec_alloc_context();
#else
		_context=new AVCodecContext;
    memset(_context, 0, sizeof(AVCodecContext));
#endif
				ADM_assert(_context);
				memset(&_frame,0,sizeof(_frame));

				_context->max_b_frames=0;

		  		_context->width=_w;
				_context->height=_h;
			  	_context->pix_fmt=PIX_FMT_YUV420P;//PIX_FMT_RGBA32
				//_context->debug=1;

			    _internalBuffer=new uint8_t[w*h*3];
			   
			    _swapUV=0;
				//_context->strict_std_compliance=-1;
			    printf("FFMpeg build : %d\n", LIBAVCODEC_BUILD);
}

//_____________________________________________________

decoderFF::~decoderFF()
{
		avcodec_close(_context);
		free(_context);
		delete [] _internalBuffer;	
		printf("FF base destroyed\n");
}

//-------------------------------
uint32_t  decoderFF::frameType( void )
{
	uint32_t flag=0;

	#define SET(x) flag=x;aprintf(" Frame is %s\n",#x);

	switch(_frame.pict_type)
	{
		case FF_B_TYPE:
				SET(AVI_B_FRAME);
				if(_frame.key_frame)
					aprintf("\n But keyframe is  set\n");
				break;
		case FF_S_TYPE:
		case FF_I_TYPE:
				SET(AVI_KEY_FRAME);
				if(!_frame.key_frame)
					aprintf("\n But keyframe is not set\n");
				break;
		case FF_P_TYPE:
				SET(AVI_P_FRAME);
				if(_frame.key_frame)
					aprintf("\n But keyframe is  set\n");
				break;
		default:
//				printf("\n OOops XXX frame ?\n");
				break;
		}
	return flag;
}
uint8_t decoderFF::decodeHeaderOnly(void )
{
	_context->hurry_up=5;
	printf("\n FFmpeg: Hurry up\n");
	return 1;
}
uint8_t decoderFF::decodeFull(void )
{
	_context->hurry_up=0;
	printf("\n FFmpeg: full decoding\n");
	return 1;
}

uint8_t     decoderFF::uncompress(uint8_t *in,ADMImage *out,uint32_t len,uint32_t *flagz)

{
 int got_picture=0;
 uint8_t *oBuff[3];
 int strideTab[3];
 int strideTab2[3];
 int ret=0;

		if(len==0 && !_allowNull) // Null frame, silently skipped
				{
            				if(flagz) *flagz=AVI_KEY_FRAME;
              				printf("\n ff4: null frame\n");
					return 1;
				}

		ret= avcodec_decode_video(_context,&_frame,&got_picture, in, len);
		
		if(0>ret && !_context->hurry_up)
		{
					printf("\n error in FFMP43/mpeg4!\n");
					return 0;
		}
		if(!got_picture && !_context->hurry_up)
		{
				// Some encoder code a vop header with the 
				// vop flag set to 0
				// it is meant to mean frame skipped but very dubious
				if(len<8)
					{
						printf("Probably pseudo black frame...\n");
						return 1;
					}
				// allow null means we allow null frame in and so potentially
				// have no frame out for a time
				// in that case silently fill with black and returns it as KF
				if(_allowNull)
				{
					if(flagz) *flagz=AVI_KEY_FRAME;
					memset(out->data,0,_w*_h);
					memset(out->data+_w*_h,128,(_w*_h)>>1);
					printf("\n ignoring got pict ==0\n");
					return 1;

				}
					printf("\n error in FFMP43/mpeg4!: got picture \n");
					//GUI_Alert("Please retry with misc->Turbo off");
					//return 1;
					return 0;			 			
		}		  
		if(_context->hurry_up)
		{
			if(flagz)
			{
			*flagz=frameType();
			}
 			return 1;
		}
		// convert ffmpeg to our format : yv12
		uint8_t **src;
		uint32_t stridex[3];
		uint8_t  *inx[3];
		//printf("cspace %d\n", _context->pix_fmt);

		switch(_context->pix_fmt)
		{
		case PIX_FMT_YUV411P:
			stridex[0]=	_frame.linesize[0 ];
			stridex[1]=	_frame.linesize[1 ];
			stridex[2]=	_frame.linesize[2 ];

			inx[0]=_frame.data[0];
			inx[1]=_frame.data[1];
			inx[2]=_frame.data[2];


			COL_411_YV12(inx,stridex,_internalBuffer,_w,_h);

			oBuff[0]=_internalBuffer;
		 	oBuff[1]=_internalBuffer+_w*_h;
 		 	oBuff[2]=oBuff[1]+((_w*_h)>>2);
			src= (uint8_t **)oBuff;
			_frame.linesize[0 ]=_w;
			_frame.linesize[1 ]=_w>>1;
			_frame.linesize[2 ]=_w>>1;
			// No pp
			out->_qStride=0;
			break;

		case PIX_FMT_YUV422P:
			//printf("422p\n");
			stridex[0]=	_frame.linesize[0 ];
			stridex[1]=	_frame.linesize[1 ];
			stridex[2]=	_frame.linesize[2 ];

			inx[0]=_frame.data[0];
			inx[1]=_frame.data[1];
			inx[2]=_frame.data[2];

			
			COL_422_YV12(inx,stridex,_internalBuffer,_w,_h);

			oBuff[0]=_internalBuffer;
		 	oBuff[1]=_internalBuffer+_w*_h;
 		 	oBuff[2]=oBuff[1]+((_w*_h)>>2);
			src= (uint8_t **)oBuff;
			_frame.linesize[0 ]=_w;
			_frame.linesize[1 ]=_w>>1;
			_frame.linesize[2 ]=_w>>1;
			// No pp
			out->_qStride=0;
			
			break;
		
		case PIX_FMT_YUV420P:
		case PIX_FMT_YUVJ420P:
		// Default is YV12 or I420
		// In that case depending on swap u/v
		// we do it or not
	
				//printf("420p\n");
				src= (uint8_t **) _frame.data;
#if 1				
				if(out->quant)
				{
					if(_frame.qstride<out->_qStride || ! out->_qStride)
					{
						printf("FF43: Stride differs %d %d\n",
								_frame.qstride,out->_qStride);
						out->_qStride=0;
					}
					else
					{
						uint8_t *src,*dst;
						src=(uint8_t *)_frame.qscale_table;
						dst=out->quant;
						// Pack them
						for(uint32_t y=0;y<((_h+15)>>4);y++)
						{				
							memcpy(dst,src,out->_qStride);
							src+=_frame.qstride;
							dst+=out->_qStride;
							
						}
					}
				}
#endif				
#if 0
			if(_postproc.postProcType && _postproc.postProcStrength)
			{ 	// we do postproc !
				// keep

		 				oBuff[0]=_internalBuffer;
		 				oBuff[1]=_internalBuffer+_frame.linesize[0]*_h;
 		 				oBuff[2]=oBuff[1]+((_frame.linesize[1]*_h)>>1);
        			for(uint32_t s=0;s<3;s++)
	          		{
			            strideTab[s]=strideTab2[s]=_frame.linesize[s];
            			}
		 		   pp_postprocess(
		      			_frame.data,
		        		strideTab,
		          		oBuff,
		         		strideTab2,
		      			_w,
		        		_h,
		          		_frame.qscale_table,
		          		_frame.qstride,
		         		_postproc.ppMode,
		          		_postproc.ppContext,
		          		_frame.pict_type);
			  	src= (uint8_t **)oBuff;
			}
#endif
			break;
/*
uint8_t COL_RawRGB32toYV12(uint8_t *data1,uint8_t *data2, uint8_t *oy,uint8_t *oy2, 
				uint8_t *u, uint8_t *v,uint32_t lineSize)
*/			
		case PIX_FMT_RGBA32:
				{
				uint32_t stride;
				uint8_t *data1,*data2;
				uint8_t *oy,*oy2,*u,*v;
				
						
						stride=	_frame.linesize[0 ];
						data1=(uint8_t *)_frame.data[0];
						data2=(uint8_t *)_frame.data[0]+stride;
						oy=out->data;
						oy2=out->data+_w;
						if(!_swapUV)
						{
							u=out->data+(_w*_h);
							v=out->data+((_w*_h*5)>>2);
						
						}
						else
						{
							v=out->data+(_w*_h);
							u=out->data+((_w*_h*5)>>2);	
						}
						
						 COL_RawRGB32toYV12(data1,data2, oy,oy2, 
							u, v,_w,_h,stride)	;
						_lastQ=0; //_context->quality;	
						if(flagz)
						{
							*flagz=frameType();
						}
						out->_qStride=0;
 						return 1;
				}
		default:
				printf("\n Unhandled colorspace:%d\n",_context->pix_fmt);
				return 0;
				
		}
		// Pack the output so that stride=width
		
		uint8_t *tmp;
		tmp=src[0];
		uint8_t *otmp= out->data;
		uint32_t stride;

		stride=  _frame.linesize[0 ];

		for(uint32_t y=_h;y>0;y--)
			{
				 	memcpy(otmp,tmp,_w);
				  	tmp+=stride;
   				  	otmp+=_w;
				}
		

		if(!_swapUV)
		{
			tmp= src[1];
			stride=  _frame.linesize[1];
		}
		else
		{
			tmp= src[2];			
			stride=  _frame.linesize[2];
		}
			

		otmp= out->data+((5*_w*_h)>>2);

		for(uint32_t y=_h>>1;y>0;y--)
			{
				 	memcpy(otmp,tmp,_w>>1);
				  	tmp+=stride;
   				  	otmp+=_w>>1;
				}
		if(!_swapUV)
		{
			tmp= src[2];			
			stride=  _frame.linesize[2];
		}
		else
		{
			tmp= src[1];
			stride=  _frame.linesize[1];
		}
		otmp= out->data+_w*_h;

		for(uint32_t y=_h>>1;y>0;y--)
			{
				 	memcpy(otmp,tmp,_w>>1);
				  	tmp+=stride;
   				  	otmp+=_w>>1;
				}

		_lastQ=0; //_context->quality;
	//	printf("FF %d :: \n",_frame.pict_type);
		out->_Qp=(_frame.quality*32)/FF_LAMBDA_MAX;
		//printf("%d %d\n",_frame.quality,FF_LAMBDA_MAX);
		out->flags=frameType();
		if(flagz)
		{

			*flagz=out->flags;
		}
		
 		return 1;
}



decoderFFDiv3::decoderFFDiv3(uint32_t w,uint32_t h)    :decoderFF(w,h) 
{
		      if (avcodec_open(_context, &msmpeg4v3_decoder) < 0) 
		      {
					printf(" Decoder init: FFMpeg MS MPEG43 video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg MS MPEG43 video decoder initialized!\n");
				}
}              
decoderFFMpeg4VopPacked::decoderFFMpeg4VopPacked(uint32_t w,uint32_t h)       :decoderFF(w,h)
{
// force low delay as avidemux don't handle B-frames


      if (avcodec_open(_context,& mpeg4_decoder) < 0)
	      {
				printf(" Decoder init: FFMpeg MPEG4 video decoder failed!\n");
		}
	else
		{
				printf(" Decoder init: FFMpeg MPEG4 video vop packed  decoder initialized!\n");
		}
	_allowNull=1;
}
decoderFFMpeg4::decoderFFMpeg4(uint32_t w,uint32_t h)       :decoderFF(w,h)
{
// force low delay as avidemux don't handle B-frames

		  _context->flags|=CODEC_FLAG_LOW_DELAY;
 		//  _context->flags|=FF_DEBUG_VIS_MV;
      if (avcodec_open(_context,& mpeg4_decoder) < 0)
	      {
					printf(" Decoder init: FFMpeg MPEG4 video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg MPEG4 video decoder initialized!\n");
				}
}
decoderFFDV:: decoderFFDV(uint32_t w,uint32_t h,uint32_t l,uint8_t *d)        :decoderFF(w,h)
{
      _context->extradata=(void *)d;
      _context->extradata_size=(int)l;
      if (avcodec_open(_context,& dvvideo_decoder) < 0) 
	      {
					printf(" Decoder init: FFMpeg DV video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg DV video decoder initialized!\n");
				}
	
}
decoderFFMP42::decoderFFMP42(uint32_t w,uint32_t h)       :decoderFF(w,h)
{
      if (avcodec_open(_context,& msmpeg4v2_decoder) < 0)
	      {
					printf(" Decoder init: FFMpeg MP42 video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg MP42 video decoder initialized!\n");
				}

}
decoderFFMpeg12::decoderFFMpeg12(uint32_t w,uint32_t h,uint32_t extraLen,uint8_t *extraData)
	    :decoderFF(w,h)
{
int got_picture=0;
  	_context->flags|=CODEC_FLAG_LOW_DELAY;
      if (avcodec_open(_context,& mpeg1video_decoder) < 0)
	      {
					printf(" Decoder init: FFMpeg Mpeg1/2 video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg Mpeg1/2 video decoder initialized!\n");
				}

}
decoderFFSVQ3::decoderFFSVQ3(uint32_t w,uint32_t h,uint32_t extraLen,uint8_t *extraData)
	    :decoderFF(w,h)
{
int got_picture=0;
  	_context->flags|=CODEC_FLAG_LOW_DELAY;
      if (avcodec_open(_context,& svq3_decoder) < 0)
	      {
					printf(" Decoder init: FFMpeg SVQ3 video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg SVQ3 video decoder initialized!\n");
				}

}

decoderFFH263::decoderFFH263(uint32_t w,uint32_t h)       :decoderFF(w,h)
{
      if (avcodec_open(_context,& h263_decoder) < 0)
	      {
					printf(" Decoder init: FFMpeg h263 video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg h263 video decoder initialized!\n");
				}

}
decoderFFV1::decoderFFV1(uint32_t w,uint32_t h)       :decoderFF(w,h)
{
      if (avcodec_open(_context,& ffv1_decoder) < 0)
	      {
					printf(" Decoder init: FFMpeg ffV1 video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg ffv1 video decoder initialized!\n");
				}

}

decoderFFhuff::decoderFFhuff(uint32_t w,uint32_t h,uint32_t l,uint8_t *d)       :decoderFF(w,h) 
{
      _context->extradata=(void *)d;
      _context->extradata_size=(int)l;
      
      if (avcodec_open(_context,& huffyuv_decoder) < 0)
	      {
					printf(" Decoder init: FFMpeg huffy_uv video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg huffy_uv video decoder initialized!\n");
				}
	
}
decoderFFWMV2::decoderFFWMV2(uint32_t w,uint32_t h,uint32_t l,uint8_t *d)       :decoderFF(w,h)
{
      _context->extradata=(void *)d;
      _context->extradata_size=(int)l;
      
      if (avcodec_open(_context,& wmv2_decoder) < 0) 
	      			{
					printf(" Decoder init: FFMpeg WMV2 video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg WMV2 video decoder initialized!\n");		
				}
	
}
decoderFFcyuv::decoderFFcyuv(uint32_t w,uint32_t h,uint32_t l,uint8_t *d)       :decoderFF(w,h)
{
      _context->extradata=(void *)d;
      _context->extradata_size=(int)l;

      if (avcodec_open(_context,& cyuv_decoder) < 0)
	      			{
					printf(" Decoder init: FFMpeg cyuv video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg cyuv video decoder initialized!\n");
				}

}
decoderFFMJPEG::decoderFFMJPEG(uint32_t w,uint32_t h)       :decoderFF(w,h)
{


      if (avcodec_open(_context,& mjpeg_decoder) < 0)
	      			{
					printf(" Decoder init: FFMpeg mjpeg video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg mjpeg video decoder initialized!\n");
				}

}
decoderSnow::decoderSnow(uint32_t w,uint32_t h)       :decoderFF(w,h)
{


      if (avcodec_open(_context,& snow_decoder) < 0)
	      			{
					printf(" Decoder init: FFMpeg snow video decoder failed!\n");
				}
				else
				{
					printf(" Decoder init: FFMpeg snow video decoder initialized!\n");
				}

}

#endif
