// Generated by admPyClass.pl do not edit !
// audioCodec -> int scriptSetAudioCodec (str int couples ) 
static tp_obj zzpy_audioCodec(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=pm.asThis(&self,100);
  const char *p0= pm.asString();
  int p1= pm.asInt();
  CONFcouple *p2=NULL;
  pm.makeCouples(&p2);
  int r=  scriptSetAudioCodec(p0,p1,p2); 
  return tp_number(r);
}
// addVideoFilter -> int scriptAddVideoFilter (str couples ) 
static tp_obj zzpy_addVideoFilter(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=pm.asThis(&self,100);
  const char *p0= pm.asString();
  CONFcouple *p1=NULL;
  pm.makeCouples(&p1);
  int r=  scriptAddVideoFilter(p0,p1); 
  return tp_number(r);
}
// loadVideo -> int scriptLoadVideo (str  ) 
static tp_obj zzpy_loadVideo(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=pm.asThis(&self,100);
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
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=pm.asThis(&self,100);
  int p0= pm.asInt();
  int p1= pm.asInt();
  int p2= pm.asInt();
  int r=  scriptSetPostProc(p0,p1,p2); 
  return tp_number(r);
}
// appendVideo -> int scriptAppendVideo (str  ) 
static tp_obj zzpy_appendVideo(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=pm.asThis(&self,100);
  const char *p0= pm.asString();
  int r=  scriptAppendVideo(p0); 
  return tp_number(r);
}
// audioMixer -> int scriptAudioMixer (str  ) 
static tp_obj zzpy_audioMixer(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=pm.asThis(&self,100);
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
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=pm.asThis(&self,100);
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
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=pm.asThis(&self,100);
  int p0= pm.asInt();
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
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=pm.asThis(&self,100);
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
tp_obj zzpy__pyAdm_get(tp_vm *vm)
{
  tp_obj self=tp_getraw( vm);
  tinyParams pm(vm);
  void *me=(void *)pm.asThis(&self,100);
  char const *key = pm.asString();
  if (!strcmp(key, "audioResample"))
  {
     return tp_number(scriptGetResample());
  }
  if (!strcmp(key, "markerA"))
  {
     return tp_number(scriptGetMarkerA());
  }
  if (!strcmp(key, "markerB"))
  {
     return tp_number(scriptGetMarkerB());
  }
  if (!strcmp(key, "audioCodec"))
  {
     return tp_method(vm,self,zzpy_audioCodec);
  }
  if (!strcmp(key, "addVideoFilter"))
  {
     return tp_method(vm,self,zzpy_addVideoFilter);
  }
  if (!strcmp(key, "loadVideo"))
  {
     return tp_method(vm,self,zzpy_loadVideo);
  }
  if (!strcmp(key, "clearSegments"))
  {
     return tp_method(vm,self,zzpy_clearSegments);
  }
  if (!strcmp(key, "getHeight"))
  {
     return tp_method(vm,self,zzpy_getHeight);
  }
  if (!strcmp(key, "setPostProc"))
  {
     return tp_method(vm,self,zzpy_setPostProc);
  }
  if (!strcmp(key, "appendVideo"))
  {
     return tp_method(vm,self,zzpy_appendVideo);
  }
  if (!strcmp(key, "audioMixer"))
  {
     return tp_method(vm,self,zzpy_audioMixer);
  }
  if (!strcmp(key, "getFps1000"))
  {
     return tp_method(vm,self,zzpy_getFps1000);
  }
  if (!strcmp(key, "videoCodec"))
  {
     return tp_method(vm,self,zzpy_videoCodec);
  }
  if (!strcmp(key, "getWidth"))
  {
     return tp_method(vm,self,zzpy_getWidth);
  }
  if (!strcmp(key, "addSegment"))
  {
     return tp_method(vm,self,zzpy_addSegment);
  }
  if (!strcmp(key, "clearVideoFilters"))
  {
     return tp_method(vm,self,zzpy_clearVideoFilters);
  }
  if (!strcmp(key, "setContainer"))
  {
     return tp_method(vm,self,zzpy_setContainer);
  }
  if (!strcmp(key, "audioReset"))
  {
     return tp_method(vm,self,zzpy_audioReset);
  }
  if (!strcmp(key, "getVideoCodec"))
  {
     return tp_method(vm,self,zzpy_getVideoCodec);
  }
  return tp_get(vm,self,tp_string(key));
}
tp_obj zzpy__pyAdm_set(tp_vm *vm)
{
  tp_obj self=tp_getraw( vm);
  tinyParams pm(vm);
  void *me=pm.asThis(&self,100);
  char const *key = pm.asString();
  if (!strcmp(key, "audioResample"))
  {
     int val=pm.asInt();
     scriptSetResample(val);
     return tp_None;
  }
  if (!strcmp(key, "markerA"))
  {
     double val=pm.asDouble();
     scriptSetMarkerA(val);
     return tp_None;
  }
  if (!strcmp(key, "markerB"))
  {
     double val=pm.asDouble();
     scriptSetMarkerB(val);
     return tp_None;
  }
  return tp_None;
}
// Dctor
static void myDtorpyAdm(tp_vm *vm,tp_obj self)
{
}
// Ctor ()
static tp_obj myCtorpyAdm(tp_vm *vm)
{
  tp_obj self = tp_getraw(vm);
  void *me=NULL;
  tp_obj cdata = tp_data(vm, 100, me);
  cdata.data.info->xfree = myDtorpyAdm;
  tp_set(vm, self, tp_string("cdata"), cdata);
  return tp_None;
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
  tp_set(vm,myClass,tp_string("__set__"),tp_fnc(vm,zzpy__pyAdm_set));
  tp_set(vm,myClass,tp_string("__get__"),tp_fnc(vm,zzpy__pyAdm_get));
  tp_set(vm,myClass,tp_string("help"),tp_fnc(vm,zzpy__pyAdm_help));
  return myClass;
}
