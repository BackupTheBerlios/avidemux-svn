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
#include "ADM_default.h"
#include "ADM_editor/ADM_Video.h"

#include "fourcc.h"
#include "ADM_inputs/ADM_mp4/ADM_mp4.h"
#include "ADM_video/ADM_videoInfoExtractor.h"

#include "ADM_osSupport/ADM_debugID.h"
#define MODULE_NAME MODULE_3GP
#include "ADM_osSupport/ADM_debug.h"

extern "C"
{
#include "ADM_libraries/ffmpeg/config.h"
#include "libavcodec/get_bits.h"
#include "libavcodec/golomb.h"
#include "libavcodec/ff_spsinfo.h"
#include "libavcodec/parser.h"
#include "libavcodec/avcodec.h"
extern int ff_h264_info(AVCodecParserContext *parser,ffSpsInfo *ndo);

#undef printf
}

#include "ADM_infoExtractor/ADM_h264_tag.h"
#include "ADM_codecs/ADM_codec.h"

static void refineH264FrameType(uint8_t *head,uint8_t *tail,uint32_t *flags);

/**
    \fn unescapeH264
    \brief Remove escape stuff

*/
static uint32_t unescapeH264(uint32_t len,uint8_t *in, uint8_t *out)
{
  uint32_t outlen=0;
  uint8_t *tail=in+len;
    if(len<3) return 0;
    while(in<tail-3)
    {
      if(!in[0]  && !in[1] && in[2]==3)
      {
        out[0]=0;
        out[1]=0;
        out+=2;
        outlen+=2;
        in+=3; 
      }
      *out++=*in++;
      outlen++;
    }
    // copy last bytes
    uint32_t left=tail-in;
    memcpy(out,in,left);
    outlen+=left;
    return outlen;
    
}

static uint8_t extractVUIInfo(GetBitContext *s, int *fps1000, int *darNum, int *darDen)
{
	*fps1000 = *darNum = *darDen = 0;

	if (get_bits1(s))
	{
		unsigned int aspect_ratio_information = get_bits(s, 8);

		if (aspect_ratio_information == 255)
		{
			*darNum = get_bits_long(s, 16);
			*darDen = get_bits_long(s, 16);
		}
		else if (aspect_ratio_information < sizeof(pixel_aspect) / sizeof(*pixel_aspect))
		{
			*darNum = pixel_aspect[aspect_ratio_information].num;
			*darDen = pixel_aspect[aspect_ratio_information].den;
		}
	}

	if (get_bits1(s))	// overscan
		get_bits1(s);

	if (get_bits1(s))	// vsp_color
	{
		get_bits(s, 4);

		if (get_bits1(s))
		{
			get_bits(s, 8);
			get_bits(s, 8);
			get_bits(s, 8);
		}
	}

	if (get_bits1(s))	// chroma
	{
		get_ue_golomb(s);
		get_ue_golomb(s);
	}

	if (get_bits1(s))	// timing
	{
		uint32_t timeinc_unit = get_bits_long(s, 32);
		uint32_t timeinc_resolution = get_bits_long(s, 32);
		uint32_t fixed_fps = get_bits1(s);

		if (timeinc_unit > 0 && timeinc_resolution > 0)
			*fps1000 = (uint32_t)(((float)timeinc_resolution / (float)timeinc_unit) * 1000);

		if (fixed_fps)
			*fps1000 /= 2;
	}

	return 1;
}

