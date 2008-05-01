MACRO(checkX264)
	OPTION(X264 "" ON)

	MESSAGE(STATUS "Checking for x264")
	MESSAGE(STATUS "*****************")

	IF (X264)
		FIND_HEADER_AND_LIB(X264 x264.h x264 x264_encoder_open)
		PRINT_LIBRARY_INFO("x264" X264_FOUND "${X264_INCLUDE_DIR}" "${X264_LIBRARY_DIR}")
	ELSE (X264)
		MESSAGE("${MSG_DISABLE_OPTION}")
	ENDIF (X264)

	SET(ENV{ADM_HAVE_X264} ${X264_FOUND})

	MESSAGE("")
ENDMACRO(checkX264)