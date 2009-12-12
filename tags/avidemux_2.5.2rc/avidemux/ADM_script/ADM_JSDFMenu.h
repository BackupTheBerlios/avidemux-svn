#ifndef _ADM_JSDFMenu_H
#define _ADM_JSDFMenu_H

#include "config.h"
#include "jsapi.h"
#include "DIA_factory.h"
#include <vector>

class ADM_JSDFMenuHelper
{
private:
	char* _title;
	uint32_t _index;
	diaMenuEntry *_menuEntries;
	std::vector <char*> _items;

public:
	ADM_JSDFMenuHelper(const char *title);
	~ADM_JSDFMenuHelper(void);
	void addItem(const char* item);
	diaElem* getControl(void);
	int index(void);
	void setIndex(int index);
};

class ADM_JSDFMenu
{
public:
	ADM_JSDFMenu(void) {}

	static JSBool JSConstructor(JSContext *cx, JSObject *obj, uintN argc, 
								jsval *argv, jsval *rval);
	static void JSDestructor(JSContext *cx, JSObject *obj);
	static JSObject *JSInit(JSContext *cx, JSObject *obj, JSObject *proto = NULL);
	static JSBool JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
	static JSBool JSSetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

	static JSBool addItem(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);	

	static JSPropertySpec properties[];
	static JSFunctionSpec methods[];
	static JSClass m_dfMenuHelper;

	enum
	{
		indexProperty
	};
};

#endif
