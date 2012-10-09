########################################
# Definitions and Includes
########################################
if (NOT PLUGINS)
	SET(AVIDEMUX_TOP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../..)
endif (NOT PLUGINS)

SET(CMAKE_MODULE_PATH "${AVIDEMUX_TOP_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

IF (WIN32)
	SET(ADM_PLUGIN_DIR "plugins")
ELSE (WIN32)
	SET(ADM_PLUGIN_DIR "ADM_plugins6")
ENDIF (WIN32)

include(admConfigSummary)
INITIALISE_SUMMARY_LISTS()

########################################
# Shared cmake part
########################################
ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64 -D_LARGE_FILES)
include(admMainChecks)

########################################
# Add include dirs
########################################
include(admCoreIncludes)
# Add ffmpeg to resolve ADM_libavcodec to the actual name, needed for vdpau
include(admFFmpegUtil)
registerFFmpeg("${AVIDEMUX_SEARCH_INCLUDE_DIR}/avidemux/2.6/ffmpeg" "${AVIDEMUX_SEARCH_LIB_DIR}")

# Verify ADM_coreConfig is there
if(NOT EXISTS "${AVIDEMUX_SEARCH_INCLUDE_DIR}/avidemux/2.6/ADM_coreConfig.h")
        MESSAGE(FATAL_ERROR "CMAKE_INSTALL_PREFIX does not contain include/avidemux/2.6/ADM_coreConfig.h (${AVIDEMUX_SEARCH_INCLUDE_DIR}/avidemux/2.6/ADM_coreConfig.h)")
endif(NOT EXISTS "${AVIDEMUX_SEARCH_INCLUDE_DIR}/avidemux/2.6/ADM_coreConfig.h")

LINK_DIRECTORIES("${AVIDEMUX_SEARCH_LIB_DIR}")
#
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/ADM_muxerGate/include/")

IF (GETTEXT_FOUND)
	INCLUDE_DIRECTORIES(${GETTEXT_INCLUDE_DIR})
ENDIF (GETTEXT_FOUND)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../common/")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../common/ADM_audioFilter/include")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../common/ADM_commonUI")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../common/ADM_videoFilter2/include")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../common/ADM_videoEncoder/include")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/../common/ADM_editor/include")
#
# Add main source
#
SET(ADM_EXE_SRCS
../common/ADM_preview.cpp
../common/ADM_previewNavigate.cpp
../common/gui_main.cpp
../common/gui_autodrive.cpp
../common/GUI_jobs.cpp
../common/gui_navigate.cpp
../common/gui_play.cpp
../common/gui_save.cpp
../common/gui_savenew.cpp
../common/main.cpp
../common/gui_action.cpp
../common/gui_blackframes.cpp
../common/ADM_gettext.cpp
../common/ADM_slave.cpp
)

#############################################
# Add core libs
#############################################
SET(coreLibs
ADM_core6
ADM_audioParser6
ADM_coreAudio6
ADM_coreAudioFilterAPI6
ADM_coreAudioDevice6
ADM_coreAudioEncoder6
ADM_coreDemuxer6
ADM_coreImage6
ADM_coreMuxer6
ADM_coreUI6
ADM_coreUtils6
ADM_coreSocket6
ADM_coreVideoEncoder6
ADM_coreVideoFilter6
)

#############################################
# Add common libs
#############################################
SET(commonLibs1
ADM_muxerGate6
ADM_audioFilter6
ADM_editor6
ADM_audiocodec6
ADM_videocodec6
ADM_coreVideoCodec6
ADM_commonUI6
)

if (USE_VDPAU)
	SET(commonLibs1 ${commonLibs1} ADM_coreVDPAU6)
	SET(commonLibs1 ${commonLibs1} ADM_libavcodec ADM_libavutil)
endif (USE_VDPAU)

SET(commonLibs2
ADM_coreJobs
ADM_osSupport6
ADM_requant6
ADM_script6
ADM_videoEncoder6
ADM_internalVideoFilter6
ADM_toolkit6
)

# END
