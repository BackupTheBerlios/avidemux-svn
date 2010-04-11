SET(ADM_LIB ADM_commonUI)

SET(${ADM_LIB}_SRCS
DIA_v2v.cpp
DIA_audioFilter.cpp   
DIA_gototime.cpp    
DIA_postproc.cpp  
DIA_bitrateHisto.cpp  
DIA_prefs.cpp     
DIA_builtin.cpp       
DIA_lavDecoder.cpp  
DIA_requant.cpp   
DIA_ocr.cpp             
DIA_tsSub.cpp        
DIA_jobs_save.cpp
DIA_plugins.cpp
DIA_muxerMpegPs.cpp
)

ADD_ADM_LIB_ALL_TARGETS(${ADM_LIB} ${${ADM_LIB}_SRCS})