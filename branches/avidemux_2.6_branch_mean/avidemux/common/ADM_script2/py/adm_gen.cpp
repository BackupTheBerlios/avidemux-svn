//int  jsLoadVideo <char * >
tp_obj zzpy_loadVideo(TP)
{
char * p0=(char *)TP_STR().string.val;
int r=jsLoadVideo(p0); 
return tp_number(r);
}
//int  jsClearSegments <void>
tp_obj zzpy_clearSegments(TP)
{
int r=jsClearSegments(); 
return tp_number(r);
}
//int  jsAppendVideo <char * >
tp_obj zzpy_appendVideo(TP)
{
char * p0=(char *)TP_STR().string.val;
int r=jsAppendVideo(p0); 
return tp_number(r);
}
//int  jsAddSegment <int  float   float >
tp_obj zzpy_addSegment(TP)
{
int p0=TP_NUM();
float p1=TP_NUM();
float p2=TP_NUM();
int r=jsAddSegment(p0,p1,p2); 
return tp_number(r);
}
//int  jsSetPostProc <int  int   int >
tp_obj zzpy_setPostProc(TP)
{
int p0=TP_NUM();
int p1=TP_NUM();
int p2=TP_NUM();
int r=jsSetPostProc(p0,p1,p2); 
return tp_number(r);
}
//int  jsGetWidth <void>
tp_obj zzpy_getWidth(TP)
{
int r=jsGetWidth(); 
return tp_number(r);
}
//int  jsGetHeight <void>
tp_obj zzpy_getHeight(TP)
{
int r=jsGetHeight(); 
return tp_number(r);
}
//int  jsGetFps1000 <void>
tp_obj zzpy_getFps1000(TP)
{
int r=jsGetFps1000(); 
return tp_number(r);
}
//str  jsGetVideoCodec <void>
tp_obj zzpy_getVideoCodec(TP)
{
char *r=jsGetVideoCodec(); 
return tp_string(r);
}
//int  jsAudioReset <void>
tp_obj zzpy_audioReset(TP)
{
int r=jsAudioReset(); 
return tp_number(r);
}
//int  jsAudioMixer <char * >
tp_obj zzpy_audioMixer(TP)
{
char * p0=(char *)TP_STR().string.val;
int r=jsAudioMixer(p0); 
return tp_number(r);
}
//int  jsClearVideoFilters <void>
tp_obj zzpy_clearVideoFilters(TP)
{
int r=jsClearVideoFilters(); 
return tp_number(r);
}
static tp_obj myCtor(tp_vm *vm)
{
}
tp_obj initClasspyAdm(tp_vm *vm)
{
 tp_obj myClass=tp_class(vm);
 tp_set(vm,myClass,tp_string("__init__"),tp_fnc(vm,myCtor));
 tp_set(vm,myClass,tp_string("loadVideo"),tp_fnc(vm,zzpy_loadVideo));
 tp_set(vm,myClass,tp_string("clearSegments"),tp_fnc(vm,zzpy_clearSegments));
 tp_set(vm,myClass,tp_string("appendVideo"),tp_fnc(vm,zzpy_appendVideo));
 tp_set(vm,myClass,tp_string("addSegment"),tp_fnc(vm,zzpy_addSegment));
 tp_set(vm,myClass,tp_string("setPostProc"),tp_fnc(vm,zzpy_setPostProc));
 tp_set(vm,myClass,tp_string("getWidth"),tp_fnc(vm,zzpy_getWidth));
 tp_set(vm,myClass,tp_string("getHeight"),tp_fnc(vm,zzpy_getHeight));
 tp_set(vm,myClass,tp_string("getFps1000"),tp_fnc(vm,zzpy_getFps1000));
 tp_set(vm,myClass,tp_string("getVideoCodec"),tp_fnc(vm,zzpy_getVideoCodec));
 tp_set(vm,myClass,tp_string("audioReset"),tp_fnc(vm,zzpy_audioReset));
 tp_set(vm,myClass,tp_string("audioMixer"),tp_fnc(vm,zzpy_audioMixer));
 tp_set(vm,myClass,tp_string("clearVideoFilters"),tp_fnc(vm,zzpy_clearVideoFilters));
 return myClass;
}
