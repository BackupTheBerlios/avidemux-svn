#include "ADM_default.h"
#include "ADM_JSAudioTrackInfo.h"

extern const char *getStrFromAudioCodec(uint32_t codec);

JSPropertySpec ADM_JSAudioTrackInfo::properties[] = 
{
	{ "codec", codecProperty, JSPROP_ENUMERATE },
	{ "bitrate", bitrateProperty, JSPROP_ENUMERATE },
	{ "channelCount", channelCountProperty, JSPROP_ENUMERATE },
	{ "frequency", frequencyProperty, JSPROP_ENUMERATE },
	{ "sourceIndex", sourceIndexProperty, JSPROP_ENUMERATE },
	{ "targetIndex", targetIndexProperty, JSPROP_ENUMERATE },
	{ 0 }
};

JSClass ADM_JSAudioTrackInfo::m_audioTrackInfo = 
{
	"AudioTrackInfo", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub,
	ADM_JSAudioTrackInfo::JSGetProperty, JS_PropertyStub,
	JS_EnumerateStub, JS_ResolveStub, 
	JS_ConvertStub, ADM_JSAudioTrackInfo::JSDestructor
};

ADM_JSAudioTrackInfo::ADM_JSAudioTrackInfo(int sourceIndex, uint32_t encoding, int bitrate, int channelCount, int frequency)
{
	this->sourceIndex = sourceIndex;
	this->targetIndex = -1;
	this->bitrate = bitrate;
	this->channelCount = channelCount;
	this->frequency = frequency;
	this->encoding = encoding;
}

ADM_JSAudioTrackInfo::ADM_JSAudioTrackInfo(int sourceIndex, int targetIndex, uint32_t encoding, int bitrate, int channelCount, int frequency)
{
	this->sourceIndex = sourceIndex;
	this->targetIndex = targetIndex;
	this->bitrate = bitrate;
	this->channelCount = channelCount;
	this->frequency = frequency;
	this->encoding = encoding;
}

ADM_JSAudioTrackInfo::~ADM_JSAudioTrackInfo(void)
{

}

JSObject *ADM_JSAudioTrackInfo::JSInit(JSContext *cx, JSObject *obj, JSObject *proto, int sourceIndex, audioInfo* info)
{
	JSObject *newObj = JS_InitClass(cx, obj, proto, &m_audioTrackInfo, NULL,
		0, ADM_JSAudioTrackInfo::properties, NULL, NULL, NULL);
	ADM_JSAudioTrackInfo *p = new ADM_JSAudioTrackInfo(
		sourceIndex, info->encoding, info->bitrate, info->channels, info->frequency);

	JS_SetPrivate(cx, newObj, p);

	return newObj;
}

JSObject *ADM_JSAudioTrackInfo::JSInit(JSContext *cx, JSObject *obj, JSObject *proto, int sourceIndex, int targetIndex, WAVHeader* header)
{
	JSObject *newObj = JS_InitClass(
		cx, obj, proto, &m_audioTrackInfo, NULL, 0, ADM_JSAudioTrackInfo::properties, NULL, NULL, NULL);
	ADM_JSAudioTrackInfo *p = new ADM_JSAudioTrackInfo(
		sourceIndex, targetIndex, header->encoding, (header->byterate * 8) / 1000, 
		header->channels, header->frequency);

	JS_SetPrivate(cx, newObj, p);

	return newObj;
}

void ADM_JSAudioTrackInfo::JSDestructor(JSContext *cx, JSObject *obj)
{

}

JSBool ADM_JSAudioTrackInfo::JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	ADM_JSAudioTrackInfo *p = (ADM_JSAudioTrackInfo *)JS_GetPrivate(cx, obj);

	if (JSVAL_IS_INT(id))
	{
		switch(JSVAL_TO_INT(id))
		{
			case sourceIndexProperty:
			{
				*vp = INT_TO_JSVAL(p->sourceIndex);
				break;
			}
			case targetIndexProperty:
			{
				*vp = INT_TO_JSVAL(p->targetIndex);
				break;
			}
			case codecProperty:
			{
				*vp = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, getStrFromAudioCodec(p->encoding)));
				break;
			}
			case bitrateProperty:
			{
				*vp = INT_TO_JSVAL(p->bitrate);
				break;
			}
			case channelCountProperty:
			{
				*vp = INT_TO_JSVAL(p->channelCount);
				break;
			}
			case frequencyProperty:
			{
				*vp = INT_TO_JSVAL(p->frequency);
				break;
			}
		}
	}

	return JS_TRUE;
}
