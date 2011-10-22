INCLUDE(av_plugin)


SET(ADM_av_audiocore_SRCS ADM_deviceAudioCore.cpp)

ADD_LIBRARY(ADM_av_audiocore SHARED ${ADM_av_audiocore_SRCS})
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux")
TARGET_LINK_LIBRARIES(ADM_av_audiocore ADM_core ADM_coreAudio "-Wl,-framework,Carbon" "-Wl,-framework,AudioUnit")
INIT_AUDIO_DEVICE(ADM_av_audiocore)
INSTALL_AUDIODEVICE(ADM_av_audiocore)
