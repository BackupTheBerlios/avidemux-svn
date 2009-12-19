
/***************************************************************************
                        Simple equalizer or linear luma/chroma filter
    

   (C) 2004/2005 Mean fixounet@free.fr
   GPL version 2

 ***************************************************************************/
#include "ADM_default.h"
#include <math.h>

#include "ADM_videoFilterDynamic.h"
#include "ADM_vidEqualizer.h"


static FILTER_PARAM equalizer_template={256,
{
"x0","x1","x2","x3","x4","x5","x6","x7","x8","x9","x10","x11","x12","x13","x14","x15","x16","x17","x18",
"x19","x20","x21","x22","x23","x24"
/*,"x25","x26","x27","x28","x29","x30","x31","x32","x33","x34","x35","x36",
"x37","x38","x39","x40","x41","x42","x43","x44","x45","x46","x47","x48","x49","x50","x51","x52","x53","x54",
"x55","x56","x57","x58","x59","x60","x61","x62","x63","x64","x65","x66","x67","x68","x69","x70","x71","x72",
"x73","x74","x75","x76","x77","x78","x79","x80","x81","x82","x83","x84","x85","x86","x87","x88","x89","x90",
"x91","x92","x93","x94","x95","x96","x97","x98","x99","x100","x101","x102","x103","x104","x105","x106",
"x107","x108","x109","x110","x111","x112","x113","x114","x115","x116","x117","x118","x119","x120","x121",
"x122","x123","x124","x125","x126","x127","x128","x129","x130","x131","x132","x133","x134","x135","x136",
"x137","x138","x139","x140","x141","x142","x143","x144","x145","x146","x147","x148","x149","x150","x151",
"x152","x153","x154","x155","x156","x157","x158","x159","x160","x161","x162","x163","x164","x165","x166",
"x167","x168","x169","x170","x171","x172","x173","x174","x175","x176","x177","x178","x179","x180","x181",
"x182","x183","x184","x185","x186","x187","x188","x189","x190","x191","x192","x193","x194","x195","x196",
"x197","x198","x199","x200","x201","x202","x203","x204","x205","x206","x207","x208","x209","x210","x211",
"x212","x213","x214","x215","x216","x217","x218","x219","x220","x221","x222","x223","x224","x225","x226",
"x227","x228","x229","x230","x231","x232","x233","x234","x235","x236","x237","x238","x239","x240","x241",
"x242","x243","x244","x245","x246","x247","x248","x249","x250","x251","x252","x253","x254","x255"
*/
}
};
//REGISTERX(VF_COLORS, "equalizer",QT_TR_NOOP("Luma equalizer"),
//QT_TR_NOOP("Luma correction filter with histogram."),VF_EQUALIZER,1,equalizer_create,equalizer_script);
VF_DEFINE_FILTER_UI(vidEqualizer,equalizer_template,
                equalizer,
                QT_TR_NOOP("Luma equalizer"),
                1,
                VF_COLORS,
                QT_TR_NOOP("Luma correction filter with histogram."));


extern uint8_t DIA_getEqualizer(EqualizerParam *param, AVDMGenericVideoStream *incoming);

uint8_t vidEqualizer::configure(AVDMGenericVideoStream *in)
{
ADMImage *video1;
uint32_t l,f;
uint8_t r;

	_in=in;		
	r= DIA_getEqualizer(_param,in);
	return r;
	
}

char *vidEqualizer::printConf( void )
{
 	ADM_FILTER_DECLARE_CONF(" Equalizer");
        
}

vidEqualizer::vidEqualizer(AVDMGenericVideoStream *in,CONFcouple *couples) 
{
		_in=in;		
   		memcpy(&_info,_in->getInfo(),sizeof(_info));    
  		_info.encoding=1;
		_uncompressed=NULL;
		
  		_info.encoding=1;
		_uncompressed=new ADMImage(_info.width,_info.height);
		
		
		_param=NEW(EqualizerParam);
		if(couples)
		{
		        char dummy[10];
                        for(int i=0;i<256;i++)  
                        {
                                sprintf(dummy,"x%d",i);
                                couples->getCouple((char *)dummy,&(_param->_scaler[i]));
                                //printf("%d",_param->_scaler[i]);
                        }
		}
		else // Default
  		{
                                for(int i=0;i<256;i++)
                                        _param->_scaler[i]=i;
		}
}
//____________________________________________________________________
vidEqualizer::~vidEqualizer()
{
		
	delete _uncompressed;
	delete _param;
	_param=NULL;
	_uncompressed=NULL;
		
		
}

//______________________________________________________________
uint8_t vidEqualizer::getFrameNumberNoAlloc(uint32_t frame,
				uint32_t *len,
   				ADMImage *data,
				uint32_t *flags)
{

        if(frame>= _info.nb_frames) return 0;
	if(!_in->getFrameNumberNoAlloc(frame,len,_uncompressed,flags)) return 0;
	
	uint8_t *src,*dst;
	src=_uncompressed->data;
	dst=data->data;
	for(uint32_t y=_info.height;y>0;y--)
	for(uint32_t x=_info.width;x>0;x--)
		*(dst++)=_param->_scaler[*(src++)];

	uint32_t square=_info.width*_info.height;
	square>>=2;
	// copy u & v too
	memcpy(data->data+4*square,_uncompressed->data+4*square,2*square);
	return 1;
}


uint8_t	vidEqualizer::getCoupledConf( CONFcouple **couples)
{
char dummy[10];
			ADM_assert(_param);
			*couples=new CONFcouple(256);

        for(int i=0;i<256;i++)  
        {
                sprintf(dummy,"x%d",i);
                (*couples)->setCouple(dummy,(_param->_scaler[i]));
        }
	return 1;
}

// EOF
