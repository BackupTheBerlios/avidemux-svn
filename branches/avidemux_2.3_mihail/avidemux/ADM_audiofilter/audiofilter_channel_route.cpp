//
// C++ Implementation: audiofilter_channel_route
//
// Description: 
//
//
// Author: Mihail Zenkov <kreator@tut.by>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <stdint.h>
#include <ADM_audio/ADM_audiodef.h>
#include "audiofilter_channel_route.h"

ChannelRoute ch_route;

bool ChannelRoute::compareChType(WAVHeader *wh1, WAVHeader *wh2)
{
	int i = 0;
	if (wh1->channels == wh2->channels)
		for (int j = 0; j < wh1->channels; j++)
			if (input_type[i] == output_type[j]) {
				i++;
				if (i == wh1->channels)
					return 1;
				j = -1;
			}

	return 0;
}
