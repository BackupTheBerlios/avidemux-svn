INCLUDE(vf_plugin)
include(admFFmpegUtil)

getFfmpegLibNames("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")

SET(ADM_vf_forcedPP_SRCS ADM_vidForcedPP.cpp)

add_library(ADM_libpostproc UNKNOWN IMPORTED)
set_property(TARGET ADM_libpostproc PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBPOSTPROC_LIB}")

ADD_LIBRARY(ADM_vf_forcedPP SHARED ${ADM_vf_forcedPP_SRCS})
TARGET_LINK_LIBRARIES(ADM_vf_forcedPP ADM_libpostproc)

INIT_VIDEOFILTER_PLUGIN(ADM_vf_forcedPP)
INSTALL_VIDEOFILTER(ADM_vf_forcedPP)
