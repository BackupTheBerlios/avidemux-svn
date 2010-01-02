#ifndef ADM_ps_muxer_CONF_H
#define ADM_ps_muxer_CONF_H

#define PS_MUXER_DVD 0
#define PS_MUXER_VCD 1
#define PS_MUXER_SVCD 2

typedef struct {
	uint32_t muxingType;
	uint32_t acceptNonCompliant;
} ps_muxer;

const ps_muxer ps_muxer_default = {PS_MUXER_DVD, 0};

#endif
