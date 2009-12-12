INCLUDE(vf_plugin)


SET(ADM_vf_resampleFps_SRCS ADM_vidResampleFPS.cpp)

ADD_LIBRARY(ADM_vf_resampleFps SHARED ${ADM_vf_resampleFps_SRCS})

IF (APPLE)
	TARGET_LINK_LIBRARIES(ADM_vf_resampleFps -Wl,-read_only_relocs,suppress)
ENDIF (APPLE)

INIT_VIDEOFILTER_PLUGIN(ADM_vf_resampleFps)
INSTALL_VIDEOFILTER(ADM_vf_resampleFps)
