#ifndef IPluginManager_h
#define IPluginManager_h

#include <vector>

#include "ADM_core6_export.h"
#include "IAdmPlugin.h"

class ADM_CORE6_EXPORT IPluginManager
{
public:
	~IPluginManager();

	virtual void loadAll() = 0;

	virtual const std::vector<IAdmPlugin*>& muxers(void) = 0;
	virtual int muxerIndex(const char* id) = 0;
};

#endif