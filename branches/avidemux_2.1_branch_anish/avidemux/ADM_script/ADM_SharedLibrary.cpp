//
// C++ Implementation: ADM_SharedLibrary
//
// Description: 
//
//
// Author: Anish Mistry <amistry@am-productions.biz>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <link.h>
#include <dlfcn.h>
#include <string>

#include "ADM_SharedLibrary.h"

/*
TODO: Implement dlinfo() wrapper
*/

ADM_SharedLibrary::ADM_SharedLibrary(std::string sPath)
{// begin constructor 2
	m_pLibrary = NULL;
	m_sPath = "";
	if(Open(sPath) == false)
		throw("ADM_SharedLibrary: Error loading \""+sPath+"\"!");
}// end constructor 2

ADM_SharedLibrary::~ADM_SharedLibrary(void)
{// begin ~ADM_SharedLibrary
	Close();
}// end ~ADM_SharedLibrary

bool ADM_SharedLibrary::Open(std::string sPath)
// returns false if there is already a library load or the library fails to load
// GetError() will display the error if the library fails to load.
// returns true if the library was successfully loaded
{// begin Open
	if(m_pLibrary)
		return false;
	m_pLibrary = dlopen(sPath.c_str(), RTLD_LAZY);
	if(m_pLibrary == NULL)
		return false;
	m_sPath = sPath;
	return true;
}// end Open

bool ADM_SharedLibrary::Close()
// if the library is successfully closed or there was no library open
// then return true, otherwise return false and use GetError() to
// determine the problem
{// begin Close
	if(m_pLibrary)
	{// begin close handle
		if(dlclose(m_pLibrary) == -1)
			return false;
	}// end close handle
	m_pLibrary = NULL;
	m_sPath = "";
	return true;
}// end Close

std::string ADM_SharedLibrary::GetError()
{// begin GetError
	return dlerror();
}// end GetError

JSBool ADM_SharedLibrary::Call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
// in the array argv the first argument is the function name (string), and the second is the return type (string)
{// begin Call
	if(m_pLibrary == NULL || argc < 2)
		return JS_FALSE;
	if(JSVAL_IS_STRING(argv[0]) == false || JSVAL_IS_STRING(argv[1]) == false)
		return JS_FALSE;
	const char *pFunctionName = JS_GetStringBytes(JSVAL_TO_STRING(argv[0]));
	std::string sReturnType = JS_GetStringBytes(JSVAL_TO_STRING(argv[1]));

	dlfunc_t SomeFunc = dlfunc(m_pLibrary, pFunctionName);
	if(SomeFunc == NULL)
		return JS_FALSE;

	int int_return;
	double double_return;
/*	void *pointer_return;*/
	// argument list
	av_alist alist;
	if(sReturnType == "void")
		av_start_void(alist, SomeFunc);
	else if(sReturnType == "int")
		av_start_int(alist, SomeFunc, &int_return);
	else if(sReturnType == "double")
		av_start_double(alist, SomeFunc, &double_return);
/*	else if(sReturnType == "struct")
		av_start_struct (alist, SomeFunc, struct_type, splittable, &struct_return);
	else if(sReturnType == "ptr")
		av_start_ptr(alist, SomeFunc, pointer_type, &pointer_return);
*/
	else
	{// begin programming error
		JS_ReportError(cx, "In call \"%s\" %s is an invalid return type.", pFunctionName, sReturnType.c_str());
		return JS_FALSE;	// something is fubar'd. bail.
	}// end programming error
		
	for(uintN i = 0;i < argc;i++)
	{
	double n = 2;
	double result;
	av_double (alist, n);
	av_double (alist, n);
	}
	av_call(alist);
}// end Call

