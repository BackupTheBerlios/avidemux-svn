/***************************************************************************
                          audiodeng_buildfilters.cpp  -  description
                             -------------------
    begin                : Mon Dec 2 2002
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
#include <ADM_assert.h>
#include <math.h>

#include "config.h"
#include "avifmt.h"
#include "avifmt2.h"
#include "avio.hxx"
#include "fourcc.h"
#include "audioprocess.hxx"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_audiofilter/audioeng_buildfilters.h"
#include "ADM_audiofilter/audioeng_film2pal.h"

//#include "ADM_gui/GUI_mp3.h"
#include "ADM_audiofilter/audioeng_ffmp2.h"
#include "ADM_audiofilter/audioeng_libtoolame.h"
#ifdef USE_FAAC
	#include "ADM_audiofilter/audioeng_faac.h"
#endif
#ifdef HAVE_LIBMP3LAME
	#include "lame/lame.h"
#endif
#ifdef USE_VORBIS
	#include "audioeng_vorbis.h"
#endif

#include "gui_action.hxx"
/*
  	Ugly should get ride of it. Temporary fix.
*/
#include "avi_vars.h"
#include "audioeng_toolame.h"
#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_AUDIO_FILTER
#include "ADM_toolkit/ADM_debug.h"
extern void UI_setAProcessToggleStatus( uint8_t status );
extern uint8_t DIA_audioCodec( AUDIOENCODER *codec );
extern int DIA_audioEncoder(int *pmode, int *pbitrate,const char *title);
extern void UI_setAudioCodec( int i);
uint32_t audioFilterGetNbEncoder(void);
const char* audioFilterGetIndexedName(uint32_t i);

typedef struct {
	const char *name;
	const char *menuName;
	AUDIOENCODER codec;
}CODECLIST;

static CODECLIST myCodecList[]=
{
#ifdef HAVE_LIBMP3LAME
		{"lame","Lame", AUDIOENC_MP3},
#endif
#ifdef USE_FAAC
		{"aac","FAAC", AUDIOENC_FAAC},
#endif
#ifdef USE_VORBIS
		{"vorbis","Vorbis", AUDIOENC_VORBIS},
#endif

		{"mp2", "FFm MP2", AUDIOENC_MP2},
		{"ac3", "FFm AC3",AUDIOENC_AC3},
		{"toolame","Toolame", AUDIOENC_2LAME},
		{"none", "Wav PCM",AUDIOENC_NONE}
};
extern int DIA_getLameSettings(int *pmode, int *pbitrate,ADM_LAME_PRESET *preset);
extern void UI_PrintCurrentACodec( const char *s);

#define MAX_AUDIO_FILTER 10
static AVDMProcessAudioStream *filters[MAX_AUDIO_FILTER];
static uint32_t filtercount = 0;

 int  audioNormalizeMode = 0;
 int  audioFreq=48000;
 RESAMPLING  audioResampleMode = RESAMPLING_NONE;
static int  audioDRC = 0;


// These are globals for the moment
int 	   audioShift = 0;
int	   audioDelay=0;

FILMCONV audioFilmConv=FILMCONV_NONE;


// MP3 parameters
 int audioMP3mode = 0, audioMP3bitrate = 128;
 ADM_LAME_PRESET audioMP3preset=ADM_LAME_PRESET_CBR;
//
static AUDIOENCODER  activeAudioEncoder=  AUDIOENC_NONE;

void audioCodecChanged(int newcodec)
{
	ADM_assert(newcodec<sizeof(myCodecList) /sizeof(CODECLIST));
	activeAudioEncoder=myCodecList[newcodec].codec;

}

uint32_t audioFilterGetNbEncoder(void)
{
	return sizeof(myCodecList) /sizeof(CODECLIST);
}
const char* audioFilterGetIndexedName(uint32_t i)
{
 	ADM_assert(i<sizeof(myCodecList) /sizeof(CODECLIST));
	return myCodecList[i].menuName;
}

void audioFilterNormalize(uint8_t onoff)
{
	audioNormalizeMode=onoff;
}

uint8_t audioFilterDelay(int32_t delay)
{	
	if(delay)
	{
		audioShift=1;
		audioDelay=delay;
	}
	else
	{
		audioShift=audioDelay=0;
	}
	return 1;

}
uint8_t audioFilterFilm2Pal(uint8_t onoff)
{
	if(onoff) audioFilmConv=FILMCONV_FILM2PAL;
	else audioFilmConv=FILMCONV_NONE;
	return 1;
}
void audioFilterDownsample(uint8_t onoff)
{
	if(onoff)
		audioResampleMode=RESAMPLING_DOWNSAMPLING;
	else
		audioResampleMode=RESAMPLING_NONE;
}
void audioFilterResample(uint32_t onoff)
{
	if(onoff)
	{
		audioResampleMode=RESAMPLING_CUSTOM;
		audioFreq=onoff;
	}
	else
		audioResampleMode=RESAMPLING_NONE;

} 
//______________________________
void audioFilter_SetBitrate( int i)
{
	audioMP3bitrate=i;
}

