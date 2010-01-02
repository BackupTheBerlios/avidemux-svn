/**
    \file ADM_jsAvidemux
    \brief Standard includes and defines
*/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ADM_JS_AVIDEMUX_H
#define ADM_JS_AVIDEMUX_H
#include "ADM_inttype.h"
#include "jsapi.h"
#ifdef __cplusplus
extern "C" {
#endif

int jsLoadVideo(const char *c);
int jsAppendVideo(const char *c);
int jsClearSegments(void);
int jsAddSegment(int ref, double start, double duration);
int jsAudioReset(void);
int jsAudioMixer(const char *s);
// Fq
int32_t jsGetResample(void);
void    jsSetResample(int32_t fq);
// Markers
double jsGetMarkerA(void);
double jsGetMarkerB(void);
void   jsSetMarkerA(double a);
void   jsSetMarkerB(double b);
//
int    jsClearVideoFilters();
// Info
int jsGetWidth ( void) ;
int jsGetHeight ( void) ;
int jsGetFps1000 ( void) ;
char *jsGetVideoCodec ( void) ;

// non jsapigen function, variables number of args
JSBool jsAdmaddVideoFilter(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool jsAdmaudioCodec(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool jsAdmsetContainer(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
JSBool jsAdmvideoCodec(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
#ifdef __cplusplus
};
#endif

#endif
