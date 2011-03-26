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
allCTypes["uint32_t"]="uint32_t"
allCTypes["int32_t"]="int32_t"
allCTypes["bool"]="bool"
allCTypes["string"]="char *"
allCTypes["bool"]="bool"
allCTypes["float"]="float"
allCTypes["video_encode"]="COMPRES_PARAMS"
allCTypes["lavcodec_context"]="FFcodecContext"

allATypes["uint32_t"]="ADM_param_uint32_t"
allATypes["int32_t"]="ADM_param_int32_t"
allATypes["float"]="ADM_param_float"
allATypes["bool"]="ADM_param_bool"
allATypes["string"]="ADM_param_string"
allATypes["float"]="ADM_param_float"
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

#
def processLine(varType,varName):
    if(not varType in allCTypes):
        print("Unknown var "+str(varType))
        exit(1)
    if(not varType in allATypes):
        print("Unknown var "+str(varType))
        exit(1)
    ctype=allCTypes[varType].strip()
    atype=allATypes[varType].strip()
    outputHeader( str(ctype)+" "+str(varName)+";")
    if(len(fullPath)==0):
        fullName=varName
    else:
        fullName=fullPath+"."+varName
    outputDesc(" {\""+str(varName)+"\",offsetof("+str(structName)+","+str(fullName)+"),\""+str(ctype)+"\","+str(atype)+"},")
###################################################################################################################################
def writeDescHead():
    outputHeader("// automatically generated by admSerialization.py, do not edit!")
    outputDesc("const ADM_paramList "+str(structName)+"_param[]={")

def writeDescFooter():
    outputDesc("{NULL,0,NULL}")
    outputDesc("};")


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
f=open(inputFile,'r')
while(1):
    line=f.readline()
    if(len(line)==0):
        break # eof
    # Remove #....
    if(line.find(r'{')!=-1):
        #
        structs=re.sub(r'{.*$',r'',line).strip()
        if(len(nested)==0 and gotName==False): # first one = struct Name
            structName=structs
            print("Our structure is :"+str(structName))
            writeDescHead()
            writeHeaderHead()
            gotName=True
        else:  # else we have structure inside structure
            outputHeader("struct "+structs+" {")
            nested.append(structs)
            fullPath=".".join(nested)
    elif(line.find(r'}')!=-1):
        if(len(nested)!=0):
            nested.pop()
            outputHeader("}"+str(structName)+";")
        fullPath=".".join(nested)
    elif(line.find(':')!=-1):
        if(gotName==False):
            print "No structure name !"
            exit(1)
        line=re.sub(r'#.*$',r'',line)
        line=re.sub(r'//.*$',r'',line)
        # split by :
        (varType,varName)=line.split(r':')
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
headerFile.close()
descFile.close()
print "All done"
