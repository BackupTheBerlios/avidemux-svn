/***************************************************************************
                            DIA_flyEqualiser.cpp
                            --------------------

    begin                : Tue Oct 7 2008
    copyright            : (C) 2008 by mean/gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <math.h>
#include "DIA_flyEqualiser.h"
//#include "ADM_assert.h"

#define ZOOM_FACTOR 5

flyEqualiser::flyEqualiser(uint32_t width, uint32_t height, AVDMGenericVideoStream *in, void *canvas, void *slider) : FLY_DIALOG_TYPE(width, height, in, canvas, slider, 1, RESIZE_AUTO)
{
	int crossSettings[] = {0, 36, 73, 109, 146, 182, 219, 255, -1};

	memcpy(this->crossSettings, crossSettings, 9 * sizeof(int));
	_scratchImage = new ADMImage(width, height);
	histogramIn = new uint32_t[256 * 128];
	histogramOut = new uint32_t[256 * 128];
}

flyEqualiser::~flyEqualiser()
{
	delete _scratchImage;
	delete histogramIn;
	delete histogramOut;
}

uint8_t flyEqualiser::process(void)
{
	uint8_t *src = _yuvBuffer->data;
	uint8_t *dst = _scratchImage->data;
	uint8_t *disp = _yuvBufferOut->data;

	for (int y = 0; y < _h; y++)
	{
		for (int x = 0; x < _w; x++)
		{
			*dst = scaler[*src];
			dst++;
			src++;
		}		
	}

	uint32_t half = _w >> 1;

	src = _yuvBuffer->data;
	dst = _scratchImage->data;

	for (int y = 0; y < _h; y++)
	{
		if (y > _h)
		{
			memcpy(disp, dst, half);
			memcpy(disp + half, src + half, half);
		}
		else
		{
			memcpy(disp, src, half);
			memcpy(disp + half, dst + half, half);
		}

		src += _w;
		dst += _w;
		disp += _w;
	}

	memcpy(_yuvBufferOut->data + _w * _h, _yuvBuffer->data + _w * _h, (_w * _h) >> 1);
	computeHistogram();
	copyYuvFinalToRgb();
}

void flyEqualiser::buildScaler(int *p, int *s)
{
	const int cross[8] = {0, 36, 73, 109, 146, 182, 219, 255};
	double alpha, r;
	int deltax, deltay;

	for (int i = 0; i < 7; i++)
	{
		// line that goes from i to i+1
		deltax = cross[i + 1] - cross[i];
		deltay = p[i + 1] - p[i];

		if (deltay == 0) // flat line
		{
			for (int x = cross[i]; x <= cross[i+1]; x++)
			{
				if (p[i] >= 0)
					s[x] = p[i];
				else
					s[x] = 0;
			}
		}
		else
		{
			alpha = deltay;
			alpha /= deltax;

			for (int x = cross[i]; x <= cross[i + 1]; x++)
			{
				r = (x - cross[i]) * alpha;
				r += p[i];

				if(r < 0)
					r = 0;

				s[x] = (int)floor(r + 0.49);			
			}
		}
	}

	for (int i = 0; i < 256; i++)
	{
		if (s[i] < 0)
			s[i] = 0;
		if (s[i] > 255)
			s[i] = 255;
	}
}

void flyEqualiser::computeHistogram(void)
{
	uint32_t valueIn[256];
	uint32_t valueOut[256];
	uint8_t value;

	memset(valueIn, 0, 256 * sizeof(uint32_t));
	memset(valueOut, 0, 256 * sizeof(uint32_t));

	// In
	for (int index = 0; index < _w * _h; index++)
	{
		value = _yuvBuffer->data[index];
		valueIn[value]++;	
		valueOut[scaler[value]]++;
	}

	// normalize
	double d, a = _w * _h;

	for (int i = 0; i < 256; i++)
	{
		d = valueIn[i];
		d *= 256 * ZOOM_FACTOR;
		d /= a;
		valueIn[i] = (uint32_t)floor(d + 0.49);

		if (valueIn[i] > 127)
			valueIn[i] = 127;

		d = valueOut[i];
		d *= 256 * ZOOM_FACTOR;
		d /= a;

		valueOut[i] = (uint32_t)floor(d + 0.49);

		if (valueOut[i] > 127)
			valueOut[i] = 127;
	}

	// draw
	memset(histogramIn, 0, 256 * 128 * sizeof(uint32_t));
	memset(histogramOut, 0, 256 * 128 * sizeof(uint32_t));

	int y, tgt, yout;

	for (int i = 0; i < 256; i++)
	{
		y = valueIn[i];

		for (int u = 0; u <= y; u++)
		{
			tgt = i + (127 - u) * 256;
			histogramIn[tgt] = 0xFFFFFFFF;
		}

		y = valueOut[i];

		for (int u = 0; u <= y; u++)
		{
			tgt = i + (127 - u) * 256;
			histogramOut[tgt] = 0xFFFFFFFF;
		}
	}
}
