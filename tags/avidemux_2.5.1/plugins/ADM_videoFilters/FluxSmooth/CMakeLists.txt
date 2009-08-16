INCLUDE(vf_plugin)


SET(ADM_vf_FluxSmooth_SRCS ADM_vidFlux.cpp)

ADD_LIBRARY(ADM_vf_FluxSmooth SHARED ${ADM_vf_FluxSmooth_SRCS})

IF (APPLE)
	TARGET_LINK_LIBRARIES(ADM_vf_FluxSmooth -Wl,-read_only_relocs,suppress)
ENDIF (APPLE)

INIT_VIDEOFILTER_PLUGIN(ADM_vf_FluxSmooth)
INSTALL_VIDEOFILTER(ADM_vf_FluxSmooth)
