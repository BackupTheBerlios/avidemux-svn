/***************************************************************************
                                XvidOptions.h

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

#ifndef options_h
#define options_h

#include "xvid.h"

enum MotionEstimationMode
{
	ME_NONE = 0,
	ME_LOW = XVID_ME_HALFPELREFINE16,	
	ME_MEDIUM = XVID_ME_HALFPELREFINE16 | XVID_ME_ADVANCEDDIAMOND16,	
	ME_HIGH = XVID_ME_HALFPELREFINE16 | XVID_ME_EXTSEARCH16 | XVID_ME_HALFPELREFINE8 | XVID_ME_USESQUARES16
};

enum RateDistortionMode
{
	RD_NONE = 0,
	RD_DCT_ME = XVID_VOP_MODEDECISION_RD,
	RD_HPEL_QPEL_16 = RD_DCT_ME | XVID_ME_HALFPELREFINE16_RD | XVID_ME_QUARTERPELREFINE16_RD,
	RD_HPEL_QPEL_8 = RD_HPEL_QPEL_16 | XVID_ME_HALFPELREFINE8_RD | XVID_ME_QUARTERPELREFINE8_RD | XVID_ME_CHECKPREDICTION_RD,
	RD_SQUARE = RD_HPEL_QPEL_8 | XVID_ME_EXTSEARCH_RD
};

class XvidOptions
{
protected:
	xvid_enc_create_t xvid_enc_create;
	xvid_enc_frame_t xvid_enc_frame;

public:
	XvidOptions(void);
	~XvidOptions(void);

	MotionEstimationMode getMotionEstimation(void);
	void setMotionEstimation(MotionEstimationMode motionEstimation);

	unsigned int getMaxKeyInterval(void);
	void setMaxKeyInterval(unsigned int maxKeyInterval);

	unsigned int getMaxBFrames(void);
	void setMaxBFrames(unsigned int maxBFrames);

	bool getMpegQuantisation(void);
	void setMpegQuantisation(bool mpegQuantisation);

	bool getInterlaced(void);
	void setInterlaced(bool interlaced);

	bool getInterMotionVector(void);
	void setInterMotionVector(bool interMotionVector);

	bool getTrellis(void);
	void setTrellis(bool trellis);

	bool getCartoon(void);
	void setCartoon(bool cartoon);

	bool getGreyscale(void);
	void setGreyscale(bool greyscale);

	bool getQpel(void);
	void setQpel(bool qpel);

	bool getGmc(void);
	void setGmc(bool gmc);

	bool getBvhq(void);
	void setBvhq(bool bvhq);

	bool getAcPrediction(void);
	void setAcPrediction(bool acPrediction);

	bool getChromaOptimisation(void);
	void setChromaOptimisation(bool chromaOptimisation);

	void getMinQuantiser(unsigned int *i, unsigned int *p, unsigned int *b);
	void setMinQuantiser(unsigned int i, unsigned int p, unsigned int b);

	void getMaxQuantiser(unsigned int *i, unsigned int *p, unsigned int *b);
	void setMaxQuantiser(unsigned int i, unsigned int p, unsigned int b);

	void getPar(unsigned int *width, unsigned int *height);
	void setPar(unsigned int width, unsigned int height);

	RateDistortionMode getRateDistortion(RateDistortionMode rateDistortion);
	void setRateDistortion(RateDistortionMode rateDistortion);

	bool getPacked(void);
	void setPacked(bool packed);

	bool getClosedGop(void);
	void setClosedGop(bool closedGop);
};

#endif	// options_h