/**
    \fn extractSPSInfo
    \brief Extract info from H264 SPS
    See 7.3.2.1 of 14496-10
*/
uint8_t extractSPSInfo_internal(uint8_t *data, uint32_t len, h264SpsInfo *info)
{
   GetBitContext s;
   
   uint32_t profile,constraint,level,pic_order_cnt_type,w,h, mbh;
   uint8_t buf[len];
   uint32_t outlen;
   uint32_t id,dum;
   
           outlen=unescapeH264(len,data,buf);
           init_get_bits( &s,buf, outlen*8);
            
           profile=get_bits(&s,8);
           constraint=get_bits(&s,8)>>5;
           level=get_bits(&s,8);
           id=get_ue_golomb(&s); // Seq parameter set id           
           printf("[H264]Profile : %u, Level :%u, SPSid:%u\n",profile,level,id);
           if(profile>=100) // ?? Borrowed from H264.C/FFMPEG
           {
              printf("[H264]Warning : High profile\n");
              if(get_ue_golomb(&s) == 3) //chroma_format_idc
                get_bits1(&s);  //residual_color_transform_flag
            get_ue_golomb(&s);  //bit_depth_luma_minus8
            get_ue_golomb(&s);  //bit_depth_chroma_minus8
            get_bits1(&s);
			if (get_bits1(&s))
				get_bits(&s, 8);
           }
           

           info->log2MaxFrameNumber = get_ue_golomb(&s); // log2_max_frame_num_minus4
           printf("[H264]Log2maxFrame-4:%u\n",info->log2MaxFrameNumber);
           pic_order_cnt_type=get_ue_golomb(&s);
           printf("[H264]Pic Order Cnt Type:%u\n",pic_order_cnt_type);
           if(!pic_order_cnt_type) // pic_order_cnt_type
           {
              dum=get_ue_golomb(&s); //log2_max_pic_order_cnt_lsb_minus4
              printf("[H264]Log2maxPix-4:%u\n",dum);
           }else
           {
             if(pic_order_cnt_type==1)
             {
                 get_bits1(&s);   //delta_pic_order_always_zero_flag
                 get_se_golomb(&s);   //offset_for_non_ref_pic
                 get_se_golomb(&s);  // offset_for_top_to_bottom_field
                 int i=get_ue_golomb(&s);  //num_ref_frames_in_pic_order_cnt_cycle

                 for(int j=0;j<i;j++)
                 {
                      get_se_golomb(&s);
                 }
             }else if(pic_order_cnt_type!=2)
             {
               printf("Error in SPS\n");
               return 0;
             }
           }
           dum=get_ue_golomb(&s);     //num_ref_frames
           printf("[H264] # of ref frames : %u\n",dum);
           get_bits1(&s);         // gaps_in_frame_num_value_allowed_flag
		   w = get_ue_golomb(&s) + 1;   //pic_width_in_mbs_minus1

		   mbh = get_ue_golomb(&s) + 1;
		   info->frameMbsOnlyFlag = get_bits1(&s);
		   h = (2 - info->frameMbsOnlyFlag) * mbh;   //pic_height_in_mbs_minus1

           printf("[H264] Width in mb -1  :%d\n",w); 
           printf("[H264] Height in mb -1 :%d\n", h);

		   info->width = w * 16;
		   info->height = h * 16;
           
		   if (!info->frameMbsOnlyFlag)
			   get_bits1(&s);

		   get_bits1(&s);

		   if(get_bits1(&s))
		   {
			   get_ue_golomb(&s);
			   get_ue_golomb(&s);
			   get_ue_golomb(&s);
			   get_ue_golomb(&s);
		   }

		   if(get_bits1(&s))
			   extractVUIInfo(&s, &(info->fps1000), &(info->darNum), &(info->darDen));

           return 1;
}
/**
      \fn extractH264FrameType
      \brief return frametype in flags (KEY_FRAME or 0). To be used only with  mkv/mp4 nal type (i.e. no startcode)
      
*/
uint8_t extractH264FrameType(uint32_t nalSize,uint8_t *buffer,uint32_t len,uint32_t *flags)
{
  uint8_t *head=buffer, *tail=buffer+len;
  uint8_t stream;
  
  uint32_t val,hnt;  
  
// FIXME :  no startcode only !
  
  while(head+4<tail)
  {
    
              uint32_t length=(head[0]<<24) + (head[1]<<16) +(head[2]<<8)+(head[3]);
              if(length>len||length<6)
              {
                printf("Warning , incomplete nal (%u/%u),(%0x/%0x)\n",length,len,length,len);
                *flags=0;
                return 0;
              }
              head+=4; // Skip nal lenth
              length-=4;
              stream=*(head++)&0x1F;
                switch(stream)
                {
                  case NAL_IDR: 
                                  *flags=AVI_KEY_FRAME;
                                  
                                  return 1;
                                  break; 
                  case NAL_NON_IDR: 
                                  refineH264FrameType(head,tail,flags);
                                  return 1;
                                  break;
                  default:
                          printf("??0x%x\n",stream);
                          head+=length-5;
                          continue;
                }
  }
  printf("No stream\n");
  return 0;
}

