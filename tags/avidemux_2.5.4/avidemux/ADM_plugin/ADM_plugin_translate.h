#ifndef ADM_plugin_translate_h
#define ADM_plugin_translate_h

#ifdef QT_TR_NOOP
#	define QT_TR_NOOP
#	error Qt translation functions already exist.  Bespoke functions not required.
#endif

typedef const char *TranslateFunction(const char *__domainname, const char *__msgid);
extern const char *ADM_translate(const char *__domainname, const char *__msgid);

#define QT_TR_NOOP(String) ADM_translate("avidemux", String)

#endif