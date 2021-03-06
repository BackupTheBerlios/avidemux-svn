/***************************************************************************
                          ADMedAVIAUD.cpp  -  description
                             -------------------

    Deals with the audio stream coming from the segment and try to
    present a uniq stream to ADM_edAudio

    begin                : Sat Mar 16 2002
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
#include <assert.h>
#include <math.h>

#include "config.h"
#include "fourcc.h"
#include "avio.hxx"
#include "indexer.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_editor/ADM_edAudio.hxx"

#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_AUDIO_EDITOR
#include "ADM_toolkit/ADM_debug.h"


#define AUDIOSEG 	_segments[_audioseg]._reference
#define SEG 		_segments[seg]._reference

//__________________________________________________
//
//  Go to the concerned segment and concerned offset
//
// Should never be called normally...
//
//__________________________________________________
uint8_t ADM_Composer::audioGoTo (uint32_t offset)
{
  uint32_t
    seg,
    ref;			//, sz;
  AVDMGenericAudioStream *
    as;

    aprintf("Editor: audio go to : seg %lu offset %lu \n",_audioseg,_audiooffset);
  seg = 0;
  while (1)
    {
      ref = _segments[seg]._reference;
      as = _videos[ref]._audiostream;
      if (!as)
	return 0;

      if (_segments[seg]._audio_size < offset)
	{
	  offset -= _segments[seg]._audio_size;
	  seg++;
	  if (seg >= _nb_segment)
	    return 0;
	  continue;
	}
      // we are in the right seg....
      else
	{
	  _audioseg = seg;
	  _audiooffset = offset + _segments[seg]._audio_start;
	  as->goToSync (_audiooffset);
	  return 1;
	}

    }

  return 0;
}

//__________________________________________________
//
//
uint32_t
  ADM_Composer::readPCMeq (uint32_t lenasked, uint8_t * out,
			   uint32_t * lenout)
{
  // ask in curent seg...

  uint32_t end_offset, read;

  // Offset of last byte in this seg...

  if (_audioseg == _nb_segment)
    {
      _audioseg = 0;
      _audiooffset = 0;
      return 0;
    }
  end_offset =
    _segments[_audioseg]._audio_start + _segments[_audioseg]._audio_size;

  read =
    _videos[_segments[_audioseg]._reference]._audiostream->
    readPCMeq (lenasked, out, lenout);

  _audiooffset += read;

  if (_audiooffset > end_offset)	// time to switch seg
    {
      _audioseg++;
      if (_audioseg < _nb_segment)
	// recalibrate, may cause plop
	{
	  _audiooffset = _segments[_audioseg]._audio_start;
	  _videos[_segments[_audioseg]._reference]._audiostream->
	    goToSync (_audiooffset);
	}
    }
  return read;
}

/*
		Read sequentially audio

*/
uint32_t ADM_Composer::audioRead (uint32_t len, uint8_t * buffer)
{
  // first check if the len is within the seg ...
  uint32_t    end_offset,    red;

  // Offset of last byte in this seg...
  end_offset =    _segments[_audioseg]._audio_start + _segments[_audioseg]._audio_size;

  // we are beyond the end mark   or to the end mark
  if (end_offset <= _audiooffset)
    {
      // we went out of bounds...
      if (_audiooffset > end_offset)
	{
	  printf ("\n *** Ooops , audio spilled, overshot : %lu\n",
		  _audiooffset - end_offset);
	}
      aprintf (" EDITOR audio next segments : %lu /  %lu start: %lu, size : %lu->  going to seg %ld\n",
	      _audiooffset, end_offset,
	      _segments[_audioseg]._audio_start,
	      _segments[_audioseg]._audio_size, _audioseg + 1);
      // we are at the end mark
      // switch to next seg...
      _audioseg++;
      if (_audioseg == _nb_segment)
	{
	  _audioseg = 0;
	  _audiooffset = 0;
	  memset (buffer, 0, len);	// avoid sproutch
	  return 0;
	}
      _audiooffset = _segments[_audioseg]._audio_start;
      _videos[_segments[_audioseg]._reference]._audiostream->goToSync (_audiooffset);
      return audioRead (len, buffer);
    }

  // we still got data (theoritically in current seg...
  // everything in the current seg ?
  if ((_audiooffset + len) <= end_offset)
    {
      red =	_videos[_segments[_audioseg]._reference]._audiostream->read (len,
								     buffer);
      // fill with dummy....
      if (!red)
	{

	  if (_audioseg == (_nb_segment - 1))
	    {			// last segment no filling
	      return 0;

	    }
	  printf (" Filling with dummy...\n");
	  memset (buffer + red, 0, len);
	  red = len;
	}
      _audiooffset += red;
      return red;
    }
  // we have to switch from one seg to the next one
  //
  // leftover
  aprintf ("\n switching segments\n");
  uint32_t
    left;
  left = end_offset - _audiooffset;
  red =
    _videos[_segments[_audioseg]._reference]._audiostream->read (left,
								 buffer);
  if (!red)
    {
      printf (" error readinf audio from seg : %lu\n", _audioseg);
      memset (buffer + red, 0, left);
      red = left;		// dummy
    }
  _audiooffset += left;

  if (red != len)
    {
      // there 's more coming to fetch go to next seg
      return red + audioRead (len - red, buffer + red);
    }
  else
    return red;
  return 0;

}
/*
		Convert frame from a seg to an offset
		(normally not used)
*/
uint8_t  ADM_Composer::audioFnToOff (uint32_t seg, uint32_t fn, uint32_t * noff)
{
 aprintf("Editor: audioFnToOff go to : seg %lu fn %lu \n",seg,fn);
  assert (seg < _nb_segment);
  assert (_videos[AUDIOSEG]._audiostream);

#define AS _videos[AUDIOSEG]._audiostream

  *noff = AS->convTime2Offset (getTime (fn));
  return 1;
}

