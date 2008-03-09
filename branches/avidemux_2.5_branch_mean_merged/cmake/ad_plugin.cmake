MACRO(ADM_audio_plugin  _lib)
ADD_TARGET_CFLAGS(${_lib} "-I${CMAKE_BINARY_DIR}/config/cli")
ADD_TARGET_CFLAGS(${_lib} "-I${CMAKE_SOURCE_DIR}/avidemux/ADM_audiocodec")
INSTALL(FILES ${_lib} DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ADM_plugins/audioDecoder/")
MACRO(ADM_audio_plugin)

