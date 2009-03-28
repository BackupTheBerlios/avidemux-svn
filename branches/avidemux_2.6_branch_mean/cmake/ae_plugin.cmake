#
#  Macro to declare an audio encoder plugin
#
MACRO(INIT_AUDIO_ENCODER _lib)
	INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_core/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_coreAudio/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_coreUI/include")
	INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_audioFilter/include")
    INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_coreAudioEncoder/include")

    ADD_DEFINITIONS("-DADM_MINIMAL_UI_INTERFACE")

ENDMACRO(INIT_AUDIO_ENCODER)

MACRO(INSTALL_AUDIOENCODER _lib)
	INSTALL(TARGETS ${_lib} DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ADM_plugins/audioEncoders/")
	TARGET_LINK_LIBRARIES(${_lib} ADM_core ADM_coreUI ADM_coreAudio ADM_coreAudioEncoder)
ENDMACRO(INSTALL_AUDIOENCODER)
