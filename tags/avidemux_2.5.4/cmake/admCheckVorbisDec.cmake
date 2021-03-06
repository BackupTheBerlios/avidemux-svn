########################################
# LIBVORBIS
########################################
MACRO(checkvorbis)
	IF (NOT LIBVORBIS_CHECKED)
		OPTION(LIBVORBIS "" ON)

		MESSAGE(STATUS "Checking for Vorbis Decoder")
		MESSAGE(STATUS "***************************")

		IF (LIBVORBIS)
			IF (UNIX)
				SET(LIBVORBIS_REQUIRED_FLAGS "-lm")
			ENDIF (UNIX)

			FIND_HEADER_AND_LIB(LIBVORBIS vorbis/codec.h vorbis vorbis_synthesis_init ${LIBVORBIS_REQUIRED_FLAGS})

			IF (LIBVORBIS_FOUND)
				SET(LIBVORBIS_FOUND 1)
				SET(USE_LIBVORBIS 1)
				SET(LIBVORBIS_INCLUDE_DIR "${LIBVORBIS_INCLUDE_DIR}")
				SET(LIBVORBIS_LIBRARY_DIR "${LIBVORBIS_LIBRARY_DIR}")
			ENDIF (LIBVORBIS_FOUND)

			PRINT_LIBRARY_INFO("Vorbis Decoder" LIBVORBIS_FOUND "${LIBVORBIS_INCLUDE_DIR}" "${LIBVORBIS_LIBRARY_DIR}")
		ELSE (LIBVORBIS)
			MESSAGE("${MSG_DISABLE_OPTION}")
		ENDIF (LIBVORBIS)

		SET(ENV{ADM_HAVE_VORBIS_DEC} ${LIBVORBIS_FOUND})
		SET(LIBVORBIS_CHECKED 1)

		MESSAGE("")
	ENDIF (NOT LIBVORBIS_CHECKED)
ENDMACRO(checkvorbis)
