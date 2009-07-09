INCLUDE(admCheckLibxml2)
INCLUDE(admPluginVideoEncoder)
INCLUDE(admFFmpegUtil)

if (WIN32)
	set(FFMPEG_INSTALL_DIR ${AVIDEMUX_INSTALL_PREFIX})
else (WIN32)
	set(FFMPEG_INSTALL_DIR ${AVIDEMUX_INSTALL_PREFIX}/lib)
endif (WIN32)

SET(PLUGIN_SUBDIR "avcodec")
SET(ADM_vidEnc_avcodec_SRCS  interface.c  encoder.cpp  ../common/PluginOptions.cpp  huffyuvEncoder.cpp
							 ffvhuffEncoder.cpp  ffv1Encoder.cpp  dvEncoder.cpp  mpeg1Encoder.cpp
							 mpeg1EncoderOptions.cpp)

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/config.h")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${LIBXML2_INCLUDE_DIR})
ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})

getFfmpegLibNames(${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg)

add_library(ADM_libavcodec UNKNOWN IMPORTED)
set_property(TARGET ADM_libavcodec PROPERTY IMPORTED_LOCATION "${FFMPEG_INSTALL_DIR}/${LIBAVCODEC_LIB}")

add_library(ADM_libswscale UNKNOWN IMPORTED)
set_property(TARGET ADM_libswscale PROPERTY IMPORTED_LOCATION "${FFMPEG_INSTALL_DIR}/${LIBSWSCALE_LIB}")

ADD_LIBRARY(ADM_vidEnc_avcodec SHARED ${ADM_vidEnc_avcodec_SRCS})
TARGET_LINK_LIBRARIES(ADM_vidEnc_avcodec ${LIBXML2_LIBRARIES} ADM_core ADM_coreUI ADM_libavcodec ADM_libswscale)

INIT_VIDEO_ENCODER_PLUGIN(ADM_vidEnc_avcodec)
INSTALL_VIDEO_ENCODER(ADM_vidEnc_avcodec)
INSTALL(FILES Mpeg1Param.xsd DESTINATION "${VIDENC_INSTALL_DIR}${PLUGIN_SUBDIR}")