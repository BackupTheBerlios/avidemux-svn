#ifndef IMuxerPlugin_h
#define IMuxerPlugin_h

#include "ADM_coreMuxer6_export.h"
#include "IAdmPlugin.h"
#include "ADM_muxer.h"
#include "ADM_confCouple.h"

class ADM_COREMUXER6_EXPORT IMuxerPlugin : public IAdmPlugin
{
public:
	virtual ~IMuxerPlugin(void);

	virtual const char *id() = 0;
	virtual const char *name() = 0;
	virtual int apiVersion() = 0;
	virtual PluginVersion *version() = 0;
	virtual const char *underlyingLibraryName() = 0;
	virtual const char *underlyingLibraryVersion() = 0;

	virtual bool configure(void) = 0;
	virtual ADM_muxer* createMuxer() = 0;
	virtual const char* defaultExtension() = 0;
	virtual void deleteMuxer(ADM_muxer *muxer) = 0;
	virtual const char* descriptor() = 0;
	virtual bool getConfiguration(CONFcouple **conf) = 0;
	virtual bool setConfiguration(CONFcouple *conf) = 0;
	virtual bool resetConfiguration() = 0;
};

#endif