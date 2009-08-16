//
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
#include "ADM_default.h"
#include "ADM_JSAvidemuxVideo.h"
#include "ADM_JSGlobal.h"
#include "avi_vars.h"
#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_videoFilter.h"
#include "ADM_videoFilter_internal.h"
#include "ADM_encoder/adm_encConfig.h"
#include "../ADM_userInterfaces/ADM_commonUI/GUI_ui.h"
#include "ADM_JSAppliedVideoFilter.h"

extern VF_FILTERS filterGetTagFromName(const char *inname);
extern uint8_t A_ListAllBlackFrames( char *file );
extern uint8_t loadVideoCodecConfString( const char *name);
extern uint8_t ADM_saveRaw (const char *name);
extern int A_saveJpg (char *name);
extern uint8_t loadVideoCodecConf( const char *name);
extern void filterCleanUp( void );
extern uint32_t nb_active_filter;

JSPropertySpec ADM_JSAvidemuxVideo::properties[] = 
{ 
	{ "process", videoProcessProperty, JSPROP_ENUMERATE },        // process video when saving
	{ "width", widthProperty, JSPROP_ENUMERATE },
	{ "height", heightProperty, JSPROP_ENUMERATE },
	{ "frameCount", frameCountProperty, JSPROP_ENUMERATE },
	{ "vopPacked", vopPackedProperty, JSPROP_ENUMERATE },
	{ "qPel", qPelProperty, JSPROP_ENUMERATE },
	{ "gmc", gmcProperty, JSPROP_ENUMERATE },
	{ "fcc", fccProperty, JSPROP_ENUMERATE },
	{ "fps1000", fps1000Property, JSPROP_ENUMERATE },
	{ "appliedFilters", appliedFiltersProperty, JSPROP_ENUMERATE },
	{ 0 }
};

JSFunctionSpec ADM_JSAvidemuxVideo::methods[] = 
{
	{ "clear", Clear, 0, 0, 0 },	// clear
	{ "add", Add, 3, 0, 0 },	// add
	{ "clearFilters", ClearFilters, 0, 0, 0 }, // Delete all filters
	{ "addFilter", AddFilter, 10, 0, 0 },	// Add filter to filter chain
	{ "codec", Codec, 3, 0, 0 },	// Set the video codec
	{ "codecPlugin", CodecPlugin, 4, 0, 0 },	// Set the video codec plugin
	{ "codecConf", CodecConf, 1, 0, 0 },	// load video codec config
	{ "save", Save, 1, 0, 0 },	// save video portion of the stream
	{ "saveJpeg", SaveJPEG, 1, 0, 0 },	// save the current frame as a JPEG
	{ "listBlackFrames", ListBlackFrames, 1, 0, 0 },	// output a list of the black frame to a file
	{ "setPostProc", PostProcess, 3, 0, 0 },	// Postprocess
	{ "getFrameSize", getFrameSize, 1, 0, 0 },        // FrameSize
	{ "getFrameType", getFrameType, 1, 0, 0 },        // Postprocess
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
	JSObject *newObj = JS_InitClass(cx, obj, proto, &m_classAvidemuxVideo, NULL, 0,
		ADM_JSAvidemuxVideo::properties, ADM_JSAvidemuxVideo::methods, NULL, NULL);
	ADM_JSAvidemuxVideo *p = new ADM_JSAvidemuxVideo();

	p->setObject(new ADM_AvidemuxVideo());
	JS_SetPrivate(cx, newObj, p);

	return newObj;
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
			case videoProcessProperty:
				*vp = BOOLEAN_TO_JSVAL(priv->getObject()->m_bVideoProcess);
				break;
			case widthProperty:
			{
				aviInfo info;

				video_body->getVideoInfo(&info);
				*vp = INT_TO_JSVAL(info.width);
				break;
			}
			case heightProperty:
			{
				aviInfo info;

				video_body->getVideoInfo(&info);
				*vp = INT_TO_JSVAL(info.height);
				break;
			}
			case frameCountProperty:
			{
				aviInfo info;

				video_body->getVideoInfo(&info);
				*vp = INT_TO_JSVAL(info.nb_frames);
				break;
			}
			case vopPackedProperty:
			{
				*vp = ((video_body->getSpecificMpeg4Info() & ADM_VOP_ON) == ADM_VOP_ON);
				break;
			}
			case qPelProperty:
			{
				*vp = ((video_body->getSpecificMpeg4Info() & ADM_QPEL_ON) == ADM_QPEL_ON);
				break;
			}
			case gmcProperty:
			{
				*vp = ((video_body->getSpecificMpeg4Info() & ADM_GMC_ON) == ADM_GMC_ON);
				break;
			}
			case fccProperty:
			{
				aviInfo info;

				video_body->getVideoInfo(&info);
				*vp = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, fourCC::tostring(info.fcc)));
				break;
			}
			case fps1000Property:
			{
				aviInfo info;

				video_body->getVideoInfo(&info);
				*vp = INT_TO_JSVAL(info.fps1000);
				break;
			}
			case appliedFiltersProperty:
			{
				JSObject *filters = JS_NewArrayObject(cx, 0, NULL);
				jsval appliedFilter;

				*vp = OBJECT_TO_JSVAL(filters);

				for (int filterIndex = 1; filterIndex < nb_active_filter; filterIndex++)
				{
					appliedFilter = OBJECT_TO_JSVAL(ADM_JSAppliedVideoFilter::JSInit(cx, obj, NULL, filterIndex));
					JS_SetElement(cx, filters, filterIndex - 1, &appliedFilter);
				}

				break;
			}
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
			case videoProcessProperty:
				if (!JSVAL_IS_BOOLEAN(*vp))
					break;

				priv->getObject()->m_bVideoProcess = JSVAL_TO_BOOLEAN(*vp);
				UI_setVProcessToggleStatus(priv->getObject()->m_bVideoProcess);
				break;
			case fps1000Property:
			{
				if(!JSVAL_IS_INT(*vp))
					break;

				aviInfo info;
				int fps = JSVAL_TO_INT(*vp);

				if(fps > 100000 || fps < 2000)
				{
					printf("FPS too low\n");
					break;
				}

				video_body->getVideoInfo(&info);
				info.fps1000 = fps;
				video_body->updateVideoInfo(&info);

				break;
			}
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
        enterLock();
        *rval = BOOLEAN_TO_JSVAL(video_body->deleteAllSegments());
        leaveLock();
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
        if(JSVAL_IS_INT(argv[0]) == false || JSVAL_IS_INT(argv[1]) == false  || JSVAL_IS_INT(argv[2]) == false)
                return JS_FALSE;
        printf("Adding Video... \n");
        enterLock();
        *rval = BOOLEAN_TO_JSVAL(video_body->addSegment(JSVAL_TO_INT(argv[0]),JSVAL_TO_INT(argv[1]),JSVAL_TO_INT(argv[2])));
        leaveLock();
        return JS_TRUE;
}// end Add


