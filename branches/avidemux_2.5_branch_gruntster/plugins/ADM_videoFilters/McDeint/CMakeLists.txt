INCLUDE(vf_plugin)
include(admFFmpegUtil)

getFfmpegLibNames("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux")
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")
SET(ADM_vf_mcdeint_SRCS ADM_vidMcDeint.cpp)

add_library(ADM_libavcodec UNKNOWN IMPORTED)
set_property(TARGET ADM_libavcodec PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBAVCODEC_LIB}")

add_library(ADM_libavutil UNKNOWN IMPORTED)
set_property(TARGET ADM_libavutil PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBAVUTIL_LIB}")

ADD_LIBRARY(ADM_vf_mcdeint SHARED ${ADM_vf_mcdeint_SRCS})
TARGET_LINK_LIBRARIES(ADM_vf_mcdeint ADM_libavcodec ADM_libavutil)

INIT_VIDEOFILTER_PLUGIN(ADM_vf_mcdeint)
INSTALL_VIDEOFILTER(ADM_vf_mcdeint)
