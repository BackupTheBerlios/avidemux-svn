
SET(ADMaudioCore_SRCS
ADM_audioUtils.cpp
audioencoder.cpp
ADM_audioFilter.cpp
ADM_audioResample.cpp
ADM_libsamplerate/samplerate.c
ADM_libsamplerate/src_linear.c
ADM_libsamplerate/src_sinc.c
ADM_libsamplerate/src_zoh.c
)	

ADD_DEFINITIONS(-DADM_LEGACY_PROGGY)
ADD_DEFINITIONS(-I${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(ADM_coreAudio SHARED ${ADMaudioCore_SRCS})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/avidemux/ADM_coreAudio/include)
TARGET_LINK_LIBRARIES(ADM_coreAudio ADM_core)

INSTALL(TARGETS ADM_coreAudio RUNTIME DESTINATION ${BIN_DIR}  LIBRARY DESTINATION lib${LIB_SUFFIX}  ARCHIVE DESTINATION lib${LIB_SUFFIX})
