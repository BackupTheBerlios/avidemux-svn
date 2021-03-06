/***************************************************************************
                          audioeng_ffmp2.h  -  description
                             -------------------
    begin                : Sun Dec 1 2002
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
#ifndef FFaudioMP2
#define FFaudioMP2
 
 //_____________________________________________
class AVDMProcessAudio_FFmpeg : public AVDMBufferedAudioStream
{
protected:
         uint32_t _mode;
         uint32_t _bitrate;
         uint32_t _fq;
         uint32_t _size;
				 uint8_t  _bufferin[PROCESS_BUFFER_SIZE];
         uint32_t 	grab(uint8_t *obuffer);
		     void  	*_context;
		     uint8_t	*_bfer;
		     int			_codec;
public:
				uint8_t	init(												 uint32_t bitrate);		
				~AVDMProcessAudio_FFmpeg();				
		        AVDMProcessAudio_FFmpeg(int codecid,AVDMGenericAudioStream *instream	);		
     		
};
class AVDMProcessAudio_FFmp2 : public AVDMProcessAudio_FFmpeg
{
protected:
        
		   	
public:
			
				~AVDMProcessAudio_FFmp2();				
		  	AVDMProcessAudio_FFmp2(AVDMGenericAudioStream *instream	);		
     		
};
class AVDMProcessAudio_FFAC3 : public AVDMProcessAudio_FFmpeg
{
protected:
                 
		   	
public:
			
				~AVDMProcessAudio_FFAC3();				
		  	AVDMProcessAudio_FFAC3(AVDMGenericAudioStream *instream	);		
     		
};
#endif
