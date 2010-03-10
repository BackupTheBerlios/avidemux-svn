/***************************************************************************
                          \fn ADM_Xvid4
                          \brief Front end for xvid4 Mpeg4 asp encoder
                             -------------------
    
    copyright            : (C) 2002/2009 by mean/gruntster
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

#include "ADM_default.h"
#include "ADM_xvid4.h"
#undef ADM_MINIMAL_UI_INTERFACE // we need the full UI
#include "DIA_factory.h"

#if 1
#define aprintf(...) {}
#else
#define aprintf printf
#endif

#define MMSET(x) memset(&(x),0,sizeof(x))

xvid4_encoder xvid4Settings=
{
    {
    COMPRESS_CQ, //COMPRESSION_MODE  mode;
    2,              // uint32_t          qz;           /// Quantizer
    1500,           //uint32_t          bitrate;      /// In kb/s 
    700,            //uint32_t          finalsize;    /// In ?
    1500,           //uint32_t          avg_bitrate;  /// avg_bitrate is in kb/s!!
    ADM_ENC_CAP_CBR+ADM_ENC_CAP_CQ+ADM_ENC_CAP_2PASS+ADM_ENC_CAP_2PASS_BR+ADM_ENC_CAP_GLOBAL+ADM_ENC_CAP_SAME
    },
    
            false, //mpegQuant
            0, // rdMode
            0, // MotionEstimation
            0, // cqmMode
            0, // arMode
            0, // MaxBframe
            0, // MaxKeyInterval
            true // Trellis
    
};

typedef enum
{
        ME_NONE = 0,
        ME_LOW = XVID_ME_HALFPELREFINE16,
        ME_MEDIUM = XVID_ME_HALFPELREFINE16 | XVID_ME_ADVANCEDDIAMOND16,
        ME_HIGH = XVID_ME_HALFPELREFINE16 | XVID_ME_EXTSEARCH16 | XVID_ME_HALFPELREFINE8 | XVID_ME_USESQUARES16
} MotionEstimationMode;

uint32_t motionMode[4]=
{
    ME_NONE,ME_LOW,ME_MEDIUM,ME_HIGH
};

typedef enum
{
        INTERLACED_NONE,
        INTERLACED_BFF,
        INTERLACED_TFF
} InterlacedMode;

typedef enum
{
        RD_NONE = -1,
        RD_DCT_ME = 0,
        RD_HPEL_QPEL_16 = RD_DCT_ME | XVID_ME_HALFPELREFINE16_RD | XVID_ME_QUARTERPELREFINE16_RD,
        RD_HPEL_QPEL_8 = RD_HPEL_QPEL_16 | XVID_ME_HALFPELREFINE8_RD | XVID_ME_QUARTERPELREFINE8_RD | XVID_ME_CHECKPREDICTION_RD,
        RD_SQUARE = RD_HPEL_QPEL_8 | XVID_ME_EXTSEARCH_RD
} RateDistortionMode;
uint32_t rdMode[5]=
{
    RD_NONE,
    RD_DCT_ME,
    RD_HPEL_QPEL_16,
    RD_HPEL_QPEL_8,
    RD_SQUARE
};

/**
        \fn xvid4Encoder
*/
xvid4Encoder::xvid4Encoder(ADM_coreVideoFilter *src,bool globalHeader) : ADM_coreVideoEncoder(src)
{
    printf("[xvid4] Creating.\n");
    this->globalHeader=globalHeader;
    handle=NULL;
    MMSET(xvid_enc_frame);
}
/**
    \fn query
    \brief query xvid about version and flags
*/
bool xvid4Encoder::query(void)
{

 xvid_gbl_init_t   xvid_gbl_init2;
 xvid_gbl_info_t   xvid_gbl_info;
 
  MMSET (xvid_gbl_init2);
  MMSET (xvid_gbl_info);

  printf ("[xvid] Initializing global Xvid 4\n");
  xvid_gbl_init2.version = XVID_VERSION;
  xvid_global (NULL, XVID_GBL_INIT, &xvid_gbl_init2, NULL);
  xvid_gbl_info.version = XVID_VERSION;
  xvid_global (NULL, XVID_GBL_INFO, &xvid_gbl_info, NULL);

  if (xvid_gbl_info.build)
      printf ("[xvid] Build: %s\n", xvid_gbl_info.build);

  printf ("[xvid] SIMD supported: (%x)\n", xvid_gbl_info.cpu_flags);
#define CPUF(x) if(xvid_gbl_info.cpu_flags  & XVID_CPU_##x) printf("\t\t"#x"\n");
#if defined( ARCH_X86)  || defined(ARCH_X86_64)
  CPUF (MMX);
  CPUF (MMXEXT);
  CPUF (SSE);
  CPUF (SSE2);
  CPUF (3DNOW);
  CPUF (3DNOWEXT);
#endif
}
/**
    \fn setup
*/
bool xvid4Encoder::setup(void)
{
  ADM_info("Xvid4, setting up");
  query();
  xvid_enc_create_t xvid_enc_create;
  // Here we go...
  MMSET (xvid_enc_create);
  xvid_enc_create.version = XVID_VERSION;
  xvid_enc_create.width = getWidth();
  xvid_enc_create.height =getHeight();
  
  single.version = XVID_VERSION;

    switch(xvid4Settings.params.mode)
    {
      case COMPRESS_2PASS:
      case COMPRESS_2PASS_BITRATE:
#if 0
           if(false==setupPass())
            {
                ADM_warning("[xvid4] Multipass setup failed\n");
                return false;
            }
#endif
            break;
      case COMPRESS_SAME:
      case COMPRESS_CQ:
      case COMPRESS_CBR:
                  MMSET (single);
                  plugins[0].func = xvid_plugin_single;
                  plugins[0].param = &single;

                  single.version = XVID_VERSION;
                  switch(xvid4Settings.params.mode)
                  {
                    case COMPRESS_CBR:
                            single.bitrate = xvid4Settings.params.bitrate*1000;
                            break;
                    case COMPRESS_CQ:
                    case COMPRESS_SAME:

                            break;
                   }
            break;
     default:
            return false;
    }
   

  xvid_enc_create.plugins = plugins;
  xvid_enc_create.num_plugins = 1;

    //Framerate
    xvid_enc_create.fincr = 1000;
    xvid_enc_create.fbase = 25000;
    int xerr = xvid_encore (NULL, XVID_ENC_CREATE, &xvid_enc_create, NULL);
    if (xerr < 0)
    {
      ADM_error ("[xvid] init error: %d\n", xerr);
      return false;
    }

    handle = xvid_enc_create.handle;
    image=new ADMImage(getWidth(),getHeight());
    ADM_info("Xvid4, setup ok\n");
    return true;
}