JSBool ADM_JSAvidemuxVideo::ClearFilters(JSContext *cx, JSObject *obj, uintN argc,
                                       jsval *argv, jsval *rval)
{// begin Clear
	filterCleanUp();
        return JS_TRUE;
}// end Clear


JSBool ADM_JSAvidemuxVideo::AddFilter(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin AddFilter
        VF_FILTERS filter;

        // default return value
        *rval = BOOLEAN_TO_JSVAL(false);
        if(argc == 0)
                return JS_FALSE;

        filter = filterGetTagFromName(JS_GetStringBytes(JSVAL_TO_STRING(argv[0])));
        printf("Adding Filter \"%d\"... \n",filter);

        Arg args[argc];
        char *v;
        for(int i=0;i<argc;i++) 
        {
                args[i].type=APM_STRING;
                if(JSVAL_IS_STRING(argv[i]) == false)
                {
                        return JS_FALSE;
                }
                v=ADM_strdup(JS_GetStringBytes(JSVAL_TO_STRING(argv[i])));
                args[i].arg.string=v;
        }
        enterLock();
        *rval= BOOLEAN_TO_JSVAL(filterAddScript(filter,argc,args));
        leaveLock();
        
        for(int i=0;i<argc;i++) 
        {
            ADM_dealloc(args[i].arg.string);
        }
        
        return JS_TRUE;
}// end AddFilter

JSBool ADM_JSAvidemuxVideo::Codec(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Codec
        *rval = BOOLEAN_TO_JSVAL(false);
        if(argc > 3)
                return JS_FALSE;
        printf("Codec ... \n");
        if(JSVAL_IS_STRING(argv[0]) == false || JSVAL_IS_STRING(argv[1]) == false  || JSVAL_IS_STRING(argv[2]) == false)
                return JS_FALSE;
        
                printf("[codec]%s\n",JS_GetStringBytes(JSVAL_TO_STRING(argv[0])));
                printf("[conf ]%s\n",JS_GetStringBytes(JSVAL_TO_STRING(argv[1])));
                printf("[xtra ]%s\n",JS_GetStringBytes(JSVAL_TO_STRING(argv[2])));
                
                char *codec,*conf,*codecConfString;
                codec = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
                conf = JS_GetStringBytes(JSVAL_TO_STRING(argv[1]));
                codecConfString = JS_GetStringBytes(JSVAL_TO_STRING(argv[2]));
                enterLock();
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
                        {
                                *rval = BOOLEAN_TO_JSVAL(true);
                                if(!loadVideoCodecConfString(codecConfString))
                                        *rval = BOOLEAN_TO_JSVAL(false);
                                else
                                        *rval = BOOLEAN_TO_JSVAL(true);
                        }

                }// end conf
                leaveLock();

        return JS_TRUE;
}// end Codec

