INCLUDE(vf_plugin)
include(admFFmpegUtil)

SET(ADM_vf_Mosaic_SRCS ADM_vidMosaic.cpp)

getFfmpegLibNames("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/")
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")

add_library(ADM_libswscale UNKNOWN IMPORTED)
set_property(TARGET ADM_libswscale PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBSWSCALE_LIB}")

add_library(ADM_libavcodec UNKNOWN IMPORTED)
set_property(TARGET ADM_libavcodec PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBAVCODEC_LIB}")

add_library(ADM_libavutil UNKNOWN IMPORTED)
set_property(TARGET ADM_libavutil PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBAVUTIL_LIB}")

ADD_LIBRARY(ADM_vf_Mosaic SHARED ${ADM_vf_Mosaic_SRCS})
TARGET_LINK_LIBRARIES(ADM_vf_Mosaic ADM_libswscale ADM_libavcodec ADM_libavutil)

INIT_VIDEOFILTER_PLUGIN(ADM_vf_Mosaic)
INSTALL_VIDEOFILTER(ADM_vf_Mosaic)
