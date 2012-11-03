cmake_minimum_required(VERSION 2.6)

MESSAGE("")
MESSAGE("###################################")
MESSAGE(" Configure for FFmpeg libs Started ")
MESSAGE("###################################")
MESSAGE("")

PROJECT(admFfmpeg)
MESSAGE("")

SET(AVIDEMUX_TOP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/..)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} "${AVIDEMUX_TOP_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

SET(CMAKE_C_FLAGS $ENV{CFLAGS})
SET(CMAKE_CXX_FLAGS $ENV{CXXFLAGS})
SET(CMAKE_LD_FLAGS $ENV{LDFLAGS})

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(ADM_DEBUG 1)
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")

include(admConfigHelper)
include(admDetermineSystem)
include(admGetRevision)
include(admVersion)
include(admInstallDir)
include(admFFmpegUtil)
message("")

admGetRevision(${AVIDEMUX_TOP_SOURCE_DIR} ADM_SUBVERSION)
message("")

set(FFMPEG_VERSION "1.0.0")
set(FFMPEG_SOURCE_ARCHIVE "ffmpeg-${FFMPEG_VERSION}.tar.bz2")
set(FFMPEG_SOURCE_ARCHIVE_DIR "ffmpeg-${FFMPEG_VERSION}")
set(FFMPEG_BASE_DIR "${CMAKE_BINARY_DIR}/ffmpeg")
set(FFMPEG_SOURCE_DIR "${FFMPEG_BASE_DIR}/source")
set(FFMPEG_BINARY_DIR "${FFMPEG_BASE_DIR}/build")
set(FFMPEG_TEMP_DIR "${FFMPEG_BASE_DIR}/temp")

find_package(Bourne)
find_package(Tar)
find_package(Patch)

include(CreateConfigureBootstrap)

set_directory_properties(${CMAKE_CURRENT_BINARY_DIR} ADDITIONAL_MAKE_CLEAN_FILES "${FFMPEG_BASE_DIR}")

if ("${LAST_FFMPEG_VERSION}" STREQUAL "${FFMPEG_VERSION}")
	getFfmpegLibNames("${FFMPEG_TEMP_DIR}/${FFMPEG_SOURCE_ARCHIVE_DIR}")
else ("${LAST_FFMPEG_VERSION}" STREQUAL "${FFMPEG_VERSION}")
	file(REMOVE "${FFMPEG_SOURCE_DIR}/adm_extract")
	set(LAST_FFMPEG_FLAGS "##" CACHE STRING "" FORCE)
	set(LAST_FFMPEG_VERSION "${FFMPEG_VERSION}" CACHE STRING "" FORCE)

	extractFfmpegLibNames("${CMAKE_CURRENT_SOURCE_DIR}/${FFMPEG_SOURCE_ARCHIVE}" "${FFMPEG_SOURCE_ARCHIVE_DIR}" "${FFMPEG_TEMP_DIR}")
endif ("${LAST_FFMPEG_VERSION}" STREQUAL "${FFMPEG_VERSION}")

if (NOT "${LAST_FFMPEG_FLAGS}" STREQUAL "${FFMPEG_FLAGS}")
	file(REMOVE "${FFMPEG_BINARY_DIR}/adm_configure")
	file(REMOVE "${FFMPEG_BINARY_DIR}/ffmpeg${CMAKE_EXECUTABLE_SUFFIX}")
	set(LAST_FFMPEG_FLAGS "${FFMPEG_FLAGS}" CACHE STRING "" FORCE)
endif (NOT "${LAST_FFMPEG_FLAGS}" STREQUAL "${FFMPEG_FLAGS}")

