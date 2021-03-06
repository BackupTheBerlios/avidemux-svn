/***************************************************************************
                          adm_encffmpeg.cpp  -  description
                             -------------------
    begin                : Tue Sep 10 2002
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
#include <math.h>
#include <unistd.h>

#include <time.h>
#include <sys/time.h>
#include <config.h>

#ifdef USE_FFMPEG
#include "ADM_lavcodec.h"

#include "fourcc.h"
#include "avi_vars.h"
#include "ADM_toolkit/toolkit.hxx"

#include "ADM_encoder/ADM_vidEncode.hxx"

#include "ADM_video/ADM_genvideo.hxx"
//#include "ADM_codecs/ADM_divxEncode.h"
#include "ADM_codecs/ADM_ffmpeg.h"
#include "ADM_encoder/adm_encoder.h"
#include "ADM_encoder/adm_encffmpeg.h"

#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_ENCODER
#include "ADM_toolkit/ADM_debug.h"

extern "C" {
#include "ADM_encoder/xvid_vbr.h"
};
#include "adm_encffmatrix.h"

#define FIRST_PASS_QUANTIZER 6
#define USE_FFMPEG_2PASS 1

/*_________________________________________________*/
EncoderFFMPEGMpeg1::EncoderFFMPEGMpeg1 (FF_CODEC_ID id, FFMPEGConfig *config)
: EncoderFFMPEG (id,config)

{
  _frametogo = 0;
 _pass1Done=0;
 _lastQz=0;
 _lastBitrate=0;
 memcpy(&_param,&(config->generic),sizeof(_param));
 memcpy(&_settings,&(config->specific),sizeof(_settings));
 
};

uint8_t EncoderFFMPEGMpeg1::encode (uint32_t frame, uint32_t * len, uint8_t * out,
			  uint32_t * flags)
{
uint32_t l,f;
  assert (_codec);
  assert (_in);

  	if (!_in->getFrameNumberNoAlloc (frame, &l, _vbuffer, &f))
    	{
      		printf ("\n Error : Cannot read incoming frame !");
      		return 0;
    	}
	switch(_state)
	{
		case enc_CQ:
		case enc_CBR:
      				return _codec->encode (_vbuffer, out, len, flags);
				break;
		case enc_Pass1:
					// collect result
					if(!_codec->encode(   _vbuffer,out,len,flags))
							{
								printf("\n codec error on 1st pass !");
								return 0;
							}
                                       			_frametogo++;
							return 1;
							break;
		case enc_Pass2:
        							*flags=0;
                      						if(!_codec->encode(_vbuffer,out,len,flags)) 
									return 0;		
							return 1;
					break;
		default:
				assert(0);
				break;
		}
		assert(0);
		return 0;

}
EncoderFFMPEGMpeg1::~EncoderFFMPEGMpeg1()
{ stop();};


uint8_t EncoderFFMPEGMpeg1::getLastQz( void) 
{
  myENC_RESULT enc;

	_codec->getResult((void *)&enc);   
	return enc.out_quantizer;

}

//--------------------------------
uint8_t	EncoderFFMPEGMpeg1::configure (AVDMGenericVideoStream * instream)
{

  assert (instream);
  ADV_Info *info;

	uint32_t flag1,flag2,flag3;
	flag1=flag2=flag3=0;

	  info = instream->getInfo ();
  	_fps=info->fps1000;
  	_w = info->width;
  	_h = info->height;
  	_vbuffer = new uint8_t[_w * _h * 3];
  	assert (_vbuffer);
  	_in = instream;


  switch (_param.mode)
    {
    case COMPRESS_CQ:
				printf("ffmpeg mpeg1 cq mode: %ld\n",_param.qz);
      				_state = enc_CQ;
				setMatrix(); //_settings.user_matrix,_settings.gop_size);
     				_codec = new ffmpegEncoderCQ (_w, _h,_id);
     				_codec->setConfig(&_settings);
      				_codec->init (_param.qz,_fps,0);
      				break;
    case COMPRESS_CBR:
				printf("ffmpeg mpeg1 cbr mode: %ld\n",_param.bitrate);
      				_state = enc_CBR;
				setMatrix();
     				_codec = new ffmpegEncoderCBR (_w, _h,_id);
				_codec->setConfig(&_settings);
			
				flag1=1;

     				_codec->init (_param.bitrate,_fps,flag1);

      				break;
    case COMPRESS_2PASS:
				{
				ffmpegEncoderCQ *cdec=NULL;
						_state = enc_Pass1;
						printf("\n ffmpeg dual size: %lu",_param.finalsize);
						setMatrix();
						cdec = new ffmpegEncoderCQ (_w, _h,_id); // Pass1
			    			cdec->setConfig(&_settings);
						cdec->setLogFile("/tmp/dummylog.txt");
						cdec->init (FIRST_PASS_QUANTIZER,_fps,1);
						_codec=cdec;
				}
   				 break;
    default:
      				assert (0);
				break;

    }
  _in = instream;
  printf ("\n ffmpeg Encoder , w: %lu h:%lu mode:%d", _w, _h, _state);
  return 1;

}



