#include <stdio.h>
#include <gtk/gtk.h>

#ifdef __WIN32
#include <gdk/gdkwin32.h>
#elif defined(__APPLE__)
extern "C" int getMainNSWindow(void);
#else
#include <gdk/gdkx.h>
#endif

#include "config.h"
#include "ADM_inttype.h"
#include "ADM_files.h"
#include "ADM_encoder/ADM_pluginLoad.h"
#include "ADM_translate.h"

extern GtkWidget *guiRootWindow;

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
	sprintf(desc, "GTK+ (%i.%i.%i)", gtk_major_version, gtk_minor_version, gtk_micro_version);
}

ADM_UI_TYPE UI_GetCurrentUI(void)
{
  return ADM_UI_GTK;
}

const char* getNativeRendererDesc(void)
{
	return QT_TR_NOOP("GTK+");
}

void getMainWindowHandles(intptr_t *handle, intptr_t *nativeHandle)
{
	*handle = (intptr_t)guiRootWindow;

#ifdef __WIN32
	*nativeHandle = (intptr_t)guiRootWindow->window;
#elif defined(__APPLE__)
	*nativeHandle = (intptr_t)getMainNSWindow();
#else
	*nativeHandle = (intptr_t)guiRootWindow->window;
#endif
}
