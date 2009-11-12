#
#  Macro to declare an audio encoder plugin
#
MACRO(INIT_AUDIO_ENCODER _lib)
	INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/common/ADM_audioFilter/include")

    ADD_DEFINITIONS("-DADM_MINIMAL_UI_INTERFACE")

ENDMACRO(INIT_AUDIO_ENCODER)

MACRO(INSTALL_AUDIOENCODER _lib)
	INSTALL(TARGETS ${_lib} DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ADM_plugins6/audioEncoders/")
	TARGET_LINK_LIBRARIES(${_lib} ADM_core6 ADM_coreUI6 ADM_coreAudio6 ADM_coreAudioEncoder6 ADM_coreUtils6)
ENDMACRO(INSTALL_AUDIOENCODER)
