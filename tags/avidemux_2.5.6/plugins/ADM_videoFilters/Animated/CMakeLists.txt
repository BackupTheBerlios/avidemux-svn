INCLUDE(vf_plugin)


SET(ADM_vf_animated_SRCS ADM_vidAnimated.cpp)

ADD_LIBRARY(ADM_vf_animated SHARED ${ADM_vf_animated_SRCS})

INIT_VIDEOFILTER_PLUGIN(ADM_vf_animated)
INSTALL_VIDEOFILTER(ADM_vf_animated)
