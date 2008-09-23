
// Author: mean <fixounet@free.fr>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ADM_assert.h"
#include "config.h"

#include "fourcc.h"
#include "avio.hxx"
#include "avi_vars.h"

#include "ADM_toolkit/toolkit.hxx"
#include "ADM_userInterfaces/ADM_commonUI/DIA_factory.h"
#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_codecs/ADM_x264param.h"

#ifdef USE_X264
extern "C"
{
#include "x264.h"
}
#endif
/**
      \fn DIA_x264
      \brief Dialog for x264 codec settings
*/
uint8_t DIA_x264(COMPRES_PARAMS *config)
{
int b;
int ret=0;
int code;
float AqStrength;

      ADM_x264Param localParam;
      ADM_assert(config->extraSettingsLen==sizeof(localParam));
      memcpy(&localParam,config->extraSettings,sizeof(localParam));
#define PX(x) &(localParam.x)

	  AqStrength = localParam.AqStrength;
	  AqStrength /= 100;
      
      // Our tabs
         /* Tab 1 main */
           diaElemBitrate bitrate(config,NULL);
           bitrate.setMaxQz(51);
           diaElem *main[]={&bitrate};
           diaElemTabs tabMain(QT_TR_NOOP("Bitrate"),1,main);

           
        /* Tab 2 Motion */


         diaMenuEntry partitionM[] = {
                  {0,   QT_TR_NOOP("1  - Extremely Low (Fastest)")}
                ,{1,    QT_TR_NOOP("2  - Very Low")}
                ,{2,    QT_TR_NOOP("3  - Low")}
                ,{3,    QT_TR_NOOP("4  - Medium")}
                ,{4,    QT_TR_NOOP("5  - High (Default)")}
                ,{5,    QT_TR_NOOP("6  - Very High")}
                ,{6,    QT_TR_NOOP("6B - Very High (RDO on B-frames)")}
                ,{7,    QT_TR_NOOP("7  - Ultra High")}
                ,{8,    QT_TR_NOOP("7B - Ultra High (RDO on B-frames)")}};

                            
        diaElemMenu parition(PX(PartitionDecision),QT_TR_NOOP("Partition Decision"),9,partitionM);
        
         diaMenuEntry meM[] = {
                             {0,    QT_TR_NOOP("Diamond Search")},
                             {1,    QT_TR_NOOP("Hexagonal Search")},
                             {2,    QT_TR_NOOP("Uneven Multi-hexagon")},
                             {3,    QT_TR_NOOP("Exhaustive Search")}
#if X264_BUILD >= 58
							 ,{4, QT_TR_NOOP("Hadamard Exhaustive Search")}
#endif
							 };

         diaElemMenu      me(PX(Method),QT_TR_NOOP("VHQ Mode"),
 #if X264_BUILD >= 58
			 5
#else
			 4
#endif
			 ,meM);

         diaElemUInteger  rframe(PX(MaxRefFrames),QT_TR_NOOP("Max Ref Frames"),0,15);
         diaElemUInteger  range(PX(Range),QT_TR_NOOP("Range"),0,64);
         diaElemToggle    chromaMe(PX(ChromaME),QT_TR_NOOP("Chroma ME"));
         diaElemToggle    mixedRef(PX(MixedRefs),QT_TR_NOOP("Mixed Refs"));
         diaElemToggle    fastPSkip(PX(fastPSkip),QT_TR_NOOP("Fast P Skip"));
         diaElemToggle    dctDecimate(PX(DCTDecimate),QT_TR_NOOP("DCT Decimate"));
         diaElemToggle    interlaced(PX(interlaced),QT_TR_NOOP("Interlaced"));
         
         diaElemFrame  frameMe(QT_TR_NOOP("Motion Estimation"));
         frameMe.swallow(&parition);
         frameMe.swallow(&me);
         frameMe.swallow(&rframe);
         frameMe.swallow(&range);
         frameMe.swallow(&chromaMe);
         frameMe.swallow(&mixedRef);
         frameMe.swallow(&fastPSkip);
         frameMe.swallow(&dctDecimate);
         frameMe.swallow(&interlaced);
         
           diaElem *motions[]={&frameMe};
          diaElemTabs tabMotion(QT_TR_NOOP("Motion"),1,motions);
         
         
        /* Tab3-Misc  */
           diaMenuEntry trellisM[] = {
                  {0,   QT_TR_NOOP("Disabled")}
                ,{1,    QT_TR_NOOP("Low")}
                ,{2,    QT_TR_NOOP("High")}};
           
          diaElemMenu trellis(PX(Trellis),QT_TR_NOOP("Trellis"),3,trellisM);
          diaElemToggle    cabac(PX(CABAC),QT_TR_NOOP("CABAC"));
          diaElemUInteger  noise(PX(NoiseReduction),QT_TR_NOOP("Noise Reduction"),0,255);
          diaElemToggle    deblock(PX(DeblockingFilter),QT_TR_NOOP("Deblocking Filter"));
          diaElemInteger  deblockStrength(PX(Strength),QT_TR_NOOP("Strength"),-6,6);
          diaElemInteger  deblockThreshold(PX(Threshold),QT_TR_NOOP("Threshold"),-6,6);
          deblock.link(1,&deblockStrength);
          deblock.link(1,&deblockThreshold);
          
          
          diaElemFrame  frameMisc(QT_TR_NOOP("Misc"));
          frameMisc.swallow(&trellis);
		  frameMisc.swallow(&noise);
          frameMisc.swallow(&cabac);
          frameMisc.swallow(&deblock);
          frameMisc.swallow(&deblockStrength);
          frameMisc.swallow(&deblockThreshold);

#if X264_BUILD >= 62
		  diaElemFrame  frameAq(QT_TR_NOOP("Adaptive Quantisation"));

		  diaElemToggle    aq(PX(AqMode),QT_TR_NOOP("Variance AQ"));
		  diaElemFloat  aqStrength(&AqStrength,QT_TR_NOOP("Strength"),0.5,1.5);
		  aq.link(1, &aqStrength);

		  frameAq.swallow(&aq);
		  frameAq.swallow(&aqStrength);
#endif

		  diaElem *misc[]={&frameMisc
#if X264_BUILD >= 62
			  , &frameAq
#endif
		  };

          diaElemTabs tabMisc(QT_TR_NOOP("Misc"),
#if X264_BUILD >= 62
			  2
#else
			  1
#endif
			  ,misc);
        /* Tab 4 Partition & frame*/
         
             diaElemToggle    _8x8(PX(_8x8),QT_TR_NOOP("8x8 Transform"));
             diaElemToggle    _8x8P(PX(_8x8P),QT_TR_NOOP("8x8, 8x16 and 16x8 P-frame search"));
             diaElemToggle    _8x8B(PX(_8x8B),QT_TR_NOOP("8x8, 8x16 and 16x8 B-frame search"));
             diaElemToggle    _4x4(PX(_4x4),QT_TR_NOOP("4x4, 4x8 and 8x4 P-frame search"));
             diaElemToggle    _8x8I(PX(_8x8I),QT_TR_NOOP("8x8 Intra search"));
             diaElemToggle    _4x4I(PX(_4x4I),QT_TR_NOOP("4x4 Intra search"));
          diaElemFrame  frameTransform(QT_TR_NOOP("Partition Macroblocks"));
           frameTransform.swallow(&_8x8);
           frameTransform.swallow(&_8x8P);
           frameTransform.swallow(&_8x8B);
           frameTransform.swallow(&_4x4);
           frameTransform.swallow(&_8x8I);
           frameTransform.swallow(&_4x4I);
          
           diaElemUInteger  bframe(PX(MaxBFrame),QT_TR_NOOP("Max. Consecutive"),0, 16);
		   diaElemInteger  bias(PX(Bias),QT_TR_NOOP("Bias"), -100, 100);
		   diaElemToggle    reference(PX(BasReference),QT_TR_NOOP("Use as Reference"));
		   diaElemToggle    bidirMe(PX(BidirME),QT_TR_NOOP("Bidirectional ME"));
		   diaElemToggle    weighted(PX(Weighted),QT_TR_NOOP("Weighted Biprediction"));

         diaMenuEntry bframeAdapt[] = {
                             {0,    QT_TR_NOOP("Disabled")},
                             {1,    QT_TR_NOOP("Fast")}
#if X264_BUILD >= 63
							 ,{2,    QT_TR_NOOP("Optimal")}
#endif
							 };
		 diaElemMenu      adaptativeDct(PX(Adaptative),QT_TR_NOOP("Adaptive DCT"),
#if X264_BUILD >= 63
			 3
#else
			 2
#endif
			 , bframeAdapt);

           diaMenuEntry directModeOptions[] = {
                  {0,   QT_TR_NOOP("None")}
                ,{1,    QT_TR_NOOP("Spatial")}
                ,{2,    QT_TR_NOOP("Temporal")}
				,{3,    QT_TR_NOOP("Auto")}};
           
		   diaElemMenu directMode(PX(DirectMode),QT_TR_NOOP("Direct Mode"),4,directModeOptions);
           diaElemFrame frameB(QT_TR_NOOP("B-Frames"));
           frameB.swallow(&bframe);
		   frameB.swallow(&bias);
		   frameB.swallow(&reference);
		   frameB.swallow(&bidirMe);
		   frameB.swallow(&weighted);
		   frameB.swallow(&adaptativeDct);
		   frameB.swallow(&directMode);
           
		   diaElem *bfr[]={&frameTransform,&frameB};
		   diaElemTabs tabTransform(QT_TR_NOOP("Partitions && Frames"),2,bfr);

		  // Rate control tab
		  diaElemUInteger  keyframeBoost(PX(KeyframeBoost),QT_TR_NOOP("Keyframe Boost (%)"), 0, 100);
		  diaElemUInteger  bframeReduction(PX(BframeReduction),QT_TR_NOOP("B-frame Reduction (%)"), 0, 100);
		  diaElemUInteger  bitrateVar(PX(BitrateVariability),QT_TR_NOOP("Bitrate Variability (%)"), 0, 100);
		  diaElemFrame frameBitrate(QT_TR_NOOP("Bitrate"));

		  frameBitrate.swallow(&keyframeBoost);
		  frameBitrate.swallow(&bframeReduction);
		  frameBitrate.swallow(&bitrateVar);

		  diaElemUInteger  minQp(PX(MinQp),QT_TR_NOOP("Min QP"), 10, 51);
		  diaElemUInteger  maxQp(PX(MaxQp),QT_TR_NOOP("Max QP"), 10, 51);
		  diaElemUInteger  maxQpStep(PX(QpStep),QT_TR_NOOP("Max QP Step"), 0, 10);
		  diaElemFrame frameQuantLimits(QT_TR_NOOP("Quantisation Limits"));

		  frameQuantLimits.swallow(&minQp);
		  frameQuantLimits.swallow(&maxQp);
		  frameQuantLimits.swallow(&maxQpStep);

		  diaElemUInteger  sceneCut(PX(SceneCut),QT_TR_NOOP("Scene Cut Threshold"), 0, 100);
		  diaElemUInteger  minIdr(PX(MinIdr),QT_TR_NOOP("Min IDR Frame Interval"), 0, 100);
		  diaElemUInteger  maxIdr(PX(MaxIdr),QT_TR_NOOP("Max IDR Frame Interval"), 1, 1000);
		  diaElemFrame frameMoreRate(QT_TR_NOOP("More Rate Settings"));

		  frameMoreRate.swallow(&sceneCut);
		  frameMoreRate.swallow(&minIdr);
		  frameMoreRate.swallow(&maxIdr);

		  diaElem *rateControl[]={&frameBitrate,&frameQuantLimits,&frameMoreRate};
		  diaElemTabs tabRateControl(QT_TR_NOOP("Rate Control"),3,rateControl);

		  diaElemUInteger  maxLocalBitrate(PX(vbv_max_bitrate),QT_TR_NOOP("Maximum Local Bitrate"), 0, 99999);
		  diaElemUInteger  vbvBuffer(PX(vbv_buffer_size),QT_TR_NOOP("VBV Buffer Size"), 0, 99999);
		  diaElemUInteger  initVbvBuffer(PX(vbv_buffer_init),QT_TR_NOOP("Initial VBV Buffer (%)"), 0, 100);
		  diaElemFrame frameVbv(QT_TR_NOOP("Video Buffer Verifier"));

		  frameVbv.swallow(&maxLocalBitrate);
		  frameVbv.swallow(&vbvBuffer);
		  frameVbv.swallow(&initVbvBuffer);

		  diaElem *vbv[]={&frameVbv};
		  diaElemTabs tabVbv(QT_TR_NOOP("VBV"),1,vbv);
           
          /* End of tabs */
        diaElemTabs *tabs[6]={&tabMain,&tabMotion,&tabMisc,&tabTransform,&tabRateControl,&tabVbv};
        if( diaFactoryRunTabs(QT_TR_NOOP("x264 Configuration"),6,tabs))
	{
		localParam.AqStrength = floor((AqStrength * 100) + 0.49);

           memcpy(config->extraSettings,&localParam,sizeof(localParam));
           return 1;
        }
         return 0;
}

// EOF
