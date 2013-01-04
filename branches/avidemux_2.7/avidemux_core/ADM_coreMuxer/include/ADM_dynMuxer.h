#ifndef ADM_dynMuxer_h
#define ADM_dynMuxer_h

#include "ADM_dynamicLoading.h"
#include "ADM_muxer.h"
#include "IMuxerPlugin.h"

class ADM_dynMuxer : public IMuxerPlugin
{
private:
	ADM_LibWrapper *_pluginWrapper;
	PluginVersion *_pluginVersion;

	const char *(*_getDescriptor)();
	uint32_t (*_getApiVersion)();
	const char *(*_getMuxerName)();
	const char *(*_getDisplayName)();
	const char *(*_getDefaultExtension)();
	uint8_t (*_getVersion)(uint32_t *major, uint32_t *minor, uint32_t *patch);
	const char *(*_getUnderlyingLibraryName)();
	const char *(*_getUnderlyingLibraryVersion)();

	ADM_muxer    *(*_createmuxer)();
	void         (*_deletemuxer)(ADM_muxer *muxer);

	bool  (*_configure)(void);
	bool  (*_getConfiguration)(CONFcouple **conf);
	bool  (*_resetConfiguration)();
	bool  (*_setConfiguration)(CONFcouple *conf);

	ADM_dynMuxer(ADM_LibWrapper *pluginWrapper);

public:
	~ADM_dynMuxer();

	static ADM_dynMuxer* loadPlugin(const char *file);

	const char *id();
	const char *name();
	int apiVersion();
	PluginVersion *version();
	const char *underlyingLibraryName();
	const char *underlyingLibraryVersion();

	bool configure(void);
	ADM_muxer* createMuxer();
	const char* defaultExtension();
	void deleteMuxer(ADM_muxer *muxer);
	const char* descriptor();
	bool getConfiguration(CONFcouple **conf);
	bool setConfiguration(CONFcouple *conf);
	bool resetConfiguration();
};

#endif