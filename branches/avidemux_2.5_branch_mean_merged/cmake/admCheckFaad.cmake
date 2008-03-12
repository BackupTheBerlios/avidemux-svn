# Outputs:
#   FAAD_INCLUDE_DIR
#   FAAD_LIBRARY_DIR
#   FAAD_OLD_PROTOTYPE
#   HAVE_FAAD

MACRO(checkFaad)
	OPTION (FAAD "" ON)

	MESSAGE(STATUS "Checking for FAAD")
	MESSAGE(STATUS "*****************")

	IF (FAAD)
		FIND_HEADER_AND_LIB(FAAD faad.h faad)

		IF (FAAD_FOUND)
			ADM_CHECK_FUNCTION_EXISTS(faacDecInit "${FAAD_LIBRARY_DIR}" FAACDEC_FUNCTION_FOUND)

			IF (NOT FAACDEC_FUNCTION_FOUND)
				ADM_CHECK_FUNCTION_EXISTS(NeAACDecInit "${FAAD_LIBRARY_DIR}" NEAACDEC_FUNCTION_FOUND)

				IF (NOT NEAACDEC_FUNCTION_FOUND)
					SET(FAAD_FOUND 0)
				ENDIF (NOT NEAACDEC_FUNCTION_FOUND)
			ENDIF (NOT FAACDEC_FUNCTION_FOUND)
		ENDIF (FAAD_FOUND)

		PRINT_LIBRARY_INFO("FAAD" FAAD_FOUND "${FAAD_INCLUDE_DIR}" "${FAAD_LIBRARY_DIR}")

		IF (FAAD_FOUND)
			SET(HAVE_FAAD 1)

			CHECK_CFLAGS_REQUIRED(faad_check.cpp "-DOLD_FAAD_PROTO" "${FAAD_INCLUDE_DIR}" "${FAAD_LIBRARY_DIR}" FAAD_CFLAGS_REQUIRED)

			IF (FAAD_CFLAGS_REQUIRED)
				SET(FAAD_OLD_PROTOTYPE 1)
			ENDIF (FAAD_CFLAGS_REQUIRED)
		ENDIF (FAAD_FOUND)
	ELSE (FAAD)
		MESSAGE("${MSG_DISABLE_OPTION}")
	ENDIF (FAAD)

	MESSAGE("")
ENDMACRO(checkFaad)

