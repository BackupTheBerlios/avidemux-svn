#ifndef _ADM_JSDialogFactory_H
#define _ADM_JSDialogFactory_H

#include "config.h"
#include "jsapi.h"
#include "DIA_factory.h"
#include "ADM_JSDFMenu.h"
#include <vector>

class ADM_JSDialogFactoryHelper
{
private:
	char* _title;
	std::vector <ADM_JSDFMenuHelper*> _controls;

public:
	ADM_JSDialogFactoryHelper(const char *title);
	~ADM_JSDialogFactoryHelper(void);
	void addControl(ADM_JSDFMenuHelper* control);
	diaElem** getControls(int *controlCount);
	const char* title(void);
};

class ADM_JSDialogFactory
{
public:
	ADM_JSDialogFactory(void) {}
	virtual ~ADM_JSDialogFactory(void);

	static JSBool JSConstructor(JSContext *cx, JSObject *obj, uintN argc, 
								jsval *argv, jsval *rval);
	static void JSDestructor(JSContext *cx, JSObject *obj);
	static JSObject *JSInit(JSContext *cx, JSObject *obj, JSObject *proto = NULL);
	static JSBool addControl(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool show(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

	static JSFunctionSpec methods[];
	static JSClass m_dialogFactoryHelper;
};

#endif
