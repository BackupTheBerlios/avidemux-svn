#ifndef ADM_CODEC_CONFIG_
#define ADM_CODEC_CONFIG_

#include "ADM_vidEncode.hxx"
// Yv12
extern uint8_t DIA_requant(COMPRES_PARAMS *incoming);

CODEC_INFO yv12codecInfo = {
  QT_TR_NOOP("YV12 (raw)"),
  "YV12",
  "YV12"
};

COMPRES_PARAMS yv12codec = {
  CodecYV12,
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

uint32_t RequantFactorExtra=1000; // 1000* the actual requant factor

CODEC_INFO RequantCodecInfo = {
    QT_TR_NOOP("MPEG-2 requant"),
    "REQUANT",
    "Mpeg2 Requantizer"
};

COMPRES_PARAMS RequantCodec = {
    CodecRequant,
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

CODEC_INFO CopyCodecInfo = {
	QT_TR_NOOP("Copy"),
	"Copy",
	"Copy"
};

COMPRES_PARAMS CopyCodec = { CodecCopy, COMPRESS_CQ, 4, 1500, 700,1000, 0, 0, NULL, 0 };

CODEC_INFO *internalVideoCodecInfo[] = {
  &CopyCodecInfo,
  &RequantCodecInfo,
  &yv12codecInfo
};

COMPRES_PARAMS *internalVideoCodec[] = {
  &CopyCodec,
  &RequantCodec,
  &yv12codec
};

int getInternalVideoCodecCount()
{
	return (sizeof(internalVideoCodec) / sizeof(COMPRES_PARAMS*));
}
#endif
