INCLUDE(admCheckLibxml2)
INCLUDE(admPluginVideoEncoder)
INCLUDE(admFFmpegUtil)

if (WIN32)
	set(FFMPEG_INSTALL_DIR ${AVIDEMUX_INSTALL_PREFIX})
else (WIN32)
	set(FFMPEG_INSTALL_DIR ${AVIDEMUX_INSTALL_PREFIX}/lib${LIB_SUFFIX})
endif (WIN32)

SET(ADM_vidEnc_avcodec_SRCS  interface.c  encoder.cpp  huffyuvEncoder.cpp
							 ffvhuffEncoder.cpp  ffv1Encoder.cpp  dvEncoder.cpp  ../common/PluginOptions.cpp
							 mpeg1Encoder.cpp  mpeg1EncoderOptions.cpp  mpeg2Encoder.cpp  mpeg2EncoderOptions.cpp
							 flv1Encoder.cpp  flv1EncoderOptions.cpp  mjpegEncoder.cpp  mjpegEncoderOptions.cpp
							 h263Encoder.cpp  h263EncoderOptions.cpp)

set(PLUGIN_SCHEMA_DIR "avcodec")
set(MPEG1_PLUGIN_CONFIG_DIR "avcodec/mpeg-1")
set(MPEG2_PLUGIN_CONFIG_DIR "avcodec/mpeg-2")
set(FLV1_PLUGIN_CONFIG_DIR "avcodec/flv1")
set(MJPEG_PLUGIN_CONFIG_DIR "avcodec/mjpeg")
set(MJPEG_PLUGIN_CONFIG_DIR "avcodec/h263")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${LIBXML2_INCLUDE_DIR})
ADD_DEFINITIONS(${LIBXML2_DEFINITIONS} -DPLUGIN_SCHEMA_DIR="${PLUGIN_SCHEMA_DIR}")
set_property(SOURCE mpeg1EncoderOptions.cpp PROPERTY COMPILE_FLAGS -DMPEG1_PLUGIN_CONFIG_DIR=\\\"${MPEG1_PLUGIN_CONFIG_DIR}\\\")
set_property(SOURCE mpeg2EncoderOptions.cpp PROPERTY COMPILE_FLAGS -DMPEG2_PLUGIN_CONFIG_DIR=\\\"${MPEG2_PLUGIN_CONFIG_DIR}\\\")
set_property(SOURCE flv1EncoderOptions.cpp PROPERTY COMPILE_FLAGS -DFLV1_PLUGIN_CONFIG_DIR=\\\"${FLV1_PLUGIN_CONFIG_DIR}\\\")
set_property(SOURCE mjpegEncoderOptions.cpp PROPERTY COMPILE_FLAGS -DMJPEG_PLUGIN_CONFIG_DIR=\\\"${MJPEG_PLUGIN_CONFIG_DIR}\\\")
set_property(SOURCE h263EncoderOptions.cpp PROPERTY COMPILE_FLAGS -DH263_PLUGIN_CONFIG_DIR=\\\"${H263_PLUGIN_CONFIG_DIR}\\\")

getFfmpegLibNames(${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg)

add_library(ADM_libavcodec UNKNOWN IMPORTED)
set_property(TARGET ADM_libavcodec PROPERTY IMPORTED_LOCATION "${FFMPEG_INSTALL_DIR}/${LIBAVCODEC_LIB}")

add_subdirectory(xvidRateCtl)
add_subdirectory(xml)

ADD_LIBRARY(ADM_vidEnc_avcodec SHARED ${ADM_vidEnc_avcodec_SRCS})
TARGET_LINK_LIBRARIES(ADM_vidEnc_avcodec ${LIBXML2_LIBRARIES} ADM_xvidRateCtl ADM_core ADM_coreUI ADM_libavcodec)

INIT_VIDEO_ENCODER_PLUGIN(ADM_vidEnc_avcodec)
INSTALL_VIDEO_ENCODER(ADM_vidEnc_avcodec)
INSTALL(FILES Mpeg1Param.xsd DESTINATION "${VIDENC_INSTALL_DIR}${PLUGIN_SCHEMA_DIR}")
INSTALL(FILES Mpeg2Param.xsd DESTINATION "${VIDENC_INSTALL_DIR}${PLUGIN_SCHEMA_DIR}")
INSTALL(FILES Flv1Param.xsd DESTINATION "${VIDENC_INSTALL_DIR}${PLUGIN_SCHEMA_DIR}")
INSTALL(FILES MjpegParam.xsd DESTINATION "${VIDENC_INSTALL_DIR}${PLUGIN_SCHEMA_DIR}")
INSTALL(FILES H263Param.xsd DESTINATION "${VIDENC_INSTALL_DIR}${PLUGIN_SCHEMA_DIR}")