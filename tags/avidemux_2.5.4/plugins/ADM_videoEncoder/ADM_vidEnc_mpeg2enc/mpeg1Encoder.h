 /***************************************************************************
                                mpeg1Encoder.h

    begin                : Mon Apr 5 2010
    copyright            : (C) 2010 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef mpeg1Encoder_h
#define mpeg1Encoder_h

extern "C"
{
	#include "ADM_vidEnc_plugin.h"
}

#include <limits.h>

#include "encoder.h"
#include "mpeg1Options.h"
#include "DIA_factory.h"

class Mpeg1Encoder : public Mpeg2encEncoder
{
	private:
		unsigned int _splitFile;
		
		char configName[PATH_MAX];
		ConfigMenuType configType;

		Mpeg1Options _options;
		vidEncVideoProperties _properties;

		int _bufferSize;
		uint8_t *_buffer;

		void initParameters(int *encodeModeParameter, int *maxBitrate, int *vbv);

	public:
		Mpeg1Encoder(void);
		const char* getEncoderType(void);
		const char* getEncoderDescription(void);
		const char* getFourCC(void);
		const char* getEncoderGuid(void);
		int isConfigurable(void);
		int configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
		void loadSettings(vidEncOptions *encodeOptions, Mpeg1Options *options);
		void saveSettings(vidEncOptions *encodeOptions, Mpeg1Options *options);
		int getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
		int setOptions(vidEncOptions *encodeOptions, const char *pluginOptions);
};
#endif	// mpeg1Encoder_h
