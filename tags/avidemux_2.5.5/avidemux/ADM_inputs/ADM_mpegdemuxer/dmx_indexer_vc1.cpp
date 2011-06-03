/***************************************************************************
                        2nd gen indexer                                                 
                             
    
    copyright            : (C) 2005 by mean
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

#include "ADM_default.h"

#include "DIA_fileSel.h"
#include "ADM_osSupport/ADM_quota.h"
#include "ADM_userInterfaces/ADM_commonUI/DIA_idx_pg.h"



#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_MPEG
#include "ADM_osSupport/ADM_debug.h"
#include "dmx_demuxerEs.h"
#include "dmx_demuxerPS.h"
#include "dmx_demuxerTS.h"
#include "dmx_demuxerMSDVR.h"
#include "dmx_identify.h"

#define MIN_DELTA_PTS 150 // autofix in ms
#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_MPEG
#include "ADM_osSupport/ADM_debug.h"

#include "dmx_indexer_internal.h"
#include "ADM_tsGetBits.h"
static const char vc1Type[5]={'X','I','P','B','P'};
static const uint32_t  VC1_ar[16][2] = {  // From VLC
                        { 0, 0}, { 1, 1}, {12,11}, {10,11}, {16,11}, {40,33},
                        {24,11}, {20,11}, {32,11}, {80,33}, {18,11}, {15,11},
                        {64,33}, {160,99},{ 0, 0}, { 0, 0}};

/**
    \class VC1Context
*/
class VC1Context
{
public:
        bool advanced;
        bool interlaced;
        bool interpolate;
        VC1Context() {reset();}
        bool reset(void){advanced=false;interlaced=false;interpolate=false;return true;}
};
static bool decodeVC1Pic(tsGetBits &bits,dmx_runData &video,VC1Context &vc1Context,uint32_t &frameType,uint32_t &frameStructure);
static bool decodeVC1Seq(tsGetBits &bits,dmx_runData &video,VC1Context &vc1Context);

dmx_videoIndexerVC1::dmx_videoIndexerVC1(dmx_runData *run) : dmx_videoIndexer(run)
{
  _frames=new IndFrame[MAX_PUSHED];
  firstPicPTS=ADM_NO_PTS;
  seq_found=0;
  grabbing=0;
  
}
dmx_videoIndexerVC1::~dmx_videoIndexerVC1()
{
          if(_frames) delete [] _frames;
          _frames=NULL;
}
/**
      \fn cleanup
      \brief do cleanup and purge non processed data at the end of the VC1 stream
*/
void dmx_videoIndexerVC1::cleanup(void)
{
  uint64_t lastAbs,lastRel;
          _run->demuxer->getPos(&lastAbs,&lastRel);
          if(_run->nbPushed)  gopDump(lastAbs,lastRel);
          dumpPts(firstPicPTS); 
          
}

