// automatically generated by admSerialization.py, do not edit!
#include "ADM_default.h"
#include "ADM_paramList.h"
#include "ADM_coreJson.h"
#include "x264_encoder.h"
bool  x264_encoder_jserialize(const char *file, const x264_encoder *key){
admJson json;
json.addNode("general");
json.addCompressParam("params",key->general.params);
json.addUint32("threads",key->general.threads);
json.addBool("fast_first_pass",key->general.fast_first_pass);
json.endNode();
json.addUint32("level",key->level);
json.addNode("vui");
json.addUint32("sar_height",key->vui.sar_height);
json.addUint32("sar_width",key->vui.sar_width);
json.endNode();
json.addUint32("MaxRefFrames",key->MaxRefFrames);
json.addUint32("MinIdr",key->MinIdr);
json.addUint32("MaxIdr",key->MaxIdr);
json.addUint32("MaxBFrame",key->MaxBFrame);
json.addUint32("i_bframe_adaptative",key->i_bframe_adaptative);
json.addUint32("i_bframe_bias",key->i_bframe_bias);
json.addUint32("i_bframe_pyramid",key->i_bframe_pyramid);
json.addBool("b_deblocking_filter",key->b_deblocking_filter);
json.addInt32("i_deblocking_filter_alphac0",key->i_deblocking_filter_alphac0);
json.addInt32("i_deblocking_filter_beta",key->i_deblocking_filter_beta);
json.addBool("cabac",key->cabac);
json.addBool("interlaced",key->interlaced);
json.addNode("analyze");
json.addBool("b_8x8",key->analyze.b_8x8);
json.addBool("b_i4x4",key->analyze.b_i4x4);
json.addBool("b_i8x8",key->analyze.b_i8x8);
json.addBool("b_p8x8",key->analyze.b_p8x8);
json.addBool("b_p16x16",key->analyze.b_p16x16);
json.addBool("b_b16x16",key->analyze.b_b16x16);
json.addUint32("weighted_pred",key->analyze.weighted_pred);
json.addBool("weighted_bipred",key->analyze.weighted_bipred);
json.addUint32("direct_mv_pred",key->analyze.direct_mv_pred);
json.addUint32("chroma_offset",key->analyze.chroma_offset);
json.addUint32("me_method",key->analyze.me_method);
json.addUint32("subpel_refine",key->analyze.subpel_refine);
json.addBool("chroma_me",key->analyze.chroma_me);
json.addBool("mixed_references",key->analyze.mixed_references);
json.addUint32("trellis",key->analyze.trellis);
json.addBool("fast_pskip",key->analyze.fast_pskip);
json.addBool("dct_decimate",key->analyze.dct_decimate);
json.addUint32("noise_reduction",key->analyze.noise_reduction);
json.addBool("psy",key->analyze.psy);
json.endNode();
json.addNode("ratecontrol");
json.addUint32("rc_method",key->ratecontrol.rc_method);
json.addUint32("qp_constant",key->ratecontrol.qp_constant);
json.addUint32("qp_min",key->ratecontrol.qp_min);
json.addUint32("qp_max",key->ratecontrol.qp_max);
json.addUint32("qp_step",key->ratecontrol.qp_step);
json.addUint32("bitrate",key->ratecontrol.bitrate);
json.addUint32("vbv_max_bitrate",key->ratecontrol.vbv_max_bitrate);
json.addUint32("vbv_buffer_size",key->ratecontrol.vbv_buffer_size);
json.addUint32("vbv_buffer_init",key->ratecontrol.vbv_buffer_init);
json.addFloat("ip_factor",key->ratecontrol.ip_factor);
json.addFloat("pb_factor",key->ratecontrol.pb_factor);
json.addUint32("aq_mode",key->ratecontrol.aq_mode);
json.addFloat("aq_strength",key->ratecontrol.aq_strength);
json.addBool("mb_tree",key->ratecontrol.mb_tree);
json.addUint32("lookahead",key->ratecontrol.lookahead);
json.endNode();
return json.dumpToFile(file);
};
bool  x264_encoder_jdeserialize(const char *file, const ADM_paramList *tmpl,x264_encoder *key){
admJsonToCouple json;
CONFcouple *c=json.readFromFile(file);
if(!c) {ADM_error("Cannot read json file");return false;}
bool r= ADM_paramLoadPartial(c,tmpl,key);
delete c;
return r;
};
