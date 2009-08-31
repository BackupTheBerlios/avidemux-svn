IF (WIN32)
	SET(VIDENC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/plugins/videoEncoder/")
ELSE (WIN32)
	SET(VIDENC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib/ADM_plugins/videoEncoder/")
ENDIF (WIN32)

MACRO(INIT_VIDEO_ENCODER_PLUGIN _lib)
	INCLUDE_DIRECTORIES("${CMAKE_BINARY_DIR}/config")
	INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_core/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_coreUI/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_plugin")
ENDMACRO(INIT_VIDEO_ENCODER_PLUGIN)

MACRO(INSTALL_VIDEO_ENCODER _lib)
	INSTALL(TARGETS ${_lib} DESTINATION "${VIDENC_INSTALL_DIR}")
ENDMACRO(INSTALL_VIDEO_ENCODER)
