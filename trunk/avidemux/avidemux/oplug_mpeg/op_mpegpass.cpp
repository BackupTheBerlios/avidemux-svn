//
// C++ Implementation: op_mpegpass
//
// Description: 
//   This save mpeg video to mpegPS in video copy mode
//	Audio can be in copy mode or not.
//
// Author: mean <fixounet@free.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "ADM_library/default.h"
#include "avi_vars.h"
//#include "aviaudio.hxx"
#include "ADM_audiofilter/audioprocess.hxx"

#include "ADM_dialog/DIA_working.h"

#include "ADM_audiofilter/audioeng_buildfilters.h"
#include "prefs.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_lvemux/ADM_muxer.h"


/**
	Save a cut and audio edited to mpeg-PS
	Usefull for editing PVR captured files for example

*/
 extern AVDMGenericAudioStream *mpt_getAudioStream(double *pcm);

 #define PACK_AUDIO audiolen = audio->read ((uint32_t)floor(total_wanted-total_got),buffer); \
		total_got+=audiolen; \
		if(audiolen)	\
			muxer->writeAudioPacket(audiolen,buffer); 
    
 
 
void mpeg_passthrough(  char *name )
{
  uint32_t len, flags;
  AVDMGenericAudioStream *audio=NULL;
  uint32_t audiolen;
  uint8_t *buffer = new uint8_t[avifileinfo->width * avifileinfo->height * 3];
  
  DIA_working *work;
  double pcm=0;
  
  double total_wanted=0;
  double total_got=0;
  
  MpegMuxer *muxer=NULL;
  
  	printf("Saving as mpg PS to file %s\n",name);
  
  	// First we check it is mpeg
  	if(!fourCC::check(avifileinfo->fcc,(uint8_t *)"MPEG"))
  	{
  		GUI_Alert("This is not a mpeg.\nSwitch to process mode.");
		return ;
  	}
  	if(!currentaudiostream)
  	{
  		GUI_Alert("There is no audio track.");
		return ;
  	}
  
  	audio=mpt_getAudioStream(&pcm);
	if(!audio)
	{
		GUI_Alert("Audio track is not suitable!\n");
		return;
	}
		
	if(!pcm)
	{
		GUI_Alert("Audio track is not suitable(2)!\n");
		return;
	}
	
  	muxer=new MpegMuxer();
	if(!muxer->open(name,MUX_MPEG_VRATE,avifileinfo->fps1000,audio->getInfo()))
	{
		delete muxer;
		muxer=NULL;
		printf("Muxer init failed\n");
		return ;
		
	}
  
  ///____________________________
  work=new DIA_working("Saving MpegPS stream");

  // preamble
  video_body->getRawStart (frameStart, buffer, &len);
  muxer->writeVideoPacket (len,buffer);

  for (uint32_t i = frameStart; i < frameEnd; i++)
    {
      total_wanted+=pcm;
      
      work->update (i - frameStart, frameEnd - frameStart);
      if(!work->isAlive()) goto _abt;
      assert (video_body->getFlags (i, &flags));
      if (flags & AVI_B_FRAME)	// oops
	{
	  // se search for the next i /p
	  uint32_t found = 0;

	for (uint32_t j = i + 1; j < frameEnd; j++)
	{
		assert (video_body->getFlags (j, &flags));
		if (!(flags & AVI_B_FRAME))
		{
			found = j;
			break;
		}

	}
	if (!found)	    goto _abt;
	  // Write the found frame
	video_body->getRaw (found, buffer, &len);
	muxer->writeVideoPacket (len,buffer);	
	PACK_AUDIO
	  
	  // and the B frames
	for (uint32_t j = i; j < found; j++)
	    {
		video_body->getRaw (j, buffer, &len);
		muxer->writeVideoPacket (len,buffer);
		PACK_AUDIO
    	    }
	  i = found;		// Will be plussed by for
	}
      else			// P or I frame
	{
	  	video_body->getRaw (i, buffer, &len);
		muxer->writeVideoPacket (len,buffer);
		PACK_AUDIO
	}

    }
_abt:
  delete work;
  muxer->close();
  delete muxer;
  delete buffer;
  deleteAudioFilter();

}
