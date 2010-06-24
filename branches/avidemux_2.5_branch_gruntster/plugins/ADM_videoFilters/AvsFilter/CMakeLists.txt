IF (NOT WIN32)
INCLUDE(vf_plugin)

SET(ADM_vf_avsfilter_SRCS avsfilter.cpp avspipecomm.cpp cdebug.cpp strnew.cpp)

ADD_LIBRARY(ADM_vf_avsfilter SHARED ${ADM_vf_avsfilter_SRCS})
INCLUDE(vf_plugin)

INIT_VIDEOFILTER_PLUGIN(ADM_vf_avsfilter)
INSTALL_VIDEOFILTER(ADM_vf_avsfilter)
ADD_DEFINITIONS("-DVERSION_2_5 -DDEBUGMSG")

ENDIF (NOT WIN32)

