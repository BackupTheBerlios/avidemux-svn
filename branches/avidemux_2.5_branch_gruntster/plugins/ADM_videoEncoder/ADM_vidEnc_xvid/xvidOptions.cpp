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

#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

#include "config.h"
#include "ADM_inttype.h"
#include "ADM_files.h"

#include "../common/PluginOptions.cpp"
#include "xvidOptions.h"

XvidOptions::XvidOptions(void) : PluginOptions("Xvid", "XvidParam.xsd", DEFAULT_ENCODE_MODE, DEFAULT_ENCODE_MODE_PARAMETER)
{
	reset();
}

void XvidOptions::reset(void)
{
	PluginOptions::reset();

	memset(&xvid_enc_create, 0, sizeof(xvid_enc_create_t));
	memset(&xvid_enc_frame, 0, sizeof(xvid_enc_frame_t));
	memset(&xvid_plugin_single, 0, sizeof(xvid_plugin_single_t));
	memset(&xvid_plugin_2pass2, 0, sizeof(xvid_plugin_2pass2_t));
	memset(&_intraMatrix, 8, sizeof(unsigned char) * 64);
	memset(&_interMatrix, 1, sizeof(unsigned char) * 64);

	xvid_enc_create.version = XVID_VERSION;
	xvid_enc_frame.version = XVID_VERSION;
	xvid_plugin_single.version = XVID_VERSION;
	xvid_plugin_2pass2.version = XVID_VERSION;

	xvid_enc_frame.vop_flags = XVID_VOP_HALFPEL | XVID_VOP_HQACPRED;

	// General
	setPar(1, 1);
	setParAsInput(false);

	// Quantiser
	setCqmPreset(CQM_H263);
	setMinQuantiser(1, 1, 1);
	setMaxQuantiser(31, 31, 31);
	setTrellis(true);

	// Motion
	setMotionEstimation(ME_HIGH);
	setChromaMotionEstimation(true);
	setRateDistortion(RD_DCT_ME);

	// Frame
	setMaxKeyInterval(300);
	setMaxBframes(2);
	setBframeQuantiserRatio(150);
	setBframeQuantiserOffset(100);

	// 1-Pass
	setReactionDelayFactor(16);
	setAveragingQuantiserPeriod(100);
	setSmoother(100);

	// 2-Pass
	setKeyFrameBoost(10);
	setMaxKeyFrameReduceBitrate(20);
	setKeyFrameBitrateThreshold(1);
	setOverflowControlStrength(5);
	setMaxOverflowImprovement(5);
	setMaxOverflowDegradation(5);
}

void XvidOptions::getParameters(xvid_enc_create_t **xvid_enc_create, xvid_enc_frame_t **xvid_enc_frame,
	xvid_plugin_single_t **xvid_plugin_single, xvid_plugin_2pass2_t **xvid_plugin_2pass2)
{
	*xvid_enc_create = new xvid_enc_create_t;
	*xvid_enc_frame = new xvid_enc_frame_t;
	*xvid_plugin_single = new xvid_plugin_single_t;
	*xvid_plugin_2pass2 = new xvid_plugin_2pass2_t;

	memcpy(*xvid_enc_create, &this->xvid_enc_create, sizeof(xvid_enc_create_t));
	memcpy(*xvid_enc_frame, &this->xvid_enc_frame, sizeof(xvid_enc_frame_t));
	memcpy(*xvid_plugin_single, &this->xvid_plugin_single, sizeof(xvid_plugin_single_t));
	memcpy(*xvid_plugin_2pass2, &this->xvid_plugin_2pass2, sizeof(xvid_plugin_2pass2_t));

	if (getCqmPreset() == CQM_CUSTOM)
	{
		(*xvid_enc_frame)->quant_intra_matrix = new unsigned char[64];
		(*xvid_enc_frame)->quant_inter_matrix = new unsigned char[64];

		getIntraMatrix((*xvid_enc_frame)->quant_intra_matrix);
		getInterMatrix((*xvid_enc_frame)->quant_inter_matrix);
	}
}

unsigned int XvidOptions::getThreads(void)
{
	return xvid_enc_create.num_threads;
}

