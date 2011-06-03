 /***************************************************************************
                              h263Encoder.cpp

    begin                : Wed Dec 30 2009
    copyright            : (C) 2009 by gruntster
 ***************************************************************************/

 /**************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <sstream>
#include <string>
#include <libxml/tree.h>

#include "ADM_inttype.h"
#include "ADM_plugin_translate.h"
#include "DIA_coreToolkit.h"
#include "h263Encoder.h"

extern int _uiType;
static bool changedConfig(const char* fileName, ConfigMenuType configType);
static char *serializeConfig(void);
static H263Encoder *encoder = NULL;

#ifdef __WIN32
extern void convertPathToAnsi(const char *path, char **ansiPath);
#endif

H263Encoder::H263Encoder(void)
{
	encoder = this;

	init(CODEC_ID_H263, ADM_CSP_YV12);

	_encodeOptions.structSize = sizeof(vidEncOptions);
	_encodeOptions.encodeMode = H263_DEFAULT_ENCODE_MODE;
	_encodeOptions.encodeModeParameter = H263_DEFAULT_ENCODE_MODE_PARAMETER;

	_bitrateParam.capabilities = ADM_ENC_CAP_CBR | ADM_ENC_CAP_CQ | ADM_ENC_CAP_2PASS | ADM_ENC_CAP_2PASS_BR;
	_bitrateParam.qz = H263_DEFAULT_ENCODE_MODE_PARAMETER;
	_bitrateParam.bitrate = 1500;
	_bitrateParam.avg_bitrate = 1000;
	_bitrateParam.finalsize = 700;

	_statFile = NULL;
}

int H263Encoder::initContext(const char* logFileName)
{
	int ret = AvcodecEncoder::initContext(logFileName);

	_context->me_method = (int)_options.getMotionEstimationMethod();

	if (_options.get4MotionVector())
		_context->flags |= CODEC_FLAG_4MV;

	_context->max_b_frames = _options.getMaxBFrames();

	if (_options.getQuarterPixel())
		_context->flags |= CODEC_FLAG_QPEL;
	
	if (_options.getGmc())
		_context->flags |= CODEC_FLAG_GMC;

	_context->mpeg_quant = (int)_options.getQuantisationType();

	switch (_options.getMbDecisionMode())
	{
		case H263_MBDEC_BITS:
			_context->mb_decision = FF_MB_DECISION_BITS;
			break;
		case H263_MBDEC_RD:
			_context->mb_decision = FF_MB_DECISION_RD;
			break;
		default:
			_context->mb_decision = FF_MB_DECISION_SIMPLE;
			_context->mb_cmp = FF_CMP_SAD;
	}

	_context->qmin = _options.getMinQuantiser();
	_context->qmax = _options.getMaxQuantiser();
	_context->max_qdiff = _options.getQuantiserDifference();
	_context->trellis = _options.getTrellis();
	_context->qcompress = _options.getQuantiserCompression();
	_context->qblur = _options.getQuantiserBlur();

	_context->lumi_masking = 0.05;
	_context->dark_masking = 0.01;
	_context->rc_qsquish = 1.0;
	_context->luma_elim_threshold = -2;
	_context->chroma_elim_threshold = -5;
	_context->i_quant_factor = 0.8;
	_context->bit_rate_tolerance = 1024 * 8 * 1000;
	_context->gop_size = 250;

	if (_currentPass == 1)
	{
		if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_CBR)
			_context->bit_rate = _encodeOptions.encodeModeParameter * 1000;
		else
		{
			_context->bit_rate = 0;
			_context->flags |= CODEC_FLAG_QSCALE;
		}

		if (_passCount > 1)
			_context->flags |= CODEC_FLAG_PASS1;
	}
	else
	{
		_context->flags |= CODEC_FLAG_PASS2;

		if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_SIZE)
			_context->bit_rate = calculateBitrate(_fpsNum, _fpsDen, _frameCount, _encodeOptions.encodeModeParameter);
		else
			_context->bit_rate = _encodeOptions.encodeModeParameter * 1000;
	}

	if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_SIZE || _encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_ABR)
	{
		char *log = NULL;

#ifdef __WIN32
		convertPathToAnsi(logFileName, &log);
#else
		log = new char[strlen(logFileName) + 1];
		strcpy(log, logFileName);
#endif

		if (_currentPass == 1)
		{
			_statFile = fopen(log, "wb");

			if (!_statFile)
				ret = ADM_VIDENC_ERR_FAILED;
		}
		else
		{
			FILE *statFile = fopen(log, "rb");

			if (statFile)
			{
				fseek(statFile, 0, SEEK_END);

				long statSize = ftello(statFile);

				fseek(statFile, 0, SEEK_SET);
				_context->stats_in = new char[statSize + 1];
				_context->stats_in[statSize] = 0;

				fread(_context->stats_in, statSize, 1, statFile);
				fclose(statFile);
			}
			else
				ret = ADM_VIDENC_ERR_FAILED;
		}
	}

	return ret;
}

const char* H263Encoder::getEncoderType(void)
{
	return "H.263";
}

const char* H263Encoder::getEncoderDescription(void)
{
	return "H.263 video encoder plugin for Avidemux (c) Mean/Gruntster";
}

const char* H263Encoder::getFourCC(void)
{
	return "H263";
}

const char* H263Encoder::getEncoderGuid(void)
{
	return "4279DF66-ECEF-4d3d-AFEA-1BFCCB79E219";
}

int H263Encoder::isConfigurable(void)
{
	return (_uiType == ADM_UI_GTK || _uiType == ADM_UI_QT4);
}

int H263Encoder::configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	loadSettings(&_encodeOptions, &_options);

	diaMenuEntry meE[] = {
		{0, QT_TR_NOOP("None")},
		{1, QT_TR_NOOP("Full")},
		{2, QT_TR_NOOP("Log")},
		{3, QT_TR_NOOP("Phods")},
		{4, QT_TR_NOOP("EPZS")}};

	diaMenuEntry qzE[] = {
		{0, QT_TR_NOOP("H.263")},
		{1, QT_TR_NOOP("MPEG")}};

	diaMenuEntry rdE[] = {
		{0, QT_TR_NOOP("Sum of Absolute Differences")},
		{1, QT_TR_NOOP("Fewest Bits")},
		{2, QT_TR_NOOP("Rate Distortion")}};

	// Encoding mode
	diaElemBitrate ctlBitrate(&_bitrateParam, NULL);
		
	diaElem *diamode[] = {&ctlBitrate};
	diaElemTabs tabMode(QT_TR_NOOP("Encoding Mode"), 1, diamode);

	// Motion Estimation
	diaElemMenu ctlMatrices(&_motionEst, QT_TR_NOOP("Motion Estimation Method:"), 5, meE);
	diaElemUInteger ctlMaxBFrames(&_maxBFrames, QT_TR_NOOP("_Maximum Consecutive B-frames:"), 0, 32);
	diaElemToggle ctl4MV(&_4MV, QT_TR_NOOP("4 _Motion Vector"));
	diaElemToggle ctlQpel(&_qpel, QT_TR_NOOP("_Quarter Pixel"));
	diaElemToggle ctlGmc(&_gmc, QT_TR_NOOP("_Global Motion Compensation"));

	diaElem *diaME[] = {&ctlMatrices, &ctlMaxBFrames, &ctl4MV, &ctlQpel, &ctlGmc};
	diaElemTabs tabME(QT_TR_NOOP("Motion Estimation"), 5, diaME);

	// Quantisation
	diaElemMenu ctlQuantType(&_quantType, QT_TR_NOOP("_Quantisation Type:"), 2, qzE);
	diaElemMenu ctlMbDecision(&_mbDecision, QT_TR_NOOP("_Macroblock Decision Mode:"), 3, rdE);
	diaElemUInteger ctlQuantMin(&_minQuantiser, QT_TR_NOOP("Mi_nimum Quantiser:"), 1, 31);
	diaElemUInteger ctlQuantMax(&_maxQuantiser, QT_TR_NOOP("Ma_ximum Quantiser:"), 1, 31);
	diaElemUInteger ctlQuantDiff(&_maxQuantiserDiff, QT_TR_NOOP("Maximum Quantiser _Difference:"), 1, 31);
	diaElemFloat ctlQuantCompression(&_quantCompression, QT_TR_NOOP("_Quantiser Compression:"), 0, 1);
	diaElemFloat ctlQuantBlur(&_quantBlur, QT_TR_NOOP("Quantiser _Blur:"), 0, 1);
	diaElemToggle ctlTrellis(&_trellis, QT_TR_NOOP("_Trellis Quantisation"));

	diaElem *diaQze[] = {&ctlQuantType, &ctlMbDecision, &ctlQuantMin, &ctlQuantMax, &ctlQuantDiff, &ctlQuantCompression, &ctlQuantBlur, &ctlTrellis};
	diaElemTabs tabQz(QT_TR_NOOP("Quantisation"), 8, diaQze);

	diaElemTabs *tabs[] = {&tabMode, &tabME, &tabQz};
	int controlCount = 0;

	for (int i = 0; i < 3; i++)
		controlCount += tabs[i]->nbElems;

	diaElem *diaAll[controlCount];
	int controlIndex = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < tabs[i]->nbElems; j++)
		{
			diaAll[controlIndex] = tabs[i]->dias[j];
			controlIndex++;
		}
	}

	diaElemConfigMenu ctlConfigMenu(configName, &configType, _options.getUserConfigDirectory(), _options.getSystemConfigDirectory(),
		changedConfig, serializeConfig, diaAll, controlCount);
	diaElem *elmHeader[] = {&ctlConfigMenu};

	if (diaFactoryRunTabs(QT_TR_NOOP("avcodec H.263 Configuration"), 1, elmHeader, 3, tabs))
	{
		saveSettings(&_encodeOptions, &_options);
		updateEncodeProperties(&_encodeOptions);

		return 1;
	}

	return 0;
}

void H263Encoder::loadSettings(vidEncOptions *encodeOptions, H263EncoderOptions *options)
{
	char *configurationName;

	options->getPresetConfiguration(&configurationName, (PluginConfigType*)&configType);

	if (configurationName)
	{
		strcpy(this->configName, configurationName);
		delete [] configurationName;
	}

	if (encodeOptions)
	{
		_motionEst = (H263MotionEstimationMethod)(options->getMotionEstimationMethod() - 1);
		_4MV = options->get4MotionVector();
		_maxBFrames = options->getMaxBFrames();
		_qpel = options->getQuarterPixel();
		_gmc = options->getGmc();

		_quantType = (H263QuantisationType)options->getQuantisationType();
		_mbDecision = (H263MacroblockDecisionMode)options->getMbDecisionMode();
		_minQuantiser = options->getMinQuantiser();
		_maxQuantiser = options->getMaxQuantiser();
		_maxQuantiserDiff = options->getQuantiserDifference();
		_trellis = options->getTrellis();
		_quantCompression = options->getQuantiserCompression();
		_quantBlur = options->getQuantiserBlur();

		updateEncodeProperties(encodeOptions);
	}
}

void H263Encoder::saveSettings(vidEncOptions *encodeOptions, H263EncoderOptions *options)
{
	options->setPresetConfiguration(&configName[0], (PluginConfigType)configType);

	switch (_bitrateParam.mode)
	{
		case COMPRESS_CQ:
			encodeOptions->encodeMode = ADM_VIDENC_MODE_CQP;
			encodeOptions->encodeModeParameter = _bitrateParam.qz;

			break;
		case COMPRESS_CBR:
			encodeOptions->encodeMode = ADM_VIDENC_MODE_CBR;
			encodeOptions->encodeModeParameter = _bitrateParam.bitrate;

			break;
		case COMPRESS_2PASS:
			encodeOptions->encodeMode = ADM_VIDENC_MODE_2PASS_SIZE;
			encodeOptions->encodeModeParameter = _bitrateParam.finalsize;

			break;
		case COMPRESS_2PASS_BITRATE:
			encodeOptions->encodeMode = ADM_VIDENC_MODE_2PASS_ABR;
			encodeOptions->encodeModeParameter = _bitrateParam.avg_bitrate;

			break;
	}

	options->setMotionEstimationMethod((H263MotionEstimationMethod)(_motionEst + 1));
	options->set4MotionVector(_4MV);
	options->setMaxBFrames(_maxBFrames);
	options->setQuarterPixel(_qpel);
	options->setGmc(_gmc);

	options->setQuantisationType((H263QuantisationType)_quantType);
	options->setMbDecisionMode((H263MacroblockDecisionMode)_mbDecision);
	options->setMinQuantiser(_minQuantiser);
	options->setMaxQuantiser(_maxQuantiser);
	options->setQuantiserDifference(_maxQuantiserDiff);
	options->setTrellis(_trellis);
	options->setQuantiserCompression(_quantCompression);
	options->setQuantiserBlur(_quantBlur);
}

bool changedConfig(const char* configName, ConfigMenuType configType)
{
	bool failure = false;

	if (configType == CONFIG_MENU_DEFAULT)
	{
		H263EncoderOptions defaultOptions;
		vidEncOptions *defaultEncodeOptions = defaultOptions.getEncodeOptions();

		encoder->loadSettings(defaultEncodeOptions, &defaultOptions);

		delete defaultEncodeOptions;
	}
	else
	{
		H263EncoderOptions options;

		options.setPresetConfiguration(configName, (PluginConfigType)configType);

		if (configType == CONFIG_MENU_CUSTOM)
			encoder->loadSettings(NULL, &options);
		else
		{
			vidEncOptions *encodeOptions;

			if (options.loadPresetConfiguration())
			{
				encodeOptions = options.getEncodeOptions();

				encoder->loadSettings(encodeOptions, &options);

				delete encodeOptions;
			}
			else
			{
				failure = true;
			}
		}
	}

	return (configType == CONFIG_MENU_CUSTOM) | !failure;
}

char *serializeConfig(void)
{
	vidEncOptions encodeOptions;
	H263EncoderOptions options;

	encoder->saveSettings(&encodeOptions, &options);
	options.setEncodeOptions(&encodeOptions);

	return options.toXml(PLUGIN_XML_EXTERNAL);
}

int H263Encoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
{
	char* xml = _options.toXml(PLUGIN_XML_INTERNAL);
	int xmlLength = strlen(xml);

	if (bufferSize >= xmlLength)
	{
		memcpy(pluginOptions, xml, xmlLength);
		memcpy(encodeOptions, &_encodeOptions, sizeof(vidEncOptions));
	}
	else if (bufferSize != 0)
		xmlLength = 0;

	delete [] xml;

	return xmlLength;
}

int H263Encoder::setOptions(vidEncOptions *encodeOptions, const char *pluginOptions)
{
	if (_opened)
		return ADM_VIDENC_ERR_ALREADY_OPEN;

	bool success = true;

	if (pluginOptions)
	{
		success = _options.fromXml(pluginOptions, PLUGIN_XML_INTERNAL);

		_options.loadPresetConfiguration();
	}

	if (encodeOptions && success)
	{
		memcpy(&_encodeOptions, encodeOptions, sizeof(vidEncOptions));
		updateEncodeProperties(encodeOptions);
	}

	if (success)
		return ADM_VIDENC_ERR_SUCCESS;
	else
		return ADM_VIDENC_ERR_FAILED;
}

int H263Encoder::open(vidEncVideoProperties *properties)
{
	int ret = AvcodecEncoder::open(properties);
    int profileCount = sizeof(h263Profiles) / sizeof(h263Profiles[0]);
	bool validProfile = false;

	if (ret == ADM_VIDENC_ERR_SUCCESS)
	{
		for (int i = 0; i < profileCount; i++)
		{
			if (properties->height == h263Profiles[i].height && properties->width == h263Profiles[i].width)
			{
				validProfile = true;
				break;
			}
		}

		if (!validProfile)
		{
			std::string msg;
			std::stringstream out;
			
			out << QT_TR_NOOP("The H.263 encoder only accepts the following resolutions:");

			for (int i = 0; i < profileCount; i++)
				out << "\n" << h263Profiles[i].width << " x " << h263Profiles[i].height;

			ret = ADM_VIDENC_ERR_FAILED;
			msg = out.str();

			GUI_Error_HIG(QT_TR_NOOP("Incompatible settings"), msg.c_str());
		}
	}

	return ret;
}

int H263Encoder::beginPass(vidEncPassParameters *passParameters)
{
	int qz = 0;
	int ret = AvcodecEncoder::beginPass(passParameters);

	if (_encodeOptions.encodeMode == ADM_VIDENC_MODE_CQP)
		qz = _encodeOptions.encodeModeParameter;
	else if ((_encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_SIZE || _encodeOptions.encodeMode == ADM_VIDENC_MODE_2PASS_ABR) && _currentPass == 1)
		qz = 2;

	if (qz)
		_frame.quality = (int)floor(FF_QP2LAMBDA * qz + 0.5);

	return ret;
}

int H263Encoder::finishPass(void)
{
	int ret = AvcodecEncoder::finishPass();

	if (_statFile)
	{
		fclose(_statFile);
		_statFile = NULL;
	}

	if (_context && _context->stats_in)
	{
		delete [] _context->stats_in;
		_context->stats_in = NULL;
	}

	return ret;
}

int H263Encoder::encodeFrame(vidEncEncodeParameters *encodeParams)
{
	int ret = AvcodecEncoder::encodeFrame(encodeParams);

	if (_context->stats_out && _statFile)
		fprintf(_statFile, "%s", _context->stats_out);

	return ret;
}

void H263Encoder::updateEncodeProperties(vidEncOptions *encodeOptions)
{
	switch (encodeOptions->encodeMode)
	{
		case ADM_VIDENC_MODE_CQP:
			_passCount = 1;

			_bitrateParam.mode = COMPRESS_CQ;
			_bitrateParam.qz = encodeOptions->encodeModeParameter;

			break;
		case ADM_VIDENC_MODE_CBR:
			_passCount = 1;

			_bitrateParam.mode = COMPRESS_CBR;
			_bitrateParam.bitrate = encodeOptions->encodeModeParameter;

			break;
		case ADM_VIDENC_MODE_2PASS_SIZE:
			_passCount = 2;

			_bitrateParam.mode = COMPRESS_2PASS;
			_bitrateParam.finalsize = encodeOptions->encodeModeParameter;

			break;
		case ADM_VIDENC_MODE_2PASS_ABR:
			_passCount = 2;

			_bitrateParam.mode = COMPRESS_2PASS_BITRATE;
			_bitrateParam.avg_bitrate = encodeOptions->encodeModeParameter;

			break;
	}
}

unsigned int H263Encoder::calculateBitrate(unsigned int fpsNum, unsigned int fpsDen, unsigned int frameCount, unsigned int sizeInMb)
{
	double db, ti;

	db = sizeInMb;
	db = db * 1024. * 1024. * 8.;
	// now db is in bits

	// compute duration
	ti = frameCount;
	ti *= fpsDen;
	ti /= fpsNum;	// nb sec
	db = db / ti;

	return (unsigned int)floor(db);
}
