/***************************************************************************
                          oplug_vcdff.cpp  -  description
                             -------------------
    begin                : Sun Nov 10 2002
    copyright            : (C) 2002 by mean
    email                : fixounet@free.fr
    
    Ouput using FFmpeg mpeg1 encoder
    Much faster than mjpegtools, albeit quality seems inferior
    
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

#include "config.h"
#include "ADM_default.h"
#include "ADM_threads.h"

#ifdef USE_FFMPEG
extern "C" {
#include "ADM_lavcodec.h"
};
#include "avi_vars.h"
#include "prototype.h"

//#include "ADM_colorspace/colorspace.h"
#include "DIA_coreToolkit.h"
#include "ADM_videoFilter.h"
#include "ADM_videoFilter_internal.h"


#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_encoder/adm_encoder.h"

#include "ADM_codecs/ADM_ffmpeg.h"
#include "ADM_encoder/adm_encffmpeg.h"
#include "../oplug_mpegFF/oplug_vcdff.h"

#include "ADM_userInterfaces/ADM_commonUI/DIA_encoding.h"
#include "ADM_audiofilter/audioprocess.hxx"
#include "ADM_audiofilter/audioeng_buildfilters.h"
#include "../ADM_lavformat.h"

#include "ADM_encoder/adm_encConfig.h"
#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_libraries/ADM_mplex/ADM_mthread.h"
#include "ADM_toolkit/ADM_audioQueue.h"

#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_MP4
#include "ADM_osSupport/ADM_debug.h"

static uint8_t *_buffer=NULL,*_outbuffer=NULL;
static void  end (void);
extern const char *getStrFromAudioCodec( uint32_t codec);

static char *twoPass=NULL;
static char *twoFake=NULL;

extern AVDMGenericAudioStream *mpt_getAudioStream(void);
uint8_t prepareDualPass(uint32_t bufferSize,uint8_t *buffer,DIA_encoding *encoding_gui,Encoder *_encode,uint32_t total,int reuse);
uint8_t extractVolHeader(uint8_t *data,uint32_t dataSize,uint32_t *headerSize);
extern void    UI_purge(void );
/*


*/
uint8_t oplug_mp4(const char *name, ADM_OUT_FORMAT type)
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
lavMuxer  *muxer=NULL;
aviInfo      info;
uint32_t   width,height;
DIA_encoding *encoding_gui=NULL;
Encoder         *_encode=NULL;
char            *TwoPassLogFile=NULL;
uint32_t total=0;
uint32_t videoExtraDataSize=0;
uint8_t  *videoExtraData=NULL;
uint8_t *dummy,err;
WAVHeader *audioinfo=NULL;
ADMBitstream    bitstream(0);
ADM_MUXER_TYPE muxerType=MUXER_MP4;
uint8_t dualPass=0;
uint8_t r=0;
pthread_t     audioThread;
audioQueueMT context;
PacketQueue   *pq = NULL;//("MP4 audioQ",50,2*1024*1024);
uint32_t    totalAudioSize=0;
uint32_t sent=0;
const char *containerTitle;
int reuse = 0;
int frameDelay = 0;
bool receivedFrame = false;

           switch(type)
           {
             case ADM_PSP:muxerType=MUXER_PSP;containerTitle=QT_TR_NOOP("PSP");break;
             case ADM_MP4:muxerType=MUXER_MP4;containerTitle=QT_TR_NOOP("MP4");break;
             case ADM_MATROSKA:muxerType=MUXER_MATROSKA;containerTitle=QT_TR_NOOP("MKV");break;
             default:
                ADM_assert(0);
           }
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
           _encode = getVideoEncoder (_incoming->getInfo()->width,
                        _incoming->getInfo()->height,1);
           total= _incoming->getInfo()->nb_frames;

           encoding_gui=new DIA_encoding(_incoming->getInfo()->fps1000);
           bitstream.bufferSize=_incoming->getInfo()->width*_incoming->getInfo()->height*3;
           if (!_encode)
                {
                  GUI_Error_HIG (QT_TR_NOOP("Cannot initialize the video stream"), NULL);
                        goto  stopit;
                }

                // init compressor
               
                  encoding_gui->setContainer(containerTitle);
               
                encoding_gui->setAudioCodec(QT_TR_NOOP("None"));
                if(!videoProcessMode())
                        encoding_gui->setCodec(QT_TR_NOOP("Copy"));
                else
                        encoding_gui->setCodec(_encode->getDisplayName());
                TwoPassLogFile=new char[strlen(name)+6];
                strcpy(TwoPassLogFile,name);
                strcat(TwoPassLogFile,".stat");
                _encode->setLogFile(TwoPassLogFile,total);

				dualPass = _encode->isDualPass();

				if (dualPass)
				{
					FILE *tmp;

					if ((tmp = fopen(TwoPassLogFile,"rt")))
					{
						fclose(tmp);

						if (GUI_Question(QT_TR_NOOP("Reuse the existing log file?")))
							reuse = 1;
					}
				}

                if (!_encode->configure (_incoming, reuse))
                {
                      GUI_Error_HIG (QT_TR_NOOP("Filter init failed"), NULL);
                     goto  stopit;
                }

                if(dualPass)
                {
                       
                        if(!prepareDualPass(bitstream.bufferSize,videoBuffer,encoding_gui,_encode,total,reuse))
                                goto stopit;
                }else
                {
                        encoding_gui->setPhasis (QT_TR_NOOP("Encoding"));
                }
                
                info.width=_incoming->getInfo()->width;
                info.height=_incoming->getInfo()->height;
                info.nb_frames=_incoming->getInfo()->nb_frames;
                info.fps1000=_incoming->getInfo()->fps1000;
                info.fcc=*(uint32_t *)_encode->getCodecName(); //FIXME
                _encode->hasExtraHeaderData( &videoExtraDataSize,&dummy);
                if(videoExtraDataSize)
                {
                        printf("We have extradata for video in copy mode (%d)\n",videoExtraDataSize);
                        videoExtraData=new uint8_t[videoExtraDataSize];
                        memcpy(videoExtraData,dummy,videoExtraDataSize);
                }

				ADM_assert(_encode);
				bitstream.data = videoBuffer;

