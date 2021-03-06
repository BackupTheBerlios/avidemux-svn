include(admAsNeeded)

macro (ADD_SCRIPT_ENGINE name)
	add_compiler_export_flags()
	add_definitions(-DADM_scriptEngine_plugin_EXPORTS)
    ADM_ADD_SHARED_LIBRARY(${name} ${ARGN})
endmacro (ADD_SCRIPT_ENGINE name)

macro (INSTALL_SCRIPT_ENGINE _lib)
	ADM_INSTALL_PLUGIN_LIB(scriptEngines ${_lib})
endmacro (INSTALL_SCRIPT_ENGINE)

macro (INSTALL_SCRIPT_ENGINE_HELP _engineName _sourceDirectory)
if (WIN32)
    set(helpDir "${CMAKE_INSTALL_PREFIX}/help/${_engineName}")
else (WIN32)
    set(helpDir "${CMAKE_INSTALL_PREFIX}/share/avidemux6/help/${_engineName}")
endif (WIN32)

    install(DIRECTORY "${_sourceDirectory}" DESTINATION "${helpDir}")
endmacro (INSTALL_SCRIPT_ENGINE_HELP)