//  --------------------------------------------
uint8_t ADM_Composer::audioGoToTime (uint32_t mstime, uint32_t * off)
{

  uint32_t	    cumul_offset =    0;
  uint32_t    frames =    0,    fi =    0;
  aviInfo    info;

  double    fn;

  getVideoInfo (&info);
  fn = mstime;
  fn = fn * info.fps1000;
  fn /= 1000000.;

  fi = (uint32_t) floor (fn);

  aprintf (" Editor audio GOTOtime : %lu ms\n", mstime);
  aprintf (" Editor audio frame eq: %lu \n", fi);

  // now we have virtual frame number, convert to seg & real frame
  uint32_t    seg,    relframe;

  if (!convFrame2Seg (fi, &seg, &relframe))
    return 0;

  aprintf (" Editor audio seg : %lu frame %ld\n", seg, relframe);
  // compute position from previous seg
  if (seg)
    {
      for (uint32_t i = 0; i < seg; i++)
	{
	  cumul_offset += _segments[i]._audio_size;
	  frames += _segments[i]._nb_frames;
	}
    }
  // compute start frame time
  uint32_t    time_start,    time_masked,    jump;

  time_masked = getTime (frames);
  time_start = getTime (_segments[seg]._start_frame);

  jump = mstime - time_masked;
  jump = jump + time_start;

  aprintf (" Editor audio time start : %lu\n", time_start);
  aprintf (" Editor audio time masked : %lu\n", time_masked);
  aprintf (" Editor audio time jump : %lu\n", jump);

  _audioseg = seg;
  _videos[AUDIOSEG]._audiostream->goToTime (jump);
  _audiooffset = _videos[AUDIOSEG]._audiostream->getPos ();
  *off = cumul_offset + _videos[AUDIOSEG]._audiostream->getPos ()
    - _segments[seg]._audio_start;

  return 1;
}
/*
		Go to Frame num, from segment seg
		Used to compute the duration of audio track

*/
uint8_t ADM_Composer::audioGoToFn (uint32_t seg, uint32_t fn, uint32_t * noff)
{
  uint32_t    time;

 aprintf("Editor: audioGoToFn go to : seg %lu fn %lu \n",seg,fn);

// since we got the frame we can find the segment


  assert (seg < _nb_segment);
  assert (_videos[SEG]._audiostream);
  _audioseg=seg;
#undef AS
#define AS _videos[SEG]._audiostream

  time = _videos[SEG]._aviheader->getTime (fn);
  AS->goToTime (time);
  *noff = AS->getPos ();
  return AS->goToTime (time);
}

//
//  Return audio length of all segments.
//
//
uint32_t ADM_Composer::getAudioLength (void)
{
  uint32_t
    seg,
    len =
    0;				//, sz;
  //
  for (seg = 0; seg < _nb_segment; seg++)
    {
      // for each seg compute size
      len += _segments[seg]._audio_size;
    }
  _audio_size = len;
  return len;
}

//
//  Build the internal audio stream from segs
//

uint8_t ADM_Composer::getAudioStream (AVDMGenericAudioStream ** audio)
{
  if (*audio)
    {
      delete *
	audio;
      *audio = NULL;
    }
  if (!_videos[0]._audiostream)
    {
      *audio = NULL;
      return 0;

    }
  *audio = new AVDMEditAudioStream (this);
  return 1;
};

//
//
WAVHeader *ADM_Composer::getAudioInfo (void)
{
  if (_videos[0]._audiostream)
    {
      return _videos[0]._audiostream->getInfo ();
    }
  return NULL;
}
