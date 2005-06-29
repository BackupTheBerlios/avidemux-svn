// C++ Interface: Spider Monkey interface
//
// Description: 
//
//
// Author: Anish Mistry
//      Some modification by mean
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "config.h"
#include <stdlib.h>
#include <math.h>
#include "ADM_JSAvidemux.h"
#include "ADM_library/default.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_gui2/GUI_ui.h"
#include "ADM_editor/ADM_outputfmt.h"
#include "ADM_audiofilter/audioeng_buildfilters.h"
#include "adm_scanner.h" 
#include "avi_vars.h"
#include "gui_action.hxx"
#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_encoder/adm_encoder.h"
#include "ADM_encoder/adm_encConfig.h"
#include "ADM_editor/ADM_outputfmt.h"
#include "ADM_gui2/GUI_ui.h"
#include "ADM_filter/video_filters.h"
#include "ADM_script/ADM_container.h"

#include "ADM_toolkit/ADM_debugID.h"
#define MODULE_NAME MODULE_SCRIPT
#include "ADM_toolkit/ADM_debug.h"


extern int A_openAvi (char *name);
extern int A_Save (char *name);
extern int A_appendAvi (char *name);
extern int A_saveDVDPS(char *name);
extern int ogmSave(char *name);
extern int GUI_GoToFrame(uint32_t frame);
extern int filterLoadXml(char *docname,uint8_t silent);
extern int A_delete(uint32_t start, uint32_t end);

extern uint8_t A_ListAllBlackFrames( char *file );
extern uint8_t A_jumpToTime(uint32_t hh,uint32_t mm,uint32_t ss);
extern uint8_t addFile(char *name);


JSPropertySpec ADM_JSAvidemux::avidemux_properties[] = 
{ 

	{ "videoProcess", videoProcess_prop, JSPROP_ENUMERATE },	// process video when saving
	{ "audioProcess", audioProcess_prop, JSPROP_ENUMERATE },	// process audio when saving
	{ "markerA", markerA_prop, JSPROP_ENUMERATE },	// set marker A
	{ "markerB", markerB_prop, JSPROP_ENUMERATE },	// set marker B
	{ "audio", audio_prop, JSPROP_ENUMERATE },	// audio object
	{ "video", video_prop, JSPROP_ENUMERATE },	// video object
	{ "container", container_prop, JSPROP_ENUMERATE },	// set container type
	{ "currentFrame", currentframe_prop, JSPROP_ENUMERATE },	// set current frame
	{ "fps", fps_prop, JSPROP_ENUMERATE },	// set movie frame rate
	{ 0 }
};

JSFunctionSpec ADM_JSAvidemux::avidemux_methods[] = 
{
	{ "append", Append, 1, 0, 0 },	// append video
	{ "delete", Delete, 2, 0, 0 },	// delete section
	{ "exit", Exit, 0, 0, 0 },	// exit Avidemux
	{ "load", Load, 1, 0, 0 },	// Load movie
	{ "loadFilters", LoadFilters, 1, 0, 0 },	// Load filters from file
	{ "save", Save, 1, 0, 0 },	// Save movie
	{ "saveDVD", SaveDVD, 1, 0, 0 },	// Save movie as DVD
	{ "saveOGM", SaveOGM, 1, 0, 0 },	// Save movie as OGM
        { "clearSegments", ClearSegments ,0,0,0}, // Clear all segments
        { "addSegment", AddSegment ,3,0,0}, // Clear all segments
	{ "goToTime", GoToTime, 3, 0, 0 },	// more current frame to time index
	{ 0 }
};

JSClass ADM_JSAvidemux::m_classAvidemux = 
{
	"Avidemux", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub,
	ADM_JSAvidemux::JSGetProperty, ADM_JSAvidemux::JSSetProperty,
	JS_EnumerateStub, JS_ResolveStub, 
	JS_ConvertStub, ADM_JSAvidemux::JSDestructor
};

ADM_JSAvidemux::~ADM_JSAvidemux(void)
{
	if(m_pObject != NULL)
		delete m_pObject;
	m_pObject = NULL;
}

void ADM_JSAvidemux::setObject(ADM_Avidemux *pObject)
{
	m_pObject = pObject; 
}
	
ADM_Avidemux *ADM_JSAvidemux::getObject()
{
	return m_pObject; 
}

JSObject *ADM_JSAvidemux::JSInit(JSContext *cx, JSObject *obj, JSObject *proto)
{
	JSObject *newObj = JS_InitClass(cx, obj, proto, &m_classAvidemux, 
									ADM_JSAvidemux::JSConstructor, 0,
									ADM_JSAvidemux::avidemux_properties, ADM_JSAvidemux::avidemux_methods,
									NULL, NULL);
	return newObj;
}

