INCLUDE(admCheckLibxml2)
INCLUDE(admPluginVideoEncoder)

add_subdirectory(mpeg2enc)
SET(ADM_vidEnc_mpeg2enc_SRCS  interface.c  encoder.cpp  mpeg2Encoder.cpp  mpeg2Options.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${LIBXML2_INCLUDE_DIR} ../common/pluginOptions ../common/xvidRateCtl)
ADD_DEFINITIONS(${LIBXML2_DEFINITIONS} -DMPEG2_PLUGIN_CONFIG_DIR="mpegenc")

ADD_LIBRARY(ADM_vidEnc_mpeg2enc SHARED ${ADM_vidEnc_mpeg2enc_SRCS})
TARGET_LINK_LIBRARIES(ADM_vidEnc_mpeg2enc ${LIBXML2_LIBRARIES} mpeg2enc ADM_xvidRateCtl ADM_core ADM_coreUI ADM_vidEnc_pluginOptions)

INIT_VIDEO_ENCODER_PLUGIN(ADM_vidEnc_mpeg2enc)
INSTALL_VIDEO_ENCODER(ADM_vidEnc_mpeg2enc)
INSTALL(FILES Mpeg2Param.xsd DESTINATION "${VIDENC_INSTALL_DIR}mpeg2enc")