//
// C++ Interface: ADM_SharedLibrary
//
// Description: 
//
//
// Author: Anish Mistry <amistry@am-productions.biz>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _ADM_SHAREDLIBRARY_H
#define _ADM_SHAREDLIBRARY_H

#pragma once
#include <config.h>

// Spidermonkey
#include <jsapi.h>
#include <avcall.h>
#include <string>

class ADM_SharedLibrary
{
public:
	ADM_SharedLibrary(std::string sPath);
	ADM_SharedLibrary(void) : m_pLibrary(NULL), m_sPath("") {}
	virtual ~ADM_SharedLibrary(void);

	bool Open(std::string sPath);
	bool Close();
	std::string GetError();
	JSBool Call(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

private:
	void *m_pLibrary;
	std::string m_sPath;

};

#endif
