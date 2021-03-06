/***************************************************************************
                          audiofilter_normalize
                             -------------------
    
    copyright            : (C) 2002/2006 by mean
    email                : fixounet@free.fr
    
    Compute the ratio so that the maximum is at -3db
    
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

#include <ADM_assert.h>

#include "avifmt.h"
#include "avifmt2.h"

#include "ADM_toolkit/toolkit.hxx"
#include "ADM_dialog/DIA_working.h"
#include "audioeng_process.h"
#include "audiofilter_normalize_param.h"
#include "audiofilter_normalize.h"

#include "ADM_audio/aviaudio.hxx"
extern AVDMGenericAudioStream *currentaudiostream;

// Ctor
//__________

AUDMAudioFilterNormalize::AUDMAudioFilterNormalize(AUDMAudioFilter * instream,GAINparam *param):AUDMAudioFilter (instream)
{
  float db_out;
    // nothing special here...
  switch(param->mode)
  {
    case ADM_NO_GAIN: _ratio=1;_scanned=1;printf("[Gain] Gain of 1.0\n");break; 
    case ADM_GAIN_AUTOMATIC: _ratio=1;_scanned=0;printf("[Gain] Automatic gain\n");break;
    case ADM_GAIN_MANUAL: 
                _scanned=1;
                db_out =  param->gain10/100.0; // Dbout is in DB (!)
#ifndef CYG_MANGLING
#ifdef ADM_BSD_FAMILY
		_ratio = powf(10.0,db_out);
#else
                _ratio = pow10f(db_out);
#endif
#else
		_ratio= pow(10,db_out);
#endif
                printf("[Gain] %f db (p=%d)\n", (float)(param->gain10)/10.,param->gain10);
                printf("[Gain] Linear ratio of : %03.3f\n", _ratio);
  }
    _previous->rewind();
};

AUDMAudioFilterNormalize::~AUDMAudioFilterNormalize()
{

}
//
// For normalize, we scan the input stream
// to check for maximum value
//___________________________________________
uint8_t AUDMAudioFilterNormalize::preprocess(void)
{

    int16_t *ptr;
    uint32_t scanned = 0, ch = 0;
    AUD_Status status;
    _ratio = 0;

    uint32_t percent=0;
    uint32_t current=0,llength=0;
    float max = 0;
    _previous->rewind();

    printf("\n Seeking for maximum value, that can take a while\n");

      llength=currentaudiostream->getLength()  / _wavHeader.byterate * _wavHeader.frequency * _wavHeader.channels;
    
      DIA_working *windowWorking=new DIA_working(_("Normalize : Scanning"));

      while (1)
      {
          int ready=_previous->fill(AUD_PROCESS_BUFFER_SIZE>>2,_incomingBuffer,&status);
          if(!ready)
          {
            if(status==AUD_END_OF_STREAM) 
            {
              break; 
            }
           else 
            {
              printf("Unknown cause : %d\n",status);
              ADM_assert(0); 
            }
          }
          ADM_assert(!(ready %_wavHeader.channels));
          
          int index=0;
          float current;
          int sample= ready /_wavHeader.channels;
          for(int j=0;j<sample;j++) {
            current = 0;
            for(int chan=0;chan<_wavHeader.channels;chan++)
            {
              current+=fabs(_incomingBuffer[index++]);
            }
            if(current>max) max=current;
          }
	  if(!windowWorking->isAlive() )
    	  {
    	    // cannot be aborted
            delete windowWorking;
            windowWorking=new DIA_working(_("Normalize : Scanning"));
          }
	  scanned+=ready;
	  if(scanned<llength)
            windowWorking->update(scanned,llength);
      }
    delete windowWorking;

    _previous->rewind();
    double db_in, db_out;
    db_out =  -3;
    db_in = max;
    if (db_in>0.001)
	db_in = 20 / log(10) * log(db_in);
    else
	db_in = 0;

    printf("--> %2.2f db / %2.2f \n", db_in, db_out);

    // search ratio
    _ratio=1;
    if(max)
    {

    _ratio = expf(db_out * (1.0 / (20.0 / logf(10.0))));
    _ratio = _ratio / max;
    printf("\n Using ratio of : %f\n", _ratio);
     }
    _scanned = 1;
    _previous->rewind();
    return 1;
}
//
//___________________________________________
uint32_t AUDMAudioFilterNormalize::fill( uint32_t max, float * buffer,AUD_Status *status)
{
    uint32_t rd, i, j,rd2;

    *status=AUD_OK;
    if(!_scanned) preprocess();
    rd = _previous->fill(max, _incomingBuffer,status);
    if(!rd)
    {
      if(*status==AUD_END_OF_STREAM) return 0;
      ADM_assert(0);
    }
    float *in,*out,tmp;
    for (i = 0; i < rd; i++)
    {
      tmp=_incomingBuffer[i];
      tmp*=_ratio;
      buffer[i]=tmp;
    }
    return rd;
};
//EOF

