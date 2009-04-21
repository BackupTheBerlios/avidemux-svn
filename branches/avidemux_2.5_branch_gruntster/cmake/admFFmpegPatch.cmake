find_package(Patch)

if (WIN32)
	find_package(Unix2Dos)
endif (WIN32)

file(GLOB patchFiles "${CMAKE_SOURCE_DIR}/cmake/patches/*.patch")

foreach(patchFile ${patchFiles})
	if (WIN32)
		file(MAKE_DIRECTORY "${FFMPEG_BINARY_DIR}/patches")
		get_filename_component(fileName "${patchFile}" NAME)
		execute_process(COMMAND ${UNIX2DOS_EXECUTABLE} -n ${patchFile} ${FFMPEG_BINARY_DIR}/patches/${fileName}
						${unix2dosOutput})

		set(patchFile "${FFMPEG_BINARY_DIR}/patches/${fileName}")
	endif (WIN32)

	execute_process(COMMAND ${PATCH_EXECUTABLE} -p0 -i "${patchFile}"
					WORKING_DIRECTORY "${FFMPEG_SOURCE_DIR}")
endforeach(patchFile)