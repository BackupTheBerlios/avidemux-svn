message(STATUS "Configuring FFmpeg")

file(MAKE_DIRECTORY "${FFMPEG_BINARY_DIR}")

execute_process(COMMAND ${BASH_EXECUTABLE} ffmpeg_configure.sh
				RESULT_VARIABLE FFMPEG_CONFIGURE_RESULT)

if (NOT (FFMPEG_CONFIGURE_RESULT EQUAL 0))
	MESSAGE(ERROR "configure returned <${FFMPEG_CONFIGURE_RESULT}>")
	MESSAGE(FATAL_ERROR "An error occurred")
endif (NOT (FFMPEG_CONFIGURE_RESULT EQUAL 0))

MESSAGE(STATUS "Configuring done, processing")

if (UNIX)
	include(../cmake/FindPatch.cmake)

	IF(APPLE)
		MESSAGE(STATUS "Patching config.mak - mac (2)")
		patch_file("${FFMPEG_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/config.mak.mac.diff")
	ELSE(APPLE)
		MESSAGE(STATUS "Patching config.mak - linux (2)")
		patch_file("${FFMPEG_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/config.mak.diff")
	ENDIF(APPLE)
endif (UNIX)

file(WRITE "${FFMPEG_BINARY_DIR}/adm_configure" "")