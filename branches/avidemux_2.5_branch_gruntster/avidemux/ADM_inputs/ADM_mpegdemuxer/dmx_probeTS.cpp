/***************************************************************************
                        Probe for a stream                                              
                             
    
    copyright            : (C) 2005 by mean
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
#include "config.h"

#include "ADM_default.h"
#include "ADM_assert.h"



#include "DIA_fileSel.h"
#include "fourcc.h"
#include "ADM_userInterfaces/ADM_commonUI/DIA_working.h"



#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_MPEG
#include "ADM_osSupport/ADM_debug.h"

#include "dmx_demuxerPS.h"
#include "dmx_demuxerTS.h"
#include "dmx_identify.h"
#include "dmx_probe.h"
#include "ADM_userInterfaces/ADM_commonUI/DIA_busy.h"
#include "ADM_audio/ADM_mp3info.h"
#include "ADM_audio/ADM_a52info.h"
#include "ADM_audio/ADM_dcainfo.h"



#define MAX_PROBE 2000000 // Scans the first 2 meg
#define MIN_DETECT (10*1024) // Need this to say the stream is present
#define MAX_NB_PMT 50
#define MAX_NB_TRACK 50

//#define PROBE_TS_VERBOSE

typedef struct MPEG_PMT
{
	uint16_t programNumber;
	uint16_t tid;
} MPEG_PMT;

uint8_t dmx_probeTS(const char *file,  uint32_t *nbTracks,MPEG_TRACK **tracks,DMX_TYPE t);

static uint8_t dmx_probeTSBruteForce(const char *file,  uint32_t *nbTracks,MPEG_TRACK **tracks,DMX_TYPE type);
static uint8_t dmx_probeTsPatPmt(const char *file, uint32_t *nbTracks, MPEG_TRACK **tracks, DMX_TYPE type);
static int dmx_parsePat(dmx_demuxerTS *demuxer, int *nbPmt, MPEG_PMT *pmts, int maxPmt);
static int dmx_parsePmt(dmx_demuxerTS *demuxer, int pid, MPEG_TRACK *pmts, int *cur, int max);
static int parseProgramDescriptors(uint8_t *progDescBuffer, int progDescLength, int *esType);
static const char *dmx_streamType(uint32_t type,ADM_STREAM_TYPE *streamType);
static const char *dmx_streamTypeAsString(ADM_STREAM_TYPE st);
 
extern uint32_t mpegTsCRC(uint8_t *data, uint32_t len);

uint8_t runProbe(const char *file)
{
  uint32_t nb;
  MPEG_TRACK *t;
  return dmx_probeTsPatPmt(file, &nb,&t,DMX_MPG_TS);
}

uint8_t dmx_probeTS(const char *file,  uint32_t *nbTracks,MPEG_TRACK **tracks,DMX_TYPE type)
{
	// Try through PMT/PAT first
	if (!dmx_probeTsPatPmt(file, nbTracks, tracks, type))
	{
		printf("PAT/PMT Failed, using brute force\n");
		return dmx_probeTSBruteForce(file,nbTracks,tracks,type);
	}

	return 1;
}
/**************************************
****************************************************************
    Brute force pid scanning in mpeg TS file
    We seek all PES packets and store their PID and PES id
*****************************************************************/
#define MAX_FOUND_PID 100
#define CHECK(x) val=parser->read8i(); left--;if(val!=x) goto _next;
typedef struct myPid
{
  uint32_t pid;
  uint32_t pes;

}myPid;
/**
        \fn dmx_probeTSBruteForce
        \brief Extract PID by scanning the file and guessing what they are
        @param file Filename to scan
        @param *nbTrack # of tracks found
        @param **tracks Tracks found
        @param type demuxer type to use (TS or TS2)
        @return 1 on success, 0 on failure
*/
uint8_t dmx_probeTSBruteForce(const char *file, uint32_t *nbTracks,MPEG_TRACK **tracks,DMX_TYPE type)
{

  // Brute force indexing
  //
  // Build a dummy track
MPEG_TRACK dummy[TS_ALL_PID];
fileParser *parser;
uint32_t   foundPid=0;
myPid      allPid[MAX_FOUND_PID];
uint8_t    buffer[BUFFER_SIZE];
MpegAudioInfo mpegInfo; 

    dummy[0].pid=0x1; // should no be in use
    dummy[0].pes=0xE0;

        dmx_demuxerTS demuxer(TS_ALL_PID,dummy,0,type);
        if(!demuxer.open(file))
        {
          return 0;
        }

      demuxer.setProbeSize(MAX_PROBE);
      parser=demuxer.getParser();
      // And run

      uint32_t pid,left,isPayloadStart,cc,val;
      uint64_t abs;
      while(demuxer.readPacket(&pid,&left, &isPayloadStart,&abs,&cc))
      {
        if(isPayloadStart)
        {
            // Is it a PES type packet
            // it should then start by 0x0 0x0 0x1 PID

            CHECK(0);
            CHECK(0);
            CHECK(1);
            val=parser->read8i();
            left--;
            // Check it does not exist already
            int present=0;
            for(int i=0;i<foundPid;i++) if(pid==allPid[i].pid) {present=1;break;}
            if(!present)
            {
              allPid[foundPid].pes=val;
              allPid[foundPid].pid=pid;
              foundPid++;
            }
            ADM_assert(foundPid<MAX_FOUND_PID);
        } 
_next:
        parser->forward(left);
      }
      if(!foundPid)
      {
         printf("ProbeTS: No PES packet header found\n");
         return 0;
      }
      //****************************************
      // Build information from the found Pid
      //****************************************
      for(int i=0;i<foundPid;i++) printf("Pid : %04x Pes :%02x \n",allPid[i].pid,allPid[i].pes);

      // Search for a pid for video track
      //
      *tracks=new MPEG_TRACK[foundPid];
      MPEG_TRACK *trk=*tracks;
      uint32_t vPid=0,vIdx;
      uint32_t offset,fq,br,chan;

      for(int i=0;i<foundPid;i++)
      {
        if(allPid[i].pes>=0xE0 && allPid[i].pes<=0xEA)
        {
            vPid=trk[0].pes=allPid[i].pes;
            trk[0].pid=allPid[i].pid;
            trk[0].streamType=ADM_STREAM_MPEG_VIDEO;
            vIdx=i;
            break;
        }
      }
      if(!vPid)
      {
        delete [] trk;
        *tracks=0;
        printf("probeTs: No video track\n");
        return 0;
      }
      // Now build the other audio (?) tracks
      allPid[vIdx].pid=0;
      uint32_t start=1,code,id,read;
      for(int i=0;i<foundPid;i++)
      {
        code=allPid[i].pes;
        id=allPid[i].pid;

        if(!id) continue;

        if((code>=0xC0 && code <= 0xC9) || ((code==0xbd)&& (type==DMX_MPG_TS)) || ((code==0xfd)&& (type==DMX_MPG_TS2)))
        {
            demuxer.changePid(id,code);
            demuxer.setPos(0,0);
            read=demuxer.read(buffer,BUFFER_SIZE);
            if(read!=BUFFER_SIZE) continue;
            if(code>=0xC0 && code <= 0xC9) // Mpeg audio
            {
              if(getMpegFrameInfo(buffer,read,&mpegInfo,NULL,&offset))
                   {
                      if(mpegInfo.mode!=3)  trk[start].channels=2;
                          else  trk[start].channels=1;
 
                      trk[start].bitrate=mpegInfo.bitrate;
                      trk[start].pid=id;
                      trk[start].pes=code;
                      trk[start].streamType=ADM_STREAM_MPEG_AUDIO;
                      start++;

                    }
            }
            else // AC3
            {
                  if(ADM_AC3GetInfo(buffer,read,&fq,&br,&chan,&offset))
                  {
                          trk[start].channels=chan;
                          trk[start].bitrate=(8*br)/1000;
                          trk[start].pid=id;
                          trk[start].pes=0;
                          trk[start].streamType=ADM_STREAM_AC3;
                          start++;
                  }

            }

        }
      }
      *nbTracks=start;
      return 1;
}

