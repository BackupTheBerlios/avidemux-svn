/***************************************************************************
                          mpeg4aspEncoderOptions.h

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

#ifndef mpeg4aspEncoderOptions_h
#define mpeg4aspEncoderOptions_h

#include <vector>
#include "PluginOptions.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

#define MPEG4ASP_DEFAULT_ENCODE_MODE ADM_VIDENC_MODE_CQP
#define MPEG4ASP_DEFAULT_ENCODE_MODE_PARAMETER 4

typedef enum
{
	MPEG4ASP_MEM_NONE = 1,
	MPEG4ASP_MEM_FULL,
	MPEG4ASP_MEM_LOG,
	MPEG4ASP_MEM_PHODS,
	MPEG4ASP_MEM_EPZS
} Mpeg4aspMotionEstimationMethod;

typedef enum
{
	MPEG4ASP_QUANT_H263 = 0,
	MPEG4ASP_QUANT_MPEG
} Mpeg4aspQuantisationType;

typedef enum
{
	MPEG4ASP_MBDEC_SAD = 0,
	MPEG4ASP_MBDEC_BITS,
	MPEG4ASP_MBDEC_RD
} Mpeg4aspMacroblockDecisionMode;

class Mpeg4aspEncoderOptions : public PluginOptions
{
protected:
	Mpeg4aspMotionEstimationMethod _motionEst;
	unsigned int _4MV, _maxBFrames, _qpel, _gmc;
	Mpeg4aspQuantisationType _quantType;
	Mpeg4aspMacroblockDecisionMode _mbDecision;
	unsigned int _minQuantiser, _maxQuantiser, _maxQuantiserDiff, _trellis;
	float _quantCompression, _quantBlur;

	void addOptionsToXml(xmlNodePtr xmlNodeRoot);
	void parseOptions(xmlNode *node);

public:
	Mpeg4aspEncoderOptions(void);
	void reset(void);

	Mpeg4aspMotionEstimationMethod getMotionEstimationMethod(void);
	void setMotionEstimationMethod(Mpeg4aspMotionEstimationMethod motionEst);

	bool get4MotionVector(void);
	void set4MotionVector(bool fourMv);

	unsigned int getMaxBFrames(void);
	void setMaxBFrames(unsigned int maxBFrames);

	bool getQuarterPixel(void);
	void setQuarterPixel(bool qpel);

	bool getGmc(void);
	void setGmc(bool gmc);

	Mpeg4aspQuantisationType getQuantisationType(void);
	void setQuantisationType(Mpeg4aspQuantisationType quantType);

	Mpeg4aspMacroblockDecisionMode getMbDecisionMode(void);
	void setMbDecisionMode(Mpeg4aspMacroblockDecisionMode mbDecision);

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

#endif	// mpeg4aspEncoderOptions_h
