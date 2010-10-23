/***************************************************************************
                               mpeg1Encoder.h

    begin                : Sat Jul 4 2009
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

#ifndef Mpeg1Encoder_h
#define Mpeg1Encoder_h

#ifdef __cplusplus
extern "C"
{
	#include "ADM_vidEnc_plugin.h"
}

#include "encoder.h"
#include "mpeg1EncoderOptions.h"
#include "DIA_factory.h"
#include "xvidRateCtlVbv.h"

class Mpeg1Encoder : public AvcodecEncoder
{
	private:
		COMPRES_PARAMS _bitrateParam;
		unsigned int _minBitrate, _maxBitrate, _useXvidRateControl, _bufferSize, _widescreen, _interlaced, _userMatrix, _gopSize;

		char configName[PATH_MAX];
		ConfigMenuType configType;

		Mpeg1EncoderOptions _options;
		vidEncOptions _encodeOptions;

		FILE *_statFile;
		ADM_newXvidRcVBV *_xvidRc;

		void updateEncodeProperties(vidEncOptions *encodeOptions);
		unsigned int calculateBitrate(unsigned int fpsNum, unsigned int fpsDen, unsigned int frameCount, unsigned int sizeInMb);

	public:
		Mpeg1Encoder(void);
		int initContext(const char* logFileName);
		const char* getEncoderType(void);
		const char* getEncoderDescription(void);
		const char* getFourCC(void);
		const char* getEncoderGuid(void);
		int isConfigurable(void);
		int configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
		void loadSettings(vidEncOptions *encodeOptions, Mpeg1EncoderOptions *options);
		void saveSettings(vidEncOptions *encodeOptions, Mpeg1EncoderOptions *options);
		int getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
		int setOptions(vidEncOptions *encodeOptions, const char *pluginOptions);
		int beginPass(vidEncPassParameters *passParameters);
		int encodeFrame(vidEncEncodeParameters *encodeParams);
		int finishPass(void);
};
#endif	// __cplusplus
#endif	// Mpeg1Encoder_h
