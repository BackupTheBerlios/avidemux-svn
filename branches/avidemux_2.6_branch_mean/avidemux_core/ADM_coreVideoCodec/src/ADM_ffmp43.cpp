/***************************************************************************
    \file ADM_ffmp43
    \brief Decoders using lavcodec
    \author mean & all (c) 2002-2010
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

extern "C" {
#include "ADM_lavcodec.h"
}
#include "ADM_default.h"

#include "ADM_codec.h"
#include "ADM_ffmp43.h"
#include "DIA_coreToolkit.h"
//#include "ADM_videoInfoExtractor.h"

extern int ADM_cpu_num_processors(void);
extern "C"
{
    static void ADM_releaseBuffer(struct AVCodecContext *avctx, AVFrame *pic);
    static int  ADM_getBuffer(AVCodecContext *avctx, AVFrame *pic);
}

#define aprintf(...) {}

#define WRAP_Open_Template(funcz,argz,display,codecid) \
{\
AVCodec *codec=funcz(argz);\
if(!codec) {GUI_Error_HIG("Codec",QT_TR_NOOP("Internal error finding codec"display));ADM_assert(0);} \
  codecId=codecid; \
  _context->workaround_bugs=1*FF_BUG_AUTODETECT +0*FF_BUG_NO_PADDING; \
  _context->error_concealment=3; \
  if (avcodec_open(_context, codec) < 0)  \
                      { \
                                        printf("[lavc] Decoder init: "display" video decoder failed!\n"); \
                                        GUI_Error_HIG("Codec","Internal error opening "display); \
                                        ADM_assert(0); \
                                } \
                                else \
                                { \
                                        printf("[lavc] Decoder init: "display" video decoder initialized! (%s)\n",codec->long_name); \
                                } \
}

#define WRAP_Open(x)            {WRAP_Open_Template(avcodec_find_decoder,x,#x,x);}
#define WRAP_OpenByName(x,y)    {WRAP_Open_Template(avcodec_find_decoder_by_name,#x,#x,y);}


//****************************
extern uint8_t DIA_lavDecoder (uint32_t * swapUv, uint32_t * showU);
extern "C"
{
  int av_is_voppacked (AVCodecContext * avctx, int *vop_packed, int *gmc,
		       int *qpel);
};
/**
    \fn clonePic
    \brief Convert AvFrame to ADMImage
*/
uint8_t decoderFF::clonePic (AVFrame * src, ADMImage * out)
{
  uint32_t    u,v;
  ADM_assert(out->isRef());
  ADMImageRef *ref=out->castToRef();
  ref->_planes[0] = (uint8_t *) src->data[0];
  ref->_planeStride[0] = src->linesize[0];
  if (_swapUV)
    {
      u = 1;
      v = 2;
    }
  else
    {
      u = 2;
      v = 1;
    }
  ref->_planes[1] = (uint8_t *) src->data[u];
  ref->_planeStride[1] = src->linesize[u];

  ref->_planes[2] = (uint8_t *) src->data[v];
  ref->_planeStride[2] = src->linesize[v];

  _lastQ = 0;			//_context->quality;
  out->_Qp = (src->quality * 32) / FF_LAMBDA_MAX;
  out->flags = frameType ();

  // Quant ?
  if (src->qstride && src->qscale_table && codecId != CODEC_ID_H264)
    {
      out->quant = (uint8_t *) src->qscale_table;
      out->_qStride = src->qstride;
      out->_qSize = (_w + 15) >> 4;
      out->_qSize *= (_h + 15) >> 4;	// FixME?
    }
  else
    {
      out->_qSize = out->_qStride = 0;
      out->quant = NULL;
    }
    //printf("[LAVC] Old pts :%"LLD" new pts :%"LLD"\n",out->Pts, (uint64_t)(src->reordered_opaque));
    //printf("[LAVC] pts: %"LLU"\n",src->pts);
    out->Pts= (uint64_t)(src->reordered_opaque);
 
    return 1;
}
/**
        \fn decoderMultiThread
        \brief Enabled multitheaded decoder if possible
*/
void decoderFF::decoderMultiThread (void)
{
  uint32_t threads = 0;

 // prefs->get(FEATURE_THREADING_LAVC, &threads);
#warning Fixme
    threads=1;
  if (threads == 0)
	  threads = ADM_cpu_num_processors();

  if (threads == 1)
	  threads = 0;

  if (threads)
  {
      printf ("[lavc] Enabling MT decoder with %u threads\n", threads);

      if (avcodec_thread_init (_context, threads) == -1)
	      printf ("[lavc] Failed!!\n");
	  else
          _usingMT = 1;
  }
}
uint8_t decoderFF::getPARWidth (void)
{
  if(!_context->sample_aspect_ratio.num) return 1;
  return _context->sample_aspect_ratio.num;
}
uint8_t decoderFF::getPARHeight (void)
{
  if(!_context->sample_aspect_ratio.den) return 1;
  return _context->sample_aspect_ratio.den;

}

