#ifndef ADM_CODEC_CONFIG_
#define ADM_CODEC_CONFIG_

#define REQUANT_AS_CODE
#include "ADM_vidEncode.hxx"
// Yv12
extern uint8_t DIA_requant(COMPRES_PARAMS *incoming);

COMPRES_PARAMS yv12codec = {
  CodecYV12,
  QT_TR_NOOP("YV12 (raw)"),
  "YV12",
  "YV12",
  COMPRESS_CQ,
  1,
  1500,
  700,
  1000,
  ADM_ENC_CAP_CQ,
  0,
  NULL,
  0,
  NULL
};

#include "ADM_libraries/ADM_libmpeg2enc/ADM_mpeg2enc.h"
// ************ Mpeg2enc VCD *************
Mpeg2encParam VCDExtra = {
  (1152 * 1000) >> 3,		// Max BR
  18,				// Gop size
  0,				//int   wideScreen;
  0,				//int   matrix;
  0,				//int   interlacingType;
  0				// bff
};


COMPRES_PARAMS VCDCodec = {
  CodecVCD,
  QT_TR_NOOP("VCD (mpeg2enc)"),
  "VCD",
  "Mpeg2enc (VCD)",
  COMPRESS_CBR,
  4,
  1500,
  700,
  1000, // AVG
  ADM_ENC_CAP_CBR,
  ADM_EXTRA_PARAM,
  &VCDExtra,
  sizeof (VCDExtra),
  NULL
};
//************  MPEG2Enc SVCD *******************



extern uint8_t DIA_SVCDParam (COMPRES_PARAMS * toto);


Mpeg2encParam SVCDExtra = {
  2500 ,		// Max BR
  12,				// Gop size
  0,				//int   wideScreen;
  0,				//int   matrix;
  0,				//int   interlacingType;
  0				// bff
};

COMPRES_PARAMS SVCDCodec = {
  CodecSVCD,
  QT_TR_NOOP("SVCD (mpeg2enc)"),
  "SVCD",
  "Mpeg2enc (SVCD)",
  COMPRESS_CQ,
  4,
  1500,
  700,
  1000, // AVG
  ADM_ENC_CAP_CQ + ADM_ENC_CAP_2PASS+ADM_ENC_CAP_2PASS_BR,
  ADM_EXTRA_PARAM,
  &SVCDExtra,
  sizeof (SVCDExtra),
  DIA_SVCDParam
};
//*******************
#if defined REQUANT_AS_CODE
uint32_t RequantFactorExtra=1000; // 1000* the actual requant factor
COMPRES_PARAMS RequantCodec = {
    CodecRequant,
    QT_TR_NOOP("MPEG-2 requant"),
    "REQUANT",
    "Mpeg2 Requantizer",
    COMPRESS_CQ,
    4,
    1500,
    700,
    1000, // AVG
    ADM_ENC_CAP_CQ,
    ADM_EXTRA_PARAM,
    &RequantFactorExtra,
    sizeof (RequantFactorExtra),
    DIA_requant
};
#endif
//********************************************
Mpeg2encParam DVDExtra = {
  9000            ,		// Max BR
  12,				// Gop size
  0,				//int   wideScreen;
  0,				//int   matrix;
  0,				//int   interlacingType;
  0				// bff
};

COMPRES_PARAMS DVDCodec = {
  CodecDVD,
  QT_TR_NOOP("DVD (mpeg2enc)"),
  "DVD",
  "Mpeg2enc (DVD)",
  COMPRESS_CQ,
  4,
  1500,
  700,
  1000, // AVG
  ADM_ENC_CAP_CBR + ADM_ENC_CAP_CQ + ADM_ENC_CAP_2PASS,
  ADM_EXTRA_PARAM,
  &DVDExtra,
  sizeof (DVDExtra),
  DIA_SVCDParam
};

COMPRES_PARAMS DUMMYONE =
  { CodecDummy, QT_TR_NOOP("dummy"), "dummy", "dummy", COMPRESS_CQ, 4, 1500, 700,1000, 0, 0,
NULL, 0 };
COMPRES_PARAMS CopyCodec =
  { CodecCopy, QT_TR_NOOP("Copy"), "Copy", "Copy", COMPRESS_CQ, 4, 1500, 700,1000, 0, 0, NULL,
0 };

COMPRES_PARAMS *internalVideoCodec[] = {
  &CopyCodec,
  &VCDCodec,
  &SVCDCodec,
  &DVDCodec,
  &RequantCodec,
  &yv12codec,
  &DUMMYONE
};

int getInternalVideoCodecCount()
{
	return (sizeof(internalVideoCodec) / sizeof(COMPRES_PARAMS*)) - 1;	// There is a dummy extra one at the end
}
#endif
