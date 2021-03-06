MACRO(checkXvid)
	IF (NOT XVID_CHECKED)
		OPTION(XVID "" ON)

		MESSAGE(STATUS "Checking for Xvid")
		MESSAGE(STATUS "*****************")

		IF (XVID)
			FIND_HEADER_AND_LIB(XVID xvid.h xvidcore xvid_plugin_single)
			PRINT_LIBRARY_INFO("Xvid" XVID_FOUND "${XVID_INCLUDE_DIR}" "${XVID_LIBRARY_DIR}")
		ELSE (XVID)
			MESSAGE("${MSG_DISABLE_OPTION}")
		ENDIF (XVID)

		SET(ENV{ADM_HAVE_XVID} ${XVID_FOUND})
		SET(XVID_CHECKED 1)

		MESSAGE("")
	ENDIF (NOT XVID_CHECKED)
ENDMACRO(checkXvid)