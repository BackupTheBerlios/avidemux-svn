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

#include <sstream>
#include <string>

#include "ADM_default.h"
#include "ADM_threads.h"

extern "C" {
	#include "ADM_lavcodec.h"
}

#include "fourcc.h"
#include "avi_vars.h"
#include "ADM_audio/aviaudio.hxx"
#include "ADM_audiofilter/audioprocess.hxx"

#include "ADM_videoFilter.h"
#include "ADM_videoFilter_internal.h"

#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_encoder/adm_encConfig.h"
#include "ADM_encoder/ADM_externalEncoder.h"

#include "op_aviwrite.hxx"
#include "op_avisave.h"
#include "op_savesmart.hxx"

#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_SAVE_AVI
#include "ADM_osSupport/ADM_debug.h"

extern struct COMPRES_PARAMS *AllVideoCodec;

GenericAviSaveSmart::GenericAviSaveSmart(uint32_t qf) : GenericAviSave()
{
	_cqReenc=qf;
	ADM_assert(qf>=2 && qf<32);
	_nextip=0;
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
  _encoder = NULL;
  _incoming = getFirstVideoFilter (frameStart,frameEnd-frameStart);
  encoding_gui->setFps(_incoming->getInfo()->fps1000);
  encoding_gui->setPhasis(QT_TR_NOOP("Smart Copy"));
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
  cleanupAudio();
  deleteEncoder();
}

// copy mode
// Basically ask a video frame and send it to writter
int GenericAviSaveSmart::writeVideoChunk (uint32_t frame)
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
int GenericAviSaveSmart::writeVideoChunk_recode (uint32_t frame)
{
uint32_t len;
ADMBitstream bitstream(MAXIMUM_SIZE * MAXIMUM_SIZE * 3);
	aprintf("Frame %lu encoding\n",frame);
	video_body->getFlags ( frame, &_videoFlag);
	if (_videoFlag & AVI_KEY_FRAME)
	{
	  aprintf("Smart: Stopping encoder\n");
	  // It is a kf, go back to copy mode
	  compEngaged = 0;
	  deleteEncoder();

	  return writeVideoChunk_copy(frame,1);
	}
	// Else encode it ....
	// 1-encode it
        bitstream.data=vbuffer;
        bitstream.cleanup(frame);
	    if (!_encoder->encode(frame, &bitstream))
		return -1;
        _videoFlag=bitstream.flags;
	// 2-write it
        encoding_gui->setFrame(frame-frameStart,bitstream.len,bitstream.out_quantizer,frametogo);
	if (writter->saveVideoFrame (bitstream.len, _videoFlag, vbuffer))
		return bitstream.len;
	else
		return -1;
}

//_________________________________________________________
int GenericAviSaveSmart::writeVideoChunk_copy (uint32_t frame,uint32_t first)
{
  // Check flags and seq
  uint32_t myseq=0;
  uint32_t nextip;
  uint8_t seq;
  ADMCompressedImage img;
  
  img.data=vbuffer;
  
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
					   
			if(!reigniteChunk(extraLen,extraData)) return -1;
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
				return 0;
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
			if (! video_body->getFrameNoAlloc (nextip,&img,&seq))// vbuffer, &len,		      &_videoFlag, &seq))
    				return -1;
                        _videoFlag=img.flags;
			_nextip=nextip;
                        encoding_gui->setFrame(frame-frameStart,img.dataLength,0,frametogo);

						if (writter->saveVideoFrame (img.dataLength, img.flags, img.data))
							return img.dataLength;
						else
							return -1;
		}
		else
		{	// Nth B frame
			aprintf("Smart:Next B frame\n");
			if (!video_body->getFrameNoAlloc (frame-1, &img, &seq))
    				return -1;
                         _videoFlag=img.flags;
                        encoding_gui->setFrame(frame-frameStart,img.dataLength,0,frametogo);
			if (writter->saveVideoFrame (img.dataLength, img.flags, img.data))
				return img.dataLength;
			else
				return -1;
		}
	}
	// Not a bframe
	// Is it the frame we sent previously ?
	if(frame==_nextip && _nextip)
	{
		// Send the last B frame instead
		aprintf("Smart finishing B frame %lu\n",frame-1);
		if (! video_body->getFrameNoAlloc(frame-1, &img, &seq))// (frame-1, vbuffer, &len,    &_videoFlag, &seq))
    			return -1;
                 _videoFlag=img.flags;
                encoding_gui->setFrame(frame-frameStart,img.dataLength,0,frametogo);
		if (writter->saveVideoFrame  (img.dataLength, img.flags, img.data))
			return img.dataLength;
		else
			return -1;
	
	}
	// Regular frame
	// just copy it
	if(frame)
		if(!video_body->sequentialFramesB(_nextip,frame)&&!(_videoFlag &AVI_KEY_FRAME ))  // Need to re-encode
		{
			aprintf("Seq broken..\n");
			compEngaged = 1;
			initEncoder (_cqReenc);
                        encoding_gui->setFrame(frame-frameStart,img.dataLength,0,frametogo);
			return writeVideoChunk_recode(frame);
		}
	_nextip=frame;
	aprintf("Smart: regular\n");
	if(! video_body->getFrameNoAlloc (frame, &img, &seq)) return 0;
        _videoFlag=img.flags;
	
	encoding_gui->setFrame(frame-frameStart,img.dataLength,0,frametogo);
	if(first)
	{
		ADM_assert(_videoFlag == AVI_KEY_FRAME);
		// Grab extra data ..
			uint8_t *extraData;
			uint32_t extraLen;
			uint8_t r;
   			video_body->getExtraHeaderData(&extraLen,&extraData);
			if(extraLen)
			{
			//********************************************************************
			// If we have global headers we have to duplicate the old headers as they were replaced
			// by the new headers from the section we re-encoded
			//********************************************************************
				printf("[Smart] Duplicating vop header (%d bytes)\n",extraLen);
				uint8_t *buffer=new uint8_t[extraLen+img.dataLength];
				memcpy(buffer,extraData,extraLen);
				memcpy(buffer+extraLen,img.data,img.dataLength);
				r=writter->saveVideoFrame (img.dataLength+extraLen, img.flags, buffer);;
				delete [] buffer;

				if (r)
					return img.dataLength + extraLen;
				else
					return -1;				
			}
	}
	if (writter->saveVideoFrame (img.dataLength, img.flags, img.data))
		return img.dataLength;
	else
		return -1;

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