//________________________________________________
bool  decoderFF::setParam (void)
{
  DIA_lavDecoder (&_swapUV, &_showMv);
  return true;			// no param for ffmpeg
}

//-------------------------------
decoderFF::decoderFF (uint32_t w, uint32_t h,uint32_t fcc, uint32_t extraDataLen, uint8_t *extraData,uint32_t bpp)
            :decoders (w, h,fcc,extraDataLen,extraData,bpp)
{
  codecId = 0;
//                              memset(&_context,0,sizeof(_context));
  _allowNull = 0;
  _gmc = 0;
  _context = NULL;
  _refCopy = 0;
  _usingMT = 0;
#if LIBAVCODEC_BUILD >= 4624
  _context = avcodec_alloc_context ();
#else
  _context = new AVCodecContext;
  memset (_context, 0, sizeof (AVCodecContext));
#endif
  ADM_assert (_context);
  memset (&_frame, 0, sizeof (_frame));

  _context->max_b_frames = 0;

  _context->width = _w;
  _context->height = _h;
  _context->pix_fmt = PIX_FMT_YUV420P;	//PIX_FMT_RGBA32
  //_context->debug=1;

  _internalBuffer = new uint8_t[w * h * 3];

  _swapUV = 0;
  //_context->strict_std_compliance=-1;

  _showMv = 0;
#define FF_SHOW		(FF_DEBUG_VIS_MV_P_FOR+	FF_DEBUG_VIS_MV_B_FOR+FF_DEBUG_VIS_MV_B_BACK)
//#define FF_SHOW               (FF_DEBUG_VIS_MV_P_FOR)
  printf ("[lavc] Build: %d\n", LIBAVCODEC_BUILD);
  _context->debug_mv |= FF_SHOW;
  _context->debug |= FF_DEBUG_VIS_MB_TYPE + FF_DEBUG_VIS_QP;
  
}

//_____________________________________________________

decoderFF::~decoderFF ()
{
  if (_usingMT)
    {
      printf ("[lavc] Killing decoding threads\n");
      avcodec_thread_free (_context);
      _usingMT = 0;
    }

  avcodec_close (_context);
  ADM_dealloc (_context);
  delete[]_internalBuffer;
  printf ("[lavc] Destroyed\n");
}

/**
    \fn frameType
    \return frametype of the last decoded frame
*/
uint32_t decoderFF::frameType (void)
{
  uint32_t
    flag = 0;

  AVFrame *
    target;
#define SET(x) {flag=x;aprintf("Frame is %s\n",#x);}


  target = &_frame;
  switch (target->pict_type)
    {
    case FF_B_TYPE:
      SET (AVI_B_FRAME);
      if (target->key_frame)
	aprintf ("\n But keyframe is set\n");
      break;

    case FF_I_TYPE:
      SET (AVI_KEY_FRAME);
      if (!target->key_frame)
	{
	  if (codecId == CODEC_ID_H264)
	    {
	      SET (AVI_P_FRAME);
	    }
	  else
	    printf ("\n But keyframe is not set\n");
	}
      break;
    case FF_S_TYPE:
      _gmc = 1;			// No break, just inform that gmc is there
    case FF_P_TYPE:
      SET (AVI_P_FRAME);
      if (target->key_frame)
	aprintf ("\n But keyframe is set\n");
      break;
    default:
//                              printf("\n OOops XXX frame ?\n");
      break;
    }
    flag&=~AVI_STRUCTURE_TYPE_MASK;
    if(target->interlaced_frame)
    {
        flag|=AVI_FIELD_STRUCTURE;
        if(target->top_field_first)
            flag|=AVI_TOP_FIELD;
        else
            flag|=AVI_BOTTOM_FIELD;
    }
  return flag;
}
bool decoderFF::decodeHeaderOnly (void)
{
  if (codecId == CODEC_ID_H264)
    _context->hurry_up = 4;
  else
    _context->hurry_up = 5;
  printf ("\n[lavc] Hurry up\n");
  return 1;
}
bool decoderFF::decodeFull (void)
{
  _context->hurry_up = 0;
  printf ("\n[lavc] full decoding\n");
  return 1;
}