// ____________Setup audio__________________
          if(currentaudiostream)
          {
                audio=mpt_getAudioStream();
                if(!audio)
                {
                        GUI_Error_HIG (QT_TR_NOOP("Cannot initialize the audio stream"), NULL);
                        goto  stopit;
                }
          } 
          if(audio)
          {
                audioinfo=audio->getInfo();

				if (muxerType == MUXER_MP4)
				{
					switch (audioinfo->encoding)
					{
						case WAV_MP2:
						case WAV_MP3:
						case WAV_AAC:
						case WAV_AAC_HE:
							break;
						default:
							if (!GUI_YesNo(QT_TR_NOOP("Invalid audio stream detected"), QT_TR_NOOP("The audio stream may be invalid for this container.\n\nContinue anyway?")))
								goto stopit;
					}
				}

                audio->extraData(&extraDataSize,&extraData);
                if(audioProcessMode())
                        encoding_gui->setAudioCodec(getStrFromAudioCodec(audio->getInfo()->encoding));
                else
                         encoding_gui->setAudioCodec(QT_TR_NOOP("Copy"));

           }else
           {
                encoding_gui->setAudioCodec(QT_TR_NOOP("None"));
           }

//_____________ Loop _____________________
          
          encoding_gui->setContainer(containerTitle);
         
          if(!videoProcessMode())
                encoding_gui->setCodec(QT_TR_NOOP("Copy"));
          else
                encoding_gui->setCodec(_encode->getDisplayName());
           //
          UI_purge();

//_____________ Start Audio thread _____________________
          if(audio)
          {          
            pq=new PacketQueue("MP4 audioQ",5000,2*1024*1024);
            memset(&context,0,sizeof(context));
            context.audioEncoder=audio;
            context.audioTargetSample=0xFFFF0000; ; //FIXME
            context.packetQueue=pq;
            // start audio thread
            ADM_assert(!pthread_create(&audioThread,NULL,(THRINP)defaultAudioQueueSlave,&context)); 
            ADM_usleep(4000);
          }
