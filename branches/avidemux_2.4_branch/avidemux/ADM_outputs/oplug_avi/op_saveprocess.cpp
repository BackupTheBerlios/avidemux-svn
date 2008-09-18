/***************************************************************************
                          op_saveprocess.cpp  -  description
                             -------------------
    begin                : Fri May 3 2002
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
#include <math.h>
#include <unistd.h>

#include <time.h>
#include <sys/time.h>
#include "config.h"
#include <pthread.h>
#define WIN32_CLASH
#include "fourcc.h"
#include "avi_vars.h"
#include "ADM_toolkit/toolkit.hxx"
#include "subchunk.h"
//#include "avilist.h"

#include "ADM_audio/aviaudio.hxx"
#include "ADM_audiofilter/audioprocess.hxx"

#include "ADM_video/ADM_genvideo.hxx"
//#include "ADM_gui/GUI_encoder.h"
#include "ADM_filter/video_filters.h"
#include "ADM_encoder/ADM_vidEncode.hxx"

#include "oplug_avi/op_aviwrite.hxx"
#include "oplug_avi/op_avisave.h"

#include "ADM_encoder/adm_encoder.h"
#include "oplug_avi/op_saveprocess.h"

#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_SAVE_AVI
#include "ADM_osSupport/ADM_debug.h"

GenericAviSaveProcess::GenericAviSaveProcess( void ) 
{
	TwoPassLogFile=NULL;
	_incoming=NULL;
	_encode=NULL;
	_videoProcess=1;
	_prefill=0;
};

uint8_t
GenericAviSaveProcess::setupVideo (char *name)
{
	_notnull=0;
	_incoming = getLastVideoFilter (frameStart,frameEnd-frameStart);
 	frametogo=_incoming->getInfo()->nb_frames;
	encoding_gui->setFps(_incoming->getInfo()->fps1000);
	// anish
 	if(_incoming->getInfo()->width%8)
		{
                  if(!GUI_Question(QT_TR_NOOP("Width is not a multiple of 8\n continue anyway ?")))
			return 0;

		}

  _encode = getVideoEncoder (_incoming->getInfo()->width,_incoming->getInfo()->height);
  if (!_encode)
    return 0;

  // init compressor
  TwoPassLogFile=new char[strlen(name)+6];
  strcpy(TwoPassLogFile,name);
  strcat(TwoPassLogFile,".stat");
 _encode->setLogFile(TwoPassLogFile,frametogo);
 
 
  if (!_encode->configure (_incoming))
    {
      delete 	_encode;
      _encode = NULL;
      GUI_Error_HIG (QT_TR_NOOP("Filter init failed"), NULL);
      return 0;
    };
 
  memcpy (&_bih, video_body->getBIH (), sizeof (_bih));
  _bih.biWidth = _incoming->getInfo ()->width;
  _bih.biHeight = _incoming->getInfo ()->height;
  _bih.biSize=sizeof(_bih);
  _bih.biXPelsPerMeter=_bih.biClrUsed=_bih.biYPelsPerMeter=0;

  _mainaviheader.dwTotalFrames= _incoming->getInfo ()->nb_frames;
_mainaviheader.dwMicroSecPerFrame=0;

  printf("\n Saved as %ld x %ld\n",_bih.biWidth,_bih.biHeight);
  _bih.biCompression=fourCC::get((uint8_t *)_encode->getCodecName());
   
  encoding_gui->setCodec(_encode->getDisplayName());
  
  // init save avi
//-----------------------2 Pass--------------------------------------
  if (_encode->isDualPass ())
    {
      uint8_t *buffer;
      uint32_t len, flag;
      FILE *tmp;
	uint8_t reuse=0;
	int prefill = 0, r;

 	aprintf("\n** Dual pass encoding**\n");

	
	
	if((tmp=fopen(TwoPassLogFile,"rt")))
	{
		fclose(tmp);
                if(GUI_Question(QT_TR_NOOP("\n Reuse the existing log-file ?")))
		{
			reuse=1;
		}
	}
	
	if(!reuse)
 	{
	
      	guiSetPhasis (QT_TR_NOOP("1st Pass"));
      	aprintf("**Pass 1:%lu\n",frametogo);
     	buffer = new uint8_t[_incoming->getInfo ()->width *
		    _incoming->getInfo ()->height * 3];

      	_encode->startPass1 ();

        bitstream.bufferSize = _incoming->getInfo()->width * _incoming->getInfo()->height * 3;
        bitstream.data=buffer;

preFilling:
		bitstream.cleanup(0);

		if(!_encode->encode(prefill, &bitstream))
		{
			printf("AVI: First frame error\n");
			GUI_Error_HIG(QT_TR_NOOP("Error while encoding"), NULL);
			return 0;
		}

		if (!bitstream.len)
		{
			prefill++;
			goto preFilling;
		}

		printf("Prefill: %u\n", prefill);

      //__________________________________
      //   now go to main loop.....
      //__________________________________
        for (uint32_t cf = 1; cf < frametogo; cf++)
        {
          if (guiUpdate (cf, frametogo))
            {
            abt:
                GUI_Error_HIG (QT_TR_NOOP("Aborting"), NULL);
              delete[]buffer;
              return 0;
            }
            
            bitstream.cleanup(cf);

			if (!prefill || cf + prefill < frametogo)
				r = _encode->encode(prefill + cf, &bitstream);
			else
				r = _encode->encode(frametogo - 1, &bitstream);

			if (!r)
			{
				printf("Encoding of frame %lu failed!\n", cf);
				goto abt;
			}

           encoding_gui->setFrame(cf,bitstream.len,bitstream.out_quantizer,frametogo);
    
        }

	encoding_gui->reset();
      	delete[]buffer;	
     	aprintf("**Pass 1:done\n");
    }// End of reuse

      if(!_encode->startPass2 ())
      {
      	printf("Pass2 ignition failed\n");
      	return 0;
	}
   }   //-------------------------/VBR-----------------------------------
  // init save avi

// now we build the new stream !
    	aprintf("**main pass:\n");

		memcpy(&_videostreamheader,video_body->getVideoStreamHeader (),sizeof(_videostreamheader));
		memcpy(&_videostreamheader.fccHandler	,_encode->getFCCHandler(),4);
		_videostreamheader.fccType	=fourCC::get((uint8_t *)"vids");
		_videostreamheader.dwScale=1000;
		_videostreamheader.dwRate= _incoming->getInfo ()->fps1000;

    		memcpy(&_mainaviheader	,video_body->getMainHeader (),sizeof(_mainaviheader));


  		  _mainaviheader.dwWidth=_bih.biWidth;
    		_mainaviheader.dwHeight=_bih.biHeight;
    		_videostreamheader.dwQuality=10000;

    uint8_t *data;
    uint32_t dataLen=0;

    _encode->hasExtraHeaderData( &dataLen,&data);
  	if (!writter->saveBegin (name,
			   &_mainaviheader,
			   frameEnd - frameStart + 1,
			   &_videostreamheader,
			   &_bih,
			   data,dataLen,
			   (AVDMGenericAudioStream *) audio_filter,
			   NULL))
    	{
      		return 0;
    	}
  aprintf("Setup video done\n");
  bitstream.data=vbuffer;
  bitstream.bufferSize=MAXIMUM_SIZE * MAXIMUM_SIZE * 3;
  return 1;
  //---------------------
}

//
//      Just to keep gcc happy....
//
GenericAviSaveProcess::~GenericAviSaveProcess ()
{
  cleanupAudio();
  if (_encode)
    delete      _encode;
  	_encode=NULL;
  if(TwoPassLogFile)
  {
  	delete [] TwoPassLogFile;
  	TwoPassLogFile=NULL;
  }
}

// copy mode
// Basically ask a video frame and send it to writter
uint8_t
GenericAviSaveProcess::writeVideoChunk (uint32_t frame)
{
  uint8_t    	r;
  // CBR or CQ
  if (frame == 0)
  {
    encoding_gui->setCodec(_encode->getDisplayName())  ;
  if (!_encode->isDualPass ())
  {
                          guiSetPhasis (QT_TR_NOOP("Encoding"));
        }
  else
          {
                          guiSetPhasis (QT_TR_NOOP("2nd Pass"));
        }
  }
  // first read

preFilling:
  bitstream.cleanup(0);

  if (!_prefill || frame + _prefill < frametogo) 
	  r = _encode->encode(_prefill + frame, &bitstream);
  else
	  r = _encode->encode(frametogo - 1, &bitstream);

  if (!r)
	  return 0;

  if (!bitstream.len && frame == 0)
  {
	  _prefill++;
	  goto preFilling;
  }
  else if (frame == 0)
	  printf("Prefill: %u\n", _prefill);

  _videoFlag=bitstream.flags;

  // check for split
     // check for auto split
      // if so, we re-write the last I frame
      if(muxSize)
      {
              // we overshot the limit and it is a key frame
              // start a new chunk
              if(handleMuxSize() && (_videoFlag & AVI_KEY_FRAME))
              {		
                      uint8_t *data;
                      uint32_t dataLen=0;

                      _encode->hasExtraHeaderData( &dataLen,&data);	   
                      if(!reigniteChunk(dataLen,data)) return 0;
              }
        }
        encoding_gui->setFrame(frame,bitstream.len,bitstream.out_quantizer,frametogo);	
        // If we have several null B frames dont write them
        if(bitstream.len) _notnull=1;
	else	if( !_notnull)
	{
		printf("Frame : %lu dropped\n",frame);
	 	return 1;			 
	 }
         return writter->saveVideoFrame (bitstream.len, _videoFlag, vbuffer);

}


// EOF
