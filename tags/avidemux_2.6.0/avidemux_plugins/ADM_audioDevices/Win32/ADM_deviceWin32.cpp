//
// C++ Implementation: ADM_deviceWin32
//
// Description: 
// C++ Implementation: ADM_deviceWin32
// Use MM layer to output sound
//
// Author: mean <fixounet@free.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//

#include "ADM_default.h"
#include  "ADM_audiodevice.h"
#include  "ADM_audioDeviceInternal.h"

#include <windows.h>

#include "ADM_deviceWin32.h"

#define aprintf(...) {}

#define NB_BUCKET 64

static uint32_t bucketSize;
static HWAVEOUT myDevice;
static MMRESULT myError;

static void handleMM(MMRESULT err);

WAVEHDR waveHdr[NB_BUCKET];	
ADM_DECLARE_AUDIODEVICE(Win32,win32AudioDevice,1,0,0,"Win32 audio device (c) mean");
/**
    \fn ctor
*/

win32AudioDevice::win32AudioDevice(void) 
{
	printf("[Win32] Creating audio device\n");
	_inUse=0;
}
/**
    \fn localStop
*/

bool win32AudioDevice::localStop(void) 
{
	if (!_inUse)
		return false;

	printf("[Win32] Closing audio\n");

	waveOutReset(myDevice);		

	for (uint32_t i = 0; i < NB_BUCKET; i++)
		waveOutUnprepareHeader(myDevice, &waveHdr[i], sizeof(WAVEHDR));

	myError = waveOutClose(myDevice);

	if (myError != MMSYSERR_NOERROR)
	{
		printf("[Win32] Close failed %d\n", myError);
		handleMM(myError);
		return 0;
	}

	for (uint32_t i = 0; i < NB_BUCKET; i++)
		delete[] waveHdr[i].lpData;

	_inUse=0;
	myDevice = NULL;

	return true;
}
/**
    \fn localInit
*/
bool win32AudioDevice::localInit(void) 
{
	printf("[Win32] Opening Audio, channels=%u freq=%u\n",_channels, _frequency);

	if (_inUse) 
	{
		printf("[Win32] Already running?\n");
		return false;
	}

	_inUse = 1;
	
	bucketSize = (_channels * _frequency*2*4)/100; // 40 ms bucket * 64 => 3 sec buffer

	WAVEFORMATEX wav;

	memset(&wav, 0, sizeof(WAVEFORMATEX));

	wav.wFormatTag = WAVE_FORMAT_PCM;
	wav.nSamplesPerSec = _frequency;
	wav.nChannels = _channels;
	wav.nBlockAlign = 2 * _channels;
	wav.nAvgBytesPerSec = 2 * _channels * _frequency;
	wav.wBitsPerSample = 16;

	myError = waveOutOpen(&myDevice, WAVE_MAPPER, &wav, NULL, NULL, CALLBACK_NULL);

	if (MMSYSERR_NOERROR != myError)
	{
		printf("[Win32] waveOutOpen failed\n");
		handleMM(myError);
		return 0;
	}

	for (uint32_t i = 0; i < NB_BUCKET; i++)
	{
		memset(&waveHdr[i], 0, sizeof(WAVEHDR));

		waveHdr[i].dwBufferLength = bucketSize;
		waveHdr[i].lpData = (char*)new uint8_t[bucketSize];

		if (waveOutPrepareHeader(myDevice, &waveHdr[i], sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
			printf("[Win32] waveOutPrepareHeader error\n");

		waveHdr[i].dwBufferLength = 0;
		waveHdr[i].dwFlags |= WHDR_DONE;
	}

	return 1;
}
/**
    \fn setVolume
*/
uint8_t  win32AudioDevice::setVolume(int volume) 
{
	uint32_t value;

	value = (0xffff * volume) / 100;
	value = value + (value << 16);

	waveOutSetVolume(myDevice, value);

	return 1;
}
/**
    \fn sendData
*/

void win32AudioDevice::sendData(void)
{
	uint8_t success = 0;
    uint32_t len=wrIndex-rdIndex;

	for (uint32_t i = 0; i < NB_BUCKET; i++)
	{
		if (waveHdr[i].dwFlags & WHDR_DONE)
		{
			waveHdr[i].dwFlags &= ~WHDR_DONE;
            mutex.lock();
			if (len > bucketSize)
				waveHdr[i].dwBufferLength = bucketSize;
			else
				waveHdr[i].dwBufferLength = len;

			memcpy(waveHdr[i].lpData, audioBuffer+rdIndex, waveHdr[i].dwBufferLength);
			rdIndex += waveHdr[i].dwBufferLength;
			len -= waveHdr[i].dwBufferLength;
            mutex.unlock();
            MMRESULT er=waveOutWrite(myDevice, &waveHdr[i], sizeof(WAVEHDR));
			if (er == MMSYSERR_NOERROR)
				success = 1;
			else
            {
                handleMM(er);
				break;
            }
		}

		if (len == 0)
			break;
	}
	return ;
}

/**
    \fn getWantedChannelMapping
*/
const CHANNEL_TYPE mono[MAX_CHANNELS]={ADM_CH_MONO};
const CHANNEL_TYPE stereo[MAX_CHANNELS]={ADM_CH_FRONT_LEFT,ADM_CH_FRONT_RIGHT};
const CHANNEL_TYPE fiveDotOne[MAX_CHANNELS]={ADM_CH_FRONT_LEFT,ADM_CH_FRONT_RIGHT,ADM_CH_FRONT_CENTER,
                                             ADM_CH_LFE,ADM_CH_REAR_LEFT,ADM_CH_REAR_RIGHT};
/**
    \fn getWantedChannelMapping
*/
const CHANNEL_TYPE *win32AudioDevice::getWantedChannelMapping(uint32_t channels)
{
    switch(channels)
    {
        case 1: return mono;break;
        case 2: return stereo;break;
        default:
                return fiveDotOne;
                break;
    }
    return NULL;
}
/**
    \fn handleMM
*/
void handleMM(MMRESULT err)
{
#define ERMM(x) if(err==x) printf("[Win32] "#x"\n");

	ERMM(MMSYSERR_ALLOCATED);
	ERMM(MMSYSERR_BADDEVICEID);
	ERMM(MMSYSERR_NODRIVER);
	ERMM(WAVERR_BADFORMAT);
	ERMM(WAVERR_SYNC);
    ERMM(WAVERR_STILLPLAYING);
}
// EOF
