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

#include <libxml/tree.h>
#include "xvid.h"

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
	CONFIG_CUSTOM,
	CONFIG_DEFAULT,
	CONFIG_USER,
	CONFIG_SYSTEM
} ConfigType;

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

class XvidOptions
{
protected:
	xvid_enc_create_t xvid_enc_create;
	xvid_enc_frame_t xvid_enc_frame;

	char* _configurationName;
	ConfigType _configurationType;

	bool _parAsInput;

	void cleanUp(void);
	xmlChar* number2String(xmlChar *buffer, size_t size, int number);
	xmlChar* number2String(xmlChar *buffer, size_t size, unsigned int number);
	xmlChar* number2String(xmlChar *buffer, size_t size, float number);
	xmlChar* boolean2String(xmlChar *buffer, size_t size, bool boolean);
	bool string2Boolean(char *buffer);

	void addXvidOptionsToXml(xmlNodePtr xmlNodeRoot);
	char* dumpXmlDocToMemory(xmlDocPtr xmlDoc);
	bool validateXml(xmlDocPtr doc);
	void parsePresetConfiguration(xmlNode *node);
	void parseXvidOptions(xmlNode *node);
	void parseVuiOptions(xmlNode *node);

public:
	XvidOptions(void);
	~XvidOptions(void);

	void reset(void);

	void getPresetConfiguration(char** configurationName, ConfigType *configurationType);
	void setPresetConfiguration(const char* configurationName, ConfigType configurationType);
	void clearPresetConfiguration(void);

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

	bool getGreyscale(void);
	void setGreyscale(bool greyscale);

	InterlacedMode getInterlaced(void);
	void setInterlaced(InterlacedMode interlaced);

	unsigned int getFrameDropRatio(void);
	void setFrameDropRatio(unsigned int frameDropRatio);

	unsigned int getMaxKeyInterval(void);
	void setMaxKeyInterval(unsigned int maxKeyInterval);

	unsigned int getMaxBFrames(void);
	void setMaxBFrames(unsigned int maxBFrames);

	bool getPacked(void);
	void setPacked(bool packed);

	bool getMpegQuantisation(void);
	void setMpegQuantisation(bool mpegQuantisation);

	bool getTrellis(void);
	void setTrellis(bool trellis);

	bool getCartoon(void);
	void setCartoon(bool cartoon);

	void getMinQuantiser(unsigned int *i, unsigned int *p, unsigned int *b);
	void setMinQuantiser(unsigned int i, unsigned int p, unsigned int b);

	void getMaxQuantiser(unsigned int *i, unsigned int *p, unsigned int *b);
	void setMaxQuantiser(unsigned int i, unsigned int p, unsigned int b);

	bool getClosedGop(void);
	void setClosedGop(bool closedGop);

	virtual char* toXml(void);
	virtual int fromXml(const char *xml);
};

#endif	// options_h
