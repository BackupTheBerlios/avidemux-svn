########################################
# pkg-config
########################################
MESSAGE(STATUS "Checking for pkg-config")
MESSAGE(STATUS "***********************")

FIND_PACKAGE(PkgConfig)

IF (NOT PKG_CONFIG_FOUND)
	MESSAGE(FATAL_ERROR "Could not find pkg-config")
ENDIF (NOT PKG_CONFIG_FOUND)

MESSAGE(STATUS "Found pkg-config")

IF (VERBOSE)
	MESSAGE(STATUS "Path: ${PKG_CONFIG_EXECUTABLE}")
ENDIF (VERBOSE)

MESSAGE("")

########################################
# Libxml2
########################################
MESSAGE(STATUS "Checking for Libxml2")
MESSAGE(STATUS "********************")

SET(LibXml2_FIND_QUIETLY TRUE)

FIND_PACKAGE(LibXml2)
PRINT_LIBRARY_INFO("Libxml2" LIBXML2_FOUND "${LIBXML2_INCLUDE_DIR} ${LIBXML2_DEFINITIONS}" "${LIBXML2_LIBRARIES}" FATAL_ERROR)

SET(USE_LIBXML2 1)
MESSAGE("")

########################################
# libpng
########################################
MESSAGE(STATUS "Checking for libpng")
MESSAGE(STATUS "*******************")

FIND_PACKAGE(PNG)
PRINT_LIBRARY_INFO("libpng" PNG_FOUND "${PNG_INCLUDE_DIR} ${PNG_DEFINITIONS}" "${PNG_LIBRARIES}" FATAL_ERROR)

FOREACH(_flag ${PNG_INCLUDE_DIR})
	SET(PNG_CFLAGS ${PNG_CFLAGS} -I${_flag})
ENDFOREACH(_flag)

SET(PNG_CFLAGS ${PNG_CFLAGS} ${PNG_DEFINITIONS})
SET(USE_PNG 1)
MESSAGE("")