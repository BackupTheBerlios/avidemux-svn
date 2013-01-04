#include "ADM_dynMuxer.h"

ADM_dynMuxer::ADM_dynMuxer(ADM_LibWrapper *pluginWrapper)
{
	this->_pluginWrapper = pluginWrapper;
	this->_pluginVersion = new PluginVersion();

	this->_pluginVersion->majorVersion = 1;
	this->_pluginVersion->minorVersion = 0;
	this->_pluginVersion->patchVersion = 0;
	this->_pluginVersion->buildNumber = 0;
}

ADM_dynMuxer* ADM_dynMuxer::loadPlugin(const char *file)
{
	ADM_LibWrapper *pluginWrapper = new ADM_LibWrapper();
	ADM_dynMuxer *muxer = new ADM_dynMuxer(pluginWrapper);

	bool initialised = pluginWrapper->loadLibrary(file) && pluginWrapper->getSymbols(
		14,
		&muxer->_createmuxer, "create",
		&muxer->_deletemuxer, "destroy",
		&muxer->_getMuxerName, "getName",
		&muxer->_getDisplayName, "getDisplayName",
		&muxer->_getApiVersion,  "getApiVersion",
		&muxer->_getVersion,     "getVersion",
		&muxer->_getDescriptor,  "getDescriptor",
		&muxer->_configure,"configure",
		&muxer->_setConfiguration,"setConfiguration",
		&muxer->_getConfiguration,"getConfiguration",
		&muxer->_resetConfiguration,"resetConfiguration",
		&muxer->_getDefaultExtension,"getDefaultExtension",
		&muxer->_getUnderlyingLibraryName, "getUnderlyingLibraryName",
		&muxer->_getUnderlyingLibraryVersion, "getUnderlyingLibraryVersion");

	if (initialised)
	{
		printf(
			"[Muxer] Name: %s, API version: %d, Underlying library: %s %s\n", 
			muxer->_getMuxerName(), muxer->_getApiVersion(), muxer->_getUnderlyingLibraryName(), muxer->_getUnderlyingLibraryVersion());
	}
	else
	{
		printf("[Muxer] Symbol loading failed for %s\n", file);
		delete muxer;
		muxer = NULL;
	}

	return muxer;
}

const char* ADM_dynMuxer::id()
{
	return this->_getMuxerName();
}

const char* ADM_dynMuxer::name()
{
	return this->_getDisplayName();
}

int ADM_dynMuxer::apiVersion()
{
	return this->_getApiVersion();
}

PluginVersion* ADM_dynMuxer::version()
{
	return this->_pluginVersion;
}

const char* ADM_dynMuxer::underlyingLibraryName()
{
	return this->_getUnderlyingLibraryName();
}

const char* ADM_dynMuxer::underlyingLibraryVersion()
{
	return this->_getUnderlyingLibraryVersion();
}

ADM_dynMuxer::~ADM_dynMuxer()
{
	delete this->_pluginWrapper;
}

bool ADM_dynMuxer::configure(void)
{
	return this->_configure();
}

ADM_muxer* ADM_dynMuxer::createMuxer()
{
	return this->_createmuxer();
}

const char* ADM_dynMuxer::defaultExtension()
{
	return this->_getDefaultExtension();
}

void ADM_dynMuxer::deleteMuxer(ADM_muxer *muxer)
{
	return this->_deletemuxer(muxer);
}

const char* ADM_dynMuxer::descriptor()
{
	return this->_getDescriptor();
}

bool ADM_dynMuxer::getConfiguration(CONFcouple **conf)
{
	return this->_getConfiguration(conf);
}

bool ADM_dynMuxer::setConfiguration(CONFcouple *conf)
{
	return this->_setConfiguration(conf);
}

bool ADM_dynMuxer::resetConfiguration()
{
	return this->_resetConfiguration();
}
