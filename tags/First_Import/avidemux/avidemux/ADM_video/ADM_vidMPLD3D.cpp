/***************************************************************************
                          ADM_vidMPLD3D.cpp  -  description
                             -------------------
Mplayer HQDenoise3d port to avidemux2
Original Authors
Copyright (C) 2003
Daniel Moreno <comac@comac.darktech.org>
	& A'rpi
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <assert.h>
#include <math.h>

#include "config.h"
#include "fourcc.h"
#include "avio.hxx"
#include "config.h"
#include "avi_vars.h"

#include "ADM_toolkit/toolkit.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_video/ADM_vidMPLD3D.h"

#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_FILTER
#include "ADM_toolkit/ADM_debug.h"


BUILD_CREATE(MPD3D_create,ADMVideoMPD3D);

#define PARAM1_DEFAULT 4.0
#define PARAM2_DEFAULT 3.0
#define PARAM3_DEFAULT 6.0
uint8_t DIA_d3d(double *luma,double *chroma,double *temporal);
 char 	*ADMVideoMPD3D::printConf(void)
 {
	  	static char buf[50];

 				sprintf((char *)buf," MPlayer HQ Denoise 3D (%2.1f - %2.1f - %2.1f)'",
						_param->param1,_param->param2,_param->param3);
        return buf;
}

uint8_t ADMVideoMPD3D::configure(AVDMGenericVideoStream *instream)
{

			_in=instream;
			DIA_d3d(&_param->param1,
					&_param->param2,
					&_param->param3);
			setup();

			return 1;
}
ADMVideoMPD3D::~ADMVideoMPD3D()
{

 	DELETE(_param);
	delete [] _uncompressed;
	delete [] Line;
	delete [] _storage;

	_storage=NULL;
	Line=NULL;
	_uncompressed=NULL;
}

static inline unsigned int LowPassMul(unsigned int PrevMul, unsigned int CurrMul, int* Coef){
//    int dMul= (PrevMul&0xFFFFFF)-(CurrMul&0xFFFFFF);
    int dMul= PrevMul-CurrMul;
    int d=((dMul+0x10007FF)/(65536/16));
    return CurrMul + Coef[d];
}

void ADMVideoMPD3D::deNoise(unsigned char *Frame,        // mpi->planes[x]
                    unsigned char *FrameDest,    // dmpi->planes[x]
                   uint32_t 		 *LineAnt,      // vf->priv->Line (width bytes)
		    unsigned short 	*FrameAntPtr,
                    int W, int H, int sStride, int dStride,
                    int *Horizontal, int *Vertical, int *Temporal)
{
    int X, Y;
    int sLineOffs = 0, dLineOffs = 0;
    unsigned int PixelAnt;
    int PixelDst;
    unsigned short* FrameAnt=(FrameAntPtr);
    

    /* First pixel has no left nor top neightbour. Only previous frame */
    LineAnt[0] = PixelAnt = Frame[0]<<16;
    PixelDst = LowPassMul(FrameAnt[0]<<8, PixelAnt, Temporal);
    FrameAnt[0] = ((PixelDst+0x1000007F)/256);
    FrameDest[0]= ((PixelDst+0x10007FFF)/65536);

    /* Fist line has no top neightbour. Only left one for each pixel and
     * last frame */
    for (X = 1; X < W; X++){
        LineAnt[X] = PixelAnt = LowPassMul(PixelAnt, Frame[X]<<16, Horizontal);
        PixelDst = LowPassMul(FrameAnt[X]<<8, PixelAnt, Temporal);
	FrameAnt[X] = ((PixelDst+0x1000007F)/256);
	FrameDest[X]= ((PixelDst+0x10007FFF)/65536);
    }

    for (Y = 1; Y < H; Y++){
	unsigned int PixelAnt;
	unsigned short* LinePrev=&FrameAnt[Y*W];
	sLineOffs += sStride, dLineOffs += dStride;
        /* First pixel on each line doesn't have previous pixel */
        PixelAnt = Frame[sLineOffs]<<16;
        LineAnt[0] = LowPassMul(LineAnt[0], PixelAnt, Vertical);
	PixelDst = LowPassMul(LinePrev[0]<<8, LineAnt[0], Temporal);
	LinePrev[0] = ((PixelDst+0x1000007F)/256);
	FrameDest[dLineOffs]= ((PixelDst+0x10007FFF)/65536);

        for (X = 1; X < W; X++){
	    int PixelDst;
            /* The rest are normal */
            PixelAnt = LowPassMul(PixelAnt, Frame[sLineOffs+X]<<16, Horizontal);
            LineAnt[X] = LowPassMul(LineAnt[X], PixelAnt, Vertical);
	    PixelDst = LowPassMul(LinePrev[X]<<8, LineAnt[X], Temporal);
	    LinePrev[X] = ((PixelDst+0x1000007F)/256);
	    FrameDest[dLineOffs+X]= ((PixelDst+0x10007FFF)/65536);
        }
    }
}


