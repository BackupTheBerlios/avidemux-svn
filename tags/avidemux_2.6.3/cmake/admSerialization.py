#!/usr/bin/python
# Take a .conf file and generate :
# 1- Couple header   (.h)
# 2- Couple template (_desc.cpp)
# 3....
# (c) Mean 2011
#
import os
import re
import sys

structName=r'foostruct'
allCTypes=dict()
allATypes=dict()
allJTypes=dict()

allCTypes["uint32_t"]="uint32_t"
allCTypes["int32_t"]="int32_t"
allCTypes["bool"]="bool"
allCTypes["string"]="char *"
allCTypes["float"]="float"
allCTypes["video_encode"]="COMPRES_PARAMS"
allCTypes["lavcodec_context"]="FFcodecContext"

allJTypes["uint32_t"]="json.addUint32("
allJTypes["int32_t"]="json.addInt32("
allJTypes["bool"]="json.addBool("
allJTypes["string"]="json.addString("
allJTypes["float"]="json.addFloat("
allJTypes["video_encode"]="json.addCompressParam("
allJTypes["lavcodec_context"]="json.lavcodec("


allATypes["uint32_t"]="ADM_param_uint32_t"
allATypes["int32_t"]="ADM_param_int32_t"
allATypes["float"]="ADM_param_float"
allATypes["bool"]="ADM_param_bool"
allATypes["string"]="ADM_param_string"
allATypes["video_encode"]="ADM_param_video_encode"
allATypes["lavcodec_context"]="ADM_param_lavcodec_context"

fullPath=""
nested=list()
gotName=False
def usage():
    print("python admSerialization xxxx.conf")
#########################################################
def outputHeader(st):
    tab="\t"*len(nested)
    headerFile.write(tab+str(st)+"\n")
    pass
def outputDesc(st):
    descFile.write(str(st)+"\n")
    pass
def outputJson(st):
    jsonFile.write(str(st)+"\n")
    #print(st)
    pass

#
def processLine(varType,varName):
    if(not varType in allCTypes):
        print("Unknown var type "+str(varType))
        exit(1)
    if(not varType in allATypes):
        print("Unknown var type "+str(varType))
        exit(1)
    if(not varType in allJTypes):
        print("json:Unknown var "+str(varType))
        exit(1)
    ctype=allCTypes[varType].strip()
    atype=allATypes[varType].strip()
    jtype=allJTypes[varType].strip()
    outputHeader( str(ctype)+" "+str(varName)+";")
    if(len(fullPath)==0):
        fullName=varName
    else:
        fullName=fullPath+"."+varName
    #outputDesc(" {\""+str(varName)+"\",offsetof("+str(structName)+","+str(fullName)+"),\""+str(ctype)+"\","+str(atype)+"},")
    outputDesc(" {\""+str(fullName)+"\",offsetof("+str(structName)+","+str(fullName)+"),\""+str(ctype)+"\","+str(atype)+"},")
    # json part...
    outputJson(jtype+"\""+str(varName)+"\",key->"+str(fullName)+");")
###################################################################################################################################
def writeDescHead():
    outputDesc("// automatically generated by admSerialization.py, do not edit!")
    #outputDesc("#include \"ADM_default.h\"")
    #outputDesc("#include \"ADM_paramList.h\"")
    #outputDesc("#include \""+str(headerFileName)+"\"")
    outputDesc("extern const ADM_paramList "+str(structName)+"_param[]={")

def writeDescFooter():
    outputDesc("{NULL,0,NULL}")
    outputDesc("};")

def writeJsonHead():
    outputJson("// automatically generated by admSerialization.py, do not edit!")
    outputJson("#include \"ADM_default.h\"")
    outputJson("#include \"ADM_paramList.h\"")
    #outputJson("#include \"ADM_coreVideoEncoder.h\"")
    #outputJson("#include \"ADM_encoderConf.h\"")
    outputJson("#include \"ADM_coreJson.h\"")
    outputJson("#include \""+str(headerFileName)+"\"")
    #outputJson("extern const ADM_paramList "+str(structName)+"_param[];")
    outputJson("bool  "+str(structName)+"_jserialize(const char *file, const "+str(structName)+" *key){")
    outputJson("admJson json;")