JSBool ADM_JSAvidemux::JSConstructor(JSContext *cx, JSObject *obj, uintN argc, 
								 jsval *argv, jsval *rval)
{
	if(argc != 0)
		return JS_FALSE;
	ADM_JSAvidemux *p = new ADM_JSAvidemux();
	ADM_Avidemux *pObject = new ADM_Avidemux();
	p->setObject(pObject);
	if ( ! JS_SetPrivate(cx, obj, p) )
		return JS_FALSE;
	*rval = OBJECT_TO_JSVAL(obj);
	return JS_TRUE;
}

void ADM_JSAvidemux::JSDestructor(JSContext *cx, JSObject *obj)
{
	ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
	if(p != NULL)
		delete p;
	p = NULL;
}

JSBool ADM_JSAvidemux::JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	if (JSVAL_IS_INT(id)) 
	{
		ADM_JSAvidemux *priv = (ADM_JSAvidemux *) JS_GetPrivate(cx, obj);
		switch(JSVAL_TO_INT(id))
		{
			case videoProcess_prop:
				*vp = BOOLEAN_TO_JSVAL(priv->getObject()->m_bVideoProcess);
				break;
			case audioProcess_prop:
				*vp = BOOLEAN_TO_JSVAL(priv->getObject()->m_bAudioProcess);
				break;
			case markerA_prop:
				*vp = INT_TO_JSVAL(frameStart);
				break;
			case markerB_prop:
				*vp = INT_TO_JSVAL(frameEnd);
				break;
			case audio_prop:
				*vp = OBJECT_TO_JSVAL(priv->getObject()->m_pAudio);
				break;
			case video_prop:
				*vp = OBJECT_TO_JSVAL(priv->getObject()->m_pVideo);
				break;
			case container_prop:
				*vp = STRING_TO_JSVAL(priv->getObject()->m_pContainer);
				break;
			case currentframe_prop:
				*vp = INT_TO_JSVAL(priv->getObject()->m_nCurrentFrame);
				break;
			case fps_prop:
				{
					aviInfo info;

					if (avifileinfo)
					{
						video_body->getVideoInfo(&info);
						priv->getObject()->m_dFPS = info.fps1000/1000.0; (uint32_t)floor(priv->getObject()->m_dFPS*1000.f);
						video_body->updateVideoInfo (&info);
						video_body->getVideoInfo (avifileinfo);
					} 
					else 
					{
						return JS_FALSE;
					}
					*vp = DOUBLE_TO_JSVAL(priv->getObject()->m_dFPS);
				}
				break;
		}
	}
	return JS_TRUE;
}

JSBool ADM_JSAvidemux::JSSetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	if (JSVAL_IS_INT(id)) 
	{
		ADM_JSAvidemux *priv = (ADM_JSAvidemux *) JS_GetPrivate(cx, obj);
		switch(JSVAL_TO_INT(id))
		{
			case videoProcess_prop:
				priv->getObject()->m_bVideoProcess = JSVAL_TO_BOOLEAN(*vp);
				UI_setVProcessToggleStatus(priv->getObject()->m_bVideoProcess);
				break;
			case audioProcess_prop:
				priv->getObject()->m_bAudioProcess = JSVAL_TO_BOOLEAN(*vp);
				UI_setAProcessToggleStatus(priv->getObject()->m_bAudioProcess);
				break;
			case markerA_prop:
				{
					int f=JSVAL_TO_INT(*vp);
					if (!avifileinfo)
					{
						return JS_FALSE;
					} 
					if(f==-1)
						f=avifileinfo->nb_frames-1;
					if(f<0 || f>avifileinfo->nb_frames-1)
						return JS_FALSE;
					frameStart=f;
				}
				break;
			case markerB_prop:
				{
					int f=JSVAL_TO_INT(*vp);
					if (!avifileinfo)
					{
						return JS_FALSE;
					} 
					if(f==-1)
						f=avifileinfo->nb_frames-1;
					if(f<0 || f>avifileinfo->nb_frames-1)
						return JS_FALSE;
					frameEnd=f;
				}
				break;
			case audio_prop:
				return JS_FALSE;
				break;
			case video_prop:
				return JS_FALSE;
				break;
			case container_prop:
				{
					priv->getObject()->m_pContainer = JSVAL_TO_STRING(*vp);
					char *pContainer = JS_GetStringBytes(priv->getObject()->m_pContainer);
					aprintf("Setting container format \"%s\"\n",pContainer);
					for(int i=0;i<NB_CONT;i++)
					{
						printf("%s\n",container[i].name);
						if(!strcasecmp(pContainer,container[i].name))
						{
							UI_SetCurrentFormat(container[i].type);
							return JS_TRUE;
						}
					}
					printf("Cannot set output format \"%s\"\n",pContainer);
					return JS_FALSE;
				}
				break;
			case currentframe_prop:
				{
					int frameno;
					if (!avifileinfo)
						return JS_FALSE;
					
					frameno = JSVAL_TO_INT(*vp);
					if( frameno<0)
					{
						aviInfo info;
						video_body->getVideoInfo(&info);
						frameno=-frameno;
						if(frameno>info.nb_frames)
							return JS_FALSE;
						
						frameno = info.nb_frames-frameno;
					}
					if(GUI_GoToFrame( frameno ))
						return JS_TRUE;
					return JS_FALSE;
				}
				break;
			case fps_prop:
				{
					priv->getObject()->m_dFPS = *JSVAL_TO_DOUBLE(*vp);
					aviInfo info;

					if (avifileinfo)
					{
						video_body->getVideoInfo(&info);				
						info.fps1000 = (uint32_t)floor(priv->getObject()->m_dFPS*1000.f);
						video_body->updateVideoInfo (&info);
						video_body->getVideoInfo (avifileinfo);
						return JS_TRUE;
					} else 
					{
						return JS_FALSE;
					}
				}
				break;
		}
	}
	return JS_TRUE;
}