/**
    \fn flush
    \brief empty internal buffer
*/
bool    decoderFF::flush(void)
{
    if(_context)
        avcodec_flush_buffers(_context);
    return true;
}
/**
    \fn uncompress
    \brief Actually decode an image
*/
bool   decoderFF::uncompress (ADMCompressedImage * in, ADMImage * out)
{
  int got_picture = 0;
  uint8_t *oBuff[3];
  int strideTab[3];
  int strideTab2[3];
  int ret = 0;
  out->_noPicture = 0;
  if (_showMv)
    {
      _context->debug_mv |= FF_SHOW;
      _context->debug |= 0;	//FF_DEBUG_VIS_MB_TYPE;
    }
  else
    {
      _context->debug_mv &= ~FF_SHOW;
      _context->debug &= ~(FF_DEBUG_VIS_MB_TYPE + FF_DEBUG_VIS_QP);
    }

   
    
  if (in->dataLength == 0 && !_allowNull)	// Null frame, silently skipped
    {
      
      printf ("[Codec] null frame\n");
        // search the last image
        if (_context->coded_frame && 
            _context->coded_frame->data &&
            _context->coded_frame->data[0]
            )
          {
            printf("[Codec] Cloning older pic\n");
            clonePic (_context->coded_frame, out);
            out->Pts=ADM_COMPRESSED_NO_PTS;
          }
        else
            {
                out->_noPicture = 1;
                out->Pts=ADM_COMPRESSED_NO_PTS;
                printf("[Codec] No Picture\n");
            }
          return 1;
    }
   // Put a safe value....
   out->Pts=in->demuxerPts;
    _context->reordered_opaque=in->demuxerPts;
  //_frame.opaque=(void *)out->Pts;
  //printf("Incoming Pts :%"LLD"\n",out->Pts);
  AVPacket pkt;
  av_init_packet(&pkt);
  pkt.data=in->data;
  pkt.size=in->dataLength;
  if(in->flags&AVI_KEY_FRAME)
    pkt.flags=AV_PKT_FLAG_KEY;
  else
    pkt.flags=0;
  
  ret = avcodec_decode_video2 (_context, &_frame, &got_picture, &pkt);
  if(!bFramePossible())
  {
    // No delay, the value is sure, no need to hide it in opaque
    _context->reordered_opaque=(int64_t)in->demuxerPts;
  }
  out->_qStride = 0;		//Default = no quant
  if (0 > ret && !_context->hurry_up)
    {
      printf ("\n[lavc] error in lavcodec decoder!\n");
      printf ("[lavc] Err: %d, size :%d\n", ret, in->dataLength);
      return 0;
    }
  if (!got_picture && !_context->hurry_up)
    {
      // Some encoder code a vop header with the 
      // vop flag set to 0
      // it is meant to mean frame skipped but very dubious
      if (in->dataLength <= 8 && codecId == CODEC_ID_MPEG4)
	{
	  printf ("[lavc] Probably pseudo black frame...\n");
	  out->_Qp = 2;
	  out->flags = 0;	// assume P ?

	  clonePic (_context->coded_frame, out);
	  return 1;
	}
      // allow null means we allow null frame in and so potentially
      // have no frame out for a time
      // in that case silently fill with black and returns it as KF
      if (_allowNull)
	{
	  out->flags = AVI_KEY_FRAME;
	  if (!_refCopy)
	    {
            out->blacken();
	    }
	  else
	    {
	      out->_noPicture = 1;
	    }
	  printf ("\n[lavc] ignoring got pict ==0\n");
	  return 1;

	}
#if 0
      printf ("[lavc] Err: %d, size: %d\n", ret, in->dataLength);
      printf ("\n[lavc] error in FFMP43/mpeg4!: got picture\n");
#endif
      //GUI_Alert("Please retry with misc->Turbo off");
      //return 1;
      return 0;
    }
  if (_context->hurry_up)
    {
      out->flags = frameType ();
      return 1;
    }
  // We have an image....
  switch (_context->pix_fmt)
    {
    case PIX_FMT_YUV411P:
      out->_colorspace = ADM_COLOR_YUV411;
      break;
    case PIX_FMT_YUYV422:
      out->_colorspace = ADM_COLOR_YUV422;
      break;
    case PIX_FMT_YUV422P:
    case PIX_FMT_YUVJ422P:
      out->_colorspace = ADM_COLOR_YUV422P;
      break;

    case PIX_FMT_YUV444P:
    case PIX_FMT_YUVJ444P:
      out->_colorspace = ADM_COLOR_YUV444;
      break;
    case PIX_FMT_YUV420P:
    case PIX_FMT_YUVJ420P:
    case PIX_FMT_YUVA420P:
      // Default is YV12 or I420
      // In that case depending on swap u/v
      // we do it or not
      out->_colorspace = ADM_COLOR_YV12;
      break;

    case PIX_FMT_RGBA: // ???PIX_FMT_RGBA32:
      out->_colorspace = ADM_COLOR_RGB32A;
      break;
    case PIX_FMT_RGB555:
      out->_colorspace = ADM_COLOR_RGB555;
      break;
    case PIX_FMT_VDPAU_MPEG1:
    case PIX_FMT_VDPAU_MPEG2:
    case PIX_FMT_VDPAU_WMV3:
    case PIX_FMT_VDPAU_VC1:
    case PIX_FMT_VDPAU_H264:
        out->_colorspace=ADM_COLOR_VDPAU;
        break;
    default:
      printf ("[lavc] Unhandled colorspace: %d\n", _context->pix_fmt);
      return 0;
    }
    clonePic (&_frame, out);
    //printf("[AvCodec] Pts : %llu Out Pts:%llu \n",_frame.pts,out->Pts);

  return 1;
}
// *******************************************************************
// *******************************************************************
// *******************************************************************

