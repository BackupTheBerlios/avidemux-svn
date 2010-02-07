/***************************************************************************
                          \fn ADM_ffMpeg2
                          \brief Front end for libavcodec Mpeg2 asp encoder
                             -------------------
    
    copyright            : (C) 2002/2009 by mean
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
#include "ADM_lavcodec.h"
#include "ADM_default.h"
#include "ADM_ffMpeg2.h"
#undef ADM_MINIMAL_UI_INTERFACE // we need the full UI
#include "DIA_factory.h"

#if 1
#define aprintf(...) {}
#else
#define aprintf printf
#endif

mpeg2_encoder Mp2Settings=
{
    {
    COMPRESS_CQ, //COMPRESSION_MODE  mode;
    2,              // uint32_t          qz;           /// Quantizer
    1500,           //uint32_t          bitrate;      /// In kb/s 
    700,            //uint32_t          finalsize;    /// In ?
    1500,           //uint32_t          avg_bitrate;  /// avg_bitrate is in kb/s!!
    ADM_ENC_CAP_CBR+ADM_ENC_CAP_CQ+ADM_ENC_CAP_2PASS+ADM_ENC_CAP_2PASS_BR+ADM_ENC_CAP_GLOBAL+ADM_ENC_CAP_SAME
    },
    {
        ADM_AVCODEC_SETTING_VERSION,
        2, // Multithreaded
          ME_EPZS,			// ME
          0,				// GMC     
          0,				// 4MV
          0,				// _QPEL;   
          1,				// _TREILLIS_QUANT
          2,				// qmin;
          31,				// qmax;
          3,				// max_qdiff;
          2,				// max_b_frames;
          1,				// mpeg_quant;
          1,				// is_luma_elim_threshold
          -2,				// luma_elim_threshold;
          1,				// is_chroma_elim_threshold
          -5,				// chroma_elim_threshold;
          0.05,				//lumi_masking;
          1,				// is lumi
          0.01,				//dark_masking; 
          1,				// is dark
          0.5,				// qcompress amount of qscale change between easy & hard scenes (0.0-1.0
          0.5,				// qblur;    amount of qscale smoothing over time (0.0-1.0) 
          0,   		        // min bitrate in kB/S
          10000,			// max bitrate
          0,				// default matrix
          18,				// no gop size
          NULL,             // intra_matrix
          NULL,             // intra_matrix
          0,				// interlaced
          0,				// WLA: bottom-field-first
          0,				// wide screen
          2,				// mb eval = distortion
          8000,				// vratetol 8Meg
          0,				// is temporal
          0.0,				// temporal masking
          0,				// is spatial
          0.0,				// spatial masking
          0,				// NAQ
          0				    // DUMMY 
    },
    0   // Matrix
};
/**
        \fn ADM_ffMpeg2Encoder
*/
// It works because mpeg2_encoder.h is the same as FFCodecSettings + additional fields!
ADM_ffMpeg2Encoder::ADM_ffMpeg2Encoder(ADM_coreVideoFilter *src,bool globalHeader) : 
        ADM_coreVideoEncoderFFmpeg(src,(FFcodecSettings *)&(Mp2Settings),false)
{
    printf("[ffMpeg2Encoder] Creating.\n");
   

}

/**
    \fn setup
*/
bool ADM_ffMpeg2Encoder::setup(void)
{
    
    switch(Settings.params.mode)
    {
      case COMPRESS_2PASS:
      case COMPRESS_2PASS_BITRATE:
           if(false==setupPass())
            {
                printf("[ffmpeg] Multipass setup failed\n");
                return false;
            }
            break;
      case COMPRESS_SAME:
      case COMPRESS_CQ:
            _context->flags |= CODEC_FLAG_QSCALE;
            _context->bit_rate = 0;
            break;
      case COMPRESS_CBR:
              _context->bit_rate=Settings.params.bitrate*1000; // kb->b;
            break;
     default:
            return false;
    }
    presetContext(&Settings);
    if(false== ADM_coreVideoEncoderFFmpeg::setup(CODEC_ID_MPEG2VIDEO))
        return false;

    printf("[ffMpeg] Setup ok\n");
    return true;
}


/** 
    \fn ~ADM_ffMpeg2Encoder
*/
ADM_ffMpeg2Encoder::~ADM_ffMpeg2Encoder()
{
    printf("[ffMpeg2Encoder] Destroying.\n");
   
    
}

