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
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <math.h>


#ifdef USE_FFMPEG
extern "C" {
#include "ADM_lavcodec.h"
};
#include "avi_vars.h"
#include "prototype.h"


#include "ADM_colorspace/colorspace.h"
#include "ADM_toolkit/toolkit.hxx"
#include <ADM_assert.h>
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_filter/video_filters.h"


#include "ADM_encoder/ADM_vidEncode.hxx"

#include "ADM_encoder/adm_encoder.h"
//#include "ADM_codecs/ADM_divxEncode.h"
//#include "ADM_encoder/adm_encdivx.h"

#include "ADM_codecs/ADM_ffmpeg.h"
#include "ADM_encoder/adm_encffmpeg.h"
#include "oplug_mpegFF/oplug_vcdff.h"

#include "ADM_dialog/DIA_encoding.h"
#include "ADM_audiofilter/audioprocess.hxx"
#include "ADM_audiofilter/audioeng_buildfilters.h"
#include "ADM_lavformat/ADM_lavformat.h"

#include "ADM_encoder/adm_encConfig.h"
#include "ADM_encoder/ADM_vidEncode.hxx"

#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_MP4
#include "ADM_toolkit/ADM_debug.h"

static uint8_t *_buffer=NULL,*_outbuffer=NULL;
static void  end (void);
extern const char *getStrFromAudioCodec( uint32_t codec);


extern SelectCodecType  current_codec;

static char *twoPass=NULL;
static char *twoFake=NULL;

extern AVDMGenericAudioStream *mpt_getAudioStream(void);
uint8_t prepareDualPass(uint8_t *buffer,char *TwoPassLogFile,DIA_encoding *encoding_gui,Encoder *_encode,uint32_t total);
uint8_t extractVolHeader(uint8_t *data,uint32_t dataSize,uint32_t *headerSize);

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