void XvidOptions::setThreads(unsigned int threads)
{
	xvid_enc_create.num_threads = threads;
}

bool XvidOptions::getParAsInput(void)
{
	return _parAsInput;
}

void XvidOptions::setParAsInput(bool parAsInput)
{
	_parAsInput = parAsInput;
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

	if (xvid_enc_frame.par_width == xvid_enc_frame.par_height)
		xvid_enc_frame.par = XVID_PAR_11_VGA;
	else
		xvid_enc_frame.par = XVID_PAR_EXT;
}

MotionEstimationMode XvidOptions::getMotionEstimation(void)
{
	MotionEstimationMode motionEstimation = ME_NONE;

	if ((xvid_enc_frame.motion & ME_HIGH) == ME_HIGH)
		motionEstimation = ME_HIGH;
	else if ((xvid_enc_frame.motion & ME_MEDIUM) == ME_MEDIUM)
		motionEstimation = ME_MEDIUM;
	else if ((xvid_enc_frame.motion & ME_LOW) == ME_LOW)
		motionEstimation = ME_LOW;

	return motionEstimation;
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

		if (motionEstimation == ME_NONE)
			xvid_enc_frame.type = XVID_TYPE_IVOP;
		else
			xvid_enc_frame.type = XVID_TYPE_AUTO;
	}
}

RateDistortionMode XvidOptions::getRateDistortion(void)
{
	RateDistortionMode rateDistortion = RD_NONE;

	if ((xvid_enc_frame.motion & RD_SQUARE) == RD_SQUARE)
		rateDistortion = RD_SQUARE;
	else if ((xvid_enc_frame.motion & RD_HPEL_QPEL_8) == RD_HPEL_QPEL_8)
		rateDistortion = RD_HPEL_QPEL_8;
	else if ((xvid_enc_frame.motion & RD_HPEL_QPEL_16) == RD_HPEL_QPEL_16)
		rateDistortion = RD_HPEL_QPEL_16;
	else if ((xvid_enc_frame.vop_flags & XVID_VOP_MODEDECISION_RD) == XVID_VOP_MODEDECISION_RD)
		rateDistortion = RD_DCT_ME;

	return rateDistortion;
}

void XvidOptions::setRateDistortion(RateDistortionMode rateDistortion)
{
	if (rateDistortion == RD_NONE || rateDistortion == RD_DCT_ME || rateDistortion == RD_HPEL_QPEL_16 || 
		rateDistortion == RD_HPEL_QPEL_8 || rateDistortion == RD_SQUARE)
	{
		xvid_enc_frame.motion &= ~RD_HPEL_QPEL_16;
		xvid_enc_frame.motion &= ~RD_HPEL_QPEL_8;
		xvid_enc_frame.motion &= ~RD_SQUARE;

		if (rateDistortion == RD_NONE)
			xvid_enc_frame.vop_flags &= ~XVID_VOP_MODEDECISION_RD;
		else
		{
			if (rateDistortion != RD_DCT_ME)
				xvid_enc_frame.motion |= rateDistortion;

			xvid_enc_frame.vop_flags |= XVID_VOP_MODEDECISION_RD;
		}
	}
}

bool XvidOptions::getBframeRdo(void)
{
	return xvid_enc_frame.vop_flags & XVID_VOP_RD_BVOP;
}

void XvidOptions::setBframeRdo(bool bFrameRdo)
{
	if (bFrameRdo)
		xvid_enc_frame.vop_flags |= XVID_VOP_RD_BVOP;
	else
		xvid_enc_frame.vop_flags &= ~XVID_VOP_RD_BVOP;
}

bool XvidOptions::getChromaMotionEstimation(void)
{
	return xvid_enc_frame.motion & CHROMA_ME;
}

