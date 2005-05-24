/***************************************************************************
                        2nd gen indexer                                                 
                             
    
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ADM_library/default.h"
#include <ADM_assert.h>


#include "ADM_toolkit/toolkit.hxx"
#include "ADM_toolkit/filesel.h"
#include "ADM_dialog/DIA_working.h"



#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_MPEG
#include "ADM_toolkit/ADM_debug.h"
#include "dmx_demuxerEs.h"


static uint8_t Push(uint32_t ftype,dmx_demuxer *demuxer);
static uint8_t gopDump(FILE *fd,dmx_demuxer *demuxer);

uint8_t dmx_indexer(char *mpeg,char *file);

static const uint32_t FPS[16]={
                0,                      // 0
                23976,          // 1 (23.976 fps) - FILM
                24000,          // 2 (24.000 fps)
                25000,          // 3 (25.000 fps) - PAL
                29970,          // 4 (29.970 fps) - NTSC
                30000,          // 5 (30.000 fps)
                50000,          // 6 (50.000 fps) - PAL noninterlaced
                59940,          // 7 (59.940 fps) - NTSC noninterlaced
                60000,          // 8 (60.000 fps)
                0,                      // 9
                0,                      // 10
                0,                      // 11
                0,                      // 12
                0,                      // 13
                0,                      // 14
                0                       // 15
        };

static const char Type[5]={'X','I','P','B','D'};

#define MAX_PUSHED 50

static uint32_t nbPushed,nbGop,nbImage;

static uint32_t frameType[MAX_PUSHED],frameSize[MAX_PUSHED];

static uint64_t gopStartAbs,gopStartRel;
/*
        Index the incoming mpeg file

*/
uint8_t dmx_indexer(char *mpeg,char *file)
{
        DIA_working *work;
        dmx_demuxer *demuxer;

        uint8_t streamid;                
        char *realname=PathCanonize(mpeg);
        FILE *out;        
        
        uint32_t update=0;

       
        demuxer=new dmx_demuxerES;
        demuxer->open(realname);
        out=fopen(file,"wt");
        if(!out)
        {
                        printf("\n Error : cannot open index !");
                        delete demuxer;
                        delete [] realname;
                        return 0;
        }
        fprintf(out,"ADMX0003\n");
        fprintf(out,"Type     : %c\n",'E'); // ES for now
        fprintf(out,"File     : %s\n",realname);
        fprintf(out,"Image    : %c\n",'P'); // Progressive
        fprintf(out,"Picture  : %lu x %lu %lu fps\n",0,0,0); // width...
        fprintf(out,"Nb Gop   : %lu \n",0); // width...
        fprintf(out,"Nb Images: %lu \n",0); // width...
        fprintf(out,"Nb Audio : %lu\n",0); 
        fprintf(out,"Streams  : V0000:0000\n"); 
        
        fprintf(out,"# NGop NImg nbImg Pos rel type:size type:size\n"); 

        uint8_t  firstGop=1;
        uint8_t  grabbing=0,seq_found=0;
        uint32_t imageW, imageH, imageFps;
        uint32_t total_frame=0,val;
        work=new DIA_working("Indexing mpeg");

        nbPushed=0;
        nbGop=0;
        nbImage=0;

        gopStartAbs=0;
        gopStartRel=0;

        while(1)
        {
                                if(!demuxer->sync(&streamid)) break;                                
                                update++;
                                if(update>100)
                                        {
                                                uint64_t pos,rel;
                                                demuxer->getPos(&pos,&rel);
                                                if(work->update(pos>>16,demuxer->getSize()>>16))
                                                {
                                                        // abort;
                                                        goto stop_found;

                                                }
                                                update=0;
                                        }
                                switch(streamid)
                                        {
                                        /* Useless apparently
                                                        case 0xB9: // sequence end
                                                        printf("\n End seq\n");
                                                        goto stop_found;
                                                        break;
                                        */
                                        case 0xB3: // sequence start
                                                demuxer->stamp();
                                                grabbing=1;    
                                                if(!firstGop)    
                                                {                                        
                                                        gopDump(out,demuxer);                        
                                                }
                                                else
                                                {
                                                        demuxer->getPos(&gopStartAbs,&gopStartRel);
                                                        firstGop=0;
                                                }
                                                if(seq_found)
                                                {
                                                        demuxer->forward(8);
                                                        break;
                                                }
                                                seq_found=1;
                                                val=demuxer->read32i();
                                                imageW=val>>20;
                                                imageW=((imageW+15)&~15);
                                                imageH= (((val>>8) & 0xfff)+15)& ~15;
                                                imageFps= FPS[val & 0xf];
                                                demuxer->forward(4);
                                                break;
                                        case 0xb8: // GOP
                                                //      printf("GOP\n");
                                                uint32_t gop;   
                                                if(!seq_found) continue;
                                                if(grabbing) 
                                                {
                                                        grabbing=0;        
                                                        continue;
                                                }
                                                if(!firstGop)
                                                        gopDump(out,demuxer);
                                                firstGop=0;
                                                demuxer->stamp();
                                                demuxer->forward(3);    
                                                gop=demuxer->read8i();
                                                
                                                break;
                                        case 0x00 : // picture
                                                //  printf("PIC\n");
                                                uint8_t ftype;
                                                uint8_t temporal_ref;
                                                
                                                if(!seq_found) continue;
                                                grabbing=0;
                                                total_frame++;
                                                val=demuxer->read16i();
                                                temporal_ref=val>>6;
                                                ftype=7 & (val>>3);
                                             
                                                Push(ftype,demuxer);
                                                demuxer->stamp();
                                             
                                                break;
                                        default:
                                           break;
                                        }
                }
stop_found:
          if(nbPushed)  gopDump(out,demuxer);

          fseeko(out,0,SEEK_SET);

        // Update header
        fprintf(out,"ADMX0003\n");
        fprintf(out,"Type     : %c\n",'E'); // ES for now
        fprintf(out,"File     : %s\n",realname);
        fprintf(out,"Image    : %c\n",'P'); // Progressive
        fprintf(out,"Picture  : %lu x %lu %lu fps\n",imageW,imageH,imageFps); // width...
        fprintf(out,"Nb Gop   : %lu \n",nbGop); // width...
        fprintf(out,"Nb Images: %lu \n",nbImage); // width...
        fprintf(out,"Nb Audio : %lu\n",0); 
        fprintf(out,"Streams  : V0000:0000\n"); 

          fclose(out);
          delete work;
          delete demuxer;
          delete [] realname;
          return 1;
}
/**** Push a frame**/
uint8_t Push(uint32_t ftype,dmx_demuxer *demuxer)
{
        frameType[nbPushed]=ftype;
        if(nbPushed)
                frameSize[nbPushed-1]=demuxer->elapsed();
        nbPushed++;
        ADM_assert(nbPushed<MAX_PUSHED);
        return 1;

}
/*** Pop the whold gop ***/
uint8_t gopDump(FILE *fd,dmx_demuxer *demuxer)
{
        if(!nbPushed) return 1;

        frameSize[nbPushed]=demuxer->elapsed();
        fprintf(fd,"%03lu %06lu %02lu %llu %llu ",nbGop,nbImage,nbPushed,gopStartAbs,gopStartRel);

        for(uint32_t i=0;i<nbPushed;i++) fprintf(fd,"%c:%06lu ",Type[frameType[i]],frameSize[i]);
        
        fprintf(fd,"\n");

        nbGop++;
        nbImage+=nbPushed;
        nbPushed=0;
        demuxer->getPos(&gopStartAbs,&gopStartRel);
        return 1;
        
}
