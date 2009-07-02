/***************************************************************************
                                 dvEncoder.h

    begin                : Thu Jul 2 2009
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

#ifndef dvEncoder_h
#define dvEncoder_h

#ifdef __cplusplus
extern "C"
{
	#include "ADM_vidEnc_plugin.h"
}

#include "encoder.h"

typedef struct DVprofile {
	AVRational time_base;
	int height;
	int width;
	enum PixelFormat pix_fmt;
} DVprofile;

static const DVprofile dvProfiles[] = {
	{ { 30000, 1001 }, 480, 720, PIX_FMT_YUV422P },
    { { 25, 1 }, 576, 720, PIX_FMT_YUV420P },
	{ { 50, 1 }, 720, 960, PIX_FMT_YUV422P },
    { { 60000, 1001 }, 720, 960, PIX_FMT_YUV422P },
    { { 30000, 1001 }, 1080, 1280, PIX_FMT_YUV422P },
    { { 25, 1 }, 1080, 1440, PIX_FMT_YUV422P }
};

class DVEncoder : public AvcodecEncoder
{
	public:
		DVEncoder(void);
		const char* getEncoderType(void);
		const char* getEncoderDescription(void);
		const char* getFourCC(void);
		const char* getEncoderGuid(void);
		int getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
		int setOptions(vidEncOptions *encodeOptions, char *pluginOptions);
		int open(vidEncVideoProperties *properties);
};
#endif	// __cplusplus
#endif	// dvEncoder_h
