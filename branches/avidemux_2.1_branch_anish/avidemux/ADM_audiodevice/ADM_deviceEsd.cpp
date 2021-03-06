/***************************************************************************
                          ADM_deviceEsd.cpp  -  description

  ESD support as output audio device
                          
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

#if defined( USE_ESD ) && !defined(CYG_MANGLING)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <esd.h>
 

#include "ADM_library/default.h"
#include "audio_out.h"
#include <ADM_assert.h>
#include  "ADM_audiodevice/ADM_deviceoss.h"
#include  "ADM_audiodevice/ADM_deviceEsd.h"
#include "ADM_toolkit/toolkit.hxx"






//_______________________________________________
//
//
//_______________________________________________
uint8_t  esdAudioDevice::stop(void) {
    if (esdDevice > 0) {
        esd_close(esdDevice);
        esdDevice = 0;
    }
    return 1;
}

//_______________________________________________
//
//
//_______________________________________________
uint8_t esdAudioDevice::init(uint32_t channel, uint32_t fq) 
{
esd_format_t format;

    format=ESD_STREAM | ESD_PLAY | ESD_BITS16;
    if(channel==1) format|=ESD_MONO;
        else format|=ESD_STEREO;

    printf("\n ESD  : %lu Hz, %lu channels", fq, channel);
    esdDevice=esd_play_stream(format,fq,NULL,"avidemux");
    if(esdDevice<=0) 
    {
        printf("Esd open failed\n");
        return 0;
    }
    printf("Esd open succeedeed\n");
    /*
#ifdef ADM_BIG_ENDIAN    
    int fmt = AFMT_S16_BE;
#else
    int fmt = AFMT_S16_LE;
#endif    
*/
    return 1;
}

//_______________________________________________
//
//
//_______________________________________________
uint8_t esdAudioDevice::play(uint32_t nb,uint8_t * ptr)
 {
    write(esdDevice,ptr, nb);
    return 1;
}

uint8_t esdAudioDevice::setVolume(int volume){
#ifdef OSS_SUPPORT
	ossAudioDevice dev;
	dev.setVolume(volume);
#else
#ifdef ALSA_SUPPORT
	alsaAudioDevice dev;
	dev.setVolume(volume);
#endif
#endif
	return 1;
}

#else
void dummy_ess_func( void);
void dummy_ess_func( void)
 {
}

#endif
