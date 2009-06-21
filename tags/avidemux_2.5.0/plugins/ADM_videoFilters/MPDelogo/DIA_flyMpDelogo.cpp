/***************************************************************************
                             DIA_flyMpDelogo.cpp
                             -------------------

    Common part of the MPlayer Delogo dialog
    
    copyright            : (C) 2008 by mean
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "DIA_flyDialog.h"
#include "DIA_flyMpDelogo.h"

flyMpDelogo::flyMpDelogo(uint32_t width, uint32_t height, AVDMGenericVideoStream *in, void *canvas, void *slider) : FLY_DIALOG_TYPE(width, height, in, canvas, slider, 0, RESIZE_LAST)
{
}

uint8_t flyMpDelogo::process(void)
{
	memcpy(_rgbBufferOut,_rgbBuffer, _w * _h * 4);

	uint8_t *in, *in2;
	uint8_t *buffer = _rgbBufferOut;

	in = buffer + x * 4 + y * _w * 4;
	in2 = buffer + (x + width) * 4 + y * _w * 4;

	for (int yy = 0; yy < height; yy++)
	{
		in[0] = in[2] = 0;
		in[1] = 0xff;

		in2[0] = in2[2] = 0;
		in2[1] = 0xff;

		in += _w * 4;
		in2 += _w * 4;
	}

	in = buffer + (y * _w + x) * 4;
	in2 = buffer + ((y + height) * _w + x) * 4;

	for (int yy = 0; yy < width; yy++)
	{
		in[0] = in[2] = 0;
		in[1] = 0xff;

		in2[0] = in2[2] = 0;
		in2[1] = 0xff;

		in += 4;
		in2 += 4;
	}

	copyRgbFinalToDisplay();
}
