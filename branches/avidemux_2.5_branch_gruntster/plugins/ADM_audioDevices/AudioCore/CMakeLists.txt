INCLUDE(av_plugin)


SET(ADM_av_audiocore_SRCS ADM_deviceAudioCore.cpp)

ADD_LIBRARY(ADM_av_audiocore SHARED ${ADM_av_audiocore_SRCS})
TARGET_LINK_LIBRARIES(ADM_av_audiocore winmm ADM_core ADM_coreAudio)
INIT_AUDIO_DEVICE(ADM_av_audiocore)
INSTALL_AUDIODEVICE(ADM_av_audiocore)
