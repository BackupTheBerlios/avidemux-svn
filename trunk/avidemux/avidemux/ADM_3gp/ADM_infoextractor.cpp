/***************************************************************************
                          ADM_infoextractor
                             -------------------
           - extract additionnal info from header (mp4/h263)                  
**************************************************************************/
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

#include "math.h"

#include <ADM_assert.h>
#include "ADM_library/default.h"
#include "ADM_editor/ADM_Video.h"


#include "ADM_library/fourcc.h"
#include "ADM_3gp/ADM_3gp.h"
#include "ADM_toolkit/toolkit.hxx"

#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_3GP
#include "ADM_toolkit/ADM_debug.h"

extern "C"
{
#include "../adm_lavcodec/common.h"
#include "../adm_lavcodec/bswap.h"
#include "../adm_lavcodec/bitstream.h"
}
/*
    Extract width & height from vol header passed as arg


*/
uint8_t extractMpeg4Info(uint8_t *data,uint32_t dataSize,uint32_t *w,uint32_t *h)
{
    // Search startcode
    uint8_t b;
    uint32_t idx=0;
    uint32_t mw,mh;
    uint32_t time_inc;
    
    mixDump(data,dataSize);
    printf("\n");
    while(1)
    {
        uint32_t startcode=0xffffffff;
        while(dataSize>2)
        {
            startcode=(startcode<<8)+data[idx];
            idx++;
            dataSize--;
            if((startcode&0xffffff)==1) break;
        }
        if(dataSize>2)
        {
            printf("Startcodec:%x\n",data[idx]);
            if((data[idx]&0xF0)==0x20) //VOL start
            {
                dataSize--;
                idx++;
                printf("VOL Header:\n");
                mixDump(data+idx,dataSize);printf("\n");
                // Here we go !
                GetBitContext s;
                init_get_bits( &s,data+idx, dataSize*8);
                //
                skip_bits1(&s); // Random access
                skip_bits(&s,8); // Obj type indication
                if(get_bits(&s,1)) // VO od 
                {
                      skip_bits(&s,4); // Ver
                      skip_bits(&s,3);  // Priority
                }
                if(get_bits(&s,4)==15) // custom A/R
                {
                      skip_bits(&s,8);
                      skip_bits(&s,8);
                }
                if(get_bits(&s,1)) // Vol control param
                {
                      skip_bits(&s,2);   //Chroma
                      skip_bits(&s,1);   // Low delay
                      if(!get_bits(&s,1)) // VBV Info
                      {
                        skip_bits(&s,16);
                        skip_bits(&s,16);
                        skip_bits(&s,16);
                        skip_bits(&s,15);
                        skip_bits(&s,16);
                      }
                  }
                 skip_bits(&s,2); //  Shape
                 skip_bits(&s,1); //  Marker
                 time_inc=get_bits(&s,16); // Time increment
                 skip_bits(&s,1); //  Marker
                 if(get_bits(&s,1)) // Fixed vop rate, compute how much bits needed
                 {
                    uint32_t in=time_inc;
                    uint32_t i=1;
                    in--;
                    while(in)
                    {
                        in>>=1;
                        if(in) i++;
                    }
                    skip_bits(&s,i);
                 }
                  skip_bits(&s,1); //  Marker
                  mw=get_bits(&s,13);
                  skip_bits(&s,1); //  Marker
                  mh=get_bits(&s,13);
                // /Here we go
                *h=mh;
                *w=mw;
                return 1;;
                // Free get bits ?
                // WTF ?
            }
            continue;
        }
        else
        {
            printf("No more startcode\n");
            // Free get bits ?
            return 0;
            
        }
    }
    
    return 0;
}
/*
        Extract H263 width & height from header

*/
uint8_t extractH263Info(uint8_t *data,uint32_t dataSize,uint32_t *w,uint32_t *h)
{
uint32_t val;
                GetBitContext s;
                init_get_bits( &s,data, dataSize*8);
                
                 mixDump(data,10);
                 val=get_bits(&s,16);
                 if(val)
                 {
                    printf("incorrect H263 header sync\n");
                    return 0;
                 }
                 val=get_bits(&s,6);
                 if(val!=0x20)
                 {
                    printf("incorrect H263 header sync (2)\n");
                    return 0;
                 }
                 //
                 skip_bits(&s,8); // timestamps in 30 fps tick
                 skip_bits(&s,1); // Marker
                 skip_bits(&s,1); // Id
                 skip_bits(&s,1); // Split
                 skip_bits(&s,1); // Document Camera indicator
                 skip_bits(&s,1); // Full Picture Freeze Release
                 val=get_bits(&s,3);
                 switch(val)
                 {
                   
                    case 1: *w=128;*h=96;return 1;break;
                    case 2: *w=176;*h=144;return 1;break;
                    case 6:
                    case 7:
                            printf("H263+:Todo\n");
                    default:
                        printf("Invalid format\n");return 0;break;
                 }
                 return 0;
}
//EOF
