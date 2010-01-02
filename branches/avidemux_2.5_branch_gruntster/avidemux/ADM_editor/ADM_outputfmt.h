//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// (c) Mean, fixounet@free.fr
//
// Copyright: See COPYING file that comes with this distribution
// 2002-2004
//
#ifndef ADM_OUT_FMT
#define ADM_OUT_FMT

#include "config.h"
#include "ADM_outputs/oplug_mpegFF/ps_muxer.h"

extern ps_muxer psMuxerConfig;
extern bool muxerMpegPsConfigure(void);
extern bool ADM_aviUISetMuxer(void);

typedef enum 
{
	ADM_AVI=0,
	ADM_AVI_DUAL,
	ADM_AVI_PAK,
	ADM_AVI_UNP,
	ADM_PS,
	ADM_TS,
	ADM_ES,
	ADM_MP4,
	ADM_PSP,
	ADM_OGM,
	ADM_FLV,
        ADM_MATROSKA,
	ADM_DUMMY,
	ADM_FORMAT_MAX,
} ADM_OUT_FORMAT;

typedef struct 
{
  ADM_OUT_FORMAT format;
  const char *text;
  bool (*muxerConfigure)(void);
  int configSize;
  const void *defaultConfig;
  void *currentConfig;
} ADM_FORMAT_DESC;
/**
 * 	This is used to fill-in the menus in GUIs
 */
const ADM_FORMAT_DESC ADM_allOutputFormat[]=
{
  {ADM_AVI, QT_TR_NOOP("AVI"), ADM_aviUISetMuxer, 0, NULL, NULL},
  {ADM_AVI_DUAL, QT_TR_NOOP("AVI, dual audio"), ADM_aviUISetMuxer, 0, NULL, NULL},
  {ADM_AVI_PAK, QT_TR_NOOP("AVI, pack VOP"), ADM_aviUISetMuxer, 0, NULL, NULL},
  {ADM_AVI_UNP, QT_TR_NOOP("AVI, unpack VOP"), ADM_aviUISetMuxer, 0, NULL, NULL},
  {ADM_PS, QT_TR_NOOP("MPEG-PS (A+V)"), muxerMpegPsConfigure, sizeof(ps_muxer), &ps_muxer_default, &psMuxerConfig},
  {ADM_TS, QT_TR_NOOP("MPEG-TS (A+V)"), NULL, 0, NULL, NULL},
  {ADM_ES, QT_TR_NOOP("MPEG video"), NULL, 0, NULL, NULL},
  {ADM_MP4, QT_TR_NOOP("MP4"), NULL, 0, NULL, NULL},
  {ADM_PSP, QT_TR_NOOP("MP4 (PSP)"), NULL, 0, NULL, NULL},
  {ADM_OGM, QT_TR_NOOP("OGM"), NULL, 0, NULL, NULL},
  {ADM_FLV, QT_TR_NOOP("FLV"), NULL, 0, NULL, NULL},
  {ADM_MATROSKA, QT_TR_NOOP("MKV"), NULL, 0, NULL, NULL},
  {ADM_DUMMY, QT_TR_NOOP("DUMMY"), NULL, 0, NULL, NULL}
};

#endif
//EOF
