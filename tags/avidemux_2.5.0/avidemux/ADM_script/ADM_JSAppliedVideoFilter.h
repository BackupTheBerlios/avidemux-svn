#ifndef _ADM_JSAVIDEMUXVIDEOFILTERCOLLECTION_H
#define _ADM_JSAVIDEMUXVIDEOFILTERCOLLECTION_H

#pragma once

// Spidermonkey
#include "jsapi.h"

class ADM_JSAppliedVideoFilter
{
public:
	virtual ~ADM_JSAppliedVideoFilter(void);
	static JSObject *JSInit(JSContext *cx, JSObject *obj, JSObject *proto, int filterIndex);
	int getFilterIndex(void);

	enum
	{
		argumentsProperty,
		widthProperty,
		heightProperty,
		fps1000Property
	};

protected:
	ADM_JSAppliedVideoFilter(int filterIndex);

private:
	int _filterIndex;

	static JSBool JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
	static void JSDestructor(JSContext *cx, JSObject *obj);

	static JSPropertySpec properties[];
	static JSClass m_appliedVideoFilter;
};

#endif
