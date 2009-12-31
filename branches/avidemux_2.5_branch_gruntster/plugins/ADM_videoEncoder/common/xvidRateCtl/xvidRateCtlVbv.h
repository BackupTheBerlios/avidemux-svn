#ifndef XVIDRATECTLVBV_H
#define XVIDRATECTLVBV_H

#include <inttypes.h>
#include "rateControl.h"
#include "xvidRateCtl.h"

#define AVG_LOOKUP 5
class ADM_newXvidRcVBV : public ADM_ratecontrol
{
protected:
	ADM_newXvidRc	*rc;
	uint32_t	_minbr,_maxbr,_vbvsize;
	ADM_pass_stat  *_stat;
	uint32_t	*_lastSize;
	uint32_t	_roundup;
	uint32_t	_frame;
	uint32_t	_vbv_fullness;
	uint32_t	_byte_per_image;
	double		_compr[3][AVG_LOOKUP];  
	uint32_t   _idxI,_idxP,_idxB;

	uint8_t 	project(uint32_t framenum, uint32_t q, ADM_rframe frame);
	uint8_t 	checkVBV(uint32_t framenum, uint32_t q, ADM_rframe frame);
	float 		getRatio(uint32_t newq, uint32_t oldq, float alpha);
	float 		getComp(int oldbits, int qporg, int newbits, int qpused);

public:
	ADM_newXvidRcVBV(uint32_t fps1000, char *logname);
	virtual 	~ADM_newXvidRcVBV() ;
	/** Maxbr & minbr in kbps, vbvsize in kBytes); Default is none */
	virtual 	uint8_t setVBVInfo(uint32_t maxbr,uint32_t minbr, uint32_t vbvsize);
	virtual		uint8_t startPass1( void );
	virtual		uint8_t logPass1(uint32_t qz, ADM_rframe ftype,uint32_t size);
	virtual		uint8_t startPass2( uint32_t size,uint32_t nbFrame );
	virtual		uint8_t getQz( uint32_t *qz, ADM_rframe *type );
	virtual		uint8_t logPass2( uint32_t qz, ADM_rframe ftype,uint32_t size);
	uint8_t verifyLog(const char *file,uint32_t nbFrame);
};
#endif
