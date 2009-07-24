/***************************************************************************
                            ADM_vidEnc_plugin.h

    begin                : Mon Apr 14 2008
    copyright            : (C) 2008 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ADM_vidEnc_plugin_h
#define ADM_vidEnc_plugin_h

#define ADM_VIDENC_API_VERSION 2

#define ADM_VIDENC_ERR_SUCCESS 1
#define ADM_VIDENC_ERR_FAILED 0
#define ADM_VIDENC_ERR_CLOSED -1
#define ADM_VIDENC_ERR_ALREADY_OPEN -2
#define ADM_VIDENC_ERR_INVALID_ENCODER_ID -3
#define ADM_VIDENC_ERR_PASS_SKIP -4
#define ADM_VIDENC_ERR_PASS_CLOSED -5
#define ADM_VIDENC_ERR_PASS_ALREADY_OPEN -6
#define ADM_VIDENC_ERR_PASS_COUNT_REACHED -7

#define ADM_VIDENC_MODE_CBR 1
#define ADM_VIDENC_MODE_CQP 2
#define ADM_VIDENC_MODE_AQP 3
#define ADM_VIDENC_MODE_2PASS_SIZE 4
#define ADM_VIDENC_MODE_2PASS_ABR 5

#define ADM_VIDENC_FRAMETYPE_NULL 1
#define ADM_VIDENC_FRAMETYPE_IDR 2
#define ADM_VIDENC_FRAMETYPE_B 3
#define ADM_VIDENC_FRAMETYPE_P 4

#define ADM_VIDENC_REQ_NONE 0
#define ADM_VIDENC_REQ_NULL_FLUSH 1

#define ADM_CSP_YV12 1
#define ADM_CSP_I422 2

#include "DIA_uiTypes.h"

#define ADM_VIDENC_FLAG_GLOBAL_HEADER 1

typedef struct
{
	// In
	int structSize;
	uint8_t *frameData[4];
	int frameLineSize[4];
	int frameDataSize;
	// Out
	uint8_t *encodedData;
	int encodedDataSize;
	int64_t ptsFrame;
	unsigned int quantiser;
	int frameType;
} vidEncEncodeParameters;

typedef struct
{
	// In
	int structSize;
	unsigned int width;
	unsigned int height;
	unsigned int parWidth;
	unsigned int parHeight;
	unsigned int frameCount;
	unsigned int fpsNum;
	unsigned int fpsDen;
	unsigned int flags;
	// Out
	unsigned int supportedCspsCount;
	int *supportedCsps;

} vidEncVideoProperties;

typedef struct
{
	// In
	int structSize;
	// In/Out
	unsigned int encodeMode;
	int encodeModeParameter;
} vidEncOptions;

typedef struct
{
	// In
	int structSize;
	int useExistingLogFile;
	const char* logFileName;
	int csp;
	// Out
	uint8_t *extraData;
	int extraDataSize;
} vidEncPassParameters;

typedef struct
{
	// In
	int structSize;
	intptr_t parent;
	intptr_t parentNative;
} vidEncConfigParameters;

int vidEncGetEncoders(int uiType, int **encoderIds);
const char* vidEncGetEncoderName(int encoderId);
const char* vidEncGetEncoderType(int encoderId);
const char* vidEncGetEncoderDescription(int encoderId);
const char* vidEncGetFourCC(int encoderId);
int vidEncGetEncoderApiVersion(int encoderId);
void vidEncGetEncoderVersion(int encoderId, int *major, int *minor, int *patch);
const char* vidEncGetEncoderGuid(int encoderId);
int vidEncGetEncoderRequirements(int encoderId);

int vidEncIsConfigurable(int encoderId);
int vidEncConfigure(int encoderId, vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
int vidEncGetOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
int vidEncSetOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions);

int vidEncGetPassCount(int encoderId);
int vidEncGetCurrentPass(int encoderId);

int vidEncOpen(int encoderId, vidEncVideoProperties *properties);
int vidEncBeginPass(int encoderId, vidEncPassParameters *passParameters);
int vidEncEncodeFrame(int encoderId, vidEncEncodeParameters *encodeParams);
int vidEndFinishPass(int encoderId);
int vidEncClose(int encoderId);

#endif	// ADM_vidEnc_plugin_h
