INCLUDE(av_plugin)

SET(ADM_av_jack_SRCS ADM_deviceJack.cpp)

add_definitions(-DUSE_SRC)
ADD_LIBRARY(ADM_av_jack SHARED ${ADM_av_jack_SRCS})
TARGET_LINK_LIBRARIES(ADM_av_jack ${JACK_LIBRARY_DIR})
include_directories("${JACK_INCLUDE_DIR}" "${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_coreAudio/src/ADM_libsamplerate")

INIT_AUDIO_DEVICE(ADM_av_jack)
INSTALL_AUDIODEVICE(ADM_av_jack)
