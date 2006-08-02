//
// C++ Interface: audiofilter_channel_route
//
// Description: 
//
//
// Author: Mihail Zenkov <kreator@tut.by>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDM_CHANNEL_ROUTE_H
#define AUDM_CHANNEL_ROUTE_H

#define MAX_CHANNELS 9

typedef enum CHANNEL_TYPE
{
	CH_INVALID=0,
	CH_MONO,
	CH_FRONT_LEFT,
	CH_FRONT_RIGHT,
	CH_FRONT_CENTER,
	CH_REAR_LEFT,
	CH_REAR_RIGHT,
	CH_REAR_CENTER,
	CH_SIDE_LEFT,
	CH_SIDE_RIGHT,
	CH_LFE
};

class ChannelRoute
{
public:
	ChannelRoute() {}
	~ChannelRoute() {}

	bool compareChType(WAVHeader *wh1, WAVHeader *wh2);
	void copyInToOut() { memcpy(output_type, input_type, sizeof(input_type)); }

	CHANNEL_TYPE input_type[MAX_CHANNELS];
	CHANNEL_TYPE output_type[MAX_CHANNELS];
};

extern ChannelRoute ch_route;

#endif
