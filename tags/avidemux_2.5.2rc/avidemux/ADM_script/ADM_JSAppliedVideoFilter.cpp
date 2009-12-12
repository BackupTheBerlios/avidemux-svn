#include "ADM_default.h"
#include "ADM_JSAppliedVideoFilter.h"
#include "ADM_videoFilter.h"
#include "ADM_videoFilter_internal.h"

extern void filterCleanUp(void);
extern uint32_t nb_active_filter;
extern FILTER videofilters[VF_MAX_FILTER];

JSPropertySpec ADM_JSAppliedVideoFilter::properties[] = 
{ 
	{ "arguments", argumentsProperty, JSPROP_ENUMERATE },
	{ "width", widthProperty, JSPROP_ENUMERATE },
	{ "height", heightProperty, JSPROP_ENUMERATE },
	{ "fps1000", fps1000Property, JSPROP_ENUMERATE },
	{ 0 }
};

JSClass ADM_JSAppliedVideoFilter::m_appliedVideoFilter = 
{
	"AppliedVideoFilter", JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_PropertyStub,
	ADM_JSAppliedVideoFilter::JSGetProperty, JS_PropertyStub,
	JS_EnumerateStub, JS_ResolveStub, 
	JS_ConvertStub, ADM_JSAppliedVideoFilter::JSDestructor
};

ADM_JSAppliedVideoFilter::ADM_JSAppliedVideoFilter(int filterIndex)
{
	ADM_assert(filterIndex < nb_active_filter);

	_filterIndex = filterIndex;
}

ADM_JSAppliedVideoFilter::~ADM_JSAppliedVideoFilter(void)
{

}

int ADM_JSAppliedVideoFilter::getFilterIndex(void)
{
	return _filterIndex;
}

JSObject *ADM_JSAppliedVideoFilter::JSInit(JSContext *cx, JSObject *obj, JSObject *proto, int filterIndex)
{
	JSObject *newObj = JS_InitClass(cx, obj, proto, &m_appliedVideoFilter, NULL,
		0, ADM_JSAppliedVideoFilter::properties, NULL, NULL, NULL);
	ADM_JSAppliedVideoFilter *p = new ADM_JSAppliedVideoFilter(filterIndex);

	JS_SetPrivate(cx, newObj, p);

	return newObj;
}

void ADM_JSAppliedVideoFilter::JSDestructor(JSContext *cx, JSObject *obj)
{

}

JSBool ADM_JSAppliedVideoFilter::JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
	ADM_JSAppliedVideoFilter *p = (ADM_JSAppliedVideoFilter *)JS_GetPrivate(cx, obj);
	FILTER filterEntry = videofilters[p->getFilterIndex()];

	if (JSVAL_IS_INT(id))
	{
		switch(JSVAL_TO_INT(id))
		{
			case argumentsProperty:
			{
				JSObject *args = JS_NewArrayObject(cx, 0, NULL);
				jsval arg, argString, valueString;
				char *argName, *argValue;
				CONFcouple *couple;

				*vp = OBJECT_TO_JSVAL(args);

				if (filterEntry.filter->getCoupledConf(&couple))
				{
					for(int j = 0; j < couple->getNumber(); j++)
					{
						couple->getEntry(j, &argName, &argValue);

						arg = OBJECT_TO_JSVAL(JS_NewArrayObject(cx, 0, NULL));
						argString = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, argName));
						valueString = STRING_TO_JSVAL(JS_NewStringCopyZ(cx, argValue));

						JS_SetElement(cx, args, j, &arg);
						JS_SetElement(cx, JSVAL_TO_OBJECT(arg), 0, &argString);
						JS_SetElement(cx, JSVAL_TO_OBJECT(arg), 1, &valueString);
					}

					delete couple;
				}

				break;
			}
			case widthProperty:
			{
				ADV_Info *info = filterEntry.filter->getInfo();

				*vp = INT_TO_JSVAL(info->width);
				break;
			}
			case heightProperty:
			{
				ADV_Info *info = filterEntry.filter->getInfo();

				*vp = INT_TO_JSVAL(info->height);
				break;
			}
			case fps1000Property:
			{
				ADV_Info *info = filterEntry.filter->getInfo();

				*vp = INT_TO_JSVAL(info->fps1000);
				break;
			}
		}
	}
	return JS_TRUE;
}