/** 
    \fn ~ADM_ffMpeg4Encoder
*/
xvid4Encoder::~xvid4Encoder()
{
    ADM_info("[xvid4] Destroying.\n");
    if(handle)
    {
        xvid_encore(handle, XVID_ENC_DESTROY, NULL, NULL);
        handle=NULL;
    }
}

/**
    \fn encode
*/
bool         xvid4Encoder::encode (ADMBitstream * out)
{
    // 1 fetch a frame...
    uint32_t nb;
    if(source->getNextFrame(&nb,image)==false)
    {
        ADM_warning("[xvid4] Cannot get next image\n");
        return false;
    }
    // 2-premable
    if(false==preAmble(image))
    {
        ADM_warning("[Xvid4] preAmble failed\n");
        return false;
    }
    int size = xvid_encore(handle, XVID_ENC_ENCODE, &xvid_enc_frame, &xvid_enc_stats);
    if (size < 0)
    {
        ADM_error("[Xvid] Error performing encode %d\n", size);
        return false;
    }
    
    // 3-encode
    if(false==postAmble(out,size))
    {
        ADM_warning("[Xvid4] postAmble failed\n");
        return false;     
    }
    return true;
}

/**
    \fn isDualPass

*/
bool         xvid4Encoder::isDualPass(void) 
{
    if(xvid4Settings.params.mode==COMPRESS_2PASS || xvid4Settings.params.mode==COMPRESS_2PASS_BITRATE ) return true;
    return false;

}

