/***************************************************************************
    copyright            : (C) 2006 by mean
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

#include <string.h>

#include "math.h"

#include "ADM_library/default.h"
#include "ADM_editor/ADM_Video.h"
#include <ADM_assert.h>

#include "ADM_library/fourcc.h"
#include "ADM_toolkit/toolkit.hxx"

#include "ADM_asf.h"


/*
    __________________________________________________________
*/

asfAudio::~asfAudio()
{
  printf("[asfAudio] Destroying track\n");
}
/*
    __________________________________________________________
*/
                               
asfAudio::  asfAudio(asfHeader *father)
{
  printf("[asfAudio] Creating track\n");
    _father=father;
    _wavheader=_father->_wavHeader;
   _extraDataLen=0;
   _extraData=NULL;
   _length=0xffffff;
   _streamId=_father->_audioStreamId;
   _dataStart=_father->_dataStartOffset;
   _fd=fopen(_father->myName,"rb");
   ADM_assert(_fd);
   fseeko(_fd,_dataStart,SEEK_SET);
   _packetSize=_father->_packetSize;
   _packet=new asfPacket(_fd,_packetSize,&readQueue);
  
}
/*
    __________________________________________________________
*/

uint32_t            asfAudio::read(uint32_t len,uint8_t *buffer)
{
  uint32_t lan,samples;
  if(!getPacket(      buffer,    &lan, &samples)) return 0;
  return lan;
}
/*
    __________________________________________________________
*/

uint8_t   asfAudio::goTo(uint32_t newoffset)
{
  return 1; 
}
/*
    __________________________________________________________
*/

uint8_t   asfAudio::goToTime(uint32_t newoffset)
{
  return 1; 
}
/*
    __________________________________________________________
*/

uint8_t   asfAudio::extraData(uint32_t *l,uint8_t **d)
{
  if(_father->_audioExtraData)
  {
    *l=_father->_audioExtraDataLen;
    *d=_father->_audioExtraData;  
  }
  else
  {
    *l=0;
    *d=NULL; 
  }
  return 1;
}
/*
    __________________________________________________________
*/

uint8_t  asfAudio::getPacket(uint8_t *dest, uint32_t *len, uint32_t *samples)
{
  *len=0;
  uint32_t delta;
  while(1)
  {
   
    while(!readQueue.isEmpty())
    {
      asfBit *bit;
      ADM_assert(readQueue.pop((void**)&bit));
      printf("Audio found packet of size %d seq %d\n",bit->len,bit->sequence);
      
      // still same sequence ...add
      memcpy(dest,bit->data,bit->len);
      *len=bit->len;
#warning FIXME      
      *samples=384; // ?
      delete bit;
      return 1;
    }
    if(!_packet->nextPacket(_streamId))
    {
      printf("[ASF] Audio Packet Error\n");
      return 0; 
    }
    _packet->skipPacket();
  }
  
  return 0; 
}
//EOF
