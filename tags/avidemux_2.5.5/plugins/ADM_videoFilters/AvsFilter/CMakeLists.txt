INCLUDE(vf_plugin)

SET(ADM_vf_avsfilter_SRCS avsfilter.cpp avspipecomm.cpp cdebug.cpp strnew.cpp)

ADD_LIBRARY(ADM_vf_avsfilter SHARED ${ADM_vf_avsfilter_SRCS})
INCLUDE(vf_plugin)

INIT_VIDEOFILTER_PLUGIN(ADM_vf_avsfilter)
INSTALL_VIDEOFILTER(ADM_vf_avsfilter)
ADD_DEFINITIONS("-DVERSION_2_5 -DDEBUGMSG")
ADD_DEFINITIONS(-DAVS_WINE_BINARY_PATH="${CMAKE_INSTALL_PREFIX}/share/ADM_addons/avsfilter")
