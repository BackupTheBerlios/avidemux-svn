#include "ADM_dynMuxer.h"
#include "ADM_muxerInternal.h"

ADM_dynMuxer::ADM_dynMuxer(ADM_LibWrapper *pluginWrapper)
{	
	this->_pluginWrapper = pluginWrapper;
	this->_pluginVersion = new PluginVersion();
	memset(this->_pluginVersion, 0, sizeof(PluginVersion));
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
		if (muxer->_getApiVersion() == ADM_MUXER_API_VERSION)
		{
			printf(
				"[Muxer] Name: %s, API version: %d, Underlying library: %s version %s\n", 
				muxer->_getMuxerName(), muxer->_getApiVersion(), muxer->_getUnderlyingLibraryName(), muxer->_getUnderlyingLibraryVersion());

			uint32_t majorVersion, minorVersion, patchVersion;

			muxer->_getVersion(&majorVersion, &minorVersion, &patchVersion);
			muxer->_pluginVersion->majorVersion = majorVersion;
			muxer->_pluginVersion->minorVersion = minorVersion;
			muxer->_pluginVersion->patchVersion = patchVersion;
			muxer->_pluginVersion->buildNumber = 0;
		}
		else
		{
			initialised = false;

			printf(
				"[Muxer] Incorrect API version.  Plugin: %s, API version: %d, Expected API version: %d\n",
				file, muxer->_getApiVersion(), ADM_MUXER_API_VERSION);
		}
	}
	else
	{
		printf("[Muxer] Symbol loading failed for %s\n", file);
	}

	if (!initialised)
	{
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

void ADM_dynMuxer::destroyMuxer(ADM_muxer *muxer)
{
	return this->_deletemuxer(muxer);
}

const char* ADM_dynMuxer::description()
{
	return this->_getDescriptor();
}

bool ADM_dynMuxer::getConfiguration(CONFcouple **conf)
{
	return this->_getConfiguration(conf);
}

bool ADM_dynMuxer::setConfiguration(CONFcouple *conf)
{
	return conf == NULL ? true : this->_setConfiguration(conf);
}

bool ADM_dynMuxer::resetConfiguration()
{
	return this->_resetConfiguration();
}
