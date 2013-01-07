#include <algorithm>

#include "PluginManager.h"
#include "ADM_files.h"
#include "ADM_dynamicLoading.h"
#include "ADM_dynMuxer.h"
#include "ADM_videoEncoder6.h"
#include "ADM_coreVideoEncoderInternal.h"

const ADM_videoEncoderDesc PluginManager::_copyVideoEncoderDesc = {
	"Copy", "Copy", "Copy encoder", ADM_VIDEO_ENCODER_API_VERSION,
	NULL, NULL, NULL, NULL, NULL, NULL, ADM_UI_ALL, 1, 0, 0, NULL };

PluginManager::PluginManager() {}

PluginManager::~PluginManager()
{
	this->destroyPluginList(this->_muxerList);
}

void PluginManager::destroyPluginList(std::vector<IAdmPlugin*>& pluginList)
{
	for (int pluginIndex = 0; pluginIndex < pluginList.size(); pluginIndex++)
	{
		delete pluginList[pluginIndex];
	}
}

char* PluginManager::pluginDirectory(const char* folderName)
{
#ifdef __APPLE__
    const char *startDir="../lib";
#else
    const char *startDir = ADM_RELATIVE_LIB_DIR;
#endif

	return ADM_getInstallRelativePath(startDir, ADM_PLUGIN_DIR, folderName);
}

std::vector<std::string> PluginManager::pluginFileList(const char* directory)
{
	std::vector<std::string> fileList;
	const int MAX_EXTERNAL_FILTER = 1000;
	char *files[MAX_EXTERNAL_FILTER];
	uint32_t fileCount;

	memset(files, 0, sizeof(char*) * MAX_EXTERNAL_FILTER);

	if (!buildDirectoryContent(&fileCount, directory, files, MAX_EXTERNAL_FILTER, SHARED_LIB_EXT))
	{
		printf("[PluginManager] Cannot query directory %s\n", directory);
	}

	for (uint32_t fileIndex = 0; fileIndex < fileCount; fileIndex++)
	{
		fileList.push_back(files[fileIndex]);
	}

	clearDirectoryContent(fileCount, files);

	return fileList;
}

int PluginManager::pluginIndexById(const char* id, std::vector<IAdmPlugin*>& pluginList)
{
	int index = -1;

	for (int pluginIndex = 0; pluginIndex < pluginList.size(); pluginIndex++)
	{
		if (!strcasecmp(pluginList[pluginIndex]->id(), id))
		{
			index = pluginIndex;
		}
	}

	return index;
}

bool PluginManager::sortPlugins(IAdmPlugin* plugin1, IAdmPlugin* plugin2)
{
	return strcmp(plugin1->name(), plugin2->name()) < 0;
}

void PluginManager::loadAll()
{
	this->loadMuxers();
	this->loadVideoEncoders();
}

void PluginManager::loadMuxers()
{
	char *pluginDir = this->pluginDirectory("muxers");
	std::vector<std::string> pluginFileList = this->pluginFileList(pluginDir);

	delete [] pluginDir;

	for (int pluginFileIndex = 0; pluginFileIndex < pluginFileList.size(); pluginFileIndex++)
	{
		const char* pluginPath = pluginFileList[pluginFileIndex].c_str();
		IAdmPlugin* plugin = ADM_dynMuxer::loadPlugin(pluginFileList[pluginFileIndex].c_str());

		if (plugin != NULL)
		{
			this->_muxerList.push_back(plugin);
		}
	}

	std::sort(this->_muxerList.begin(), this->_muxerList.end(), PluginManager::sortPlugins);
}

void PluginManager::loadVideoEncoders()
{
	char *pluginDir = this->pluginDirectory("videoEncoders");
	std::vector<std::string> pluginFileList = this->pluginFileList(pluginDir);

	delete [] pluginDir;

	for (int pluginFileIndex = 0; pluginFileIndex < pluginFileList.size(); pluginFileIndex++)
	{
		const char* pluginPath = pluginFileList[pluginFileIndex].c_str();
		IAdmPlugin* plugin = ADM_videoEncoder6::loadPlugin(pluginFileList[pluginFileIndex].c_str(), ADM_UI_TYPE_BUILD);

		if (plugin != NULL)
		{
			this->_videoEncoderList.push_back(plugin);
		}
	}

	std::sort(this->_videoEncoderList.begin(), this->_videoEncoderList.end(), PluginManager::sortPlugins);

	this->_videoEncoderList.insert(this->_videoEncoderList.begin(), new ADM_videoEncoder6(&PluginManager::_copyVideoEncoderDesc));
}

const std::vector<IAdmPlugin*>& PluginManager::muxers(void)
{
	return this->_muxerList;
}

int PluginManager::muxerIndex(const char* id)
{
	return this->pluginIndexById(id, this->_muxerList);
}

const std::vector<IAdmPlugin*>& PluginManager::videoEncoders(void)
{
	return this->_videoEncoderList;
}

int PluginManager::videoEncoderIndex(const char* id)
{
	return this->pluginIndexById(id, this->_videoEncoderList);
}