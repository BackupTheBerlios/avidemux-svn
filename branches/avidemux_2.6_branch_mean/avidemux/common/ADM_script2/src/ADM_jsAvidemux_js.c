/*
--- DO NOT EDIT THIS FILE !!! ---

This file has been generated automatically with 'jsapigen'.

jsapigen is a glue-code generator for SpiderMonkey. It is distributed
under the conditions of version 3 of the GNU General Public License.
Please have a look at http://jsapigen.sourceforge.net.

This file is NOT part of jsapigen and is NOT necessarily covered by
jsapigen's license. For licensing information regarding this file,
please refer to the software package which it is part of.

*/

#include "stdio.h"
#include "ADM_jsAvidemux.h"
void jsAvidemux(void)
{
        printf("Constructor invoked\n");
}

#include <string.h>
#include <wchar.h>
#include <jsapi.h>
#ifndef JS_THREADSAFE
#if JS_VERSION <= 170
#define jsrefcount int
#define JS_BeginRequest(cx)
#define JS_EndRequest(cx)
#define JS_SuspendRequest(cx)
#define JS_ResumeRequest(cx, saveDepth)
#endif
#endif
#ifndef JS_FS
#define JS_FS(name, call, nargs, flags, extra) {name, call, nargs, flags, extra}
#endif
#ifndef JS_FS_END
#define JS_FS_END {NULL, NULL, 0, 0, 0}
#endif
static JSPropertySpec jj_static_ps[] = {
    {NULL, 0, 0, NULL, NULL}
};
static JSPropertySpec jj_ps[] = {
    {NULL, 0, 0, NULL, NULL}
};
static JSFunctionSpec jj_static_fs[] = {
    JS_FS_END
};
static JSFunctionSpec jj_fs[] = {
    JS_FS_END
};
static JSBool
jjadmloadVideo(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var2;
    char *var7;
    int var3;
    int var6;
    int var8;
    jsval var9;
    JSString *var10;
    jsval var313;
    size_t var11;
    size_t var12;
    int var14;
    jschar *var13;
    jsval var314;
    jsval var15;
    JSBool var1;
    var2 = NULL;
    var7 = NULL;
    var3 = 0;
    var6 = 0;
    var8 = 0;
    var9 = JSVAL_NULL;
    var10 = NULL;
    var313 = JSVAL_NULL;
    var11 = 0;
    var12 = 0;
    var14 = 0;
    var13 = NULL;
    var314 = JSVAL_NULL;
    var15 = JSVAL_NULL;
    var1 = JS_FALSE;
    var2 = obj;
    var6 = argc;
    var8 = 0;
    var8 = var8 < var6;
    if (var8) {
    var9 = argv[0];
    var10 = JS_ValueToString(cx, var9);
    if (!var10) {
        goto do_return;
    }
    var313 = STRING_TO_JSVAL(var10);
    argv[argc+0] = var313;
    var11 = JS_GetStringLength(var10);
    var12 = 1;
    var12 += var11;
    var7 = JS_malloc(cx, var12);
    if (!var7) {
        goto do_return;
    }
    var14 = 1;
    var13 = JS_GetStringChars(var10);
    var314 = STRING_TO_JSVAL(var10);
    argv[argc+1] = var314;
    {
        size_t i;
        for (i = 0; i < var11; ++i) {
            var7[i] = wctob(var13[i]);
        }
        var7[var11] = '\0';
    }
    }
    var3 = jsLoadVideo(var7);
    if (JS_NewNumberValue(cx, var3, &var15) != JS_TRUE) {
        goto do_return;
    }
    argv[argc+2] = var15;
    if (rval) {
        *rval = var15;
    }
    var1 = JS_TRUE;
    do_return:
    if (var14) {
        JS_free(cx, var7);
        var7 = NULL;
        var14 = 0;
    }
    return var1;
}
static JSBool
jjadmclearSegments(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var17;
    int var18;
    int var21;
    jsval var22;
    JSBool var16;
    var17 = NULL;
    var18 = 0;
    var21 = 0;
    var22 = JSVAL_NULL;
    var16 = JS_FALSE;
    var17 = obj;
    var21 = argc;
    var18 = jsClearSegments();
    if (JS_NewNumberValue(cx, var18, &var22) != JS_TRUE) {
        goto do_return;
    }
    argv[argc+0] = var22;
    if (rval) {
        *rval = var22;
    }
    var16 = JS_TRUE;
    do_return:
    return var16;
}
static JSBool
jjadmappendVideo(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var24;
    char *var29;
    int var25;
    int var28;
    int var30;
    jsval var31;
    JSString *var32;
    jsval var317;
    size_t var33;
    size_t var34;
    int var36;
    jschar *var35;
    jsval var318;
    jsval var37;
    JSBool var23;
    var24 = NULL;
    var29 = NULL;
    var25 = 0;
    var28 = 0;
    var30 = 0;
    var31 = JSVAL_NULL;
    var32 = NULL;
    var317 = JSVAL_NULL;
    var33 = 0;
    var34 = 0;
    var36 = 0;
    var35 = NULL;
    var318 = JSVAL_NULL;
    var37 = JSVAL_NULL;
    var23 = JS_FALSE;
    var24 = obj;
    var28 = argc;
    var30 = 0;
    var30 = var30 < var28;
    if (var30) {
    var31 = argv[0];
    var32 = JS_ValueToString(cx, var31);
    if (!var32) {
        goto do_return;
    }
    var317 = STRING_TO_JSVAL(var32);
    argv[argc+0] = var317;
    var33 = JS_GetStringLength(var32);
    var34 = 1;
    var34 += var33;
    var29 = JS_malloc(cx, var34);
    if (!var29) {
        goto do_return;
    }
    var36 = 1;
    var35 = JS_GetStringChars(var32);
    var318 = STRING_TO_JSVAL(var32);
    argv[argc+1] = var318;
    {
        size_t i;
        for (i = 0; i < var33; ++i) {
            var29[i] = wctob(var35[i]);
        }
        var29[var33] = '\0';
    }
    }
    var25 = jsAppendVideo(var29);
    if (JS_NewNumberValue(cx, var25, &var37) != JS_TRUE) {
        goto do_return;
    }
    argv[argc+2] = var37;
    if (rval) {
        *rval = var37;
    }
    var23 = JS_TRUE;
    do_return:
    if (var36) {
        JS_free(cx, var29);
        var29 = NULL;
        var36 = 0;
    }
    return var23;
}
static JSBool
jjadmaddSegment(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var39;
    int var44;
    double var45;
    double var46;
    int var40;
    int var43;
    int var47;
    jsval var48;
    int32 var49;
    int var50;
    jsval var51;
    jsdouble var52;
    int var53;
    jsval var54;
    jsdouble var55;
    jsval var56;
    JSBool var38;
    var39 = NULL;
    var44 = 0;
    var45 = 0.0;
    var46 = 0.0;
    var40 = 0;
    var43 = 0;
    var47 = 0;
    var48 = JSVAL_NULL;
    var49 = 0;
    var50 = 0;
    var51 = JSVAL_NULL;
    var52 = 0.0;
    var53 = 0;
    var54 = JSVAL_NULL;
    var55 = 0.0;
    var56 = JSVAL_NULL;
    var38 = JS_FALSE;
    var39 = obj;
    var43 = argc;
    var47 = 0;
    var47 = var47 < var43;
    if (var47) {
    var48 = argv[0];
    if (JS_ValueToInt32(cx, var48, &var49) != JS_TRUE) {
        goto do_return;
    }
    var44 = (int)var49;
    }
    var50 = 1;
    var50 = var50 < var43;
    if (var50) {
    var51 = argv[1];
    if (JS_ValueToNumber(cx, var51, &var52) != JS_TRUE) {
        goto do_return;
    }
    var45 = (double)var52;
    }
    var53 = 2;
    var53 = var53 < var43;
    if (var53) {
    var54 = argv[2];
    if (JS_ValueToNumber(cx, var54, &var55) != JS_TRUE) {
        goto do_return;
    }
    var46 = (double)var55;
    }
    var40 = jsAddSegment(var44, var45, var46);
    if (JS_NewNumberValue(cx, var40, &var56) != JS_TRUE) {
        goto do_return;
    }
    argv[argc+0] = var56;
    if (rval) {
        *rval = var56;
    }
    var38 = JS_TRUE;
    do_return:
    return var38;
}
static JSBool
jjadmsetPostProc(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var58;
    int var63;
    int var64;
    int var65;
    int var59;
    int var62;
    int var66;
    jsval var67;
    int32 var68;
    int var69;
    jsval var70;
    int32 var71;
    int var72;
    jsval var73;
    int32 var74;
    jsval var75;
    JSBool var57;
    var58 = NULL;
    var63 = 0;
    var64 = 0;
    var65 = 0;
    var59 = 0;
    var62 = 0;
    var66 = 0;
    var67 = JSVAL_NULL;
    var68 = 0;
    var69 = 0;
    var70 = JSVAL_NULL;
    var71 = 0;
    var72 = 0;
    var73 = JSVAL_NULL;
    var74 = 0;
    var75 = JSVAL_NULL;
    var57 = JS_FALSE;
    var58 = obj;
    var62 = argc;
    var66 = 0;
    var66 = var66 < var62;
    if (var66) {
    var67 = argv[0];
    if (JS_ValueToInt32(cx, var67, &var68) != JS_TRUE) {
        goto do_return;
    }
    var63 = (int)var68;
    }
    var69 = 1;
    var69 = var69 < var62;
    if (var69) {
    var70 = argv[1];
    if (JS_ValueToInt32(cx, var70, &var71) != JS_TRUE) {
        goto do_return;
    }
    var64 = (int)var71;
    }
    var72 = 2;
    var72 = var72 < var62;
    if (var72) {
    var73 = argv[2];
    if (JS_ValueToInt32(cx, var73, &var74) != JS_TRUE) {
        goto do_return;
    }
    var65 = (int)var74;
    }
    var59 = jsSetPostProc(var63, var64, var65);
    if (JS_NewNumberValue(cx, var59, &var75) != JS_TRUE) {
        goto do_return;
    }
    argv[argc+0] = var75;
    if (rval) {
        *rval = var75;
    }
    var57 = JS_TRUE;
    do_return:
    return var57;
}
static JSBool
jjadmaudioReset(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var77;
    int var78;
    int var81;
    jsval var82;
    JSBool var76;
    var77 = NULL;
    var78 = 0;
    var81 = 0;
    var82 = JSVAL_NULL;
    var76 = JS_FALSE;
    var77 = obj;
    var81 = argc;
    var78 = jsAudioReset();
    if (JS_NewNumberValue(cx, var78, &var82) != JS_TRUE) {
        goto do_return;
    }
    argv[argc+0] = var82;
    if (rval) {
        *rval = var82;
    }
    var76 = JS_TRUE;
    do_return:
    return var76;
}
static JSBool
jjadmvideoCodec_ignore(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var84;
    char *var89;
    char **var90;
    int var85;
    int var88;
    int var91;
    jsval var92;
    JSString *var93;
    jsval var323;
    size_t var94;
    size_t var95;
    int var97;
    jschar *var96;
    jsval var324;
    int var98;
    jsval var99;
    JSObject *var100;
    jsval var325;
    jsuint var101;
    char **var103;
    size_t var104;
    int var130;
    size_t var105;
    JSString *var106;
    size_t var107;
    JSString **var113;
    int var131;
    jsuint var108;
    jsint var109;
    JSBool var110;
    jsval var111;
    JSString *var112;
    jsval var326;
    size_t var116;
    jsuint var114;
    jsint var115;
    JSString *var118;
    size_t var117;
    size_t var119;
    char var128;
    size_t var129;
    char *var120;
    int var132;
    char *var121;
    size_t var122;
    int var133;
    char *var125;
    jsuint var123;
    jsint var124;
    JSString *var127;
    size_t var126;
    size_t var134;
    size_t var135;
    int var137;
    jschar *var136;
    jsval var327;
    jsval var138;
    JSBool var83;
    var84 = NULL;
    var89 = NULL;
    var90 = NULL;
    var85 = 0;
    var88 = 0;
    var91 = 0;
    var92 = JSVAL_NULL;
    var93 = NULL;
    var323 = JSVAL_NULL;
    var94 = 0;
    var95 = 0;
    var97 = 0;
    var96 = NULL;
    var324 = JSVAL_NULL;
    var98 = 0;
    var99 = JSVAL_NULL;
    var100 = NULL;
    var325 = JSVAL_NULL;
    var101 = 0;
    var103 = NULL;
    var104 = 0;
    var130 = 0;
    var105 = 0;
    var106 = NULL;
    var107 = 0;
    var113 = NULL;
    var131 = 0;
    var108 = 0;
    var109 = 0;
    var110 = JS_FALSE;
    var111 = JSVAL_NULL;
    var112 = NULL;
    var326 = JSVAL_NULL;
    var116 = 0;
    var114 = 0;
    var115 = 0;
    var118 = NULL;
    var117 = 0;
    var119 = 0;
    var128 = 0;
    var129 = 0;
    var120 = NULL;
    var132 = 0;
    var121 = NULL;
    var122 = 0;
    var133 = 0;
    var125 = NULL;
    var123 = 0;
    var124 = 0;
    var127 = NULL;
    var126 = 0;
    var134 = 0;
    var135 = 0;
    var137 = 0;
    var136 = NULL;
    var327 = JSVAL_NULL;
    var138 = JSVAL_NULL;
    var83 = JS_FALSE;
    var84 = obj;
    var88 = argc;
    var91 = 0;
    var91 = var91 < var88;
    if (var91) {
    var92 = argv[0];
    var93 = JS_ValueToString(cx, var92);
    if (!var93) {
        goto do_return;
    }
    var323 = STRING_TO_JSVAL(var93);
    argv[argc+0] = var323;
    var94 = JS_GetStringLength(var93);
    var95 = 1;
    var95 += var94;
    var89 = JS_malloc(cx, var95);
    if (!var89) {
        goto do_return;
    }
    var97 = 1;
    var96 = JS_GetStringChars(var93);
    var324 = STRING_TO_JSVAL(var93);
    argv[argc+1] = var324;
    {
        size_t i;
        for (i = 0; i < var94; ++i) {
            var89[i] = wctob(var96[i]);
        }
        var89[var94] = '\0';
    }
    }
    var98 = 1;
    var98 = var98 < var88;
    if (var98) {
    var99 = argv[1];
    if (JS_ValueToObject(cx, var99, &var100) != JS_TRUE) {
        goto do_return;
    }
    var325 = OBJECT_TO_JSVAL(var100);
    argv[argc+2] = var325;
    if (JS_GetArrayLength(cx, var100, &var101) != JS_TRUE) {
        goto do_return;
    }
    var104 = sizeof(var103);
    var104 *= var101;
    var90 = JS_malloc(cx, var104);
    if (!var90) {
        goto do_return;
    }
    var130 = 1;
    var105 = var101;
    var107 = sizeof(var106);
    var105 *= var107;
    var113 = JS_malloc(cx, var105);
    if (!var113) {
        goto do_return;
    }
    var131 = 1;
    var108 = var101;
    var109 = -1;
    while (var108)
    {
    var108 += var109;
    var110 = JS_GetElement(cx, var100, var108, &var111);
    var112 = JS_ValueToString(cx, var111);
    if (!var112) {
        goto do_return;
    }
    var326 = STRING_TO_JSVAL(var112);
    argv[argc+3] = var326;
    var113[var108] = var112;
    }
    var116 = 0;
    var114 = var101;
    var115 = -1;
    while (var114)
    {
    var114 += var115;
    var118 = var113[var114];
    var117 = JS_GetStringLength(var118);
    var116 += var117;
    var119 = 1;
    var116 += var119;
    }
    var129 = sizeof(var128);
    var129 *= var116;
    var120 = JS_malloc(cx, var129);
    if (!var120) {
        goto do_return;
    }
    var132 = 1;
    var122 = sizeof(var121);
    var122 *= var101;
    var90 = JS_malloc(cx, var122);
    if (!var90) {
        goto do_return;
    }
    var133 = 1;
    var125 = var120;
    var125 += var116;
    var123 = var101;
    var124 = -1;
    while (var123)
    {
    var123 += var124;
    var127 = var113[var123];
    var126 = JS_GetStringLength(var127);
    var125 -= var126;
    var125 += var124;
    var134 = JS_GetStringLength(var127);
    var135 = 1;
    var135 += var134;
    var125 = JS_malloc(cx, var135);
    if (!var125) {
        goto do_return;
    }
    var137 = 1;
    var136 = JS_GetStringChars(var127);
    var327 = STRING_TO_JSVAL(var127);
    argv[argc+4] = var327;
    {
        size_t i;
        for (i = 0; i < var134; ++i) {
            var125[i] = wctob(var136[i]);
        }
        var125[var134] = '\0';
    }
    var90[var123] = var125;
    }
    }
    var85 = jsVideoCodec(var89, var90);
    if (JS_NewNumberValue(cx, var85, &var138) != JS_TRUE) {
        goto do_return;
    }
    argv[argc+5] = var138;
    if (rval) {
        *rval = var138;
    }
    var83 = JS_TRUE;
    do_return:
    if (var137) {
        JS_free(cx, var125);
        var125 = NULL;
        var137 = 0;
    }
    if (var133) {
        JS_free(cx, var90);
        var90 = NULL;
        var133 = 0;
    }
    if (var132) {
        JS_free(cx, var120);
        var120 = NULL;
        var132 = 0;
    }
    if (var131) {
        JS_free(cx, var113);
        var113 = NULL;
        var131 = 0;
    }
    if (var130) {
        JS_free(cx, var90);
        var90 = NULL;
        var130 = 0;
    }
    if (var97) {
        JS_free(cx, var89);
        var89 = NULL;
        var97 = 0;
    }
    return var83;
}
static JSBool
jjadmaddVideoFilter_ignore(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var140;
    char *var145;
    char **var146;
    int var141;
    int var144;
    int var147;
    jsval var148;
    JSString *var149;
    jsval var329;
    size_t var150;
    size_t var151;
    int var153;
    jschar *var152;
    jsval var330;
    int var154;
    jsval var155;
    JSObject *var156;
    jsval var331;
    jsuint var157;
    char **var159;
    size_t var160;
    int var186;
    size_t var161;
    JSString *var162;
    size_t var163;
    JSString **var169;
    int var187;
    jsuint var164;
    jsint var165;
    JSBool var166;
    jsval var167;
    JSString *var168;
    jsval var332;
    size_t var172;
    jsuint var170;
    jsint var171;
    JSString *var174;
    size_t var173;
    size_t var175;
    char var184;
    size_t var185;
    char *var176;
    int var188;
    char *var177;
    size_t var178;
    int var189;
    char *var181;
    jsuint var179;
    jsint var180;
    JSString *var183;
    size_t var182;
    size_t var190;
    size_t var191;
    int var193;
    jschar *var192;
    jsval var333;
    jsval var194;
    JSBool var139;
    var140 = NULL;
    var145 = NULL;
    var146 = NULL;
    var141 = 0;
    var144 = 0;
    var147 = 0;
    var148 = JSVAL_NULL;
    var149 = NULL;
    var329 = JSVAL_NULL;
    var150 = 0;
    var151 = 0;
    var153 = 0;
    var152 = NULL;
    var330 = JSVAL_NULL;
    var154 = 0;
    var155 = JSVAL_NULL;
    var156 = NULL;
    var331 = JSVAL_NULL;
    var157 = 0;
    var159 = NULL;
    var160 = 0;
    var186 = 0;
    var161 = 0;
    var162 = NULL;
    var163 = 0;
    var169 = NULL;
    var187 = 0;
    var164 = 0;
    var165 = 0;
    var166 = JS_FALSE;
    var167 = JSVAL_NULL;
    var168 = NULL;
    var332 = JSVAL_NULL;
    var172 = 0;
    var170 = 0;
    var171 = 0;
    var174 = NULL;
    var173 = 0;
    var175 = 0;
    var184 = 0;
    var185 = 0;
    var176 = NULL;
    var188 = 0;
    var177 = NULL;
    var178 = 0;
    var189 = 0;
    var181 = NULL;
    var179 = 0;
    var180 = 0;
    var183 = NULL;
    var182 = 0;
    var190 = 0;
    var191 = 0;
    var193 = 0;
    var192 = NULL;
    var333 = JSVAL_NULL;
    var194 = JSVAL_NULL;
    var139 = JS_FALSE;
    var140 = obj;
    var144 = argc;
    var147 = 0;
    var147 = var147 < var144;
    if (var147) {
    var148 = argv[0];
    var149 = JS_ValueToString(cx, var148);
    if (!var149) {
        goto do_return;
    }
    var329 = STRING_TO_JSVAL(var149);
    argv[argc+0] = var329;
    var150 = JS_GetStringLength(var149);
    var151 = 1;
    var151 += var150;
    var145 = JS_malloc(cx, var151);
    if (!var145) {
        goto do_return;
    }
    var153 = 1;
    var152 = JS_GetStringChars(var149);
    var330 = STRING_TO_JSVAL(var149);
    argv[argc+1] = var330;
    {
        size_t i;
        for (i = 0; i < var150; ++i) {
            var145[i] = wctob(var152[i]);
        }
        var145[var150] = '\0';
    }
    }
    var154 = 1;
    var154 = var154 < var144;
    if (var154) {
    var155 = argv[1];
    if (JS_ValueToObject(cx, var155, &var156) != JS_TRUE) {
        goto do_return;
    }
    var331 = OBJECT_TO_JSVAL(var156);
    argv[argc+2] = var331;
    if (JS_GetArrayLength(cx, var156, &var157) != JS_TRUE) {
        goto do_return;
    }
    var160 = sizeof(var159);
    var160 *= var157;
    var146 = JS_malloc(cx, var160);
    if (!var146) {
        goto do_return;
    }
    var186 = 1;
    var161 = var157;
    var163 = sizeof(var162);
    var161 *= var163;
    var169 = JS_malloc(cx, var161);
    if (!var169) {
        goto do_return;
    }
    var187 = 1;
    var164 = var157;
    var165 = -1;
    while (var164)
    {
    var164 += var165;
    var166 = JS_GetElement(cx, var156, var164, &var167);
    var168 = JS_ValueToString(cx, var167);
    if (!var168) {
        goto do_return;
    }
    var332 = STRING_TO_JSVAL(var168);
    argv[argc+3] = var332;
    var169[var164] = var168;
    }
    var172 = 0;
    var170 = var157;
    var171 = -1;
    while (var170)
    {
    var170 += var171;
    var174 = var169[var170];
    var173 = JS_GetStringLength(var174);
    var172 += var173;
    var175 = 1;
    var172 += var175;
    }
    var185 = sizeof(var184);
    var185 *= var172;
    var176 = JS_malloc(cx, var185);
    if (!var176) {
        goto do_return;
    }
    var188 = 1;
    var178 = sizeof(var177);
    var178 *= var157;
    var146 = JS_malloc(cx, var178);
    if (!var146) {
        goto do_return;
    }
    var189 = 1;
    var181 = var176;
    var181 += var172;
    var179 = var157;
    var180 = -1;
    while (var179)
    {
    var179 += var180;
    var183 = var169[var179];
    var182 = JS_GetStringLength(var183);
    var181 -= var182;
    var181 += var180;
    var190 = JS_GetStringLength(var183);
    var191 = 1;
    var191 += var190;
    var181 = JS_malloc(cx, var191);
    if (!var181) {
        goto do_return;
    }
    var193 = 1;
    var192 = JS_GetStringChars(var183);
    var333 = STRING_TO_JSVAL(var183);
    argv[argc+4] = var333;
    {
        size_t i;
        for (i = 0; i < var190; ++i) {
            var181[i] = wctob(var192[i]);
        }
        var181[var190] = '\0';
    }
    var146[var179] = var181;
    }
    }
    var141 = jsVideoFilter(var145, var146);
    if (JS_NewNumberValue(cx, var141, &var194) != JS_TRUE) {
        goto do_return;
    }
    argv[argc+5] = var194;
    if (rval) {
        *rval = var194;
    }
    var139 = JS_TRUE;
    do_return:
    if (var193) {
        JS_free(cx, var181);
        var181 = NULL;
        var193 = 0;
    }
    if (var189) {
        JS_free(cx, var146);
        var146 = NULL;
        var189 = 0;
    }
    if (var188) {
        JS_free(cx, var176);
        var176 = NULL;
        var188 = 0;
    }
    if (var187) {
        JS_free(cx, var169);
        var169 = NULL;
        var187 = 0;
    }
    if (var186) {
        JS_free(cx, var146);
        var146 = NULL;
        var186 = 0;
    }
    if (var153) {
        JS_free(cx, var145);
        var145 = NULL;
        var153 = 0;
    }
    return var139;
}
static JSBool
jjadmaudioCodec_ignore(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var196;
    char *var201;
    char **var202;
    int var197;
    int var200;
    int var203;
    jsval var204;
    JSString *var205;
    jsval var335;
    size_t var206;
    size_t var207;
    int var209;
    jschar *var208;
    jsval var336;
    int var210;
    jsval var211;
    JSObject *var212;
    jsval var337;
    jsuint var213;
    char **var215;
    size_t var216;
    int var242;
    size_t var217;
    JSString *var218;
    size_t var219;
    JSString **var225;
    int var243;
    jsuint var220;
    jsint var221;
    JSBool var222;
    jsval var223;
    JSString *var224;
    jsval var338;
    size_t var228;
    jsuint var226;
    jsint var227;
    JSString *var230;
    size_t var229;
    size_t var231;
    char var240;
    size_t var241;
    char *var232;
    int var244;
    char *var233;
    size_t var234;
    int var245;
    char *var237;
    jsuint var235;
    jsint var236;
    JSString *var239;
    size_t var238;
    size_t var246;
    size_t var247;
    int var249;
    jschar *var248;
    jsval var339;
    jsval var250;
    JSBool var195;
    var196 = NULL;
    var201 = NULL;
    var202 = NULL;
    var197 = 0;
    var200 = 0;
    var203 = 0;
    var204 = JSVAL_NULL;
    var205 = NULL;
    var335 = JSVAL_NULL;
    var206 = 0;
    var207 = 0;
    var209 = 0;
    var208 = NULL;
    var336 = JSVAL_NULL;
    var210 = 0;
    var211 = JSVAL_NULL;
    var212 = NULL;
    var337 = JSVAL_NULL;
    var213 = 0;
    var215 = NULL;
    var216 = 0;
    var242 = 0;
    var217 = 0;
    var218 = NULL;
    var219 = 0;
    var225 = NULL;
    var243 = 0;
    var220 = 0;
    var221 = 0;
    var222 = JS_FALSE;
    var223 = JSVAL_NULL;
    var224 = NULL;
    var338 = JSVAL_NULL;
    var228 = 0;
    var226 = 0;
    var227 = 0;
    var230 = NULL;
    var229 = 0;
    var231 = 0;
    var240 = 0;
    var241 = 0;
    var232 = NULL;
    var244 = 0;
    var233 = NULL;
    var234 = 0;
    var245 = 0;
    var237 = NULL;
    var235 = 0;
    var236 = 0;
    var239 = NULL;
    var238 = 0;
    var246 = 0;
    var247 = 0;
    var249 = 0;
    var248 = NULL;
    var339 = JSVAL_NULL;
    var250 = JSVAL_NULL;
    var195 = JS_FALSE;
    var196 = obj;
    var200 = argc;
    var203 = 0;
    var203 = var203 < var200;
    if (var203) {
    var204 = argv[0];
    var205 = JS_ValueToString(cx, var204);
    if (!var205) {
        goto do_return;
    }
    var335 = STRING_TO_JSVAL(var205);
    argv[argc+0] = var335;
    var206 = JS_GetStringLength(var205);
    var207 = 1;
    var207 += var206;
    var201 = JS_malloc(cx, var207);
    if (!var201) {
        goto do_return;
    }
    var209 = 1;
    var208 = JS_GetStringChars(var205);
    var336 = STRING_TO_JSVAL(var205);
    argv[argc+1] = var336;
    {
        size_t i;
        for (i = 0; i < var206; ++i) {
            var201[i] = wctob(var208[i]);
        }
        var201[var206] = '\0';
    }
    }
    var210 = 1;
    var210 = var210 < var200;
    if (var210) {
    var211 = argv[1];
    if (JS_ValueToObject(cx, var211, &var212) != JS_TRUE) {
        goto do_return;
    }
    var337 = OBJECT_TO_JSVAL(var212);
    argv[argc+2] = var337;
    if (JS_GetArrayLength(cx, var212, &var213) != JS_TRUE) {
        goto do_return;
    }
    var216 = sizeof(var215);
    var216 *= var213;
    var202 = JS_malloc(cx, var216);
    if (!var202) {
        goto do_return;
    }
    var242 = 1;
    var217 = var213;
    var219 = sizeof(var218);
    var217 *= var219;
    var225 = JS_malloc(cx, var217);
    if (!var225) {
        goto do_return;
    }
    var243 = 1;
    var220 = var213;
    var221 = -1;
    while (var220)
    {
    var220 += var221;
    var222 = JS_GetElement(cx, var212, var220, &var223);
    var224 = JS_ValueToString(cx, var223);
    if (!var224) {
        goto do_return;
    }
    var338 = STRING_TO_JSVAL(var224);
    argv[argc+3] = var338;
    var225[var220] = var224;
    }
    var228 = 0;
    var226 = var213;
    var227 = -1;
    while (var226)
    {
    var226 += var227;
    var230 = var225[var226];
    var229 = JS_GetStringLength(var230);
    var228 += var229;
    var231 = 1;
    var228 += var231;
    }
    var241 = sizeof(var240);
    var241 *= var228;
    var232 = JS_malloc(cx, var241);
    if (!var232) {
        goto do_return;
    }
    var244 = 1;
    var234 = sizeof(var233);
    var234 *= var213;
    var202 = JS_malloc(cx, var234);
    if (!var202) {
        goto do_return;
    }
    var245 = 1;
    var237 = var232;
    var237 += var228;
    var235 = var213;
    var236 = -1;
    while (var235)
    {
    var235 += var236;
    var239 = var225[var235];
    var238 = JS_GetStringLength(var239);
    var237 -= var238;
    var237 += var236;
    var246 = JS_GetStringLength(var239);
    var247 = 1;
    var247 += var246;
    var237 = JS_malloc(cx, var247);
    if (!var237) {
        goto do_return;
    }
    var249 = 1;
    var248 = JS_GetStringChars(var239);
    var339 = STRING_TO_JSVAL(var239);
    argv[argc+4] = var339;
    {
        size_t i;
        for (i = 0; i < var246; ++i) {
            var237[i] = wctob(var248[i]);
        }
        var237[var246] = '\0';
    }
    var202[var235] = var237;
    }
    }
    var197 = jsAudioCodec(var201, var202);
    if (JS_NewNumberValue(cx, var197, &var250) != JS_TRUE) {
        goto do_return;
    }
    argv[argc+5] = var250;
    if (rval) {
        *rval = var250;
    }
    var195 = JS_TRUE;
    do_return:
    if (var249) {
        JS_free(cx, var237);
        var237 = NULL;
        var249 = 0;
    }
    if (var245) {
        JS_free(cx, var202);
        var202 = NULL;
        var245 = 0;
    }
    if (var244) {
        JS_free(cx, var232);
        var232 = NULL;
        var244 = 0;
    }
    if (var243) {
        JS_free(cx, var225);
        var225 = NULL;
        var243 = 0;
    }
    if (var242) {
        JS_free(cx, var202);
        var202 = NULL;
        var242 = 0;
    }
    if (var209) {
        JS_free(cx, var201);
        var201 = NULL;
        var209 = 0;
    }
    return var195;
}
static JSBool
jjadmsetContainer_ignore(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var252;
    char *var257;
    char **var258;
    int var253;
    int var256;
    int var259;
    jsval var260;
    JSString *var261;
    jsval var341;
    size_t var262;
    size_t var263;
    int var265;
    jschar *var264;
    jsval var342;
    int var266;
    jsval var267;
    JSObject *var268;
    jsval var343;
    jsuint var269;
    char **var271;
    size_t var272;
    int var298;
    size_t var273;
    JSString *var274;
    size_t var275;
    JSString **var281;
    int var299;
    jsuint var276;
    jsint var277;
    JSBool var278;
    jsval var279;
    JSString *var280;
    jsval var344;
    size_t var284;
    jsuint var282;
    jsint var283;
    JSString *var286;
    size_t var285;
    size_t var287;
    char var296;
    size_t var297;
    char *var288;
    int var300;
    char *var289;
    size_t var290;
    int var301;
    char *var293;
    jsuint var291;
    jsint var292;
    JSString *var295;
    size_t var294;
    size_t var302;
    size_t var303;
    int var305;
    jschar *var304;
    jsval var345;
    jsval var306;
    JSBool var251;
    var252 = NULL;
    var257 = NULL;
    var258 = NULL;
    var253 = 0;
    var256 = 0;
    var259 = 0;
    var260 = JSVAL_NULL;
    var261 = NULL;
    var341 = JSVAL_NULL;
    var262 = 0;
    var263 = 0;
    var265 = 0;
    var264 = NULL;
    var342 = JSVAL_NULL;
    var266 = 0;
    var267 = JSVAL_NULL;
    var268 = NULL;
    var343 = JSVAL_NULL;
    var269 = 0;
    var271 = NULL;
    var272 = 0;
    var298 = 0;
    var273 = 0;
    var274 = NULL;
    var275 = 0;
    var281 = NULL;
    var299 = 0;
    var276 = 0;
    var277 = 0;
    var278 = JS_FALSE;
    var279 = JSVAL_NULL;
    var280 = NULL;
    var344 = JSVAL_NULL;
    var284 = 0;
    var282 = 0;
    var283 = 0;
    var286 = NULL;
    var285 = 0;
    var287 = 0;
    var296 = 0;
    var297 = 0;
    var288 = NULL;
    var300 = 0;
    var289 = NULL;
    var290 = 0;
    var301 = 0;
    var293 = NULL;
    var291 = 0;
    var292 = 0;
    var295 = NULL;
    var294 = 0;
    var302 = 0;
    var303 = 0;
    var305 = 0;
    var304 = NULL;
    var345 = JSVAL_NULL;
    var306 = JSVAL_NULL;
    var251 = JS_FALSE;
    var252 = obj;
    var256 = argc;
    var259 = 0;
    var259 = var259 < var256;
    if (var259) {
    var260 = argv[0];
    var261 = JS_ValueToString(cx, var260);
    if (!var261) {
        goto do_return;
    }
    var341 = STRING_TO_JSVAL(var261);
    argv[argc+0] = var341;
    var262 = JS_GetStringLength(var261);
    var263 = 1;
    var263 += var262;
    var257 = JS_malloc(cx, var263);
    if (!var257) {
        goto do_return;
    }
    var265 = 1;
    var264 = JS_GetStringChars(var261);
    var342 = STRING_TO_JSVAL(var261);
    argv[argc+1] = var342;
    {
        size_t i;
        for (i = 0; i < var262; ++i) {
            var257[i] = wctob(var264[i]);
        }
        var257[var262] = '\0';
    }
    }
    var266 = 1;
    var266 = var266 < var256;
    if (var266) {
    var267 = argv[1];
    if (JS_ValueToObject(cx, var267, &var268) != JS_TRUE) {
        goto do_return;
    }
    var343 = OBJECT_TO_JSVAL(var268);
    argv[argc+2] = var343;
    if (JS_GetArrayLength(cx, var268, &var269) != JS_TRUE) {
        goto do_return;
    }
    var272 = sizeof(var271);
    var272 *= var269;
    var258 = JS_malloc(cx, var272);
    if (!var258) {
        goto do_return;
    }
    var298 = 1;
    var273 = var269;
    var275 = sizeof(var274);
    var273 *= var275;
    var281 = JS_malloc(cx, var273);
    if (!var281) {
        goto do_return;
    }
    var299 = 1;
    var276 = var269;
    var277 = -1;
    while (var276)
    {
    var276 += var277;
    var278 = JS_GetElement(cx, var268, var276, &var279);
    var280 = JS_ValueToString(cx, var279);
    if (!var280) {
        goto do_return;
    }
    var344 = STRING_TO_JSVAL(var280);
    argv[argc+3] = var344;
    var281[var276] = var280;
    }
    var284 = 0;
    var282 = var269;
    var283 = -1;
    while (var282)
    {
    var282 += var283;
    var286 = var281[var282];
    var285 = JS_GetStringLength(var286);
    var284 += var285;
    var287 = 1;
    var284 += var287;
    }
    var297 = sizeof(var296);
    var297 *= var284;
    var288 = JS_malloc(cx, var297);
    if (!var288) {
        goto do_return;
    }
    var300 = 1;
    var290 = sizeof(var289);
    var290 *= var269;
    var258 = JS_malloc(cx, var290);
    if (!var258) {
        goto do_return;
    }
    var301 = 1;
    var293 = var288;
    var293 += var284;
    var291 = var269;
    var292 = -1;
    while (var291)
    {
    var291 += var292;
    var295 = var281[var291];
    var294 = JS_GetStringLength(var295);
    var293 -= var294;
    var293 += var292;
    var302 = JS_GetStringLength(var295);
    var303 = 1;
    var303 += var302;
    var293 = JS_malloc(cx, var303);
    if (!var293) {
        goto do_return;
    }
    var305 = 1;
    var304 = JS_GetStringChars(var295);
    var345 = STRING_TO_JSVAL(var295);
    argv[argc+4] = var345;
    {
        size_t i;
        for (i = 0; i < var302; ++i) {
            var293[i] = wctob(var304[i]);
        }
        var293[var302] = '\0';
    }
    var258[var291] = var293;
    }
    }
    var253 = jsSetContainer(var257, var258);
    if (JS_NewNumberValue(cx, var253, &var306) != JS_TRUE) {
        goto do_return;
    }
    argv[argc+5] = var306;
    if (rval) {
        *rval = var306;
    }
    var251 = JS_TRUE;
    do_return:
    if (var305) {
        JS_free(cx, var293);
        var293 = NULL;
        var305 = 0;
    }
    if (var301) {
        JS_free(cx, var258);
        var258 = NULL;
        var301 = 0;
    }
    if (var300) {
        JS_free(cx, var288);
        var288 = NULL;
        var300 = 0;
    }
    if (var299) {
        JS_free(cx, var281);
        var281 = NULL;
        var299 = 0;
    }
    if (var298) {
        JS_free(cx, var258);
        var258 = NULL;
        var298 = 0;
    }
    if (var265) {
        JS_free(cx, var257);
        var257 = NULL;
        var265 = 0;
    }
    return var251;
}
static JSBool
jjadm__construct__(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *var308;
    int var312;
    JSBool var307;
    var308 = NULL;
    var312 = 0;
    var307 = JS_FALSE;
    var308 = obj;
    var312 = argc;
    jsAvidemux();
    var307 = JS_TRUE;
    return var307;
}
static JSPropertySpec jjadm_static_ps[] = {
    {NULL, 0, 0, NULL, NULL}
};
static JSPropertySpec jjadm_ps[] = {
    {NULL, 0, 0, NULL, NULL}
};
static JSFunctionSpec jjadm_static_fs[] = {
    JS_FS("loadVideo", jjadmloadVideo, 1, 0, 3),
    JS_FS("clearSegments", jjadmclearSegments, 0, 0, 1),
    JS_FS("appendVideo", jjadmappendVideo, 1, 0, 3),
    JS_FS("addSegment", jjadmaddSegment, 3, 0, 1),
    JS_FS("setPostProc", jjadmsetPostProc, 3, 0, 1),
    JS_FS("audioReset", jjadmaudioReset, 0, 0, 1),
    JS_FS("videoCodec", jsAdmvideoCodec,  2, 0, 6),
    JS_FS("addVideoFilter", jsAdmaddVideoFilter,  2, 0, 6),
    JS_FS("audioCodec", jsAdmaudioCodec,  2, 0, 6),
    JS_FS("setContainer", jsAdmsetContainer,  2, 0, 6),
    JS_FS_END
};
static JSFunctionSpec jjadm_fs[] = {
    JS_FS_END
};
static JSClass jjadm_class = {
    "adm",
    0,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    JS_FinalizeStub,
    NULL,
    NULL,
    NULL,
    jjadm__construct__,
    NULL,
    NULL,
    NULL,
    NULL
};
static JSObject *
jjadm_init(JSContext *cx, JSObject *obj)
{
    JSObject *parent_proto, *proto;
    JSClass *class;
    if (!cx || !obj) {
        JS_ReportError(cx, "invalid parameter");
        return NULL;
    }
    if (!JS_EnterLocalRootScope(cx)) {
        JS_ReportError(cx, "JS_EnterLocalRootScope failed");
        return NULL;
    }
    parent_proto = NULL;
    proto = NULL;
    parent_proto = JS_NewObject(cx, NULL, NULL, NULL);
    if (!parent_proto) {
        JS_LeaveLocalRootScope(cx);
        JS_ReportError(cx, "failed to create prototype");
        return NULL;
    }
    class = &jjadm_class;
    proto = JS_InitClass(cx, obj, parent_proto, class, jjadm__construct__, 0, jjadm_ps, jjadm_fs, jjadm_static_ps, jjadm_static_fs);
    if (!proto) {
        JS_LeaveLocalRootScope(cx);
        JS_ReportError(cx, "failed to init class");
        return NULL;
    }
    JS_LeaveLocalRootScope(cx);
    return proto;
}


JSObject *jsAvidemuxInit(JSContext *cx,JSObject *obj)
{
          if (JS_DefineFunctions(cx, obj, jj_static_fs) != JS_TRUE) 
          {
                return NULL;
          }
          return jjadm_init(cx,obj);
}

