#ifndef IVideoEncoderPlugin_h
#define IVideoEncoderPlugin_h

#include "ADM_coreVideoEncoder6_export.h"
#include "IAdmPlugin.h"
#include "ADM_coreVideoEncoder.h"

class ADM_COREVIDEOENCODER6_EXPORT IVideoEncoderPlugin : public IAdmPlugin
{
public:
	virtual ~IVideoEncoderPlugin(void);

	virtual const char *id() = 0;
	virtual const char *name() = 0;
	virtual int apiVersion() = 0;
	virtual PluginVersion *version() = 0;
	virtual const char *underlyingLibraryName() = 0;
	virtual const char *underlyingLibraryVersion() = 0;

	virtual bool configure(void) = 0;
	virtual ADM_coreVideoEncoder* createEncoder(ADM_coreVideoFilter *head, bool globalHeader) = 0;
	virtual const char* description() = 0;
    virtual void destroyEncoder(ADM_coreVideoEncoder *codec) = 0;
    virtual bool getConfiguration(CONFcouple **c) = 0;
    virtual bool setConfiguration(CONFcouple *c, bool full) = 0;
	virtual void resetConfiguration() = 0;

	virtual ADM_UI_TYPE UIType(void) = 0;
};

#endif