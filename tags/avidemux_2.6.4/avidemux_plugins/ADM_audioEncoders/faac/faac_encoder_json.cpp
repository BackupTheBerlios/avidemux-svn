// automatically generated by admSerialization.py, do not edit!
#include "ADM_default.h"
#include "ADM_paramList.h"
#include "ADM_coreJson.h"
#include "faac_encoder.h"
bool  faac_encoder_jserialize(const char *file, const faac_encoder *key){
admJson json;
json.addUint32("bitrate",key->bitrate);
return json.dumpToFile(file);
};
bool  faac_encoder_jdeserialize(const char *file, const ADM_paramList *tmpl,faac_encoder *key){
admJsonToCouple json;
CONFcouple *c=json.readFromFile(file);
if(!c) {ADM_error("Cannot read json file");return false;}
bool r= ADM_paramLoadPartial(c,tmpl,key);
delete c;
return r;
};
