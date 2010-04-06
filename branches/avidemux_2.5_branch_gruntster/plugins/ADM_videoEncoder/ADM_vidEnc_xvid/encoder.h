/***************************************************************************
                                  encoder.h

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

#ifndef encoder_h
#define encoder_h

#ifdef __cplusplus
	#include "configGuiLoader.h"

	extern "C"
	{
	#include "ADM_vidEnc_plugin.h"
	}

	class XvidEncoder
	{
	private:
		int _uiType;

		configGuiLoader *_loader;
		XvidOptions _options;
		vidEncOptions _encodeOptions;
		vidEncVideoProperties _properties;
		char *_logFileName;
		int _frameNumber;
		int _processors;

		uint8_t *_buffer;
		int _bufferSize;

		xvid_enc_create_t _xvid_enc_create;
		xvid_enc_frame_t _xvid_enc_frame;
		xvid_plugin_single_t _xvid_plugin_single;
		xvid_plugin_2pass1_t _xvid_plugin_2pass1;
		xvid_plugin_2pass2_t _xvid_plugin_2pass2;
		xvid_enc_plugin_t _xvid_enc_plugin[2];

		unsigned char _intraMatrix[64];
		unsigned char _interMatrix[64];

		unsigned int _currentFrame;
		int _currentPass, _passCount;
		bool _opened, _openPass;

		void updateEncodeParameters(vidEncVideoProperties *properties);
		unsigned int calculateBitrate(unsigned int fpsNum, unsigned int fpsDen, unsigned int frameCount, unsigned int sizeInMb);

		void printEncCreate(xvid_enc_create_t *xvid_enc_create);
		void printEncFrame(xvid_enc_frame_t *xvid_enc_frame);
		void printArray(const int data[], int size);
		void printArray(const unsigned char data[], int size);

	public:
		XvidEncoder(void);
		~XvidEncoder(void);
		void setUiType(int uiType);
		int isConfigurable(void);
		int configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
		int getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
		int setOptions(vidEncOptions *encodeOptions, const char *pluginOptions);
		int getCurrentPass(void);
		int getPassCount(void);
		int open(vidEncVideoProperties *properties);
		int beginPass(vidEncPassParameters *passParameters);
		int encodeFrame(vidEncEncodeParameters *encodeParams);
		int finishPass(void);
		void close(void);
		void setFrameNumber(int frameNumber);
	};
#else
	void *encoders_getPointer(int uiType);
	int XvidEncoder_isConfigurable(void);
	int XvidEncoder_configure(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties);
	int XvidEncoder_getOptions(vidEncOptions *encodeOptions, char *pluginOptions, int bufferSize);
	int XvidEncoder_setOptions(vidEncOptions *encodeOptions, const char *pluginOptions);
	int XvidEncoder_getPassCount(void);
	int XvidEncoder_getCurrentPass(void);
	int XvidEncoder_open(vidEncVideoProperties *properties);
	int XvidEncoder_beginPass(vidEncPassParameters *passParameters);
	int XvidEncoder_encodeFrame(vidEncEncodeParameters *encodeParams);
	int XvidEncoder_finishPass(void);
	void XvidEncoder_close(void);
#endif	// __cplusplus
#endif	// encoder_h