uint8_t GenericAviSaveSmart::initEncoder (uint32_t qz)
{
	aviInfo info;

	video_body->getVideoInfo (&info);

	if (_encoder)
		deleteEncoder();

	if (isMpeg4Compatible(info.fcc))
	{
		std::stringstream out;

		out << "<?xml version='1.0'?><Mpeg4aspConfig><Mpeg4aspOptions><motionEstimationMethod>epzs</motionEstimationMethod><fourMotionVector>false</fourMotionVector><maximumBFrames>";
		out << _hasBframe;
		out << "</maximumBFrames><quarterPixel>false</quarterPixel><globalMotionCompensation>false</globalMotionCompensation><quantisationType>h263</quantisationType><macroblockDecisionMode>rateDistortion</macroblockDecisionMode><minimumQuantiser>2</minimumQuantiser><maximumQuantiser>31</maximumQuantiser><quantiserDifference>3</quantiserDifference><trellis>false</trellis><quantiserCompression>0.5</quantiserCompression><quantiserBlur>0.5</quantiserBlur></Mpeg4aspOptions></Mpeg4aspConfig>";
		_encoderIndex = videoCodecPluginGetIndexByGuid("0E7C20E3-FF92-4bb2-A9A9-55B7F713C45A");

		std::string settings = out.str();
		COMPRES_PARAMS *param = &AllVideoCodec[_encoderIndex];
		ADM_vidEnc_plugin *plugin = getVideoEncoderPlugin(param->extra_param);
		int length = plugin->getOptions(plugin->encoderId, NULL, NULL, 0);
		vidEncOptions options;

		options.structSize = sizeof(vidEncOptions);
		options.encodeMode = ADM_VIDENC_MODE_CQP;
		options.encodeModeParameter = qz;

		_origSettings = new char[length + 1];
		plugin->getOptions(plugin->encoderId, &_origOptions, _origSettings, length);
		plugin->setOptions(plugin->encoderId, &options, settings.c_str());

		if (_encoderIndex > -1)
		{
			_encoder = new externalEncoder(param, 0);
			_encoder->configure(_incoming, false);
		}

		return (_encoderIndex != -1);
	}

	return 1;
}

void GenericAviSaveSmart::deleteEncoder(void)
{
	if (_encoder)
	{
		COMPRES_PARAMS *param = &AllVideoCodec[_encoderIndex];
		ADM_vidEnc_plugin *plugin = getVideoEncoderPlugin(param->extra_param);

		delete [] _origSettings;
		delete _encoder;
		_encoder = NULL;

		plugin->setOptions(plugin->encoderId, &_origOptions, _origSettings);
	}
}

uint8_t
GenericAviSaveSmart::stopEncoder (void)
{
	return 1;
}

