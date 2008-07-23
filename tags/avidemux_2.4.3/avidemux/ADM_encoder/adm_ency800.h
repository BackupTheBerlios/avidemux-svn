/***************************************************************************
                          adm_ency800.h  -  description
                             -------------------
    begin                : Tue Jul 23 2002
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

#ifndef __ADM_encoder_y800__
#define __ADM_encoder_y800__

#ifdef USE_FFMPEG
#include "ADM_encoder/adm_ency800_param.h"

class EncoderY800:public Encoder
{

protected:
  ffmpegEncoderFFY800 * _codec;
public:
    EncoderY800 (COMPRES_PARAMS * conf);
  virtual uint8_t isDualPass (void)
  {
    return 0;
  };				// mjpeg is always monopass
  virtual uint8_t configure (AVDMGenericVideoStream * instream);
  virtual uint8_t encode (uint32_t frame, ADMBitstream *out);
  virtual uint8_t setLogFile (const char *p, uint32_t fr);	// for dual pass only
  virtual uint8_t stop (void);
  virtual uint8_t startPass2 (void)
  {
    assert (0);
  };				// for dual pass only
  virtual uint8_t startPass1 (void)
  {
    assert (0);
  };				// for dual pass only
  virtual const char *getCodecName (void)
  {
    return "Y800";
  }
  virtual const char *getFCCHandler (void)
  {
    return "Y800";
  }
  virtual const char *getDisplayName (void)
  {
    return QT_TR_NOOP("Y800");
  }
};


#endif
#endif
