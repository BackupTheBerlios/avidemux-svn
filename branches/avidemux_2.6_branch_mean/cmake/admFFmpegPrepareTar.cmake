if (NOT VERBOSE)
	set(ffmpegExtractOutput OUTPUT_VARIABLE FFMPEG_EXTRACT_OUTPUT)
endif (NOT VERBOSE)

if (EXISTS "${LIBRARY_SOURCE_DIR}/${FFMPEG_SOURCE_ARCHIVE}" AND EXISTS "${LIBRARY_SOURCE_DIR}/${SWSCALE_SOURCE_ARCHIVE}")
	MESSAGE(STATUS "Creating ${FFMPEG_BASE_DIR}")
	file(MAKE_DIRECTORY "${FFMPEG_BASE_DIR}")
	if (NOT EXISTS "${FFMPEG_SOURCE_DIR}/ffmpeg.c" OR NOT ${LAST_FFMPEG_VERSION} EQUAL ${FFMPEG_VERSION})
		find_package(Tar)
		MESSAGE(STATUS "Unpacking ffmpeg from ${LIBRARY_SOURCE_DIR} to ${FFMPEG_BASE_DIR}")
		execute_process(COMMAND ${TAR_EXECUTABLE} xvf ${LIBRARY_SOURCE_DIR}/${FFMPEG_SOURCE_ARCHIVE} -C ${FFMPEG_BASE_DIR}
					${ffmpegExtractOutput})

		set(FFMPEG_PERFORM_PATCH 1)
		set(FFMPEG_PERFORM_BUILD 1)
	endif (NOT EXISTS "${FFMPEG_SOURCE_DIR}/ffmpeg.c" OR NOT ${LAST_FFMPEG_VERSION} EQUAL ${FFMPEG_VERSION})

	set(FFMPEG_PREPARED 1)
	set(LAST_FFMPEG_VERSION "${FFMPEG_VERSION}" CACHE STRING "" FORCE)
endif (EXISTS "${LIBRARY_SOURCE_DIR}/${FFMPEG_SOURCE_ARCHIVE}" AND EXISTS "${LIBRARY_SOURCE_DIR}/${SWSCALE_SOURCE_ARCHIVE}")
