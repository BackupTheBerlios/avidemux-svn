MACRO(INIT_DEMUXER _lib)
	add_compiler_export_flags()
	add_definitions(-DADM_demuxer_plugin_EXPORTS)
ENDMACRO(INIT_DEMUXER)

MACRO(INSTALL_DEMUXER _lib)
	TARGET_LINK_LIBRARIES(${_lib} ADM_core6 ADM_coreUtils6 ADM_coreAudio6 ADM_coreImage6 ADM_coreUI6 ADM_core6 ADM_coreDemuxer6)

	if (UNIX)
		target_link_libraries(${_lib} m)
	endif (UNIX)

	ADM_INSTALL_PLUGIN_LIB(demuxers ${_lib})
ENDMACRO(INSTALL_DEMUXER)

MACRO(ADD_DEMUXER name)
        ADM_ADD_SHARED_LIBRARY(${name} ${ARGN})
ENDMACRO(ADD_DEMUXER name)

