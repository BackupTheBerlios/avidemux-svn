#ifndef PluginManager_h
#define PluginManager_h

#include <string>
#include <vector>

#include "IAdmPlugin.h"
#include "IPluginManager.h"
#include "ADM_videoEncoderPlugin.h"

class PluginManager : public IPluginManager
{
private:
	std::vector<IAdmPlugin*> _muxerList, _videoEncoderList;
	static const ADM_videoEncoderDesc _copyVideoEncoderDesc;

	void destroyPluginList(std::vector<IAdmPlugin*>& pluginList);
	bool isValidPlugin(const char* pluginPath, IAdmPlugin* plugin, int expectedApiVersion);
	char* pluginDirectory(const char* folderName);
	std::vector<std::string> pluginFileList(const char* directory);
	int pluginIndexById(const char* id, std::vector<IAdmPlugin*>& pluginList);
	static bool sortPlugins(IAdmPlugin* plugin1, IAdmPlugin* plugin2);

public:
	PluginManager();
	~PluginManager();

	void loadAll();
	void loadMuxers();
	void loadVideoEncoders();

	const std::vector<IAdmPlugin*>& muxers(void);
	int muxerIndex(const char* id);

	const std::vector<IAdmPlugin*>& videoEncoders(void);
	int videoEncoderIndex(const char* id);
};

#endif