 /***************************************************************************
                              mpeg4aspEncoder.h

    begin                : Wed Dec 30 2009
    copyright            : (C) 2009 by gruntster
 ***************************************************************************/

 /**************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef Mpeg4aspEncoder_h
#define Mpeg4aspEncoder_h

#ifdef __cplusplus
extern "C"
{
	#include "ADM_vidEnc_plugin.h"
}

#include "encoder.h"
#include "mpeg4aspEncoderOptions.h"
#include "DIA_factory.h"

class Mpeg4aspEncoder : public AvcodecEncoder
{
	private:
		COMPRES_PARAMS _bitrateParam;
		unsigned int _motionEst, _4MV, _maxBFrames, _qpel, _gmc;
		unsigned int _quantType, _mbDecision, _minQuantiser, _maxQuantiser, _maxQuantiserDiff, _trellis;
		float _quantCompression, _quantBlur;

		char configName[PATH_MAX];
		ConfigMenuType configType;

		Mpeg4aspEncoderOptions _options;
		vidEncOptions _encodeOptions;

		FILE *_statFile;

		void updateEncodeProperties(vidEncOptions *encodeOptions);
		unsigned int calculateBitrate(unsigned int fpsNum, unsigned int fpsDen, unsigned int frameCount, unsigned int sizeInMb);

	public:
		Mpeg4aspEncoder(void);
		int initContext(const char* logFileName);
		const char* getEncoderType(void);
		const char* getEncoderDescription(void);
		const char* getFourCC(void);
		const char* getEncoderGuid(void);
		int isConfigurable(void);
		int configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
		void loadSettings(vidEncOptions *encodeOptions, Mpeg4aspEncoderOptions *options);
		void saveSettings(vidEncOptions *encodeOptions, Mpeg4aspEncoderOptions *options);
		int getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
		int setOptions(vidEncOptions *encodeOptions, const char *pluginOptions);
		int beginPass(vidEncPassParameters *passParameters);
		int encodeFrame(vidEncEncodeParameters *encodeParams);
		int finishPass(void);
};
#endif	// __cplusplus
#endif	// Mpeg4asp
