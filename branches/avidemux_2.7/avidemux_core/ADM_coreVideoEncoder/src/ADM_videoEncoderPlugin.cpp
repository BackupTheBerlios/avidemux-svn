#include "ADM_videoEncoderPlugin.h"
#include "ADM_coreVideoEncoderInternal.h"

ADM_videoEncoderPlugin::ADM_videoEncoderPlugin(ADM_LibWrapper *pluginWrapper)
{
	this->_pluginWrapper = pluginWrapper;
	this->_pluginVersion = new PluginVersion();
	memset(this->_pluginVersion, 0, sizeof(PluginVersion));
}

ADM_videoEncoderPlugin::ADM_videoEncoderPlugin(const ADM_videoEncoderDesc *encoderDesc)
{
	this->_pluginWrapper = NULL;
	this->_pluginVersion = new PluginVersion();
	memset(this->_pluginVersion, 0, sizeof(PluginVersion));

	this->_encoderDesc = encoderDesc;
	this->_getUnderlyingLibraryName = NULL;
	this->_getUnderlyingLibraryVersion = NULL;
	this->_pluginVersion->majorVersion = this->_encoderDesc->major;
	this->_pluginVersion->minorVersion = this->_encoderDesc->minor;
	this->_pluginVersion->patchVersion = this->_encoderDesc->patch;
}

ADM_videoEncoderPlugin::~ADM_videoEncoderPlugin()
{
	if (this->_pluginWrapper)
	{
		delete this->_pluginWrapper;
	}
}

ADM_videoEncoderPlugin* ADM_videoEncoderPlugin::loadPlugin(const char *file, ADM_UI_TYPE uiType)
{
	ADM_LibWrapper *pluginWrapper = new ADM_LibWrapper();
	ADM_videoEncoderPlugin *encoder = new ADM_videoEncoderPlugin(pluginWrapper);
	ADM_videoEncoderDesc *(*getInfo)();
	bool initialised = 
		(pluginWrapper->loadLibrary(file) && pluginWrapper->getSymbols(
		3,
		&getInfo, "getInfo",
		&encoder->_getUnderlyingLibraryName, "getUnderlyingLibraryName",
		&encoder->_getUnderlyingLibraryVersion, "getUnderlyingLibraryVersion"));

	if (initialised)
	{
		encoder->_encoderDesc = getInfo();

		if (encoder->_encoderDesc->apiVersion == ADM_VIDEO_ENCODER_API_VERSION)
		{
			if ((encoder->_encoderDesc->UIType & uiType) == uiType)
			{
				printf(
					"[VideoEncoder] Name: %s, API version: %d, Underlying library: %s version %s\n", 
					encoder->_encoderDesc->encoderName, encoder->_encoderDesc->apiVersion, 
					encoder->_getUnderlyingLibraryName(), encoder->_getUnderlyingLibraryVersion());

				encoder->_pluginVersion->majorVersion = encoder->_encoderDesc->major;
				encoder->_pluginVersion->minorVersion = encoder->_encoderDesc->minor;
				encoder->_pluginVersion->patchVersion = encoder->_encoderDesc->patch;
			}
			else
			{
				initialised = false;

				printf(
					"[VideoEncoder] Incorrect UI type.  Plugin: %s, UI type: %d, Expected UI type: %d\n",
					file, encoder->_encoderDesc->UIType, uiType);
			}
		}
		else
		{
			initialised = false;

			printf(
				"[VideoEncoder] Incorrect API version.  Plugin: %s, API version: %d, Expected API version: %d\n",
				file, encoder->_encoderDesc->apiVersion, ADM_VIDEO_ENCODER_API_VERSION);
		}
	}
	else
	{
		printf("[VideoEncoder] Symbol loading failed for %s\n", file);
	}

	if (!initialised)
	{
		delete encoder;
		encoder = NULL;
	}

	return encoder;
}

const char *ADM_videoEncoderPlugin::id()
{
	return this->_encoderDesc->encoderName;
}

const char *ADM_videoEncoderPlugin::name()
{
	return this->_encoderDesc->menuName;
}

int ADM_videoEncoderPlugin::apiVersion()
{
	return this->_encoderDesc->apiVersion;
}

PluginVersion *ADM_videoEncoderPlugin::version()
{
	return this->_pluginVersion;
}

const char *ADM_videoEncoderPlugin::underlyingLibraryName()
{
	return this->_getUnderlyingLibraryName == NULL ? NULL : this->_getUnderlyingLibraryName();
}

const char *ADM_videoEncoderPlugin::underlyingLibraryVersion()
{
	return this->_getUnderlyingLibraryVersion == NULL ? NULL : this->_getUnderlyingLibraryVersion();
}

bool ADM_videoEncoderPlugin::configure(void)
{
	if (this->_encoderDesc->configure)
	{
		return this->_encoderDesc->configure();
	}
	else
	{
		return true;
	}
}

ADM_coreVideoEncoder* ADM_videoEncoderPlugin::createEncoder(ADM_coreVideoFilter *head, bool globalHeader)
{
	return this->_encoderDesc->create(head, globalHeader);
}

const char* ADM_videoEncoderPlugin::description(void)
{
	return this->_encoderDesc->description;
}

void ADM_videoEncoderPlugin::destroyEncoder(ADM_coreVideoEncoder *codec)
{
	return this->_encoderDesc->destroy(codec);
}

bool ADM_videoEncoderPlugin::getConfiguration(CONFcouple **c)
{
	if (this->_encoderDesc->getConfigurationData)
	{
		return this->_encoderDesc->getConfigurationData(c);
	}
	else
	{
		*c = NULL;
		return true;
	}
}

bool ADM_videoEncoderPlugin::setConfiguration(CONFcouple *c, bool full)
{
	if (this->_encoderDesc->setConfigurationData)
	{
		return this->_encoderDesc->setConfigurationData(c, full);
	}
	else
	{
		return true;
	}
}

void ADM_videoEncoderPlugin::resetConfiguration(void)
{
	return this->_encoderDesc->resetConfigurationData();
}

ADM_UI_TYPE ADM_videoEncoderPlugin::UIType(void)
{
	return this->_encoderDesc->UIType;
}