 /***************************************************************************
                                  encoder.h

    begin                : Thu Dec 31 2009
    copyright            : (C) 2009 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef encoder_h
#define encoder_h

#ifdef __cplusplus
#include <inttypes.h>

extern "C"
{
	#include "ADM_vidEnc_plugin.h"
}

#include "mpeg2parm.h"
#include "xvidRateCtlVbv.h"

class Mpeg2encEncoder
{
	protected:
		unsigned int _width, _height;
		unsigned int _fpsNum, _fpsDen;
		unsigned int _frameCount;

		int _currentPass, _passCount;
		bool _opened, _openPass;

		uint8_t *_buffer;
		int _bufferSize;

		vidEncOptions _encodeOptions;
		mpeg2parm _param;
		ADM_newXvidRcVBV *_xvidRc;

		int getFrameType(int flags);
		unsigned int calculateBitrate(unsigned int fpsNum, unsigned int fpsDen, unsigned int frameCount, unsigned int sizeInMb);
		virtual void initParameters(int *encodeModeParameter, int *maxBitrate, int *vbv) = 0;

	public:
		virtual ~Mpeg2encEncoder(void);
		virtual const char* getEncoderName(void);
		virtual const char* getEncoderType(void) = 0;
		virtual const char* getEncoderDescription(void) = 0;
		virtual const char* getFourCC(void) = 0;
		virtual int getEncoderRequirements(void);
		virtual const char* getEncoderGuid(void) = 0;
		virtual int isConfigurable(void);
		virtual int configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
		virtual int getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize) = 0;
		virtual int setOptions(vidEncOptions *encodeOptions, const char *pluginOptions) = 0;
		virtual int getCurrentPass(void);
		virtual int getPassCount(void);
		virtual int open(vidEncVideoProperties *properties);
		virtual int beginPass(vidEncPassParameters *passParameters);
		virtual int encodeFrame(vidEncEncodeParameters *encodeParams);
		virtual int finishPass(void);
		virtual int close(void);
};
#else
	void *mpeg2encEncoder_getPointers(int uiType, int *count);
	const char* mpeg2encEncoder_getEncoderName(int encoderId);
	const char* mpeg2encEncoder_getEncoderType(int encoderId);
	const char* mpeg2encEncoder_getEncoderDescription(int encoderId);
	const char* mpeg2encEncoder_getFourCC(int encoderId);
	int mpeg2encEncoder_getEncoderRequirements(int encoderId);
	const char* mpeg2encEncoder_getEncoderGuid(int encoderId);
	int mpeg2encEncoder_isConfigurable(int encoderId);
	int mpeg2encEncoder_configure(int encoderId, vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
	int mpeg2encEncoder_getOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
	int mpeg2encEncoder_setOptions(int encoderId, vidEncOptions *encodeOptions, const char *pluginOptions);
	int mpeg2encEncoder_getPassCount(int encoderId);
	int mpeg2encEncoder_getCurrentPass(int encoderId);
	int mpeg2encEncoder_open(int encoderId, vidEncVideoProperties *properties);
	int mpeg2encEncoder_beginPass(int encoderId, vidEncPassParameters *passParameters);
	int mpeg2encEncoder_encodeFrame(int encoderId, vidEncEncodeParameters *encodeParams);
	int mpeg2encEncoder_finishPass(int encoderId);
	int mpeg2encEncoder_close(int encoderId);
#endif	// __cplusplus
#endif	// encoder_h
