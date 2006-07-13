/***************************************************************************
                          audioeng_buff.cpp  -  description

  	That is a derivated class to handle generically buffered input/output
	
                             -------------------
    begin                : Thu Feb 7 2002
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
#include <string.h>
//#include <stream.h>
#include <ADM_assert.h>
#include <math.h>

#include "config.h"
#include "avifmt.h"
#include "avifmt2.h"
#include "avio.hxx"
#include "fourcc.h"
//#include "aviaudio.hxx"
#include "audioprocess.hxx"
//#include "../toolkit.hxx"
//
AVDMBufferedAudioStream::AVDMBufferedAudioStream(AVDMGenericAudioStream * instream):AVDMGenericAudioStream()
{
        _instream=instream;
        _in = NULL;
        _chunk = 4096;
        memset(_dither, 0, 256*sizeof(float));

    _headBuff=_tailBuff=0;
}

AVDMBufferedAudioStream::~AVDMBufferedAudioStream()
{
	if (_in)
		delete _in;
}

//
//
uint32_t AVDMBufferedAudioStream::read(uint32_t len, uint8_t * buffer)//deprecate
{
	uint32_t avail = 0;
	uint32_t got=0;
	uint32_t grabbed;
	uint8_t  *myBuffer;
	myBuffer=(uint8_t *)internalBuffer;

more:
	// have we already got what's needed ?
	avail=_tailBuff-_headBuff;
	//    printf("\n asked : %lu, available : %lu",len,avail);
	// got everything ?
	if (avail >= len) {
		memcpy(buffer, myBuffer+_headBuff, len);
		_headBuff+=len;
		got+=len;
		//printf("Got : %lu\n",got);
		return got;
	}
	// first empty what was available
	memcpy(buffer, myBuffer+_headBuff, avail);

	got+=avail;
	len -= avail;
	buffer+=avail;
	_headBuff+=avail;

	// rewind to beginning
	_tailBuff=_headBuff=0;

	//printf("\n grabing...");
	grabbed = grab(myBuffer);
	//  printf("grabbed :%lu\n",grabbed);
	// Minus one means we could not get a single byte
	if (grabbed == MINUS_ONE) {
		printf("\n ** end stream **\n");
		_tailBuff=_headBuff=0;	
		return got;		// we got everything  !
	}
	_tailBuff+=grabbed;
	goto more;
};

uint32_t AVDMBufferedAudioStream::read(uint32_t len, float *buffer)
{
    uint32_t avail = 0;
    uint32_t got = 0;
    uint32_t grabbed;
    // this disable the filter
    // return _instream->read(len,buffer);

more:


    // have we already got what's needed ?
    avail=_tailBuff-_headBuff;
 //    printf("\n asked : %lu, available : %lu",len,avail);  
    // got everything ?
    if (avail >= len)
      {	
	  memcpy(buffer, internalBuffer_float + _headBuff, sizeof(float) * len);
	  _headBuff+=len;
	  got+=len;
//	  printf("Got : %lu\n",got);
	  return got;
      }
    // first empty what was available
    memcpy(buffer, internalBuffer_float + _headBuff, sizeof(float) * avail);

    got+=avail;
    len -= avail;
    buffer+=avail;
    _headBuff+=avail;


    // rewind to beginning
    _tailBuff=_headBuff=0;

    //printf("\n grabing...");
    grabbed = grab(internalBuffer_float);
  //  printf("grabbed :%lu\n",grabbed);
    // Minus one means we could not get a single byte
    if (grabbed == MINUS_ONE)
      {
	  printf("\n ** end stream **\n");
	  _tailBuff=_headBuff=0;	
	  return got;		// we got everything  !
      }
    _tailBuff+=grabbed;
    goto more;
};

/*do triangle dither and convert to int16_t with minimal overhead*/
void AVDMBufferedAudioStream::dither16bit()
{
	float dp;
	int16_t *data_int = (int16_t *)_in;
	float *data = _in;
	uint16_t len = _chunk / _wavheader->channels;

	for (int i = 0; i < len; i++)
		for (int c = 0; c < _wavheader->channels; c++) {
			if (*data > 1) *data = 1;
			if (*data < -1) *data = -1;

			dp = _dither[c];
			_dither[c] = rand() / (float)RAND_MAX * 2.0 - 1.0;
			*data_int = (int16_t)(*data * 32765 + _dither[c] - dp);//32767 - 2 to avoid clipping
			data++;
			data_int++;
		}
}


uint8_t AVDMBufferedAudioStream::goTo(uint32_t newoffset) {
        ADM_assert(!newoffset);
        goToTime(0);
        return 1;
}

uint8_t AVDMBufferedAudioStream::goToTime(uint32_t newoffset) {
        ADM_assert(!newoffset);
        _instream->goToTime(0);
        _headBuff=_tailBuff=0;
        return 1;
}

// EOF