/**
      \fn extractH264FrameType_startCode
      \brief return frametype in flags (KEY_FRAME or 0). To be used only with  avi / mpeg TS nal type (i.e. with startcode)
      
*/
uint8_t extractH264FrameType_startCode(uint32_t nalSize,uint8_t *buffer,uint32_t len,uint32_t *flags)
{
  uint8_t *head=buffer, *tail=buffer+len;
  uint8_t stream;
  uint32_t val,hnt;
  
  while(head+4<tail)
  {
          // Search startcode
      
                hnt=(head[0]<<24) + (head[1]<<16) +(head[2]<<8)+(head[3]);
                head+=4;
                while((hnt!=1) && head<tail)
                {

                        hnt<<=8;
                        val=*head++;
                        hnt+=val;
                }
                if(head>=tail) break;
                stream=*(head++) &0x1f;
                switch(stream)
                {
                  case NAL_IDR: 
                                  *flags=AVI_KEY_FRAME;
                                 // printf("IDR\n");
                                  return 1;
                                  break; 
                  case NAL_NON_IDR: 
                                  refineH264FrameType(head,tail,flags);
                                  return 1;
                                  break;
                  default:
                          printf("??0x%x\n",stream);
                          continue;
                }
  }
  printf("No stream\n");
  return 0;
}
/**
    \fn refineH264FrameType
    \brief Try to detect B slice, warning the stream is not escaped!
*/
void refineH264FrameType(uint8_t *head,uint8_t *tail,uint32_t *flags)
{
GetBitContext s;
uint32_t sliceType;
            *flags=0;
            init_get_bits(&s,head, (tail-head)*8);
            get_ue_golomb(&s);
            sliceType= get_ue_golomb(&s);
            if(sliceType > 9) 
            {
              printf("Weird Slice %d\n",sliceType);
              return ;
            }
            if(sliceType > 4)
                sliceType -= 5;
            if(sliceType==3) *flags=AVI_B_FRAME;  
}
#if 0
int getH264SliceHeader(uint8_t *buffer, int length, h264SpsInfo *spsInfo, h264SliceHeader *sliceHeader)
{
	int sliceType;
	GetBitContext bc;

	init_get_bits(&bc, buffer, length * 8);
	get_ue_golomb(&bc);	// pps_id

	sliceType = get_ue_golomb(&bc);

	if (sliceType > 9) 
	{
		printf("Slice type too large: %d\n", sliceType);
		return -1;
	}
	else
	{
		if (sliceType == 0 || sliceType == 3 || sliceType == 5 || sliceType == 8)
			sliceHeader->sliceType = AVI_P_FRAME;
		else if (sliceType == 1 || sliceType == 6)
			sliceHeader->sliceType = AVI_B_FRAME;
		else
			sliceHeader->sliceType = AVI_KEY_FRAME;
	}

	sliceHeader->frameNumber = get_bits(&bc, spsInfo->log2MaxFrameNumber);

	if (spsInfo->frameMbsOnlyFlag)
		sliceHeader->pictureType = FRAME;
	else
	{
		if(get_bits1(&bc))	// field_pic_flag
		{
			if (get_bits1(&bc))
				sliceHeader->pictureType = TOP_FIELD;
			else
				sliceHeader->pictureType = BOTTOM_FIELD;
		}
		else
			sliceHeader->pictureType = FRAME;
	}

	return 1;
}
#endif

/**
        \fn extractSPSInfo2
        \brief Same as extractSPSInfo, but using libavcodec
*/
#define ADM_info printf
#define ADM_warning printf
#define ADM_error printf
uint8_t extractSPSInfo_lavcodec (uint8_t * data, uint32_t len, h264SpsInfo *spsinfo)
{
    static const uint8_t sig[5]={0,0,0,1,0};
    uint32_t myLen=len+5+5+FF_INPUT_BUFFER_PADDING_SIZE;
    uint8_t myData[myLen];
    bool r=false;

    memset(myData,myLen,1);
    mixDump(data,len);
    // put dummy SPS tag + dummy filler at the end
    // so that lavcodec can split it
    memcpy(myData,sig,5);
    memcpy(myData+5,data,len);
    memcpy(myData+5+len,sig,5);
    myData[4]=NAL_SPS;
    myData[5+len+4]=NAL_FILLER;
    
    
    // 1-Create parser
    AVCodecParserContext *parser=av_parser_init(CODEC_ID_H264);
    AVCodecContext *ctx=NULL; 
    AVCodec *codec=NULL;
    uint8_t *d=NULL;
  
    if(!parser)
    {
        ADM_error("cannot create h264 parser\n");
        goto theEnd;
    }
    ADM_info("Parser created\n");
    codec=avcodec_find_decoder(CODEC_ID_H264);
    if(!codec)
    {
        ADM_error("cannot create h264 codec\n");
        goto theEnd;
    }
    ADM_info("Codec created\n");
    ctx=avcodec_alloc_context();
   if (avcodec_open(ctx, codec) < 0)  
   {
        ADM_error("cannot create h264 context\n");
        goto theEnd;
    }
    ADM_info("Context created\n");
    //2- Parse, let's add SPS prefix + Filler postfix to make life easier for libavcodec parser
    ctx->extradata=myData;
    ctx->extradata_size=myLen;
     {
         uint8_t *outptr=NULL;
         int outsize=0;
         int consumed = av_parser_parse2(parser, ctx, 
                                      &outptr, &outsize,
                                      d, 0,
                                      0, 0,0);
    }
    ADM_info("Width  : %d\n",ctx->width);
    ADM_info("Height : %d\n",ctx->height);
    {
        ffSpsInfo nfo;
        if(!ff_h264_info(parser,&nfo))
        {
            ADM_error("Cannot get sps info from lavcodec\n");
            r=false;
            goto theEnd;
        }
        ADM_info("Width2 : %d\n",nfo.width);
        ADM_info("Height2: %d\n",nfo.height);
        #define CPY(x) spsinfo->x=nfo.x
        CPY(width);
        CPY(height);
        CPY(fps1000);
//        CPY(hasStructInfo);
//        CPY(CpbDpbToSkip);
        CPY(darNum);
        CPY(darDen);
        r=true;
     }
    // cleanup
theEnd:
    if(ctx)
        avcodec_close(ctx);
    if(parser)
        av_parser_close(parser);
    return r;
}
uint8_t extractSPSInfo(uint8_t *data, uint32_t len, h264SpsInfo *info)
{
    return extractSPSInfo_lavcodec(data,len,info);
    //return extractSPSInfo_internal(data,len,info);
}