void XvidOptions::setChromaMotionEstimation(bool chromaMotionEstimation)
{
	if (chromaMotionEstimation)
		xvid_enc_frame.motion |= CHROMA_ME;
	else
		xvid_enc_frame.motion &= ~CHROMA_ME;
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

bool XvidOptions::getTurboMode(void)
{
	return xvid_enc_frame.motion & TURBO_MODE;
}

void XvidOptions::setTurboMode(bool turboMode)
{
	if (turboMode)
		xvid_enc_frame.motion |= TURBO_MODE;
	else
		xvid_enc_frame.motion &= ~TURBO_MODE;
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

InterlacedMode XvidOptions::getInterlaced(void)
{
	InterlacedMode interlaced = INTERLACED_NONE;

	if (xvid_enc_frame.vop_flags & XVID_VOP_TOPFIELDFIRST)
		interlaced = INTERLACED_TFF;
	else if (xvid_enc_frame.vol_flags & XVID_VOL_INTERLACING)
		interlaced = INTERLACED_BFF;

	return interlaced;
}

void XvidOptions::setInterlaced(InterlacedMode interlaced)
{
	if (interlaced == INTERLACED_NONE)
	{
		xvid_enc_frame.vol_flags &= ~XVID_VOL_INTERLACING;
		xvid_enc_frame.vop_flags &= ~XVID_VOP_TOPFIELDFIRST;
	}
	else if (interlaced == INTERLACED_BFF || interlaced == INTERLACED_TFF)
	{
		xvid_enc_frame.vol_flags |= XVID_VOL_INTERLACING;

		if (interlaced == INTERLACED_TFF)
			xvid_enc_frame.vop_flags |= XVID_VOP_TOPFIELDFIRST;
		else
			xvid_enc_frame.vop_flags &= ~XVID_VOP_TOPFIELDFIRST;
	}
}

unsigned int XvidOptions::getFrameDropRatio(void)
{
	return xvid_enc_create.frame_drop_ratio;
}

void XvidOptions::setFrameDropRatio(unsigned int frameDropRatio)
{
	if (frameDropRatio <= 100)
		xvid_enc_create.frame_drop_ratio = frameDropRatio;
}

unsigned int XvidOptions::getMaxKeyInterval(void)
{
	return xvid_enc_create.max_key_interval;
}

void XvidOptions::setMaxKeyInterval(unsigned int maxKeyInterval)
{
	xvid_enc_create.max_key_interval = maxKeyInterval;
}

unsigned int XvidOptions::getMaxBframes(void)
{
	return xvid_enc_create.max_bframes;
}

void XvidOptions::setMaxBframes(unsigned int maxBframes)
{
	if (maxBframes <= 20)
		xvid_enc_create.max_bframes = maxBframes;
}

int XvidOptions::getBframeSensitivity(void)
{
	return xvid_enc_frame.bframe_threshold;
}

void XvidOptions::setBframeSensitivity(int bFrameSensitivity)
{
	if (bFrameSensitivity >= -255 && bFrameSensitivity <= 255)
		xvid_enc_frame.bframe_threshold = bFrameSensitivity;
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

void XvidOptions::getMinQuantiser(unsigned int *i, unsigned int *p, unsigned int *b)
{
	*i = xvid_enc_create.min_quant[0];
	*p = xvid_enc_create.min_quant[1];
	*b = xvid_enc_create.min_quant[2];
}

void XvidOptions::setMinQuantiser(unsigned int i, unsigned int p, unsigned int b)
{
	if (i > 0 && i <= 31)
		xvid_enc_create.min_quant[0] = i;

	if (p > 0 && p <= 31)
		xvid_enc_create.min_quant[1] = p;

	if (b > 0 && b <= 31)
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
	if (i > 0 && i <= 31)
		xvid_enc_create.max_quant[0] = i;

	if (p > 0 && p <= 31)
		xvid_enc_create.max_quant[1] = p;

	if (b > 0 && b <= 31)
		xvid_enc_create.max_quant[2] = b;
}

unsigned int XvidOptions::getBframeQuantiserRatio(void)
{
	return xvid_enc_create.bquant_ratio;
}

void XvidOptions::setBframeQuantiserRatio(unsigned int ratio)
{
	if (ratio <= 200)
		xvid_enc_create.bquant_ratio = ratio;
}

unsigned int XvidOptions::getBframeQuantiserOffset(void)
{
	return xvid_enc_create.bquant_offset;
}

void XvidOptions::setBframeQuantiserOffset(unsigned int offset)
{
	if (offset <= 200)
		xvid_enc_create.bquant_offset = offset;
}

CqmPresetMode XvidOptions::getCqmPreset(void)
{
	return _cqmPreset;
}

void XvidOptions::setCqmPreset(CqmPresetMode cqmPreset)
{
	if (cqmPreset == CQM_H263 || cqmPreset == CQM_MPEG || cqmPreset == CQM_CUSTOM)
	{
		_cqmPreset = cqmPreset;

		switch (cqmPreset)
		{
			case CQM_MPEG:
			case CQM_CUSTOM:
				xvid_enc_frame.vol_flags |= XVID_VOL_MPEGQUANT;
				break;
			default:
				xvid_enc_frame.vol_flags &= ~XVID_VOL_MPEGQUANT;
		}
	}
}

void XvidOptions::getIntraMatrix(unsigned char intra[64])
{
	memcpy(intra, _intraMatrix, 64 * sizeof(unsigned char));
}

void XvidOptions::setIntraMatrix(unsigned char intra[64])
{
	memcpy(_intraMatrix, intra, 64 * sizeof(unsigned char));
}

void XvidOptions::getInterMatrix(unsigned char inter[64])
{
	memcpy(inter, _interMatrix, 64 * sizeof(unsigned char));
}

void XvidOptions::setInterMatrix(unsigned char inter[64])
{
	memcpy(_interMatrix, inter, 64 * sizeof(unsigned char));
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

unsigned int XvidOptions::getReactionDelayFactor(void)
{
	return xvid_plugin_single.reaction_delay_factor;
}

void XvidOptions::setReactionDelayFactor(unsigned int delayFactor)
{
	if (delayFactor <= 100)
		xvid_plugin_single.reaction_delay_factor = delayFactor;
}

unsigned int XvidOptions::getAveragingQuantiserPeriod(void)
{
	return xvid_plugin_single.averaging_period;
}

void XvidOptions::setAveragingQuantiserPeriod(unsigned int averagingPeriod)
{
	xvid_plugin_single.averaging_period = averagingPeriod;
}

unsigned int XvidOptions::getSmoother(void)
{
	return xvid_plugin_single.buffer;
}

void XvidOptions::setSmoother(unsigned int smoother)
{
	xvid_plugin_single.buffer = smoother;
}

unsigned int XvidOptions::getKeyFrameBoost(void)
{
	return xvid_plugin_2pass2.keyframe_boost;
}

void XvidOptions::setKeyFrameBoost(unsigned int keyFrameBoost)
{
	if (keyFrameBoost <= 100)
		xvid_plugin_2pass2.keyframe_boost = keyFrameBoost;
}

unsigned int XvidOptions::getMaxKeyFrameReduceBitrate(void)
{
	return xvid_plugin_2pass2.kfreduction;
}

void XvidOptions::setMaxKeyFrameReduceBitrate(unsigned int bitrateReduction)
{
	if (bitrateReduction <= 100)
		xvid_plugin_2pass2.kfreduction = bitrateReduction;
}

unsigned int XvidOptions::getKeyFrameBitrateThreshold(void)
{
	return xvid_plugin_2pass2.kfthreshold;
}

void XvidOptions::setKeyFrameBitrateThreshold(unsigned int bitrateThreshold)
{
	xvid_plugin_2pass2.kfthreshold = bitrateThreshold;
}

unsigned int XvidOptions::getOverflowControlStrength(void)
{
	return xvid_plugin_2pass2.overflow_control_strength;
}

void XvidOptions::setOverflowControlStrength(unsigned int overflowControlStrength)
{
	if (overflowControlStrength <= 100)
		xvid_plugin_2pass2.overflow_control_strength = overflowControlStrength;
}

unsigned int XvidOptions::getMaxOverflowImprovement(void)
{
	return xvid_plugin_2pass2.max_overflow_improvement;
}

void XvidOptions::setMaxOverflowImprovement(unsigned int overflowImprovement)
{
	if (overflowImprovement <= 100)
		xvid_plugin_2pass2.max_overflow_improvement = overflowImprovement;
}

unsigned int XvidOptions::getMaxOverflowDegradation(void)
{
	return xvid_plugin_2pass2.max_overflow_degradation;
}

void XvidOptions::setMaxOverflowDegradation(unsigned int overflowDegradation)
{
	if (overflowDegradation <= 100)
		xvid_plugin_2pass2.max_overflow_degradation = overflowDegradation;
}

unsigned int XvidOptions::getAboveAverageCurveCompression(void)
{
	return xvid_plugin_2pass2.curve_compression_high;
}

void XvidOptions::setAboveAverageCurveCompression(unsigned int curveCompression)
{
	if (curveCompression <= 100)
		xvid_plugin_2pass2.curve_compression_high = curveCompression;
}

unsigned int XvidOptions::getBelowAverageCurveCompression(void)
{
	return xvid_plugin_2pass2.curve_compression_low;
}

void XvidOptions::setBelowAverageCurveCompression(unsigned int curveCompression)
{
	if (curveCompression <= 100)
		xvid_plugin_2pass2.curve_compression_low = curveCompression;
}

unsigned int XvidOptions::getVbvBufferSize(void)
{
	return xvid_plugin_2pass2.vbv_size;
}

void XvidOptions::setVbvBufferSize(unsigned int bufferSize)
{
	if (bufferSize <= 6291456)
	{
		xvid_plugin_2pass2.vbv_size = bufferSize;
		xvid_plugin_2pass2.vbv_initial = (bufferSize * 3) >> 2;
	}
}

unsigned int XvidOptions::getMaxVbvBitrate(void)
{
	return xvid_plugin_2pass2.vbv_maxrate;
}

void XvidOptions::setMaxVbvBitrate(unsigned int bitrate)
{
	if (bitrate <= 9708400)
		xvid_plugin_2pass2.vbv_maxrate = bitrate;
}

unsigned int XvidOptions::getVbvPeakBitrate(void)
{
	return xvid_plugin_2pass2.vbv_peakrate;
}

void XvidOptions::setVbvPeakBitrate(unsigned int peakBitrate)
{
	if (peakBitrate <= 16000000)
		xvid_plugin_2pass2.vbv_peakrate = peakBitrate;
}

void XvidOptions::addOptionsToXml(xmlNodePtr xmlNodeRoot)
{
	const int bufferSize = 100;
	xmlChar xmlBuffer[bufferSize + 1];
	xmlNodePtr xmlNodeChild, xmlNodeChild2;

	xmlNodeRoot = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)getOptionsTagRoot(), NULL);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"threads", number2String(xmlBuffer, bufferSize, getThreads()));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"vui", NULL);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"sarAsInput", boolean2String(xmlBuffer, bufferSize, getParAsInput()));

	unsigned int width, height;

	getPar(&width, &height);

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"sarHeight", number2String(xmlBuffer, bufferSize, height));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"sarWidth", number2String(xmlBuffer, bufferSize, width));

	switch (getMotionEstimation())
	{
		case ME_LOW:
			strcpy((char*)xmlBuffer, "low");
			break;
		case ME_MEDIUM:
			strcpy((char*)xmlBuffer, "medium");
			break;
		case ME_HIGH:
			strcpy((char*)xmlBuffer, "high");
			break;
		default:
			strcpy((char*)xmlBuffer, "none");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"motionEstimation", xmlBuffer);

	switch (getRateDistortion())
	{
		case RD_DCT_ME:
			strcpy((char*)xmlBuffer, "dct");
			break;
		case RD_HPEL_QPEL_16:
			strcpy((char*)xmlBuffer, "hpelQpel16");
			break;
		case RD_HPEL_QPEL_8:
			strcpy((char*)xmlBuffer, "hpelQpel8");
			break;
		case RD_SQUARE:
			strcpy((char*)xmlBuffer, "square");
			break;
		default:
			strcpy((char*)xmlBuffer, "none");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"rdo", xmlBuffer);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"bFrameRdo", boolean2String(xmlBuffer, bufferSize, getBframeRdo()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"chromaMotionEstimation", boolean2String(xmlBuffer, bufferSize, getChromaMotionEstimation()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"qPel", boolean2String(xmlBuffer, bufferSize, getQpel()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"gmc", boolean2String(xmlBuffer, bufferSize, getGmc()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"turboMode", boolean2String(xmlBuffer, bufferSize, getTurboMode()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"chromaOptimiser", boolean2String(xmlBuffer, bufferSize, getChromaOptimisation()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"fourMv", boolean2String(xmlBuffer, bufferSize, getInterMotionVector()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"cartoon", boolean2String(xmlBuffer, bufferSize, getCartoon()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"greyscale", boolean2String(xmlBuffer, bufferSize, getGreyscale()));

	switch (getInterlaced())
	{
		case INTERLACED_BFF:
			strcpy((char*)xmlBuffer, "bff");
			break;
		case INTERLACED_TFF:
			strcpy((char*)xmlBuffer, "tff");
			break;
		default:
			strcpy((char*)xmlBuffer, "none");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"interlaced", xmlBuffer);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"frameDropRatio", number2String(xmlBuffer, bufferSize, getFrameDropRatio()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"maxIframeInterval", number2String(xmlBuffer, bufferSize, getMaxKeyInterval()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"maxBframes", number2String(xmlBuffer, bufferSize, getMaxBframes()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"bFrameSensitivity", number2String(xmlBuffer, bufferSize, getBframeSensitivity()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"closedGop", boolean2String(xmlBuffer, bufferSize, getClosedGop()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"packed", boolean2String(xmlBuffer, bufferSize, getPacked()));

	unsigned int minI, minP, minB;
	unsigned int maxI, maxP, maxB;

	getMinQuantiser(&minI, &minP, &minB);
	getMaxQuantiser(&maxI, &maxP, &maxB);

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantImin", number2String(xmlBuffer, bufferSize, minI));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantPmin", number2String(xmlBuffer, bufferSize, minP));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantBmin", number2String(xmlBuffer, bufferSize, minB));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantImax", number2String(xmlBuffer, bufferSize, maxI));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantPmax", number2String(xmlBuffer, bufferSize, maxP));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantBmax", number2String(xmlBuffer, bufferSize, maxB));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantBratio", number2String(xmlBuffer, bufferSize, getBframeQuantiserRatio()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantBoffset", number2String(xmlBuffer, bufferSize, getBframeQuantiserOffset()));

	switch (getCqmPreset())
	{
		case CQM_H263:
			strcpy((char*)xmlBuffer, "h.263");
			break;
		case CQM_MPEG:
			strcpy((char*)xmlBuffer, "mpeg");
			break;
		case CQM_CUSTOM:
			strcpy((char*)xmlBuffer, "custom");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"quantType", xmlBuffer);

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"intraMatrix", NULL);

	for (int i = 0; i < 64; i++)
		xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"value", number2String(xmlBuffer, bufferSize, _intraMatrix[i]));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"interMatrix", NULL);

	for (int i = 0; i < 64; i++)
		xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"value", number2String(xmlBuffer, bufferSize, _interMatrix[i]));

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"trellis", boolean2String(xmlBuffer, bufferSize, getTrellis()));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"singlePass", NULL);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"reactionDelayFactor", number2String(xmlBuffer, bufferSize, getReactionDelayFactor()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"averagingQuantiserPeriod", number2String(xmlBuffer, bufferSize, getAveragingQuantiserPeriod()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"smoother", number2String(xmlBuffer, bufferSize, getSmoother()));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"twoPass", NULL);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"keyFrameBoost", number2String(xmlBuffer, bufferSize, getKeyFrameBoost()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"maxKeyFrameReduceBitrate", number2String(xmlBuffer, bufferSize, getMaxKeyFrameReduceBitrate()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"keyFrameBitrateThreshold", number2String(xmlBuffer, bufferSize, getKeyFrameBitrateThreshold()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"overflowControlStrength", number2String(xmlBuffer, bufferSize, getOverflowControlStrength()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"maxOverflowImprovement", number2String(xmlBuffer, bufferSize, getMaxOverflowImprovement()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"maxOverflowDegradation", number2String(xmlBuffer, bufferSize, getMaxOverflowDegradation()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"aboveAverageCurveCompression", number2String(xmlBuffer, bufferSize, getAboveAverageCurveCompression()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"belowAverageCurveCompression", number2String(xmlBuffer, bufferSize, getBelowAverageCurveCompression()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"vbvBufferSize", number2String(xmlBuffer, bufferSize, getVbvBufferSize()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"maxVbvBitrate", number2String(xmlBuffer, bufferSize, getMaxVbvBitrate()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"vbvPeakBitrate", number2String(xmlBuffer, bufferSize, getVbvPeakBitrate()));
}

void XvidOptions::parseOptions(xmlNode *node)
{
	int minI = -1, minP = -1, minB = -1;
	int maxI = -1, maxP = -1, maxB = -1;

	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "threads") == 0)
				setThreads(atoi(content));
			else if (strcmp((char*)xmlChild->name, "vui") == 0)
				parseVuiOptions(xmlChild);
			else if (strcmp((char*)xmlChild->name, "motionEstimation") == 0)
			{
				MotionEstimationMode motionEstimation = ME_NONE;

				if (strcmp(content, "low") == 0)
					motionEstimation = ME_LOW;
				else if (strcmp(content, "medium") == 0)
					motionEstimation = ME_MEDIUM;
				else if (strcmp(content, "high") == 0)
					motionEstimation = ME_HIGH;

				setMotionEstimation(motionEstimation);
			}
			else if (strcmp((char*)xmlChild->name, "rdo") == 0)
			{
				RateDistortionMode rateDistortion = RD_NONE;

				if (strcmp(content, "dct") == 0)
					rateDistortion = RD_DCT_ME;
				else if (strcmp(content, "hpelQpel16") == 0)
					rateDistortion = RD_HPEL_QPEL_16;
				else if (strcmp(content, "hpelQpel8") == 0)
					rateDistortion = RD_HPEL_QPEL_8;
				else if (strcmp(content, "square") == 0)
					rateDistortion = RD_SQUARE;

				setRateDistortion(rateDistortion);
			}
			else if (strcmp((char*)xmlChild->name, "bFrameRdo") == 0)
				setBframeRdo(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "chromaMotionEstimation") == 0)
				setChromaMotionEstimation(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "qPel") == 0)
				setQpel(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "gmc") == 0)
				setGmc(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "turboMode") == 0)
				setTurboMode(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "chromaOptimiser") == 0)
				setChromaOptimisation(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "fourMv") == 0)
				setInterMotionVector(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "cartoon") == 0)
				setCartoon(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "greyscale") == 0)
				setGreyscale(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "interlaced") == 0)
			{
				InterlacedMode interlaced = INTERLACED_NONE;

				if (strcmp(content, "bff") == 0)
					interlaced = INTERLACED_BFF;
				else if (strcmp(content, "tff") == 0)
					interlaced = INTERLACED_TFF;

				setInterlaced(interlaced);
			}
			else if (strcmp((char*)xmlChild->name, "frameDropRatio") == 0)
				setFrameDropRatio(atoi(content));
			else if (strcmp((char*)xmlChild->name, "maxIframeInterval") == 0)
				setMaxKeyInterval(atoi(content));
			else if (strcmp((char*)xmlChild->name, "maxBframes") == 0)
				setMaxBframes(atoi(content));
			else if (strcmp((char*)xmlChild->name, "bFrameSensitivity") == 0)
				setBframeSensitivity(atoi(content));
			else if (strcmp((char*)xmlChild->name, "closedGop") == 0)
				setClosedGop(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "packed") == 0)
				setPacked(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "quantImin") == 0)
				minI = atoi(content);
			else if (strcmp((char*)xmlChild->name, "quantPmin") == 0)
				minP = atoi(content);
			else if (strcmp((char*)xmlChild->name, "quantBmin") == 0)
				minB = atoi(content);
			else if (strcmp((char*)xmlChild->name, "quantImax") == 0)
				maxI = atoi(content);
			else if (strcmp((char*)xmlChild->name, "quantPmax") == 0)
				maxP = atoi(content);
			else if (strcmp((char*)xmlChild->name, "quantBmax") == 0)
				maxB = atoi(content);
			else if (strcmp((char*)xmlChild->name, "quantBratio") == 0)
				setBframeQuantiserRatio(atoi(content));
			else if (strcmp((char*)xmlChild->name, "quantBoffset") == 0)
				setBframeQuantiserOffset(atoi(content));
			else if (strcmp((char*)xmlChild->name, "quantType") == 0)
			{
				if (strcmp(content, "mpeg") == 0)
					setCqmPreset(CQM_MPEG);
				else if (strcmp(content, "custom") == 0)
					setCqmPreset(CQM_CUSTOM);
				else
					setCqmPreset(CQM_H263);
			}
			else if (strcmp((char*)xmlChild->name, "intraMatrix") == 0)
			{
				unsigned char intraMatrix[64];

				parseCqmOption(xmlChild, intraMatrix);
				setIntraMatrix(intraMatrix);
			}
			else if (strcmp((char*)xmlChild->name, "interMatrix") == 0)
			{
				unsigned char interMatrix[64];

				parseCqmOption(xmlChild, interMatrix);
				setInterMatrix(interMatrix);
			}
			else if (strcmp((char*)xmlChild->name, "trellis") == 0)
				setTrellis(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "singlePass") == 0)
				parseSinglePassOptions(xmlChild);
			else if (strcmp((char*)xmlChild->name, "twoPass") == 0)
				parseTwoPassOptions(xmlChild);

			xmlFree(content);
		}
	}

	if (minI != -1 && minP != -1 && minB != -1)
		setMinQuantiser(minI, minP, minB);

	if (maxI != -1 && maxP != -1 && maxB != -1)
		setMaxQuantiser(maxI, maxP, maxB);
}

