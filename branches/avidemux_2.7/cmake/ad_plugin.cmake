include(admAsNeeded)

MACRO(INIT_AUDIO_PLUGIN _lib)
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/common/ADM_audiocodec")
	add_compiler_export_flags()
	add_definitions(-DADM_audioDecoder_plugin_EXPORTS)
ENDMACRO(INIT_AUDIO_PLUGIN)

MACRO(INSTALL_AUDIODECODER _lib)
	TARGET_LINK_LIBRARIES(${_lib} ADM_core6)

	if (UNIX)
		target_link_libraries(${_lib} m)
	endif (UNIX)

	ADM_INSTALL_PLUGIN_LIB(audioDecoder ${_lib})
ENDMACRO(INSTALL_AUDIODECODER)

MACRO(ADD_AUDIO_DECODER name)
	ADM_ADD_SHARED_LIBRARY(${name} ${ARGN})
ENDMACRO(ADD_AUDIO_DECODER name)

