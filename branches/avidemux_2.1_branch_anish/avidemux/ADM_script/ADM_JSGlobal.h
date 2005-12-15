#ifndef _ADM_JSGLOBAL_H
#define _ADM_JSGLOBAL_H

// Spidermonkey
// HACK: Add this to configure later
#include <jsapi.h>
#include <pthread.h>

// javscript debugging helper
void printJSError(JSContext *cx, const char *message, JSErrorReport *report);
bool SpidermonkeyInit();
void SpidermonkeyDestroy();
bool parseECMAScript(const char *name);
void JS_setSuccess(bool bSuccess);
void *StartThreadSpidermonkey(void *pData);
static pthread_t g_pThreadSpidermonkey = NULL;
static pthread_mutex_t g_pSpiderMonkeyMutex = PTHREAD_MUTEX_INITIALIZER;
#endif