decoderFFDiv3::decoderFFDiv3 (uint32_t w, uint32_t h,uint32_t fcc, uint32_t extraDataLen, uint8_t *extraData,uint32_t bpp):
decoderFF (w, h,fcc,extraDataLen,extraData,bpp)
{
  _refCopy = 1;			// YUV420 only
  WRAP_Open (CODEC_ID_MSMPEG4V3);
}

decoderFFMpeg4::decoderFFMpeg4 (uint32_t w, uint32_t h,uint32_t fcc, uint32_t extraDataLen, uint8_t *extraData,uint32_t bpp):
decoderFF (w, h,fcc,extraDataLen,extraData,bpp)
{
// force low delay as avidemux don't handle B-frames
  ADM_info ("[lavc] Using %d bytes of extradata for MPEG4 decoder\n", (int)extraDataLen);
  
  _refCopy = 1;			// YUV420 only
  _context->extradata = (uint8_t *) extraData;
  _context->extradata_size = (int)extraDataLen  ;
  _context->codec_tag=fcc;
  _context->stream_codec_tag=fcc;
  decoderMultiThread ();
  //  _context->flags|=FF_DEBUG_VIS_MV;
  WRAP_Open (CODEC_ID_MPEG4);
}
bool decoderFFMpeg4::uncompress (ADMCompressedImage * in, ADMImage * out)
{
    // For pseudo startcode
    if(in->dataLength)
    {
        in->data[in->dataLength]=0;
        in->data[in->dataLength+1]=0;
    }
    return decoderFF::uncompress(in,out);

}
//************************************
decoderFFDV::decoderFFDV (uint32_t w, uint32_t h,uint32_t fcc, uint32_t extraDataLen, uint8_t *extraData,uint32_t bpp):
decoderFF (w, h,fcc,extraDataLen,extraData,bpp)
{
  _context->extradata = (uint8_t *) extraData;
  _context->extradata_size = (int) extraDataLen;
  WRAP_Open (CODEC_ID_DVVIDEO);

}
decoderFFMpeg12::decoderFFMpeg12 (uint32_t w, uint32_t h,uint32_t fcc, uint32_t extraDataLen, uint8_t *extraData,uint32_t bpp):
                decoderFF (w, h,fcc,extraDataLen,extraData,bpp)
{
  _refCopy = 1;			// YUV420 only
  decoderMultiThread ();
  WRAP_Open (CODEC_ID_MPEG2VIDEO);
}

