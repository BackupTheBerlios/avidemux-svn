
# Override...
SET(CMAKE_C_FLAGS $ENV{CFLAGS})
SET(CMAKE_CXX_FLAGS $ENV{CXXFLAGS})
SET(CMAKE_LD_FLAGS $ENV{LDFLAGS})

include(admCrossCompile)
MESSAGE(STATUS "C flags:      ${CMAKE_C_FLAGS}")
MESSAGE(STATUS "CXX flags:    ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "LD flags:     ${CMAKE_LD_FLAGS}")
MESSAGE(STATUS "Compiler:     ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "Linker:       ${CMAKE_LINKER}")
MESSAGE(STATUS "Source Path:  ${AVIDEMUX_TOP_SOURCE_DIR}")
MESSAGE("")

PROJECT(${ADM_PROJECT})

IF (${Avidemux_SOURCE_DIR} MATCHES ${Avidemux_BINARY_DIR})
	MESSAGE("Please do an out-of-tree build:")
	MESSAGE("rm CMakeCache.txt; mkdir build; cd build; cmake ..; make")
	MESSAGE(FATAL_ERROR "in-tree-build detected")
ENDIF (${Avidemux_SOURCE_DIR} MATCHES ${Avidemux_BINARY_DIR})

if (${CMAKE_VERSION} VERSION_GREATER 2.8.5)
	include(GenerateExportHeader)
else (${CMAKE_VERSION} VERSION_GREATER 2.8.5)
	include(_GenerateExportHeader)
endif (${CMAKE_VERSION} VERSION_GREATER 2.8.5)

INCLUDE(admConfigHelper)
include(admGetRevision)

########################################
# Global options
########################################
OPTION(VERBOSE "" OFF)

IF (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE "Release")
ENDIF (NOT CMAKE_BUILD_TYPE)

########################################
# Avidemux system specific tweaks
########################################
INCLUDE(admDetermineSystem)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	# jog shuttle is only available on Linux due to its interface
	SET(USE_JOG 1)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

IF (WIN32)
	SET(BIN_DIR .)

	IF (MINGW)
		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-enable-auto-image-base -Wl,-enable-auto-import")
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-enable-auto-import")

		IF (CMAKE_BUILD_TYPE STREQUAL "Release")
			SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-s")
			SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-s")
		ENDIF (CMAKE_BUILD_TYPE STREQUAL "Release")
	ENDIF (MINGW)
ELSE (WIN32)
	SET(BIN_DIR bin)
ENDIF (WIN32)

if (CMAKE_COMPILER_IS_GNUCC)
	add_definitions("-Werror=attributes")
endif (CMAKE_COMPILER_IS_GNUCC)

########################################
# Standard Avidemux defines
########################################
# Define internal flags for GTK+ and Qt4 builds.  These are turned off
# if a showstopper is found.  CLI is automatically assumed as possible
# since it uses the minimum set of required libraries and CMake will 
# fail if these aren't met.
include(admInstallDir)

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(ADM_DEBUG 1)
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")
########################################
# Subversion
########################################

MESSAGE("")
MESSAGE(STATUS "Checking for SCM")
MESSAGE(STATUS "****************")
admGetRevision( ${AVIDEMUX_TOP_SOURCE_DIR} ADM_SUBVERSION)
MESSAGE("")


########################################
# Check for libraries
########################################
SET(MSG_DISABLE_OPTION "Disabled per request")
INCLUDE(admCheckRequiredLibs)
IF(NOT PLUGINS)
INCLUDE(admCheckMiscLibs)
INCLUDE(FindThreads)
INCLUDE(admCheckVDPAU)
checkVDPAU()
ENDIF(NOT PLUGINS)

########################################
# Check functions and includes
########################################
if (MSVC)
	set(msvc_include_dir "${AVIDEMUX_TOP_SOURCE_DIR}/avidemux_core/ADM_core/include/msvc") 
	include_directories(${msvc_include_dir})
	set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${msvc_include_dir})
endif (MSVC)

IF (NOT SYSTEM_HEADERS_CHECKED)
	MESSAGE(STATUS "Checking system headers")
	MESSAGE(STATUS "***********************")

	INCLUDE(CheckIncludeFiles)
	INCLUDE(CheckFunctionExists)

	set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${CMAKE_INCLUDE_PATH})
	CHECK_FUNCTION_EXISTS(gettimeofday HAVE_GETTIMEOFDAY)

	CHECK_INCLUDE_FILES(inttypes.h   HAVE_INTTYPES_H)	# internal use, mpeg2enc, mplex
	CHECK_INCLUDE_FILES(stdint.h     HAVE_STDINT_H)		# internal use, mpeg2enc, mplex
	SET(SYSTEM_HEADERS_CHECKED 1 CACHE BOOL "")
	MARK_AS_ADVANCED(SYSTEM_HEADERS_CHECKED)

	MESSAGE("")
ENDIF (NOT SYSTEM_HEADERS_CHECKED)

