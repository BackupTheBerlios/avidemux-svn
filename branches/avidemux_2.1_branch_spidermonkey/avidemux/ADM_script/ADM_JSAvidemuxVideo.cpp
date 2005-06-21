#include <stdlib.h>
#include "ADM_JSAvidemuxVideo.h"
#include "ADM_library/default.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_gui2/GUI_ui.h"
#include "ADM_audiofilter/audioeng_buildfilters.h"
#include "avi_vars.h"
#include "gui_action.hxx"
#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_encoder/adm_encoder.h"
#include "ADM_encoder/adm_encConfig.h"
#include "ADM_editor/ADM_outputfmt.h"
#include "ADM_gui2/GUI_ui.h"
#include "ADM_filter/video_filters.h"


typedef struct ADM_CONTAINER
{
    ADM_OUT_FORMAT type;
    const char     *name;
}ADM_CONTAINER;
#define MK_CONT(x) {ADM_##x,#x}

const ADM_CONTAINER container[]=
{
  MK_CONT(AVI),
  MK_CONT(OGM),
  MK_CONT(ES),
  MK_CONT(PS),
  MK_CONT(AVI_DUAL),
  MK_CONT(AVI_UNP),
  MK_CONT(FMT_DUMMY)  
};

extern VF_FILTERS filterGetTagFromName(char *inname);
extern uint8_t indexMpeg(char *mpeg,char *file,uint8_t aid);
extern uint8_t A_ListAllBlackFrames( char *file );
extern uint8_t loadVideoCodecConf( char *name);
extern int ADM_saveRaw (char *name);
extern int A_saveJpg (char *name);
extern uint8_t loadVideoCodecConf( char *name);
extern int videoCodecConfigure(char *p,uint32_t i, uint8_t  *c);

JSPropertySpec ADM_JSAvidemuxVideo::avidemuxvideo_properties[] = 
{ 
	{ 0 }
};

JSFunctionSpec ADM_JSAvidemuxVideo::avidemuxvideo_methods[] = 
{
	{ "Clear", Clear, 0, 0, 0 },
	{ "Add", Add, 3, 0, 0 },
	{ "IndexMPEG", IndexMPEG, 3, 0, 0 },
	{ "AddFilter", AddFilter, 10, 0, 0 },
	{ "Codec", Codec, 3, 0, 0 },
	{ "CodecConf", CodecConf, 1, 0, 0 },
	{ "Save", Save, 1, 0, 0 },
	{ "SaveJPEG", SaveJPEG, 1, 0, 0 },
	{ "ListBlackFrames", ListBlackFrames, 1, 0, 0 },
	{ "PostProcess", PostProcess, 3, 0, 0 },
	{ 0 }
};

JSClass ADM_JSAvidemuxVideo::m_classAvidemuxVideo = 
{
	"AvidemuxVideo", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub,
	ADM_JSAvidemuxVideo::JSGetProperty, ADM_JSAvidemuxVideo::JSSetProperty,
	JS_EnumerateStub, JS_ResolveStub, 
	JS_ConvertStub, ADM_JSAvidemuxVideo::JSDestructor
};

ADM_JSAvidemuxVideo::~ADM_JSAvidemuxVideo(void)
{
	if(m_pObject != NULL)
		delete m_pObject;
	m_pObject = NULL;
}

void ADM_JSAvidemuxVideo::setObject(ADM_AvidemuxVideo *pObject)
{
	m_pObject = pObject; 
}
	
ADM_AvidemuxVideo *ADM_JSAvidemuxVideo::getObject()
{
	return m_pObject; 
}

JSObject *ADM_JSAvidemuxVideo::JSInit(JSContext *cx, JSObject *obj, JSObject *proto)
{
	JSObject *newObj = JS_InitClass(cx, obj, proto, &m_classAvidemuxVideo, 
									ADM_JSAvidemuxVideo::JSConstructor, 0,
									ADM_JSAvidemuxVideo::avidemuxvideo_properties, ADM_JSAvidemuxVideo::avidemuxvideo_methods,
									NULL, NULL);
	return newObj;
}

JSBool ADM_JSAvidemuxVideo::JSConstructor(JSContext *cx, JSObject *obj, uintN argc, 
								 jsval *argv, jsval *rval)
{
	if(argc != 0)
		return JS_FALSE;
	ADM_JSAvidemuxVideo *p = new ADM_JSAvidemuxVideo();
	ADM_AvidemuxVideo *pObject = new ADM_AvidemuxVideo();
	p->setObject(pObject);
	if ( ! JS_SetPrivate(cx, obj, p) )
		return JS_FALSE;
	*rval = OBJECT_TO_JSVAL(obj);
	return JS_TRUE;
}

void ADM_JSAvidemuxVideo::JSDestructor(JSContext *cx, JSObject *obj)
{
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	if(p != NULL)
		delete p;
	p = NULL;
}

JSBool ADM_JSAvidemuxVideo::JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	if (JSVAL_IS_INT(id)) 
	{
		ADM_JSAvidemuxVideo *priv = (ADM_JSAvidemuxVideo *) JS_GetPrivate(cx, obj);
		switch(JSVAL_TO_INT(id))
		{

		}
	}
	return JS_TRUE;
}

