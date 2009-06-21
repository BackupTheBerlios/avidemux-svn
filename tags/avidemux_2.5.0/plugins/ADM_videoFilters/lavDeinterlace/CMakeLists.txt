INCLUDE(vf_plugin)
include(admFFmpegUtil)

SET(ADM_vf_lavDeinterlace_SRCS ADM_lavpp_deint.cpp)

getFfmpegLibNames("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/")
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")

add_library(ADM_libswscale UNKNOWN IMPORTED)
set_property(TARGET ADM_libswscale PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBSWSCALE_LIB}")

add_library(ADM_libpostproc UNKNOWN IMPORTED)
set_property(TARGET ADM_libpostproc PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBPOSTPROC_LIB}")

ADD_LIBRARY(ADM_vf_lavDeinterlace SHARED ${ADM_vf_lavDeinterlace_SRCS})
TARGET_LINK_LIBRARIES(ADM_vf_lavDeinterlace ADM_libswscale ADM_libpostproc)

INIT_VIDEOFILTER_PLUGIN(ADM_vf_lavDeinterlace)
INSTALL_VIDEOFILTER(ADM_vf_lavDeinterlace)