/**
      \fn     dmx_probeTSPat(char *file, uint32_t *nbTracks,MPEG_TRACK **tracks)
      \brief  Try to extract info from a Mpeg TS file using PAT, PMT etc..
      @return 1 on success, 0 on failure
      @param file: File to scan
      @param *nbTrack : number of track found (out)
      @param **tracks : contains info about the tracks found (out)

*/
uint8_t dmx_probeTsPatPmt(const char *file, uint32_t *nbTracks, MPEG_TRACK **tracks, DMX_TYPE type)
{
	MPEG_TRACK dummy[TS_ALL_PID];
	fileParser *parser;
	uint32_t pid, left, cc, isPayloadStart;
	uint64_t abso;
	int nbPmt = 0, cur = 0;
	MPEG_PMT pmts[MAX_NB_PMT];
	MPEG_TRACK xtracks[MAX_NB_TRACK];

	dummy[0].pid=0x00; // should not be in use
	dummy[0].pes=0xE0;

	dmx_demuxerTS demuxer(TS_ALL_PID, dummy, 1, type);

	if (!demuxer.open(file))
		return 0;

	demuxer.setProbeSize(MAX_PROBE);

	parser = demuxer.getParser();

	while (demuxer.readPacket(&pid, &left, &isPayloadStart, &abso, &cc))
	{
		if (!isPayloadStart || left <= (9 + 4))
		{
			parser->forward(left);
			continue;
		}

		if (pid == 0)
		{
			aprintf("[TS] parse PAT: (pid: %d)\n", pid);
			dmx_parsePat(&demuxer, &nbPmt, pmts, MAX_NB_PMT);
		}
		else
		{
			aprintf("[TS] PMT found (pid: %d)\n", pid);

			for (int i = 0; i < nbPmt; i++)
			{
				if (pmts[i].tid == pid)
				{
					aprintf("[TS] parse PMT (pid: %d)\n", pid);
					dmx_parsePmt(&demuxer, pid, xtracks, &cur, MAX_NB_TRACK);

					break;
				}
			}
		}
	}

	if (!cur)
		return 0;

	for(int i = 0; i < cur; i++)
		printf("Tid: %04x Type: %d (%s)\n", xtracks[i].pid, xtracks[i].streamType, dmx_streamTypeAsString(xtracks[i].streamType));

	// Search first video track
	*tracks = new MPEG_TRACK[cur];
	int found = -1;

	for(int j = 0; j < cur; j++)
	{
		ADM_STREAM_TYPE type = xtracks[j].streamType;

		if(type == ADM_STREAM_MPEG_VIDEO || type==ADM_STREAM_MPEG4 || type==ADM_STREAM_H264)
		{
			found = j;
			break;
		}
	}

	if (found < 0)
	{
		printf("No video track\n");
		delete [] *tracks;

		return 0; 
	}

	memcpy(*tracks, &(xtracks[found]), sizeof(MPEG_TRACK));
	*nbTracks = 1;

	// Now do audio
	for (int j = 0; j < cur; j++)
	{
		MPEG_TRACK *t = &(xtracks[j]);
		ADM_STREAM_TYPE type = t->streamType;

		if (type != ADM_STREAM_MPEG_AUDIO && type != ADM_STREAM_AC3 && type != ADM_STREAM_AAC)
			continue; // Only mpega & AC3 for now

		switch(type)
		{
			case ADM_STREAM_AAC:
				t->pes = 0xb0;
			case ADM_STREAM_MPEG_AUDIO:
			case ADM_STREAM_AC3:
				memcpy(&((*tracks)[*nbTracks]), t, sizeof(MPEG_TRACK));
				ADM_assert(*nbTracks < cur);
				(*nbTracks)++;

				break;
			default:
				ADM_assert(0); 
		}
	}

	printf("Found %u tracks\n", *nbTracks);

	return 1;
}