JSBool ADM_JSAvidemux::Load(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Load
	ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	printf("Loading \"%s\"\n",pTempStr);
	*rval = BOOLEAN_TO_JSVAL(A_openAvi(pTempStr));
	return JS_TRUE;
}// end Load

JSBool ADM_JSAvidemux::LoadFilters(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin LoadFilters
	ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	printf("Loading Filters \"%s\"\n",pTempStr);
	*rval = BOOLEAN_TO_JSVAL(filterLoadXml(pTempStr,0));
	return JS_TRUE;
}// end LoadFilters


JSBool ADM_JSAvidemux::Append(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Append
	ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	printf("Appending \"%s\"\n",pTempStr);
	*rval = BOOLEAN_TO_JSVAL(A_appendAvi(pTempStr));
	return JS_TRUE;
}// end Append

JSBool ADM_JSAvidemux::Delete(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Delete
	ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	int a = JSVAL_TO_INT(argv[0]);
	int b = JSVAL_TO_INT(argv[1]);
	aprintf("Deleting %d-%d\n",a,b);
	*rval = BOOLEAN_TO_JSVAL(A_delete(a,b));
	return JS_TRUE;
}// end Delete

JSBool ADM_JSAvidemux::Save(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Save
	ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	printf("Saving \"%s\"\n",pTempStr);
	*rval = BOOLEAN_TO_JSVAL(A_Save(pTempStr));
	return JS_TRUE;
}// end Save

JSBool ADM_JSAvidemux::SaveDVD(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin SaveDVD
	ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	printf("Saving as DVD \"%s\"\n",pTempStr);
	*rval = BOOLEAN_TO_JSVAL(A_saveDVDPS(pTempStr));
	return JS_TRUE;
}// end SaveDVD

JSBool ADM_JSAvidemux::SaveOGM(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin SaveOGM
	ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 1)
		return JS_FALSE;
	char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	printf("Saving as DVD \"%s\"\n",pTempStr);
	*rval = BOOLEAN_TO_JSVAL(A_saveDVDPS(pTempStr));
	return JS_TRUE;
}// end SaveOGM
JSBool ADM_JSAvidemux::ClearSegments(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin ClearSegments
        ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
        // default return value
        *rval = BOOLEAN_TO_JSVAL(false);
        if(argc != 0)
                return JS_FALSE;
        printf("clearing segments \n");
        *rval = BOOLEAN_TO_JSVAL(video_body->deleteAllSegments());
        return JS_TRUE;
}// end ClearSegments
/*
add a segment. addsegment(source video,startframe, nbframes)",     
*/
JSBool ADM_JSAvidemux::AddSegment(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin AddSegment
        ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
        // default return value
        *rval = BOOLEAN_TO_JSVAL(false);
        if(argc != 3)
                return JS_FALSE;
        int a = JSVAL_TO_INT(argv[0]);
        int b = JSVAL_TO_INT(argv[1]);
        int c = JSVAL_TO_INT(argv[2]);
        aprintf("adding segment :%d %d %d\n",a,b,c);
        *rval = BOOLEAN_TO_JSVAL( video_body->addSegment(a,b,c));
        return JS_TRUE;
}// end AddSegment

JSBool ADM_JSAvidemux::Exit(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Exit
	ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 0)
		return JS_FALSE;
	exit(0);
	*rval = INT_TO_JSVAL(1);
	return JS_TRUE;
}// end Exit

JSBool ADM_JSAvidemux::GoToTime(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin GoToTime
	ADM_JSAvidemux *p = (ADM_JSAvidemux *)JS_GetPrivate(cx, obj);
	// default return value
	*rval = BOOLEAN_TO_JSVAL(false);
	if(argc != 3)
		return JS_FALSE;
	*rval = INT_TO_JSVAL(A_jumpToTime(JSVAL_TO_INT(argv[0]),JSVAL_TO_INT(argv[1]),JSVAL_TO_INT(argv[2])));
	return JS_TRUE;
}// end GoToTime

