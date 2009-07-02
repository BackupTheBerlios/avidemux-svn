/***************************************************************************
                                dvEncoder.cpp

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

#include <sstream>
#include <iomanip>

#include "ADM_inttype.h"
#include "dvEncoder.h"
#include "DIA_coreToolkit.h"

DVEncoder::DVEncoder(void)
{
	init(CODEC_ID_DVVIDEO, PIX_FMT_YUV420P);
}

const char* DVEncoder::getEncoderType(void)
{
	return "DV";
}

const char* DVEncoder::getEncoderDescription(void)
{
	return "DV video encoder plugin for Avidemux (c) Mean/Gruntster";
}

const char* DVEncoder::getFourCC(void)
{
	return "dvsd";
}

const char* DVEncoder::getEncoderGuid(void)
{
	return "D5118154-0B9A-4e9a-BEC6-95DA343E7215";
}

int DVEncoder::open(vidEncVideoProperties *properties)
{
	int ret = AvcodecEncoder::open(properties);
    int profileCount = sizeof(dvProfiles) / sizeof(dvProfiles[0]);
	bool validProfile = false;

	if (ret == ADM_VIDENC_ERR_SUCCESS)
	{
		for (int i = 0; i < profileCount; i++)
		{
			if (properties->height == dvProfiles[i].height && properties->width == dvProfiles[i].width && 
				((int)(properties->fpsNum * 1000.0 / properties->fpsDen) == (int)(dvProfiles[i].time_base.num * 1000.0 / dvProfiles[i].time_base.den)))
			{
				_context->pix_fmt = dvProfiles[i].pix_fmt;
				validProfile = true;
				break;
			}
		}

		if (!validProfile)
		{
			std::string msg;
			std::stringstream out;
			
			out << "The DV encoder only accepts the following profiles:";

			for (int i = 0; i < profileCount; i++)
			{
				out << "\n" << dvProfiles[i].width << " x " << dvProfiles[i].height << " @ " << std::fixed
					<< std::setprecision(2) << (float)dvProfiles[i].time_base.num / dvProfiles[i].time_base.den << "fps";
			}

			ret = ADM_VIDENC_ERR_FAILED;
			msg = out.str();

			GUI_Error_HIG("Incompatible settings", msg.c_str());
		}
	}

	return ret;
}

int DVEncoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
{
	if (encodeOptions)
	{
		encodeOptions->encodeMode = ADM_VIDENC_MODE_CQP;
		encodeOptions->encodeModeParameter = 1000;
	}

	return 0;
}

int DVEncoder::setOptions(vidEncOptions *encodeOptions, char *pluginOptions)
{
	return 0;
}
