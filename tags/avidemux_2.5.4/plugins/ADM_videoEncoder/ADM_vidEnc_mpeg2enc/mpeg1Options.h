 /***************************************************************************
                               mpeg1Options.h

    begin                : Tue Apr 6 2010
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

#ifndef mpeg1Options_h
#define mpeg1Options_h

#include <vector>
#include "PluginOptions.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

#define MPEG1_DEFAULT_ENCODE_MODE ADM_VIDENC_MODE_CBR
#define MPEG1_DEFAULT_ENCODE_MODE_PARAMETER 1000

class Mpeg1Options : public PluginOptions
{
protected:
	unsigned int _fileSplit;

	void addOptionsToXml(xmlNodePtr xmlNodeRoot);
	void parseOptions(xmlNode *node);

public:
	Mpeg1Options(void);
	void reset(void);

	unsigned int getFileSplit(void);
	void setFileSplit(unsigned int mb);
};

#endif	// mpeg1Options_h
