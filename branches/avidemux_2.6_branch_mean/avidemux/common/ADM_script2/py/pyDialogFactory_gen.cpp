// Generated by admPyClass.pl do not edit !
// show -> int run (void ) 
static tp_obj zzpy_show(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  ADM_scriptDialogFactoryHelper *me=(ADM_scriptDialogFactoryHelper *)pm.asThis(&self,ADM_PYID_DIALOGF);
  int r=  me->run(); 
  return tp_number(r);
}
// addControl -> void addControl (ptr@ADM_scriptDFBaseHelper ) 
static tp_obj zzpy_addControl(TP)
 {
  tp_obj self=tp_getraw( tp);
  tinyParams pm(tp);
  ADM_scriptDialogFactoryHelper *me=(ADM_scriptDialogFactoryHelper *)pm.asThis(&self,ADM_PYID_DIALOGF);
  ADM_scriptDFBaseHelper *p0= ( ADM_scriptDFBaseHelper *)pm.asObjectPointer();
  me->addControl(p0); 
 return tp_None;
}
tp_obj zzpy__pyDialogFactory_get(tp_vm *vm)
{
  tp_obj self=tp_getraw( vm);
  tinyParams pm(vm);
  ADM_scriptDialogFactoryHelper *me=(ADM_scriptDialogFactoryHelper *)pm.asThis(&self,ADM_PYID_DIALOGF);
  char const *key = pm.asString();
  if (!strcmp(key, "show"))
  {
     return tp_method(vm,self,zzpy_show);
  }
  if (!strcmp(key, "addControl"))
  {
     return tp_method(vm,self,zzpy_addControl);
  }
  return tp_get(vm,self,tp_string(key));
}
tp_obj zzpy__pyDialogFactory_set(tp_vm *vm)
{
  tp_obj self=tp_getraw( vm);
  tinyParams pm(vm);
  ADM_scriptDialogFactoryHelper *me=(ADM_scriptDialogFactoryHelper *)pm.asThis(&self,ADM_PYID_DIALOGF);
  char const *key = pm.asString();
  return tp_None;
}
// Dctor
static void myDtorpyDialogFactory(tp_vm *vm,tp_obj self)
{
  ADM_scriptDialogFactoryHelper *cookie=(ADM_scriptDialogFactoryHelper *)self.data.val;
  if(cookie) delete cookie;
  self.data.val=NULL;
}
// Ctor (str)
static tp_obj myCtorpyDialogFactory(tp_vm *vm)
{
  tp_obj self = tp_getraw(vm);
  tinyParams pm(vm);
  const char *p0= pm.asString();
  ADM_scriptDialogFactoryHelper *me=new ADM_scriptDialogFactoryHelper(p0);
  tp_obj cdata = tp_data(vm, ADM_PYID_DIALOGF, me);
  cdata.data.info->xfree = myDtorpyDialogFactory;
  tp_set(vm, self, tp_string("cdata"), cdata);
  return tp_None;
}
static tp_obj zzpy__pyDialogFactory_help(TP)
 {
	jsLog("show(void)\n");
	jsLog("addControl(ptr@ADM_scriptDFBaseHelper)\n");
return tp_None;
};
tp_obj initClasspyDialogFactory(tp_vm *vm)
{
  tp_obj myClass=tp_class(vm);
  tp_set(vm,myClass,tp_string("__init__"),tp_fnc(vm,myCtorpyDialogFactory));
  tp_set(vm,myClass,tp_string("__set__"),tp_fnc(vm,zzpy__pyDialogFactory_set));
  tp_set(vm,myClass,tp_string("__get__"),tp_fnc(vm,zzpy__pyDialogFactory_get));
  tp_set(vm,myClass,tp_string("help"),tp_fnc(vm,zzpy__pyDialogFactory_help));
  return myClass;
}
