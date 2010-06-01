// Generated by admPyClass.pl do not edit !
// audioCodec -> int scriptSetAudioCodec (str int couples ) 
static tp_obj zzpy_audioCodec(TP)
 {
  tinyParams pm(tp);
  const char *p0= pm.asString();
  int p1= pm.asDouble();
  CONFcouple *p2=NULL;
  pm.makeCouples(&p2);
  int r=  scriptSetAudioCodec(p0,p1,p2); 
  return tp_number(r);
}
// addVideoFilter -> int scriptAddVideoFilter (str couples ) 
static tp_obj zzpy_addVideoFilter(TP)
 {
  tinyParams pm(tp);
  const char *p0= pm.asString();
  CONFcouple *p1=NULL;
  pm.makeCouples(&p1);
  int r=  scriptAddVideoFilter(p0,p1); 
  return tp_number(r);
}
// loadVideo -> int scriptLoadVideo (str  ) 
static tp_obj zzpy_loadVideo(TP)
 {
  tinyParams pm(tp);
  const char *p0= pm.asString();
  int r=  scriptLoadVideo(p0); 
  return tp_number(r);
}
// clearSegments -> int scriptClearSegments (void ) 
static tp_obj zzpy_clearSegments(TP)
 {
  int r=  scriptClearSegments(); 
  return tp_number(r);
}
// getHeight -> int scriptGetHeight (void ) 
static tp_obj zzpy_getHeight(TP)
 {
  int r=  scriptGetHeight(); 
  return tp_number(r);
}
// setPostProc -> int scriptSetPostProc (int  int   int  ) 
static tp_obj zzpy_setPostProc(TP)
 {
  tinyParams pm(tp);
  int p0= pm.asDouble();
  int p1= pm.asDouble();
  int p2= pm.asDouble();
  int r=  scriptSetPostProc(p0,p1,p2); 
  return tp_number(r);
}
// appendVideo -> int scriptAppendVideo (str  ) 
static tp_obj zzpy_appendVideo(TP)
 {
  tinyParams pm(tp);
  const char *p0= pm.asString();
  int r=  scriptAppendVideo(p0); 
  return tp_number(r);
}
// audioMixer -> int scriptAudioMixer (str  ) 
static tp_obj zzpy_audioMixer(TP)
 {
  tinyParams pm(tp);
  const char *p0= pm.asString();
  int r=  scriptAudioMixer(p0); 
  return tp_number(r);
}
// getFps1000 -> int scriptGetFps1000 (void ) 
static tp_obj zzpy_getFps1000(TP)
 {
  int r=  scriptGetFps1000(); 
  return tp_number(r);
}
// videoCodec -> int scriptSetVideoCodec (str couples ) 
static tp_obj zzpy_videoCodec(TP)
 {
  tinyParams pm(tp);
  const char *p0= pm.asString();
  CONFcouple *p1=NULL;
  pm.makeCouples(&p1);
  int r=  scriptSetVideoCodec(p0,p1); 
  return tp_number(r);
}
// getWidth -> int scriptGetWidth (void ) 
static tp_obj zzpy_getWidth(TP)
 {
  int r=  scriptGetWidth(); 
  return tp_number(r);
}
// addSegment -> int scriptAddSegment (int  float   float  ) 
static tp_obj zzpy_addSegment(TP)
 {
  tinyParams pm(tp);
  int p0= pm.asDouble();
  float p1= pm.asFloat();
  float p2= pm.asFloat();
  int r=  scriptAddSegment(p0,p1,p2); 
  return tp_number(r);
}
// clearVideoFilters -> int scriptClearVideoFilters (void ) 
static tp_obj zzpy_clearVideoFilters(TP)
 {
  int r=  scriptClearVideoFilters(); 
  return tp_number(r);
}
// setContainer -> int scriptSetContainer (str couples ) 
static tp_obj zzpy_setContainer(TP)
 {
  tinyParams pm(tp);
  const char *p0= pm.asString();
  CONFcouple *p1=NULL;
  pm.makeCouples(&p1);
  int r=  scriptSetContainer(p0,p1); 
  return tp_number(r);
}
// audioReset -> int scriptAudioReset (void ) 
static tp_obj zzpy_audioReset(TP)
 {
  int r=  scriptAudioReset(); 
  return tp_number(r);
}
// getVideoCodec -> str scriptGetVideoCodec (void ) 
static tp_obj zzpy_getVideoCodec(TP)
 {
  char *r=  scriptGetVideoCodec(); 
  return tp_string(r);
}
static tp_obj myCtorpyAdm(tp_vm *vm)
{
}
static tp_obj zzpy__pyAdm_help(TP)
 {
  jsLog("audioCodec(str,int,couples)");
  jsLog("addVideoFilter(str,couples)");
  jsLog("loadVideo(str )");
  jsLog("clearSegments(void)");
  jsLog("getHeight(void)");
  jsLog("setPostProc(int ,int , int )");
  jsLog("appendVideo(str )");
  jsLog("audioMixer(str )");
  jsLog("getFps1000(void)");
  jsLog("videoCodec(str,couples)");
  jsLog("getWidth(void)");
  jsLog("addSegment(int ,float , float )");
  jsLog("clearVideoFilters(void)");
  jsLog("setContainer(str,couples)");
  jsLog("audioReset(void)");
  jsLog("getVideoCodec(void)");
};
tp_obj initClasspyAdm(tp_vm *vm)
{
  tp_obj myClass=tp_class(vm);
  tp_set(vm,myClass,tp_string("__init__"),tp_fnc(vm,myCtorpyAdm));
  tp_set(vm,myClass,tp_string("help"),tp_fnc(vm,zzpy__pyAdm_help));
  tp_set(vm,myClass,tp_string("audioCodec"),tp_fnc(vm,zzpy_audioCodec));
  tp_set(vm,myClass,tp_string("addVideoFilter"),tp_fnc(vm,zzpy_addVideoFilter));
  tp_set(vm,myClass,tp_string("loadVideo"),tp_fnc(vm,zzpy_loadVideo));
  tp_set(vm,myClass,tp_string("clearSegments"),tp_fnc(vm,zzpy_clearSegments));
  tp_set(vm,myClass,tp_string("getHeight"),tp_fnc(vm,zzpy_getHeight));
  tp_set(vm,myClass,tp_string("setPostProc"),tp_fnc(vm,zzpy_setPostProc));
  tp_set(vm,myClass,tp_string("appendVideo"),tp_fnc(vm,zzpy_appendVideo));
  tp_set(vm,myClass,tp_string("audioMixer"),tp_fnc(vm,zzpy_audioMixer));
  tp_set(vm,myClass,tp_string("getFps1000"),tp_fnc(vm,zzpy_getFps1000));
  tp_set(vm,myClass,tp_string("videoCodec"),tp_fnc(vm,zzpy_videoCodec));
  tp_set(vm,myClass,tp_string("getWidth"),tp_fnc(vm,zzpy_getWidth));
  tp_set(vm,myClass,tp_string("addSegment"),tp_fnc(vm,zzpy_addSegment));
  tp_set(vm,myClass,tp_string("clearVideoFilters"),tp_fnc(vm,zzpy_clearVideoFilters));
  tp_set(vm,myClass,tp_string("setContainer"),tp_fnc(vm,zzpy_setContainer));
  tp_set(vm,myClass,tp_string("audioReset"),tp_fnc(vm,zzpy_audioReset));
  tp_set(vm,myClass,tp_string("getVideoCodec"),tp_fnc(vm,zzpy_getVideoCodec));
  return myClass;
}
