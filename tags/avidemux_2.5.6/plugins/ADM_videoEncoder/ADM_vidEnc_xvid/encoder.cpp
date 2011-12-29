/***************************************************************************
                               XvidEncoder.cpp

    begin                : Wed Jun 11 2008
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

#include <math.h>

#include "config.h"
#include "ADM_inttype.h"
#include "ADM_files.h"
#include "encoder.h"
#include "xvidOptions.h"

int avidemuxHook(void *handle, int opt, void *param1, void *param2);
static XvidEncoder encoder;
static void* encoders = { &encoder };
static int supportedCsps[] = { ADM_CSP_YV12 };

extern "C"
{
	void *encoders_getPointer(int uiType) { encoder.setUiType(uiType); return &encoders; } 
	int XvidEncoder_isConfigurable(void) { return encoder.isConfigurable(); }
	int XvidEncoder_configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties) { return encoder.configure(configParameters, properties); }
	int XvidEncoder_getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize) { return encoder.getOptions(encodeOptions, pluginOptions, bufferSize); };
	int XvidEncoder_setOptions(vidEncOptions *encodeOptions, const char *pluginOptions) { return encoder.setOptions(encodeOptions, pluginOptions); };
	int XvidEncoder_getPassCount(void) { return encoder.getPassCount(); }
	int XvidEncoder_getCurrentPass(void) { return encoder.getCurrentPass(); }
	int XvidEncoder_open(vidEncVideoProperties *properties) { return encoder.open(properties); }
	int XvidEncoder_beginPass(vidEncPassParameters *passParameters) { return encoder.beginPass(passParameters); }
	int XvidEncoder_encodeFrame(vidEncEncodeParameters *encodeParams) { return encoder.encodeFrame(encodeParams); }
	int XvidEncoder_finishPass(void) { return encoder.finishPass(); }
	void XvidEncoder_close(void) { encoder.close(); }
}

#ifdef __WIN32
extern void convertPathToAnsi(const char *path, char **ansiPath);
#endif

XvidEncoder::XvidEncoder(void)
{
	_loader = NULL;
	_opened = false;

	_passCount = 1;
	_currentPass = 0;
	_openPass = false;

	_logFileName = NULL;
	_buffer = NULL;

	_encodeOptions.structSize = sizeof(vidEncOptions);
	_encodeOptions.encodeMode = DEFAULT_ENCODE_MODE;
	_encodeOptions.encodeModeParameter = DEFAULT_ENCODE_MODE_PARAMETER;

	xvid_gbl_init_t xvid_gbl_init;
	xvid_gbl_info_t xvid_gbl_info;

	memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init_t));
	memset(&xvid_gbl_info, 0, sizeof(xvid_gbl_info_t));

	printf ("[Xvid] Initialising Xvid\n");

	xvid_gbl_init.version = XVID_VERSION;
	xvid_gbl_info.version = XVID_VERSION;

	xvid_global(NULL, XVID_GBL_INIT, &xvid_gbl_init, NULL);	
	xvid_global(NULL, XVID_GBL_INFO, &xvid_gbl_info, NULL);

	_processors = xvid_gbl_info.num_threads;

	if (xvid_gbl_info.build)
		printf ("[Xvid] Build: %s\n", xvid_gbl_info.build);

	printf ("[Xvid] SIMD supported: (%x)\n", xvid_gbl_info.cpu_flags);

#define PRINT_CPU_FLAG(x) if (xvid_gbl_info.cpu_flags & XVID_CPU_##x) printf("\t\t"#x"\n");

	PRINT_CPU_FLAG(MMX);
	PRINT_CPU_FLAG(MMXEXT);
	PRINT_CPU_FLAG(SSE);
	PRINT_CPU_FLAG(SSE2);

#if XVID_API >= XVID_MAKE_API(4, 2)
	PRINT_CPU_FLAG(SSE3);
	PRINT_CPU_FLAG(SSE41);
#endif

	PRINT_CPU_FLAG(3DNOW);
	PRINT_CPU_FLAG(3DNOWEXT);
	PRINT_CPU_FLAG(ALTIVEC);
}

XvidEncoder::~XvidEncoder(void)
{
	close();

	if (_logFileName)
		delete [] _logFileName;

	if (_buffer)
		delete [] _buffer;
}

void XvidEncoder::setUiType(int uiType)
{
	_uiType = uiType;
}

int XvidEncoder::isConfigurable(void)
{
	return (_uiType == ADM_UI_GTK || _uiType == ADM_UI_QT4);
}

int XvidEncoder::configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties)
{
	if (_loader == NULL)
	{
		char* pluginPath = ADM_getPluginPath();
		const char* configGuiLibName;

		if (_uiType == ADM_UI_GTK)
			configGuiLibName = GTK_PLUGIN_NAME;
		else
			configGuiLibName = QT_PLUGIN_NAME;

		char* configGuiPath = new char[strlen(pluginPath) + 1 + strlen(PLUGIN_CONFIG_DIR) + 1 + strlen(PLUGIN_PREFIX) + strlen(configGuiLibName) + strlen(PLUGIN_SUFFIX) + 1];

		strcpy(configGuiPath, pluginPath);
		strcat(configGuiPath, PLUGIN_CONFIG_DIR);
		strcat(configGuiPath, "/");
		strcat(configGuiPath, PLUGIN_PREFIX);
		strcat(configGuiPath, configGuiLibName);
		strcat(configGuiPath, PLUGIN_SUFFIX);

		_loader = new configGuiLoader(configGuiPath);

		delete [] pluginPath;
		delete [] configGuiPath;
	}

	if (_loader->isAvailable())
	{
		if (_loader->showXvidConfigDialog(configParameters, properties, &_encodeOptions, &_options))
		{
			updateEncodeParameters(NULL);

			return 1;
		}
	}

	return 0;
}

int XvidEncoder::getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize)
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

int XvidEncoder::setOptions(vidEncOptions *encodeOptions, const char *pluginOptions)
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
		updateEncodeParameters(NULL);
	}

	if (success)
		return ADM_VIDENC_ERR_SUCCESS;
	else
		return ADM_VIDENC_ERR_FAILED;
}

int XvidEncoder::getCurrentPass(void)
{
	return _currentPass;
}

int XvidEncoder::getPassCount(void)
{
	return _passCount;
}

int XvidEncoder::open(vidEncVideoProperties *properties)
{
	if (_opened)
		return ADM_VIDENC_ERR_ALREADY_OPEN;

	_opened = true;
	_currentPass = 0;
	_bufferSize = (properties->width * properties->height) + 2 * (((properties->width + 1) >> 1) * ((properties->height + 1) >> 1));
	_buffer = new uint8_t[_bufferSize];

	memcpy(&_properties, properties, sizeof(vidEncVideoProperties));

	if (_options.getParAsInput())
		_options.setPar(_properties.parWidth, _properties.parHeight);

	updateEncodeParameters(&_properties);

	_xvid_enc_create.width = _properties.width;
	_xvid_enc_create.height = _properties.height;
	_xvid_enc_create.fincr = _properties.fpsDen;
	_xvid_enc_create.fbase = _properties.fpsNum;

	if (_options.getThreads() == 0)
		_xvid_enc_create.num_threads = _processors;

	properties->supportedCspsCount = 1;
	properties->supportedCsps = supportedCsps;

	return ADM_VIDENC_ERR_SUCCESS;
}

int XvidEncoder::beginPass(vidEncPassParameters *passParameters)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	if (_openPass)
		return ADM_VIDENC_ERR_PASS_ALREADY_OPEN;

	if (_currentPass == _passCount)
		return ADM_VIDENC_ERR_PASS_COUNT_REACHED;

	if (_passCount > 1 && _currentPass == 0 && passParameters->useExistingLogFile)
	{
		_currentPass++;
		return ADM_VIDENC_ERR_PASS_SKIP;
	}

	_openPass = true;
	_currentPass++;
	_currentFrame = 0;

	printf("[Xvid] begin pass %d/%d\n", _currentPass, _passCount);

	if (_passCount > 1)
	{
		if (_logFileName)
			delete [] _logFileName;

#ifdef __WIN32
		convertPathToAnsi(passParameters->logFileName, &_logFileName);
#else
		_logFileName = new char[strlen(passParameters->logFileName) + 1];
		strcpy(_logFileName, passParameters->logFileName);
#endif

		if (_currentPass == 1)
		{
			_xvid_plugin_2pass1.filename = _logFileName;
			_xvid_enc_plugin[0].func = xvid_plugin_2pass1;
			_xvid_enc_plugin[0].param = &_xvid_plugin_2pass1;

			printf("[Xvid] writing to %s\n", _logFileName);
		}
		else
		{
			_xvid_plugin_2pass2.filename = _logFileName;
			_xvid_enc_plugin[0].func = xvid_plugin_2pass2;
			_xvid_enc_plugin[0].param = &_xvid_plugin_2pass2;

			printf("[Xvid] reading from %s\n", _logFileName);
		}
	}
	else
	{
		_xvid_enc_plugin[0].func = xvid_plugin_single;
		_xvid_enc_plugin[0].param = &_xvid_plugin_single;
	}

	_xvid_enc_plugin[1].func = avidemuxHook;
	_xvid_enc_plugin[1].param = NULL;

	_xvid_enc_create.plugins = _xvid_enc_plugin;
	_xvid_enc_create.num_plugins = 2;

	int err = xvid_encore(NULL, XVID_ENC_CREATE, &_xvid_enc_create, NULL);

	if (err < 0)
	{
		printf ("[Xvid] Init error: %d\n", err);

		return ADM_VIDENC_ERR_FAILED;
	}

	if (_currentPass == 1)
	{
		printEncCreate(&_xvid_enc_create);
		printEncFrame(&_xvid_enc_frame);
	}

	return ADM_VIDENC_ERR_SUCCESS;
}

int XvidEncoder::encodeFrame(vidEncEncodeParameters *encodeParams)
{
	xvid_enc_stats_t xvid_enc_stats;

	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	memset(&xvid_enc_stats, 0, sizeof(xvid_enc_stats_t));

	xvid_enc_stats.version = XVID_VERSION;
	_xvid_enc_frame.bitstream = _buffer;

	if (encodeParams->frameData[0])
	{
		_xvid_enc_frame.length = 0;
		_xvid_enc_frame.input.csp = XVID_CSP_YV12;
		_xvid_enc_frame.input.stride[0] = encodeParams->frameLineSize[0];
		_xvid_enc_frame.input.stride[1] = encodeParams->frameLineSize[1];
		_xvid_enc_frame.input.stride[2] = encodeParams->frameLineSize[2];
		_xvid_enc_frame.input.plane[0] = encodeParams->frameData[0];
		_xvid_enc_frame.input.plane[1] = encodeParams->frameData[1];
		_xvid_enc_frame.input.plane[2] = encodeParams->frameData[2];
	}
	else
	{
		_xvid_enc_frame.length = -1;
		_xvid_enc_frame.input.csp = XVID_CSP_NULL;
	}

	int size = xvid_encore(_xvid_enc_create.handle, XVID_ENC_ENCODE, &_xvid_enc_frame, &xvid_enc_stats);

    if (size < 0)
    {
        printf("[Xvid] Error performing encode %d\n", size);
        return ADM_VIDENC_ERR_FAILED;
    }

	encodeParams->encodedDataSize = size;

	if (_xvid_enc_frame.out_flags & XVID_KEYFRAME)
		encodeParams->frameType = ADM_VIDENC_FRAMETYPE_IDR;
	else if (xvid_enc_stats.type == XVID_TYPE_BVOP)
		encodeParams->frameType = ADM_VIDENC_FRAMETYPE_B;
	else
		encodeParams->frameType = ADM_VIDENC_FRAMETYPE_P;

	encodeParams->quantiser = xvid_enc_stats.quant;
	encodeParams->ptsFrame= _frameNumber;
	encodeParams->encodedData = _buffer;

	return ADM_VIDENC_ERR_SUCCESS;
}

int XvidEncoder::finishPass(void)
{
	if (!_opened)
		return ADM_VIDENC_ERR_CLOSED;

	if (_openPass)
		_openPass = false;

	if (_xvid_enc_create.handle)
	{
		xvid_encore(_xvid_enc_create.handle, XVID_ENC_DESTROY, NULL, NULL);
		_xvid_enc_create.handle = NULL;
	}

	return ADM_VIDENC_ERR_SUCCESS;
}

void XvidEncoder::close(void)
{
	if (_openPass)
		finishPass();

	_opened = false;
	_currentPass = 0;

	if (_buffer)
	{
		delete [] _buffer;
		_buffer = NULL;
	}
}

void XvidEncoder::updateEncodeParameters(vidEncVideoProperties *properties)
{
	xvid_enc_create_t *tmp_xvid_enc_create;
	xvid_enc_frame_t *tmp_xvid_enc_frame;
	xvid_plugin_single_t *tmp_xvid_plugin_single;
	xvid_plugin_2pass2_t *tmp_xvid_plugin_2pass2;

	_options.getParameters(&tmp_xvid_enc_create, &tmp_xvid_enc_frame, &tmp_xvid_plugin_single, &tmp_xvid_plugin_2pass2);

	memcpy(&_xvid_enc_create, tmp_xvid_enc_create, sizeof(xvid_enc_create_t));
	memcpy(&_xvid_enc_frame, tmp_xvid_enc_frame, sizeof(xvid_enc_frame_t));
	memcpy(&_xvid_plugin_single, tmp_xvid_plugin_single, sizeof(xvid_plugin_single_t));
	memset(&_xvid_plugin_2pass1, 0, sizeof(xvid_plugin_2pass1_t));
	memcpy(&_xvid_plugin_2pass2, tmp_xvid_plugin_2pass2, sizeof(xvid_plugin_2pass2_t));

	_xvid_plugin_2pass1.version = XVID_VERSION;

	if (tmp_xvid_enc_frame->quant_intra_matrix)
	{
		memcpy(_intraMatrix, tmp_xvid_enc_frame->quant_intra_matrix, sizeof(unsigned char) * 64);
		_xvid_enc_frame.quant_intra_matrix = _intraMatrix;

		delete tmp_xvid_enc_frame->quant_intra_matrix;
	}

	if (tmp_xvid_enc_frame->quant_inter_matrix)
	{
		memcpy(_interMatrix, tmp_xvid_enc_frame->quant_inter_matrix, sizeof(unsigned char) * 64);
		_xvid_enc_frame.quant_inter_matrix = _interMatrix;

		delete tmp_xvid_enc_frame->quant_inter_matrix;
	}

	delete tmp_xvid_enc_create;
	delete tmp_xvid_enc_frame;
	delete tmp_xvid_plugin_single;
	delete tmp_xvid_plugin_2pass2;

	switch (_encodeOptions.encodeMode)
	{
		case ADM_VIDENC_MODE_CBR:
			_passCount = 1;
			_xvid_plugin_single.bitrate = _encodeOptions.encodeModeParameter * 1000;

			break;
		case ADM_VIDENC_MODE_CQP:
			_passCount = 1;
			_xvid_enc_frame.quant = _encodeOptions.encodeModeParameter;

			break;
		case ADM_VIDENC_MODE_2PASS_SIZE:
			_passCount = 2;

			if (properties)
				_xvid_plugin_2pass2.bitrate = calculateBitrate(properties->fpsNum, properties->fpsDen, properties->frameCount, _encodeOptions.encodeModeParameter);
			else
				_xvid_plugin_2pass2.bitrate = 1500;

			break;
		case ADM_VIDENC_MODE_2PASS_ABR:
			_passCount = 2;
			_xvid_plugin_2pass2.bitrate = _encodeOptions.encodeModeParameter * 1000;

			break;
	}
}

unsigned int XvidEncoder::calculateBitrate(unsigned int fpsNum, unsigned int fpsDen, unsigned int frameCount, unsigned int sizeInMb)
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

void XvidEncoder::setFrameNumber(int frameNumber)
{
	_frameNumber = frameNumber;
}

void XvidEncoder::printEncCreate(xvid_enc_create_t *xvid_enc_create)
{
	printf("[Xvid] # xvid_enc_create #\n");
	printf("[Xvid] version = %d\n", xvid_enc_create->version);
	printf("[Xvid] profile = %d\n", xvid_enc_create->profile);
	printf("[Xvid] width = %d, height = %d\n", xvid_enc_create->width, xvid_enc_create->height);
	printf("[Xvid] num_zones = %d\n", xvid_enc_create->num_zones);
	printf("[Xvid] num_plugins = %d\n", xvid_enc_create->num_plugins);
	printf("[Xvid] num_threads = %d\n", xvid_enc_create->num_threads);
	printf("[Xvid] max_bframes = %d\n", xvid_enc_create->max_bframes);
	printf("[Xvid] global = %d\n", xvid_enc_create->global);
	printf("[Xvid] fincr = %d\n", xvid_enc_create->fincr);
	printf("[Xvid] fbase = %d\n", xvid_enc_create->fbase);
	printf("[Xvid] max_key_interval = %d\n", xvid_enc_create->max_key_interval);
	printf("[Xvid] frame_drop_ratio = %d\n", xvid_enc_create->frame_drop_ratio);
	printf("[Xvid] bquant_ratio = %d\n", xvid_enc_create->bquant_ratio);
	printf("[Xvid] bquant_offset = %d\n", xvid_enc_create->bquant_offset);

	printf("[Xvid] min_quant = ");
	printArray(xvid_enc_create->min_quant, 3);

	printf("\n[Xvid] max_quant = ");
	printArray(xvid_enc_create->max_quant, 3);

	printf("\n");
}

void XvidEncoder::printEncFrame(xvid_enc_frame_t *xvid_enc_frame)
{
	printf("[Xvid] # xvid_enc_frame #\n");
	printf("[Xvid] version = %d\n", xvid_enc_frame->version);
	printf("[Xvid] vol_flags = %d\n", xvid_enc_frame->vol_flags);

	printf("[Xvid] quant_intra_matrix = ");

	if (xvid_enc_frame->quant_intra_matrix)
		printArray(xvid_enc_frame->quant_intra_matrix, 64);
	else
		printf("NULL");

	printf("\n[Xvid] quant_inter_matrix = ");

	if (xvid_enc_frame->quant_inter_matrix)
		printArray(xvid_enc_frame->quant_inter_matrix, 64);
	else
		printf("NULL");

	printf("\n[Xvid] par = %d\n", xvid_enc_frame->par);
	printf("[Xvid] par_width = %d\n", xvid_enc_frame->par_width);
	printf("[Xvid] par_height = %d\n", xvid_enc_frame->par_height);
	printf("[Xvid] fincr = %d\n", xvid_enc_frame->fincr);
	printf("[Xvid] vop_flags = %d\n", xvid_enc_frame->vop_flags);
	printf("[Xvid] motion = %d\n", xvid_enc_frame->motion);
	printf("[Xvid] type = %d\n", xvid_enc_frame->type);
	printf("[Xvid] quant = %d\n", xvid_enc_frame->quant);
	printf("[Xvid] bframe_threshold = %d\n", xvid_enc_frame->bframe_threshold);
}

void XvidEncoder::printArray(const int data[], int size)
{
	for (int index = 0; index < size; index++)
		printf("%d ", data[index]);
}

void XvidEncoder::printArray(const unsigned char data[], int size)
{
	for (int index = 0; index < size; index++)
		printf("%d ", data[index]);
}

int avidemuxHook(void *handle, int opt, void *param1, void *param2)
{
	xvid_plg_data_t *xvid_plg_data = (xvid_plg_data_t*)param1;

	if (opt == XVID_PLG_FRAME)
		encoder.setFrameNumber(xvid_plg_data->frame_num);

	return 0;
}
