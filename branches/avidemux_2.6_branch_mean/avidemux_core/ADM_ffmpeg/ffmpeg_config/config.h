#include "ADM_coreConfig.h"
#define CONFIG_H263_VAAPI_HWACCEL 0
#define CONFIG_MPEG2_VAAPI_HWACCEL 0
#define CONFIG_MPEG4_VAAPI_HWACCEL 0
#define CONFIG_VC1_VAAPI_HWACCEL 0
#define CONFIG_WMV3_VAAPI_HWACCEL 0
#ifdef USE_VDPAU
#define CONFIG_H264_VDPAU_DECODER 1
#else // USE_VDPAU
#define CONFIG_H264_VDPAU_DECODER 0
#endif // 
#define CONFIG_MPEG_VDPAU_DECODER 0
#define CONFIG_MPEG1_VDPAU_DECODER 0
#define CONFIG_VC1_VDPAU_DECODER 0
#define CONFIG_WMV3_VDPAU_DECODER 0
#define CONFIG_DCA_DECODER 1
#define CONFIG_MP3_DECODER 1
#define CONFIG_MP2_DECODER 1
#define CONFIG_AC3_DECODER 1
#define CONFIG_EAC3_DECODER 1
#define CONFIG_NELLYMOSER_DECODER 1
#define CONFIG_ADPCM_IMA_AMV_DECODER 1
#define CONFIG_CINEPAK_DECODER 1
#define CONFIG_DNXHD_DECODER 1
#define CONFIG_FOURXM_DECODER 1
#define CONFIG_FRAPS_DECODER 1
#define CONFIG_H263_DECODER 1
#define CONFIG_H264_DECODER 1
#define CONFIG_INDEO2_DECODER 1
#define CONFIG_INDEO3_DECODER 1
#define CONFIG_INTERPLAY_VIDEO_DECODER 1
#define CONFIG_MPEGVIDEO_DECODER 1
#define CONFIG_MSVIDEO1_DECODER 1
#define CONFIG_VC1_DECODER 1
#define CONFIG_VCR1_DECODER 1
#define CONFIG_VP5_DECODER 1
#define CONFIG_VP6_DECODER 1
#define CONFIG_VP6A_DECODER 1
#define CONFIG_VP6F_DECODER 1
#define CONFIG_WMV3_DECODER 1
#define CONFIG_WMV1_DECODER 1
#define CONFIG_WMV2_DECODER 1
#define CONFIG_VP3_DECODER 1
#define CONFIG_MPEG4_DECODER 1
#define CONFIG_MSMPEG4V1_DECODER 1
#define CONFIG_MSMPEG4V2_DECODER 1
#define CONFIG_MSMPEG4V3_DECODER 1
#define CONFIG_MJPEGB_DECODER 1
#define CONFIG_MJPEG_DECODER 1
#define CONFIG_WMAV2_DECODER 1
#define CONFIG_DVVIDEO_DECODER 1
#define CONFIG_HUFFYUV_DECODER 1
#define CONFIG_FFVHUFF_DECODER 1
#define CONFIG_SVQ3_DECODER 1
#define CONFIG_TSCC_DECODER 1
#define CONFIG_QDM2_DECODER 1
#define CONFIG_FFV1_DECODER 1
#define CONFIG_MPEG1VIDEO_DECODER 1
#define CONFIG_MPEG2VIDEO_DECODER 1
#define CONFIG_RV10_DECODER 1
#define CONFIG_RV20_DECODER 1
#define CONFIG_DVBSUB_DECODER 1
#define CONFIG_FLV_DECODER 1
#define CONFIG_SNOW_DECODER 1
#define CONFIG_AMV_DECODER 1
#define CONFIG_LIBSPEEX_DECODER 0
#define CONFIG_LIBSCHROEDINGER_DECODER 0
#define CONFIG_LIBOPENJPEG_DECODER 0
#define CONFIG_LIBDIRAC_DECODER 0
#define CONFIG_ADPCM_IMA_ISS_DECODER 0
#define CONFIG_ADPCM_EA_MAXIS_XA_DECODER 0
#define CONFIG_PCM_F64LE_DECODER 0
#define CONFIG_PCM_F64BE_DECODER 0
#define CONFIG_PCM_F32LE_DECODER 0
#define CONFIG_PCM_F32BE_DECODER 0
#define CONFIG_PCM_DVD_DECODER 0
#define CONFIG_TRUEHD_DECODER 0
#define CONFIG_QCELP_DECODER 0
#define CONFIG_MP1_DECODER 0
#define CONFIG_MLP_DECODER 0
#define CONFIG_ALAC_DECODER 0
#define CONFIG_AAC_DECODER 0
#define CONFIG_PGMYUV_DECODER 0
#define CONFIG_PPM_DECODER 0
#define CONFIG_EATGV_DECODER 0
#define CONFIG_EATQI_DECODER 0
#define CONFIG_EIGHTSVX_EXP_DECODER 0
#define CONFIG_EIGHTSVX_FIB_DECODER 0
#define CONFIG_ESCAPE124_DECODER 0
#define CONFIG_MIMIC_DECODER 0
#define CONFIG_MOTIONPIXELS_DECODER 0
#define CONFIG_PAM_DECODER 0
#define CONFIG_PBM_DECODER 0
#define CONFIG_PGM_DECODER 0
#define CONFIG_RL2_DECODER 0
#define CONFIG_RV30_DECODER 0
#define CONFIG_RV40_DECODER 0
#define CONFIG_V210X_DECODER 0
#define CONFIG_BFI_DECODER 0
#define CONFIG_EACMV_DECODER 0
#define CONFIG_EATGQ_DECODER 0
#define CONFIG_PCX_DECODER 0
#define CONFIG_SUNRAST_DECODER 0
#define CONFIG_VB_DECODER 0
#define CONFIG_XSUB_DECODER 0
#define CONFIG_APE_DECODER 0
#define CONFIG_MPC8_DECODER 0
#define CONFIG_PCM_S16LE_PLANAR_DECODER 0
#define CONFIG_PCM_ZORK_DECODER 0
#define CONFIG_ADPCM_EA_R1_DECODER 0
#define CONFIG_ADPCM_EA_R2_DECODER 0
#define CONFIG_ADPCM_EA_R3_DECODER 0
#define CONFIG_ADPCM_EA_XAS_DECODER 0
#define CONFIG_ADPCM_IMA_EA_EACS_DECODER 0
#define CONFIG_ADPCM_IMA_EA_SEAD_DECODER 0
#define CONFIG_AASC_DECODER 0
#define CONFIG_AVS_DECODER 0
#define CONFIG_BETHSOFTVID_DECODER 0
#define CONFIG_C93_DECODER 0
#define CONFIG_CAVS_DECODER 0
#define CONFIG_CLJR_DECODER 0
#define CONFIG_CSCD_DECODER 0
#define CONFIG_CYUV_DECODER 0
#define CONFIG_DSICINVIDEO_DECODER 0
#define CONFIG_DXA_DECODER 0
#define CONFIG_EIGHTBPS_DECODER 0
#define CONFIG_FLIC_DECODER 0
#define CONFIG_IDCIN_DECODER 0
#define CONFIG_KMVC_DECODER 0
#define CONFIG_LOCO_DECODER 0
#define CONFIG_MDEC_DECODER 0
#define CONFIG_MMVIDEO_DECODER 0
#define CONFIG_MPEG_XVMC_DECODER 0
#define CONFIG_MSRLE_DECODER 0
#define CONFIG_MSZH_DECODER 0
#define CONFIG_NUV_DECODER 0
#define CONFIG_PTX_DECODER 0
#define CONFIG_QDRAW_DECODER 0
#define CONFIG_QPEG_DECODER 0
#define CONFIG_RPZA_DECODER 0
#define CONFIG_SMACKER_DECODER 0
#define CONFIG_SMC_DECODER 0
#define CONFIG_SP5X_DECODER 0
#define CONFIG_THEORA_DECODER 0
#define CONFIG_THP_DECODER 0
#define CONFIG_TIERTEXSEQVIDEO_DECODER 0
#define CONFIG_TRUEMOTION1_DECODER 0
#define CONFIG_TRUEMOTION2_DECODER 0
#define CONFIG_TXD_DECODER 0
#define CONFIG_ULTI_DECODER 0
#define CONFIG_VMDVIDEO_DECODER 0
#define CONFIG_VMNC_DECODER 0
#define CONFIG_VQA_DECODER 0
#define CONFIG_WNV1_DECODER 0
#define CONFIG_XAN_WC3_DECODER 0
#define CONFIG_XL_DECODER 0
#define CONFIG_MPEG4AAC_DECODER 0
#define CONFIG_ALAC_DECODER 0
#define CONFIG_ATRAC3_DECODER 0
#define CONFIG_COOK_DECODER 0
#define CONFIG_DSICINAUDIO_DECODER 0
#define CONFIG_IMC_DECODER 0
#define CONFIG_LIBA52_DECODER 0
#define CONFIG_LIBFAAD_DECODER 0
#define CONFIG_MACE3_DECODER 0
#define CONFIG_MACE6_DECODER 0
#define CONFIG_MP3ADU_DECODER 0
#define CONFIG_MP3ON4_DECODER 0
#define CONFIG_MPC7_DECODER 0
#define CONFIG_RA_144_DECODER 0
#define CONFIG_RA_288_DECODER 0
#define CONFIG_SHORTEN_DECODER 0
#define CONFIG_SMACKAUD_DECODER 0
#define CONFIG_TRUESPEECH_DECODER 0
#define CONFIG_TTA_DECODER 0
#define CONFIG_VMDAUDIO_DECODER 0
#define CONFIG_WAVPACK_DECODER 0
#define CONFIG_WS_SND1_DECODER 0
#define CONFIG_INTERPLAY_DPCM_DECODER 0
#define CONFIG_SOL_DPCM_DECODER 0
#define CONFIG_XAN_DPCM_DECODER 0
#define CONFIG_ADPCM_THP_DECODER 0
#define CONFIG_ASV1_DECODER 0
#define CONFIG_ASV2_DECODER 0
#define CONFIG_BMP_DECODER 0
#define CONFIG_FLASHSV_DECODER 0
#define CONFIG_GIF_DECODER 0
#define CONFIG_H261_DECODER 0
#define CONFIG_H263I_DECODER 0
#define CONFIG_JPEGLS_DECODER 0
#define CONFIG_PNG_DECODER 0
#define CONFIG_QTRLE_DECODER 0
#define CONFIG_RAWVIDEO_DECODER 0
#define CONFIG_ROQ_DECODER 0
#define CONFIG_SGI_DECODER 0
#define CONFIG_SVQ1_DECODER 0
#define CONFIG_TARGA_DECODER 0
#define CONFIG_TIFF_DECODER 0
#define CONFIG_ZLIB_DECODER 0
#define CONFIG_ZMBV_DECODER 0
#define CONFIG_FLAC_DECODER 0
#define CONFIG_LIBAMR_NB_DECODER 0
#define CONFIG_LIBAMR_WB_DECODER 0
#define CONFIG_LIBGSM_DECODER 0
#define CONFIG_LIBGSM_MS_DECODER 0
#define CONFIG_SONIC_DECODER 0
#define CONFIG_VORBIS_DECODER 0
#define CONFIG_WMAV1_DECODER 0
#define CONFIG_PCM_ALAW_DECODER 0
#define CONFIG_PCM_MULAW_DECODER 0
#define CONFIG_PCM_S8_DECODER 0
#define CONFIG_PCM_S16BE_DECODER 0
#define CONFIG_PCM_S16LE_DECODER 0
#define CONFIG_PCM_S24BE_DECODER 0
#define CONFIG_PCM_S24DAUD_DECODER 0
#define CONFIG_PCM_S24LE_DECODER 0
#define CONFIG_PCM_S32BE_DECODER 0
#define CONFIG_PCM_S32LE_DECODER 0
#define CONFIG_PCM_U8_DECODER 0
#define CONFIG_PCM_U16BE_DECODER 0
#define CONFIG_PCM_U16LE_DECODER 0
#define CONFIG_PCM_U24BE_DECODER 0
#define CONFIG_PCM_U24LE_DECODER 0
#define CONFIG_PCM_U32BE_DECODER 0
#define CONFIG_PCM_U32LE_DECODER 0
#define CONFIG_ROQ_DPCM_DECODER 0
#define CONFIG_ADPCM_4XM_DECODER 0
#define CONFIG_ADPCM_ADX_DECODER 0
#define CONFIG_ADPCM_CT_DECODER 0
#define CONFIG_ADPCM_EA_DECODER 0
#define CONFIG_ADPCM_G726_DECODER 0
#define CONFIG_ADPCM_IMA_DK3_DECODER 0
#define CONFIG_ADPCM_IMA_DK4_DECODER 0
#define CONFIG_ADPCM_IMA_QT_DECODER 0
#define CONFIG_ADPCM_IMA_SMJPEG_DECODER 0
#define CONFIG_ADPCM_IMA_WAV_DECODER 0
#define CONFIG_ADPCM_IMA_WS_DECODER 0
#define CONFIG_ADPCM_MS_DECODER 0
#define CONFIG_ADPCM_SBPRO_2_DECODER 0
#define CONFIG_ADPCM_SBPRO_3_DECODER 0
#define CONFIG_ADPCM_SBPRO_4_DECODER 0
#define CONFIG_ADPCM_SWF_DECODER 0
#define CONFIG_ADPCM_XA_DECODER 0
#define CONFIG_ADPCM_YAMAHA_DECODER 0
#define CONFIG_DVDSUB_DECODER 0
#define CONFIG_LIBVORBIS_DECODER 0
#define CONFIG_H263_PARSER 1
#define CONFIG_H264_PARSER 1
#define CONFIG_MPEG4VIDEO_PARSER 1
#define CONFIG_AAC_PARSER 0
#define CONFIG_AC3_PARSER 0
#define CONFIG_CAVSVIDEO_PARSER 0
#define CONFIG_DCA_PARSER 0
#define CONFIG_DVBSUB_PARSER 0
#define CONFIG_DVDSUB_PARSER 0
#define CONFIG_H261_PARSER 0
#define CONFIG_MJPEG_PARSER 0
#define CONFIG_MPEGAUDIO_PARSER 0
#define CONFIG_MPEGVIDEO_PARSER 0
#define CONFIG_PNM_PARSER 0
#define CONFIG_VC1_PARSER 0
#define CONFIG_VP3_PARSER 0
#define CONFIG_DNXHD_PARSER 0
#define CONFIG_DIRAC_PARSER 0
#define CONFIG_MJPEG_ENCODER 1
#define CONFIG_MSMPEG4V3_ENCODER 1
#define CONFIG_H263P_ENCODER 1
#define CONFIG_AC3_ENCODER 1
#define CONFIG_FFV1_ENCODER 1
#define CONFIG_FLV1_ENCODER 1
#define CONFIG_FFVHUFF_ENCODER 1
#define CONFIG_H263_ENCODER 1
#define CONFIG_MPEG1VIDEO_ENCODER 1
#define CONFIG_MPEG2VIDEO_ENCODER 1
#define CONFIG_MPEG4_ENCODER 1
#define CONFIG_FLV_ENCODER 1
#define CONFIG_MP2_ENCODER 1
#define CONFIG_DVVIDEO_ENCODER 1
#define CONFIG_HUFFYUV_ENCODER 1
#define CONFIG_DVBSUB_ENCODER 1
#define CONFIG_SNOW_ENCODER 1
#define CONFIG_LIBSCHROEDINGER_ENCODER 0
#define CONFIG_LIBDIRAC_ENCODER 0
#define CONFIG_PCM_F64LE_ENCODER 0
#define CONFIG_PCM_F64BE_ENCODER 0
#define CONFIG_PCM_F32BE_ENCODER 0
#define CONFIG_PCM_F32LE_ENCODER 0
#define CONFIG_NELLYMOSER_ENCODER 0
#define CONFIG_ALAC_ENCODER 0
#define CONFIG_PCX_ENCODER 0
#define CONFIG_DNXHD_ENCODER 0
#define CONFIG_PCM_ZORK_ENCODER 0
#define CONFIG_ADPCM_IMA_AMV_ENCODER 0
#define CONFIG_LIBX264_ENCODER 0
#define CONFIG_LIBXVID_ENCODER 0
#define CONFIG_LJPEG_ENCODER 0
#define CONFIG_PAM_ENCODER 0
#define CONFIG_PBM_ENCODER 0
#define CONFIG_PGM_ENCODER 0
#define CONFIG_PGMYUV_ENCODER 0
#define CONFIG_PPM_ENCODER 0
#define CONFIG_LIBFAAC_ENCODER 0
#define CONFIG_LIBMP3LAME_ENCODER 0
#define CONFIG_LIBTHEORA_ENCODER 0
#define CONFIG_SONIC_LS_ENCODER 0
#define CONFIG_ASV1_ENCODER 0
#define CONFIG_ASV2_ENCODER 0
#define CONFIG_BMP_ENCODER 0
#define CONFIG_FLASHSV_ENCODER 0
#define CONFIG_GIF_ENCODER 0
#define CONFIG_H261_ENCODER 0
#define CONFIG_JPEGLS_ENCODER 0
#define CONFIG_MSMPEG4V1_ENCODER 0
#define CONFIG_MSMPEG4V2_ENCODER 0
#define CONFIG_PNG_ENCODER 0
#define CONFIG_QTRLE_ENCODER 0
#define CONFIG_RAWVIDEO_ENCODER 0
#define CONFIG_ROQ_ENCODER 0
#define CONFIG_RV10_ENCODER 0
#define CONFIG_RV20_ENCODER 0
#define CONFIG_SGI_ENCODER 0
#define CONFIG_SVQ1_ENCODER 0
#define CONFIG_TARGA_ENCODER 0
#define CONFIG_TIFF_ENCODER 0
#define CONFIG_WMV1_ENCODER 0
#define CONFIG_WMV2_ENCODER 0
#define CONFIG_ZLIB_ENCODER 0
#define CONFIG_ZMBV_ENCODER 0
#define CONFIG_FLAC_ENCODER 0
#define CONFIG_LIBAMR_NB_ENCODER 0
#define CONFIG_LIBAMR_WB_ENCODER 0
#define CONFIG_LIBGSM_ENCODER 0
#define CONFIG_LIBGSM_MS_ENCODER 0
#define CONFIG_SONIC_ENCODER 0
#define CONFIG_VORBIS_ENCODER 0
#define CONFIG_WMAV1_ENCODER 0
#define CONFIG_WMAV2_ENCODER 0
#define CONFIG_PCM_ALAW_ENCODER 0
#define CONFIG_PCM_MULAW_ENCODER 0
#define CONFIG_PCM_S8_ENCODER 0
#define CONFIG_PCM_S16BE_ENCODER 0
#define CONFIG_PCM_S16LE_ENCODER 0
#define CONFIG_PCM_S24BE_ENCODER 0
#define CONFIG_PCM_S24DAUD_ENCODER 0
#define CONFIG_PCM_S24LE_ENCODER 0
#define CONFIG_PCM_S32BE_ENCODER 0
#define CONFIG_PCM_S32LE_ENCODER 0
#define CONFIG_PCM_U8_ENCODER 0
#define CONFIG_PCM_U16BE_ENCODER 0
#define CONFIG_PCM_U16LE_ENCODER 0
#define CONFIG_PCM_U24BE_ENCODER 0
#define CONFIG_PCM_U24LE_ENCODER 0
#define CONFIG_PCM_U32BE_ENCODER 0
#define CONFIG_PCM_U32LE_ENCODER 0
#define CONFIG_ROQ_DPCM_ENCODER 0
#define CONFIG_ADPCM_4XM_ENCODER 0
#define CONFIG_ADPCM_ADX_ENCODER 0
#define CONFIG_ADPCM_CT_ENCODER 0
#define CONFIG_ADPCM_EA_ENCODER 0
#define CONFIG_ADPCM_G726_ENCODER 0
#define CONFIG_ADPCM_IMA_DK3_ENCODER 0
#define CONFIG_ADPCM_IMA_DK4_ENCODER 0
#define CONFIG_ADPCM_IMA_QT_ENCODER 0
#define CONFIG_ADPCM_IMA_SMJPEG_ENCODER 0
#define CONFIG_ADPCM_IMA_WAV_ENCODER 0
#define CONFIG_ADPCM_IMA_WS_ENCODER 0
#define CONFIG_ADPCM_MS_ENCODER 0
#define CONFIG_ADPCM_SBPRO_2_ENCODER 0
#define CONFIG_ADPCM_SBPRO_3_ENCODER 0
#define CONFIG_ADPCM_SBPRO_4_ENCODER 0
#define CONFIG_ADPCM_SWF_ENCODER 0
#define CONFIG_ADPCM_XA_ENCODER 0
#define CONFIG_ADPCM_YAMAHA_ENCODER 0
#define CONFIG_DVDSUB_ENCODER 0
#define CONFIG_LIBVORBIS_ENCODER 0
#define CONFIG_MP3_HEADER_COMPRESS_BSF 0
#define CONFIG_IMX_DUMP_HEADER_BSF 0
#define CONFIG_DUMP_EXTRADATA_BSF 0
#define CONFIG_REMOVE_EXTRADATA_BSF 0
#define CONFIG_NOISE_BSF 0
#define CONFIG_MP3_HEADER_DECOMPRESS_BSF 0
#define CONFIG_MJPEGA_DUMP_HEADER_BSF 0
#define CONFIG_H264_MP4TOANNEXB_BSF 0
#define CONFIG_MOV2TEXTSUB_BSF 0
#define CONFIG_TEXT2MOVSUB_BSF 0
#define CONFIG_MUXERS 1
#define CONFIG_MOV_MUXER 1
#define CONFIG_IPOD_MUXER 1
#define CONFIG_MP4_MUXER 1
#define CONFIG_PSP_MUXER 1
#define CONFIG_TG2_MUXER 1
#define CONFIG_TGP_MUXER 1
#define CONFIG_ENCODERS 1
#define CONFIG_DVVIDEO_ENCODER 1
#define CONFIG_SNOW_ENCODER 1
#define CONFIG_DECODERS 1
#define CONFIG_DVVIDEO_DECODER 1
#define CONFIG_H263_DECODER 1
#define CONFIG_MPEG4_DECODER 1
#define CONFIG_SNOW_DECODER 1
#define CONFIG_VC1_DECODER 1
#define CONFIG_WMV2_DECODER 1
#define CONFIG_WMV3_DECODER 1
#define CONFIG_MPEGAUDIO_HP 1
#define CONFIG_ZLIB 1
#define CONFIG_GPL 1
#define CONFIG_ARMV4L 0
#define CONFIG_MLIB 0
#define CONFIG_SPARC 0
#define CONFIG_ALPHA 0
#define CONFIG_MMI 0
#define CONFIG_SH4 0
#define CONFIG_BFIN 0
#define CONFIG_SMALL 0
#define CONFIG_MLP_PARSER 0
//****************** SYSTEM *******************
#	define ARCH_ARM 0
#	define ARCH_ALPHA 0
#	define ARCH_PPC 0
#	define ARCH_SH4 0
#	define ARCH_BFIN 0
#	define HAVE_MMI 0
#	define HAVE_VIS 0
#	define CONFIG_GRAY 0
#	define HAVE_TRUNCF 1
#ifdef __APPLE__
#	define CONFIG_DARWIN 1
#endif
#if defined(ADM_CPU_X86_32)
#	define ARCH_X86 1
#	define ARCH_X86_32 1
#elif defined(ADM_CPU_X86_64)
#	define ARCH_X86 1
#	define ARCH_X86_64 1
#elif defined(ADM_CPU_PPC)
#	define ARCH_POWERPC 1
#ifdef ADM_CPU_ALTIVEC
#	define HAVE_ALTIVEC 1
#ifndef __APPLE__
#	define HAVE_ALTIVEC_H 1
#endif	// __APPLE__
#endif	// ADM_CPU_ALTIVEC
#ifdef ADM_CPU_DCBZL
#	define HAVE_DCBZL 1
#endif	// ADM_CPU_DCBZL
#endif	// ADM_CPU_X86_32
#ifndef ADM_MINIMAL_INCLUDE
#if defined(ADM_CPU_X86_32) && defined(ADM_CPU_MMX2)
#	define HAVE_MMX2 1
#endif
#ifdef ADM_CPU_SSSE3
#	define HAVE_SSSE3 1
#endif
#endif //ADM_MINIMAL_INCLUDE
#ifdef ARCH_POWERPC
#	define ENABLE_POWERPC 1
#else
#	define ENABLE_POWERPC 0
#endif
#ifdef ARCH_X86
#	define ENABLE_BSWAP 1
#ifndef ADM_MINIMAL_INCLUDE
#	define HAVE_MMX 1
#	define ENABLE_MMX 1
#endif //ADM_MINIMAL_INCLUDE
#	define HAVE_FAST_UNALIGNED 1
#	define HAVE_EBP_AVAILABLE 1
#	define HAVE_EBX_AVAILABLE 1
#else
#	define ENABLE_MMX 0
#endif
#ifdef ARCH_X86_64
#	define HAVE_FAST_64BIT 1
#endif
#ifdef ADM_BIG_ENDIAN
#	define WORDS_BIGENDIAN 1
#endif
#define ENABLE_YASM      0
#define ENABLE_ARM      0
#define ENABLE_PPC      0
#define ENABLE_THREADS 1
#define ENABLE_ENCODERS 1
#define CONFIG_MUXERS 1
#define CONFIG_DEMUXERS 1
#define ENABLE_VIS 0
#define ENABLE_GRAY 0
#define HAVE_LRINTF 1
#define HAVE_LLRINT 1
#define HAVE_LRINT 1
#define HAVE_ROUND 1
#define HAVE_ROUNDF 1
#define HAVE_THREADS 1
#define RUNTIME_CPUDETECT 1
#ifdef __MINGW32__
#define EXTERN_PREFIX "_"
#else // __MINGW32__
#define EXTERN_PREFIX
#endif // __MINGW32__
#define restrict __restrict__
