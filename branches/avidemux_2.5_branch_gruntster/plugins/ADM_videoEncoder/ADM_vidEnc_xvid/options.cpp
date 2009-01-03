 /***************************************************************************
                               XvidOptions.cpp

	These settings are intended for scripting and can contain a Preset 
	Configuration block.  If this block exists then "internal" settings are
	ignored and an external configuration file should be read instead, 
	e.g. PlayStation Portable profile.  However, if the external file is 
	missing, internal settings have to be used and will reflect a snapshot
	of the external configuration file at the time the script was generated.

    begin                : Fri Jun 13 2008
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

#include <string.h>
#include "options.h"

XvidOptions::XvidOptions(void)
{
	memset(&xvid_enc_create, 0, sizeof(xvid_enc_create_t));
	memset(&xvid_enc_frame, 0, sizeof(xvid_enc_frame_t));

	xvid_enc_frame.vop_flags = XVID_VOP_HALFPEL;

	setMaxKeyInterval(240);
	setMinQuantiser(2, 2, 2);
	setMaxQuantiser(31, 31, 31);
	setPar(1, 1);
}

XvidOptions::~XvidOptions(void)
{

}

MotionEstimationMode XvidOptions::getMotionEstimation(void)
{
	if (xvid_enc_frame.motion & ME_HIGH == ME_HIGH)
		return ME_HIGH;
	else if (xvid_enc_frame.motion & ME_MEDIUM == ME_MEDIUM)
		return ME_MEDIUM;
	else if (xvid_enc_frame.motion & ME_LOW == ME_LOW)
		return ME_LOW;
	else
		return ME_NONE;
}

void XvidOptions::setMotionEstimation(MotionEstimationMode motionEstimation)
{
	if (motionEstimation == ME_NONE || motionEstimation == ME_LOW || motionEstimation == ME_MEDIUM || motionEstimation == ME_HIGH)
	{
		xvid_enc_frame.motion &= ~ME_NONE;
		xvid_enc_frame.motion &= ~ME_LOW;
		xvid_enc_frame.motion &= ~ME_MEDIUM;
		xvid_enc_frame.motion &= ~ME_HIGH;
		xvid_enc_frame.motion |= motionEstimation;
	}
}

unsigned int XvidOptions::getMaxKeyInterval(void)
{
	return xvid_enc_create.max_key_interval;
}

void XvidOptions::setMaxKeyInterval(unsigned int maxKeyInterval)
{
	if (maxKeyInterval > 0 && maxKeyInterval <= 900)
		xvid_enc_create.max_key_interval = maxKeyInterval;
}

unsigned int XvidOptions::getMaxBFrames(void)
{
	return xvid_enc_create.max_bframes;
}

void XvidOptions::setMaxBFrames(unsigned int maxBFrames)
{
	if (maxBFrames <= 3)
		xvid_enc_create.max_bframes = maxBFrames;
}

bool XvidOptions::getMpegQuantisation(void)
{
	return xvid_enc_frame.vol_flags & XVID_VOL_MPEGQUANT;
}

void XvidOptions::setMpegQuantisation(bool mpegQuantisation)
{
	if (mpegQuantisation)
		xvid_enc_frame.vol_flags |= XVID_VOL_MPEGQUANT;
	else
		xvid_enc_frame.vol_flags &= ~XVID_VOL_MPEGQUANT;
}

bool XvidOptions::getInterlaced(void)
{
	return xvid_enc_frame.vol_flags & XVID_VOL_INTERLACING;
}

void XvidOptions::setInterlaced(bool interlaced)
{
	if (interlaced)
		xvid_enc_frame.vol_flags |= XVID_VOL_INTERLACING;
	else
		xvid_enc_frame.vol_flags &= ~XVID_VOL_INTERLACING;
}

bool XvidOptions::getInterMotionVector(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_INTER4V;
}

void XvidOptions::setInterMotionVector(bool interMotionVector)
{
	if (interMotionVector)
	{
		xvid_enc_frame.vop_flags |= XVID_VOP_INTER4V;

		if (getQpel())
			setQpel(true);
	}
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_INTER4V;
}

bool XvidOptions::getTrellis(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_TRELLISQUANT;
}

void XvidOptions::setTrellis(bool trellis)
{
	if (trellis)
		xvid_enc_frame.vop_flags |= XVID_VOP_TRELLISQUANT;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_TRELLISQUANT;
}

bool XvidOptions::getCartoon(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_CARTOON;
}

void XvidOptions::setCartoon(bool cartoon)
{
	if (cartoon)
		xvid_enc_frame.vop_flags |= XVID_VOP_CARTOON;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_CARTOON;
}

bool XvidOptions::getGreyscale(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_GREYSCALE;
}

void XvidOptions::setGreyscale(bool greyscale)
{
	if (greyscale)
		xvid_enc_frame.vop_flags |= XVID_VOP_GREYSCALE;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_GREYSCALE;
}

bool XvidOptions::getQpel(void)
{
	return xvid_enc_frame.vol_flags & XVID_VOL_QUARTERPEL;
}

void XvidOptions::setQpel(bool qpel)
{
	if (qpel)
	{
		xvid_enc_frame.vol_flags |= XVID_VOL_QUARTERPEL;
		xvid_enc_frame.motion |= XVID_ME_QUARTERPELREFINE16;

		if (getInterMotionVector())
			xvid_enc_frame.motion |= XVID_ME_QUARTERPELREFINE8;
	}
	else
	{
		xvid_enc_frame.vol_flags &= ~XVID_VOL_QUARTERPEL;
		xvid_enc_frame.motion &= ~XVID_ME_QUARTERPELREFINE16;
		xvid_enc_frame.motion &= ~XVID_ME_QUARTERPELREFINE8;
	}
}

bool XvidOptions::getGmc(void)
{
	return xvid_enc_frame.vol_flags & XVID_VOL_GMC;
}

void XvidOptions::setGmc(bool gmc)
{
	if (gmc)
	{
		xvid_enc_frame.vol_flags |= XVID_VOL_GMC;
		xvid_enc_frame.motion |= XVID_ME_GME_REFINE;
	}
	else
	{
		xvid_enc_frame.vol_flags &= ~XVID_VOL_GMC;
		xvid_enc_frame.motion &= ~XVID_ME_GME_REFINE;
	}
}

bool XvidOptions::getBvhq(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_RD_BVOP;
}

void XvidOptions::setBvhq(bool bvhq)
{
	if (bvhq)
		xvid_enc_frame.vop_flags |= XVID_VOP_RD_BVOP;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_RD_BVOP;
}

bool XvidOptions::getAcPrediction(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_HQACPRED;
}

void XvidOptions::setAcPrediction(bool acPrediction)
{
	if (acPrediction)
		xvid_enc_frame.vop_flags |= XVID_VOP_HQACPRED;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_HQACPRED;
}

bool XvidOptions::getChromaOptimisation(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_CHROMAOPT;
}

void XvidOptions::setChromaOptimisation(bool chromaOptimisation)
{
	if (chromaOptimisation)
		xvid_enc_frame.vop_flags |= XVID_VOP_CHROMAOPT;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_CHROMAOPT;
}

void XvidOptions::getMinQuantiser(unsigned int *i, unsigned int *p, unsigned int *b)
{
	*i = xvid_enc_create.min_quant[0];
	*p = xvid_enc_create.min_quant[1];
	*b = xvid_enc_create.min_quant[2];
}

void XvidOptions::setMinQuantiser(unsigned int i, unsigned int p, unsigned int b)
{
	if (i > 0 && i <= 51)
		xvid_enc_create.min_quant[0] = i;

	if (p > 0 && p <= 51)
		xvid_enc_create.min_quant[1] = p;

	if (b > 0 && b <= 51)
		xvid_enc_create.min_quant[2] = b;
}

void XvidOptions::getMaxQuantiser(unsigned int *i, unsigned int *p, unsigned int *b)
{
	*i = xvid_enc_create.max_quant[0];
	*p = xvid_enc_create.max_quant[1];
	*b = xvid_enc_create.max_quant[2];
}

void XvidOptions::setMaxQuantiser(unsigned int i, unsigned int p, unsigned int b)
{
	if (i > 0 && i <= 51)
		xvid_enc_create.max_quant[0] = i;

	if (p > 0 && p <= 51)
		xvid_enc_create.max_quant[1] = p;

	if (b > 0 && b <= 51)
		xvid_enc_create.max_quant[2] = b;
}

void XvidOptions::getPar(unsigned int *width, unsigned int *height)
{
	*width = xvid_enc_frame.par_width;
	*height = xvid_enc_frame.par_height;
}

void XvidOptions::setPar(unsigned int width, unsigned int height)
{
	if (width > 0)
		xvid_enc_frame.par_width = width;

	if (height > 0)
		xvid_enc_frame.par_height = height;

	  if (xvid_enc_frame.par_width != xvid_enc_frame.par_height)
		  xvid_enc_frame.par = XVID_PAR_EXT;
	  else
		  xvid_enc_frame.par = XVID_PAR_11_VGA;
}

RateDistortionMode XvidOptions::getRateDistortion(RateDistortionMode rateDistortion)
{
	if (xvid_enc_frame.motion & RD_SQUARE == RD_SQUARE)
		return RD_SQUARE;
	else if (xvid_enc_frame.motion & RD_HPEL_QPEL_8 == RD_HPEL_QPEL_8)
		return RD_HPEL_QPEL_8;
	else if (xvid_enc_frame.motion & RD_HPEL_QPEL_16 == RD_HPEL_QPEL_16)
		return RD_HPEL_QPEL_16;
	else if (xvid_enc_frame.motion & RD_DCT_ME == RD_DCT_ME)
		return RD_DCT_ME;
	else
		return RD_NONE;
}

void XvidOptions::setRateDistortion(RateDistortionMode rateDistortion)
{
	if (rateDistortion == RD_NONE || rateDistortion == RD_DCT_ME || rateDistortion == RD_HPEL_QPEL_16 || 
		rateDistortion == RD_HPEL_QPEL_8 || rateDistortion == RD_SQUARE)
	{
		xvid_enc_frame.motion &= ~RD_NONE;
		xvid_enc_frame.motion &= ~RD_DCT_ME;
		xvid_enc_frame.motion &= ~RD_HPEL_QPEL_16;
		xvid_enc_frame.motion &= ~RD_HPEL_QPEL_8;
		xvid_enc_frame.motion &= ~RD_SQUARE;
		xvid_enc_frame.motion |= rateDistortion;
	}
}

bool XvidOptions::getPacked(void)
{
	return xvid_enc_create.global & XVID_GLOBAL_PACKED;
}

void XvidOptions::setPacked(bool packed)
{
	if (packed)
		xvid_enc_create.global |= XVID_GLOBAL_PACKED;
	else
		xvid_enc_create.global &= ~XVID_GLOBAL_PACKED;
}

bool XvidOptions::getClosedGop(void)
{
	return xvid_enc_create.global & XVID_GLOBAL_CLOSED_GOP;
}

void XvidOptions::setClosedGop(bool closedGop)
{
	if (closedGop)
		xvid_enc_create.global |= XVID_GLOBAL_CLOSED_GOP;
	else
		xvid_enc_create.global &= ~XVID_GLOBAL_CLOSED_GOP;
}