decoderFFPng::decoderFFPng(uint32_t w, uint32_t h, uint32_t fcc, uint32_t extraDataLen, uint8_t *extraData, uint32_t bpp) : decoderFF(w, h, fcc, extraDataLen, extraData, bpp)
{
	WRAP_Open (CODEC_ID_PNG);
}

decoderFF_ffhuff::decoderFF_ffhuff (uint32_t w, uint32_t h,uint32_t fcc, uint32_t extraDataLen, uint8_t *extraData,uint32_t bpp)
:decoderFF (w, h,fcc,extraDataLen,extraData,bpp)
{
  _context->extradata = (uint8_t *) extraData;
  _context->extradata_size = (int) extraDataLen;
  _context->bits_per_coded_sample=bpp;
  ADM_info ("[lavc] FFhuff: We have %d bytes of extra data\n", (int)extraDataLen);
  WRAP_Open (CODEC_ID_FFVHUFF);

}
decoderFFH264::decoderFFH264 (uint32_t w, uint32_t h,uint32_t fcc, uint32_t extraDataLen, uint8_t *extraData,uint32_t bpp)
        :decoderFF (w, h,fcc,extraDataLen,extraData,bpp)
{

  _refCopy = 1;			// YUV420 only
  _context->extradata = (uint8_t *) extraData;
  _context->extradata_size = (int) extraDataLen;
  decoderMultiThread ();
  ADM_info ("[lavc] Initializing H264 decoder with %d extradata\n", (int)extraDataLen);
  WRAP_Open(CODEC_ID_H264);
}
//*********************
extern "C" {int av_getAVCStreamInfo(AVCodecContext *avctx, uint32_t  *nalSize, uint32_t *isAvc);}
/**
    \fn uncompress
*/
bool   decoderFFH264::uncompress (ADMCompressedImage * in, ADMImage * out)
{
  if(!_context->hurry_up) return decoderFF::uncompress (in, out);
    ADM_assert(0);
#if 0  
  uint32_t nalSize, isAvc;
  av_getAVCStreamInfo(_context,&nalSize,&isAvc);
  if(isAvc)
  {
      return extractH264FrameType(nalSize, in->data,in->dataLength,&(out->flags));
  }else
  {
    return extractH264FrameType_startCode(nalSize, in->data,in->dataLength,&(out->flags));
  }
#endif
}
//*********************
decoderFFhuff::decoderFFhuff (uint32_t w, uint32_t h,uint32_t fcc, uint32_t extraDataLen, uint8_t *extraData,uint32_t bpp):
decoderFF (w, h,fcc,extraDataLen,extraData,bpp)
{
  _context->extradata = (uint8_t *) extraData;
  _context->extradata_size = (int) extraDataLen;
  _context->bits_per_coded_sample = bpp;
  WRAP_Open (CODEC_ID_HUFFYUV);
}

//***************
extern uint8_t  lavformat_init(void);
extern void     avcodec_init(void );
extern  void    avcodec_register_all(void );
extern "C"
{
  void adm_lavLogCallback(void  *instance, int level, const char* fmt, va_list list);
}
/**
    \fn ADM_lavInit
    \brief Init both lavcodec and lavformat
*/
void ADM_lavInit(void)
{
    avcodec_init();
    avcodec_register_all();
//BAZOOKA    lavformat_init();
    av_log_set_callback(adm_lavLogCallback);
#ifdef ADM_DEBUG
  //  av_log_set_level(AV_LOG_DEBUG);
#endif

}
void adm_lavLogCallback(void  *instance, int level, const char* fmt, va_list list)
{
   // if(level>1) return;
    char buf[256];
  
    vsnprintf(buf, sizeof(buf), fmt, list);
    printf("[lavc] %s",buf);
}

void ADM_lavDestroy(void)
{
	//av_free_static();
}

// EOF
