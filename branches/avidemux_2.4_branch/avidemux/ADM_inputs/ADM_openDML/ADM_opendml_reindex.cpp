//
// C++ Implementation: ADM_odml_regular
//
// Description: This file reindexes completly an avi file
//	It is used when all other indexer have failed
// 	Main goal : Big file support, like the ones generated by
//	mencoder
//
//
// Author: mean <fixounet@free.fr>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "math.h"
#include <ADM_assert.h>
#ifdef ADM_DEBUG
//	#define ODML_INDEX_VERBOSE
#endif

#include "default.h"
#include "ADM_editor/ADM_Video.h"


#include "fourcc.h"
#include "ADM_openDML/ADM_openDML.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_dialog/DIA_working.h"

#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_ODML
#include "ADM_osSupport/ADM_debug.h"
#if 0
uint8_t 	OpenDMLHeader::indexReindex(uint32_t vidTrack,uint32_t audioTrack,uint32_t audioTrackNumber)
{
uint32_t fcc,len,shortFCC,track;
uint64_t pos;
uint32_t achunk=0,vchunk=0;
uint8_t end=0;
uint32_t shortTotal,shortCur,highFCC;

	UNUSED_ARG(audioTrackNumber);
	UNUSED_ARG(vidTrack);


	printf("Re-indexing the file, it can take a while....\n");
	// jump to the movi chunk
	if(!_movi.offset) 
	{
		printf("No movi chunk!\n");
		return 0;
	}
	fseeko(_fd,_movi.offset,SEEK_SET);
	pos=ftello(_fd);
#define MAXX 4*30*3600	
	// we build ~ 4 hour worth of video and audio
	_idx=new odmlIndex[MAXX];
	_audioIdx=new odmlIndex[MAXX];
	shortTotal=(_fileSize)>>20;
	
	DIA_working *working=new DIA_working("Indexing...");
	
	// 
	while(pos<_fileSize-20 && achunk<MAXX-1 && vchunk < MAXX-1 && !end)
	{
		
		shortCur=pos>>20;
		
		working->update(shortCur,shortTotal);
		if(!working->isAlive())
		{
			delete working;
			return 0;
		}
		
		
		fcc=read32();
		len=read32();
		aprintf("%s size:%lu (%llx/%llx)\n",fourCC::tostring(fcc),len,pos,ftello(_fd));
		shortFCC=fcc>>16;
		track=((fcc>>8)&0xff)-'0'; 
		pos+=8;
		highFCC=fcc&0xffff;
		if(highFCC==MKFCC('i','x',0,0))
		{
					printf("OpenDML index found, skipping it\n");
					fseeko(_fd,len,SEEK_CUR);
					pos+=len;
					continue;
		}
				
		
		switch(shortFCC)
		{
			case MKFCC('d','c',0,0):	// video tracks
			case MKFCC('d','b',0,0):
					aprintf("\tVid.\n");
					_idx[vchunk].offset=pos;
					_idx[vchunk].size=len;
					_idx[vchunk].intra=AVI_KEY_FRAME;
					vchunk++;	
					if(len&1) len++;				
					fseeko(_fd,len,SEEK_CUR);
					pos+=len;
					break;
			case MKFCC('w','b',0,0):
					if(track==audioTrack)
					{
					aprintf("\tAud.\n");
					_audioIdx[achunk].offset=pos;
					_audioIdx[achunk].size=len;					
					achunk++;
					}
					if(len&1) len++;
					fseeko(_fd,len,SEEK_CUR);					
					pos+=len;
					
					break;
			default:
					printf("\n unknown fcc: ");fourCC::print(fcc);printf("\n");
					printf("\n ShortFcc: ");fourCC::print(shortFCC);printf("\n");
					printf("\n HighFCC: ");fourCC::print(highFCC);printf("\n");
					end=1;
					break;
		
		}
			
	}
	printf("%lu audio and %lu video chunks found\n",(unsigned long int)vchunk,(unsigned long int)achunk);
	// update stuff
	_videostream.dwLength= _mainaviheader.dwTotalFrames=vchunk;
	_nbAudioChunk=achunk;
	delete working;
	return 1;
	
}
#endif