uint8_t  ADMVideoMPD3D::setup(void)
{
 double LumSpac, LumTmp, ChromSpac, ChromTmp;

        LumSpac = _param->param1;
        LumTmp = _param->param3;

        ChromSpac = _param->param2;
        ChromTmp = LumTmp * ChromSpac / LumSpac;

        PrecalcCoefs((int *)Coefs[0], LumSpac);
        PrecalcCoefs((int *)Coefs[1], LumTmp);
        PrecalcCoefs((int *)Coefs[2], ChromSpac);
        PrecalcCoefs((int *)Coefs[3], ChromTmp);

	aprintf("\n Param : %lf %lf %lf \n",
		_param->param1,
		_param->param2,
		_param->param3);

	return 1;
}
//--------------------------------------------------------
ADMVideoMPD3D::ADMVideoMPD3D(
									AVDMGenericVideoStream *in,CONFcouple *couples)

{
  //uint32_t frame;
  _storage=NULL;
  _uncompressed=NULL;
  Line=NULL;

  _in=in;

  Line=new uint32_t[in->getInfo()->width];
  memcpy(&_info,in->getInfo(),sizeof(_info));
  _info.encoding=1;
  if(couples)
  {
			_param=NEW(MPD3D_PARAM);
			GET(param1);
			GET(param2);
			GET(param3);
	}
	else
	{
			_param=NEW( MPD3D_PARAM);
			_param->param1=PARAM1_DEFAULT;
			_param->param2=PARAM2_DEFAULT;
			_param->param3=PARAM3_DEFAULT;
	}
	_uncompressed=new unsigned short[(_info.width*_info.height*3)>>1];
	_storage=new uint8_t [(_info.width*_info.height*3)>>1];
	setup();

	_last=0xFFFFFFF;
}


uint8_t	ADMVideoMPD3D::getCoupledConf( CONFcouple **couples)
{

			assert(_param);
			*couples=new CONFcouple(3);
 			//(*couples)->setCouple((char *)"param",*_param);
#define CSET(x)  (*couples)->setCouple((char *)#x,(_param->x))
			CSET(param1);
			CSET(param2);
			CSET(param3);
			return 1;

}
//                     1
//		Get in range in 121 + coeff matrix
//                     1
//
// If the value is too far away we ignore it
// else we blend

uint8_t ADMVideoMPD3D::getFrameNumberNoAlloc(uint32_t frame,
																	uint32_t *len,
   																	uint8_t *data,
   																	uint32_t *flags)
{
UNUSED_ARG(flags);

	int cw= _info.width>>1;
	int ch= _info.height>>1;
        int W = _info.width;
	int H  = _info.height;
	uint32_t dlen,dflags;

  		if(frame> _info.nb_frames-1) return 0;
		*len=(W*H*3)>>1;
		// First and last frame we don"t modify.
		if(!frame || (frame!=_last+1))
			{
					 if(!_in->getFrameNumberNoAlloc(frame, &dlen,data,&dflags))
					 {
					 	return 0;
					}
				 	unsigned short* dst=_uncompressed;
	    				unsigned char* src=data;
					for (int Y = 0; Y < (W*H*3)>>1; Y++)
						{
	    						 	*(dst++)=*(src++)<<8;
						}
					_last=frame;
					return 1;

			}
			assert(frame<_info.nb_frames);
			// read uncompressed frame
			// else we fill previous/current/next
			if(!_in->getFrameNumberNoAlloc(frame, &dlen,_storage,&dflags))
			{
				return 0;
			}

		uint8_t *c,*n;
		unsigned short *ant;

		ant=_uncompressed;
		n=data;
		c=_storage;
//
   		deNoise(c, n,
			Line,ant, W, H,
                	W,W,
                	(int *)Coefs[0],
                	(int *)Coefs[0],
                	(int *)Coefs[1]);


		ant=ant+W*H;
		n=n+W*H;
		c=c+W*H;


		deNoise(c, n,
			Line, ant, cw, ch,
                	cw,cw,
                	(int *)Coefs[2],
                	(int *)Coefs[2],
                	(int *)Coefs[3]);


		ant=ant+((W*H)>>2);
		n=n+((W*H)>>2);
		c=c+((W*H)>>2);

		deNoise(c, n,
			Line, ant, cw, ch,
                	cw,cw,
                	(int *)Coefs[2],
                	(int *)Coefs[2],
                	(int *)Coefs[3]);

	// n is out....
	_last=frame;
	return 1;


}

#define ABS(A) ( (A) > 0 ? (A) : -(A) )

void ADMVideoMPD3D::PrecalcCoefs(int *Ct, double Dist25)
{
    int i;
    double Gamma, Simil, C;

    Gamma = log(0.25) / log(1.0 - Dist25/255.0 - 0.00001);

    for (i = -256*16; i < 256*16; i++)
    {
        Simil = 1.0 - ABS(i) / (16*255.0);
	C=4096.*(double)i;
        C *= pow(Simil, Gamma) ;
       Ct[16*256+i] = (int)((C<0) ? (C-0.5) : (C+0.5));
    }

}


