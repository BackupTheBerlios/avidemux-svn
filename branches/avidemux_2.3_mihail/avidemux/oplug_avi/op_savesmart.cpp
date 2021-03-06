/***************************************************************************
                          op_savesmart.cpp  -  description
                             -------------------
    begin                : Mon May 6 2002
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
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include <time.h>
#include <sys/time.h>

#ifdef USE_FFMPEG
extern "C" {
	#include "ADM_lavcodec.h"
	};
#endif

#include "fourcc.h"
#include "avi_vars.h"
#ifdef HAVE_ENCODER

#include "ADM_toolkit/toolkit.hxx"

#include "ADM_audio/aviaudio.hxx"
#include "ADM_audiofilter/audioprocess.hxx"

#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_filter/video_filters.h"
#include "ADM_codecs/ADM_divxEncode.h"
#include "ADM_encoder/ADM_vidEncode.hxx"

#include "ADM_assert.h" 


#include "oplug_avi/op_aviwrite.hxx"
#include "oplug_avi/op_avisave.h"
#include "oplug_avi/op_savesmart.hxx"

#ifdef USE_FFMPEG		
#include "ADM_codecs/ADM_ffmpeg.h"		
#endif


#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_SAVE_AVI
#include "ADM_toolkit/ADM_debug.h"
GenericAviSaveSmart::GenericAviSaveSmart(uint32_t qf) : GenericAviSave()
{
	_cqReenc=qf;
	ADM_assert(qf>=2 && qf<32);
	_nextip=0;
	encoderReady=0;
        _hasBframe=0;
}
uint8_t	GenericAviSaveSmart::setupVideo (char *name)
{

int value=4;;
	 

		printf("\n Q: %u",_cqReenc);
  // init save avi
  memcpy(&_bih,video_body->getBIH (),sizeof(_bih));
  memcpy(&_videostreamheader,video_body->getVideoStreamHeader (),sizeof( _videostreamheader));
  memcpy(&_mainaviheader,video_body->getMainHeader (),sizeof(_mainaviheader));
 
   uint8_t *extraData;
   uint32_t extraLen;
  _lastIPFrameSent=0xfffffff;
   video_body->getExtraHeaderData(&extraLen,&extraData);

  if (!writter->saveBegin (name,
			   &_mainaviheader,
			   frameEnd - frameStart + 1, 
			   &_videostreamheader,
			   &_bih,
			   extraData,extraLen,
			   audio_filter,
			   NULL
		))
    {

      return 0;
    }
  compEngaged = 0;
  encoderReady = 0;
  _encoder = NULL;
  aImage=new ADMImage(_mainaviheader.dwWidth,_mainaviheader.dwHeight);
  _incoming = getFirstVideoFilter (frameStart,frameEnd-frameStart);
  encoding_gui->setFps(_incoming->getInfo()->fps1000);
  encoding_gui->setPhasis("Smart Copy");
  // B frame ?
    for(int i=frameStart;i<frameEnd;i++)
    {
      if(!video_body->getFlags ( i, &_videoFlag)) break;
      if(_videoFlag & AVI_B_FRAME) _hasBframe=1;
    }
    if(_hasBframe) printf("The original file has bframe, expect a shift of 1 frame\n");
  //
  return 1;
  //---------------------
}

//
//      Just to keep gcc happy....
//
GenericAviSaveSmart::~GenericAviSaveSmart ()
{
  if (encoderReady && _encoder)
    {
      _encoder->stopEncoder ();
    }
  if (_encoder)
    delete      _encoder;
 if(aImage)
 {
 	delete aImage;
	aImage=NULL;
 }
}

// copy mode
// Basically ask a video frame and send it to writter
uint8_t
GenericAviSaveSmart::writeVideoChunk (uint32_t frame)
{
  uint32_t    len;
  uint8_t     ret1, seq;

  frame+=frameStart;
  if (compEngaged)		// we were re-encoding
    {
    	return writeVideoChunk_recode(frame);
    }
    return writeVideoChunk_copy(frame);
}
//_________________________________________________________
uint8_t GenericAviSaveSmart::writeVideoChunk_recode (uint32_t frame)
{
uint32_t len;
ADMBitstream bitstream;
	aprintf("Frame %lu encoding\n",frame);
	video_body->getFlags ( frame, &_videoFlag);
	if (_videoFlag & AVI_KEY_FRAME)
	{
	  aprintf("Smart: Stopping encoder\n");
	  // It is a kf, go back to copy mode
	  compEngaged = 0;
	  stopEncoder ();	// Tobias F
	  delete	    _encoder;
	  _encoder = NULL;
	  return writeVideoChunk_copy(frame);
	}
	// Else encode it ....
	//1-Read it
	if (! video_body->getUncompressedFrame (frame, aImage))
		return 0;
	// 2-encode it
        bitstream.data=vbuffer;
        bitstream.cleanup(frame);
        if (!_encoder->encode (aImage, &bitstream))//vbuffer, &len, &_videoFlag))
		return 0;
        _videoFlag=bitstream.flags;
	// 3-write it
	return writter->saveVideoFrame (bitstream.len, _videoFlag, vbuffer);
}
//_________________________________________________________
uint8_t GenericAviSaveSmart::writeVideoChunk_copy (uint32_t frame)
{
  // Check flags and seq
  uint32_t myseq=0;
  uint32_t nextip,len;
  uint8_t seq;
  
  	aprintf("Frame %lu copying\n",frame);
        
  	// all gop should be closed, so it should be safe to do it here
	if(muxSize)
      	{
		// we overshot the limit and it is a key frame
		// start a new chunk
		if(handleMuxSize() && (_videoFlag & AVI_KEY_FRAME))
		{		
		 	uint8_t *extraData;
			uint32_t extraLen;

   			video_body->getExtraHeaderData(&extraLen,&extraData);
					   
			if(!reigniteChunk(extraLen,extraData)) return 0;
		}
	}
  
  	video_body->getFlags( frame,&_videoFlag);	
        if(frame==frameStart)
        {
          if(!(_videoFlag & AVI_KEY_FRAME))
          {
            aprintf("1st frame is not a kef:There is a broken reference, encoding\n");
            compEngaged = 1;
            initEncoder (_cqReenc);
            return writeVideoChunk_recode(frame);
            
          }
        }
	
  	if(_videoFlag & AVI_B_FRAME) // lookup next I/P frame
	{
		if(_nextip<frame) // new forward frame
		{
			aprintf("Smart:New forward frame\n");
			if(!seekNextRef(frame,&nextip))
			{
				aprintf("Smart:B Frame without reference frame\n");
				return 1;
			}
			// check if that the frame -1,....,next forward ref are all sequential
			if(!video_body->sequentialFramesB(frame-1,nextip)&&!(_videoFlag &AVI_KEY_FRAME )) 
			{
				aprintf("Smart:There is a broken reference, encoding\n");
				compEngaged = 1;
				initEncoder (_cqReenc);
				return writeVideoChunk_recode(frame);
			}
			
			aprintf("Smart : using %lu as next\n",nextip);
			// Seems ok, write it and mark it
			if (! video_body->getFrameNoAlloc (nextip, vbuffer, &len,
				      &_videoFlag, &seq))
    				return 0;
			_nextip=nextip;
			return writter->saveVideoFrame (len, _videoFlag, vbuffer);
		}
		else
		{	// Nth B frame
			aprintf("Smart:Next B frame\n");
			if (!video_body->getFrameNoAlloc (frame-1, vbuffer, &len,
				      &_videoFlag, &seq))
    				return 0;
			return writter->saveVideoFrame (len, _videoFlag, vbuffer);
		}
	}
	// Not a bframe
	// Is it the frame we sent previously ?
	if(frame==_nextip && _nextip)
	{
		// Send the last B frame instead
		aprintf("Smart finishing B frame %lu\n",frame-1);
		if (! video_body->getFrameNoAlloc (frame-1, vbuffer, &len,
		      &_videoFlag, &seq))
    			return 0;
		return writter->saveVideoFrame (len, _videoFlag, vbuffer);
	
	}
	// Regular frame
	// just copy it
	if(frame)
		if(!video_body->sequentialFramesB(_nextip,frame)&&!(_videoFlag &AVI_KEY_FRAME ))  // Need to re-encode
		{
			aprintf("Seq broken..\n");
			compEngaged = 1;
			initEncoder (_cqReenc);
			return writeVideoChunk_recode(frame);
		}
	_nextip=frame;
	aprintf("Smart: regular\n");
	if(! video_body->getFrameNoAlloc (frame, vbuffer, &len,
				      &_videoFlag, &seq)) return 0;
 
	
	
	return writter->saveVideoFrame (len, _videoFlag, vbuffer);
}
//_________________________________________________________
uint8_t GenericAviSaveSmart::seekNextRef(uint32_t frame,uint32_t *nextip)
{
uint32_t flags;
	for(uint32_t i=frame+1;i<frameEnd;i++)
	{
		video_body->getFlags( i,&flags);
		if(!(flags & AVI_B_FRAME)) 
		{
			*nextip=i;
			return 1;
		}
	
	}
	return 0;
}
 //
 //
uint8_t
GenericAviSaveSmart::initEncoder (uint32_t qz)
{
  aviInfo
    info;
  video_body->getVideoInfo (&info);
  ADM_assert (0 == encoderReady);
  encoderReady = 1;
  uint8_t ret=0;
  FFcodecSetting myConfig=
	 {
	 ME_EPZS,//	ME
	 0, // 		GMC	
	 0,	// 4MV
	 0,//		_QPEL;	 
	 0,//		_TREILLIS_QUANT
	 2,//		qmin;
	 31,//		qmax;
	 3,//		max_qdiff;
	 1,//		max_b_frames;
	 0, //		mpeg_quant;
	 1, //
	 -2, // 		luma_elim_threshold;
	 1,//
	 -5, 		// chroma_elim_threshold;
	 0.05,		//lumi_masking;
	 1,		// is lumi
	 0.01,		//dark_masking; 
	 1,		// is dark
 	 0.5,		// qcompress amount of qscale change between easy & hard scenes (0.0-1.0
    	 0.5,		// qblur;    amount of qscale smoothing over time (0.0-1.0) 
	0,		// min bitrate in kB/S
	0,		// max bitrate
	0, 		// default matrix
	0, 		// no gop size
	NULL,
	NULL,
	0,		// interlaced
	0,		// WLA: bottom-field-first
	0,		// wide screen
	2,		// mb eval = distortion
	8000,		// vratetol 8Meg
	0,		// is temporal
	0.0,		// temporal masking
	0,		// is spatial
	0.0,		// spatial masking
	0,		// NAQ
	0		// DUMMY 
 	} ;


  if(  isMpeg4Compatible(info.fcc) )
  	{
/*	
#ifdef USE_DIVX		 
		 	 _encoder = new divxEncoderCQ (info.width, info.height);
	   
#else			
*/
// 	uint8_t				setConfig(FFcodecSetting *set);	
			ffmpegEncoderCQ *tmp;		
			tmp = new ffmpegEncoderCQ (info.width, info.height,FF_MPEG4);					
			myConfig.max_b_frames=_hasBframe; // In fact does the original have b frame ?
			tmp->setConfig(&myConfig);
			printf("\n init qz %ld\n",qz);
	    		ret= tmp->init (qz,25000);
			_encoder=tmp;
/*			
#endif		  		  
*/
		
#warning 25 fps hardcoded

		 }
		 else
		 {
#ifdef USE_FFMPEG			 
			 if(isMSMpeg4Compatible(info.fcc) ) // DIV3
			 {
				 ffmpegEncoderCQ *tmp;
				  tmp = new ffmpegEncoderCQ (info.width, info.height,FF_MSMP4V3);
                                  myConfig.max_b_frames=0;
				  tmp->setConfig(&myConfig);
			    	  ret= tmp->init (qz,25000);
			    	  _encoder=tmp;
				}
				else
					{
				       ADM_assert(0);
					}			
			}
#else
			ADM_assert(0);
			}			
#endif

		return ret;
}

uint8_t
GenericAviSaveSmart::stopEncoder (void)
{
  ADM_assert (1 == encoderReady);
  encoderReady = 0;
  return (_encoder->stopEncoder ());
}

#endif
