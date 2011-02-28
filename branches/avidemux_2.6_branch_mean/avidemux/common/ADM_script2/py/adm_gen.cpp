// Generated by admPyClass.pl do not edit !
// audioCodec -> int scriptSetAudioCodec (str int couples ) 
static tp_obj zzpy_audioCodec(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  int p1= pm.asInt();
  CONFcouple *p2=NULL;
  pm.makeCouples(&p2);
  int r=  scriptSetAudioCodec(p0,p1,p2); 
  return tp_number(r);
}
// saveBmp -> int A_saveImg (str ) 
static tp_obj zzpy_saveBmp(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  int r=  A_saveImg(p0); 
  return tp_number(r);
}
// addVideoFilter -> int scriptAddVideoFilter (str couples ) 
static tp_obj zzpy_addVideoFilter(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
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
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  int r=  scriptLoadVideo(p0); 
  return tp_number(r);
}
// getPARWidth -> int scriptGetPARWidth (void ) 
static tp_obj zzpy_getPARWidth(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int r=  scriptGetPARWidth(); 
  return tp_number(r);
}
// clearSegments -> int scriptClearSegments (void ) 
static tp_obj zzpy_clearSegments(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int r=  scriptClearSegments(); 
  return tp_number(r);
}
// getHeight -> int scriptGetHeight (void ) 
static tp_obj zzpy_getHeight(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int r=  scriptGetHeight(); 
  return tp_number(r);
}
// setPostProc -> int scriptSetPostProc (int  int   int  ) 
static tp_obj zzpy_setPostProc(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int p0= pm.asInt();
  int p1= pm.asInt();
  int p2= pm.asInt();
  int r=  scriptSetPostProc(p0,p1,p2); 
  return tp_number(r);
}
// save -> int A_Save (str ) 
static tp_obj zzpy_save(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  int r=  A_Save(p0); 
  return tp_number(r);
}
// videoCodecChangeParam -> int scriptSetVideoCodecParam (str couples ) 
static tp_obj zzpy_videoCodecChangeParam(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  CONFcouple *p1=NULL;
  pm.makeCouples(&p1);
  int r=  scriptSetVideoCodecParam(p0,p1); 
  return tp_number(r);
}
// appendVideo -> int scriptAppendVideo (str  ) 
static tp_obj zzpy_appendVideo(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  int r=  scriptAppendVideo(p0); 
  return tp_number(r);
}
// audioMixer -> int scriptAudioMixer (str  ) 
static tp_obj zzpy_audioMixer(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  int r=  scriptAudioMixer(p0); 
  return tp_number(r);
}
// getFps1000 -> int scriptGetFps1000 (void ) 
static tp_obj zzpy_getFps1000(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int r=  scriptGetFps1000(); 
  return tp_number(r);
}
// saveAudio -> int A_audioSave (str ) 
static tp_obj zzpy_saveAudio(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  int r=  A_audioSave(p0); 
  return tp_number(r);
}
// videoCodec -> int scriptSetVideoCodec (str couples ) 
static tp_obj zzpy_videoCodec(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  CONFcouple *p1=NULL;
  pm.makeCouples(&p1);
  int r=  scriptSetVideoCodec(p0,p1); 
  return tp_number(r);
}
// getWidth -> int scriptGetWidth (void ) 
static tp_obj zzpy_getWidth(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int r=  scriptGetWidth(); 
  return tp_number(r);
}
// addSegment -> int scriptAddSegment (int  double   double  ) 
static tp_obj zzpy_addSegment(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int p0= pm.asInt();
  double p1= pm.asDouble();
  double p2= pm.asDouble();
  int r=  scriptAddSegment(p0,p1,p2); 
  return tp_number(r);
}
// clearVideoFilters -> int scriptClearVideoFilters (void ) 
static tp_obj zzpy_clearVideoFilters(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int r=  scriptClearVideoFilters(); 
  return tp_number(r);
}
// saveJpeg -> int A_saveJpg (str ) 
static tp_obj zzpy_saveJpeg(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  int r=  A_saveJpg(p0); 
  return tp_number(r);
}
// setAudioTrack -> int scriptAudioSetTrack (int  ) 
static tp_obj zzpy_setAudioTrack(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int p0= pm.asInt();
  int r=  scriptAudioSetTrack(p0); 
  return tp_number(r);
}
// setContainer -> int scriptSetContainer (str couples ) 
static tp_obj zzpy_setContainer(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  const char *p0= pm.asString();
  CONFcouple *p1=NULL;
  pm.makeCouples(&p1);
  int r=  scriptSetContainer(p0,p1); 
  return tp_number(r);
}
// audioReset -> int scriptAudioReset (void ) 
static tp_obj zzpy_audioReset(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int r=  scriptAudioReset(); 
  return tp_number(r);
}
// getVideoCodec -> str scriptGetVideoCodec (void ) 
static tp_obj zzpy_getVideoCodec(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  char *r=  scriptGetVideoCodec(); 
  if(!r) pm.raise("pyAdm : null pointer");
  
  tp_obj o=tp_string_copy(tp,r,  strlen(r));
  ADM_dealloc(r);
  return o;
}
// getPARHeight -> int scriptGetPARHeight (void ) 
static tp_obj zzpy_getPARHeight(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  int r=  scriptGetPARHeight(); 
  return tp_number(r);
}
tp_obj zzpy__pyAdm_get(tp_vm *vm)
{
  tp_obj self=tp_getraw( vm);
  tinyParams pm(vm);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  char const *key = pm.asString();
  if (!strcmp(key, "audioFilm2pal"))
  {
     return tp_number(scriptGetFilm2pal());
  }
  if (!strcmp(key, "audioEncoding"))
  {
     return tp_number(scriptGetAudioEncoding());
  }
  if (!strcmp(key, "audioPal2film"))
  {
     return tp_number(scriptGetPal2film());
  }
  if (!strcmp(key, "audioResample"))
  {
     return tp_number(scriptGetResample());
  }
  if (!strcmp(key, "markerA"))
  {
     return tp_number(scriptGetMarkerA());
  }
  if (!strcmp(key, "audioNormalizeMode"))
  {
     return tp_number(scriptGetNormalizeMode());
  }
  if (!strcmp(key, "audioFrequency"))
  {
     return tp_number(scriptGetAudioFrequency());
  }
  if (!strcmp(key, "audioChannels"))
  {
     return tp_number(scriptGetAudioChannels());
  }
  if (!strcmp(key, "markerB"))
  {
     return tp_number(scriptGetMarkerB());
  }
  if (!strcmp(key, "audioNormalizeGain"))
  {
     return tp_number(scriptGetNormalizeValue());
  }
  if (!strcmp(key, "audioCodec"))
  {
     return tp_method(vm,self,zzpy_audioCodec);
  }
  if (!strcmp(key, "saveBmp"))
  {
     return tp_method(vm,self,zzpy_saveBmp);
  }
  if (!strcmp(key, "addVideoFilter"))
  {
     return tp_method(vm,self,zzpy_addVideoFilter);
  }
  if (!strcmp(key, "loadVideo"))
  {
     return tp_method(vm,self,zzpy_loadVideo);
  }
  if (!strcmp(key, "getPARWidth"))
  {
     return tp_method(vm,self,zzpy_getPARWidth);
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
  if (!strcmp(key, "save"))
  {
     return tp_method(vm,self,zzpy_save);
  }
  if (!strcmp(key, "videoCodecChangeParam"))
  {
     return tp_method(vm,self,zzpy_videoCodecChangeParam);
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
  if (!strcmp(key, "saveAudio"))
  {
     return tp_method(vm,self,zzpy_saveAudio);
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
  if (!strcmp(key, "saveJpeg"))
  {
     return tp_method(vm,self,zzpy_saveJpeg);
  }
  if (!strcmp(key, "setAudioTrack"))
  {
     return tp_method(vm,self,zzpy_setAudioTrack);
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
  if (!strcmp(key, "getPARHeight"))
  {
     return tp_method(vm,self,zzpy_getPARHeight);
  }
  return tp_get(vm,self,tp_string(key));
}
tp_obj zzpy__pyAdm_set(tp_vm *vm)
{
  tp_obj self=tp_getraw( vm);
  tinyParams pm(vm);
  void *me=(void *)pm.asThis(&self,ADM_PYID_AVIDEMUX);
  char const *key = pm.asString();
  if (!strcmp(key, "audioFilm2pal"))
  {
     int val=pm.asInt();
     scriptSetFilm2pal(val);
     return tp_None;
  }
  if (!strcmp(key, "audioEncoding"))
  {
     int val=pm.asInt();
     scriptSetAudioEncoding(val);
     return tp_None;
  }
  if (!strcmp(key, "audioPal2film"))
  {
     int val=pm.asInt();
     scriptSetPal2film(val);
     return tp_None;
  }
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
  if (!strcmp(key, "audioNormalizeMode"))
  {
     int val=pm.asInt();
     scriptSetNormalizeMode(val);
     return tp_None;
  }
  if (!strcmp(key, "audioFrequency"))
  {
     int val=pm.asInt();
     scriptSetAudioFrequency(val);
     return tp_None;
  }
  if (!strcmp(key, "audioChannels"))
  {
     int val=pm.asInt();
     scriptSetAudioChannels(val);
     return tp_None;
  }
  if (!strcmp(key, "markerB"))
  {
     double val=pm.asDouble();
     scriptSetMarkerB(val);
     return tp_None;
  }
  if (!strcmp(key, "audioNormalizeGain"))
  {
     int val=pm.asInt();
     scriptSetNormalizeValue(val);
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
  tinyParams pm(vm);
  void *me=NULL;
  tp_obj cdata = tp_data(vm, ADM_PYID_AVIDEMUX, me);
  cdata.data.info->xfree = myDtorpyAdm;
  tp_set(vm, self, tp_string("cdata"), cdata);
  return tp_None;
}
static tp_obj zzpy__pyAdm_help(TP)
 {
	jsLog("audioCodec(str,int,couples)\n");
	jsLog("saveBmp(str)\n");
	jsLog("addVideoFilter(str,couples)\n");
	jsLog("loadVideo(str )\n");
	jsLog("getPARWidth(void)\n");
	jsLog("clearSegments(void)\n");
	jsLog("getHeight(void)\n");
	jsLog("setPostProc(int ,int , int )\n");
	jsLog("save(str)\n");
	jsLog("videoCodecChangeParam(str,couples)\n");
	jsLog("appendVideo(str )\n");
	jsLog("audioMixer(str )\n");
	jsLog("getFps1000(void)\n");
	jsLog("saveAudio(str)\n");
	jsLog("videoCodec(str,couples)\n");
	jsLog("getWidth(void)\n");
	jsLog("addSegment(int ,double , double )\n");
	jsLog("clearVideoFilters(void)\n");
	jsLog("saveJpeg(str)\n");
	jsLog("setAudioTrack(int )\n");
	jsLog("setContainer(str,couples)\n");
	jsLog("audioReset(void)\n");
	jsLog("getVideoCodec(void)\n");
	jsLog("getPARHeight(void)\n");
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
