// automatically generated by admSerialization.py, do not edit!
extern const ADM_paramList x264_encoder_param[]={
 {"general.params",offsetof(x264_encoder,general.params),"COMPRES_PARAMS",ADM_param_video_encode},
 {"general.threads",offsetof(x264_encoder,general.threads),"uint32_t",ADM_param_uint32_t},
 {"general.fast_first_pass",offsetof(x264_encoder,general.fast_first_pass),"bool",ADM_param_bool},
 {"level",offsetof(x264_encoder,level),"uint32_t",ADM_param_uint32_t},
 {"vui.sar_height",offsetof(x264_encoder,vui.sar_height),"uint32_t",ADM_param_uint32_t},
 {"vui.sar_width",offsetof(x264_encoder,vui.sar_width),"uint32_t",ADM_param_uint32_t},
 {"MaxRefFrames",offsetof(x264_encoder,MaxRefFrames),"uint32_t",ADM_param_uint32_t},
 {"MinIdr",offsetof(x264_encoder,MinIdr),"uint32_t",ADM_param_uint32_t},
 {"MaxIdr",offsetof(x264_encoder,MaxIdr),"uint32_t",ADM_param_uint32_t},
 {"i_scenecut_threshold",offsetof(x264_encoder,i_scenecut_threshold),"uint32_t",ADM_param_uint32_t},
 {"MaxBFrame",offsetof(x264_encoder,MaxBFrame),"uint32_t",ADM_param_uint32_t},
 {"i_bframe_adaptive",offsetof(x264_encoder,i_bframe_adaptive),"uint32_t",ADM_param_uint32_t},
 {"i_bframe_bias",offsetof(x264_encoder,i_bframe_bias),"uint32_t",ADM_param_uint32_t},
 {"i_bframe_pyramid",offsetof(x264_encoder,i_bframe_pyramid),"uint32_t",ADM_param_uint32_t},
 {"b_deblocking_filter",offsetof(x264_encoder,b_deblocking_filter),"bool",ADM_param_bool},
 {"i_deblocking_filter_alphac0",offsetof(x264_encoder,i_deblocking_filter_alphac0),"int32_t",ADM_param_int32_t},
 {"i_deblocking_filter_beta",offsetof(x264_encoder,i_deblocking_filter_beta),"int32_t",ADM_param_int32_t},
 {"cabac",offsetof(x264_encoder,cabac),"bool",ADM_param_bool},
 {"interlaced",offsetof(x264_encoder,interlaced),"bool",ADM_param_bool},
 {"analyze.b_8x8",offsetof(x264_encoder,analyze.b_8x8),"bool",ADM_param_bool},
 {"analyze.b_i4x4",offsetof(x264_encoder,analyze.b_i4x4),"bool",ADM_param_bool},
 {"analyze.b_i8x8",offsetof(x264_encoder,analyze.b_i8x8),"bool",ADM_param_bool},
 {"analyze.b_p8x8",offsetof(x264_encoder,analyze.b_p8x8),"bool",ADM_param_bool},
 {"analyze.b_p16x16",offsetof(x264_encoder,analyze.b_p16x16),"bool",ADM_param_bool},
 {"analyze.b_b16x16",offsetof(x264_encoder,analyze.b_b16x16),"bool",ADM_param_bool},
 {"analyze.weighted_pred",offsetof(x264_encoder,analyze.weighted_pred),"uint32_t",ADM_param_uint32_t},
 {"analyze.weighted_bipred",offsetof(x264_encoder,analyze.weighted_bipred),"bool",ADM_param_bool},
 {"analyze.direct_mv_pred",offsetof(x264_encoder,analyze.direct_mv_pred),"uint32_t",ADM_param_uint32_t},
 {"analyze.chroma_offset",offsetof(x264_encoder,analyze.chroma_offset),"uint32_t",ADM_param_uint32_t},
 {"analyze.me_method",offsetof(x264_encoder,analyze.me_method),"uint32_t",ADM_param_uint32_t},
 {"analyze.mv_range",offsetof(x264_encoder,analyze.mv_range),"uint32_t",ADM_param_uint32_t},
 {"analyze.subpel_refine",offsetof(x264_encoder,analyze.subpel_refine),"uint32_t",ADM_param_uint32_t},
 {"analyze.chroma_me",offsetof(x264_encoder,analyze.chroma_me),"bool",ADM_param_bool},
 {"analyze.mixed_references",offsetof(x264_encoder,analyze.mixed_references),"bool",ADM_param_bool},
 {"analyze.trellis",offsetof(x264_encoder,analyze.trellis),"uint32_t",ADM_param_uint32_t},
 {"analyze.psy_rd",offsetof(x264_encoder,analyze.psy_rd),"float",ADM_param_float},
 {"analyze.psy_trellis",offsetof(x264_encoder,analyze.psy_trellis),"float",ADM_param_float},
 {"analyze.fast_pskip",offsetof(x264_encoder,analyze.fast_pskip),"bool",ADM_param_bool},
 {"analyze.dct_decimate",offsetof(x264_encoder,analyze.dct_decimate),"bool",ADM_param_bool},
 {"analyze.noise_reduction",offsetof(x264_encoder,analyze.noise_reduction),"uint32_t",ADM_param_uint32_t},
 {"analyze.psy",offsetof(x264_encoder,analyze.psy),"bool",ADM_param_bool},
 {"analyze.intra_luma",offsetof(x264_encoder,analyze.intra_luma),"uint32_t",ADM_param_uint32_t},
 {"analyze.inter_luma",offsetof(x264_encoder,analyze.inter_luma),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.rc_method",offsetof(x264_encoder,ratecontrol.rc_method),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.qp_constant",offsetof(x264_encoder,ratecontrol.qp_constant),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.qp_min",offsetof(x264_encoder,ratecontrol.qp_min),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.qp_max",offsetof(x264_encoder,ratecontrol.qp_max),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.qp_step",offsetof(x264_encoder,ratecontrol.qp_step),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.bitrate",offsetof(x264_encoder,ratecontrol.bitrate),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.vbv_max_bitrate",offsetof(x264_encoder,ratecontrol.vbv_max_bitrate),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.vbv_buffer_size",offsetof(x264_encoder,ratecontrol.vbv_buffer_size),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.vbv_buffer_init",offsetof(x264_encoder,ratecontrol.vbv_buffer_init),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.ip_factor",offsetof(x264_encoder,ratecontrol.ip_factor),"float",ADM_param_float},
 {"ratecontrol.pb_factor",offsetof(x264_encoder,ratecontrol.pb_factor),"float",ADM_param_float},
 {"ratecontrol.aq_mode",offsetof(x264_encoder,ratecontrol.aq_mode),"uint32_t",ADM_param_uint32_t},
 {"ratecontrol.aq_strength",offsetof(x264_encoder,ratecontrol.aq_strength),"float",ADM_param_float},
 {"ratecontrol.mb_tree",offsetof(x264_encoder,ratecontrol.mb_tree),"bool",ADM_param_bool},
 {"ratecontrol.lookahead",offsetof(x264_encoder,ratecontrol.lookahead),"uint32_t",ADM_param_uint32_t},
{NULL,0,NULL}
};
