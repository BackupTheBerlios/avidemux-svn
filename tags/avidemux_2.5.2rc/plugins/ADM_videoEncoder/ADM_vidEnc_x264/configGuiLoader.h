#ifndef configGuiLoader_h
#define configGuiLoader_h

#include "ADM_dynamicLoading.h"
#include "x264Options.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

typedef bool _showX264ConfigDialog(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties, vidEncOptions *encodeOptions, x264Options *options);

class configGuiLoader : public ADM_LibWrapper
{
	public:
		_showX264ConfigDialog *showX264ConfigDialog;

		configGuiLoader(const char *file);
};

#endif	// configGuiLoader_h
