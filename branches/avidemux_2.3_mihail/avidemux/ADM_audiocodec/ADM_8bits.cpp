/***************************************************************************
                          ADM_8bits.cpp  -  description
                             -------------------
    begin                : Fri Aug 5 2003
    copyright            : (C) 2003 by mean
    email                : fixounet@free.fr

	This is a dummy coded that converts a 8 bits input sample to
	a 16 bits ouput samples

	It is used as it is because internally avidemux can only deal with
	16 bits audio samples

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
//#include <sstream>
#include <ADM_assert.h>
#include <math.h>

#include "config.h"
#include "fourcc.h"
#include "ADM_audio/aviaudio.hxx"
#include "ADM_audiocodec/ADM_audiocodec.h"


ADM_Audiocodec8Bits::ADM_Audiocodec8Bits( uint32_t fourcc) :   ADM_Audiocodec(fourcc)
{
 	ADM_assert(fourcc==WAV_8BITS||fourcc==WAV_8BITS_UNSIGNED);
	if(fourcc==WAV_8BITS_UNSIGNED)
			_unsign=1;
	else
			_unsign=0;

}
ADM_Audiocodec8Bits::~ADM_Audiocodec8Bits( )
{

}
uint8_t ADM_Audiocodec8Bits::run(uint8_t *inptr, uint32_t nbIn, float *outptr, uint32_t *nbOut, ADM_ChannelMatrix *matrix)
{
	for (int n = 0; n < nbIn; n++) {
		if(_unsign)
			*(outptr++) = (*inptr - 128) / 256;
		else
			*(outptr++) = *inptr / 256;
		inptr++;
	}
	*nbOut=nbIn;

	return 1;
}


