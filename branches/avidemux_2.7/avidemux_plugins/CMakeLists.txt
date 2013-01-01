cmake_minimum_required(VERSION 2.6)

SET(ADM_PROJECT AdmPlugins)

MESSAGE("")
MESSAGE("#####################################")
MESSAGE("Plugins Configure Started")
MESSAGE("#####################################")
MESSAGE("")

SET(PLUGINS TRUE)
########################################
# Where is the top dir ?
########################################
IF (NOT AVIDEMUX_SOURCE_DIR)
	MESSAGE(FATAL_ERROR "Please add -DAVIDEMUX_SOURCE_DIR=path_to_avidemux_source")
ELSE (NOT AVIDEMUX_TOP_SOURCE_DIR)
	# We need TOP_SOURCE_DIR
	SET(AVIDEMUX_TOP_SOURCE_DIR "${AVIDEMUX_SOURCE_DIR}" CACHE STRING "")
ENDIF (NOT AVIDEMUX_SOURCE_DIR)

# Common definitions...
SET(CMAKE_MODULE_PATH "${AVIDEMUX_TOP_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")
include(${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/commonCmakeApplication.cmake)

###########################################
# MSVC install hack
###########################################
if (MSVC)
	ADD_CUSTOM_TARGET(install_after_all ALL COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_CONFIG_NAME=$<CONFIGURATION> -P ${CMAKE_BINARY_DIR}/cmake_install.cmake)
	CONFIGURE_FILE("${AVIDEMUX_TOP_SOURCE_DIR}/cmake/ALL_BUILD.vcxproj.user.cmake" "${CMAKE_BINARY_DIR}/ALL_BUILD.vcxproj.user")
endif (MSVC)

########################################
# Add include dirs
########################################
include(admInstallDir)

#########################################
# Get regular stuff from main app
#########################################
include(admCoreIncludes)

########################################
# retrieve core config, needed to get USE_VDPAU and friends
########################################
include(${ADM_CORE_INCLUDE_PATH}/ADM_coreConfig.cmake)

########################################
# Global options
########################################
OPTION(VERBOSE "" OFF)

########################################
# Select our UI
########################################
SET(DO_GTK    0)
SET(DO_QT4    0)
SET(DO_COMMON 0)
SET(DO_CLI    0)

IF (NOT PLUGIN_UI)
	MESSAGE(FATAL_ERROR "Please add -DPLUGIN_UI=[ALL|QT4|GTK|CLI|COMMON]")
ENDIF (NOT PLUGIN_UI)

if(PLUGIN_UI MATCHES "ALL")
    SET(DO_COMMON 1)
    SET(DO_GTK    1)
    SET(DO_QT4    1)
    SET(DO_CLI    1)
endif(PLUGIN_UI MATCHES "ALL")

if(PLUGIN_UI MATCHES "QT4")
    SET(DO_QT4    1)
endif(PLUGIN_UI MATCHES "QT4")

if(PLUGIN_UI MATCHES "GTK")
    SET(DO_GTK    1)
endif(PLUGIN_UI MATCHES "GTK")

if(PLUGIN_UI MATCHES "COMMON")
    SET(DO_COMMON    1)
endif(PLUGIN_UI MATCHES "COMMON")

if(PLUGIN_UI MATCHES "CLI")
    SET(DO_CLI    1)
endif(PLUGIN_UI MATCHES "CLI")

MESSAGE(STATUS "Plugin UI : ${PLUGIN_UI}")
MESSAGE("")

########################################
# Check for UI libraries
########################################
MACRO(CHECK_ADM_CONFIG_H flavour result)
    if ("${flavour}" STREQUAL "cli")
        set(adm_include_dir "${ADM_CLI_INCLUDE_PATH}")
        set(adm_lib_dir "${ADM_CLI_LIBRARY_PATH}")
    elseif ("${flavour}" STREQUAL "gtk")
        set(adm_include_dir "${ADM_GTK_INCLUDE_PATH}")
        set(adm_lib_dir "${ADM_GTK_LIBRARY_PATH}")
    elseif ("${flavour}" STREQUAL "qt4")
        set(adm_include_dir "${ADM_QT_INCLUDE_PATH}")
        set(adm_lib_dir "${ADM_QT_LIBRARY_PATH}")
    endif ("${flavour}" STREQUAL "cli")

    MESSAGE(STATUS "Checking for ${flavour} includes and lib...")
    MESSAGE(STATUS "It should be ${adm_include_dir}/config.h")
    set(ADM_INC "NOTFOUND")
    FIND_PATH(ADM_INC config.h "${adm_include_dir}")

    if (${ADM_INC} MATCHES "NOTFOUND")
        SET(${result} 0)
        MESSAGE(STATUS "Include for ${flavour} not found")
    else (${ADM_INC} MATCHES "NOTFOUND")
        SET(${result} 1)
        MESSAGE(STATUS "Include for ${flavour} found")

        link_directories("${adm_lib_dir}")
        include_directories("${adm_include_dir}")
    endif (${ADM_INC} MATCHES "NOTFOUND")
ENDMACRO(CHECK_ADM_CONFIG_H flavour result)

APPEND_SUMMARY_LIST("User Interface" "Common" "${DO_COMMON}")

#*****
# GTK
#*****
if(DO_GTK)
    INCLUDE(admCheckGtk)
    INCLUDE(admCheckGettext)
    checkGtk()
    checkGettext()
    IF (GTK_FOUND AND GTHREAD_FOUND)
        MESSAGE(STATUS "Gtk and gthread found, good")
        CHECK_ADM_CONFIG_H(gtk ADM_INC)
        IF(NOT ADM_INC)
            MESSAGE("ERROR GTK UI requested, but cannot find header file for avidemux/gtk...disabling it")
			MESSAGE("")
            SET(DO_GTK 0)
        ENDIF(NOT ADM_INC)
    else (GTK_FOUND AND GTHREAD_FOUND)
        MESSAGE("ERROR GTK UI requested, but gtk/gthread libraries not found... disabling it")
		MESSAGE("")
        SET(DO_GTK 0)
    endif (GTK_FOUND AND GTHREAD_FOUND)
endif(DO_GTK)

APPEND_SUMMARY_LIST("User Interface" "GTK+" "${DO_GTK}")

#*****
# QT4
#*****
if(DO_QT4)
    INCLUDE(admCheckQt4)
    checkQt4()
    IF (NOT QT4_FOUND)
        SET(DO_QT4 0)
        MESSAGE("ERROR QT4 UI requested, but qt4 libraries not found... disabling it")
		MESSAGE("")
    ELSE (NOT QT4_FOUND)
        MESSAGE(STATUS "QT4 libraries found, good")
        CHECK_ADM_CONFIG_H(qt4 ADM_INC)
		MESSAGE("")

        INCLUDE(admCheckOpenGl)
        IF(NOT ADM_INC)
            MESSAGE("ERROR QT4 UI requested, but cannot find header file for avidemux/qt4...disabling it")
			MESSAGE("")
            SET(DO_QT4 0)
        ENDIF(NOT ADM_INC)
    ENDIF (NOT QT4_FOUND)
endif(DO_QT4)

APPEND_SUMMARY_LIST("User Interface" "Qt 4" "${DO_QT4}")

#*****
# CLI
#*****
if(DO_CLI)
		MESSAGE(STATUS "Checking for CLI")
		MESSAGE(STATUS "****************")

        CHECK_ADM_CONFIG_H(cli ADM_INC)

        IF(NOT ADM_INC)
            MESSAGE("ERROR CLI UI requested, but cannot find header file for avidemux/cli...disabling it")
            SET(DO_CLI 0)
        ENDIF(NOT ADM_INC)

		MESSAGE("")
endif(DO_CLI)

APPEND_SUMMARY_LIST("User Interface" "CLI" "${DO_CLI}")

########################################
# Actual work to do..
########################################
IF(DO_COMMON)
    ADD_SUBDIRECTORY(ADM_audioDecoders)
    ADD_SUBDIRECTORY(ADM_audioDevices)
    ADD_SUBDIRECTORY(ADM_videoDecoder)
    ADD_SUBDIRECTORY(ADM_audioEncoders)
    ADD_SUBDIRECTORY(ADM_demuxers)
    ADD_SUBDIRECTORY(ADM_muxers)
    ADD_SUBDIRECTORY(ADM_autoScrips)
    add_subdirectory(ADM_scriptEngines)
ENDIF(DO_COMMON)

ADD_SUBDIRECTORY(ADM_videoEncoder)
ADD_SUBDIRECTORY(ADM_videoFilters6)
IF(DO_QT4)
	IF(USE_OPENGL)
         ADD_SUBDIRECTORY(ADM_videoFilters6_openGl)
	ENDIF(USE_OPENGL)
ENDIF(DO_QT4)

########################################
# Config Summary
########################################
DISPLAY_SUMMARY_LIST()

include(admPackager)
admPackager(pluginsPackage)
MESSAGE("")
