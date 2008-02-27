
SET(ADM_osSupport_SRCS 
ADM_debug.cpp   
ADM_queue.cpp  
ADM_crashHook.cpp  
ADM_fileio.cpp 
ADM_misc.cpp 
ADM_quota.cpp)

ADD_LIBRARY(ADM_osSupport STATIC ${ADM_osSupport_SRCS})
add_definitions(${LIBXML2_DEFINITIONS})
include_directories(${LIBXML2_INCLUDE_DIR})
