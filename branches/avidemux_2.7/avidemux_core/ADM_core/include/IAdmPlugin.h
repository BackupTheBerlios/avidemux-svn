#ifndef IAdmPlugin_h
#define IAdmPlugin_h

#include "ADM_core6_export.h"
#include "PluginVersion.h"

class ADM_CORE6_EXPORT IAdmPlugin
{
public:
	virtual ~IAdmPlugin(void);

	virtual const char *id() = 0;
	virtual const char *name() = 0;
	virtual int apiVersion() = 0;
	virtual PluginVersion *version() = 0;
	virtual const char *underlyingLibraryName() = 0;
	virtual const char *underlyingLibraryVersion() = 0;
};

#endif
