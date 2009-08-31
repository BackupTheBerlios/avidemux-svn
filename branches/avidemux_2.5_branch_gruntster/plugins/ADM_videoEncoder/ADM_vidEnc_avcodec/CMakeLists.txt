INCLUDE(admCheckLibxml2)
INCLUDE(admPluginVideoEncoder)
INCLUDE(admFFmpegUtil)

if (WIN32)
	set(FFMPEG_INSTALL_DIR ${AVIDEMUX_INSTALL_PREFIX})
else (WIN32)
	set(FFMPEG_INSTALL_DIR ${AVIDEMUX_INSTALL_PREFIX}/lib${LIB_SUFFIX})
endif (WIN32)

SET(ADM_vidEnc_avcodec_SRCS  interface.c  encoder.cpp  huffyuvEncoder.cpp
							 ffvhuffEncoder.cpp  ffv1Encoder.cpp  dvEncoder.cpp  mpeg1Encoder.cpp
							 mpeg1EncoderOptions.cpp)

set(PLUGIN_SCHEMA_DIR "avcodec")
set(MPEG1_PLUGIN_CONFIG_DIR "avcodec/mpeg1")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${LIBXML2_INCLUDE_DIR})
ADD_DEFINITIONS(${LIBXML2_DEFINITIONS} -DPLUGIN_SCHEMA_DIR="${PLUGIN_SCHEMA_DIR}")
set_property(SOURCE mpeg1EncoderOptions.cpp PROPERTY COMPILE_FLAGS -DPLUGIN_CONFIG_DIR=\\\"${MPEG1_PLUGIN_CONFIG_DIR}\\\")

getFfmpegLibNames(${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg)

add_library(ADM_libavcodec UNKNOWN IMPORTED)
set_property(TARGET ADM_libavcodec PROPERTY IMPORTED_LOCATION "${FFMPEG_INSTALL_DIR}/${LIBAVCODEC_LIB}")

ADD_LIBRARY(ADM_vidEnc_avcodec SHARED ${ADM_vidEnc_avcodec_SRCS})
TARGET_LINK_LIBRARIES(ADM_vidEnc_avcodec ${LIBXML2_LIBRARIES} ADM_core ADM_coreUI ADM_libavcodec)

INIT_VIDEO_ENCODER_PLUGIN(ADM_vidEnc_avcodec)
INSTALL_VIDEO_ENCODER(ADM_vidEnc_avcodec)
INSTALL(FILES Mpeg1Param.xsd DESTINATION "${VIDENC_INSTALL_DIR}${PLUGIN_SCHEMA_DIR}")