/**
        \fn preAmble
        \fn prepare a frame to be encoded
*/
bool  xvid4Encoder::preAmble (ADMImage * in)
{
  MMSET (xvid_enc_stats);

  xvid_enc_frame.version = XVID_VERSION;
  xvid_enc_stats.version = XVID_VERSION;

  /* Bind output buffer */

  xvid_enc_frame.length = 0;
  if (xvid4Settings.mpegQuant)
    xvid_enc_frame.vol_flags |= XVID_VOL_MPEGQUANT;
 

#define SVOP(x,y) if(xvid4Settings.x) xvid_enc_frame.vop_flags|=XVID_VOP_##y

  xvid_enc_frame.motion = motionMode[xvid4Settings.motionEstimation];
  xvid_enc_frame.vop_flags|=XVID_VOP_INTER4V;
  xvid_enc_frame.vop_flags |= XVID_VOP_HALFPEL;
  
  SVOP (trellis, TRELLISQUANT);
 // SVOP (hqac, HQACPRED);
 // SVOP (bvhq, RD_BVOP);
//  SVOP (chroma_opt, CHROMAOPT);
  
  // ME 
  //if (_param.chroma_me)
  //  {
    //  xvid_enc_frame.motion |= XVID_ME_CHROMA_BVOP;
      //xvid_enc_frame.motion |= XVID_ME_CHROMA_PVOP;
    //}
 

    xvid_enc_frame.motion|=rdMode[xvid4Settings.rdMode];
 
#if 0

  if (_param.turbo)
    {
      xvid_enc_frame.motion |= XVID_ME_FASTREFINE16;
      xvid_enc_frame.motion |= XVID_ME_FASTREFINE8;
      xvid_enc_frame.motion |= XVID_ME_SKIP_DELTASEARCH;
      xvid_enc_frame.motion |= XVID_ME_FAST_MODEINTERPOLATE;
      xvid_enc_frame.motion |= XVID_ME_BFRAME_EARLYSTOP;
    }
#endif
  //xvid_enc_frame.bframe_threshold = _param.bframe_threshold;

  xvid_enc_frame.input.csp = XVID_CSP_YV12;
  xvid_enc_frame.input.stride[0] = getWidth();
  xvid_enc_frame.input.stride[1] = getWidth()>>1;
  xvid_enc_frame.input.stride[2] = getWidth()>> 1;
  xvid_enc_frame.type = XVID_TYPE_AUTO;


  /* Set up motion estimation flags */
  xvid_enc_frame.input.plane[0] = YPLANE(in);
  xvid_enc_frame.input.plane[1] = UPLANE(in);
  xvid_enc_frame.input.plane[2] = VPLANE(in);
#if 0
  xvid_enc_frame.par_width = _param.par_width;
  xvid_enc_frame.par_height = _param.par_height;
  //printf("Using AR : %u x %u\n",xvid_enc_frame.par_width,xvid_enc_frame.par_height );
  if (xvid_enc_frame.par_width != xvid_enc_frame.par_height)
      xvid_enc_frame.par = XVID_PAR_EXT;
  else
      xvid_enc_frame.par = XVID_PAR_11_VGA;

  /* Custome matrices */
  if(_param.useCustomIntra) 
  {
  if(!xvid_enc_frame.quant_intra_matrix)  
      printf("[xvid] Using custom intra matrix\n");
      xvid_enc_frame.quant_intra_matrix=_param.intraMatrix;
  }
  if(_param.useCustomInter)
  {
    if(!xvid_enc_frame.quant_inter_matrix)
      printf("[xvid] Using custom inter matrix\n");
     xvid_enc_frame.quant_inter_matrix=_param.interMatrix;
  }
#endif
  return 1;
}
/**
    \fn postAmble
    \brief update after a frame has been succesfully encoded
*/
bool xvid4Encoder::postAmble (ADMBitstream * out,int size)
{
  out->flags = 0;
  if (xvid_enc_frame.out_flags & XVID_KEYFRAME)
    {
      out->flags = AVI_KEY_FRAME;
    }
  else if (xvid_enc_stats.type == XVID_TYPE_BVOP)
    {
      out->flags = AVI_B_FRAME;

    }
  out->len=size;
  //out->pts=ADM_NO_PTS;
  //out->dts=ADM_NO_PTS;
  return 1;
}
// EOF