/**
      \fn     dmx_probePat(dmx_demuxerTS *demuxer, uint32_t *nbPmt,MPEG_PMT *pmts,uint32_t maxPMT)
      \brief  Search for PAT and returns PMT info if found
      @return 1 on success, 0 on failure
      @param demuxer: mpegTS demuxer (input)
      @param *nbPmt : number of PMTS found (output)
      @param *pmts : contains info about the PMT found (out but must be allocated by caller)
      @param maxPMT : Maximum # of PMT we accept in pmts (in)

*/
int dmx_parsePat(dmx_demuxerTS *demuxer, int *nbPmt, MPEG_PMT *pmts, int maxPmt)
{
	uint8_t tableId, sectionNumber, lastSectionNumber;
	uint16_t sectionLength, progId;
	uint64_t startPos;
	fileParser *parser = demuxer->getParser();
	int entries;

	parser->getpos(&startPos);

	// Decode PSI header
	parser->read8i();	// Pointer field, can be ignored (?)

	tableId = parser->read8i();	// 0

	if (tableId != 0)
	{
		parser->setpos(startPos);
		return 0;
	}

	sectionLength = ((parser->read8i() & 0x03) << 8 ) | parser->read8i();	// 1 & 2
	parser->read16i();	// 3 & 4
	parser->read8i(); // 5
	sectionNumber = parser->read8i();	// 6
	lastSectionNumber = parser->read8i(); // 7	

	aprintf("[TS] parsePat: sectionLength: %d, section %d/%d @ %"LLU"\n", sectionLength, sectionNumber, lastSectionNumber, startPos);

	entries = (int)(sectionLength - 9) / 4;	// entries per section
	*nbPmt = 0;

	for (int i = 0; i < entries; i++)
	{
		bool foundProg = false;

		progId = (parser->read8i() << 8) | parser->read8i();

		for (int j = 0; j < *nbPmt; j++)
		{
			if (pmts[j].programNumber == progId)
			{
				foundProg = true;
				break;
			}
		}

		if (!foundProg && *nbPmt < maxPmt)
		{
			pmts[*nbPmt].programNumber = progId;
			pmts[*nbPmt].tid = ((parser->read8i() & 0x1F) << 8) | parser->read8i();

			aprintf("[TS] parsePat: programNumber: %d (%d/%d), PMT: %d\n", pmts[*nbPmt].programNumber, i + 1, entries, pmts[*nbPmt].tid);

			(*nbPmt)++;
		}
	}

	parser->setpos(startPos);

	return 1;
}

