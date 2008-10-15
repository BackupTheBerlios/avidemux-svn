/***************************************************************************
                             DIA_flyEqualiser.h
                             ------------------

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
#ifndef FLYEQUALISER_H
#define FLYEQUALISER_H

//#include "default.h"
//#include "ADM_image.h"
//#include "ADM_video/ADM_genvideo.hxx"
#include "DIA_flyDialog.h"

class flyEqualiser : public FLY_DIALOG_TYPE
{
private:
	ADMImage *_scratchImage;
	int points[8];

public:
	uint32_t *histogramIn, *histogramOut;
	int scaler[256];
	int crossSettings[9];

	flyEqualiser(uint32_t width, uint32_t height, AVDMGenericVideoStream *in, void *canvas, void *slider);
	~flyEqualiser();

	uint8_t process(void);
	uint8_t upload(void);
	uint8_t download(void);
	void buildScaler(int *points, int *scaler);
	void computeHistogram(void);
};
#endif
