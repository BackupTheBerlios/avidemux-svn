/***************************************************************************
                          ADM_coreMuxer.cpp  -  description
                             -------------------
    copyright            : (C) 2008 by mean
    email                : fixounet@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ADM_default.h"
#include "ADM_muxerProto.h"

extern "C"
{
    #include "libavformat/avformat.h"
	#include "libavformat/url.h"
};

void ADM_lavFormatInit(void)
{
	av_register_all();

	// Make sure avformat is correctly configured
	const char* formats[] = {"mpegts", "dvd", "vcd", "svcd", "mp4", "psp", "flv", "matroska"};
	AVOutputFormat *avfmt;

	for (int i = 0; i < 8; i++)
	{
		avfmt = av_guess_format(formats[i], NULL, NULL);

		if (avfmt == NULL)
		{
			printf("Error: %s muxer isn't registered\n", formats[i]);
			ADM_assert(0);
		}
	}

	URLProtocol *up = ffurl_protocol_next(NULL);

	if (strcmp(up->name, "file") != 0)
	{
		printf("Error: file protocol isn't registered\n");
		ADM_assert(0);
	}
}
// EOF

