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
typedef struct FFMPEGConfig
{
		COMPRES_PARAMS generic;
		FFcodecSetting specific;
}FFMPEGConfig;


class EncoderFFMPEG:public Encoder
{

protected:

  ffmpegEncoder 		*_codec;
  uint8_t			_internal;
  FF_CODEC_ID			_id;
  uint32_t			_fps;
  FFcodecSetting 		_settings;
public:
    EncoderFFMPEG (FF_CODEC_ID id ,FFMPEGConfig *config);
	~EncoderFFMPEG() { stop();}; // can be called twice if needed ..
  virtual uint8_t isDualPass (void);
  virtual uint8_t configure (AVDMGenericVideoStream * instream);
  virtual uint8_t encode (uint32_t frame, uint32_t * len, uint8_t * out,
			  uint32_t * flags);
  virtual uint8_t setLogFile (const char *p, uint32_t fr);
  virtual uint8_t stop (void);
  virtual uint8_t startPass2 (void);
  virtual uint8_t startPass1 (void);
  virtual const char *getDisplayName(void ) {return "LavCodec";}
  virtual const char *getCodecName(void ) ; //{return "DX50";}
  virtual const char *getFCCHandler(void ) {return "divx";}
  virtual uint8_t    getLastQz( void) ;

};

class EncoderFFMPEGHuff:public EncoderFFMPEG
{

protected:


public:
    	EncoderFFMPEGHuff (FFMPEGConfig *config);
	~EncoderFFMPEGHuff() { stop();}; // can be called twice if needed ..
  virtual uint8_t isDualPass (void) { return 0;};
  virtual uint8_t configure (AVDMGenericVideoStream * instream);
  virtual uint8_t setLogFile (const char *p, uint32_t fr)  { UNUSED_ARG(p); UNUSED_ARG(fr); return 1;};
  virtual uint8_t startPass2 (void) { return 1;};
  virtual uint8_t startPass1 (void) { return 1;};
  virtual const char *getDisplayName(void ) {return "LavCodec HUFFYUV";}
  virtual const char *getFCCHandler(void ) {return "HFYU";}
  virtual uint8_t encode (uint32_t frame, uint32_t * len, uint8_t * out,
			  uint32_t * flags);
  virtual uint8_t hasExtraHeaderData( uint32_t *l,uint8_t **data);

};

class EncoderFFMPEGFFV1:public EncoderFFMPEG
{

protected:


public:
    	EncoderFFMPEGFFV1 (FFMPEGConfig *config);
	~EncoderFFMPEGFFV1() { stop();}; // can be called twice if needed ..
  virtual uint8_t isDualPass (void) { return 0;};
  virtual uint8_t configure (AVDMGenericVideoStream * instream);
  virtual uint8_t setLogFile (const char *p, uint32_t fr)  { UNUSED_ARG(p); UNUSED_ARG(fr); return 1;};
  virtual uint8_t startPass2 (void) { return 1;};
  virtual uint8_t startPass1 (void) { return 1;};
  virtual const char *getDisplayName(void ) {return "LavCodec FFV1";}
  virtual const char *getFCCHandler(void ) {return "FFV1";}
  virtual uint8_t encode (uint32_t frame, uint32_t * len, uint8_t * out,
			  uint32_t * flags);
  virtual uint8_t hasExtraHeaderData( uint32_t *l,uint8_t **data);

};


class EncoderFFMPEGMpeg1:public EncoderFFMPEG
{
private:
	uint8_t  setMatrix( void);
	uint8_t	_lastQz;
	uint32_t	_lastBitrate;

public:


  	uint32_t			_totalframe;
	uint32_t			_pass1Done;
	uint8_t  			updateStats (uint32_t len);
	uint8_t				_use_xvid_ratecontrol;	

public:
    			EncoderFFMPEGMpeg1 ( FF_CODEC_ID id, FFMPEGConfig *config);
  virtual		~EncoderFFMPEGMpeg1(); // can be called twice if needed ..
  virtual uint8_t isDualPass (void);
  virtual uint8_t configure (AVDMGenericVideoStream * instream);
  virtual uint8_t encode (uint32_t frame, uint32_t * len, uint8_t * out,
			  uint32_t * flags);
  virtual uint8_t setLogFile (const char *p, uint32_t fr);
  virtual uint8_t stop (void);
  virtual uint8_t startPass2 (void);
  virtual uint8_t startPass1 (void);
  	   //uint8_t getQuantizer( void );
	   //uint32_t getBitrate( void );
 virtual uint8_t    getLastQz( void) ;	   


};


#endif
