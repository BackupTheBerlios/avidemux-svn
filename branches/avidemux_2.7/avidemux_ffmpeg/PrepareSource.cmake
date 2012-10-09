include(../cmake/FindPatch.cmake)

if (NOT VERBOSE)
	set(ffmpegExtractOutput OUTPUT_VARIABLE FFMPEG_EXTRACT_OUTPUT)
endif (NOT VERBOSE)

MESSAGE(STATUS "Creating ${FFMPEG_BASE_DIR}")
file(MAKE_DIRECTORY "${FFMPEG_BASE_DIR}")

MESSAGE(STATUS "Unpacking ffmpeg from ${CMAKE_CURRENT_SOURCE_DIR}/${FFMPEG_SOURCE_ARCHIVE} to ${FFMPEG_BASE_DIR}")        

execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${FFMPEG_SOURCE_ARCHIVE}" "${FFMPEG_BASE_DIR}")

execute_process(COMMAND ${TAR_EXECUTABLE} xvfj "${FFMPEG_SOURCE_ARCHIVE}"
				WORKING_DIRECTORY "${FFMPEG_BASE_DIR}"
				${ffmpegExtractOutput})

file(REMOVE_RECURSE "${FFMPEG_SOURCE_DIR}")
execute_process(COMMAND ${CMAKE_COMMAND} -E rename "${FFMPEG_SOURCE_ARCHIVE_DIR}" "${FFMPEG_SOURCE_DIR}"
				WORKING_DIRECTORY "${FFMPEG_BASE_DIR}")

file(GLOB patchFiles "${CMAKE_CURRENT_SOURCE_DIR}/patches/*.patch")

foreach(patchFile ${patchFiles})
	patch_file("${FFMPEG_SOURCE_DIR}" "${patchFile}")
endforeach(patchFile)

if (UNIX)
	MESSAGE(STATUS "Patching Linux common.mak")
	patch_file("${FFMPEG_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/patches/common.mak.diff") 

	message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}/patches/config.mak.diff to ${FFMPEG_BASE_DIR}")

	if (APPLE)
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/patches/config.mak.mac.diff" "${FFMPEG_BASE_DIR}/..")
	else (APPLE)
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/patches/config.mak.diff" "${FFMPEG_BASE_DIR}/..")
	endif (APPLE)
endif (UNIX)

message("")

file(WRITE "${FFMPEG_SOURCE_DIR}/adm_extract" "")