/**
      \fn     dmx_probePat(dmx_demuxerTS *demuxer, uint32_t *nbPmt,MPEG_PMT *pmts,uint32_t maxPMT)
      \brief  Search for PAT and returns PMT info if found
      @return 1 on success, 0 on failure
      @param demuxer: mpegTS demuxer (input)
      @param *nbPmt : number of PMTS found (output)
      @param *pmts : contains info about the PMT found (out but must be allocated by caller)
      @param maxPMT : Maximum # of PMT we accept in pmts (in)

*/
int dmx_parsePmt(dmx_demuxerTS *demuxer, int pid, MPEG_TRACK *pmts, int *cur, int max)
{
	uint8_t tableId, sectionNumber, lastSectionNumber;
	uint16_t sectionLength, progId, progDescLength;
	int32_t sectionBytes;
	uint64_t startPos;
	fileParser *parser = demuxer->getParser();
	int entries;

	parser->getpos(&startPos);

	// Decode PSI header
	parser->read8i();	// Pointer field, can be ignored (?)

	tableId = parser->read8i();	// 0

	if (tableId != 2)
	{
		parser->setpos(startPos);
		return 0;
	}

	sectionLength = ((parser->read8i() & 0xf) << 8 ) | parser->read8i();	// 1 & 2
	parser->read16i();	// 3 & 4
	parser->read8i(); // 5
	sectionNumber = parser->read8i();	// 6
	lastSectionNumber = parser->read8i(); // 7
	parser->read16i(); // 8 & 9
	progDescLength = ((parser->read8i() & 0xf) << 8 ) | parser->read8i();	// 10 & 11

	if (progDescLength > sectionLength - 9)
	{
		printf("[TS] parsePmt: Invalid progDesc length (%d/%d)\n", progDescLength, sectionLength - 9);
		parser->setpos(startPos);

		return 0;
	}

	parser->forward(progDescLength);

	sectionBytes = sectionLength - 13 - progDescLength;	

	while (sectionBytes >= 5)
	{
		int esType, esPid, esDescLength;
		const char *idString;
		ADM_STREAM_TYPE streamType;
		bool streamFound = false;

		esType = parser->read8i();
		esPid = ((parser->read8i() & 0x1f) << 8) | parser->read8i();
		esDescLength = ((parser->read8i() & 0xf) << 8) | parser->read8i();

		if (esDescLength > sectionBytes - 5)
		{
			printf("[TS] parsePmt: esDescLength too large %d > %d\n", esDescLength, sectionBytes - 5);
			return 0;
		}

		for (int i = 0; i < *cur; i++)
		{
			if (pmts[i].pid == esPid)
			{
				streamFound = true;
				break;
			}
		}

		if (esDescLength)
		{
			uint8_t esProgDesc[esDescLength];

			parser->read32(esDescLength, esProgDesc);
			parseProgramDescriptors(esProgDesc, esDescLength, &esType);
		}

		idString = dmx_streamType(esType, &streamType);

		if (!streamFound && *cur < max)
		{
			pmts[*cur].pid = esPid;
			pmts[*cur].streamType = streamType;

			if (streamType == ADM_STREAM_MPEG_AUDIO)
				pmts[*cur].pes = 0xC0;
			else if(streamType == ADM_STREAM_MPEG_VIDEO)
				pmts[*cur].pes = 0xE0;
			else
				pmts[*cur].pes = 0;

			(*cur)++;
		}

		sectionBytes -= 5 + esDescLength;

		aprintf("[TS] parsePmt: esPid: %d, esType: 0x%x (%s), esDescLength: %d, bytes left: %d\n", esPid, esType, idString, esDescLength, sectionBytes);
	}

	parser->setpos(startPos);

	return 1;
}

