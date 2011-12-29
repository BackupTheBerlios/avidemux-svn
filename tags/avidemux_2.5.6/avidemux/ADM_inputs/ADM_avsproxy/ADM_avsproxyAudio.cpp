/***************************************************************************
    \file ADM_avsproxyAudio.cpp
    \author (C) 2007-2010 by mean  fixounet@free.fr

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
#include "DIA_coreToolkit.h"

#include "fourcc.h"
#include "ADM_avsproxy.h"
#include "ADM_avsproxy_internal.h"
#define AVS_AUDIO_BUFFER_SIZE (48000*6*4)


#define ADM_info printf
#define ADM_warning printf
#define ADM_error printf

/**
    \fn ADM_avsAccess
*/
avsAudio::avsAudio(avsNet *net, WAVHeader *wav,uint64_t duration)
{
    network=net;
    this->wavHeader=wav;
    this->duration=duration;
    nextSample=0;
    audioBuffer=new uint8_t[AVS_AUDIO_BUFFER_SIZE];
    _wavheader=wavHeader;
}
/**
    \fn ~ADM_avsAccess
*/

avsAudio::~avsAudio()
{
    if(audioBuffer)
        delete [] audioBuffer;
    audioBuffer=NULL;
}
/**
    \fn getDurationInUs
*/
uint64_t  avsAudio::getDurationInUs(void)
{
    return duration;
}
/**
    \fn goToTime
*/
bool      avsAudio::goToTime(uint64_t timeUs)
{
    // convert us to sample
    float f=timeUs;
    f*=wavHeader->frequency;
    f/=1000000.;
    nextSample=(uint32_t )f;
    return true;
}
/**
    \fn sampleToTime
*/
uint64_t avsAudio::sampleToTime(uint64_t sample)
{
    float f=sample;
    f/=wavHeader->frequency;
    f*=1000000;
    return (uint64_t)f;
}
/**
    \fn increment
*/
void avsAudio::increment(uint64_t sample)
{
    nextSample+=sample;
}
/**
    \fn getPacket
*/
bool      avsAudio::getPacket(uint8_t *buffer, uint32_t *size, uint32_t maxSize,uint64_t *dts)
{
    avsNetPacket out;
    avsNetPacket in;
    avsAudioFrame aFrame;
    aFrame.startSample=nextSample;
    
#warning this is incorrect
    aFrame.sizeInFloatSample=maxSize/(2*wavHeader->channels);
    in.buffer=(uint8_t *)&aFrame;
    in.size=sizeof(aFrame);

    out.buffer=audioBuffer;
    out.sizeMax=maxSize+sizeof(aFrame);
    out.size=0;
    //printf("Asking for frame %d\n",framenum);
    if(!network->command(AvsCmd_GetAudio,0,&in,&out))
    {
        ADM_error("Get audio failed for frame \n");
        return false;   
    }
  //  printf("Out size : %d\n",(int)out.size);
    
    //
    //
    memcpy(&aFrame,audioBuffer,sizeof(aFrame));
   // printf("NbSample : %d\n",(int)aFrame.sizeInFloatSample);
    if(!aFrame.sizeInFloatSample)
    {
        ADM_warning("Error in audio (Zero samples\n");
        return false;
    }
    *dts=sampleToTime(nextSample);
    increment(aFrame.sizeInFloatSample);
    *size=out.size-sizeof(aFrame);
    memcpy(buffer,audioBuffer+sizeof(aFrame),out.size-sizeof(aFrame));
  return true;
};

//**************** 2.5****************
uint32_t            avsAudio::read(uint32_t len,uint8_t *buffer)
{
    uint32_t size;
    uint64_t dts;
    if(true==getPacket(buffer,&size,len,&dts))
    {
        return size;
    }
    return 0;
}
uint8_t             avsAudio::goTo(uint32_t newoffset)
{
    uint32_t f=newoffset;
    f/=(2*wavHeader->channels); 
    nextSample=f;
    return 1;
}
uint8_t             avsAudio::getPacket(uint8_t *dest, uint32_t *len, uint32_t *samples)
{
    uint64_t dts;
    if(true==getPacket(dest, len, (wavHeader->channels*wavHeader->frequency*2)/10,&dts)) // 50 ms chunk
    {
        *samples=*len/(2*wavHeader->channels);
        return true;
    }
    return false;
}
uint8_t             avsAudio::goToTime(uint32_t mstime)
{
uint64_t t=mstime;
            t=t*1000;
            return goToTime(t);
}
uint8_t             avsAudio::extraData(uint32_t *l,uint8_t **d)
{
        *l=0;
        *d=NULL;
        return 1;
}
//EOF