void XvidOptions::parseVuiOptions(xmlNode *node)
{
	unsigned int width = 0, height = 0;

	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "sarAsInput") == 0)
				setParAsInput(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "sarHeight") == 0)
				height = atoi(content);
			else if (strcmp((char*)xmlChild->name, "sarWidth") == 0)
				width = atoi(content);

			xmlFree(content);
		}
	}

	setPar(width, height);
}

void XvidOptions::parseCqmOption(xmlNode *node, unsigned char matrix[])
{
	int index = 0;

	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			matrix[index] = atoi(content);
			index++;

			xmlFree(content);
		}
	}
}

void XvidOptions::parseSinglePassOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "reactionDelayFactor") == 0)
				setReactionDelayFactor(atoi(content));
			else if (strcmp((char*)xmlChild->name, "averagingQuantiserPeriod") == 0)
				setAveragingQuantiserPeriod(atoi(content));
			else if (strcmp((char*)xmlChild->name, "smoother") == 0)
				setSmoother(atoi(content));

			xmlFree(content);
		}
	}
}

void XvidOptions::parseTwoPassOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "keyFrameBoost") == 0)
				setKeyFrameBoost(atoi(content));
			else if (strcmp((char*)xmlChild->name, "maxKeyFrameReduceBitrate") == 0)
				setMaxKeyFrameReduceBitrate(atoi(content));
			else if (strcmp((char*)xmlChild->name, "keyFrameBitrateThreshold") == 0)
				setKeyFrameBitrateThreshold(atoi(content));
			else if (strcmp((char*)xmlChild->name, "overflowControlStrength") == 0)
				setOverflowControlStrength(atoi(content));
			else if (strcmp((char*)xmlChild->name, "maxOverflowImprovement") == 0)
				setMaxOverflowImprovement(atoi(content));
			else if (strcmp((char*)xmlChild->name, "maxOverflowDegradation") == 0)
				setMaxOverflowDegradation(atoi(content));
			else if (strcmp((char*)xmlChild->name, "aboveAverageCurveCompression") == 0)
				setAboveAverageCurveCompression(atoi(content));
			else if (strcmp((char*)xmlChild->name, "belowAverageCurveCompression") == 0)
				setBelowAverageCurveCompression(atoi(content));
			else if (strcmp((char*)xmlChild->name, "vbvBufferSize") == 0)
				setVbvBufferSize(atoi(content));
			else if (strcmp((char*)xmlChild->name, "maxVbvBitrate") == 0)
				setMaxVbvBitrate(atoi(content));
			else if (strcmp((char*)xmlChild->name, "vbvPeakBitrate") == 0)
				setVbvPeakBitrate(atoi(content));

			xmlFree(content);
		}
	}
}
