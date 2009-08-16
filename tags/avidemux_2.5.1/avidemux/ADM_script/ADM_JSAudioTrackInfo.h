#ifndef _ADM_JSAUDIOTRACKINFO_H
#define _ADM_JSAUDIOTRACKINFO_H

#include "jsapi.h"
#include "ADM_editor/ADM_Video.h"

class ADM_JSAudioTrackInfo
{
public:
	virtual ~ADM_JSAudioTrackInfo(void);
	static JSObject *JSInit(JSContext *cx, JSObject *obj, JSObject *proto, int sourceIndex, audioInfo* info);
	static JSObject *JSInit(JSContext *cx, JSObject *obj, JSObject *proto, int sourceIndex, int targetIndex, WAVHeader* header);

	enum
	{
		codecProperty,
		bitrateProperty,
		channelCountProperty,
		frequencyProperty,
		sourceIndexProperty,
		targetIndexProperty
	};

protected:
	ADM_JSAudioTrackInfo(int sourceIndex, uint32_t encoding, int bitrate, int channelCount, int frequency);
	ADM_JSAudioTrackInfo(int sourceIndex, int targetIndex, uint32_t encoding, int bitrate, int channelCount, int frequency);

private:
	int bitrate;
	int channelCount;
	int frequency;
	int sourceIndex, targetIndex;
	uint32_t encoding;

	static JSBool JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
	static void JSDestructor(JSContext *cx, JSObject *obj);

	static JSPropertySpec properties[];
	static JSClass m_audioTrackInfo;
};

#endif

