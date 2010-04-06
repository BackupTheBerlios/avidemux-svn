INCLUDE(admCheckLibxml2)
INCLUDE(admPluginVideoEncoder)

SET(ADM_vidEnc_mpeg2enc_SRCS  interface.c  encoder.cpp  mpeg1Options.cpp  mpeg1Encoder.cpp  mpeg2Encoder.cpp  mpeg2Options.cpp)

set(PLUGIN_SCHEMA_DIR "mpeg2enc")
set(MPEG1_PLUGIN_CONFIG_DIR "mpeg2enc/mpeg-1")
set(MPEG2_PLUGIN_CONFIG_DIR "mpeg2enc/mpeg-2")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${LIBXML2_INCLUDE_DIR} ../common/pluginOptions ../common/xvidRateCtl mpeg2enc)
ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})
set_property(SOURCE mpeg1Options.cpp PROPERTY COMPILE_FLAGS -DMPEG1_PLUGIN_CONFIG_DIR=\\\"${MPEG1_PLUGIN_CONFIG_DIR}\\\")
set_property(SOURCE mpeg2Options.cpp PROPERTY COMPILE_FLAGS -DMPEG2_PLUGIN_CONFIG_DIR=\\\"${MPEG2_PLUGIN_CONFIG_DIR}\\\")

add_subdirectory(mpeg2enc)
add_subdirectory(xml)
ADD_LIBRARY(ADM_vidEnc_mpeg2enc SHARED ${ADM_vidEnc_mpeg2enc_SRCS})
TARGET_LINK_LIBRARIES(ADM_vidEnc_mpeg2enc ${LIBXML2_LIBRARIES} mpeg2enc ADM_xvidRateCtl ADM_core ADM_coreUI ADM_vidEnc_pluginOptions)

INIT_VIDEO_ENCODER_PLUGIN(ADM_vidEnc_mpeg2enc)
INSTALL_VIDEO_ENCODER(ADM_vidEnc_mpeg2enc)
INSTALL(FILES Mpeg1Param.xsd Mpeg2Param.xsd DESTINATION "${VIDENC_INSTALL_DIR}mpeg2enc")