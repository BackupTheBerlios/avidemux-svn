MACRO(checkFreeType)
########################################
# FreeType2
########################################
OPTION(FREETYPE2 "" ON)

MESSAGE(STATUS "Checking for FreeType2")
MESSAGE(STATUS "**********************")

IF (FREETYPE2)
	PKG_CHECK_MODULES(FREETYPE2 freetype2)

	if (NOT FREETYPE2_FOUND)
		message("Checking for header and lib")
		unset(FREETYPE2_FOUND CACHE)
		FIND_HEADER_AND_LIB(FREETYPE2 freetype2/freetype/freetype.h freetype FT_Init_FreeType)
	endif (NOT FREETYPE2_FOUND)

	PRINT_LIBRARY_INFO("FreeType2" FREETYPE2_FOUND "${FREETYPE2_CFLAGS}" "${FREETYPE2_LDFLAGS}")

	IF (FREETYPE2_FOUND)
		SET(USE_FREETYPE 1)
	ENDIF (FREETYPE2_FOUND)
ELSE (FREETYPE2)
	MESSAGE("${MSG_DISABLE_OPTION}")
ENDIF (FREETYPE2)

APPEND_SUMMARY_LIST("Miscellaneous" "FreeType2" "${FREETYPE2_FOUND}")

MESSAGE("")

########################################
# libiconv
########################################
IF (USE_FREETYPE)
	MESSAGE(STATUS "Checking for libiconv")
	MESSAGE(STATUS "*********************")

	FIND_HEADER_AND_LIB(LIBICONV iconv.h iconv)
	PRINT_LIBRARY_INFO("libiconv" LIBICONV_FOUND "${LIBICONV_INCLUDE_DIR}" "${LIBICONV_LIBRARY_DIR}")

	IF (NOT LIBICONV_INCLUDE_DIR)
		MESSAGE(STATUS "iconv.h not found, disabling FreeType2")
		SET(USE_FREETYPE)
	ELSE (NOT LIBICONV_INCLUDE_DIR)
		IF (LIBICONV_LIBRARY_DIR)
			MESSAGE(STATUS "libiconv is probably needed")

			CHECK_CFLAGS_REQUIRED(iconv_check.cpp "-DICONV_NEED_CONST" "${LIBICONV_INCLUDE_DIR}" "${LIBICONV_LIBRARY_DIR}" LIBICONV_CFLAGS_REQUIRED)
		ELSE (LIBICONV_LIBRARY_DIR)
			SET(LIBICONV_LIBRARY_DIR "")
			MESSAGE(STATUS "libiconv is probably not needed")

			ADM_COMPILE(iconv_check.cpp "-DICONV_NEED_CONST" "${LIBICONV_INCLUDE_DIR}" "" LIBICONV_CFLAGS_REQUIRED LIBICONV_COMPILE_OUTPUT)
		ENDIF (LIBICONV_LIBRARY_DIR)

		IF (LIBICONV_CFLAGS_REQUIRED)
			SET(LIBICONV_DEFINITIONS "-DICONV_NEED_CONST")
			SET(ICONV_NEED_CONST 1)
		ENDIF(LIBICONV_CFLAGS_REQUIRED)
	ENDIF (NOT LIBICONV_INCLUDE_DIR)

	MESSAGE("")
ENDIF (USE_FREETYPE)

ENDMACRO(checkFreeType)
