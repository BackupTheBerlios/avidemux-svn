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

#ifndef xvidOptions_h
#define xvidOptions_h

#include <libxml/tree.h>
#include "xvid.h"
#include "PluginOptions.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

#define DEFAULT_ENCODE_MODE	(ADM_VIDENC_MODE_CQP)
#define DEFAULT_ENCODE_MODE_PARAMETER (4)
#define TURBO_MODE (XVID_ME_FASTREFINE16 | XVID_ME_FASTREFINE8 | XVID_ME_SKIP_DELTASEARCH | XVID_ME_FAST_MODEINTERPOLATE | XVID_ME_BFRAME_EARLYSTOP)
#define CHROMA_ME (XVID_ME_CHROMA_PVOP | XVID_ME_CHROMA_BVOP)

typedef enum
{
	ME_NONE = 0,
	ME_LOW = XVID_ME_HALFPELREFINE16,	
	ME_MEDIUM = XVID_ME_HALFPELREFINE16 | XVID_ME_ADVANCEDDIAMOND16,	
	ME_HIGH = XVID_ME_HALFPELREFINE16 | XVID_ME_EXTSEARCH16 | XVID_ME_HALFPELREFINE8 | XVID_ME_USESQUARES16
} MotionEstimationMode;

typedef enum
{
	INTERLACED_NONE,
	INTERLACED_BFF,
	INTERLACED_TFF
} InterlacedMode;

typedef enum
{
	RD_NONE = -1,
	RD_DCT_ME = 0,
	RD_HPEL_QPEL_16 = RD_DCT_ME | XVID_ME_HALFPELREFINE16_RD | XVID_ME_QUARTERPELREFINE16_RD,
	RD_HPEL_QPEL_8 = RD_HPEL_QPEL_16 | XVID_ME_HALFPELREFINE8_RD | XVID_ME_QUARTERPELREFINE8_RD | XVID_ME_CHECKPREDICTION_RD,
	RD_SQUARE = RD_HPEL_QPEL_8 | XVID_ME_EXTSEARCH_RD
} RateDistortionMode;

typedef enum
{
	CQM_H263,
	CQM_MPEG,
	CQM_CUSTOM
} CqmPresetMode;

class XvidOptions : public PluginOptions
{
protected:
	xvid_enc_create_t xvid_enc_create;
	xvid_enc_frame_t xvid_enc_frame;
	xvid_plugin_single_t xvid_plugin_single;
	xvid_plugin_2pass2_t xvid_plugin_2pass2;

	bool _parAsInput;
	CqmPresetMode _cqmPreset;
	unsigned char _intraMatrix[64], _interMatrix[64];

	void addOptionsToXml(xmlNodePtr xmlNodeRoot);
	void parseOptions(xmlNode *node);
	void parseVuiOptions(xmlNode *node);
	void parseCqmOption(xmlNode *node, unsigned char matrix[]);
	void parseSinglePassOptions(xmlNode *node);
	void parseTwoPassOptions(xmlNode *node);

public:
	XvidOptions(void);
	void reset(void);

	void getParameters(xvid_enc_create_t **xvid_enc_create, xvid_enc_frame_t **xvid_enc_frame,
		xvid_plugin_single_t **xvid_plugin_single, xvid_plugin_2pass2_t **xvid_plugin_2pass2);

	unsigned int getThreads(void);
	void setThreads(unsigned int threads);

	bool getParAsInput(void);
	void setParAsInput(bool parAsInput);

	void getPar(unsigned int *width, unsigned int *height);
	void setPar(unsigned int width, unsigned int height);

	MotionEstimationMode getMotionEstimation(void);
	void setMotionEstimation(MotionEstimationMode motionEstimation);

	RateDistortionMode getRateDistortion(void);
	void setRateDistortion(RateDistortionMode rateDistortion);

	bool getBframeRdo(void);
	void setBframeRdo(bool bFrameRdo);

	bool getChromaMotionEstimation(void);
	void setChromaMotionEstimation(bool chromaMotionEstimation);

