INCLUDE(vf_plugin)


SET(ADM_vf_resampleFps_SRCS ADM_vidResampleFPS.cpp)

ADD_LIBRARY(ADM_vf_resampleFps SHARED ${ADM_vf_resampleFps_SRCS})

IF (APPLE AND NOT ADM_CPU_X86_64)
	TARGET_LINK_LIBRARIES(ADM_vf_resampleFps -Wl,-read_only_relocs,suppress)
ENDIF (APPLE AND NOT ADM_CPU_X86_64)

INIT_VIDEOFILTER_PLUGIN(ADM_vf_resampleFps)
INSTALL_VIDEOFILTER(ADM_vf_resampleFps)
