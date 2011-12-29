/***************************************************************************
                                  encoder.h

    begin                : Thu Apr 10 2009
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

#define __STDC_CONSTANT_MACROS
#include <stdint.h>

extern "C"
{
	#include "ADM_vidEnc_plugin.h"
	#include "libavcodec/avcodec.h"
}

class AvcodecEncoder
{
	protected:
		enum CodecID _codecId;
		int _supportedCsps[1];

		unsigned int _width, _height;
		unsigned int _fpsNum, _fpsDen;
		unsigned int _frameCount;

		int _currentPass, _passCount;
		bool _opened, _openPass;

		AVCodecContext *_context;
		AVFrame _frame;

		int _bufferSize;
		uint8_t *_buffer;

		virtual void init(enum CodecID id, int targetColourSpace);
		virtual int initContext(const char* logFileName);
		AVCodec *getAvCodec(void);
		enum PixelFormat getAvCodecColourSpace(int colourSpace);
		virtual int getFrameType(void);
		virtual void updateEncodeParameters(vidEncEncodeParameters *encodeParams, uint8_t *buffer, int bufferSize);
		void printContext(void);

	public:
		virtual ~AvcodecEncoder(void);
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
	void *avcodecEncoder_getPointers(int uiType, int *count);
	const char* avcodecEncoder_getEncoderName(int encoderId);
	const char* avcodecEncoder_getEncoderType(int encoderId);
	const char* avcodecEncoder_getEncoderDescription(int encoderId);
	const char* avcodecEncoder_getFourCC(int encoderId);
	int avcodecEncoder_getEncoderRequirements(int encoderId);
	const char* avcodecEncoder_getEncoderGuid(int encoderId);
	int avcodecEncoder_isConfigurable(int encoderId);
	int avcodecEncoder_configure(int encoderId, vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
	int avcodecEncoder_getOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
	int avcodecEncoder_setOptions(int encoderId, vidEncOptions *encodeOptions, const char *pluginOptions);
	int avcodecEncoder_getPassCount(int encoderId);
	int avcodecEncoder_getCurrentPass(int encoderId);
	int avcodecEncoder_open(int encoderId, vidEncVideoProperties *properties);
	int avcodecEncoder_beginPass(int encoderId, vidEncPassParameters *passParameters);
	int avcodecEncoder_encodeFrame(int encoderId, vidEncEncodeParameters *encodeParams);
	int avcodecEncoder_finishPass(int encoderId);
	int avcodecEncoder_close(int encoderId);
#endif	// __cplusplus
#endif	// encoder_h
