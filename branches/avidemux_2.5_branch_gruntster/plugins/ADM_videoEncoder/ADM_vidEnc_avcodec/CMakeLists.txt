INCLUDE(admPluginVideoEncoder)
INCLUDE(admFFmpegUtil)

if (WIN32)
	set(FFMPEG_INSTALL_DIR ${AVIDEMUX_INSTALL_PREFIX})
else (WIN32)
	set(FFMPEG_INSTALL_DIR ${AVIDEMUX_INSTALL_PREFIX}/lib)
endif (WIN32)

getFfmpegLibNames(${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg)

add_library(ADM_libavcodec UNKNOWN IMPORTED)
set_property(TARGET ADM_libavcodec PROPERTY IMPORTED_LOCATION "${FFMPEG_INSTALL_DIR}/${LIBAVCODEC_LIB}")

add_library(ADM_libswscale UNKNOWN IMPORTED)
set_property(TARGET ADM_libswscale PROPERTY IMPORTED_LOCATION "${FFMPEG_INSTALL_DIR}/${LIBSWSCALE_LIB}")

SET(PLUGIN_SUBDIR "avcodec")
SET(ADM_vidEnc_avcodec_SRCS interface.c encoder.cpp huffyuvEncoder.cpp ffvhuffEncoder.cpp ffv1Encoder.cpp dvEncoder.cpp)

ADD_LIBRARY(ADM_vidEnc_avcodec SHARED ${ADM_vidEnc_avcodec_SRCS})
TARGET_LINK_LIBRARIES(ADM_vidEnc_avcodec ADM_core ADM_coreUI ADM_libavcodec ADM_libswscale)

INIT_VIDEO_ENCODER_PLUGIN(ADM_vidEnc_avcodec)
INSTALL_VIDEO_ENCODER(ADM_vidEnc_avcodec)