// Generated by admPyClass.pl do not edit !
// environ -> str pyGetEnv (IEditor str ) 
static tp_obj zzpy_environ(TP)
 {
  tp_obj self = tp_getraw(tp);
  IScriptEngine *engine = (IScriptEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(tp);
  void *me = (void *)pm.asThis(&self, ADM_PYID_OS);

  IEditor *p0 = editor;
  const char *p1 = pm.asString();
  char *r =   pyGetEnv(p0,p1); 
  if(!r) pm.raise("pyOS : null pointer");

  tp_obj o = tp_string_copy(tp, r, strlen(r));
  ADM_dealloc(r);
  return o;
}
tp_obj zzpy__pyOS_get(tp_vm *vm)
{
  tp_obj self = tp_getraw(vm);
  IScriptEngine *engine = (IScriptEngine*)tp_get(vm, vm->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(vm);
  void *me=(void *)pm.asThis(&self, ADM_PYID_OS);
  char const *key = pm.asString();
  if (!strcmp(key, "environ"))
  {
     return tp_method(vm, self, zzpy_environ);
  }
  return tp_get(vm, self, tp_string(key));
}
tp_obj zzpy__pyOS_set(tp_vm *vm)
{
  tp_obj self = tp_getraw(vm);
  IScriptEngine *engine = (IScriptEngine*)tp_get(vm, vm->builtins, tp_string("userdata")).data.val;
  IEditor *editor = engine->editor();
  TinyParams pm(vm);
  void *me = (void *)pm.asThis(&self, ADM_PYID_OS);
  char const *key = pm.asString();
  return tp_None;
}
// Dctor
static void myDtorpyOS(tp_vm *vm,tp_obj self)
{
}
// Ctor (str)
static tp_obj myCtorpyOS(tp_vm *vm)
{
  tp_obj self = tp_getraw(vm);
  TinyParams pm(vm);
  void *me = NULL;
  tp_obj cdata = tp_data(vm, ADM_PYID_OS, me);
  cdata.data.info->xfree = myDtorpyOS;
  tp_set(vm, self, tp_string("cdata"), cdata);
  return tp_None;
}
static tp_obj zzpy__pyOS_help(TP)
{
	PythonEngine *engine = (PythonEngine*)tp_get(tp, tp->builtins, tp_string("userdata")).data.val;

	engine->callEventHandlers(IScriptEngine::Information, NULL, -1, "environ(IEditor,str)\n");

	return tp_None;
};
tp_obj initClasspyOS(tp_vm *vm)
{
  tp_obj myClass = tp_class(vm);
  tp_set(vm,myClass, tp_string("__init__"), tp_fnc(vm,myCtorpyOS));
  tp_set(vm,myClass, tp_string("__set__"), tp_fnc(vm,zzpy__pyOS_set));
  tp_set(vm,myClass, tp_string("__get__"), tp_fnc(vm,zzpy__pyOS_get));
  tp_set(vm,myClass, tp_string("help"), tp_fnc(vm,zzpy__pyOS_help));
  return myClass;
}