/**
    \fn encode
*/
bool         ADM_ffMpeg2Encoder::encode (ADMBitstream * out)
{
int sz,q;
again:
    sz=0;
    if(false==preEncode()) // Pop - out the frames stored in the queue due to B-frames
    {
        if ((sz = avcodec_encode_video (_context, out->data, out->bufferSize, NULL)) <= 0)
        {
            printf("[ffMpeg2] Error %d encoding video\n",sz);
            return false;
        }
        printf("[ffMpeg2] Popping delayed bframes (%d)\n",sz);
        goto link;
        return false;
    }
    q=image->_Qp;
    
    if(!q) q=2;
    switch(Settings.params.mode)
    {
      case COMPRESS_SAME:
                // Keep same frame type & same Qz as the incoming frame...
            _frame.quality = (int) floor (FF_QP2LAMBDA * q+ 0.5);

            if(image->flags & AVI_KEY_FRAME)    _frame.pict_type=FF_I_TYPE;
            else if(image->flags & AVI_B_FRAME) _frame.pict_type=FF_B_TYPE;
            else                                _frame.pict_type=FF_P_TYPE;

            break;
      case COMPRESS_2PASS:
      case COMPRESS_2PASS_BITRATE:
            switch(pass)
            {
                case 1: 
                        break;
                case 2: 
                        break; // Get Qz for this frame...
            }
      case COMPRESS_CQ:
            _frame.quality = (int) floor (FF_QP2LAMBDA * Settings.params.qz+ 0.5);
            break;
      case COMPRESS_CBR:
            break;
     default:
            printf("[ffMpeg2] Unsupported encoding mode\n");
            return false;
    }
    aprintf("[CODEC] Flags = 0x%x, QSCALE=%x, bit_rate=%d, quality=%d qz=%d incoming qz=%d\n",_context->flags,CODEC_FLAG_QSCALE,
                                     _context->bit_rate,  _frame.quality, _frame.quality/ FF_QP2LAMBDA,q);     
    
    _frame.reordered_opaque=image->Pts;
    if ((sz = avcodec_encode_video (_context, out->data, out->bufferSize, &_frame)) < 0)
    {
        printf("[ffMpeg2] Error %d encoding video\n",sz);
        return false;
    }
    
    if(sz==0) // no pic, probably pre filling, try again
        goto again;
link:
    postEncode(out,sz);
   
    return true;
}

/**
    \fn isDualPass

*/
bool         ADM_ffMpeg2Encoder::isDualPass(void) 
{
    if(Settings.params.mode==COMPRESS_2PASS || Settings.params.mode==COMPRESS_2PASS_BITRATE ) return true;
    return false;

}

/**
    \fn jpegConfigure
    \brief UI configuration for jpeg encoder
*/

bool         ffMpeg2Configure(void)
{         
diaMenuEntry meE[]={
  {1,QT_TR_NOOP("None")},
  {2,QT_TR_NOOP("Full")},
  {3,QT_TR_NOOP("Log")},
  {4,QT_TR_NOOP("Phods")},
  {5,QT_TR_NOOP("EPZS")},
  {6,QT_TR_NOOP("X1")}
};       


diaMenuEntry rdE[]={
  {0,QT_TR_NOOP("MB comparison")},
  {1,QT_TR_NOOP("Fewest bits (vhq)")},
  {2,QT_TR_NOOP("Rate distortion")}
};     
diaMenuEntry threads[]={
  {0,QT_TR_NOOP("One thread")},
  {2,QT_TR_NOOP("Two threads)")},
  {3,QT_TR_NOOP("Three threads")},
  {99,QT_TR_NOOP("Auto (#cpu)")}
};     


        mpeg2_encoder *conf=&Mp2Settings;

uint32_t me=(uint32_t)conf->lavcSettings.me_method;  
#define PX(x) &(conf->lavcSettings.x)

         diaElemBitrate   bitrate(&(Mp2Settings.params),NULL);

         diaElemMenu      threadM(PX(MultiThreaded),QT_TR_NOOP("Threading"),4,threads);
         diaElemUInteger  qminM(PX(qmin),QT_TR_NOOP("Mi_n. quantizer:"),1,31);
         diaElemUInteger  qmaxM(PX(qmax),QT_TR_NOOP("Ma_x. quantizer:"),1,31);
         diaElemUInteger  qdiffM(PX(max_qdiff),QT_TR_NOOP("Max. quantizer _difference:"),1,31);
         
         diaElemToggle    trellis(PX(_TRELLIS_QUANT),QT_TR_NOOP("_Trellis quantization"));
         
         diaElemUInteger  max_b_frames(PX(max_b_frames),QT_TR_NOOP("_Number of B frames:"),0,32);
         
         diaElemMenu     rdM(PX(mb_eval),QT_TR_NOOP("_Macroblock decision:"),3,rdE);
         
         diaElemUInteger filetol(PX(vratetol),QT_TR_NOOP("_Filesize tolerance (kb):"),0,100000);
         
         diaElemFloat    qzComp(PX(qcompress),QT_TR_NOOP("_Quantizer compression:"),0,1);
         diaElemFloat    qzBlur(PX(qblur),QT_TR_NOOP("Quantizer _blur:"),0,1);
         
        diaElemUInteger GopSize(PX(gop_size),QT_TR_NOOP("_Gop Size:"),1,30); 
          /* First Tab : encoding mode */
        diaElem *diamode[]={&GopSize,&threadM,&max_b_frames,&bitrate};
        diaElemTabs tabMode(QT_TR_NOOP("User Interface"),4,diamode);
        
        /* 2nd Tab : ME */
        /* 3nd Tab : Qz */
        
        diaElem *diaQze[]={&rdM,&qminM,&qmaxM,&qdiffM,&trellis};
        diaElemTabs tabQz(QT_TR_NOOP("Quantization"),5,diaQze);
        
        /* 4th Tab : RControl */
        
         diaElem *diaRC[]={&filetol,&qzComp,&qzBlur};
        diaElemTabs tabRC(QT_TR_NOOP("Rate Control"),3,diaRC);
        
         diaElemTabs *tabs[]={&tabMode,&tabQz,&tabRC};
        if( diaFactoryRunTabs(QT_TR_NOOP("libavcodec MPEG-2 configuration"),3,tabs))
        {
          conf->lavcSettings.me_method=(Motion_Est_ID)me;
          return true;
        }
         return false;
}
// EOF
