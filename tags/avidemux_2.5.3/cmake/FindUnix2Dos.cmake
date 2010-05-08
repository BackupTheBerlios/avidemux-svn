if (NOT UNIX2DOS_EXECUTABLE)
	message(STATUS "Checking for unix2dos")
	message(STATUS "*********************")

	find_program(UNIX2DOS_EXECUTABLE unix2dos)
	set(UNIX2DOS_EXECUTABLE ${UNIX2DOS_EXECUTABLE} CACHE STRING "")

	if (UNIX2DOS_EXECUTABLE)
		message(STATUS "Found unix2dos")
		
		if (VERBOSE)
			message(STATUS "Path: ${UNIX2DOS_EXECUTABLE}")
		endif (VERBOSE)
	else (UNIX2DOS_EXECUTABLE)
		message(FATAL_ERROR "unix2dos not found")
	endif (UNIX2DOS_EXECUTABLE)

	message("")
endif (NOT UNIX2DOS_EXECUTABLE)