	bool getQpel(void);
	void setQpel(bool qpel);

	bool getGmc(void);
	void setGmc(bool gmc);

	bool getTurboMode(void);
	void setTurboMode(bool turboMode);

	bool getChromaOptimisation(void);
	void setChromaOptimisation(bool chromaOptimisation);

	bool getInterMotionVector(void);
	void setInterMotionVector(bool interMotionVector);

	bool getCartoon(void);
	void setCartoon(bool cartoon);

	bool getGreyscale(void);
	void setGreyscale(bool greyscale);

	InterlacedMode getInterlaced(void);
	void setInterlaced(InterlacedMode interlaced);

	unsigned int getFrameDropRatio(void);
	void setFrameDropRatio(unsigned int frameDropRatio);

	unsigned int getMaxKeyInterval(void);
	void setMaxKeyInterval(unsigned int maxKeyInterval);

	unsigned int getMaxBframes(void);
	void setMaxBframes(unsigned int maxBFrames);

	int getBframeSensitivity(void);
	void setBframeSensitivity(int bFrameSensitivity);

	bool getClosedGop(void);
	void setClosedGop(bool closedGop);

	bool getPacked(void);
	void setPacked(bool packed);

	void getMinQuantiser(unsigned int *i, unsigned int *p, unsigned int *b);
	void setMinQuantiser(unsigned int i, unsigned int p, unsigned int b);

	void getMaxQuantiser(unsigned int *i, unsigned int *p, unsigned int *b);
	void setMaxQuantiser(unsigned int i, unsigned int p, unsigned int b);

	unsigned int getBframeQuantiserRatio(void);
	void setBframeQuantiserRatio(unsigned int ratio);

	unsigned int getBframeQuantiserOffset(void);
	void setBframeQuantiserOffset(unsigned int offset);

	CqmPresetMode getCqmPreset(void);
	void setCqmPreset(CqmPresetMode cqmPreset);

	void getIntraMatrix(unsigned char intra[64]);
	void setIntraMatrix(unsigned char intra[64]);

	void getInterMatrix(unsigned char inter[64]);
	void setInterMatrix(unsigned char inter[64]);

	bool getTrellis(void);
	void setTrellis(bool trellis);

	unsigned int getReactionDelayFactor(void);
	void setReactionDelayFactor(unsigned int delayFactor);

	unsigned int getAveragingQuantiserPeriod(void);
	void setAveragingQuantiserPeriod(unsigned int averagingPeriod);

	unsigned int getSmoother(void);
	void setSmoother(unsigned int smoother);

	unsigned int getKeyFrameBoost(void);
	void setKeyFrameBoost(unsigned int keyFrameBoost);

	unsigned int getMaxKeyFrameReduceBitrate(void);
	void setMaxKeyFrameReduceBitrate(unsigned int bitrateReduction);

	unsigned int getKeyFrameBitrateThreshold(void);
	void setKeyFrameBitrateThreshold(unsigned int bitrateThreshold);

	unsigned int getOverflowControlStrength(void);
	void setOverflowControlStrength(unsigned int overflowControlStrength);

	unsigned int getMaxOverflowImprovement(void);
	void setMaxOverflowImprovement(unsigned int overflowImprovement);

	unsigned int getMaxOverflowDegradation(void);
	void setMaxOverflowDegradation(unsigned int overflowDegradation);

	unsigned int getAboveAverageCurveCompression(void);
	void setAboveAverageCurveCompression(unsigned int curveCompression);

	unsigned int getBelowAverageCurveCompression(void);
	void setBelowAverageCurveCompression(unsigned int curveCompression);

	unsigned int getVbvBufferSize(void);
	void setVbvBufferSize(unsigned int bufferSize);

	unsigned int getMaxVbvBitrate(void);
	void setMaxVbvBitrate(unsigned int bitrate);

	unsigned int getVbvPeakBitrate(void);
	void setVbvPeakBitrate(unsigned int peakBitrate);
};

#endif	// xvidOptions_h
