/***************************************************************************
                          ADM_devicePulseSimple.cpp  -  description

  Simple Pulse audio out
                          
    copyright            : (C) 2008 by mean
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
#if defined( USE_PULSE_SIMPLE ) && !defined(ADM_WIN32)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/ioctl.h>

 

#include "default.h"
#include "ADM_audiodevice.h"
#include <ADM_assert.h>
#include "ADM_audiodevice/ADM_deviceoss.h"
#include "ADM_toolkit/toolkit.hxx"
#include  "ADM_audiodevice/ADM_devicePulseSimple.h"

#include "pulse/simple.h"
//#define ADM_PULSE_INT16
/**
    \fn pulseSimpleAudioDevice
    \brief Constructor

*/
pulseSimpleAudioDevice::pulseSimpleAudioDevice()
{
    instance=NULL;
}

#define INSTANCE  ((pa_simple *)instance)
/**
    \fn stop
    \brief stop & release device

*/

uint8_t  pulseSimpleAudioDevice::stop(void) 
{
int er;
    ADM_assert(instance);
    pa_simple_flush(INSTANCE,&er);
    pa_simple_free(INSTANCE);
    instance=NULL;
    printf("[PulseAudio] Stopped\n");
    return 1;
}

/**
    \fn init
    \brief Take & initialize the device

*/
uint8_t pulseSimpleAudioDevice::init(uint8_t channels, uint32_t fq) 
{

pa_simple *s;
pa_sample_spec ss;
int er;
 
#ifdef ADM_PULSE_INT16
  ss.format = PA_SAMPLE_S16NE;
#else
    ss.format = PA_SAMPLE_FLOAT32NE;//PA_SAMPLE_S16NE; //FIXME big endian
#endif
  ss.channels = channels;
  ss.rate =fq;
 
  instance= pa_simple_new(NULL,               // Use the default server.
                    "Avidemux2",           // Our application's name.
                    PA_STREAM_PLAYBACK,
                    NULL,               // Use the default device.
                    "Sound",            // Description of our stream.
                    &ss,                // Our sample format.
                    NULL,               // Use default channel map
                    NULL ,             // Use default buffering attributes.
                    &er               // Ignore error code.
                    );
  if(!instance)
    {
        printf("[PulseSimple] open failed\n");
        return 0;
    }
    printf("[PulseSimple] open ok\n");
    return 1;

}

/**
    \fn play
    \brief Playback samples

*/
uint8_t pulseSimpleAudioDevice::play(uint32_t len, float *data)
{
int er;
#ifdef ADM_PULSE_INT16
	dither16(data, len, _channels);
    pa_simple_write(INSTANCE,data,len*2,&er);
#else
    pa_simple_write(INSTANCE,data,len*4,&er);
#endif
	return 1;
}
/**
    \fn setVolume
    \brief Cannot be done with pulse simple
*/
uint8_t pulseSimpleAudioDevice::setVolume(int volume)
{
	return 1;
}

#else
void dummy_pulseSimple_func( void);
void dummy_pulseSimple_func( void)
 {
}

#endif
