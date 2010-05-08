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

COMPRES_PARAMS DUMMYONE =
  { CodecDummy, QT_TR_NOOP("dummy"), "dummy", "dummy", COMPRESS_CQ, 4, 1500, 700,1000, 0, 0,
NULL, 0 };
COMPRES_PARAMS CopyCodec =
  { CodecCopy, QT_TR_NOOP("Copy"), "Copy", "Copy", COMPRESS_CQ, 4, 1500, 700,1000, 0, 0, NULL,
0 };

COMPRES_PARAMS *internalVideoCodec[] = {
  &CopyCodec,
  &RequantCodec,
  &yv12codec,
  &DUMMYONE
};

int getInternalVideoCodecCount()
{
	return (sizeof(internalVideoCodec) / sizeof(COMPRES_PARAMS*)) - 1;	// There is a dummy extra one at the end
}
#endif
