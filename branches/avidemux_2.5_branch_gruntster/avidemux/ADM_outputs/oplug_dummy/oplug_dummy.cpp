/***************************************************************************
                          oplug_dummy.cpp  -  Container that discards all inputs
                          						Video only!
                             -------------------
    
    copyright            : (C) 2007 by mean
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
#define __STDC_LIMIT_MACROS

#include <math.h>

#include "config.h"
#include "ADM_default.h"
#include "ADM_threads.h"

extern "C" {
#include "ADM_lavcodec.h"
};

#include "ADM_editor/ADM_Video.h"
#include "DIA_coreToolkit.h"
#include "ADM_videoFilter.h"
#include "ADM_videoFilter_internal.h"

#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_codecs/ADM_codec.h"
#include "ADM_encoder/adm_encoder.h"
#include "../oplug_mpegFF/oplug_vcdff.h"

#include "ADM_userInterfaces/ADM_commonUI/DIA_encoding.h"
#include "ADM_audiofilter/audioprocess.hxx"
#include "ADM_audiofilter/audioeng_buildfilters.h"
#include "../ADM_lavformat.h"
#include "fourcc.h"
#include "ADM_encoder/adm_encConfig.h"
#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_libraries/ADM_mplex/ADM_mthread.h"
#include "ADM_toolkit/ADM_audioQueue.h"

#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_MP4
#include "ADM_osSupport/ADM_debug.h"


extern void   	 				UI_purge(void );

extern uint32_t					videoProcessMode (void);
extern uint32_t 				frameStart,frameEnd;



static 				uint8_t *_buffer=NULL,*_outbuffer=NULL;

/*
 * 		\fn    Oplug_dummy
		\brief Main function to save in dummy format. 
		This containers drops all datas. Useful for testing or some weird filters

*/
uint8_t oplug_dummy(const char *name)
{
AVDMGenericVideoStream *_incoming=NULL;
AVDMGenericAudioStream  *audio=NULL;

uint8_t		audioBuffer[48000];
uint8_t         *videoBuffer=NULL;

uint32_t alen;//,flags;
uint32_t size;

uint32_t  sample_got=0,sample;
uint32_t  extraDataSize=0;
uint8_t   *extraData=NULL;
aviInfo      info;
uint32_t   width,height;
DIA_encoding *encoding_gui=NULL;
Encoder         *_encode=NULL;
uint32_t total=0;
uint32_t videoExtraDataSize=0;
uint8_t  *videoExtraData=NULL;
uint8_t *dummy,err;
int prefill=0;
uint32_t displayFrame=0;
ADMBitstream    bitstream(0);
uint32_t        frameWrite=0;
uint8_t r=0;
int frameDelay = 0;
bool receivedFrame = false;

uint32_t    totalAudioSize=0;

        // Setup video
        
        if(videoProcessMode())
        {
             _incoming = getLastVideoFilter (frameStart,frameEnd-frameStart);
        }else
        {
             _incoming = getFirstVideoFilter (frameStart,frameEnd-frameStart);
        }

           videoBuffer=new uint8_t[_incoming->getInfo()->width*_incoming->getInfo()->height*3];
                // Set global header encoding, needed for H264
           _encode = getVideoEncoder(1);
           total= _incoming->getInfo()->nb_frames;

           info.fcc=*(uint32_t *)_encode->getCodecName(); //FIXME
           
           encoding_gui=new DIA_encoding(_incoming->getInfo()->fps1000);
           bitstream.bufferSize=_incoming->getInfo()->width*_incoming->getInfo()->height*3;
           if (!_encode)
                {
                  GUI_Error_HIG ("[FLV]",QT_TR_NOOP("Cannot initialize the video stream"));
                        goto  stopit;
                }

                // init compressor
                encoding_gui->setContainer(QT_TR_NOOP("Dummy"));
                encoding_gui->setAudioCodec(QT_TR_NOOP("None"));
                if(!videoProcessMode())
                        encoding_gui->setCodec(QT_TR_NOOP("Copy"));
                else
                        encoding_gui->setCodec(_encode->getDisplayName());
                
                if (!_encode->configure (_incoming, 0))
                {
                      GUI_Error_HIG (QT_TR_NOOP("Filter init failed"), NULL);
                     goto  stopit;
                };

                encoding_gui->setPhasis (QT_TR_NOOP("Encoding"));
                
                
                info.width=_incoming->getInfo()->width;
                info.height=_incoming->getInfo()->height;
                info.nb_frames=_incoming->getInfo()->nb_frames;
                info.fps1000=_incoming->getInfo()->fps1000;
                
                _encode->hasExtraHeaderData( &videoExtraDataSize,&dummy);
                if(videoExtraDataSize)
                {
                        printf("We have extradata for video in copy mode (%d)\n",videoExtraDataSize);
                        videoExtraData=new uint8_t[videoExtraDataSize];
                        memcpy(videoExtraData,dummy,videoExtraDataSize);
                }
        // _________________Setup video (cont) _______________
        // ___________ Read 1st frame _________________
             
             ADM_assert(_encode);
             bitstream.data=videoBuffer;

			 if(!videoProcessMode())
				 encoding_gui->setCodec(QT_TR_NOOP("Copy"));
			 else
				 encoding_gui->setCodec(_encode->getDisplayName());

			 UI_purge();

			 for (uint32_t frame = 0; frame < total; frame++)
			 {
				 if (!encoding_gui->isAlive())
				 {
					 r = 0;
					 break;
				 }

				 for (;;)
				 {
					 bitstream.cleanup(frame);

					 if (frame + frameDelay >= total)
					 {
						 if (_encode->getRequirements() & ADM_ENC_REQ_NULL_FLUSH)
							 r = _encode->encode(UINT32_MAX, &bitstream);
						 else
							 r = 0;
					 }
					 else
						 r = _encode->encode(frame + frameDelay, &bitstream);

					 if (!r)
					 {
						 printf("Encoding of frame %lu failed!\n", frame);
						 GUI_Error_HIG (QT_TR_NOOP("Error while encoding"), NULL);
						 break;
					 }
					 else if (!receivedFrame && bitstream.len > 0)
					 {
						 if (!(bitstream.flags & AVI_KEY_FRAME))
						 {
							 GUI_Error_HIG (QT_TR_NOOP("KeyFrame error"), QT_TR_NOOP("The beginning frame is not a key frame.\nPlease move the A marker."));
							 r = 0;
							 break;
						 }
						 else
							 receivedFrame = true;
					 }

					 if (bitstream.len == 0 && (_encode->getRequirements() & ADM_ENC_REQ_NULL_FLUSH))
					 {
						 printf("skipping frame: %u size: %i\n", frame + frameDelay, bitstream.len);
						 frameDelay++;
					 }
					 else
						 break;
				 }

				 if (!r)
					 break;

				 encoding_gui->setFrame(frame, bitstream.len, bitstream.out_quantizer, total);
			 }

			 encoding_gui->reset();

stopit:
    
           if(encoding_gui) delete encoding_gui;
           if(videoBuffer) delete [] videoBuffer;
           if(_encode) delete _encode;	
           if(videoExtraData) delete [] videoExtraData;
           return r;
}

	
// EOF