uint8_t   ret=0;

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
int prefill=0;
uint32_t displayFrame=0;
ADMBitstream    bitstream;
uint32_t        frameWrite=0;
ADM_MUXER_TYPE muxerType=MUXER_MP4;
uint8_t dualPass=0;
uint8_t r=0;
uint32_t skipping=1;
           if(type==ADM_PSP)
               muxerType=MUXER_PSP;
           else
               muxerType=MUXER_MP4;
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

           if (!_encode)
                {
                  GUI_Error_HIG (_("Cannot initialize the video stream"), NULL);
                        goto  stopit;
                }

                // init compressor
                encoding_gui->setContainer("MP4");
                encoding_gui->setAudioCodec("None");
                if(!videoProcessMode())
                        encoding_gui->setCodec("Copy");
                else
                        encoding_gui->setCodec(_encode->getDisplayName());
                TwoPassLogFile=new char[strlen(name)+6];
                strcpy(TwoPassLogFile,name);
                strcat(TwoPassLogFile,".stat");
                _encode->setLogFile(TwoPassLogFile,total);

                if (!_encode->configure (_incoming))
                {
                      GUI_Error_HIG (_("Filter init failed"), NULL);
                     goto  stopit;
                };

                dualPass=_encode->isDualPass();
                if(dualPass)
                {
                       
                        if(!prepareDualPass(videoBuffer,TwoPassLogFile,encoding_gui,_encode,total))
                                goto stopit;
                }else
                {
                        encoding_gui->setPhasis ("Encoding");
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
        // _________________Setup video (cont) _______________
        // ___________ Read 1st frame _________________
             
             ADM_assert(_encode);
             bitstream.data=videoBuffer;
             
preFilling:
             bitstream.cleanup(0);
             if(!(err=_encode->encode ( prefill, &bitstream)))//&len, videoBuffer, &flags,&displayFrame))
             {
                        printf("MP4:First frame error\n");
                        GUI_Error_HIG (_("Error while encoding"), NULL);
                        goto  stopit;
              }
              if(!bitstream.len)
              {
                prefill++;
                goto preFilling;
              }
              if(!bitstream.flags & AVI_KEY_FRAME)
              {
                GUI_Error_HIG (_("KeyFrame error"),_( "The beginning frame is not a key frame.\nPlease move the A marker."));
                  goto  stopit; 
              }
           //len=bitstream.len;
           // If needed get VOL header
           if(isMpeg4Compatible(info.fcc) && !videoExtraDataSize && bitstream.len)
           {
                // And put them as extradata for esds atom
                uint32_t voslen=0;
               
                if(extractVolHeader(videoBuffer,bitstream.len,&voslen))
                {
                        if(voslen)
                        {
                                videoExtraDataSize=voslen;
                                videoExtraData=new uint8_t[videoExtraDataSize];
                                memcpy(videoExtraData,videoBuffer,videoExtraDataSize);
                        }
                } else  printf("Oops should be settings data for esds\n");
            }

// ____________Setup audio__________________
          if(currentaudiostream)
          {
                audio=mpt_getAudioStream();
                if(!audio)
                {
                        GUI_Error_HIG (_("Cannot initialize the audio stream"), NULL);
                        goto  stopit;
                }
          } 
          if(audio)
          {
                audioinfo=audio->getInfo();
                audio->extraData(&extraDataSize,&extraData);
                if(audioProcessMode())
                        encoding_gui->setAudioCodec(getStrFromAudioCodec(audio->getInfo()->encoding));
                else
                         encoding_gui->setAudioCodec("Copy");

           }else
           {
                encoding_gui->setAudioCodec("None");
           }
// ____________Setup Muxer _____________________
           muxer= new lavMuxer;
           
           if(!muxer->open(
                name,
                2000000, // Muxrate
                MUXER_MP4,
                &info,videoExtraDataSize,videoExtraData,
                audioinfo,extraDataSize,extraData))
                         goto stopit;
           //_____________ Loop _____________________
          
          encoding_gui->setContainer("MP4");
          if(audio)
                encoding_gui->setAudioCodec(getStrFromAudioCodec(audio->getInfo()->encoding));
          if(!videoProcessMode())
                encoding_gui->setCodec("Copy");
          else
                encoding_gui->setCodec(_encode->getDisplayName());
           //
          if(bitstream.len)
          {
            muxer->writeVideoPacket( &bitstream);
            frameWrite++;
          }

           

           for(int frame=1;frame<total;frame++)
           {
                while(muxer->needAudio())
               {
                     if(!audio->getPacket(audioBuffer,&alen,&sample)) break;
                     if(alen)
                     {
                        muxer->writeAudioPacket(alen,audioBuffer,sample_got);
                        encoding_gui->feedAudioFrame(alen);
                        sample_got+=sample;
                     }
               }
               ADM_assert(_encode);
               bitstream.cleanup(frameWrite);
               if(!prefill || frame+prefill<total) 
               {
                  r=_encode->encode ( prefill+frame, &bitstream);
               }
                else
                {
                    r=_encode->encode ( total-1, &bitstream);
                }
               if(!r)
               {
                        printf("MP4:Frame %u error\n",frame);
                        GUI_Error_HIG (_("Error while encoding"), NULL);
                        goto  stopit;
                }
                if(!bitstream.len && skipping)
                {
                    printf("Frame skipped (xvid ?)\n");
                    continue;
                }
                skipping=0;
            //    printf("Prefill %u FrameWrite :%u Frame %u PtsFrame :%u\n",prefill,frameWrite,frame,bitstream.ptsFrame);
                frameWrite++;
                muxer->writeVideoPacket( &bitstream);

               encoding_gui->setFrame(frame,total);
               encoding_gui->feedFrame(bitstream.len);
               if(!encoding_gui->isAlive())
                {
                  if(GUI_YesNo(_("Stop Request"), _("Do you want to abort encoding ?")))
                                goto stopit;
                }
               
           }
           ret=1;
           
stopit:
           if(muxer) muxer->close();
           if(encoding_gui) delete encoding_gui;
           if(TwoPassLogFile) delete [] TwoPassLogFile;
           if(videoBuffer) delete [] videoBuffer;
           if(muxer) delete muxer;
           if(_encode) delete _encode;	
           if(videoExtraData) delete [] videoExtraData;
           // Cleanup
           deleteAudioFilter (audio);
           return ret;
}
uint8_t prepareDualPass(uint8_t *buffer,char *TwoPassLogFile,DIA_encoding *encoding_gui,Encoder *_encode,uint32_t total)
{
      uint32_t len, flag;
      FILE *tmp;
      uint8_t reuse=0;
      ADMBitstream bitstream;

        aprintf("\n** Dual pass encoding**\n");

        if((tmp=fopen(TwoPassLogFile,"rt")))
        {
                fclose(tmp);
                if(GUI_Question(_("\n Reuse the existing log-file ?")))
                {
                        reuse=1;
                }
        }
        
        if(!reuse)
        {
        
                encoding_gui->setPhasis ("1st Pass");
                aprintf("**Pass 1:%lu\n",total);
                _encode->startPass1 ();
                bitstream.data=buffer;
                for (uint32_t cf = 0; cf < total; cf++)
                {
                        encoding_gui->setFrame (cf, total);
                        if (!encoding_gui->isAlive())
                        {
                                abt:
                                    GUI_Error_HIG (_("Aborting"), NULL);
                                return 0;
                        }
                        bitstream.cleanup(cf);
                        if (!_encode->encode (cf,&bitstream))// &len, buffer, &flag))
                        {
                                printf("\n Encoding of frame %lu failed !\n",cf);
                                return 0;
                        }
                        encoding_gui->setQuant(bitstream.out_quantizer);
                        encoding_gui->feedFrame(bitstream.len);                
                }
                encoding_gui->reset();
                aprintf("**Pass 1:done\n");
        }// End of reuse

        if(!_encode->startPass2 ())
        {
                printf("Pass2 ignition failed\n");
                return 0;
        }
        encoding_gui->setPhasis ("2nd Pass");
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
