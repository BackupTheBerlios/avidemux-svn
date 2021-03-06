#include <stdio.h>

#include "config.h"
#include "ADM_inttype.h"
#include "ADM_files.h"
#include "ADM_encoder/ADM_pluginLoad.h"
#include "ADM_translate.h"
#include "DIA_uiTypes.h"

#ifdef HAVE_GETTEXT
#include <libintl.h>

extern void initGetText(void);

const char* translate(const char *__domainname, const char *__msgid)
{
	return (const char*)dgettext(PACKAGE, __msgid);
}

void initTranslator(void)
{
	initGetText();
	ADM_translateInit(translate);
}
#else
const char* translate(const char *__domainname, const char *__msgid)
{
	return __msgid;
}

void initTranslator(void)
{
	ADM_translateInit(translate);
}
#endif

void getUIDescription(char* desc)
{
	sprintf(desc, "CLI");
}

const char* getNativeRendererDesc(void)
{
	return "";
}

ADM_UI_TYPE UI_GetCurrentUI(void)
{
  return ADM_UI_CLI;
}

void getMainWindowHandles(intptr_t *handle, intptr_t *nativeHandle)
{
	*handle = 0;
	*nativeHandle = 0;
}
