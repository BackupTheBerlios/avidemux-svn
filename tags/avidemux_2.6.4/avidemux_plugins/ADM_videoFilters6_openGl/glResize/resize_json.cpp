// automatically generated by admSerialization.py, do not edit!
#include "ADM_default.h"
#include "ADM_paramList.h"
#include "ADM_coreJson.h"
#include "resize.h"
bool  gl_resize_jserialize(const char *file, const gl_resize *key){
admJson json;
json.addUint32("width",key->width);
json.addUint32("height",key->height);
return json.dumpToFile(file);
};
bool  gl_resize_jdeserialize(const char *file, const ADM_paramList *tmpl,gl_resize *key){
admJsonToCouple json;
CONFcouple *c=json.readFromFile(file);
if(!c) {ADM_error("Cannot read json file");return false;}
bool r= ADM_paramLoadPartial(c,tmpl,key);
delete c;
return r;
};
