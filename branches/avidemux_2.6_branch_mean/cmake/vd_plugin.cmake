#
#  Macro to declare an video decoder plugin
#
SET(VD_PLUGIN_DIR "${AVIDEMUX_LIB_DIR}/ADM_plugins6/videoDecoders/")
MACRO(INIT_VIDEO_DECODER _lib)
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_core/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_coreVideoCodec/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_coreUI/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_coreUtils/include/")
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_coreImage/include/")
    ADD_DEFINITIONS("-DADM_MINIMAL_UI_INTERFACE")

ENDMACRO(INIT_VIDEO_DECODER)

MACRO(INSTALL_VIDEO_DECODER _lib)
	INSTALL(TARGETS ${_lib} DESTINATION "${VD_PLUGIN_DIR}")
	TARGET_LINK_LIBRARIES(${_lib} ADM_core6 ADM_coreUI6 ADM_coreVideoCodec6 ADM_coreImage6 ADM_coreUtils6)
ENDMACRO(INSTALL_VIDEO_DECODER)

