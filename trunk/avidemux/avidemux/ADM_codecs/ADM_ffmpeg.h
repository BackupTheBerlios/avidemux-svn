/***************************************************************************
                          ADM_ffmpeg.h  -  description
                             -------------------
                             
                             
	Mpeg4 ****encoder******** using ffmpeg
	                             
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

#ifndef __FFMPEG_ENC__
#define   __FFMPEG_ENC__
typedef enum
{
  FF_MSMP4V3 = 1,
  FF_MPEG4 = 2,
  FF_MPEG1 = 3,
  FF_H263 = 4,
  FF_H263P = 5,
  FF_HUFF = 6,
  FF_FFV1 = 7,
  FF_MJPEG = 8,
  FF_MPEG2 = 9,
  FF_SNOW = 10
} FF_CODEC_ID;

/*
	Some of FFmpeg internal options as encoder
	User tunable
*/

#include "ADM_codecs/ADM_ffmpegConfig.h"

class ffmpegEncoder:public encoder
{
  protected:uint8_t _init;
  uint8_t _swap;
  AVCodecContext *_context;
  AVFrame _frame;
  FF_CODEC_ID _id;
  uint32_t _last_coded_frame;
  void mplayer_init (void);
  uint8_t initContext (void);
  uint8_t gopMpeg1 (void);
  uint32_t frameType (void);
  uint8_t encodePreamble (uint8_t * in);
  uint8_t _settingsPresence;
  FFcodecSetting _settings;
    public:uint8_t setConfig (FFcodecSetting * set);
  uint8_t setLogFile (const char *name);
    ffmpegEncoder (uint32_t width, uint32_t height, FF_CODEC_ID id);


    virtual ~ ffmpegEncoder ()
  {
    printf ("\n destroying..\n");
    stopEncoder ();
  }
  virtual uint8_t stopEncoder (void)
  {
    if (_init)
      {
	_init = 0;
	avcodec_close (_context);
	ADM_dealloc (_context);
	_context = NULL;
      }
    return 1;
  }
  virtual uint8_t init (uint32_t val, uint32_t fps1000);
  virtual uint8_t init (uint32_t val, uint32_t fps1000, uint8_t sw);
  virtual uint8_t getResult (void *ress);
  virtual uint8_t encode (ADMImage * in, uint8_t * out, uint32_t * len,
			  uint32_t * flags);
  uint8_t getExtraData (uint32_t * l, uint8_t ** d);
  virtual uint8_t setCustomMatrices (uint16_t * intra, uint16_t * inter);
  virtual uint8_t setGopSize (uint32_t size);	// !!! IT DOES NOT WORK, DONE TOO LATE!!!!
  	  uint32_t getCodedFrame(void);
};

class ffmpegEncoderCQ:public ffmpegEncoder
{
  protected:uint32_t _qual;
  uint8_t _vbr;
  FILE *_statfile;

    public:ffmpegEncoderCQ (uint32_t width, uint32_t height,
			    FF_CODEC_ID id):ffmpegEncoder (width, height, id)
  {
    _qual = 3;
    _vbr = 0;
    _statfile = NULL;
  };

  virtual uint8_t init (uint32_t val, uint32_t fps1000, uint8_t vbr = 0);

  virtual uint8_t encode (ADMImage * in,
			  uint8_t * out, uint32_t * len, uint32_t * flags);

  virtual ~ ffmpegEncoderCQ ()
  {
    stopEncoder ();
    if (_statfile)
      {
	fclose (_statfile);
      }
  }
};


class ffmpegEncoderCBR:public ffmpegEncoder
{
  protected:uint32_t _br;
  public:ffmpegEncoderCBR (uint32_t width, uint32_t height,
			   FF_CODEC_ID id):ffmpegEncoder (width, height, id)
  {
    _br = 1000;
  };

  virtual ~ ffmpegEncoderCBR ()
  {
    stopEncoder ();
  }
  virtual uint8_t init (uint32_t val, uint32_t fps1000);
  virtual uint8_t encode (ADMImage * in,
			  uint8_t * out, uint32_t * len, uint32_t * flags);


};

class ffmpegEncoderVBR:public ffmpegEncoderCQ
{
  protected:uint8_t encodeVBR (ADMImage * in,
			       uint8_t * out,
			       uint32_t * len,
			       uint32_t * flags,
			       uint16_t nq, uint8_t forcekey);
  uint8_t _internal;




    public:virtual uint8_t init (uint32_t val, uint32_t fps1000);
    ffmpegEncoderVBR (uint32_t width, uint32_t height,
		      FF_CODEC_ID id):ffmpegEncoderCQ (width, height, id)
  {
    _internal = 1;
  }
  ffmpegEncoderVBR (uint32_t width, uint32_t height, uint8_t inter,
		    FF_CODEC_ID id):ffmpegEncoderCQ (width, height, id)
  {
    _internal = inter;
  }


  virtual uint8_t encode (ADMImage * in,
			  uint8_t * out, uint32_t * len, uint32_t * flags);

  virtual ~ ffmpegEncoderVBR ();
};
class ffmpegEncoderVBRExternal:public ffmpegEncoderCQ
{
  protected:uint8_t encodeVBR (ADMImage * in, uint8_t * out,
			       uint32_t * len, uint32_t * flags,
			       uint16_t nq, uint8_t forcekey);



  public:virtual uint8_t init (uint32_t val, uint32_t fps1000);
  ffmpegEncoderVBRExternal (uint32_t width, uint32_t height,
			    FF_CODEC_ID id):ffmpegEncoderCQ (width, height,
							     id)
  {
  }



  virtual uint8_t encode (ADMImage * in,
			  uint8_t * out, uint32_t * len, uint32_t * flags);

  virtual ~ ffmpegEncoderVBRExternal ();
};

class ffmpegEncoderHuff:public ffmpegEncoder
{
  protected:public:ffmpegEncoderHuff (uint32_t width, uint32_t height,
				      FF_CODEC_ID id):ffmpegEncoder (width,
								     height,
								     id)
  {

  };

  virtual uint8_t init (uint32_t val, uint32_t fps1000, uint8_t vbr = 0);

  virtual uint8_t encode (ADMImage * in,
			  uint8_t * out, uint32_t * len, uint32_t * flags);

  virtual ~ ffmpegEncoderHuff ()
  {
    stopEncoder ();

  }
};

class ffmpegEncoderFFV1:public ffmpegEncoder
{
  protected:public:ffmpegEncoderFFV1 (uint32_t width, uint32_t height,
				      FF_CODEC_ID id):ffmpegEncoder (width,
								     height,
								     id)
  {

  };

  virtual uint8_t init (uint32_t val, uint32_t fps1000, uint8_t vbr = 0);

  virtual uint8_t encode (ADMImage * in,
			  uint8_t * out, uint32_t * len, uint32_t * flags);

  virtual ~ ffmpegEncoderFFV1 ()
  {
    stopEncoder ();

  }
};
class ffmpegEncoderFFMjpeg:public ffmpegEncoder
{
  protected:uint32_t _qual;

  public:ffmpegEncoderFFMjpeg (uint32_t width, uint32_t height,
			       FF_CODEC_ID id):ffmpegEncoder (width, height,
							      id)
  {
    _qual = 4;
  };

  virtual uint8_t init (uint32_t val, uint32_t fps1000, uint8_t vbr = 0);

  virtual uint8_t encode (ADMImage * in,
			  uint8_t * out, uint32_t * len, uint32_t * flags);

  virtual ~ ffmpegEncoderFFMjpeg ()
  {
    stopEncoder ();

  }
};




#endif
