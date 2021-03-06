/***************************************************************************
                        audioouput.h  -  description
                             -------------------
    begin                : Thu Dec 27 2001
    copyright            : (C) 2001 by mean
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

#ifndef AUDIOOUPUT_H
#define AUDIOOUPUT_H
//#include "avi_vars.h"
#ifdef HAVE_AUDIO
typedef enum
{
	DEVICE_OSS=1,
	DEVICE_DUMMY,
	DEVICE_ARTS,
	DEVICE_ALSA,
	DEVICE_COREAUDIO,
	DEVICE_SDL,
	DEVICE_WIN32,
	DEVICE_ESD,
	DEVICE_JACK
}AUDIO_DEVICE;

typedef struct 
{
	AUDIO_DEVICE id;
	const char	     *name;
}DEVICELIST;
#define MKADID(x) {DEVICE_##x,#x}
static const DEVICELIST audioDeviceList[]=
{
	
#ifdef OSS_SUPPORT	
	MKADID(OSS),
#endif	
#ifdef USE_ARTS
	MKADID(ARTS),
#endif	
#ifdef ALSA_SUPPORT
	MKADID(ALSA),
#endif	
#ifdef __APPLE__
	MKADID(COREAUDIO),
#endif
#if	defined(USE_SDL) && !defined(__WIN32)	
	MKADID(SDL),
#endif	
#ifdef __WIN32	
	MKADID(WIN32),
#endif	
#ifdef USE_ESD	
	MKADID(ESD),
#endif	
#ifdef USE_JACK	
	MKADID(JACK),
#endif	

	MKADID(DUMMY)
};


void 		AVDM_audioSave( void );
void 		AVDM_audioInit( void );
void		AVDM_cleanup(void);
void 		AVDM_switch( AUDIO_DEVICE action );

uint8_t 	AVDM_AudioPlay(float *ptr, uint32_t nb);
uint32_t 	AVDM_AudioSetup(uint32_t fq, uint8_t channel);
void 		AVDM_AudioClose(void);
AUDIO_DEVICE 	AVDM_getCurrentDevice( void);
uint8_t         AVDM_setVolume(int volume);

#endif
#endif
