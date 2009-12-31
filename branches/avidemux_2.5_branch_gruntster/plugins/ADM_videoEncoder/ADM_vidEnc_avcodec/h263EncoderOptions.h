/***************************************************************************
                            h263EncoderOptions.h

    begin                : Wed Dec 30 2009
    copyright            : (C) 2009 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef h263EncoderOptions_h
#define h263EncoderOptions_h

#include <vector>
#include "PluginOptions.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

#define H263_DEFAULT_ENCODE_MODE ADM_VIDENC_MODE_CQP
#define H263_DEFAULT_ENCODE_MODE_PARAMETER 4

typedef enum
{
	H263_MEM_NONE = 1,
	H263_MEM_FULL,
	H263_MEM_LOG,
	H263_MEM_PHODS,
	H263_MEM_EPZS
} H263MotionEstimationMethod;

typedef enum
{
	H263_QUANT_H263 = 0,
	H263_QUANT_MPEG
} H263QuantisationType;

typedef enum
{
	H263_MBDEC_SAD = 0,
	H263_MBDEC_BITS,
	H263_MBDEC_RD
} H263MacroblockDecisionMode;

class H263EncoderOptions : public PluginOptions
{
protected:
	H263MotionEstimationMethod _motionEst;
	unsigned int _4MV, _maxBFrames, _qpel, _gmc;
	H263QuantisationType _quantType;
	H263MacroblockDecisionMode _mbDecision;
	unsigned int _minQuantiser, _maxQuantiser, _maxQuantiserDiff, _trellis;
	float _quantCompression, _quantBlur;

	void addOptionsToXml(xmlNodePtr xmlNodeRoot);
	void parseOptions(xmlNode *node);

public:
	H263EncoderOptions(void);
	void reset(void);

	H263MotionEstimationMethod getMotionEstimationMethod(void);
	void setMotionEstimationMethod(H263MotionEstimationMethod motionEst);

	bool get4MotionVector(void);
	void set4MotionVector(bool fourMv);

	unsigned int getMaxBFrames(void);
	void setMaxBFrames(unsigned int maxBFrames);

	bool getQuarterPixel(void);
	void setQuarterPixel(bool qpel);

	bool getGmc(void);
	void setGmc(bool gmc);

	H263QuantisationType getQuantisationType(void);
	void setQuantisationType(H263QuantisationType quantType);

	H263MacroblockDecisionMode getMbDecisionMode(void);
	void setMbDecisionMode(H263MacroblockDecisionMode mbDecision);

	unsigned int getMinQuantiser(void);
	void setMinQuantiser(unsigned int quantiser);

	unsigned int getMaxQuantiser(void);
	void setMaxQuantiser(unsigned int quantiser);

	unsigned int getQuantiserDifference(void);
	void setQuantiserDifference(unsigned int difference);

	bool getTrellis(void);
	void setTrellis(bool trellis);

	float getQuantiserCompression(void);
	void setQuantiserCompression(float compression);

	float getQuantiserBlur(void);
	void setQuantiserBlur(float blur);
};

#endif	// h263EncoderOptions_h
