#include "ADM_MuxerPlugin.h"
#include "ADM_muxerInternal.h"

ADM_MuxerPlugin::ADM_MuxerPlugin(ADM_LibWrapper *pluginWrapper)
{	
	this->_pluginWrapper = pluginWrapper;
	this->_pluginVersion = new PluginVersion();
	memset(this->_pluginVersion, 0, sizeof(PluginVersion));
}

ADM_MuxerPlugin* ADM_MuxerPlugin::loadPlugin(const char *file)
{
	ADM_LibWrapper *pluginWrapper = new ADM_LibWrapper();
	ADM_MuxerPlugin *muxer = new ADM_MuxerPlugin(pluginWrapper);
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

const char* ADM_MuxerPlugin::id()
{
	return this->_getMuxerName();
}

const char* ADM_MuxerPlugin::name()
{
	return this->_getDisplayName();
}

int ADM_MuxerPlugin::apiVersion()
{
	return this->_getApiVersion();
}

PluginVersion* ADM_MuxerPlugin::version()
{
	return this->_pluginVersion;
}

const char* ADM_MuxerPlugin::underlyingLibraryName()
{
	return this->_getUnderlyingLibraryName();
}

const char* ADM_MuxerPlugin::underlyingLibraryVersion()
{
	return this->_getUnderlyingLibraryVersion();
}

ADM_MuxerPlugin::~ADM_MuxerPlugin()
{
	delete this->_pluginWrapper;
}

bool ADM_MuxerPlugin::configure(void)
{
	return this->_configure();
}

ADM_muxer* ADM_MuxerPlugin::createMuxer()
{
	return this->_createmuxer();
}

const char* ADM_MuxerPlugin::defaultExtension()
{
	return this->_getDefaultExtension();
}

void ADM_MuxerPlugin::destroyMuxer(ADM_muxer *muxer)
{
	return this->_deletemuxer(muxer);
}

const char* ADM_MuxerPlugin::description()
{
	return this->_getDescriptor();
}

bool ADM_MuxerPlugin::getConfiguration(CONFcouple **conf)
{
	return this->_getConfiguration(conf);
}

bool ADM_MuxerPlugin::setConfiguration(CONFcouple *conf)
{
	return conf == NULL ? true : this->_setConfiguration(conf);
}

bool ADM_MuxerPlugin::resetConfiguration()
{
	return this->_resetConfiguration();
}
