#ifndef _ADM_JSAVIDEMUXAUDIO_H
#define _ADM_JSAVIDEMUXAUDIO_H

#pragma once

#include "config.h"

// Spidermonkey
#include "jsapi.h"
#include "ADM_AvidemuxAudio.h"
#include "avi_vars.h"
#include "ADM_audiofilter/audioeng_buildfilters.h"

class ADM_JSAvidemuxAudio
{
public:
	virtual ~ADM_JSAvidemuxAudio(void);
	static JSObject *JSInit(JSContext *cx, JSObject *obj, JSObject *proto = NULL);

	enum
	{
		audioprocess_prop,
		resample_prop,
		delay_prop,
		film2pal_prop,
		pal2film_prop,
		normalizemode_prop,
		normalizevalue_prop,
		drc_prop,
		mixerProperty,
		sourceTrackInfoProperty,
		targetTrackInfoProperty
	};

private:
	ADM_AvidemuxAudio *m_pObject;
	static JSPropertySpec properties[];
	static JSFunctionSpec methods[];
	static JSClass m_classAvidemuxAudio;

	ADM_JSAvidemuxAudio(void) : m_pObject(NULL) {}
	void setObject(ADM_AvidemuxAudio *pObject);
	ADM_AvidemuxAudio *getObject();

	static JSBool JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
	static JSBool JSSetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
	static void JSDestructor(JSContext *cx, JSObject *obj);
	static JSBool ScanVBR(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool Save(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool Load(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool Reset(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool Codec(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static void AddAudioSourceToJSArray(JSContext *cx, JSObject *obj, int trackIndex, AudioSource audioSource, AVDMGenericAudioStream *audioStream, JSObject *tracks);
	static JSBool setTrack(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	static JSBool secondAudioTrack(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
};

#endif
