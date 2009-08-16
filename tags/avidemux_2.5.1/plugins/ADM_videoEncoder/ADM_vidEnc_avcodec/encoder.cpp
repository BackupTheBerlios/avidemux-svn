/***************************************************************************
                                encoder.cpp

    begin                : Thu Apr 10 2009
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
#include <libxml/tree.h>

#include "encoder.h"
#include "dvEncoder.h"
#include "ffv1Encoder.h"
#include "ffvhuffEncoder.h"
#include "huffyuvEncoder.h"
#include "mpeg1Encoder.h"
#include "ADM_inttype.h"

int uiType;

static DVEncoder dv;
static FFV1Encoder ffv1;
static FFVHuffEncoder ffvhuff;
static HuffyuvEncoder huffyuv;
static Mpeg1Encoder mpeg1Encoder;

static int encoderIds[] = { 0, 1, 2, 3, 4 };
static AvcodecEncoder* encoders[] = { &dv, &ffv1, &ffvhuff, &huffyuv, &mpeg1Encoder};

extern "C"
{
	void *avcodecEncoder_getPointers(int _uiType, int *count)
	{
		uiType = _uiType;
		*count = sizeof(encoderIds) / sizeof(int);

		return &encoderIds;
	}

	const char* avcodecEncoder_getEncoderName(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->getEncoderName();
	}

	const char* avcodecEncoder_getEncoderType(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->getEncoderType();
	}

	const char* avcodecEncoder_getEncoderDescription(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->getEncoderDescription();
	}

	const char* avcodecEncoder_getFourCC(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->getFourCC();
	}

	int avcodecEncoder_getEncoderRequirements(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->getEncoderRequirements();
	}

	const char* avcodecEncoder_getEncoderGuid(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->getEncoderGuid();
	}

	int avcodecEncoder_isConfigurable(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->isConfigurable();
	}

	int avcodecEncoder_configure(int encoderId, vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->configure(configParameters, properties);
	}

	int avcodecEncoder_getOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->getOptions(encodeOptions, pluginOptions, bufferSize);
	}

	int avcodecEncoder_setOptions(int encoderId, vidEncOptions *encodeOptions, char *pluginOptions)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->setOptions(encodeOptions, pluginOptions);
	}

	int avcodecEncoder_getPassCount(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->getPassCount();
	}

	int avcodecEncoder_getCurrentPass(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->getCurrentPass();
	}

	int avcodecEncoder_open(int encoderId, vidEncVideoProperties *properties)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->open(properties);
	}

	int avcodecEncoder_beginPass(int encoderId, vidEncPassParameters *passParameters)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->beginPass(passParameters);
	}

	int avcodecEncoder_encodeFrame(int encoderId, vidEncEncodeParameters *encodeParams)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->encodeFrame(encodeParams);
	}

	int avcodecEncoder_finishPass(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->finishPass();
	}

	int avcodecEncoder_close(int encoderId)
	{
		AvcodecEncoder *encoder = encoders[encoderId];
		return encoder->close();
	}
}

void AvcodecEncoder::init(enum CodecID id, int targetColourSpace)
{
	_codecId = id;
	_opened = false;

	_passCount = 1;
	_currentPass = 0;
	_openPass = false;
	_context = NULL;

	_supportedCsps[0] = targetColourSpace;
}

int AvcodecEncoder::initContext(const char* logFileName)
{
	_context->width = _width;
	_context->height = _height;
	_context->time_base = (AVRational){_fpsDen, _fpsNum};
	_context->pix_fmt = getAvCodecColourSpace(_supportedCsps[0]);

	return ADM_VIDENC_ERR_SUCCESS;
}

AVCodec *AvcodecEncoder::getAvCodec(void)
{
	return avcodec_find_encoder(_codecId);
}

enum PixelFormat AvcodecEncoder::getAvCodecColourSpace(int colourSpace)
{
	enum PixelFormat pixFmt = PIX_FMT_NONE;

	switch (colourSpace)
	{
		case ADM_CSP_YV12:
			pixFmt = PIX_FMT_YUV420P;
			break;
		case ADM_CSP_I422:
			pixFmt = PIX_FMT_YUV422P;
			break;
	}

	return pixFmt;
}

int AvcodecEncoder::getFrameType(void)
{
	if (_context->coded_frame->key_frame == 1)
		return ADM_VIDENC_FRAMETYPE_IDR;
	else if (_context->coded_frame->pict_type == FF_B_TYPE)
		return ADM_VIDENC_FRAMETYPE_B;

	return ADM_VIDENC_FRAMETYPE_P;
}

void AvcodecEncoder::updateEncodeParameters(vidEncEncodeParameters *encodeParams, uint8_t *buffer, int bufferSize)
{
	encodeParams->frameType = getFrameType();
	encodeParams->ptsFrame = _context->coded_frame->display_picture_number;
	encodeParams->encodedDataSize = bufferSize;
	encodeParams->encodedData = buffer;

	if (_context->coded_frame->quality)
		encodeParams->quantiser =(int)floor(_context->coded_frame->quality / (float)FF_QP2LAMBDA);
	else
		encodeParams->quantiser = (int)floor(_frame.quality / (float)FF_QP2LAMBDA);
}

AvcodecEncoder::~AvcodecEncoder(void)
{
	close();
}

const char* AvcodecEncoder::getEncoderName(void)
{
	return "avcodec";
}

int AvcodecEncoder::getEncoderRequirements(void)
{
	AVCodec *codec = getAvCodec();

	return (codec && (codec->capabilities & CODEC_CAP_DELAY) ? ADM_VIDENC_REQ_NULL_FLUSH : ADM_VIDENC_REQ_NONE);
}

int AvcodecEncoder::isConfigurable(void)
{
	return 0;
}

int AvcodecEncoder::configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	return 0;
}

int AvcodecEncoder::getCurrentPass(void)
{
	return _currentPass;
}

int AvcodecEncoder::getPassCount(void)
{
	return _passCount;
}

int AvcodecEncoder::open(vidEncVideoProperties *properties)
{
	if (_opened)
		return ADM_VIDENC_ERR_ALREADY_OPEN;

	_opened = true;
	_currentPass = 0;

	properties->supportedCspsCount = 1;
	properties->supportedCsps = &_supportedCsps[0];

	_width = properties->width;
	_height = properties->height;

	_fpsNum = properties->fpsNum;
	_fpsDen = properties->fpsDen;

	_frameCount = properties->frameCount;

	return ADM_VIDENC_ERR_SUCCESS;
}

int AvcodecEncoder::beginPass(vidEncPassParameters *passParameters)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	if (_openPass)
		return ADM_VIDENC_ERR_PASS_ALREADY_OPEN;

	if (_currentPass == _passCount)
		return ADM_VIDENC_ERR_PASS_COUNT_REACHED;

	if (_passCount > 1 && _currentPass == 0 && passParameters->useExistingLogFile)
	{
		_currentPass++;
		return ADM_VIDENC_ERR_PASS_SKIP;
	}

	_openPass = true;
	_currentPass++;
	_context = avcodec_alloc_context();

	if (!_context)
		return ADM_VIDENC_ERR_FAILED;

	memset(&_frame, 0, sizeof(_frame));
	_frame.pts = AV_NOPTS_VALUE;

	int ret = initContext(passParameters->logFileName);

	if (ret != ADM_VIDENC_ERR_SUCCESS)
		return ret;

	AVCodec *codec = getAvCodec();

	if (!codec)
		return ADM_VIDENC_ERR_FAILED;

	if (avcodec_open(_context, codec) < 0)
	{
		//this->printContext();
		finishPass();

		return ADM_VIDENC_ERR_FAILED;
	}

	//this->printContext();

	AVPicture encodedPicture;

	_bufferSize = avpicture_fill(&encodedPicture, NULL, _context->pix_fmt, _context->width, _context->height);
	_buffer = new uint8_t[_bufferSize];

	passParameters->extraData = _context->extradata;
	passParameters->extraDataSize = _context->extradata_size;

	return ADM_VIDENC_ERR_SUCCESS;
}

int AvcodecEncoder::encodeFrame(vidEncEncodeParameters *encodeParams)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	_frame.key_frame = 0;
	_frame.pict_type = 0;

	if (encodeParams->frameData[0])
	{
		if (_supportedCsps[0] == ADM_CSP_YV12)
		{
			// Swap planes so YV12 looks like YUV420P
			uint8_t *tmpPlane = encodeParams->frameData[1];

			encodeParams->frameData[1] = encodeParams->frameData[2];
			encodeParams->frameData[2] = tmpPlane;
		}

		_frame.data[0] = encodeParams->frameData[0];
		_frame.data[1] = encodeParams->frameData[1];
		_frame.data[2] = encodeParams->frameData[2];
		_frame.linesize[0] = encodeParams->frameLineSize[0];
		_frame.linesize[1] = encodeParams->frameLineSize[1];
		_frame.linesize[2] = encodeParams->frameLineSize[2];

		int size = avcodec_encode_video(_context, _buffer, _bufferSize, &_frame);

		if (size < 0)
			return ADM_VIDENC_ERR_FAILED;

		updateEncodeParameters(encodeParams, _buffer, size);
	}

	return ADM_VIDENC_ERR_SUCCESS;
}

int AvcodecEncoder::finishPass(void)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	if (_openPass)
		_openPass = false;

	if (_context)
	{
		avcodec_close(_context);
		_context = NULL;
	}

	if (_buffer)
	{
		delete [] _buffer;
		_buffer = NULL;
	}

	return ADM_VIDENC_ERR_SUCCESS;
}

int AvcodecEncoder::close(void)
{
	if (_openPass)
		finishPass();

	_opened = false;
	_currentPass = 0;

	return ADM_VIDENC_ERR_SUCCESS;
}

void AvcodecEncoder::printContext(void)
{
	printf("bit_rate: %d\n", _context->bit_rate);
	printf("bit_rate_tolerance: %d\n", _context->bit_rate_tolerance);
	printf("flags: %d\n", _context->flags);
	printf("sub_id: %d\n", _context->sub_id);
	printf("me_method: %d\n", _context->me_method);
	printf("extradata_size: %d\n", _context->extradata_size);
	printf("time_base %d, %d\n", _context->time_base.num, _context->time_base.den);
	printf("width: %d\n", _context->width);
	printf("height: %d\n", _context->height);
	printf("gop_size: %d\n", _context->gop_size);
	printf("pix_fmt: %d\n", _context->pix_fmt);
	printf("rate_emu: %d\n", _context->rate_emu);
	printf("frame_size: %d\n", _context->frame_size);
	printf("frame_number: %d\n", _context->frame_number);
	printf("delay: %d\n", _context->delay);
	printf("qcompress: %f\n", _context->qcompress);
	printf("qblur: %f\n", _context->qblur);
	printf("qmin: %d\n", _context->qmin);
	printf("qmax: %d\n", _context->qmax);
	printf("max_qdiff: %d\n", _context->max_qdiff);
	printf("max_b_frames: %d\n", _context->max_b_frames);
	printf("b_quant_factor: %f\n", _context->b_quant_factor);
	printf("rc_strategy: %d\n", _context->rc_strategy);
	printf("b_frame_strategy: %d\n", _context->b_frame_strategy);
	printf("hurry_up: %d\n", _context->hurry_up);
	printf("rtp_payload_size: %d\n", _context->rtp_payload_size);
	printf("mv_bits: %d\n", _context->mv_bits);
	printf("header_bits: %d\n", _context->header_bits);
	printf("i_tex_bits: %d\n", _context->i_tex_bits);
	printf("p_tex_bits: %d\n", _context->p_tex_bits);
	printf("i_count: %d\n", _context->i_count);
	printf("p_count: %d\n", _context->p_count);
	printf("skip_count: %d\n", _context->skip_count);
	printf("misc_bits: %d\n", _context->misc_bits);
	printf("frame_bits: %d\n", _context->frame_bits);
	printf("codec_name: %s\n", _context->codec_name);
	printf("codec_type: %d\n", _context->codec_type);
	printf("codec_id: %d\n", _context->codec_id);
	printf("codec_tag: %d\n", _context->codec_tag);
	printf("workaround_bugs: %d\n", _context->workaround_bugs);
	printf("luma_elim_threshold: %d\n", _context->luma_elim_threshold);
	printf("chroma_elim_threshold: %d\n", _context->chroma_elim_threshold);
	printf("strict_std_compliance: %d\n", _context->strict_std_compliance);
	printf("b_quant_offset: %f\n", _context->b_quant_offset);
	printf("error_recognition: %d\n", _context->error_recognition);
	printf("has_b_frames: %d\n", _context->has_b_frames);
	printf("block_align: %d\n", _context->block_align);
	printf("parse_only: %d\n", _context->parse_only);
	printf("mpeg_quant: %d\n", _context->mpeg_quant);
	printf("rc_qsquish: %f\n", _context->rc_qsquish);
	printf("rc_qmod_amp: %f\n", _context->rc_qmod_amp);
	printf("rc_qmod_freq: %d\n", _context->rc_qmod_freq);
	printf("rc_override_count: %d\n", _context->rc_override_count);
	printf("rc_eq: %s\n", _context->rc_eq);
	printf("rc_max_rate: %d\n", _context->rc_max_rate);
	printf("rc_min_rate: %d\n", _context->rc_min_rate);
	printf("rc_max_rate_header: %d\n", _context->rc_max_rate_header);
	printf("rc_buffer_size: %d\n", _context->rc_buffer_size);
	printf("rc_buffer_size_header: %d\n", _context->rc_buffer_size_header);
	printf("rc_buffer_aggressivity: %f\n", _context->rc_buffer_aggressivity);
	printf("i_quant_factor: %f\n", _context->i_quant_factor);
	printf("i_quant_offset: %f\n", _context->i_quant_offset);
	printf("rc_initial_cplx: %f\n", _context->rc_initial_cplx);
	printf("dct_algo: %d\n", _context->dct_algo);
	printf("lumi_masking: %f\n", _context->lumi_masking);
	printf("temporal_cplx_masking: %f\n", _context->temporal_cplx_masking);
	printf("spatial_cplx_masking: %f\n", _context->spatial_cplx_masking);
	printf("p_masking: %f\n", _context->p_masking);
	printf("dark_masking: %f\n", _context->dark_masking);
	printf("idct_algo: %d\n", _context->idct_algo);
	printf("slice_count: %d\n", _context->slice_count);
	printf("*slice_offset: %d\n", _context->slice_offset);
	printf("error_concealment: %d\n", _context->error_concealment);
	printf("dsp_mask: %d\n", _context->dsp_mask);
	printf("bits_per_coded_sample: %d\n", _context->bits_per_coded_sample);
	printf("prediction_method: %d\n", _context->prediction_method);
	printf("sample_aspect_ratio: %d, %d\n", _context->sample_aspect_ratio.num, _context->sample_aspect_ratio.den);
	printf("debug: %d\n", _context->debug);
	printf("debug_mv: %d\n", _context->debug_mv);
	printf("mb_qmin: %d\n", _context->mb_qmin);
	printf("mb_qmax: %d\n", _context->mb_qmax);
	printf("me_cmp: %d\n", _context->me_cmp);
	printf("me_sub_cmp: %d\n", _context->me_sub_cmp);
	printf("mb_cmp: %d\n", _context->mb_cmp);
	printf("ildct_cmp: %d\n", _context->ildct_cmp);
	printf("dia_size: %d\n", _context->dia_size);
	printf("last_predictor_count: %d\n", _context->last_predictor_count);
	printf("pre_me: %d\n", _context->pre_me);
	printf("me_pre_cmp: %d\n", _context->me_pre_cmp);
	printf("pre_dia_size: %d\n", _context->pre_dia_size);
	printf("me_subpel_quality: %d\n", _context->me_subpel_quality);
	printf("dtg_active_format: %d\n", _context->dtg_active_format);
	printf("me_range: %d\n", _context->me_range);
	printf("intra_quant_bias: %d\n", _context->intra_quant_bias);
	printf("inter_quant_bias: %d\n", _context->inter_quant_bias);
	printf("color_table_id: %d\n", _context->color_table_id);
	printf("internal_buffer_count: %d\n", _context->internal_buffer_count);
	printf("global_quality: %d\n", _context->global_quality);
	printf("coder_type: %d\n", _context->coder_type);
	printf("context_model: %d\n", _context->context_model);
	printf("slice_flags: %d\n", _context->slice_flags);
	printf("xvmc_acceleration: %d\n", _context->xvmc_acceleration);
	printf("mb_decision: %d\n", _context->mb_decision);
	printf("stream_codec_tag: %d\n", _context->stream_codec_tag);
	printf("scenechange_threshold: %d\n", _context->scenechange_threshold);
	printf("lmin: %d\n", _context->lmin);
	printf("lmax: %d\n", _context->lmax);
	printf("noise_reduction: %d\n", _context->noise_reduction);
	printf("rc_initial_buffer_occupancy: %d\n", _context->rc_initial_buffer_occupancy);
	printf("inter_threshold: %d\n", _context->inter_threshold);
	printf("flags2: %d\n", _context->flags2);
	printf("error_rate: %d\n", _context->error_rate);
	printf("antialias_algo: %d\n", _context->antialias_algo);
	printf("quantizer_noise_shaping: %d\n", _context->quantizer_noise_shaping);
	printf("thread_count: %d\n", _context->thread_count);
	printf("me_threshold: %d\n", _context->me_threshold);
	printf("mb_threshold: %d\n", _context->mb_threshold);
	printf("intra_dc_precision: %d\n", _context->intra_dc_precision);
	printf("nsse_weight: %d\n", _context->nsse_weight);
	printf("skip_top: %d\n", _context->skip_top);
	printf("skip_bottom: %d\n", _context->skip_bottom);
	printf("profile: %d\n", _context->profile);
	printf("level: %d\n", _context->level);
	printf("lowres: %d\n", _context->lowres);
	printf("coded_width: %d\n", _context->coded_width);
	printf("coded_height: %d\n", _context->coded_height);
	printf("frame_skip_threshold: %d\n", _context->frame_skip_threshold);
	printf("frame_skip_factor: %d\n", _context->frame_skip_factor);
	printf("frame_skip_exp: %d\n", _context->frame_skip_exp);
	printf("frame_skip_cmp: %d\n", _context->frame_skip_cmp);
	printf("border_masking: %f\n", _context->border_masking);
	printf("mb_lmin: %d\n", _context->mb_lmin);
	printf("mb_lmax: %d\n", _context->mb_lmax);
	printf("me_penalty_compensation: %d\n", _context->me_penalty_compensation);
	printf("skip_loop_filter: %d\n", _context->skip_loop_filter);
	printf("skip_idct: %d\n", _context->skip_idct);
	printf("skip_frame: %d\n", _context->skip_frame);
	printf("bidir_refine: %d\n", _context->bidir_refine);
	printf("brd_scale: %d\n", _context->brd_scale);
	printf("crf: %f\n", _context->crf);
	printf("cqp: %d\n", _context->cqp);
	printf("keyint_min: %d\n", _context->keyint_min);
	printf("refs: %d\n", _context->refs);
	printf("chromaoffset: %d\n", _context->chromaoffset);
	printf("bframebias: %d\n", _context->bframebias);
	printf("trellis: %d\n", _context->trellis);
	printf("complexityblur: %f\n", _context->complexityblur);
	printf("deblockalpha: %d\n", _context->deblockalpha);
	printf("deblockbeta: %d\n", _context->deblockbeta);
	printf("partitions: %d\n", _context->partitions);
	printf("directpred: %d\n", _context->directpred);
	printf("cutoff: %d\n", _context->cutoff);
	printf("scenechange_factor: %d\n", _context->scenechange_factor);
	printf("mv0_threshold: %d\n", _context->mv0_threshold);
	printf("b_sensitivity: %d\n", _context->b_sensitivity);
	printf("compression_level: %d\n", _context->compression_level);
	printf("use_lpc: %d\n", _context->use_lpc);
	printf("lpc_coeff_precision: %d\n", _context->lpc_coeff_precision);
	printf("min_prediction_order: %d\n", _context->min_prediction_order);
	printf("max_prediction_order: %d\n", _context->max_prediction_order);
	printf("prediction_order_method: %d\n", _context->prediction_order_method);
	printf("min_partition_order: %d\n", _context->min_partition_order);
	printf("max_partition_order: %d\n", _context->max_partition_order);
	printf("timecode_frame_start: %"LLU"\n", _context->timecode_frame_start);
	printf("drc_scale: %f\n", _context->drc_scale);
	printf("reordered_opaque: %"LLU"\n", _context->reordered_opaque);
	printf("bits_per_raw_sample: %d\n", _context->bits_per_raw_sample);
	printf("rc_max_available_vbv_use: %f\n", _context->rc_max_available_vbv_use);
	printf("rc_min_vbv_overflow_use: %f\n", _context->rc_min_vbv_overflow_use);
	printf("ticks_per_frame: %d\n", _context->ticks_per_frame);
	printf("color_primaries: %d\n", _context->color_primaries);
	printf("color_trc: %d\n", _context->color_trc);
	printf("colorspace: %d\n", _context->colorspace);
	printf("color_range: %d\n", _context->color_range);
	printf("chroma_sample_location: %d\n", _context->chroma_sample_location);
}
