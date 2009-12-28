 /***************************************************************************
                               x264Options.cpp

	These settings are intended for scripting and can contain a Preset 
	Configuration block.  If this block exists then "internal" settings are
	ignored and an external configuration file should be read instead, 
	e.g. PlayStation Portable profile.  However, if the external file is 
	missing, internal settings have to be used and will reflect a snapshot
	of the external configuration file at the time the script was generated.

    begin                : Mon Apr 21 2008
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

#include <math.h>
#include <locale.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

#include "config.h"
#include "ADM_inttype.h"
#include "ADM_files.h"

#include "x264Options.h"

x264Options::x264Options(void) : PluginOptions(PLUGIN_CONFIG_DIR, "x264", "x264Param.xsd", DEFAULT_ENCODE_MODE, DEFAULT_ENCODE_MODE_PARAMETER)
{
	memset(&_param, 0, sizeof(x264_param_t));

	reset();
}

void x264Options::cleanUp(void)
{
	PluginOptions::cleanUp();
	clearZones();
}

void x264Options::reset(void)
{
	PluginOptions::reset();

	x264_param_default(&_param);
	_param.vui.i_sar_height = 1;
	_param.vui.i_sar_width = 1;
	_param.i_threads = 0;	// set to auto-detect; default is disabled

	_sarAsInput = false;
}

x264_param_t* x264Options::getParameters(void)
{
	x264_param_t *param = new x264_param_t;

	memcpy(param, &_param, sizeof(x264_param_t));
	param->rc.i_zones = getZoneCount();

	if (param->rc.i_zones)
	{
		param->rc.zones = new x264_zone_t[param->rc.i_zones];

		for (int zone = 0; zone < param->rc.i_zones; zone++)
			_zoneOptions[zone]->setX264Zone(&param->rc.zones[zone]);
	}

	return param;
}

int x264Options::getThreads(void)
{
	return _param.i_threads;
}

void x264Options::setThreads(int threads)
{
	if (threads >= 0)
		_param.i_threads = threads;
}

bool x264Options::getDeterministic(void)
{
	return _param.b_deterministic;
}

void x264Options::setDeterministic(bool deterministic)
{
	_param.b_deterministic = deterministic;
}

#if X264_BUILD >= 75
int x264Options::getThreadedLookahead(void)
{
	return _param.i_sync_lookahead;
}

void x264Options::setThreadedLookahead(int frames)
{
	if (frames >= -1 && frames <= 250)
		_param.i_sync_lookahead = frames;
}
#endif

int x264Options::getIdcLevel(void)
{
	return _param.i_level_idc;
}

void x264Options::setIdcLevel(int idcLevel)
{
	switch (idcLevel)
	{
		case -1:
		case 10:
		case 11:
		case 12:
		case 13:
		case 20:
		case 21:
		case 22:
		case 30:
		case 31:
		case 32:
		case 40:
		case 41:
		case 42:
		case 50:
		case 51:
			_param.i_level_idc = idcLevel;
	}
}

bool x264Options::getSarAsInput(void)
{
	return _sarAsInput;
}

void x264Options::setSarAsInput(bool sarAsInput)
{
	_sarAsInput = sarAsInput;
}

unsigned int x264Options::getSarHeight(void)
{
	return _param.vui.i_sar_height;
}

void x264Options::setSarHeight(unsigned int height)
{
	_param.vui.i_sar_height = height;
}

unsigned int x264Options::getSarWidth(void)
{
	return _param.vui.i_sar_width;
}

void x264Options::setSarWidth(unsigned int width)
{
	_param.vui.i_sar_width = width;
}

unsigned int x264Options::getOverscan(void)
{
	return _param.vui.i_overscan;
}

void x264Options::setOverscan(unsigned int overscan)
{
	if (overscan <= 2)
		_param.vui.i_overscan = overscan;
}

unsigned int x264Options::getVideoFormat(void)
{
	return _param.vui.i_vidformat;
}

void x264Options::setVideoFormat(unsigned int videoFormat)
{
	if (videoFormat <= 5)
		_param.vui.i_vidformat = videoFormat;
}

bool x264Options::getFullRangeSamples(void)
{
	return _param.vui.b_fullrange;
}

void x264Options::setFullRangeSamples(bool fullRangeSamples)
{
	_param.vui.b_fullrange = fullRangeSamples;
}

unsigned int x264Options::getColorPrimaries(void)
{
	return _param.vui.i_colorprim;
}

void x264Options::setColorPrimaries(unsigned int colorPrimaries)
{
	if (colorPrimaries <= 8 && colorPrimaries != 0 && colorPrimaries != 3)
		_param.vui.i_colorprim = colorPrimaries;
}

unsigned int x264Options::getTransfer(void)
{
	return _param.vui.i_transfer;
}

void x264Options::setTransfer(unsigned int transfer)
{
	if (transfer <= 10 && transfer != 0 && transfer != 3)
		_param.vui.i_transfer = transfer;
}

unsigned int x264Options::getColorMatrix(void)
{
	return _param.vui.i_colmatrix;
}

void x264Options::setColorMatrix(unsigned int colorMatrix)
{
	if (colorMatrix <= 8 && colorMatrix != 3)
		_param.vui.i_colmatrix = colorMatrix;
}

unsigned int x264Options::getChromaSampleLocation(void)
{
	return _param.vui.i_chroma_loc;
}

void x264Options::setChromaSampleLocation(unsigned int chromaSampleLocation)
{
	if (chromaSampleLocation <= 5)
		_param.vui.i_chroma_loc = chromaSampleLocation;
}

unsigned int x264Options::getReferenceFrames(void)
{
	return _param.i_frame_reference;
}

void x264Options::setReferenceFrames(unsigned int referenceFrames)
{
	if (referenceFrames <= 16)
		_param.i_frame_reference = referenceFrames;
}

unsigned int x264Options::getGopMinimumSize(void)
{
	return _param.i_keyint_min;
}

void x264Options::setGopMinimumSize(unsigned int gopSize)
{
	if (gopSize <= 100)
		_param.i_keyint_min = gopSize;
}

unsigned int x264Options::getGopMaximumSize(void)
{
	return _param.i_keyint_max;
}

void x264Options::setGopMaximumSize(unsigned int gopSize)
{
	if (gopSize <= 1000)
		_param.i_keyint_max = gopSize;
}

unsigned int x264Options::getScenecutThreshold(void)
{
	return _param.i_scenecut_threshold;
}

void x264Options::setScenecutThreshold(unsigned int scenecutThreshold)
{
	if (scenecutThreshold <= 100)
		_param.i_scenecut_threshold = scenecutThreshold;
}

unsigned int x264Options::getBFrames(void)
{
	return _param.i_bframe;
}

void x264Options::setBFrames(unsigned int bFrames)
{
	if (bFrames <= 100)
		_param.i_bframe = bFrames;
}

unsigned int x264Options::getAdaptiveBFrameDecision(void)
{
	return _param.i_bframe_adaptive;
}

void x264Options::setAdaptiveBFrameDecision(unsigned int adaptiveBframeDecision)
{
	if (adaptiveBframeDecision <= 2)
		_param.i_bframe_adaptive = adaptiveBframeDecision;
}

int x264Options::getBFrameBias(void)
{
	return _param.i_bframe_bias;
}

void x264Options::setBFrameBias(int bFrameBias)
{
	if (bFrameBias >= -100 && bFrameBias <= 100)
		_param.i_bframe_bias = bFrameBias;
}

unsigned int x264Options::getBFrameReferences(void)
{
#if X264_BUILD >= 78
	return _param.i_bframe_pyramid;
#else
	return _param.b_bframe_pyramid;
#endif
}

void x264Options::setBFrameReferences(unsigned int bFrameReferences)
{
#if X264_BUILD >= 78
	if (bFrameReferences <= 2)
		_param.i_bframe_pyramid = bFrameReferences;
#else
	_param.b_bframe_pyramid = !!bFrameReferences;
#endif
}

bool x264Options::getLoopFilter(void)
{
	return _param.b_deblocking_filter;
}

void x264Options::setLoopFilter(bool loopFilter)
{
	_param.b_deblocking_filter = loopFilter;
}

int x264Options::getLoopFilterAlphaC0(void)
{
	return _param.i_deblocking_filter_alphac0;
}

void x264Options::setLoopFilterAlphaC0(int loopFilterAlphaC0)
{
	if (loopFilterAlphaC0 >= -6 && loopFilterAlphaC0 <= 6)
		_param.i_deblocking_filter_alphac0 = loopFilterAlphaC0;
}

int x264Options::getLoopFilterBeta(void)
{
	return _param.i_deblocking_filter_beta;
}

void x264Options::setLoopFilterBeta(int loopFilterBeta)
{
	if (loopFilterBeta >= -6 && loopFilterBeta <= 6)
		_param.i_deblocking_filter_beta = loopFilterBeta;
}

bool x264Options::getCabac(void)
{
	return _param.b_cabac;
}

void x264Options::setCabac(bool cabac)
{
	_param.b_cabac = cabac;
}

bool x264Options::getInterlaced(void)
{
	return _param.b_interlaced;
}

void x264Options::setInterlaced(bool interlaced)
{
	_param.b_interlaced = interlaced;
}

#if X264_BUILD >= 77
bool x264Options::getConstrainedIntraPrediction(void)
{
	return _param.b_constrained_intra;
}

void x264Options::setConstrainedIntraPrediction(bool constrainedIntra)
{
	_param.b_constrained_intra = constrainedIntra;
}
#endif

unsigned int x264Options::getCqmPreset(void)
{
	return _param.i_cqm_preset;
}

void x264Options::setCqmPreset(unsigned int cqmPreset)
{
	if (cqmPreset <= 2)
		_param.i_cqm_preset = cqmPreset;
}

uint8_t* x264Options::getIntra4x4Luma(void)
{
	return _param.cqm_4iy;
}

void x264Options::setIntra4x4Luma(uint8_t intra4x4Luma[])
{
	memcpy(_param.cqm_4iy, intra4x4Luma, 16 * sizeof(uint8_t));
}

uint8_t* x264Options::getIntraChroma(void)
{
	return _param.cqm_4ic;
}

void x264Options::setIntraChroma(uint8_t intraChroma[])
{
	memcpy(_param.cqm_4ic, intraChroma, 16 * sizeof(uint8_t));
}

uint8_t* x264Options::getInter4x4Luma(void)
{
	return _param.cqm_4py;
}

void x264Options::setInter4x4Luma(uint8_t inter4x4Luma[])
{
	memcpy(_param.cqm_4py, inter4x4Luma, 16 * sizeof(uint8_t));
}

uint8_t* x264Options::getInterChroma(void)
{
	return _param.cqm_4pc;
}

void x264Options::setInterChroma(uint8_t interChroma[])
{
	memcpy(_param.cqm_4pc, interChroma, 16 * sizeof(uint8_t));
}

uint8_t* x264Options::getIntra8x8Luma(void)
{
	return _param.cqm_8iy;
}

void x264Options::setIntra8x8Luma(uint8_t intra8x8Luma[])
{
	memcpy(_param.cqm_8iy, intra8x8Luma, 64 * sizeof(uint8_t));
}

uint8_t* x264Options::getInter8x8Luma(void)
{
	return _param.cqm_8py;
}

void x264Options::setInter8x8Luma(uint8_t inter8x8Luma[])
{
	memcpy(_param.cqm_8py, inter8x8Luma, 64 * sizeof(uint8_t));
}

bool x264Options::getPartitionI4x4(void)
{
	return (_param.analyse.inter & X264_ANALYSE_I4x4) == X264_ANALYSE_I4x4;
}

void x264Options::setPartitionI4x4(bool partitionI4x4)
{
	if (partitionI4x4)
		_param.analyse.inter |= X264_ANALYSE_I4x4;
	else
		_param.analyse.inter &= ~X264_ANALYSE_I4x4;
}

bool x264Options::getPartitionI8x8(void)
{
	return (_param.analyse.inter & X264_ANALYSE_I8x8) == X264_ANALYSE_I8x8;
}

void x264Options::setPartitionI8x8(bool partitionI8x8)
{
	if (partitionI8x8)
		_param.analyse.inter |= X264_ANALYSE_I8x8;
	else
		_param.analyse.inter &= ~X264_ANALYSE_I8x8;
}

bool x264Options::getPartitionP8x8(void)
{
	return (_param.analyse.inter & X264_ANALYSE_PSUB16x16) == X264_ANALYSE_PSUB16x16;
}

void x264Options::setPartitionP8x8(bool partitionP8x8)
{
	if (partitionP8x8)
		_param.analyse.inter |= X264_ANALYSE_PSUB16x16;
	else
		_param.analyse.inter &= ~X264_ANALYSE_PSUB16x16;
}

bool x264Options::getPartitionP4x4(void)
{
	return (_param.analyse.inter & X264_ANALYSE_PSUB8x8) == X264_ANALYSE_PSUB8x8;
}

void x264Options::setPartitionP4x4(bool partitionP4x4)
{
	if (partitionP4x4)
		_param.analyse.inter |= X264_ANALYSE_PSUB8x8;
	else
		_param.analyse.inter &= ~X264_ANALYSE_PSUB8x8;
}

bool x264Options::getPartitionB8x8(void)
{
	return (_param.analyse.inter & X264_ANALYSE_BSUB16x16) == X264_ANALYSE_BSUB16x16;
}

void x264Options::setPartitionB8x8(bool partitionB8x8)
{
	if (partitionB8x8)
		_param.analyse.inter |= X264_ANALYSE_BSUB16x16;
	else
		_param.analyse.inter &= ~X264_ANALYSE_BSUB16x16;
}

bool x264Options::getDct8x8(void)
{
	return _param.analyse.b_transform_8x8;
}

void x264Options::setDct8x8(bool dct8x8)
{
	_param.analyse.b_transform_8x8 = dct8x8;
}

#if X264_BUILD >= 79
unsigned int x264Options::getWeightedPredictionPFrames(void)
{
	return _param.analyse.i_weighted_pred;
}

void x264Options::setWeightedPredictionPFrames(unsigned int weightedPrediction)
{
	_param.analyse.i_weighted_pred = weightedPrediction;
}
#endif

bool x264Options::getWeightedPrediction(void)
{
	return _param.analyse.b_weighted_bipred;
}

void x264Options::setWeightedPrediction(bool weightedPrediction)
{
	_param.analyse.b_weighted_bipred = weightedPrediction;
}

unsigned int x264Options::getDirectPredictionMode(void)
{
	return _param.analyse.i_direct_mv_pred;
}

void x264Options::setDirectPredictionMode(unsigned int directPredictionMode)
{
	if (directPredictionMode <= 3)
		_param.analyse.i_direct_mv_pred = directPredictionMode;
}

int x264Options::getChromaLumaQuantiserDifference(void)
{
	return _param.analyse.i_chroma_qp_offset;
}

void x264Options::setChromaLumaQuantiserDifference(int chromaLumaQuantiserDifference)
{
	if (chromaLumaQuantiserDifference >= -12 && chromaLumaQuantiserDifference <= 12)
		_param.analyse.i_chroma_qp_offset = chromaLumaQuantiserDifference;
}

unsigned int x264Options::getMotionEstimationMethod(void)
{
	return _param.analyse.i_me_method;
}

void x264Options::setMotionEstimationMethod(unsigned int motionEstimationMethod)
{
	if (motionEstimationMethod <= 4)
		_param.analyse.i_me_method = motionEstimationMethod;
}

unsigned int x264Options::getMotionVectorSearchRange(void)
{
	return _param.analyse.i_me_range;
}

void x264Options::setMotionVectorSearchRange(unsigned int motionVectorSearchRange)
{
	if (motionVectorSearchRange <= 64)
		_param.analyse.i_me_range = motionVectorSearchRange;
}

int x264Options::getMotionVectorLength(void)
{
	return _param.analyse.i_mv_range;
}

void x264Options::setMotionVectorLength(int motionVectorLength)
{
	if (motionVectorLength == -1 || (motionVectorLength >= 32 && motionVectorLength <= 512))
		_param.analyse.i_mv_range = motionVectorLength;
}

int x264Options::getMotionVectorThreadBuffer(void)
{
	return _param.analyse.i_mv_range_thread;
}

void x264Options::setMotionVectorThreadBuffer(int motionVectorThreadBuffer)
{
	_param.analyse.i_mv_range_thread = motionVectorThreadBuffer;
}

unsigned int x264Options::getSubpixelRefinement(void)
{
	return _param.analyse.i_subpel_refine;
}

void x264Options::setSubpixelRefinement(unsigned int subpixelRefinement)
{
	if (subpixelRefinement >= 1 && subpixelRefinement <= 9)
		_param.analyse.i_subpel_refine = subpixelRefinement;
}

bool x264Options::getChromaMotionEstimation(void)
{
	return _param.analyse.b_chroma_me;
}

void x264Options::setChromaMotionEstimation(bool chromaMotionEstimation)
{
	_param.analyse.b_chroma_me = chromaMotionEstimation;
}

bool x264Options::getMixedReferences(void)
{
	return _param.analyse.b_mixed_references;
}

void x264Options::setMixedReferences(bool mixedReferences)
{
	_param.analyse.b_mixed_references = mixedReferences;
}

unsigned int x264Options::getTrellis(void)
{
	return _param.analyse.i_trellis;
}

void x264Options::setTrellis(unsigned int trellis)
{
	if (trellis <= 2)
		_param.analyse.i_trellis = trellis;
}

bool x264Options::getFastPSkip(void)
{
	return _param.analyse.b_fast_pskip;
}

void x264Options::setFastPSkip(bool fastPSkip)
{
	_param.analyse.b_fast_pskip = fastPSkip;
}

bool x264Options::getDctDecimate(void)
{
	return _param.analyse.b_dct_decimate;
}

void x264Options::setDctDecimate(bool dctDecimate)
{
	_param.analyse.b_dct_decimate = dctDecimate;
}

float x264Options::getPsychoRdo(void)
{
	return _param.analyse.f_psy_rd;
}

void x264Options::setPsychoRdo(float psychoRdo)
{
	if (psychoRdo >= 0 && psychoRdo <= 10)
		_param.analyse.f_psy_rd = psychoRdo;
}

unsigned int x264Options::getNoiseReduction(void)
{
	return _param.analyse.i_noise_reduction;
}

void x264Options::setNoiseReduction(unsigned int noiseReduction)
{
	if (noiseReduction <= 65536)
		_param.analyse.i_noise_reduction = noiseReduction;
}

unsigned int x264Options::getInterLumaDeadzone(void)
{
	return _param.analyse.i_luma_deadzone[0];
}

void x264Options::setInterLumaDeadzone(unsigned int interLumaDeadzone)
{
	if (interLumaDeadzone <= 32)
		_param.analyse.i_luma_deadzone[0] = interLumaDeadzone;
}

unsigned int x264Options::getIntraLumaDeadzone(void)
{
	return _param.analyse.i_luma_deadzone[1];
}

void x264Options::setIntraLumaDeadzone(unsigned int intraLumaDeadzone)
{
	if (intraLumaDeadzone <= 32)
		_param.analyse.i_luma_deadzone[1] = intraLumaDeadzone;
}

unsigned int x264Options::getQuantiserMinimum(void)
{
	return _param.rc.i_qp_min;
}

void x264Options::setQuantiserMinimum(unsigned int quantiserMinimum)
{
	if (quantiserMinimum >= 10 && quantiserMinimum <= 51)
		_param.rc.i_qp_min = quantiserMinimum;
}

unsigned int x264Options::getQuantiserMaximum(void)
{
	return _param.rc.i_qp_max;
}

void x264Options::setQuantiserMaximum(unsigned int quantiserMaximum)
{
	if (quantiserMaximum >= 10 && quantiserMaximum <= 51)
		_param.rc.i_qp_max = quantiserMaximum;
}

unsigned int x264Options::getQuantiserStep(void)
{
	return _param.rc.i_qp_step;
}

void x264Options::setQuantiserStep(unsigned int quantiserStep)
{
	if (quantiserStep <= 10)
		_param.rc.i_qp_step = quantiserStep;
}

float x264Options::getAverageBitrateTolerance(void)
{
	return _param.rc.f_rate_tolerance;
}

void x264Options::setAverageBitrateTolerance(float averageBitrateTolerance)
{
	if (averageBitrateTolerance >= 0 && averageBitrateTolerance <= 1)
		_param.rc.f_rate_tolerance = averageBitrateTolerance;
}

unsigned int x264Options::getVbvMaximumBitrate(void)
{
	return _param.rc.i_vbv_max_bitrate;
}

void x264Options::setVbvMaximumBitrate(unsigned int vbvMaximumBitrate)
{
	if (vbvMaximumBitrate <= 99999)
		_param.rc.i_vbv_max_bitrate = vbvMaximumBitrate;
}

unsigned int x264Options::getVbvBufferSize(void)
{
	return _param.rc.i_vbv_buffer_size;
}

void x264Options::setVbvBufferSize(unsigned int vbvBufferSize)
{
	if (vbvBufferSize <= 99999)
		_param.rc.i_vbv_buffer_size = vbvBufferSize;
}

float x264Options::getVbvInitialOccupancy(void)
{
	return _param.rc.f_vbv_buffer_init;
}

void x264Options::setVbvInitialOccupancy(float vbvInitialOccupancy)
{
	if (vbvInitialOccupancy >= 0 && vbvInitialOccupancy <= 1)
		_param.rc.f_vbv_buffer_init = vbvInitialOccupancy;
}

float x264Options::getIpFrameQuantiser(void)
{
	return _param.rc.f_ip_factor;
}

void x264Options::setIpFrameQuantiser(float ipFrameQuantiser)
{
	if (ipFrameQuantiser >= 1 && ipFrameQuantiser <= 10)
		_param.rc.f_ip_factor = ipFrameQuantiser;
}

float x264Options::getPbFrameQuantiser(void)
{
	return _param.rc.f_pb_factor;
}

void x264Options::setPbFrameQuantiser(float pbFrameQuantiser)
{
	if (pbFrameQuantiser >= 1 && pbFrameQuantiser <= 10)
		_param.rc.f_pb_factor = pbFrameQuantiser;
}

unsigned int x264Options::getAdaptiveQuantiserMode(void)
{
	return _param.rc.i_aq_mode;
}

void x264Options::setAdaptiveQuantiserMode(unsigned int adaptiveQuantiserMode)
{
	if (adaptiveQuantiserMode <= 1)
		_param.rc.i_aq_mode = adaptiveQuantiserMode;
}

float x264Options::getAdaptiveQuantiserStrength(void)
{
	return _param.rc.f_aq_strength;
}

void x264Options::setAdaptiveQuantiserStrength(float adaptiveQuantiserStrength)
{
	_param.rc.f_aq_strength = adaptiveQuantiserStrength;
}

#if X264_BUILD >= 69
bool x264Options::getMbTree(void)
{
	return _param.rc.b_mb_tree;
}

void x264Options::setMbTree(bool mbTree)
{
	_param.rc.b_mb_tree = mbTree;
}

unsigned int x264Options::getFrametypeLookahead(void)
{
	return _param.rc.i_lookahead;
}

void x264Options::setFrametypeLookahead(unsigned int frames)
{
	if (frames <= 250)
		_param.rc.i_lookahead = frames;
}
#endif

float x264Options::getQuantiserCurveCompression(void)
{
	return _param.rc.f_qcompress;
}

void x264Options::setQuantiserCurveCompression(float quantiserCurveCompression)
{
	if (quantiserCurveCompression >= 0 && quantiserCurveCompression <= 1)
		_param.rc.f_qcompress = quantiserCurveCompression;
}

float x264Options::getReduceFluxBeforeCurveCompression(void)
{
	return _param.rc.f_complexity_blur;
}

void x264Options::setReduceFluxBeforeCurveCompression(float reduceFluxBeforeCurveCompression)
{
	if (reduceFluxBeforeCurveCompression >= 0 && reduceFluxBeforeCurveCompression <= 999)
		_param.rc.f_complexity_blur = reduceFluxBeforeCurveCompression;
}

float x264Options::getReduceFluxAfterCurveCompression(void)
{
	return _param.rc.f_qblur;
}

void x264Options::setReduceFluxAfterCurveCompression(float reduceFluxAfterCurveCompression)
{
	if (reduceFluxAfterCurveCompression >= 0 && reduceFluxAfterCurveCompression <= 1)
		_param.rc.f_qblur = reduceFluxAfterCurveCompression;
}

unsigned int x264Options::getZoneCount(void)
{
	return _zoneOptions.size();
}

x264ZoneOptions** x264Options::getZones(void)
{
	unsigned int zoneCount = getZoneCount();
	x264ZoneOptions **zoneOptions = new x264ZoneOptions*[zoneCount];

	for (int zone = 0; zone < zoneCount; zone++)
		zoneOptions[zone] = _zoneOptions[zone]->clone();

	return zoneOptions;
}

void x264Options::clearZones(void)
{
	for (int zone = 0; zone < getZoneCount(); zone++)
		delete _zoneOptions[zone];

	_zoneOptions.clear();
}

void x264Options::addZone(x264ZoneOptions *zoneOptions)
{
	x264ZoneOptions *clonedOptions = zoneOptions->clone();

	_zoneOptions.push_back(clonedOptions);
}

bool x264Options::getAccessUnitDelimiters(void)
{
	return _param.b_aud;
}

void x264Options::setAccessUnitDelimiters(bool accessUnitDelimiters)
{
	_param.b_aud = accessUnitDelimiters;
}

unsigned int x264Options::getSpsIdentifier(void)
{
	return _param.i_sps_id;
}

void x264Options::setSpsIdentifier(unsigned int spsIdentifier)
{
	switch (spsIdentifier)
	{
		case 0:
		case 1:
		case 3:
		case 7:
		case 15:
		case 31:
			_param.i_sps_id = spsIdentifier;
	}
}

#if X264_BUILD >= 73
unsigned int x264Options::getSliceMaxSize(void)
{
	return _param.i_slice_max_size;
}

void x264Options::setSliceMaxSize(unsigned int maxSize)
{
	_param.i_slice_max_size = maxSize;
}

unsigned int x264Options::getSliceMaxMacroblocks(void)
{
	return _param.i_slice_max_mbs;
}

void x264Options::setSliceMaxMacroblocks(unsigned int maxMbs)
{
	_param.i_slice_max_mbs = maxMbs;
}

unsigned int x264Options::getSliceCount(void)
{
	return _param.i_slice_count;
}

void x264Options::setSliceCount(unsigned int sliceCount)
{
	_param.i_slice_count = sliceCount;
}
#endif

void x264Options::addOptionsToXml(xmlNodePtr xmlNodeRoot)
{
	const int bufferSize = 100;
	xmlChar xmlBuffer[bufferSize + 1];
	xmlNodePtr xmlNodeChild, xmlNodeChild2;

	xmlNodeRoot = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)getOptionsTagRoot(), NULL);
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"threads", number2String(xmlBuffer, bufferSize, getThreads()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"deterministic", boolean2String(xmlBuffer, bufferSize, getDeterministic()));
#if X264_BUILD >= 75
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"threadedLookahead", number2String(xmlBuffer, bufferSize, getThreadedLookahead()));
#endif
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"idcLevel", number2String(xmlBuffer, bufferSize, getIdcLevel()));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"vui", NULL);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"sarAsInput", boolean2String(xmlBuffer, bufferSize, getSarAsInput()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"sarHeight", number2String(xmlBuffer, bufferSize, getSarHeight()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"sarWidth", number2String(xmlBuffer, bufferSize, getSarWidth()));

	switch (getOverscan())
	{
		case 0:
			strcpy((char*)xmlBuffer, "undefined");
			break;
		case 1:
			strcpy((char*)xmlBuffer, "show");
			break;
		case 2:
			strcpy((char*)xmlBuffer, "crop");
			break;
	}

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"overscan", xmlBuffer);

	switch (getVideoFormat())
	{
		case 0:
			strcpy((char*)xmlBuffer, "component");
			break;
		case 1:
			strcpy((char*)xmlBuffer, "pal");
			break;
		case 2:
			strcpy((char*)xmlBuffer, "ntsc");
			break;
		case 3:
			strcpy((char*)xmlBuffer, "secam");
			break;
		case 4:
			strcpy((char*)xmlBuffer, "mac");
			break;
		case 5:
			strcpy((char*)xmlBuffer, "undefined");
			break;
	}

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"videoFormat", xmlBuffer);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"fullRangeSamples", boolean2String(xmlBuffer, bufferSize, getFullRangeSamples()));

	switch (getColorPrimaries())
	{
		case 1:
			strcpy((char*)xmlBuffer, "bt709");
			break;
		case 2:
			strcpy((char*)xmlBuffer, "undefined");
			break;
		case 4:
			strcpy((char*)xmlBuffer, "bt470m");
			break;
		case 5:
			strcpy((char*)xmlBuffer, "bt470bg");
			break;
		case 6:
			strcpy((char*)xmlBuffer, "smpte170m");
			break;
		case 7:
			strcpy((char*)xmlBuffer, "smpte240m");
			break;
		case 8:
			strcpy((char*)xmlBuffer, "film");
			break;
	}

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"colorPrimaries", xmlBuffer);

	switch (getTransfer())
	{
		case 1:
			strcpy((char*)xmlBuffer, "bt709");
			break;
		case 2:
			strcpy((char*)xmlBuffer, "undefined");
			break;
		case 4:
			strcpy((char*)xmlBuffer, "bt470m");
			break;
		case 5:
			strcpy((char*)xmlBuffer, "bt470bg");
			break;
		case 6:
			strcpy((char*)xmlBuffer, "smpte170m");
			break;
		case 7:
			strcpy((char*)xmlBuffer, "smpte240m");
			break;
		case 8:
			strcpy((char*)xmlBuffer, "linear");
			break;
		case 9:
			strcpy((char*)xmlBuffer, "log100");
			break;
		case 10:
			strcpy((char*)xmlBuffer, "log316");
			break;
	}

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"transfer", xmlBuffer);

	switch (getColorMatrix())
	{
		case 0:
			strcpy((char*)xmlBuffer, "gbr");
			break;
		case 1:
			strcpy((char*)xmlBuffer, "bt709");
			break;
		case 2:
			strcpy((char*)xmlBuffer, "undefined");
			break;
		case 4:
			strcpy((char*)xmlBuffer, "fcc");
			break;
		case 5:
			strcpy((char*)xmlBuffer, "bt470bg");
			break;
		case 6:
			strcpy((char*)xmlBuffer, "smpte170m");
			break;
		case 7:
			strcpy((char*)xmlBuffer, "smpte240m");
			break;
		case 8:
			strcpy((char*)xmlBuffer, "ycgco");
			break;
	}

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"colorMatrix", xmlBuffer);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"chromaSampleLocation", number2String(xmlBuffer, bufferSize, getChromaSampleLocation()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"referenceFrames", number2String(xmlBuffer, bufferSize, getReferenceFrames()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"gopMaximumSize", number2String(xmlBuffer, bufferSize, getGopMaximumSize()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"gopMinimumSize", number2String(xmlBuffer, bufferSize, getGopMinimumSize()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"scenecutThreshold", number2String(xmlBuffer, bufferSize, getScenecutThreshold()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"bFrames", number2String(xmlBuffer, bufferSize, getBFrames()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"adaptiveBframeDecision", number2String(xmlBuffer, bufferSize, getAdaptiveBFrameDecision()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"bFrameBias", number2String(xmlBuffer, bufferSize, getBFrameBias()));

#if X264_BUILD >= 78
	switch (getBFrameReferences())
	{
		case 0:
			strcpy((char*)xmlBuffer, "none");
			break;
		case 1:
			strcpy((char*)xmlBuffer, "strict");
			break;
		case 2:
			strcpy((char*)xmlBuffer, "normal");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"bFrameReferences", xmlBuffer);
#else
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"bFrameReferences", boolean2String(xmlBuffer, bufferSize, (bool)getBFrameReferences()));
#endif


	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"loopFilter", boolean2String(xmlBuffer, bufferSize, getLoopFilter()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"loopFilterAlphaC0", number2String(xmlBuffer, bufferSize, getLoopFilterAlphaC0()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"loopFilterBeta", number2String(xmlBuffer, bufferSize, getLoopFilterBeta()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"cabac", boolean2String(xmlBuffer, bufferSize, getCabac()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"interlaced", boolean2String(xmlBuffer, bufferSize, getInterlaced()));
#if X264_BUILD >= 77
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"constrainedIntraPrediction", boolean2String(xmlBuffer, bufferSize, getConstrainedIntraPrediction()));
#endif

	switch (getCqmPreset())
	{
		case X264_CQM_FLAT:
			strcpy((char*)xmlBuffer, "flat");
			break;
		case X264_CQM_JVT:
			strcpy((char*)xmlBuffer, "jvt");
			break;
		case X264_CQM_CUSTOM:
			strcpy((char*)xmlBuffer, "custom");
			break;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"cqmPreset", xmlBuffer);

	uint8_t* intra4x4Luma = getIntra4x4Luma();
	uint8_t* intraChroma = getIntraChroma();
	uint8_t* inter4x4Luma = getInter4x4Luma();
	uint8_t* interChroma = getInterChroma();
	uint8_t* intra8x8Luma = getIntra8x8Luma();
	uint8_t* inter8x8Luma = getInter8x8Luma();

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"intra4x4Luma", NULL);

	for (int i = 0; i < 16; i++)
		xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"value", number2String(xmlBuffer, bufferSize, intra4x4Luma[i]));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"intraChroma", NULL);

	for (int i = 0; i < 16; i++)
		xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"value", number2String(xmlBuffer, bufferSize, intraChroma[i]));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"inter4x4Luma", NULL);

	for (int i = 0; i < 16; i++)
		xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"value", number2String(xmlBuffer, bufferSize, inter4x4Luma[i]));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"interChroma", NULL);

	for (int i = 0; i < 16; i++)
		xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"value", number2String(xmlBuffer, bufferSize, interChroma[i]));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"intra8x8Luma", NULL);

	for (int i = 0; i < 64; i++)
		xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"value", number2String(xmlBuffer, bufferSize, intra8x8Luma[i]));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"inter8x8Luma", NULL);

	for (int i = 0; i < 64; i++)
		xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"value", number2String(xmlBuffer, bufferSize, inter8x8Luma[i]));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"analyse", NULL);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"partitionI4x4", boolean2String(xmlBuffer, bufferSize, getPartitionI4x4()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"partitionI8x8", boolean2String(xmlBuffer, bufferSize, getPartitionI8x8()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"partitionP8x8", boolean2String(xmlBuffer, bufferSize, getPartitionP8x8()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"partitionP4x4", boolean2String(xmlBuffer, bufferSize, getPartitionP4x4()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"partitionB8x8", boolean2String(xmlBuffer, bufferSize, getPartitionB8x8()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"dct8x8", boolean2String(xmlBuffer, bufferSize, getDct8x8()));

#if X264_BUILD >= 79
	switch (getWeightedPredictionPFrames())
	{
		case X264_WEIGHTP_NONE:
			strcpy((char*)xmlBuffer, "none");
			break;
		case X264_WEIGHTP_BLIND:
			strcpy((char*)xmlBuffer, "blind");
			break;
		case X264_WEIGHTP_SMART:
			strcpy((char*)xmlBuffer, "smart");
			break;
	}

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"weightedPredictionPframes", xmlBuffer);
#endif

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"weightedPrediction", boolean2String(xmlBuffer, bufferSize, getWeightedPrediction()));

	switch (getDirectPredictionMode())
	{
		case X264_DIRECT_PRED_NONE:
			strcpy((char*)xmlBuffer, "none");
			break;
		case X264_DIRECT_PRED_SPATIAL:
			strcpy((char*)xmlBuffer, "spatial");
			break;
		case X264_DIRECT_PRED_TEMPORAL:
			strcpy((char*)xmlBuffer, "temporal");
			break;
		case X264_DIRECT_PRED_AUTO:
			strcpy((char*)xmlBuffer, "auto");
			break;
	}

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"directPredictionMode", xmlBuffer);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"chromaLumaQuantiserDifference", number2String(xmlBuffer, bufferSize, getChromaLumaQuantiserDifference()));

	switch (getMotionEstimationMethod())
	{
		case X264_ME_DIA:
			strcpy((char*)xmlBuffer, "diamond");
			break;
		case X264_ME_UMH:
			strcpy((char*)xmlBuffer, "multi-hexagonal");
			break;
		case X264_ME_ESA:
			strcpy((char*)xmlBuffer, "exhaustive");
			break;
		case X264_ME_TESA:
			strcpy((char*)xmlBuffer, "hadamard");
			break;
		case X264_ME_HEX:
		default:
			strcpy((char*)xmlBuffer, "hexagonal");
			break;
	}

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"motionEstimationMethod", xmlBuffer);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"motionVectorSearchRange", number2String(xmlBuffer, bufferSize, getMotionVectorSearchRange()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"motionVectorLength", number2String(xmlBuffer, bufferSize, getMotionVectorLength()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"motionVectorThreadBuffer", number2String(xmlBuffer, bufferSize, getMotionVectorThreadBuffer()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"subpixelRefinement", number2String(xmlBuffer, bufferSize, getSubpixelRefinement()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"chromaMotionEstimation", boolean2String(xmlBuffer, bufferSize, getChromaMotionEstimation()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"mixedReferences", boolean2String(xmlBuffer, bufferSize, getMixedReferences()));

	switch (getTrellis())
	{
		case 0:
			strcpy((char*)xmlBuffer, "disabled");
			break;
		case 1:
			strcpy((char*)xmlBuffer, "finalMacroblock");
			break;
		case 2:
			strcpy((char*)xmlBuffer, "allModeDecisions");
			break;
	}

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"trellis", xmlBuffer);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"fastPSkip", boolean2String(xmlBuffer, bufferSize, getFastPSkip()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"dctDecimate", boolean2String(xmlBuffer, bufferSize, getDctDecimate()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"psychoRdo", number2String(xmlBuffer, bufferSize, getPsychoRdo()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"noiseReduction", number2String(xmlBuffer, bufferSize, getNoiseReduction()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"interLumaDeadzone", number2String(xmlBuffer, bufferSize, getInterLumaDeadzone()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"intraLumaDeadzone", number2String(xmlBuffer, bufferSize, getIntraLumaDeadzone()));

	xmlNodeChild = xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"rateControl", NULL);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"quantiserMinimum", number2String(xmlBuffer, bufferSize, getQuantiserMinimum()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"quantiserMaximum", number2String(xmlBuffer, bufferSize, getQuantiserMaximum()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"quantiserStep", number2String(xmlBuffer, bufferSize, getQuantiserStep()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"averageBitrateTolerance", number2String(xmlBuffer, bufferSize, getAverageBitrateTolerance()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"vbvMaximumBitrate", number2String(xmlBuffer, bufferSize, getVbvMaximumBitrate()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"vbvBufferSize", number2String(xmlBuffer, bufferSize, getVbvBufferSize()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"vbvInitialOccupancy", number2String(xmlBuffer, bufferSize, getVbvInitialOccupancy()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"ipFrameQuantiser", number2String(xmlBuffer, bufferSize, getIpFrameQuantiser()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"pbFrameQuantiser", number2String(xmlBuffer, bufferSize, getPbFrameQuantiser()));

	switch (getAdaptiveQuantiserMode())
	{
		case X264_AQ_NONE:
			strcpy((char*)xmlBuffer, "none");
			break;
		case X264_AQ_VARIANCE:
			strcpy((char*)xmlBuffer, "variance");
			break;
	}

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"adaptiveQuantiserMode", xmlBuffer);
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"adaptiveQuantiserStrength", number2String(xmlBuffer, bufferSize, getAdaptiveQuantiserStrength()));

#if X264_BUILD >= 69
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"mbTree", boolean2String(xmlBuffer, bufferSize, getMbTree()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"frametypeLookahead", number2String(xmlBuffer, bufferSize, getFrametypeLookahead()));
#endif

	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"quantiserCurveCompression", number2String(xmlBuffer, bufferSize, getQuantiserCurveCompression()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"reduceFluxBeforeCurveCompression", number2String(xmlBuffer, bufferSize, getReduceFluxBeforeCurveCompression()));
	xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"reduceFluxAfterCurveCompression", number2String(xmlBuffer, bufferSize, getReduceFluxAfterCurveCompression()));

	int zoneCount = getZoneCount();

	if (zoneCount)
	{
		x264ZoneOptions** zoneOptions = getZones();

		for (int zoneIndex = 0; zoneIndex < zoneCount; zoneIndex++)
		{
			xmlNodeChild2 = xmlNewChild(xmlNodeChild, NULL, (xmlChar*)"zone", NULL);
			x264ZoneOptions* option = zoneOptions[zoneIndex];

			xmlNewChild(xmlNodeChild2, NULL, (xmlChar*)"frameStart", number2String(xmlBuffer, bufferSize, option->getFrameStart()));
			xmlNewChild(xmlNodeChild2, NULL, (xmlChar*)"frameEnd", number2String(xmlBuffer, bufferSize, option->getFrameEnd()));

			if (zoneOptions[zoneIndex]->getZoneMode() == ZONE_MODE_QUANTISER)
				xmlNewChild(xmlNodeChild2, NULL, (xmlChar*)"quantiser", number2String(xmlBuffer, bufferSize, option->getZoneParameter()));
			else
				xmlNewChild(xmlNodeChild2, NULL, (xmlChar*)"bitrateFactor", number2String(xmlBuffer, bufferSize, option->getZoneParameter() / 100.f));

			delete option;
		}

		delete [] zoneOptions;
	}

	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"accessUnitDelimiters", boolean2String(xmlBuffer, bufferSize, getAccessUnitDelimiters()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"spsIdentifier", number2String(xmlBuffer, bufferSize, getSpsIdentifier()));

#if X264_BUILD >= 73
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"sliceMaxSize", number2String(xmlBuffer, bufferSize, getSliceMaxSize()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"sliceMaxMacroblocks", number2String(xmlBuffer, bufferSize, getSliceMaxMacroblocks()));
	xmlNewChild(xmlNodeRoot, NULL, (xmlChar*)"sliceCount", number2String(xmlBuffer, bufferSize, getSliceCount()));
#endif
}

int x264Options::fromXml(const char *xml, PluginXmlType xmlType)
{
	clearZones();

	return PluginOptions::fromXml(xml, xmlType);
}

void x264Options::parseOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "threads") == 0)
				setThreads(atoi(content));
			else if (strcmp((char*)xmlChild->name, "deterministic") == 0)
				setDeterministic(string2Boolean(content));
#if X264_BUILD >= 75
			else if (strcmp((char*)xmlChild->name, "threadedLookahead") == 0)
				setThreadedLookahead(atoi(content));
#endif
			else if (strcmp((char*)xmlChild->name, "idcLevel") == 0)
				setIdcLevel(atoi(content));
			else if (strcmp((char*)xmlChild->name, "vui") == 0)
				parseVuiOptions(xmlChild);
			else if (strcmp((char*)xmlChild->name, "referenceFrames") == 0)
				setReferenceFrames(atoi(content));
			else if (strcmp((char*)xmlChild->name, "gopMaximumSize") == 0)
				setGopMaximumSize(atoi(content));
			else if (strcmp((char*)xmlChild->name, "gopMinimumSize") == 0)
				setGopMinimumSize(atoi(content));
			else if (strcmp((char*)xmlChild->name, "scenecutThreshold") == 0)
				setScenecutThreshold(atoi(content));
			else if (strcmp((char*)xmlChild->name, "bFrames") == 0)
				setBFrames(atoi(content));
			else if (strcmp((char*)xmlChild->name, "adaptiveBframeDecision") == 0)
				setAdaptiveBFrameDecision(atoi(content));
			else if (strcmp((char*)xmlChild->name, "bFrameBias") == 0)
				setBFrameBias(atoi(content));
			else if (strcmp((char*)xmlChild->name, "bFrameReferences") == 0)
			{
				int bFrameReferences = 0;

				if (strcmp(content, "strict") == 0)
					bFrameReferences = 1;
				else if (strcmp(content, "normal") == 0 || strcmp(content, "1") == 0 || strcmp(content, "true") == 0)
#if X264_BUILD >= 78
					bFrameReferences = 2;
#else
					bFrameReferences = 1;
#endif

				setBFrameReferences(bFrameReferences);
			}
			else if (strcmp((char*)xmlChild->name, "loopFilter") == 0)
				setLoopFilter(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "loopFilterAlphaC0") == 0)
				setLoopFilterAlphaC0(atoi(content));
			else if (strcmp((char*)xmlChild->name, "loopFilterBeta") == 0)
				setLoopFilterBeta(atoi(content));
			else if (strcmp((char*)xmlChild->name, "cabac") == 0)
				setCabac(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "interlaced") == 0)
				setInterlaced(string2Boolean(content));
#if X264_BUILD >= 77
			else if (strcmp((char*)xmlChild->name, "constrainedIntraPrediction") == 0)
				setConstrainedIntraPrediction(string2Boolean(content));
#endif
			else if (strcmp((char*)xmlChild->name, "cqmPreset") == 0)
			{
				int cqmPreset = 0;

				if (strcmp(content, "flat") == 0)
					cqmPreset = X264_CQM_FLAT;
				else if (strcmp(content, "jvt") == 0)
					cqmPreset = X264_CQM_JVT;
				else if (strcmp(content, "custom") == 0)
					cqmPreset = X264_CQM_CUSTOM;

				setCqmPreset(cqmPreset);
			}
			else if (strcmp((char*)xmlChild->name, "intra4x4Luma") == 0)
			{
				uint8_t intra4x4Luma[16];

				parseCqmOption(xmlChild, intra4x4Luma);
				setIntra4x4Luma(intra4x4Luma);
			}
			else if (strcmp((char*)xmlChild->name, "intraChroma") == 0)
			{
				uint8_t intraChroma[16];

				parseCqmOption(xmlChild, intraChroma);
				setIntraChroma(intraChroma);
			}
			else if (strcmp((char*)xmlChild->name, "inter4x4Luma") == 0)
			{
				uint8_t inter4x4Luma[16];

				parseCqmOption(xmlChild, inter4x4Luma);
				setInter4x4Luma(inter4x4Luma);
			}
			else if (strcmp((char*)xmlChild->name, "interChroma") == 0)
			{
				uint8_t interChroma[16];

				parseCqmOption(xmlChild, interChroma);
				setInterChroma(interChroma);
			}
			else if (strcmp((char*)xmlChild->name, "intra8x8Luma") == 0)
			{
				uint8_t intra8x8Luma[64];

				parseCqmOption(xmlChild, intra8x8Luma);
				setIntra8x8Luma(intra8x8Luma);
			}
			else if (strcmp((char*)xmlChild->name, "inter8x8Luma") == 0)
			{
				uint8_t inter8x8Luma[64];

				parseCqmOption(xmlChild, inter8x8Luma);
				setInter8x8Luma(inter8x8Luma);
			}
			else if (strcmp((char*)xmlChild->name, "analyse") == 0)
				parseAnalyseOptions(xmlChild);
			else if (strcmp((char*)xmlChild->name, "rateControl") == 0)
				parseRateControlOptions(xmlChild);
			else if (strcmp((char*)xmlChild->name, "accessUnitDelimiters") == 0)
				setAccessUnitDelimiters(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "spsIdentifier") == 0)
				setSpsIdentifier(atoi(content));
#if X264_BUILD >= 73
			else if (strcmp((char*)xmlChild->name, "sliceMaxSize") == 0)
				setSliceMaxSize(atoi(content));
			else if (strcmp((char*)xmlChild->name, "sliceMaxMacroblocks") == 0)
				setSliceMaxMacroblocks(atoi(content));
			else if (strcmp((char*)xmlChild->name, "sliceCount") == 0)
				setSliceCount(atoi(content));
#endif

			xmlFree(content);
		}
	}
}

void x264Options::parseVuiOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "sarAsInput") == 0)
				setSarAsInput(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "sarHeight") == 0)
				setSarHeight(atoi(content));
			else if (strcmp((char*)xmlChild->name, "sarWidth") == 0)
				setSarWidth(atoi(content));
			else if (strcmp((char*)xmlChild->name, "overscan") == 0)
			{
				int overscan = 0;

				if (strcmp(content, "undefined") == 0)
					overscan = 0;
				else if (strcmp(content, "show") == 0)
					overscan = 1;
				else if (strcmp(content, "crop") == 0)
					overscan = 2;

				setOverscan(overscan);
			}
			else if (strcmp((char*)xmlChild->name, "videoFormat") == 0)
			{
				int videoFormat = 5;

				if (strcmp(content, "component") == 0)
					videoFormat = 0;
				else if (strcmp(content, "pal") == 0)
					videoFormat = 1;
				else if (strcmp(content, "ntsc") == 0)
					videoFormat = 2;
				else if (strcmp(content, "secam") == 0)
					videoFormat = 3;
				else if (strcmp(content, "mac") == 0)
					videoFormat = 4;
				else if (strcmp(content, "undefined") == 0)
					videoFormat = 5;

				setVideoFormat(videoFormat);
			}
			else if (strcmp((char*)xmlChild->name, "fullRangeSamples") == 0)
				setFullRangeSamples(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "colorPrimaries") == 0)
			{
				int colorPrimaries = 2;

				if (strcmp(content, "bt709") == 0)
					colorPrimaries = 1;
				else if (strcmp(content, "undefined") == 0)
					colorPrimaries = 2;
				else if (strcmp(content, "bt470m") == 0)
					colorPrimaries = 4;
				else if (strcmp(content, "bt470bg") == 0)
					colorPrimaries = 5;
				else if (strcmp(content, "smpte170m") == 0)
					colorPrimaries = 6;
				else if (strcmp(content, "smpte240m") == 0)
					colorPrimaries = 7;
				else if (strcmp(content, "film") == 0)
					colorPrimaries = 8;

				setColorPrimaries(colorPrimaries);
			}
			else if (strcmp((char*)xmlChild->name, "transfer") == 0)
			{
				int transfer = 2;

				if (strcmp(content, "bt709") == 0)
					transfer = 1;
				else if (strcmp(content, "undefined") == 0)
					transfer = 2;
				else if (strcmp(content, "bt470m") == 0)
					transfer = 4;
				else if (strcmp(content, "bt470bg") == 0)
					transfer = 5;
				else if (strcmp(content, "smpte170m") == 0)
					transfer = 6;
				else if (strcmp(content, "smpte240m") == 0)
					transfer = 7;
				else if (strcmp(content, "linear") == 0)
					transfer = 8;
				else if (strcmp(content, "log100") == 0)
					transfer = 9;
				else if (strcmp(content, "log316") == 0)
					transfer = 8;

				setTransfer(transfer);
			}
			else if (strcmp((char*)xmlChild->name, "colorMatrix") == 0)
			{
				int colorMatrix = 2;

				if (strcmp(content, "gbr") == 0)
					colorMatrix = 0;
				else if (strcmp(content, "bt709") == 0)
					colorMatrix = 1;
				else if (strcmp(content, "undefined") == 0)
					colorMatrix = 2;
				else if (strcmp(content, "fcc") == 0)
					colorMatrix = 4;
				else if (strcmp(content, "bt470bg") == 0)
					colorMatrix = 5;
				else if (strcmp(content, "smpte170m") == 0)
					colorMatrix = 6;
				else if (strcmp(content, "smpte240m") == 0)
					colorMatrix = 7;
				else if (strcmp(content, "ycgco") == 0)
					colorMatrix = 8;

				setColorMatrix(colorMatrix);
			}
			else if (strcmp((char*)xmlChild->name, "chromaSampleLocation") == 0)
				setChromaSampleLocation(atoi(content));

			xmlFree(content);
		}
	}
}

void x264Options::parseCqmOption(xmlNode *node, uint8_t cqm[])
{
	int index = 0;

	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			cqm[index] = atoi(content);
			index++;

			xmlFree(content);
		}
	}
}

void x264Options::parseAnalyseOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "partitionI4x4") == 0)
				setPartitionI4x4(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "partitionI8x8") == 0)
				setPartitionI8x8(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "partitionP8x8") == 0)
				setPartitionP8x8(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "partitionP4x4") == 0)
				setPartitionP4x4(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "partitionB8x8") == 0)
				setPartitionB8x8(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "dct8x8") == 0)
				setDct8x8(string2Boolean(content));
#if X264_BUILD >= 79
			else if (strcmp((char*)xmlChild->name, "weightedPredictionPframes") == 0)
			{
				int weightedPredPFrames = X264_WEIGHTP_NONE;

				if (strcmp(content, "blind") == 0)
					weightedPredPFrames = X264_WEIGHTP_BLIND;
				else if (strcmp(content, "smart") == 0)
					weightedPredPFrames = X264_WEIGHTP_SMART;

				setWeightedPredictionPFrames(weightedPredPFrames);
			}
#endif
			else if (strcmp((char*)xmlChild->name, "weightedPrediction") == 0)
				setWeightedPrediction(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "directPredictionMode") == 0)
			{
				int directPredictionMode = X264_DIRECT_PRED_SPATIAL;

				if (strcmp(content, "none") == 0)
					directPredictionMode = X264_DIRECT_PRED_NONE;
				else if (strcmp(content, "spatial") == 0)
					directPredictionMode = X264_DIRECT_PRED_SPATIAL;
				else if (strcmp(content, "temporal") == 0)
					directPredictionMode = X264_DIRECT_PRED_TEMPORAL;
				else if (strcmp(content, "auto") == 0)
					directPredictionMode = X264_DIRECT_PRED_AUTO;

				setDirectPredictionMode(directPredictionMode);
			}
			else if (strcmp((char*)xmlChild->name, "chromaLumaQuantiserDifference") == 0)
				setChromaLumaQuantiserDifference(atoi(content));
			else if (strcmp((char*)xmlChild->name, "motionEstimationMethod") == 0)
			{
				int motionEstimationMethod = X264_ME_HEX;

				if (strcmp(content, "diamond") == 0)
					motionEstimationMethod = X264_ME_DIA;
				else if (strcmp(content, "hexagonal") == 0)
					motionEstimationMethod = X264_ME_HEX;
				else if (strcmp(content, "multi-hexagonal") == 0)
					motionEstimationMethod = X264_ME_UMH;
				else if (strcmp(content, "exhaustive") == 0)
					motionEstimationMethod = X264_ME_ESA;
				else if (strcmp(content, "hadamard") == 0)
					motionEstimationMethod = X264_ME_TESA;

				setMotionEstimationMethod(motionEstimationMethod);
			}
			else if (strcmp((char*)xmlChild->name, "motionVectorSearchRange") == 0)
				setMotionVectorSearchRange(atoi(content));
			else if (strcmp((char*)xmlChild->name, "motionVectorLength") == 0)
				setMotionVectorLength(atoi(content));
			else if (strcmp((char*)xmlChild->name, "motionVectorThreadBuffer") == 0)
				setMotionVectorThreadBuffer(atoi(content));
			else if (strcmp((char*)xmlChild->name, "subpixelRefinement") == 0)
				setSubpixelRefinement(atoi(content));
			else if (strcmp((char*)xmlChild->name, "chromaMotionEstimation") == 0)
				setChromaMotionEstimation(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "mixedReferences") == 0)
				setMixedReferences(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "trellis") == 0)
			{
				int trellis = 0;

				if (strcmp(content, "disabled") == 0)
					trellis = 0;
				else if (strcmp(content, "finalMacroblock") == 0)
					trellis = 1;
				else if (strcmp(content, "allModeDecisions") == 0)
					trellis = 2;

				setTrellis(trellis);
			}
			else if (strcmp((char*)xmlChild->name, "fastPSkip") == 0)
				setFastPSkip(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "dctDecimate") == 0)
				setDctDecimate(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "psychoRdo") == 0)
				setPsychoRdo(string2Float(content));
			else if (strcmp((char*)xmlChild->name, "noiseReduction") == 0)
				setNoiseReduction(atoi(content));
			else if (strcmp((char*)xmlChild->name, "interLumaDeadzone") == 0)
				setInterLumaDeadzone(atoi(content));
			else if (strcmp((char*)xmlChild->name, "intraLumaDeadzone") == 0)
				setIntraLumaDeadzone(atoi(content));

			xmlFree(content);
		}
	}
}

void x264Options::parseRateControlOptions(xmlNode *node)
{
	for (xmlNode *xmlChild = node->children; xmlChild; xmlChild = xmlChild->next)
	{
		if (xmlChild->type == XML_ELEMENT_NODE)
		{
			char *content = (char*)xmlNodeGetContent(xmlChild);

			if (strcmp((char*)xmlChild->name, "quantiserMinimum") == 0)
				setQuantiserMinimum(atoi(content));
			else if (strcmp((char*)xmlChild->name, "quantiserMaximum") == 0)
				setQuantiserMaximum(atoi(content));
			else if (strcmp((char*)xmlChild->name, "quantiserStep") == 0)
				setQuantiserStep(atoi(content));
			else if (strcmp((char*)xmlChild->name, "averageBitrateTolerance") == 0)
				setAverageBitrateTolerance(string2Float(content));
			else if (strcmp((char*)xmlChild->name, "vbvMaximumBitrate") == 0)
				setVbvMaximumBitrate(atoi(content));
			else if (strcmp((char*)xmlChild->name, "vbvBufferSize") == 0)
				setVbvBufferSize(atoi(content));
			else if (strcmp((char*)xmlChild->name, "vbvInitialOccupancy") == 0)
				setVbvInitialOccupancy(string2Float(content));
			else if (strcmp((char*)xmlChild->name, "ipFrameQuantiser") == 0)
				setIpFrameQuantiser(string2Float(content));
			else if (strcmp((char*)xmlChild->name, "pbFrameQuantiser") == 0)
				setPbFrameQuantiser(string2Float(content));
			else if (strcmp((char*)xmlChild->name, "adaptiveQuantiserMode") == 0)
			{
				int adaptiveQuantiserMode = X264_AQ_VARIANCE;

				if (strcmp(content, "none") == 0)
					adaptiveQuantiserMode = X264_AQ_NONE;
				else if (strcmp(content, "variance") == 0)
					adaptiveQuantiserMode = X264_AQ_VARIANCE;

				setAdaptiveQuantiserMode(adaptiveQuantiserMode);
			}
			else if (strcmp((char*)xmlChild->name, "adaptiveQuantiserStrength") == 0)
				setAdaptiveQuantiserStrength(string2Float(content));
#if X264_BUILD >= 69
			else if (strcmp((char*)xmlChild->name, "mbTree") == 0)
				setMbTree(string2Boolean(content));
			else if (strcmp((char*)xmlChild->name, "frametypeLookahead") == 0)
				setFrametypeLookahead(atoi(content));
#endif
			else if (strcmp((char*)xmlChild->name, "quantiserCurveCompression") == 0)
				setQuantiserCurveCompression(string2Float(content));
			else if (strcmp((char*)xmlChild->name, "reduceFluxBeforeCurveCompression") == 0)
				setReduceFluxBeforeCurveCompression(string2Float(content));
			else if (strcmp((char*)xmlChild->name, "reduceFluxAfterCurveCompression") == 0)
				setReduceFluxAfterCurveCompression(string2Float(content));
			else if (strcmp((char*)xmlChild->name, "zone") == 0)
				parseZoneOptions(xmlChild);
			else
				printf("%s\n", xmlChild->name);

			xmlFree(content);
		}
	}
}

void x264Options::parseZoneOptions(xmlNode *zoneNode)
{
	x264ZoneOptions option;

	for (xmlNode *xmlChild = zoneNode->children; xmlChild; xmlChild = xmlChild->next)
	{
		char *content = (char*)xmlNodeGetContent(xmlChild);

		if (strcmp((char*)xmlChild->name, "frameStart") == 0)
			option.setFrameRange(atoi(content), option.getFrameEnd());
		else if (strcmp((char*)xmlChild->name, "frameEnd") == 0)
			option.setFrameRange(option.getFrameStart(), atoi(content));
		else if (strcmp((char*)xmlChild->name, "quantiser") == 0)
			option.setQuantiser(atoi(content));
		else if (strcmp((char*)xmlChild->name, "bitrateFactor") == 0)
			option.setBitrateFactor((int)floor(string2Float(content) * 100 + .5));

		xmlFree(content);
	}

	addZone(&option);
}
