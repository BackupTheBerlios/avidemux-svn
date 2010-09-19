########################################
# gettext
########################################
INCLUDE(admCheckGettext)

checkGettext()

IF (NOT ADMLOCALE)
	SET(ADM_LOCALE "${CMAKE_INSTALL_PREFIX}/share/locale")
ENDIF (NOT ADMLOCALE)

########################################
# OpenGL
########################################
OPTION(OPENGL "" ON)

MESSAGE(STATUS "Checking for OpenGL")
MESSAGE(STATUS "*******************")

IF (OPENGL)
	IF (QT4_FOUND AND QT_VERSION_MINOR GREATER 5)
		IF (QT_QTOPENGL_FOUND)
			MESSAGE(STATUS "Found QtOpenGL")
			FIND_PACKAGE(OpenGL)
			PRINT_LIBRARY_INFO("OpenGL" OPENGL_FOUND "${OPENGL_INCLUDE_DIR}" "${OPENGL_LIBRARIES}")
		ELSE (QT_QTOPENGL_FOUND)
			MESSAGE(STATUS "QtOpenGL was not found")
		ENDIF (QT_QTOPENGL_FOUND)
	ELSE (QT4_FOUND AND QT_VERSION_MINOR GREATER 5)
		MESSAGE(STATUS "OpenGL is only available for Qt 4.6 or later")
	ENDIF (QT4_FOUND AND QT_VERSION_MINOR GREATER 5)

	IF (OPENGL_FOUND)
		SET(USE_OPENGL 1)
	ENDIF (OPENGL_FOUND)
ELSE (OPENGL)
	MESSAGE("${MSG_DISABLE_OPTION}")
ENDIF (OPENGL)

MESSAGE("")

########################################
# SDL
########################################
OPTION(SDL "" ON)

MESSAGE(STATUS "Checking for SDL")
MESSAGE(STATUS "****************")

IF (SDL)
	FIND_PACKAGE(SDL)
	PRINT_LIBRARY_INFO("SDL" SDL_FOUND "${SDL_INCLUDE_DIR}" "${SDL_LIBRARY}")
	
	MARK_AS_ADVANCED(SDLMAIN_LIBRARY)
	MARK_AS_ADVANCED(SDL_INCLUDE_DIR)
	MARK_AS_ADVANCED(SDL_LIBRARY)

	IF (SDL_FOUND)
		SET(USE_SDL 1)
	ENDIF (SDL_FOUND)
ELSE (SDL)
	MESSAGE("${MSG_DISABLE_OPTION}")
ENDIF (SDL)

MESSAGE("")

########################################
# XVideo
########################################
IF (UNIX AND NOT APPLE)
	OPTION(XVIDEO "" ON)

	IF (XVIDEO)
		MESSAGE(STATUS "Checking for XVideo")
		MESSAGE(STATUS "*******************")

		FIND_HEADER_AND_LIB(XVIDEO X11/extensions/Xvlib.h Xv XvShmPutImage)
		PRINT_LIBRARY_INFO("XVideo" XVIDEO_FOUND "${XVIDEO_INCLUDE_DIR}" "${XVIDEO_LIBRARY_DIR}")

		IF (XVIDEO_FOUND)
			SET(USE_XV 1)
		ENDIF (XVIDEO_FOUND)

		MESSAGE("")
	ENDIF (XVIDEO)
ELSE (UNIX AND NOT APPLE)
	SET(XVIDEO_CAPABLE FALSE)
ENDIF (UNIX AND NOT APPLE)

########################################
# SpiderMonkey
########################################
OPTION(USE_SYSTEM_SPIDERMONKEY "" OFF)

MESSAGE(STATUS "Checking for SpiderMonkey")
MESSAGE(STATUS "*************************")

IF (USE_SYSTEM_SPIDERMONKEY)
	FIND_HEADER_AND_LIB(SPIDERMONKEY jsapi.h js JS_InitStandardClasses)
	PRINT_LIBRARY_INFO("SpiderMonkey" SPIDERMONKEY_FOUND "${SPIDERMONKEY_INCLUDE_DIR}" "${SPIDERMONKEY_LIBRARY_DIR}" FATAL_ERROR)
ELSE (USE_SYSTEM_SPIDERMONKEY)
	MESSAGE("Skipping check and using bundled version.")
ENDIF (USE_SYSTEM_SPIDERMONKEY)

MESSAGE("")