uint8_t EncoderFFMPEGMpeg1::startPass1 (void)
{
 assert (_state == enc_Pass1);
  _frametogo = 0;
  printf ("\n Starting pass 1\n");
  printf (" Creating logfile :%s\n", _logname);
  _pass1Done=1;  	
  return 1;
}


uint8_t EncoderFFMPEGMpeg1::isDualPass (void)
{
    if ((_state == enc_Pass1) || (_state == enc_Pass2))
    {
      return 1;
    }
  return 0;
}

uint8_t EncoderFFMPEGMpeg1::setLogFile (const char *lofile, uint32_t nbframe)
{
   strcpy (_logname, lofile);
  _frametogo = nbframe;
   _totalframe= nbframe;
  return 1;

}

//_______________________________
uint8_t EncoderFFMPEGMpeg1::stop (void)
{
	if(_codec)	  delete       _codec;
	_codec = NULL;
	return 1;
}
uint8_t EncoderFFMPEGMpeg1::startPass2 (void)
{
uint32_t vbr;
uint32_t avg_bitrate;
 assert (_state = enc_Pass1);
  printf ("\n Starting pass 2\n");


   float db,ti;

   	db= _param.finalsize;
	db=db*1024.*1024.*8.;
	// now deb is in Bits
	  
	  // compute duration
	  ti=frameEnd-frameStart+1;
	  ti*=1000;
	  ti/=_fps;  // nb sec
	  db=db/ti;
	  
	  avg_bitrate=(uint32_t)floor(db);
	  
	printf("\n ** Total size     : %lu MBytes \n",_param.finalsize);
	printf(" ** Total frame    : %lu  \n",_totalframe);
	printf(" ** Average bitrate: %d \n",avg_bitrate/1000);

	
	

  printf ("\n VBR parameters computed\n");
  _state = enc_Pass2;
  old_bits = 0;
  // Delete codec and start new one
  if (_codec)
    {
      delete _codec;
      _codec = NULL;
    }

  
  
  _codec=  new ffmpegEncoderVBR (_w, _h,_internal ,_id); //0 -> external 1 -> internal
  _codec->setConfig(&_settings);
  setMatrix();
   //_codec->setLogFile(_logname);
   _codec->setLogFile("/tmp/dummylog.txt");
   _codec->init (avg_bitrate,_fps);
  
  printf ("\n ready to encode in 2pass (%s)\n",_logname);
  _frametogo=0;
  return 1;

}
/*
uint8_t  EncoderFFMPEGMpeg1::updateStats (uint32_t len)
{

	 myENC_RESULT enc;

      	 _codec->getResult(&enc);
#ifndef USE_FFMPEG_2PASS
	vbrUpdate(&vbrstate,
					enc.quantizer,
					enc.is_key_frame,
					0,
					len,
					0,0,0);
#endif
			return 1;
}
*/
//
//	Allow the user to set a custom matrix
//     The size parameter is not used
//
// *intra_matrix;
//	uint16_t *inter_matrix;
uint8_t  EncoderFFMPEGMpeg1::setMatrix( void)
{
		switch(_settings.user_matrix)
					{
						case ADM_MATRIX_DEFAULT:
								break;
						case ADM_MATRIX_TMP:
								printf("\n using custom matrix : Tmpg\n");
								_settings.intra_matrix=		tmpgenc_intra;
								_settings.inter_matrix=		tmpgenc_inter;
								break;
						case ADM_MATRIX_ANIME:
								printf("\n using custom matrix : anim\n");
								_settings.intra_matrix=		anime_intra;
								_settings.inter_matrix=		anime_inter;

								break;
						case ADM_MATRIX_KVCD:
								printf("\n using custom matrix : kvcd\n");
								_settings.intra_matrix=		kvcd_intra;
								_settings.inter_matrix=		kvcd_inter;
								break;
					}
		return 1;
}

#endif



