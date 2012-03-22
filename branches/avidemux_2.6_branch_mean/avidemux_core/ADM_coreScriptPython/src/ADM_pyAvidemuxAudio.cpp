/***************************************************************************
   \file ADM_pyAvidemux.cpp
    \brief binding between tinyPy and avidemux
    \author mean/gruntster 2011/2012
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ADM_pyAvidemux.h"
#include "ADM_audiodef.h"
#include "ADM_vidMisc.h"
#include "fourcc.h"
#include "DIA_fileSel.h"
#include "DIA_coreToolkit.h"

/**
    \fn audioProlog
*/
static bool audioProlog(IEditor *editor, int dex,WAVHeader &info)
{
    ADM_audioStream *s=editor->getAudioStreamAt(dex);
    if(!s)
    {
        ADM_warning("No audio at index %d\n",dex);
        return false;
    }
    WAVHeader *h=s->getInfo();
    if(!h)
    {
        ADM_warning("No header at index %d\n",dex);
        return false;
    }
    info=*h;
    return true;
}

/**
    \fn
    \brief
*/
int pyChangeAudioStream(IEditor *editor, int track)
{
//	editor->changeAudioStream(0, track);
    return 0;
}

/**
    \fn pyGetAudioChannels
    \brief
*/

int pyGetAudioChannels(IEditor *editor,int dex)
{
    WAVHeader h;
    if(!audioProlog(editor,dex,h)) return 0;
	return h.channels;
}
/**
    \fn
    \brief
*/

int pyGetAudioFrequency(IEditor *editor,int dex)
{
    WAVHeader h;
    if(!audioProlog(editor,dex,h)) return 0;
	return h.frequency;

}
/**
    \fn
    \brief
*/

int pyGetAudioEncoding(IEditor *editor,int dex)
{
    WAVHeader h;
    if(!audioProlog(editor,dex,h)) return 0;
	return h.encoding;
}
/**
    \fn
    \brief
*/

int32_t pyGetPal2Film(IEditor *editor,int dex)
{
	if (editor->getAudioFilterFrameRate(dex) == FILMCONV_PAL2FILM)
	{
		return 1;
	}

	return 0;
}
/**
    \fn
    \brief
*/

int32_t pyGetFilm2Pal(IEditor *editor,int dex)
{
	if (editor->getAudioFilterFrameRate(dex) == FILMCONV_FILM2PAL)
	{
		return 1;
	}

	return 0;
}
/**
    \fn
    \brief
*/

void pySetPal2Film(IEditor *editor, int dex,int32_t rate)
{
	if (rate)
	{
		editor->setAudioFilterFrameRate(dex,FILMCONV_PAL2FILM);
	}
	else if (pyGetPal2Film(editor,dex))
	{
		editor->setAudioFilterFrameRate(dex,FILMCONV_NONE);
	}
}
/**
    \fn
    \brief
*/

void pySetFilm2Pal(IEditor *editor, int dex,int32_t rate)
{
	if (rate)
	{
		editor->setAudioFilterFrameRate(dex,FILMCONV_FILM2PAL);
	}
	else if (pyGetFilm2Pal(editor,dex))
	{
		editor->setAudioFilterFrameRate(dex,FILMCONV_NONE);
	}
}
/**
    \fn
    \brief
*/

int pyGetNormalizeMode(IEditor *editor,int dex)
{
	ADM_GAINMode m;
	uint32_t gain;

	editor->getAudioFilterNormalise(dex,&m, &gain);

	return m;
}
/**
    \fn
    \brief
*/

int pyGetNormalizeValue(IEditor *editor,int dex)
{
	ADM_GAINMode m;
	uint32_t gain;

	editor->getAudioFilterNormalise(dex,&m, &gain);

	return (int)gain;
}
/**
    \fn
    \brief
*/

void pySetNormalizeMode(IEditor *editor, int dex,int mode)
{
	ADM_GAINMode m;
	uint32_t gain;

	editor->getAudioFilterNormalise(dex,&m, &gain);
	m = (ADM_GAINMode)mode;
	editor->setAudioFilterNormalise(dex,m, gain);
}
/**
    \fn
    \brief
*/

void pySetNormalizeValue(IEditor *editor, int dex,int value)
{
	ADM_GAINMode m;
	uint32_t gain;

	editor->getAudioFilterNormalise(dex,&m, &gain);
	gain = (uint32_t)value;
	editor->setAudioFilterNormalise(dex,m, gain);
}
/**
    \fn
    \brief
*/
int pyClearAudioTracks(IEditor *editor)
{
    editor->clearAudioTracks();
    return true;
}
/**
    \fn
    \brief
*/
int pyAddAudioTrack(IEditor *editor,int poolIndex)
{
    return editor->addAudioTrack(poolIndex);
}

// EOF