add_custom_command(OUTPUT "${FFMPEG_SOURCE_DIR}/adm_extract"
				   COMMAND "${CMAKE_COMMAND}" -D FFMPEG_BASE_DIR="${FFMPEG_BASE_DIR}" -D FFMPEG_SOURCE_DIR="${FFMPEG_SOURCE_DIR}"
				   -D FFMPEG_SOURCE_ARCHIVE="${FFMPEG_SOURCE_ARCHIVE}" -D FFMPEG_SOURCE_ARCHIVE_DIR="${FFMPEG_SOURCE_ARCHIVE_DIR}"
				   -D PATCH_EXECUTABLE="${PATCH_EXECUTABLE}" -D TAR_EXECUTABLE="${TAR_EXECUTABLE}" -D VERBOSE="${VERBOSE}"
				   -P "${CMAKE_CURRENT_SOURCE_DIR}/PrepareSource.cmake"
				   WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_command(OUTPUT "${FFMPEG_BINARY_DIR}/adm_configure"
				   COMMAND "${CMAKE_COMMAND}" -D FFMPEG_BINARY_DIR="${FFMPEG_BINARY_DIR}" -D BASH_EXECUTABLE=${BASH_EXECUTABLE}
						   -P "${CMAKE_CURRENT_SOURCE_DIR}/ConfigureFFmpeg.cmake"
				   DEPENDS "${FFMPEG_SOURCE_DIR}/adm_extract")

add_custom_command(OUTPUT "${FFMPEG_BINARY_DIR}/ffmpeg${CMAKE_EXECUTABLE_SUFFIX}"
				   COMMAND ${CMAKE_BUILD_TOOL} WORKING_DIRECTORY "${FFMPEG_BINARY_DIR}"
				   DEPENDS "${FFMPEG_BINARY_DIR}/adm_configure")

add_custom_target(ffmpeg ALL DEPENDS "${FFMPEG_BINARY_DIR}/ffmpeg${CMAKE_EXECUTABLE_SUFFIX}")

# Add and install libraries
if (MINGW)
	find_program(MSVC_LIB_PATH lib)

	if (MSVC_LIB_PATH)
		# FFmpeg will also produce MSVC libraries which we need to install
		message(STATUS "MSVC Library Manager detected")

		set(MINGW 0)
		set(MSVC 1)
		set(orig_cmake_import_library_suffix "${CMAKE_IMPORT_LIBRARY_SUFFIX}")
		set(CMAKE_IMPORT_LIBRARY_SUFFIX ".lib")
		getFfmpegLibNames("${FFMPEG_TEMP_DIR}/${FFMPEG_SOURCE_ARCHIVE_DIR}")

		ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libswscale/${LIBSWSCALE_IMPORT_LIB}")
		ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libpostproc/${LIBPOSTPROC_IMPORT_LIB}")
		ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libavutil/${LIBAVUTIL_IMPORT_LIB}")
		ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libavcodec/${LIBAVCODEC_IMPORT_LIB}")
		ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libavformat/${LIBAVFORMAT_IMPORT_LIB}")

		set(MINGW 1)
		set(MSVC 0)
		set(CMAKE_IMPORT_LIBRARY_SUFFIX "${orig_cmake_import_library_suffix}")
		getFfmpegLibNames("${FFMPEG_TEMP_DIR}/${FFMPEG_SOURCE_ARCHIVE_DIR}")
	else (MSVC_LIB_PATH)
		message(STATUS "MSVC Library Manager not detected")
	endif (MSVC_LIB_PATH)
endif (MINGW)

ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libswscale/${LIBSWSCALE_IMPORT_LIB}")
ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libpostproc/${LIBPOSTPROC_IMPORT_LIB}")
ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libavutil/${LIBAVUTIL_IMPORT_LIB}")
ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libavcodec/${LIBAVCODEC_IMPORT_LIB}")
ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libavformat/${LIBAVFORMAT_IMPORT_LIB}")

ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libswscale/${LIBSWSCALE_SHARED_LIB}")
ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libpostproc/${LIBPOSTPROC_SHARED_LIB}")
ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libavutil/${LIBAVUTIL_SHARED_LIB}")
ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libavcodec/${LIBAVCODEC_SHARED_LIB}")
ADM_INSTALL_LIB_FILES("${FFMPEG_BINARY_DIR}/libavformat/${LIBAVFORMAT_SHARED_LIB}")

ADM_INSTALL_FFMPEG_HEADER_FILES(libavutil "${FFMPEG_BINARY_DIR}/libavutil/avconfig.h")
ADM_INSTALL_FFMPEG_HEADER_FILES(libavcodec
	"${FFMPEG_SOURCE_DIR}/libavcodec/ac3.h" "${FFMPEG_SOURCE_DIR}/libavcodec/ac3_parser.h"
	"${FFMPEG_SOURCE_DIR}/libavcodec/ac3tab.h" "${FFMPEG_SOURCE_DIR}/libavcodec/avcodec.h"
	"${FFMPEG_SOURCE_DIR}/libavcodec/ff_spsinfo.h" "${FFMPEG_SOURCE_DIR}/libavcodec/get_bits.h"
	"${FFMPEG_SOURCE_DIR}/libavcodec/golomb.h" "${FFMPEG_SOURCE_DIR}/libavcodec/mathops.h"
	"${FFMPEG_SOURCE_DIR}/libavcodec/old_codec_ids.h" "${FFMPEG_SOURCE_DIR}/libavcodec/parser.h"
	"${FFMPEG_SOURCE_DIR}/libavcodec/put_bits.h" "${FFMPEG_SOURCE_DIR}/libavcodec/vdpau.h"
	"${FFMPEG_SOURCE_DIR}/libavcodec/version.h")
ADM_INSTALL_FFMPEG_HEADER_FILES(libavformat
	"${FFMPEG_SOURCE_DIR}/libavformat/avformat.h" "${FFMPEG_SOURCE_DIR}/libavformat/avio.h"
	"${FFMPEG_SOURCE_DIR}/libavformat/flv.h" "${FFMPEG_SOURCE_DIR}/libavformat/url.h"
	"${FFMPEG_SOURCE_DIR}/libavformat/version.h")
ADM_INSTALL_FFMPEG_HEADER_FILES(libavutil
	"${FFMPEG_SOURCE_DIR}/libavutil/attributes.h" "${FFMPEG_SOURCE_DIR}/libavutil/audioconvert.h"
	"${FFMPEG_SOURCE_DIR}/libavutil/avassert.h"	"${FFMPEG_SOURCE_DIR}/libavutil/avutil.h"
	"${FFMPEG_SOURCE_DIR}/libavutil/bswap.h" "${FFMPEG_SOURCE_DIR}/libavutil/common.h"
	"${FFMPEG_SOURCE_DIR}/libavutil/cpu.h" "${FFMPEG_SOURCE_DIR}/libavutil/dict.h"
	"${FFMPEG_SOURCE_DIR}/libavutil/error.h" "${FFMPEG_SOURCE_DIR}/libavutil/intfloat.h"
	"${FFMPEG_SOURCE_DIR}/libavutil/intreadwrite.h" "${FFMPEG_SOURCE_DIR}/libavutil/intfloat_readwrite.h"
	"${FFMPEG_SOURCE_DIR}/libavutil/log.h" "${FFMPEG_SOURCE_DIR}/libavutil/mathematics.h"
	"${FFMPEG_SOURCE_DIR}/libavutil/mem.h" "${FFMPEG_SOURCE_DIR}/libavutil/opt.h"
	"${FFMPEG_SOURCE_DIR}/libavutil/pixfmt.h" "${FFMPEG_SOURCE_DIR}/libavutil/rational.h"
	"${FFMPEG_SOURCE_DIR}/libavutil/samplefmt.h" "${FFMPEG_SOURCE_DIR}/libavutil/time.h"
	"${FFMPEG_SOURCE_DIR}/libavutil/version.h")
ADM_INSTALL_FFMPEG_HEADER_FILES(libpostproc "${FFMPEG_SOURCE_DIR}/libpostproc/postprocess.h")
ADM_INSTALL_FFMPEG_HEADER_FILES(libswscale
	"${FFMPEG_SOURCE_DIR}/libswscale/swscale.h" "${FFMPEG_SOURCE_DIR}/libswscale/version.h")

message("")

include(admPackager)
admPackager(ffmpegPackage)
