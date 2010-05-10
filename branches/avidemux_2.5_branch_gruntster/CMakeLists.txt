MESSAGE("#####################################")
MESSAGE("Configure Started")
MESSAGE("#####################################")
MESSAGE("")

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2 FATAL_ERROR)

if (COMMAND cmake_policy)
	cmake_policy(SET CMP0004 OLD)
endif (COMMAND cmake_policy)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

IF (NOT CMAKE_CXX_COMPILER_WORKS MATCHES "^${CMAKE_CXX_COMPILER_WORKS}$")
	SET(FRESH_BUILD 1)
ENDIF (NOT CMAKE_CXX_COMPILER_WORKS MATCHES "^${CMAKE_CXX_COMPILER_WORKS}$")

# need for shared cmake script between avidemux & plugins
SET(AVIDEMUX_SOURCE_DIR ${CMAKE_SOURCE_DIR})
MESSAGE(STATUS "Source dir is ${AVIDEMUX_SOURCE_DIR}")
MESSAGE("")

PROJECT(Avidemux)

IF (${Avidemux_SOURCE_DIR} MATCHES ${Avidemux_BINARY_DIR})
	MESSAGE("Please do an out-of-tree build:")
	MESSAGE("rm CMakeCache.txt; mkdir build; cd build; cmake ..; make")
	MESSAGE(FATAL_ERROR "in-tree-build detected")
ENDIF (${Avidemux_SOURCE_DIR} MATCHES ${Avidemux_BINARY_DIR})

INCLUDE(admConfigHelper)

IF (FRESH_BUILD)
	MESSAGE("")
ENDIF (FRESH_BUILD)

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

IF (CYGWIN)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwin32")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwin32")
ENDIF (CYGWIN)

IF (ADM_CPU_ALTIVEC)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ADM_ALTIVEC_FLAGS}")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADM_ALTIVEC_FLAGS}")
ENDIF (ADM_CPU_ALTIVEC)

IF (UNIX AND NOT APPLE)
	# jog shuttle is only available on Linux due to its interface
	SET(USE_JOG 1)
ENDIF (UNIX AND NOT APPLE)

IF (WIN32)
	SET(BIN_DIR .)
	SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-enable-auto-image-base -Wl,-enable-auto-import")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-enable-auto-import")

	IF (CMAKE_BUILD_TYPE STREQUAL "Release")
		SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-s")
		SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-s")
	ENDIF (CMAKE_BUILD_TYPE STREQUAL "Release")
ELSE (WIN32)
	SET(BIN_DIR bin)
ENDIF (WIN32)

