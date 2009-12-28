//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: Mean (fixounet@free.fr)
//
// Copyright: See COPYING file that comes with this distribution GPL.
//
// FIXME : We need total frame # to do only pass2
//
#ifndef ADM_RTCL
#define ADM_RTCL

#include <inttypes.h>

typedef enum 
{
	RF_I=1,
	RF_P=2,
	RF_B=3,
} ADM_rframe;

typedef enum
{
	RS_IDLE,
	RS_PASS1,
	RS_PASS2
} ADM_rstate;

typedef struct
{
	uint32_t quant;
	uint32_t size;
	ADM_rframe type;
} ADM_pass_stat;

class ADM_ratecontrol
{
protected:
	uint32_t _nbFrames;
	uint32_t _fps1000;
	char 	*_logname;
	ADM_rstate _state;

public:
	ADM_ratecontrol(uint32_t fps1000, char *logname);
	virtual ~ADM_ratecontrol();
	/** Maxbr & minbr in Bps, vbvsize in kBytes); Default is none */
	virtual uint8_t setVBVInfo(uint32_t maxbr, uint32_t minbr, uint32_t vbvsize) = 0;
	virtual	uint8_t startPass1(void) = 0;
	virtual	uint8_t logPass1(uint32_t qz, ADM_rframe ftype, uint32_t size) = 0;
	virtual	uint8_t startPass2(uint32_t size, uint32_t nbFrame) = 0;
	virtual	uint8_t getQz(uint32_t *qz, ADM_rframe *type) = 0;
	virtual	uint8_t logPass2(uint32_t qz, ADM_rframe ftype, uint32_t size) = 0;
};

#endif
//EOF
