/***************************************************************************
                               flv1Encoder.h

    begin                : Tue Dec 29 2009
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

#ifndef FLV1Encoder_h
#define FLV1Encoder_h

#ifdef __cplusplus
extern "C"
{
	#include "ADM_vidEnc_plugin.h"
}

#include "encoder.h"
#include "flv1EncoderOptions.h"
#include "DIA_factory.h"

class FLV1Encoder : public AvcodecEncoder
{
	private:
		unsigned int _bitrate, _gopSize;

		char configName[PATH_MAX];
		ConfigMenuType configType;

		FLV1EncoderOptions _options;
		vidEncOptions _encodeOptions;

		void updateEncodeProperties(vidEncOptions *encodeOptions);

	public:
		FLV1Encoder(void);
		int initContext(const char* logFileName);
		const char* getEncoderType(void);
		const char* getEncoderDescription(void);
		const char* getFourCC(void);
		const char* getEncoderGuid(void);
		int isConfigurable(void);
		int configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
		void loadSettings(vidEncOptions *encodeOptions, FLV1EncoderOptions *options);
		void saveSettings(vidEncOptions *encodeOptions, FLV1EncoderOptions *options);
		int getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
		int setOptions(vidEncOptions *encodeOptions, const char *pluginOptions);
};
#endif	// __cplusplus
#endif	// FLV1Encoder_h