JSBool ADM_JSAvidemuxVideo::CodecPlugin(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
	*rval = BOOLEAN_TO_JSVAL(false);

	if (argc != 4)
		return JS_FALSE;

	printf("Codec Plugin ... \n");

	if (!JSVAL_IS_STRING(argv[0]) || !JSVAL_IS_STRING(argv[1]) || !JSVAL_IS_STRING(argv[2]) || !JSVAL_IS_STRING(argv[3]))
		return JS_FALSE;

	char *guid = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	char *desc = JS_GetStringBytes(JSVAL_TO_STRING(argv[1]));
	char *conf = JS_GetStringBytes(JSVAL_TO_STRING(argv[2]));
	char *data = JS_GetStringBytes(JSVAL_TO_STRING(argv[3]));

	printf("[guid] %s\n", guid);
	printf("[desc] %s\n", desc);
	printf("[conf] %s\n", conf);
	printf("[data] %s\n", data);

	enterLock();

	if (!videoCodecPluginSelectByGuid(guid))
		*rval = BOOLEAN_TO_JSVAL(false);
	else
		*rval = BOOLEAN_TO_JSVAL(videoCodecConfigure(conf, 0, (uint8_t*)data));

	leaveLock();

	return JS_TRUE;
}

JSBool ADM_JSAvidemuxVideo::CodecConf(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{
	*rval = BOOLEAN_TO_JSVAL(false);

	if (argc != 1)
		return JS_FALSE;

	if (!JSVAL_IS_STRING(argv[0]))
		return JS_FALSE;

	char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));

	printf("Codec Conf Video \"%s\"\n", pTempStr);

	enterLock();
	*rval = INT_TO_JSVAL(loadVideoCodecConf(pTempStr));
	leaveLock();

	return JS_TRUE;
}

JSBool ADM_JSAvidemuxVideo::Save(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin Save
        // default return value
        *rval = BOOLEAN_TO_JSVAL(false);
        if(argc != 1)
                return JS_FALSE;
        if(JSVAL_IS_STRING(argv[0]) == false)
                return JS_FALSE;
        char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
        printf("Saving Video \"%s\"\n",pTempStr);
        enterLock();
        *rval = INT_TO_JSVAL(ADM_saveRaw(pTempStr));
        leaveLock();
        return JS_TRUE;
}// end Save

JSBool ADM_JSAvidemuxVideo::SaveJPEG(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin SaveJPG
        // default return value
        *rval = BOOLEAN_TO_JSVAL(false);
        if(argc != 1)
                return JS_FALSE;
        if(JSVAL_IS_STRING(argv[0]) == false)
                return JS_FALSE;
        char *pTempStr = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
        printf("Saving JPEG \"%s\"\n",pTempStr);
        enterLock();
        *rval = INT_TO_JSVAL(A_saveJpg(pTempStr));
        leaveLock();
        return JS_TRUE;
}// end SaveJPG

JSBool ADM_JSAvidemuxVideo::ListBlackFrames(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin ListBlackFrames
        
        // default return value
        *rval = BOOLEAN_TO_JSVAL(false);
        if(argc != 1)
          return JS_FALSE;
        if(JSVAL_IS_STRING(argv[0]) == false)
          return JS_FALSE;
        
        enterLock();
        A_ListAllBlackFrames(JS_GetStringBytes(JSVAL_TO_STRING(argv[0])));
        leaveLock();
        *rval = BOOLEAN_TO_JSVAL(true);
        return JS_TRUE;
}// end ListBlackFrames

JSBool ADM_JSAvidemuxVideo::PostProcess(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{// begin PostProcess
        // default return value
        *rval = BOOLEAN_TO_JSVAL(false);
        if(argc != 3)
                return JS_FALSE;
        if(JSVAL_IS_INT(argv[0]) == false || JSVAL_IS_INT(argv[1]) == false || JSVAL_IS_INT(argv[2]) == false)
                return JS_FALSE;
        
        enterLock();
        int rtn =video_body->setPostProc(
            JSVAL_TO_INT(argv[0]),JSVAL_TO_INT(argv[1]),JSVAL_TO_INT(argv[2]));
        leaveLock();
        *rval = BOOLEAN_TO_JSVAL(rtn);
        return JS_TRUE;
}// end PostProcess

JSBool ADM_JSAvidemuxVideo::getFrameSize(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{
uint32_t info;
uint32_t frame;
uint32_t sz;
        if(argc != 1)
          return JS_FALSE;
        
        enterLock();
        frame=JSVAL_TO_INT(argv[0]);
        if(!video_body->getFrameSize(frame,&sz)) return JS_FALSE;
        leaveLock(); 
        
        *rval=INT_TO_JSVAL(sz);
        return JS_TRUE;
}

JSBool ADM_JSAvidemuxVideo::getFrameType(JSContext *cx, JSObject *obj, uintN argc, 
                                       jsval *argv, jsval *rval)
{
uint32_t info;
uint32_t frame;
uint32_t sz;
        if(argc != 1)
          return JS_FALSE;
        
        enterLock();
        frame=JSVAL_TO_INT(argv[0]);
        if(!video_body->getFlags(frame,&sz)) return JS_FALSE;
        leaveLock(); 
        
        *rval=INT_TO_JSVAL(sz);
        return JS_TRUE;
}
/* EOF */
