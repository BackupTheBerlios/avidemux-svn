#ifndef XVIDRATECTL_H
#define XVIDRATECTL_H

#include <inttypes.h>
#include "rateControl.h"

class ADM_newXvidRc : public ADM_ratecontrol
{
protected:
	uint32_t _totalFrame;
public:
	ADM_newXvidRc(uint32_t fps1000, char *logname);
	virtual 	~ADM_newXvidRc();
	/** Maxbr & minbr in kbps, vbvsize in kBytes); Default is none */
	virtual 	uint8_t setVBVInfo(uint32_t maxbr,uint32_t minbr, uint32_t vbvsize);
	virtual		uint8_t startPass1( void );
	virtual		uint8_t logPass1(uint32_t qz, ADM_rframe ftype,uint32_t size);
	virtual		uint8_t startPass2( uint32_t size,uint32_t nbFrame );
	virtual		uint8_t getQz( uint32_t *qz, ADM_rframe *type );
	virtual		uint8_t logPass2( uint32_t qz, ADM_rframe ftype,uint32_t size);
	// Used for VBV
	uint8_t getInfo(uint32_t framenum, uint32_t *qz, uint32_t *size,ADM_rframe *type);

};
#endif
