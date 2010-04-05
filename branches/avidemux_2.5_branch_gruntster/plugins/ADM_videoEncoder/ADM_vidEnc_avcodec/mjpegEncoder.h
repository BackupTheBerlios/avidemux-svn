/***************************************************************************
                               mjpegEncoder.h

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

#ifndef MjpegEncoder_h
#define MjpegEncoder_h

#ifdef __cplusplus
extern "C"
{
	#include "ADM_vidEnc_plugin.h"
}

#include "encoder.h"
#include "mjpegEncoderOptions.h"
#include "DIA_factory.h"

class MjpegEncoder : public AvcodecEncoder
{
	private:
		unsigned int _quantiser;

		char configName[PATH_MAX];
		ConfigMenuType configType;

		MjpegEncoderOptions _options;
		vidEncOptions _encodeOptions;

		void updateEncodeProperties(vidEncOptions *encodeOptions);

	public:
		MjpegEncoder(void);
		int initContext(const char* logFileName);
		const char* getEncoderType(void);
		const char* getEncoderDescription(void);
		const char* getFourCC(void);
		const char* getEncoderGuid(void);
		int isConfigurable(void);
		int configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
		void loadSettings(vidEncOptions *encodeOptions, MjpegEncoderOptions *options);
		void saveSettings(vidEncOptions *encodeOptions, MjpegEncoderOptions *options);
		int getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
		int setOptions(vidEncOptions *encodeOptions, const char *pluginOptions);
		int beginPass(vidEncPassParameters *passParameters);
};
#endif	// __cplusplus
#endif	// MjpegEncoder_h
