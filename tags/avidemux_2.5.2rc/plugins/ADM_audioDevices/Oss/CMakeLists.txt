INCLUDE(av_plugin)


SET(ADM_av_oss_SRCS ADM_deviceoss.cpp)

ADD_LIBRARY(ADM_av_oss SHARED ${ADM_av_oss_SRCS})
TARGET_LINK_LIBRARIES(ADM_av_oss ADM_coreAudio)
include_directories(${OSS_INCLUDE_DIR})
INIT_AUDIO_DEVICE(ADM_av_oss)
INSTALL_AUDIODEVICE(ADM_av_oss)