JSBool ADM_JSAvidemuxVideo::JSSetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	if (JSVAL_IS_INT(id)) 
	{
		ADM_JSAvidemuxVideo *priv = (ADM_JSAvidemuxVideo *) JS_GetPrivate(cx, obj);
		switch(JSVAL_TO_INT(id))
		{

		}
	}
	return JS_TRUE;
}

JSBool ADM_JSAvidemuxVideo::Clear(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Clear
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 0)
		return JS_FALSE;
	printf("Clearing Video... \n");
	*rval = BOOLEAN_TO_JSVAL(video_body->deleteAllSegments());
	return JS_TRUE;
}// end Clear

JSBool ADM_JSAvidemuxVideo::Add(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Add
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 3)
		return JS_FALSE;
	printf("Adding Video... \n");
	*rval = BOOLEAN_TO_JSVAL(video_body->addSegment(JSVAL_TO_INT(argv[0]),JSVAL_TO_INT(argv[1]),JSVAL_TO_INT(argv[2])));
	return JS_TRUE;
}// end Add

JSBool ADM_JSAvidemuxVideo::IndexMPEG(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin IndexMPEG
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 3)
		return JS_FALSE;
	printf("Indexing MPEG... \n");
	*rval = BOOLEAN_TO_JSVAL(indexMpeg(JS_GetStringBytes(JSVAL_TO_STRING(argv[0])),JS_GetStringBytes(JSVAL_TO_STRING(argv[1])),JSVAL_TO_INT(argv[2])));
	return JS_TRUE;
}// end IndexMPEG

JSBool ADM_JSAvidemuxVideo::AddFilter(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin AddFilter
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc == 0)
		return JS_FALSE;
	VF_FILTERS filter;
	filter = filterGetTagFromName(JS_GetStringBytes(JSVAL_TO_STRING(argv[0])));
	printf("Adding Filter \"%d\"... \n",filter);
	if(filter==VF_DUMMY)
		*rval = BOOLEAN_TO_JSVAL(false);
	else
	{
		printf("unimplented!\n");
//		*rval = BOOLEAN_TO_JSVAL(filterAddScript(filter,argc,argv));
	}
	return JS_TRUE;
}// end AddFilter

JSBool ADM_JSAvidemuxVideo::Codec(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Codec
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc > 3)
		return JS_FALSE;
	printf("Codec ... \n");
	if(!loadVideoCodecConf(JS_GetStringBytes(JSVAL_TO_STRING(argv[2]))))
		*rval = BOOLEAN_TO_JSVAL(false);
	else
	{// begin valid
		char *codec,*conf;
		codec = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
		conf = JS_GetStringBytes(JSVAL_TO_STRING(argv[1]));
		if(!videoCodecSelectByName(codec)) 
			*rval = BOOLEAN_TO_JSVAL(false);
		else
		{// begin conf
			// now do the conf
			// format CBR=bitrate in kbits
			//	  CQ=Q
			//	  2 Pass=size
			// We have to replace
			if(!videoCodecConfigure(conf,0,NULL))
				*rval = BOOLEAN_TO_JSVAL(false);
			else
				*rval = BOOLEAN_TO_JSVAL(true);
		}// end conf
	}// end valid
	return JS_TRUE;
}// end Codec

JSBool ADM_JSAvidemuxVideo::CodecConf(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin CodecConf
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	printf("Codec Conf Video \"%s\"\n",pTempStr);
	*rval = INT_TO_JSVAL(loadVideoCodecConf(pTempStr));
	return JS_TRUE;
}// end CodecConf

JSBool ADM_JSAvidemuxVideo::Save(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Save
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	printf("Saving Video \"%s\"\n",pTempStr);
	*rval = INT_TO_JSVAL(ADM_saveRaw(pTempStr));
	return JS_TRUE;
}// end Save

JSBool ADM_JSAvidemuxVideo::SaveJPEG(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin SaveJPG
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	printf("Saving JPEG \"%s\"\n",pTempStr);
	*rval = INT_TO_JSVAL(A_saveJpg(pTempStr));
	return JS_TRUE;
}// end SaveJPG

JSBool ADM_JSAvidemuxVideo::ListBlackFrames(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin ListBlackFrames
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	A_ListAllBlackFrames(JS_GetStringBytes(JSVAL_TO_STRING(argv[0])));
	*rval = BOOLEAN_TO_JSVAL(true);
	return JS_TRUE;
}// end ListBlackFrames

JSBool ADM_JSAvidemuxVideo::PostProcess(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin PostProcess
	ADM_JSAvidemuxVideo *p = (ADM_JSAvidemuxVideo *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 3)
		return JS_FALSE;
	int rtn = video_body->setPostProc(JSVAL_TO_INT(argv[0]),JSVAL_TO_INT(argv[1]),JSVAL_TO_INT(argv[2]));
	*rval = BOOLEAN_TO_JSVAL(rtn);
	return JS_TRUE;
}// end PostProcess
