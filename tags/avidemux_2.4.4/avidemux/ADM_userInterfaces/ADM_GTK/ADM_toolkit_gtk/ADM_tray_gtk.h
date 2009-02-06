/***************************************************************************
                                ADM_qtray.cpp
                                -------------

    begin                : Tue Sep 2 2008
    copyright            : (C) 2008 by gruntster

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ADM_GTKTRAY_H
#define ADM_GTKTRAY_H

#include "ADM_tray.h"

class ADM_gtktray : public ADM_tray
{
public:
	ADM_gtktray(void *parent);
	~ADM_gtktray();
	uint8_t setPercent(int percent);
	uint8_t setStatus(int working);
};
#endif
