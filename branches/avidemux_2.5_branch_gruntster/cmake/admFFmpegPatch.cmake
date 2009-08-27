macro(find_patch)
	find_package(Patch)

	#if (WIN32)
		#find_package(Unix2Dos)

		#if (NOT VERBOSE)
			#set(unix2dosOutput OUTPUT_VARIABLE UNIX2DOS_OUTPUT)
		#endif (NOT VERBOSE)
	#endif (WIN32)
endmacro(find_patch)

macro(patch_file baseDir patchFile)
	#if (WIN32)
		#set(tempPatchDir ${CMAKE_BINARY_DIR}/temp)
		#file(MAKE_DIRECTORY "${tempPatchDir}")
		#get_filename_component(fileName "${patchFile}" NAME)

		#execute_process(COMMAND ${UNIX2DOS_EXECUTABLE} -n ${patchFile} ${tempPatchDir}/${fileName}
						#${unix2dosOutput})

		#set(_patchFile "${tempPatchDir}/${fileName}")
	#else (WIN32)
		set(_patchFile "${patchFile}")
	#endif (WIN32)

	execute_process(COMMAND ${PATCH_EXECUTABLE} -p0 -i "${_patchFile}"
					WORKING_DIRECTORY "${baseDir}")
endmacro(patch_file)
