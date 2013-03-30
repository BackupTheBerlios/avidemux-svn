/***************************************************************************
            \file            muxerffPS
            \brief           i/f to lavformat PS muxer
                             -------------------
    
    copyright            : (C) 2009 by mean
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

#include "ADM_default.h"
#include "fourcc.h"
#include "muxerffPS.h"
#include "DIA_coreToolkit.h"
#include "ADM_muxerUtils.h"
#include "ADM_codecType.h"
#if 0
#define aprintf(...) {}
#else
#define aprintf printf
#endif

ps_muxer psMuxerConfig=   // Default is standard DVD
{
    MUXER_DVD,false,
    11000,9800,224
};

/**
    \fn     muxerffPS
    \brief  Constructor
*/
muxerffPS::muxerffPS() 
{
};
/**
    \fn     muxerffPS
    \brief  Destructor
*/

muxerffPS::~muxerffPS() 
{
   
}
/**
    \fn open
    \brief Check that the streams are ok, initialize context...
*/
bool muxerffPS::open(const char *file, ADM_videoStream *s,uint32_t nbAudioTrack,ADM_audioStream **a)
{
const char *er;

    if(verifyCompatibility(psMuxerConfig.acceptNonCompliant,psMuxerConfig.muxingType,s,nbAudioTrack,a,&er)==false)
    {
        GUI_Error_HIG("[Mismatch]","%s",er);
        return false;
    }

    const char *fmt;
    switch(psMuxerConfig.muxingType)
    {
        default:
        case MUXER_DVD: fmt="dvd";break;
        case MUXER_VCD: fmt="vcd";break;
        case MUXER_SVCD: fmt="svcd";break;
    }
    if(false==setupMuxer(fmt,file))
    {
        printf("[ffPS] Failed to open muxer\n");
        return false;
    }
 
   if(initVideo(s)==false) 
    {
        printf("[ffPS] Failed to init video\n");
        return false;
    }
  
    
        AVCodecContext *c;
        c = video_st->codec;

        // Override codec settings
        rescaleFps(s->getAvgFps1000(),&(c->time_base));
        myTimeBase=video_st->time_base=c->time_base;
        myTimeBase.num=1;
        c->bit_rate=psMuxerConfig.videoRatekBits*1000;
        c->rc_buffer_size=psMuxerConfig.bufferSizekBytes*8*1024;
        c->rc_buffer_size_header=psMuxerConfig.bufferSizekBytes*8*1024;
        c->gop_size=15;

        // Audio
        if(initAudio(nbAudioTrack,a)==false)
        {
            printf("[ffPS] Failed to init audio\n");
            return false;
        }
        for(int i=0;i<nbAudioTrack;i++)
        {
            audio_st[i]->codec->bit_rate=a[i]->getInfo()->byterate*8;        
        }

        int erx = avio_open(&(oc->pb), file, AVIO_FLAG_WRITE);

        if (erx)
        {
            ADM_error("[PS]: Failed to open file :%s, er=%d\n",file,erx);
            return false;
        }

        AVDictionary *dict = NULL;
		char buf[64];
        
        snprintf(buf, sizeof(buf), "%d", psMuxerConfig.muxRatekBits * 1000);
		av_dict_set(&dict, "muxrate", buf, 0);
        av_dict_set(&dict, "preload", "0", 0);
        av_dict_set(&dict, "max_delay", "2000", 0);

        ADM_assert(avformat_write_header(oc, &dict) >= 0);
        vStream=s;
        aStreams=a;
        nbAStreams=nbAudioTrack;
        initialized=true;
        return true;
}

/**
    \fn save
*/
bool muxerffPS::save(void) 
{
    const char *title=QT_TR_NOOP("Saving mpeg PS (ff)");
    return saveLoop(title);
}
// Clock is 90 Khz for all mpeg streams
// Since the unit is in us=10e6,
// time=time/10E6*90E3
// time=(time*9)/100
bool muxerffPS::muxerRescaleVideoTimeDts(uint64_t *time,uint64_t computedDts)
{
    if(*time==ADM_NO_PTS)
    {
        *time=computedDts;
    }
    return muxerRescaleVideoTime(time);
}
  
/**
    \fn close
    \brief Cleanup is done in the dtor
*/
bool muxerffPS::close(void) 
{
   
    printf("[ffPS] Closing\n");
    return closeMuxer();
}

/**
    \fn verifyCompatibility
    \return true if the streams are ok to be muxed by selected muxer

*/
#define FAIL(x) {*er=x;return false;}
bool muxerffPS::verifyCompatibility(bool nonCompliantOk, uint32_t muxingType,
                                    ADM_videoStream *s,uint32_t nbAudioTrack,ADM_audioStream **a, 
                                    const char **er)
{
    uint32_t fcc=s->getFCC();
    uint32_t w,h;
     w=s->getWidth();
     h=s->getHeight();
     *er="??";

     if(!isMpeg12Compatible(fcc))
     {
            FAIL(" video not compatible\n");
     }
    if(!nonCompliantOk)
    {
        switch(muxingType)
        {
            case MUXER_VCD:
                    if(w!=352 || (h!=240 && h!=288))
                    {
                            FAIL(" Bad width/height for VCD\n");
                    }
                    break;
            case MUXER_SVCD:
                    if((w!=352 && w!=480)|| (h!=576 && h!=480))
                    {
                            FAIL(" Bad width/height for SVCD\n");
                    }
                    break;
            case MUXER_DVD:
                    if((w!=720 && w!=704)|| (h!=576 && h!=480))
                    {
                            FAIL(" Bad width/height for DVD\n");
                    }
                    break;
            case MUXER_FREE: break;
            default:
                    ADM_assert(0);
        }
    }
    if(!nbAudioTrack) 
        {
            FAIL(" One audio track needed\n");
            
        }
    for(int i=0;i<nbAudioTrack;i++)
    {
        WAVHeader *head=a[i]->getInfo();
        switch(muxingType)
        {
            case MUXER_VCD:
            case MUXER_SVCD:
                    if(head->encoding!=WAV_MP2) 
                    {
                        FAIL(" VCD : only MP2 audio accepted\n");
                    }
                    if(!nonCompliantOk)
                        if(head->frequency!=44100) 
                        {
                            FAIL(" VCD : only 44.1 khz audio accepted\n");
                        }
                    break;
            case MUXER_DVD:
                    if(!nonCompliantOk)
                        if(head->frequency!=48000) 
                        {
                            FAIL(" DVD : only 48 khz audio accepted\n");
                        }
                    // no break
            case MUXER_FREE:
                    if(head->encoding!=WAV_MP2 && head->encoding!=WAV_AC3 && head->encoding!=WAV_DTS) 
                    {
                        FAIL("[ffPS] DVD : only MP2/AC3/DTS audio accepted\n");
                    }
                    break;
            default:
                    ADM_assert(0);
        }
    }
    return true;
}
//EOF



