//
// C++ Interface: ADM_cache
//
// Description: 
//
//
// Author: mean <fixounet@free.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
class VideoCache
{
	private:
		uint32_t 	nbEntry;
		uint32_t 	*frameNum;
		ADMImage 	**frameBuffer;
		uint8_t		*frameLock;
		AVDMGenericVideoStream *incoming;
		ADV_Info 	info;
		uint32_t	lastUsed;
		
		int32_t 	searchFrame( uint32_t frame);
		int32_t 	searchPtr( ADMImage *ptr);
	public:
		VideoCache(uint32_t nb,AVDMGenericVideoStream *in);
		~VideoCache(void);
		
		ADMImage *getImage(uint32_t frame);
		uint8_t unlockAll(void);
		uint8_t unlock(ADMImage  *frame);
		uint8_t purge(void);
};
