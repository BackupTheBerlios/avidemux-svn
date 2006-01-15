#ifndef _ADM_JSGLOBAL_H
#define _ADM_JSGLOBAL_H

// Spidermonkey
// HACK: Add this to configure later
// XXX - HACK
//#define JS_THREADSAFE

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
// expose our main javascript context to the entire program
static bool g_bJSSuccess = 0;
static JSObject *g_pObject;
static JSContext *g_pCx;
static JSRuntime *g_pRt;
static JSClass g_globalClass =
{
	"Global", 0,
	JS_PropertyStub,  JS_PropertyStub,
	JS_PropertyStub, JS_PropertyStub,
	JS_EnumerateStub, JS_ResolveStub,
	JS_ConvertStub,  JS_FinalizeStub
};

#endif
