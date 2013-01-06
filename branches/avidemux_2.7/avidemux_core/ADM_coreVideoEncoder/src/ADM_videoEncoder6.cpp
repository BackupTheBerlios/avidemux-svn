#include "ADM_videoEncoder6.h"

ADM_videoEncoder6::ADM_videoEncoder6(ADM_LibWrapper *pluginWrapper)
{
	this->_pluginWrapper = pluginWrapper;
	this->_pluginVersion = new PluginVersion();
	memset(this->_pluginVersion, 0, sizeof(PluginVersion));
}

ADM_videoEncoder6::ADM_videoEncoder6(ADM_videoEncoderDesc *encoderDesc)
{
	this->_pluginWrapper = NULL;
	this->_pluginVersion = new PluginVersion();
	memset(this->_pluginVersion, 0, sizeof(PluginVersion));

	this->_encoderDesc = encoderDesc;
	this->_pluginVersion->majorVersion = this->_encoderDesc->major;
	this->_pluginVersion->minorVersion = this->_encoderDesc->minor;
	this->_pluginVersion->patchVersion = this->_encoderDesc->patch;

}

ADM_videoEncoder6::~ADM_videoEncoder6()
{
	if (this->_pluginWrapper)
	{
		delete this->_pluginWrapper;
	}
}

ADM_videoEncoder6* ADM_videoEncoder6::loadPlugin(const char *file)
{
	ADM_LibWrapper *pluginWrapper = new ADM_LibWrapper();
	ADM_videoEncoder6 *encoder = new ADM_videoEncoder6(pluginWrapper);
	bool initialised = (pluginWrapper->loadLibrary(file) && pluginWrapper->getSymbols(1, &encoder->_getInfo, "getInfo"));

	if (initialised)
	{
		encoder->_encoderDesc = encoder->_getInfo();
		encoder->_pluginVersion->majorVersion = encoder->_encoderDesc->major;
		encoder->_pluginVersion->minorVersion = encoder->_encoderDesc->minor;
		encoder->_pluginVersion->patchVersion = encoder->_encoderDesc->patch;

		printf(
			"[VideoEncoder] Name: %s, API version: %d, Underlying library: %s %s\n", 
			encoder->_encoderDesc->encoderName, encoder->_encoderDesc->apiVersion, NULL, NULL);
	}
	else
	{
		printf("[VideoEncoder] Symbol loading failed for %s\n", file);
		delete encoder;
		encoder = NULL;
	}

	return encoder;
}

const char *ADM_videoEncoder6::id()
{
	return this->_encoderDesc->encoderName;
}

const char *ADM_videoEncoder6::name()
{
	return this->_encoderDesc->menuName;
}

int ADM_videoEncoder6::apiVersion()
{
	return this->_encoderDesc->apiVersion;
}

PluginVersion *ADM_videoEncoder6::version()
{
	return this->_pluginVersion;
}

const char *ADM_videoEncoder6::underlyingLibraryName()
{
	return NULL;
}

const char *ADM_videoEncoder6::underlyingLibraryVersion()
{
	return NULL;
}

bool ADM_videoEncoder6::configure(void)
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

ADM_coreVideoEncoder* ADM_videoEncoder6::createEncoder(ADM_coreVideoFilter *head, bool globalHeader)
{
	return this->_encoderDesc->create(head, globalHeader);
}

const char* ADM_videoEncoder6::description(void)
{
	return this->_encoderDesc->description;
}

void ADM_videoEncoder6::destroyEncoder(ADM_coreVideoEncoder *codec)
{
	return this->_encoderDesc->destroy(codec);
}

bool ADM_videoEncoder6::getConfiguration(CONFcouple **c)
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

bool ADM_videoEncoder6::setConfiguration(CONFcouple *c, bool full)
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

void ADM_videoEncoder6::resetConfiguration(void)
{
	return this->_encoderDesc->resetConfigurationData();
}

ADM_UI_TYPE ADM_videoEncoder6::UIType(void)
{
	return this->_encoderDesc->UIType;
}