def writeJsonFooter():
    outputJson("return json.dumpToFile(file);")
    outputJson("};")
    outputJson("bool  "+str(structName)+"_jdeserialize(const char *file, const ADM_paramList *tmpl,"+str(structName)+" *key){")
    outputJson("admJsonToCouple json;")
    outputJson("CONFcouple *c=json.readFromFile(file);")
    outputJson("if(!c) {ADM_error(\"Cannot read json file\");return false;}")
    outputJson("bool r= ADM_paramLoadPartial(c,tmpl,key);")
    outputJson("delete c;")
    outputJson("return r;")
    outputJson("};")



def writeHeaderHead():
    outputHeader( "// automatically generated by admSerialization.py do not edit")
    outputHeader(  "#ifndef ADM_"+str(structName)+"_CONF_H")
    outputHeader(  "#define ADM_"+str(structName)+"_CONF_H")
    outputHeader(  "typedef struct {")

def writeHeadFooter():
    outputHeader("}"+structName+";")
    outputHeader("#endif")
###################################################################################################################################
# Main...
###################################################################################################################################
nb=len(sys.argv)
if(nb!=2):       
    usage()
    exit(1)
print("Processing "+str(sys.argv[1]))
inputFile=sys.argv[1]
if(not os.path.isfile(inputFile)):
    print "no such file "+str(inputFile)
    exit(1)
structName=re.sub(r'.conf',r'',inputFile)
#
headerFileName=re.sub(r'.conf',r'.h',inputFile)
headerFile=open(headerFileName,'w')
#
descFileName=re.sub(r'.conf',r'_desc.cpp',inputFile)
descFile=open(descFileName,'w')
#
jsonFileName=re.sub(r'.conf',r'_json.cpp',inputFile)
jsonFile=open(jsonFileName,'w')
#
f=open(inputFile,'r')
while(1):
    line=f.readline()
    if(len(line)==0):
        break # eof
    line=re.sub(r'#.*$',r'',line)
    line=re.sub(r'//.*$',r'',line)
    line=line.strip()
    if(len(line)==0):
        continue # blank
    # Remove #....
    if(line.find(r'{')!=-1):
        #
        structs=re.sub(r'{.*$',r'',line).strip()
        if(len(nested)==0 and gotName==False): # first one = struct Name
            structName=structs
            print("Our structure is :"+str(structName))
            writeDescHead()
            writeHeaderHead()
            writeJsonHead()
            gotName=True
        else:  # else we have structure inside structure
            #outputHeader("struct "+structs+" {")
            outputHeader("struct  {")
            nested.append(structs)
            fullPath=".".join(nested)
            outputJson("json.addNode(\""+str(structs)+"\");")
    elif(line.find(r'}')!=-1):
        if(len(nested)!=0):
            last=nested.pop()
            outputJson("json.endNode();")
            outputHeader("}"+str(last)+";")
            #outputHeader("};")
        fullPath=".".join(nested)
    elif(line.find(':')!=-1):
        if(gotName==False):
            print "No structure name !"
            exit(1)
        line=re.sub(r'#.*$',r'',line)
        line=re.sub(r'//.*$',r'',line)
        if(len(line)==0):
            continue
        # split by :
        (varType,varName)=line.split(r':')
        #
        # Remote extra ,.... used by prefs
        varName=re.sub(r',.*$',r';',varName)
        #
        varName=re.sub(r';.*$',r'',varName)
        varName=varName.strip()
        varType=varType.strip()
        processLine(varType,varName)
    else:
        print "Invalid line "+str(line)
        exit(1)
f.close()
writeDescFooter()
writeHeadFooter()
writeJsonFooter()
headerFile.close()
descFile.close()
jsonFile.close()
print "All done"
