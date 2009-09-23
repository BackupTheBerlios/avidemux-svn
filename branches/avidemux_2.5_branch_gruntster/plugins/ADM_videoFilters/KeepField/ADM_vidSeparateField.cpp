/***************************************************************************
                          Separate Fields.cpp  -  description
                             -------------------
Convert a x*y * f fps video into -> x*(y/2)*fps/2 video

Same idea as for avisynth separatefield


    begin                : Thu Mar 21 2002
    copyright            : (C) 2002 by mean
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
#include "ADM_videoFilterDynamic.h"
#include "ADM_vidFieldUtil.h"
#include "ADM_vidSeparateField.h"


static FILTER_PARAM swapParam={0,{""}};
VF_DEFINE_FILTER(AVDMVideoSeparateField,swapParam,
                separatefields,
                QT_TR_NOOP("Separate Fields"),
                1,
                VF_INTERLACING,
                QT_TR_NOOP("Each field becomes full picture, half sized."));

char *AVDMVideoSeparateField::printConf( void )
{
 	ADM_FILTER_DECLARE_CONF(" Separate Fields");
 
}

//_______________________________________________________________
AVDMVideoSeparateField::AVDMVideoSeparateField(
									AVDMGenericVideoStream *in,CONFcouple *setup)
{
UNUSED_ARG(setup);
  	_in=in;
   	memcpy(&_info,_in->getInfo(),sizeof(_info));
	

	_info.height>>=1;
	_info.fps1000*=2;
	_info.nb_frames*=2;
	vidCache=new VideoCache(2,_in);

}

// ___ destructor_____________
AVDMVideoSeparateField::~AVDMVideoSeparateField()
{
 	
	delete vidCache;
	vidCache=NULL;
}

//
//	Basically ask a uncompressed frame from editor and ask
//		GUI to decompress it .
//

uint8_t AVDMVideoSeparateField::getFrameNumberNoAlloc(uint32_t frame,
				uint32_t *len,
   				ADMImage *data,
				uint32_t *flags)
{
uint32_t ref;
ADMImage *ptr;
		if(frame>=_info.nb_frames) return 0;
		ref=frame>>1;
		
		ptr=vidCache->getImage(ref);
		if(!ptr) return 0;

		ADM_assert(ptr->data);
		ADM_assert(data->data);
		if(frame&1) // odd image
			 vidFieldKeepOdd(_info.width,_info.height,ptr->data,data->data);
		else
			 vidFieldKeepEven(_info.width,_info.height,ptr->data,data->data);
		data->copyInfo(ptr);	
		vidCache->unlockAll();
      return 1;
}

