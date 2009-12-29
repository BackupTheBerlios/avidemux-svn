/***************************************************************************
                          adm_encffmpeg.h  -  description
                             -------------------
    begin                : Tue Sep 10 2002
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
#ifndef __ADM_encoder_ff__
#define __ADM_encoder_ff__



class EncoderFFMPEG:public Encoder
{

protected:

  ffmpegEncoder * _codec;
  uint8_t _internal;
  FF_CODEC_ID _id;
  uint32_t _fps;
  FFcodecSetting _settings;
public:
    EncoderFFMPEG (FF_CODEC_ID id, COMPRES_PARAMS * codecParam);
   ~EncoderFFMPEG ()
  {
    stop ();
  };				// can be called twice if needed ..
  virtual int getRequirements (void);
  virtual uint8_t isDualPass (void);
  virtual uint8_t configure (AVDMGenericVideoStream * instream, int useExistingLogFile);
  virtual uint8_t encode (uint32_t frame,ADMBitstream *out);
  virtual uint8_t setLogFile (const char *p, uint32_t fr);
  virtual uint8_t stop (void);
  virtual uint8_t startPass2 (void);
  virtual uint8_t startPass1 (void);
  virtual const char *getDisplayName (void)
  {
    return QT_TR_NOOP("LavCodec");
  }
  virtual const char *getCodecName (void);	//{return "DX50";}
  virtual const char *getFCCHandler (void)
  {
    return "divx";
  }


};
#endif
