INCLUDE(vf_plugin)
include(admFFmpegUtil)

SET(resizeCommon_SRCS ADM_vidMPLResize.cpp)
SET(resizeCommonCLI_SRCS cli/cli_resize.cpp)
SET(resizeCommonGtk_SRCS gtk/gtk_resize.cpp)
SET(resizeCommonQT_SRCS qt4/Q_resize.cpp)
SET(resizeCommonQT_Headers qt4/Q_resize.h)
SET(resizeCommonQT_UI qt4/resizing)

getFfmpegLibNames("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/")
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")

add_library(ADM_libswscale UNKNOWN IMPORTED)
set_property(TARGET ADM_libswscale PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBSWSCALE_LIB}")

add_library(ADM_libavcodec UNKNOWN IMPORTED)
set_property(TARGET ADM_libavcodec PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBAVCODEC_LIB}")

add_library(ADM_libavutil UNKNOWN IMPORTED)
set_property(TARGET ADM_libavutil PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBAVUTIL_LIB}")
 # GTK
INCLUDE(vf_plugin_gtk)
INIT_VIDEOFILTER_PLUGIN_GTK(ADM_vf_mplayerResize_gtk ${resizeCommonGtk_SRCS} ${resizeCommon_SRCS})

IF (GTK_FOUND AND GTHREAD_FOUND)
	TARGET_LINK_LIBRARIES(ADM_vf_mplayerResize_gtk ADM_libswscale ADM_libavcodec ADM_libavutil ADM_UIGtk)
ENDIF (GTK_FOUND AND GTHREAD_FOUND)
#/GTK
#QT
INCLUDE(vf_plugin_qt4)
INIT_VIDEOFILTER_PLUGIN_QT4(ADM_vf_mplayerResize_qt4 ${resizeCommonQT_SRCS} ${resizeCommonQT_Headers} ${resizeCommonQT_UI} ${resizeCommon_SRCS})

IF (QT4_FOUND)
	TARGET_LINK_LIBRARIES(ADM_vf_mplayerResize_qt4 ADM_libswscale ADM_libavcodec ADM_libavutil ADM_UIQT4)
ENDIF (QT4_FOUND)
#/QT
#CLI
##
INCLUDE (vf_plugin_cli)
INIT_VIDEOFILTER_PLUGIN_CLI(ADM_vf_mplayerResize_cli ${resizeCommon_SRCS} ${resizeCommonCLI_SRCS})
TARGET_LINK_LIBRARIES(ADM_vf_mplayerResize_cli ADM_libswscale ADM_libavcodec ADM_libavutil )
INSTALL_VIDEOFILTER(ADM_vf_mplayerResize_cli)
#CLI