extern  int DIA_getAudioFilter(int *normalized, RESAMPLING *downsamplingmethod, int *tshifted,
  			 int *shiftvalue, int *drc,int *freqvalue,FILMCONV *filmconv);

void audioFilter_configureFilters( void )
{
	 DIA_getAudioFilter(&audioNormalizeMode,&audioResampleMode,&audioShift,&audioDelay,&audioDRC,&audioFreq,
	 		&audioFilmConv );

}

/*

*/

uint8_t audioCodecSetByName( const char *name)
{
		for(uint32_t i=0;i<sizeof(myCodecList)/sizeof(CODECLIST);i++)
		{
			if(!strcmp(name,myCodecList[i].name))
			{

				audioCodecSetcodec(myCodecList[i].codec);
				return 1;
			}

		}
		printf("\n Mmmm Select audio codec by name failed...(%s).\n",name);
		return 0;
}
	#define Read(x) { \
		tmp=name; \
		if((tmp=strstr(name,#x))) \
			{ \
				tmp+=strlen(#x); \
				aprintf("-- %s\n",tmp); \
				sscanf(tmp,"=%d ",&x); \
			} \
			 else \
			{ printf("*** %s not found !***\n",#x);} \
		}
#define Add(x) {sprintf(tmp,"%s=%d ",#x,x);strcat(conf,tmp);}

uint8_t audioCodecSetConf(char *name )
{
 const char *tmp;
 uint8_t newmode,oldmode;
	aprintf("-Audio filter by name : %s\n",name);

	oldmode=audioProcessMode;
	Read(audioProcessMode);
	newmode=audioProcessMode;
	audioProcessMode=oldmode;

	Read(audioMP3mode);
	Read(audioMP3bitrate);
	Read(audioMP3preset);
	UI_setAProcessToggleStatus( newmode );
	return 1;
}

const char *audioCodecGetConf( void )
{
 static char conf[400];
	static char tmp[200];
	conf[0]=0;
	Add(audioProcessMode);
	Add(audioMP3mode);
	Add(audioMP3bitrate);
	Add(audioMP3preset);

	return conf;

}
const char *audioCodecGetName( void )
{
	for(uint32_t i=0;i<sizeof(myCodecList)/sizeof(CODECLIST);i++)
	{
		if(activeAudioEncoder==myCodecList[i].codec)
		{
			return myCodecList[i].name;
		}

	}
	printf("\n Mmmm get audio  codec  name failed..\n");
	return NULL;
}
uint8_t audioFilterSetByName( const char *name)
{
	const char *tmp;
	aprintf("-Audio filter by name : %s\n",name);

	Read(audioNormalizeMode);
	Read(audioResampleMode);
	Read(audioDRC);
	Read(audioShift);
	Read(audioDelay);
	Read(audioFreq);
	return 1;
}

const char *audioFilterGetName( void )
{
	static char conf[400];
	static char tmp[200];
	conf[0]=0;
	#undef Add
	#define Add(x) {sprintf(tmp,"%s=%d ",#x,x);strcat(conf,tmp);}
	Add(audioNormalizeMode);
	Add(audioResampleMode);
	Add(audioDRC);
	Add(audioShift);
	Add(audioDelay);
	Add(audioFreq);
	return conf;

}
void audioPrintCurrentCodec(void)
{

	for(uint32_t i=0;i<sizeof(myCodecList)/sizeof(CODECLIST);i++)
	{
		if(activeAudioEncoder==myCodecList[i].codec)
		{
			UI_setAudioCodec(i);
			return;
		}
	
	}
	ADM_assert(0);
}
 void audioCodecSetcodec(AUDIOENCODER codec)
{

	activeAudioEncoder=codec;
	audioPrintCurrentCodec();

}

void audioCodecSelect( void )
{
	DIA_audioCodec( &activeAudioEncoder );
	audioPrintCurrentCodec();


}
/*
	Refresh   activeAudioEncoder value
	depending on what's selected

*/
	 

/* _____________________________________________________
  		Handle the configure signal from GUI

				pop up GUI 

*/
void audioCodecConfigure( void )
{
	int mode,bitrate;
	ADM_LAME_PRESET preset;
	bitrate= audioMP3bitrate;
	preset=audioMP3preset;
	mode=audioMP3mode;
	
	switch(activeAudioEncoder)
	{
		case AUDIOENC_NONE:
								return;
#ifdef USE_FAAC
		case AUDIOENC_FAAC:
						
							DIA_audioEncoder(&audioMP3mode, &audioMP3bitrate,"AAC parameter");
							return;
						return;
#endif	
#ifdef USE_VORBIS
		case AUDIOENC_VORBIS:
						
							DIA_audioEncoder(&audioMP3mode, &audioMP3bitrate,"VORBIS parameter");
							return;
						return;
#endif		
#ifdef HAVE_LIBMP3LAME								
		case AUDIOENC_MP3:
						DIA_getLameSettings(&audioMP3mode, &audioMP3bitrate,&audioMP3preset);
						return;
						break;
#endif

		case AUDIOENC_MP2:
							if (DIA_audioEncoder(&mode, &bitrate,"MP2 parameter"))
								{
									audioMP3mode=mode;
									audioMP3bitrate=bitrate;
								}
								return;
		case AUDIOENC_2LAME:
							if (DIA_audioEncoder(&mode, &bitrate,"Toolame parameter"))
								{
									audioMP3mode=mode;
									audioMP3bitrate=bitrate;
								}
								return;								
		case AUDIOENC_AC3:
							if (DIA_audioEncoder(&mode, &bitrate,"AC3 parameter"))
								{
									audioMP3mode=mode;
									audioMP3bitrate=bitrate;										
								}
								return;
		default:
					ADM_assert(0);																				
		
	}
	
}
//
// Build audio filters
// Starttime : starttime in ms
// mode : 0 for playback,  1 for audio save
//_______________________________________


AVDMProcessAudioStream *buildInternalAudioFilter(AVDMGenericAudioStream *currentaudiostream,
				uint32_t starttime, uint32_t size)
{

    AVDMProcessAudioStream *firstFilter = NULL;
    AVDMProcessAudioStream *lastFilter = NULL;
    AVDMProcessAudioStream *normalize = NULL;
    AVDMProcessAudioStream *downsample = NULL;
    AVDMProcessAudioStream *resample = NULL;
    AVDMProcessAudioStream *tshift = NULL;
    AVDMProcessAudioStream *drc = NULL;


    printf("\n Audio codec : %d",activeAudioEncoder);
// In fact it is more a decompress filter than a null filter
//
/* Mean: Ugly hack, in case of Film2Pal filter, the filter will need
		more samples in than out (assuming we changed the fps)
	So we correct the filter length beforehand to avoid the 24/25 too short audio */
	

	
/*   */
	if(audioFilmConv==FILMCONV_FILM2PAL)	
	{
		double d;
		d=size;
		d*=25000;
		d/=23976;
		size=(uint32_t)floor(d);
		size=(size+1)&0xfffffffe;
	}
    firstFilter = new AVDMProcessAudio_Null(currentaudiostream,
					    starttime, size);
    filtercount = 0;
    lastFilter = firstFilter;
    filters[filtercount++] = firstFilter;

//_______________________________________________________
    if (audioDelay)
      {
	  if (audioShift)
	    {
		AVDMProcessAudio_TimeShift *ts;
		ts = new AVDMProcessAudio_TimeShift(lastFilter,
						    audioDelay);
		tshift = (AVDMProcessAudioStream *) ts;
		lastFilter = tshift;
		printf("\n Time shift activated with %d %d ms", audioShift,audioDelay);
		filters[filtercount++] = lastFilter;
	    }
      }


//_______________________________________________________
    if ( audioNormalizeMode)	// Normalize activated ?
      {
	  printf("\n  normalize activated...\n");
	  normalize = new AVDMProcessAudio_Normalize(lastFilter);
	  lastFilter = normalize;
	  filters[filtercount++] = lastFilter;
	  // Preprocess input if needed....

	  currentaudiostream->beginDecompress();
	  normalize->preprocess();
	  currentaudiostream->endDecompress();


      }
    if (audioDRC)
      {
	  AVDMProcessAudio_Compress *pdrc = NULL;
	  printf("\n  DRC activated...\n");
	  pdrc = new AVDMProcessAudio_Compress(lastFilter);
	  drc = (AVDMProcessAudioStream *) pdrc;
	  lastFilter = drc;
	  filters[filtercount++] = lastFilter;

      }

      switch(audioResampleMode)
      {
      	
      	case RESAMPLING_NONE: break;
	case RESAMPLING_DOWNSAMPLING:
	 	if ((lastFilter->getInfo()->frequency != 48000) 
	  		||  (lastFilter->getInfo()->channels != 2)
			)
	    	{
			GUI_Alert(" input is not stereo@48 khz\n Downsample disabled\n");
	  	} 
		else
	    	{
			printf("\n downsample activated....");
			downsample = new AVDMProcessAudio_Resample(lastFilter, 2);
			lastFilter = downsample;
			filters[filtercount++] = lastFilter;
	    	}
		break;
	case RESAMPLING_CUSTOM:
		printf("\n resampling to %d\n",audioFreq);
		resample = new AVDMProcessAudio_SoxResample(lastFilter, audioFreq);
		lastFilter = resample;
		filters[filtercount++] = lastFilter;	
		break;
	
		
      	}
	
      switch(audioFilmConv)
      {
      	default:
		ADM_assert(0);
      	case FILMCONV_NONE:
		break;
	case FILMCONV_PAL2FILM:		
	 AVDMProcessAudio_Pal2Film *p2f;
		printf("\n Pal2Film\n");
		
		p2f = new AVDMProcessAudio_Pal2Film(lastFilter);
		lastFilter = p2f;
		filters[filtercount++] = lastFilter;	
		break;
	 	
	
	case FILMCONV_FILM2PAL:
		AVDMProcessAudio_Film2Pal *f2p;
		printf("\n Film2pal\n");
		
		f2p = new AVDMProcessAudio_Film2Pal(lastFilter);
		lastFilter = f2p;
		filters[filtercount++] = lastFilter;	
		break;
	
		
      	}      		
	
	
//_______________________________________________________



    currentaudiostream->beginDecompress();
    return lastFilter;
}

AVDMProcessAudioStream *buildPlaybackFilter(AVDMGenericAudioStream *currentaudiostream,
				uint32_t starttime, uint32_t size)
{
AVDMProcessAudioStream *lastFilter;

 	lastFilter = new AVDMProcessAudio_Null(currentaudiostream,starttime, size);
 	filtercount = 0;
 	filters[filtercount++] = lastFilter;
	if (audioDelay)
      			{
				  if (audioShift)
				    {
						AVDMProcessAudio_TimeShift *ts;
						ts = new AVDMProcessAudio_TimeShift(lastFilter,    audioDelay);

						lastFilter = ts;
						printf("\n Time shift activated with %d %d ms", audioShift,audioDelay);
						filters[filtercount++] = lastFilter;

	    				}
			}
	return lastFilter;
}
void audioForceDownSample( void)
{
	audioResampleMode=RESAMPLING_DOWNSAMPLING;
}
void audioSetResample(uint32_t fq)
{

	audioResampleMode=RESAMPLING_CUSTOM;
	audioFreq=fq;
}
/**
		Build filter to save
*/
AVDMProcessAudioStream *buildAudioFilter(AVDMGenericAudioStream *currentaudiostream,
				uint32_t starttime, uint32_t size)
{
AVDMProcessAudioStream *lastFilter=NULL;

	// if audio is set to copy, we just return the first filter
	if(!audioProcessMode)
	{
 			lastFilter = new AVDMProcessAudio_Null(currentaudiostream,
					    starttime, size);
    			filtercount = 0;
    			lastFilter = lastFilter;
    			filters[filtercount++] = lastFilter;
			return lastFilter;
	}



// else we build the full chain
			buildInternalAudioFilter(currentaudiostream,starttime, size);
			lastFilter=filters[filtercount-1];
// and add encoder...


//_______________________________________________________
uint8_t init;

		switch(activeAudioEncoder)
		{
		case 	AUDIOENC_NONE:
			// If we are dealing with big endian and using raw wav
			// we have to swap the endianness 
			#ifdef ADM_BIG_ENDIAN						
 				lastFilter = new AVDMProcessAudio_LEBE(lastFilter);
 				filters[filtercount++] = lastFilter;
			#endif
			break;
#ifdef USE_VORBIS
		case AUDIOENC_VORBIS:
		{
				AVDMProcessAudio_Vorbis *vorbis;
				vorbis = new AVDMProcessAudio_Vorbis(lastFilter);
				if(vorbis->init(audioMP3bitrate))
				{
					lastFilter = vorbis;
					filters[filtercount++] = lastFilter;
				}
 				else
				{
					delete vorbis;
					GUI_Alert("VORBIS initialization failed\n"
							" NOT ACTIVATED");
				}
		}
		break;
#endif			
#ifdef USE_FAAC
		case AUDIOENC_FAAC:
		{
				AVDMProcessAudio_Faac *faac;
				faac = new AVDMProcessAudio_Faac(lastFilter);
				if(faac->init(audioMP3bitrate))
				{
					lastFilter = faac;
					filters[filtercount++] = lastFilter;
				}
 				else
				{
					delete faac;
					GUI_Alert("FAAC initialization failed\n"
							" NOT ACTIVATED");
				}
		}
		break;
#endif		
#ifdef HAVE_LIBMP3LAME
		case AUDIOENC_MP3:
        		{
	  		AVDMProcessAudio_Lame *plame = NULL;

		  		plame = new AVDMProcessAudio_Lame(lastFilter);
				printf("\n Init of lame with bitrate %d , mode %d ",
							 audioMP3bitrate, audioMP3mode);
			  	init = plame->initLame(0, 	(uint32_t) audioMP3mode,
			      					(uint32_t) audioMP3bitrate,
								audioMP3preset);

			  	if (init)
			    	{
						lastFilter = plame;
						filters[filtercount++] = lastFilter;
				  } else
			    	{
						delete plame;
						GUI_Alert("Lame initialization failed\n"
							" NOT ACTIVATED");
			    	}
			   }
		    break;
#endif
//______________________________________________________

		case  AUDIOENC_MP2:
        {
	  		AVDMProcessAudio_FFmp2 *mp2enc = NULL;
			  // First get parameters from user

			  mp2enc = new AVDMProcessAudio_FFmp2(lastFilter);
			  printf("\n Init of FFmpeg with bitrate %d , mode %d ",
				 audioMP3bitrate, audioMP3mode);
			  init = mp2enc->init((uint32_t) audioMP3bitrate);

			  if (init)
			    {
						lastFilter = mp2enc;
						filters[filtercount++] = lastFilter;
				  } else
			    {
			    		delete mp2enc;
					GUI_Alert("FF MP2 initialization failed\n NOT ACTIVATED");
			    }
			    }
    	  break;
	  case  AUDIOENC_2LAME:
        	{
	  		AVDMProcessAudio_LibToolame *toolame_enc = NULL;
			  toolame_enc = new AVDMProcessAudio_LibToolame(lastFilter);
			  printf("\n Init of toolame with bitrate %d , mode %d ",
				 audioMP3bitrate, audioMP3mode);
			  init = toolame_enc->init((uint32_t)audioMP3mode,
			  			(uint32_t) audioMP3bitrate);

			if (init)
			{
				lastFilter = toolame_enc;
				filters[filtercount++] = lastFilter;
			} else
			{
				delete toolame_enc;
				GUI_Alert("LibToolame initialization failed\n"
					" NOT ACTIVATED");
			}
		}
    	  break;
       case  AUDIOENC_AC3:
        	{
	  		AVDMProcessAudio_FFAC3 *ac3enc = NULL;
			  // First get parameters from user

			  ac3enc = new AVDMProcessAudio_FFAC3(lastFilter);
			  printf("\n Init of FFmpeg AC3 with bitrate %d , mode %d ",
				 audioMP3bitrate, audioMP3mode);
			  init = ac3enc->init((uint32_t) audioMP3bitrate);

			  if (init)
			    {
						lastFilter = ac3enc;
						filters[filtercount++] = lastFilter;
				  } else
			    {
			    		delete ac3enc;
					GUI_Alert("FF AC3enc initialization failed\n NOT ACTIVATED");
			    }
		  }
    	  break;

      default:
      ADM_assert(0);
  }
//_______________________________________________________



    currentaudiostream->beginDecompress();
    return lastFilter;
}

// delete audio filters
//______________________
void deleteAudioFilter(void)
{
    for (uint32_t i = 0; i < filtercount; i++)
      {
	  delete filters[i];
	  filters[i] = NULL;
      }
    filtercount = 0;
    if (currentaudiostream)
	currentaudiostream->endDecompress();

}
// Build a simple filter chain
// That is starting from startTime in ms, has a duration of duration ms and is shifter
// by shift ms

AVDMGenericAudioStream *buildRawAudioFilter( uint32_t startTime, uint32_t duration, int32_t shift)
{
AVDMProcessAudioStream *firstFilter = NULL;
AVDMProcessAudioStream *lastFilter = NULL;
 
	// Start from a clean state
	printf("Building raw audio filter with start:%lu duration:%lu shift:%d\n",startTime,duration,shift);
	deleteAudioFilter();	
	lastFilter=new AVDMProcessAudio_RawShift(currentaudiostream,shift,startTime);
	filters[filtercount++] = lastFilter;
	return lastFilter;
	
	
}
