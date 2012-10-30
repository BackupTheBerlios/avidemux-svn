#
#  Macro to declare an video encoder plugin
#
MACRO(INIT_VIDEO_ENCODER _lib)
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_core/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_coreVideoEncoder/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_coreUI/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_coreUtils/include/")
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_coreVideoFilter/include/")
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/ADM_coreImage/include/")
	add_compiler_export_flags()
	add_definitions(-DADM_videoEncoder_plugin_EXPORTS)
    ADD_DEFINITIONS("-DADM_MINIMAL_UI_INTERFACE")
ENDMACRO(INIT_VIDEO_ENCODER)

MACRO(INSTALL_VIDEO_ENCODER _lib)
	ADM_INSTALL_PLUGIN_LIB(videoEncoders ${_lib})
	TARGET_LINK_LIBRARIES(${_lib} ADM_core6 ADM_coreUI6 ADM_coreVideoEncoder6 ADM_coreImage6 ADM_coreUtils6)

	if (UNIX)
		target_link_libraries(${_lib} m)
	endif (UNIX)
ENDMACRO(INSTALL_VIDEO_ENCODER)

MACRO(ADD_VIDEO_ENCODER name)
        ADM_ADD_SHARED_LIBRARY(${name} ${ARGN})
ENDMACRO(ADD_VIDEO_ENCODER name)

