if (NOT DEFINED PKG_CONFIG_FOUND)
	MESSAGE(STATUS "Checking for pkg-config")
	MESSAGE(STATUS "***********************")

	FIND_PACKAGE(PkgConfig)

	if (${CMAKE_VERSION} VERSION_EQUAL 2.8.8)
		# workaround for bug in CMake 2.8.8 (http://www.cmake.org/Bug/view.php?id=13125)
		set(PKG_CONFIG_FOUND ${PKGCONFIG_FOUND})
	endif (${CMAKE_VERSION} VERSION_EQUAL 2.8.8)

	IF (NOT PKG_CONFIG_FOUND)
		MESSAGE(WARNING "Could not find pkg-config")
	ENDIF (NOT PKG_CONFIG_FOUND)

	MESSAGE(STATUS "Found pkg-config")

	IF (VERBOSE)
		MESSAGE(STATUS "Path: ${PKG_CONFIG_EXECUTABLE}")
	ENDIF (VERBOSE)

	MESSAGE("")
endif (NOT DEFINED PKG_CONFIG_FOUND)