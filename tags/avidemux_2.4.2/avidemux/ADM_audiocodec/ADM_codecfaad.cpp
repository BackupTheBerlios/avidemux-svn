//
// C++ Implementation: ADM_codecfaad
//
// Description: class ADM_faad, interface to libfaad2 .
//
// The init can be done 2 ways
// 1- some info are used as extradata (esds atom ot whatever) 
// 2- the init is done when decoding the actual raw stream
//
// The big drawback if that in some case you can eat up a lot of the stream
// before finding a packet start code to enable correct init of the codec
//
// Author: mean <fixounet@free.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "config.h"
#ifdef USE_FAAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ADM_assert.h>
#include <math.h>

#include "fourcc.h"
#include "ADM_audio/aviaudio.hxx"
#include "ADM_audiocodec/ADM_audiocodec.h"
#include "ADM_audiofilter/audiofilter_channel_route.h"
#include "faad.h"


ADM_faad::ADM_faad( uint32_t fourcc ,uint32_t highEfficiency,WAVHeader *info,uint32_t l,uint8_t *d) :   ADM_Audiocodec(fourcc)
{
faacDecConfigurationPtr conf;
#ifdef OLD_FAAD_PROTO
unsigned long int srate;
#else
uint32_t srate;
#endif
unsigned char chan;
		_inited=0;
		_instance=NULL;
		_inbuffer=0;
		_instance = faacDecOpen();
		conf=faacDecGetCurrentConfiguration(_instance);
		// Update the field we know about
		conf->outputFormat=FAAD_FMT_16BIT;
		conf->defSampleRate=info->frequency;
                if(!highEfficiency)
                {
		  conf->defObjectType =LC;
                }else
                {
                   conf->defObjectType =HE_AAC;
                }
		faacDecSetConfiguration(_instance, conf);
                printf("[FAAD] using %u bytes of extradata\n",l);
		// if we have some extra data, it means we can init it from it
		if(l)
		{
			_inited = 1;
			faacDecInit2(_instance, d,l, &srate,&chan);
			printf("[FAAD]Found :%d rate %d channels\n",srate,chan);
                        if(srate!=info->frequency)
                        {
                            printf("[FAAD]Frequency mismatch!!! %d to %d (SBR ?)\n",info->frequency,srate);
                            info->frequency=srate;
                        }	
                        if(chan!=info->channels) // Ask for stereo !
                        {
                             printf("[FAAD]channel mismatch!!! %d to %d \n",info->channels,chan);
                            info->channels=chan;
                        }
		}
		else // we will init it later on...
		{
			_inited=0;
			printf("No conf header, will try to init later\n");
			
		}

	ch_route.input_type[0] = CH_FRONT_CENTER;
	ch_route.input_type[1] = CH_FRONT_LEFT;
	ch_route.input_type[2] = CH_FRONT_RIGHT;
	ch_route.input_type[3] = CH_REAR_LEFT;
	ch_route.input_type[4] = CH_REAR_RIGHT;
	ch_route.input_type[5] = CH_LFE;

		printf("[FAAD]Faad decoder created\n");
}

ADM_faad::~ADM_faad()
{
	if(_instance)
		faacDecClose(_instance);
	_instance=NULL;
	printf("Faad decoder closed\n");
}

void ADM_faad::purge(void)
{
	_inbuffer=0;
	 faacDecPostSeekReset(_instance, 0);
}
uint8_t ADM_faad:: beginDecompress( void )
{
	_inbuffer=0;
}
uint8_t ADM_faad::endDecompress( void ) 
{
	_inbuffer=0;
	 faacDecPostSeekReset(_instance, 0);
}
uint8_t ADM_faad::run(uint8_t *inptr, uint32_t nbIn, float *outptr, uint32_t *nbOut)
{
uint32_t xin;
long int res;
void *outbuf;
faacDecFrameInfo info;
int max=0;
#ifdef OLD_FAAD_PROTO
unsigned long int srate;
#else
uint32_t srate;
#endif
unsigned char chan=0;
uint8_t first=0;


		ADM_assert(_instance);
		*nbOut=0;
		if(!_inited) // we still have'nt initialized the codec
		{
			// Try
			printf("Trying with %d bytes\n",nbIn);
			res=faacDecInit(_instance,inptr,nbIn,&srate,&chan);
			if(res>=0)
			{
				printf("Faad Inited : rate:%d chan:%d off:%ld\n",srate,chan,res);
				_inited=1;
				first=1;
				_inbuffer=0;
				inptr+=res;
				nbIn-=res;
				
			}
		}
		if(!_inited)
		{
			printf("No dice...\n");
			return 1;	
		}
		// The codec is initialized, feed him
		do
		{
			max=FAAD_BUFFER-_inbuffer;
			if(nbIn<max) max=nbIn;
			memcpy(_buffer+_inbuffer,inptr,max);
			inptr+=max;
			nbIn-=max;
			_inbuffer+=max;
			/*
			if(_inbuffer<FAAD_MIN_STREAMSIZE*2)
			{
				return 1;
			}*/
			memset(&info,0,sizeof(info));
			//printf("%x %x\n",_buffer[0],_buffer[1]);
		 	outbuf = faacDecDecode(_instance, &info, _buffer, _inbuffer);
			if(info.error)
			{
				printf("Faad: Error %d :%s\n",info.error,
					faacDecGetErrorMessage(info.error));
				printf("Bye consumed %u, bytes dropped %u \n",info.bytesconsumed,_inbuffer);

                                _inbuffer=0; // Purge buffer
				return 1;
			}
			if(first)
			{
				printf("Channels : %d\n",info.channels);
				printf("Frequency: %d\n",info.samplerate);
				printf("SBR      : %d\n",info.sbr);
				
			
			}
			xin=info.bytesconsumed ;
			if(xin>_inbuffer) xin=0;
			
			memmove(_buffer,_buffer+xin,_inbuffer-xin);
			_inbuffer-=xin;
			if(info.samples)
			{
				*nbOut+= info.samples;
				int16_t *in = (int16_t *) outbuf;
				for (int i = 0; i < info.samples; i++) {
					*(outptr++) = (float)*in / 32768;
					in++;
				}
			}
		}while(nbIn);
		return 1;
}
#endif