IF (APPLE)
	## Add needed Compiler and Linker flags for OSX
	SET(CMAKE_C_FLAGS:STRING "\"$CMAKE_C_FLAGS\" -flat_namespace -undefined suppress" )
	SET(CMAKE_CXX_FLAGS:STRING "\"$CMAKE_CXX_FLAGS\" -flat_namespace -undefined suppress" )
	SET(CMAKE_LDFLAGS:STRING "\"$CMAKE_LD_FLAGS\"" )
	SET(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib" )

	IF (${CMAKE_BUILD_TYPE} STREQUAL "Release")
		SET(CMAKE_C_FLAGS_RELEASE:STRING " $CMAKE_C_FLAGS_RELEASE ")
		SET(CMAKE_CXX_FLAGS_RELEASE:STRING " $CMAKE_CXX_FLAGS_RELEASE ")
	ENDIF (${CMAKE_BUILD_TYPE} STREQUAL "Release")

	if (${MAC_SELF_CONTAINED_BUNDLE})
		MESSAGE(STATUS "Configuring avidemux for fully bundled version. (not yet working correctly)")
		SET(CMAKE_INSTALL_NAME_DIR "@executable_path/../Libraries" )
	endif (${MAC_SELF_CONTAINED_BUNDLE})
ENDIF (APPLE)

########################################
# Standard Avidemux defines
########################################
SET(VERSION 2.5.3)

# Define internal flags for GTK+ and Qt4 builds.  These are turned off
# if a showstopper is found.  CLI is automatically assumed as possible
# since it uses the minimum set of required libraries and CMake will 
# fail if these aren't met.
SET(ADM_UI_GTK 1)
SET(ADM_UI_QT4 1)

SET(ADM_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(ADM_DEBUG 1)
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")

########################################
# Subversion
########################################
OPTION(SVN "" ON)
SET(ADM_SUBVERSION 0)

IF (SVN)
	MESSAGE(STATUS "Checking for SCM Revision")
	MESSAGE(STATUS "*************************")

	SET(Subversion_FIND_REQUIRED OFF)
	include(admGetRevision)
	admGetRevision(${PROJECT_SOURCE_DIR} ADM_SUBVERSION)
	MESSAGE(STATUS "  revision: ${ADM_SUBVERSION}")

	MESSAGE("")
ENDIF (SVN)

########################################
# Check for libraries
########################################
SET(MSG_DISABLE_OPTION "Disabled per request")

INCLUDE(admCheckRequiredLibs)
INCLUDE(admCheckGtk)
INCLUDE(admCheckQt4)

checkGtk()
checkQt4()

#INCLUDE(admCheckAudioCodecLibs)
#INCLUDE(admCheckAudioDeviceLibs)
INCLUDE(admCheckMiscLibs)

########################################
# Check functions and includes
########################################
IF (NOT SYSTEM_HEADERS_CHECKED)
	MESSAGE(STATUS "Checking system headers")
	MESSAGE(STATUS "***********************")

	INCLUDE(CheckIncludeFiles)
	INCLUDE(CheckFunctionExists)

	CHECK_FUNCTION_EXISTS(gettimeofday HAVE_GETTIMEOFDAY)

	CHECK_INCLUDE_FILES(inttypes.h   HAVE_INTTYPES_H)	# internal use, mpeg2enc, mplex
	CHECK_INCLUDE_FILES(stdint.h     HAVE_STDINT_H)		# internal use, mpeg2enc, mplex
	CHECK_INCLUDE_FILES(sys/types.h  HAVE_SYS_TYPES_H)	# mad, mpeg2enc
	CHECK_INCLUDE_FILES(malloc.h     HAVE_MALLOC_H)		# mplex
	SET(SYSTEM_HEADERS_CHECKED 1 CACHE BOOL "")
	MARK_AS_ADVANCED(SYSTEM_HEADERS_CHECKED)

	MESSAGE("")
ENDIF (NOT SYSTEM_HEADERS_CHECKED)

########################################
# Debug Summary
########################################
IF (VERBOSE)
	INCLUDE(adm_log)
	MESSAGE("")
ENDIF (VERBOSE)

########################################
# Generate config.h
########################################
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake/ADM_coreConfig.h.cmake" "${CMAKE_BINARY_DIR}/config/ADM_coreConfig.h")
MESSAGE(STATUS "ADM_coreConfig.h generated")

SET(CONFIG_HEADER_TYPE ADM_BUILD_CLI)
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake/config.h.cmake" "${CMAKE_BINARY_DIR}/config/cli/config.h")
MESSAGE(STATUS "CLI config.h generated")

SET(CONFIG_HEADER_TYPE ADM_BUILD_GTK)
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake/config.h.cmake" "${CMAKE_BINARY_DIR}/config/gtk/config.h")
MESSAGE(STATUS "GTK config.h generated")

SET(CONFIG_HEADER_TYPE ADM_BUILD_QT4)
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake/config.h.cmake" "${CMAKE_BINARY_DIR}/config/qt4/config.h")
MESSAGE(STATUS "Qt4 config.h generated")

MESSAGE("")

########################################
# Directories to build
########################################
ADD_SUBDIRECTORY(avidemux)
ADD_SUBDIRECTORY(po)
ADD_SUBDIRECTORY(scripts)
# MEANX : DOES NOT WORK ADD_SUBDIRECTORY(plugins EXCLUDE_FROM_ALL)

########################################
# Config Summary
########################################
INCLUDE(admConfigSummary)
MESSAGE("")
