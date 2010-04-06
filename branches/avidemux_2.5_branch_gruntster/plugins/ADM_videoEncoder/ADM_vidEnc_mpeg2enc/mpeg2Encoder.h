 /***************************************************************************
                                mpeg2Encoder.h

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

#ifndef mpeg2Encoder_h
#define mpeg2Encoder_h

extern "C"
{
	#include "ADM_vidEnc_plugin.h"
}

#include <limits.h>

#include "encoder.h"
#include "mpeg2Options.h"
#include "DIA_factory.h"

class Mpeg2Encoder : public Mpeg2encEncoder
{
	private:
		COMPRES_PARAMS _bitrateParam;
		unsigned int _maxBitrate, _splitFile, _streamType, _widescreen, _interlaced, _userMatrix;

		char configName[PATH_MAX];
		ConfigMenuType configType;

		Mpeg2Options _options;
		vidEncVideoProperties _properties;

		int _bufferSize;
		uint8_t *_buffer;

		void updateEncodeProperties(vidEncOptions *encodeOptions);
		void initParameters(int *encodeModeParameter, int *maxBitrate, int *vbv);

	public:
		Mpeg2Encoder(void);
		const char* getEncoderType(void);
		const char* getEncoderDescription(void);
		const char* getFourCC(void);
		const char* getEncoderGuid(void);
		int isConfigurable(void);
		int configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
		void loadSettings(vidEncOptions *encodeOptions, Mpeg2Options *options);
		void saveSettings(vidEncOptions *encodeOptions, Mpeg2Options *options);
		int getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
		int setOptions(vidEncOptions *encodeOptions, const char *pluginOptions);
};
#endif	// mpeg2Encoder_h