//_____________GO !___________________
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

				  if (videoCodecGetType() == CodecExternal && strcmp(videoCodecPluginGetGuid(), "32BCB447-21C9-4210-AE9A-4FCE6C8588AE") == 0)
					  bitstream.dtsFrame = UINT32_MAX;	// let libavformat calculate it

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

			  if (!muxer)
			  {
				  // If needed get VOL header
				  if (isMpeg4Compatible(info.fcc) && !videoExtraDataSize && bitstream.len)
				  {
					  // And put them as extradata for esds atom
					  uint32_t voslen = 0;

					  if(extractVolHeader(videoBuffer, bitstream.len, &voslen))
					  {
						  if (voslen)
						  {
							  videoExtraDataSize = voslen;
							  videoExtraData = new uint8_t[videoExtraDataSize];
							  memcpy(videoExtraData, videoBuffer, videoExtraDataSize);
						  }
					  }
					  else 
						  printf("Oops should be settings data for esds\n");
				  }

				  muxer = new lavMuxer;

				  if (!muxer->open(
					  name,
					  2000000, // Muxrate
					  muxerType,
					  &info, videoExtraDataSize, videoExtraData,
					  audioinfo, extraDataSize, extraData))
					  break;
			  }

			  while (muxer->needAudio())
			  {
				  if (pq->Pop(audioBuffer, &alen, &sample))
				  {
					  if (alen)
					  {
						  muxer->writeAudioPacket(alen, audioBuffer, sample_got);
						  totalAudioSize += alen;
						  encoding_gui->setAudioSize(totalAudioSize);
						  sample_got += sample;
					  }
				  }
				  else
					  break;
			  }

			  muxer->writeVideoPacket(&bitstream);
			  encoding_gui->setFrame(frame, bitstream.len, bitstream.out_quantizer, total);
		  }
		  
stopit:
    printf("2nd pass, sent %u frames\n", bitstream.frameNumber + frameDelay);
    // Flush slave Q
    if(pq)
    {
        context.audioAbort=1;
        pq->Abort();
        // Wait for audio slave to be over
        while(!context.audioDone)
        {
          printf("Waiting Audio thread\n");
          ADM_usleep(500000); 
        }
        delete pq;
    }
    //
           if(muxer) muxer->close();
           if(encoding_gui) delete encoding_gui;
           if(TwoPassLogFile) delete [] TwoPassLogFile;
           if(videoBuffer) delete [] videoBuffer;
           if(muxer) delete muxer;
           if(_encode) delete _encode;	
           if(videoExtraData) delete [] videoExtraData;
           // Cleanup
           deleteAudioFilter (audio);
           return r;
}
uint8_t prepareDualPass(uint32_t bufferSize,uint8_t *buffer,DIA_encoding *encoding_gui,Encoder *_encode,uint32_t total,int reuse)
{
      uint32_t len, flag;
      uint8_t r;
      ADMBitstream bitstream(0);
	  int frameDelay = 0;
	  bool receivedFrame = false;
      
        aprintf("\n** Dual pass encoding**\n");

        if(!reuse)
        {
                encoding_gui->setPhasis (QT_TR_NOOP("1st Pass"));
                aprintf("**Pass 1:%lu\n",total);
                _encode->startPass1 ();
                bitstream.data=buffer;
                bitstream.bufferSize=bufferSize;

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

						if (videoCodecGetType() == CodecExternal && strcmp(videoCodecPluginGetGuid(), "32BCB447-21C9-4210-AE9A-4FCE6C8588AE") == 0)
							bitstream.dtsFrame = UINT32_MAX;	// let libavformat calculate it

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
						return 0;

					encoding_gui->setFrame(frame, bitstream.len, bitstream.out_quantizer, total);
				}

				encoding_gui->reset();
        }// End of reuse

        if(!_encode->startPass2 ())
        {
                printf("Pass2 ignition failed\n");
                return 0;
        }
        printf("First pass : send %u frames\n", bitstream.frameNumber + frameDelay);
        encoding_gui->setPhasis (QT_TR_NOOP("2nd Pass"));
        return 1;
}
void end (void)
{

}
uint8_t extractVolHeader(uint8_t *data,uint32_t dataSize,uint32_t *headerSize)
{
    // Search startcode
    uint8_t b;
    uint32_t idx=0;
    uint32_t mw,mh;
    uint32_t time_inc;
    
    *headerSize=0;

    while(dataSize)
    {
        uint32_t startcode=0xffffffff;
        while(dataSize>2)
        {
            startcode=(startcode<<8)+data[idx];
            idx++;
            dataSize--;
            if((startcode&0xffffff)==1) break;
        }
     
            printf("Startcodec:%x\n",data[idx]);
            if(data[idx]==0xB6 && idx>4) // vop start 
            {
                printf("Vop start found at %d\n",idx);
                *headerSize=idx-4;
                return 1;
            }
            
        }
        printf("No vop start found\n");
        return 0;
    
}	
#endif	
// EOF