/**
      \fn run 
      \brief main indexing loop for VC1 payload
*/
uint8_t   dmx_videoIndexerVC1::run  (void)
{
dmx_demuxer *demuxer=_run->demuxer;
uint64_t syncAbs,syncRel;
uint8_t streamid;   
uint32_t temporal_ref,ftype,val;
uint64_t pts,dts;
uint32_t imageAR;
VC1Context vc1Context;      

      while(1)
      {

              if(!demuxer->sync(&streamid,&syncAbs,&syncRel,&pts,&dts)) return 0;   
              if((_run->totalFileSize>>16)>50)
              {
                    _run->work->update(syncAbs>>16,_run->totalFileSize>>16,
                               _run->nbImage,_run->lastStamp.hh,_run->lastStamp.mm,_run->lastStamp.ss);
              }else
              {
                    _run->work->update(syncAbs,_run->totalFileSize,_run->nbImage,
                            _run->lastStamp.hh,_run->lastStamp.mm,_run->lastStamp.ss);
              }
              if(_run->work->isAborted()) return 0;
              switch(streamid)
                      {
                  case 0x0f: // sequence start
                          
                          if(seq_found)
                          {
                              if(grabbing)  
                                  continue;;
                              gopDump(syncAbs,syncRel);
                              grabbing=1;
                              break;
                          }
                          // Verify it is high/advanced profile
                          {
                          int seqSize=0;
                          tsGetBits bits(demuxer);
                          if(!bits.peekBits(1)) continue; // simple/main profile

                          if(!decodeVC1Seq(bits,*_run,vc1Context)) continue;
#if 0
                          seqSize=bits.getConsumed();
                          video.extraDataLength=seqSize+4+1;
                          memcpy(video.extraData+4,bits.data,seqSize);
                            // Add info so that ffmpeg is happy
                          video.extraData[0]=0;
                          video.extraData[1]=0;
                          video.extraData[2]=1;
                          video.extraData[3]=0xf;
                          video.extraData[seqSize+4+0]=0;
#endif
                          seq_found=1;
                          // Hi Profile
                          _frames[0].abs=syncAbs;
                          _frames[0].rel=syncRel;
                          grabbing=1;
                          continue;
                          }
                          break;
                    case 0x0D: //  Picture start
                        { 
                          int type;
                          uint8_t buffer[4];
                          uint32_t fType,sType;
                          if(!seq_found)
                          { 
                                  printf("[TsIndexer]No sequence start yet, skipping..\n");
                                  continue;
                          }      
                          tsGetBits bits(demuxer);

                          if(!decodeVC1Pic(bits,*_run,vc1Context,fType,sType)) continue;
                          grabbing=0;
                          Push(fType,syncAbs,syncRel);
                        }
                          break;
                  default:
                    break;
                  }
      }
      return 1;   
}
/**** Push a frame
There is a +- 2 correction when we switch gop
as in the frame header we read 2 bytes
Between frames, the error is self cancelling.

**/
/**
      \fn Push
      \brief Add a frame to the current gop

*/
uint8_t dmx_videoIndexerVC1::Push(uint32_t ftype,uint64_t abs,uint64_t rel)
{
       printf("%d ",ftype)  ;
        _frames[_run->nbPushed].type=ftype;
        
        if(_run->nbPushed)
        {                
                _frames[_run->nbPushed-1].size=_run->demuxer->elapsed();
                if(_run->nbPushed==1) _frames[_run->nbPushed-1].size-=2;
                _frames[_run->nbPushed].abs=abs;
                _frames[_run->nbPushed].rel=rel;        
                _run->demuxer->stamp();
        
        }
        //aprintf("\tpushed %d %"LLX"\n",nbPushed,abs);
        _run->nbPushed++;
        
        ADM_assert(_run->nbPushed<MAX_PUSHED);
        return 1;

}
/**
    \fn dumpPts
    \brief

*/
uint8_t dmx_videoIndexerVC1::dumpPts(uint64_t firstPts)
{
uint64_t stats[_run->nbTrack],p;
double d;
dmx_demuxer *demuxer=_run->demuxer;

        if(!demuxer->getAllPTS(stats)) return 0;
        qfprintf(_run->fd,"# Video 1st PTS : %07"LLU"\n",firstPts);
        if(firstPts==ADM_NO_PTS) return 1;
        for(int i=1;i<_run->nbTrack;i++)
        {
                p=stats[i];
                if(p==ADM_NO_PTS)
                {
                        qfprintf(_run->fd,"# track %d no pts\n",i);
                }
                else
                {
                        
                        d=firstPts; // it is in 90 khz tick
                        d-=stats[i];
                        d/=90.;
                        qfprintf(_run->fd,"# track %d PTS : %07"LLU" ",i,stats[i]);
                        qfprintf(_run->fd," delta=%04d ms\n",(int)d);
                }

        }
        return 1;
}
/**
      \fn       gopDump
      \brief    Dump the content of a gop into the index file
*/
uint8_t dmx_videoIndexerVC1::gopDump(uint64_t abs,uint64_t rel)
{
  dmx_demuxer *demuxer=_run->demuxer;
        if(!_run->nbPushed && !_run->nbImage) demuxer->stamp();
        if(!_run->nbPushed) return 1;

uint64_t stats[_run->nbTrack];

        _frames[_run->nbPushed-1].size=demuxer->elapsed()+2;
        qfprintf(_run->fd,"V %03u %06u %02u ",_run->nbGop,_run->nbImage,_run->nbPushed);

        // For each picture Type : abs position : relat position : size
        for(uint32_t i=0;i<_run->nbPushed;i++) 
        {

                qfprintf(_run->fd,"%c:%08"LLX",%05"LLX,
                        vc1Type[_frames[i].type],
                        _frames[i].abs,
                        _frames[i].rel);
                qfprintf(_run->fd,",%05x ",
                        _frames[i].size);
        }
        
        qfprintf(_run->fd,"\n");

        // audio if any
        //*******************************************
        // Nb image abs_pos audio seen
        // The Nb image is used to compute a drift
        //*******************************************
        if(demuxer->hasAudio() & _run->nbTrack>1)
        {
                demuxer->getStats(stats);
                
                qfprintf(_run->fd,"A %u %"LLX" ",_run->nbImage,abs);
                for(uint32_t i=1;i<_run->nbTrack;i++)
                {
                        qfprintf(_run->fd,":%"LLX" ",stats[i]);
                }
                qfprintf(_run->fd,"\n");
                
        }
        // Print some gop stamp infos, does not hurt
        qfprintf(_run->fd,"# Timestamp %02d:%02d:%02d,%03d\n",
                 _run->lastStamp.hh,_run->lastStamp.mm,_run->lastStamp.ss,_run->lastStamp.ff);
        _run->nbGop++;
        _run->nbImage+=_run->nbPushed;
        _run->nbPushed=0;
                
        _frames[0].abs=abs;
        _frames[0].rel=rel;
        demuxer->stamp();
        return 1;
        
}
/**
      \fn gopUpdate( 
      \brief Update the timecode hh:mm:ss.xx inside a gop header
*/
uint8_t dmx_videoIndexerVC1::gopUpdate(void)
{
        return 1;
}
/**
    \fn decodeVc1Seq
    \brief http://wiki.multimedia.cx/index.php?title=VC-1#Setup_Data_.2F_Sequence_Layer
#warning we should de-escape it!
        Large part of this borrowed from VLC
        Advanced/High profile only
*/
bool decodeVC1Seq(tsGetBits &bits,dmx_runData &video,VC1Context &vc1Context)
{

int v;
int consumed;
    vc1Context.advanced=true;

#define VX(a,b) v=bits.getBits(a);printf("[VC1] %d "#b"\n",v);consumed+=a;
    VX(2,profile);
    VX(3,level);

    VX(2,chroma_format);
    VX(3,Q_frame_rate_unused);
    VX(5,Q_bit_unused);

    VX(1,postproc_flag);

    VX(12,coded_width);
    video.imageW=v*2+2;
    VX(12,coded_height);
    video.imageH=v*2+2;

    VX(1,pulldown_flag);
    VX(1,interlaced_flag);
    vc1Context.interlaced=v;
    VX(1,frame_counter_flag);

    VX(1,interpolation_flag);
    vc1Context.interpolate=v;

    VX(1,reserved_bit);
    VX(1,psf);

    VX(1,display_extension);
    if(v)
    {
         VX(14,display_extension_coded_width);
         VX(14,display_extension_coded_height);
         VX(1,aspect_ratio_flag);
     

         if(v)
         {
                VX(4,aspect_ratio);
                switch(v)
                {
                    case 15: 
                             video.imageDarNum=bits.getBits(8);
                             video.imageDarDen=bits.getBits(8);
                             break;
                    default:
                             video.imageDarNum=VC1_ar[v][0];
                             video.imageDarDen=VC1_ar[v][1];
                             break;
                }
                printf("[VC1] Aspect ratio %d x %d\n",video.imageDarNum,video.imageDarDen);
         }
    
        VX(1,frame_rate);
        if(v)
        {
                VX(1,frame_rate32_flag);
                if(v)
                {
                    VX(16,frame_rate32);
                    float f=v;
                    f=(f+1)/32;
                    f*=1000;
                    video.imageFPS=(uint32_t)f;
                }else
                {
                    float den,num;
                    VX(8,frame_rate_num)
                    switch( v )
                        {
                        case 1: num = 24000; break;
                        case 2: num = 25000; break;
                        case 3: num = 30000; break;
                        case 4: num = 50000; break;
                        case 5: num = 60000; break;
                        case 6: num = 48000; break;
                        case 7: num = 72000; break;
                        }
                    VX(4,frame_rate_den)
                    switch( v )
                        {
                        default:
                        case 1: den = 1000; break;
                        case 2: den = 1001; break;
                        }

                    float f=num*1000;
                    f/=den;
                    video.imageFPS=(uint32_t)f;
                }
            }else
            {
                video.imageFPS=25000;
            }
            //
            VX(1,color_flag);
            if(v){
                    VX(8,color_prim);
                    VX(8,transfer_char);
                    VX(8,matrix_coef);
                }
    }
    VX(1,hrd_param_flag);
    int leaky=0;
    if(v)
    {
        VX(5,hrd_num_leaky_buckets);
        leaky=v;
        VX(4,bitrate_exponent);
        VX(4,buffer_size_exponent);
        for(int i = 0; i < leaky; i++) 
        {
                bits.getBits(16);
                bits.getBits(16);
        }
    }
    // Now we need an entry point
    bits.flush();
    uint8_t a[4];
    uint8_t entryPoint[4]={0,0,1,0x0E};
    for(int i=0;i<4;i++) a[i]=bits.getBits(8);
    for(int i=0;i<4;i++) printf("%02x ",a[i]);
    printf(" as marker\n");
    if(memcmp(a,entryPoint,4))
    {
        printf("Bad entry point");
        return false;
    }
    VX(6,ep_flags);
    VX(1,extended_mv);
    int extended_mv=v;
    VX(6,ep_flags2);

    for(int i=0;i<leaky;i++)
            bits.getBits(8);
    VX(1,ep_coded_dimension);
    if(v)
    {
         VX(12,ep_coded_width);
         VX(12,ep_coded_height);
    }
    if(extended_mv) VX(1,dmv);
    VX(1,range_mappy_flags);
    if(v) VX(3,mappy_flags);
    VX(1,range_mappuv_flags);
    if(v) VX(3,mappuv_flags);
    return true;

}
/**
    \fn decodeVC1Pic
    \brief Decode info from VC1 picture
    Borrowed a lot from VLC also

*/
bool decodeVC1Pic(tsGetBits &bits,dmx_runData &video,VC1Context &vc1Context,uint32_t &frameType,uint32_t &frameStructure)
{
    frameStructure=3;
    frameType=1;
    bool field=false;
    if(vc1Context.interlaced)
    {
        if(bits.getBits(1))
        {
            if(bits.getBits(1))
               field=true;
        }
    }
    if(field)
    {
            int fieldType=bits.getBits(3);
            frameStructure=1; // Top
            switch(fieldType)
            {
                case 0: /* II */
                case 1: /* IP */
                case 2: /* PI */
                    frameType=1;
                    break;
                case 3: /* PP */
                    frameType=2;
                    break;
                case 4: /* BB */
                case 5: /* BBi */
                case 6: /* BiB */
                case 7: /* BiBi */
                    frameType=3;
                    break;

            }
    }else
    {
                frameStructure=3; // frame
                if( !bits.getBits(1))
                    frameType=2;
                else if( !bits.getBits(1))
                    frameType=3;
                else if( !bits.getBits(1))
                    frameType=1;
                else if( !bits.getBits(1))
                    frameType=3;
                else
                    frameType=2;
    }

    return true;
}

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

//
