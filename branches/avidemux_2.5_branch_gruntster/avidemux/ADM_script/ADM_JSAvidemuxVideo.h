#ifndef _ADM_JSAVIDEMUXVIDEO_H
#define _ADM_JSAVIDEMUXVIDEO_H

#pragma once

// Spidermonkey
#include "jsapi.h"
#include "ADM_AvidemuxVideo.h"

class ADM_JSAvidemuxVideo
{
public:
	virtual ~ADM_JSAvidemuxVideo(void);
	static JSObject *JSInit(JSContext *cx, JSObject *obj, JSObject *proto = NULL);

	enum
	{
		videoProcessProperty,
		widthProperty,
		heightProperty,
		frameCountProperty,
		vopPackedProperty,
		qPelProperty,
		gmcProperty,
		fccProperty,
		fps1000Property,
		appliedFiltersProperty
	};

private:
	ADM_AvidemuxVideo *m_pObject;
	static JSPropertySpec properties[];
	static JSFunctionSpec methods[];
	static JSClass m_classAvidemuxVideo;

	ADM_JSAvidemuxVideo(void) : m_pObject(NULL) {}
	void setObject(ADM_AvidemuxVideo *pObject);
	ADM_AvidemuxVideo *getObject();

	static JSBool JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
	static JSBool JSSetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
	static void JSDestructor(JSContext *cx, JSObject *obj);
	static JSBool Clear(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool Add(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool ClearFilters(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool AddFilter(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool IndexMPEG(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool Codec(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool CodecPlugin(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool CodecConf(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool Save(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool SaveJPEG(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool ListBlackFrames(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool PostProcess(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool RebuildIBFrames(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool getFrameSize(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool getFrameType(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
};

#endif
