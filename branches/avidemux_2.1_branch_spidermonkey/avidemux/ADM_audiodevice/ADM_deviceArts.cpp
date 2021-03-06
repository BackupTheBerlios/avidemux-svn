/***************************************************************************
                          ADM_deviceArts.cpp  -  description
                             -------------------
    begin                : Sat Sep 28 2002
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
#include "config.h"
 
#ifdef USE_ARTS
#include <artsc.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <ADM_assert.h>  
#include "ADM_library/default.h"
#include "audio_out.h"
#include  "ADM_audiodevice/ADM_deviceoss.h"
#include  "ADM_audiodevice/ADM_deviceArts.h"
uint8_t artsInitialized=0;
//_______________________________________________
//
//
//_______________________________________________
uint8_t  artsAudioDevice::stop(void) {
		if(!_stream)  
		{
			printf("\n Arts: no stream\n");
			return 0;
		}
		
		arts_close_stream(_stream);
		// apparently arts 3.2 alpha does not like this
	   	//arts_free();
		_stream=NULL;
		printf("\n Arts stopped\n");
    	return 1;
}

//_______________________________________________
//
//
//_______________________________________________
uint8_t artsAudioDevice::init(uint32_t channel, uint32_t fq) 
{
    if(_stream)
    	{
			printf("\n purging previous instance\n");
			stop();
		}
    printf("\n Arts  : %lu Hz, %lu channels", fq, channel);
	if(!artsInitialized)
	{
		if(arts_init()) 
		{
			printf("\n Error initializing artsd\n");
			return 0;
		}
		artsInitialized=1;
	}

	_stream=arts_play_stream(fq, 16,channel, "Avidemux");

	if(!_stream)
	 {
		printf("\n Problem setting fq/channel, aborting\n");
		arts_free();
		return 0;
	}
	arts_stream_set(_stream, ARTS_P_BLOCKING, 1);
	arts_stream_set(_stream, ARTS_P_BUFFER_TIME, 50); // Ask for 1 sec buffer
	//arts_stream_set(_stream,  ARTS_P_PACKET_SETTINGS, (11<<16)+10);
	
	
    return 1;
}

//_______________________________________________
//
//
//_______________________________________________
uint8_t artsAudioDevice::play(uint32_t nb,uint8_t * ptr)
 {
		if(!_stream) return 0;
			return arts_write(_stream, ptr, nb);

}
#else
void dummy_art_func( void);
void dummy_art_func( void)
 {
}
#endif