int parseProgramDescriptors(uint8_t *progDescBuffer, int progDescLength, int *esType)
{
	int i, descLen, len;

	i = 0;
	len = progDescLength;

	while (len > 2)
	{
		descLen = progDescBuffer[i + 1];
		aprintf("[TS]\tdescriptor id: 0x%x, len = %d (bytes left: %d)\n", progDescBuffer[i], descLen, len);

		if (descLen > len)
		{
			printf("[TS] Invalid descriptor length for id %02x: %d (bytes left: %d)\n", progDescBuffer[i], descLen, len);
			return 0;
		}

		if ((progDescBuffer[i] == 0x6a || progDescBuffer[i] == 0x7a) && *esType == 0x6)	// AC3
			*esType = 0x81;
		//else if (progDescBuffer[i] == 0x7b && *esType == 0x6)	// DTS
		else if (progDescBuffer[i] == 0x5)
		{
			if (descLen < 4)
				printf("[TS] Registration descriptor too short: %d\n", descLen);
			else
			{
				uint8_t *d = &progDescBuffer[i + 2];

				if (d[0] == 'A' && d[1] == 'C' && d[2] == '-' && d[3] == '3')	// AC3
					*esType = 0x81;
				//else if (d[0] == 'D' && d[1] == 'T' && d[2] == 'S' && d[3] == '1')	// DTS
				//else if (d[0] == 'D' && d[1] == 'T' && d[2] == 'S' && d[3] == '2')	// DTS
				//else if (d[0] == 'V' && d[1] == 'C' && d[2] == '-' && d[3] == '1')	// VC-1
			}
		}

		len -= 2 + descLen;
		i += 2 + descLen;
	}

	return 1;
}

const char *dmx_streamType(uint32_t type,ADM_STREAM_TYPE *streamType)
{
 switch(type)
 {
   case 1:case 2: *streamType=ADM_STREAM_MPEG_VIDEO;return "Mpeg Video";
   case 3:case 4: *streamType=ADM_STREAM_MPEG_AUDIO;return "Mpeg Audio";
   case 0x11: case 0xF:        *streamType=ADM_STREAM_AAC;return "AAC  Audio";
   case 0x10:        *streamType=ADM_STREAM_MPEG4;return "MP4 Video";
   case 0x1B: *streamType=ADM_STREAM_H264;return "H264";
   case 0x81: *streamType=ADM_STREAM_AC3;return "Private (AC3?)";
 }
 *streamType=ADM_STREAM_UNKNOWN;
  return "???";
}

/**
      \fn dmx_streamTypeAsSTring
      \brief returns stream type as a printable string
*/
static const char *dmx_streamTypeAsString(ADM_STREAM_TYPE st)
{
#define MST(x,y) case x: return #y;
  switch(st)
  {
  MST(ADM_STREAM_UNKNOWN,UNKNOWN)
  MST(ADM_STREAM_MPEG_VIDEO,MPEG12VIDEO)
  MST(ADM_STREAM_MPEG_AUDIO,MPEG12AUDIO)
  MST(ADM_STREAM_AC3,AC3)
  MST(ADM_STREAM_DTS,DTS)
  MST(ADM_STREAM_H264,H264)
  MST(ADM_STREAM_MPEG4,MPEG4)
  MST(ADM_STREAM_AAC,AAC)
    
  }
  return "???";
  
}
/****EOF**/
