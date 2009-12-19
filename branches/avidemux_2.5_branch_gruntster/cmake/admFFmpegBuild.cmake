include(admFFmpegUtil)

set(FFMPEG_VERSION 20900)	# http://git.ffmpeg.org/?p=ffmpeg;a=snapshot;h=c2a40c2de2f3b764180c6e74e8c2a88d2f56dfac;sf=tgz
set(SWSCALE_VERSION 30075)	# http://git.ffmpeg.org/?p=libswscale;a=snapshot;h=8158fe4ce45496fb7ded193cd9d64e9e75268be4;sf=tgz

set(LIBRARY_SOURCE_DIR "${CMAKE_SOURCE_DIR}/avidemux/ADM_libraries")
set(FFMPEG_SOURCE_DIR "${LIBRARY_SOURCE_DIR}/ffmpeg")
set(FFMPEG_BINARY_DIR "${CMAKE_BINARY_DIR}/avidemux/ADM_libraries/ffmpeg")

set(FFMPEG_DECODERS  adpcm_ima_amv  amv  bmp  cinepak  dca  dnxhd  dvbsub  dvvideo  ffv1  ffvhuff  flv  fourxm  fraps  h263  h264  huffyuv  indeo2  indeo3
					 interplay_video  mjpeg  mjpegb  mpeg1video  mpeg2video  mpeg4  mpegaudio_hp  mpegvideo  msmpeg4v1  msmpeg4v2  msmpeg4v3  msvideo1
					 nellymoser  png  qdm2  rawvideo rv10  rv20  snow  svq1  svq3  tscc  vc1  vcr1  vp3  vp5  vp6  vp6a  vp6f  wmav2  wmv1  wmv2  wmv3)
set(FFMPEG_ENCODERS  ac3  dvbsub  dvvideo  ffv1  ffvhuff  flv  flv1  h263  h263p  huffyuv  mjpeg  mpeg1video  mpeg2video  mp2  mpeg4  msmpeg4v3  snow)
set(FFMPEG_MUXERS  flv  ipod  matroska  mov  mp4  psp  tg2  tgp)
set(FFMPEG_PARSERS  h263  h264  mpeg4video ac3)
set(FFMPEG_PROTOCOLS  file)
set(FFMPEG_FLAGS  --enable-shared --disable-static --disable-filters --disable-protocols --disable-indevs --disable-outdevs --disable-bsfs
				  --disable-parsers --disable-decoders --disable-encoders --disable-demuxers --disable-muxers --enable-postproc --enable-gpl 
				  --enable-runtime-cpudetect --disable-ffplay --prefix=${CMAKE_INSTALL_PREFIX})

include(admFFmpegPatch)
include(admFFmpegPrepareTar)

if (NOT FFMPEG_PREPARED)
	include(admFFmpegPrepareSvn)
endif (NOT FFMPEG_PREPARED)

if (NOT VERBOSE)
	set(ffmpegBuildOutput OUTPUT_VARIABLE FFMPEG_CONFIGURE_OUTPUT)
endif (NOT VERBOSE)

message("")

if (FFMPEG_PERFORM_PATCH)
	find_patch()
	file(GLOB patchFiles "${CMAKE_SOURCE_DIR}/cmake/patches/*.patch")

	foreach(patchFile ${patchFiles})
		patch_file("${FFMPEG_SOURCE_DIR}" "${patchFile}")
	endforeach(patchFile)

	if (UNIX AND NOT APPLE)
		patch_file("${FFMPEG_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/cmake/patches/common.mak.diff")
	endif (UNIX AND NOT APPLE)

	message("")
endif (FFMPEG_PERFORM_PATCH)

# Configure FFmpeg, if required
foreach (decoder ${FFMPEG_DECODERS})
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-decoder=${decoder})
endforeach (decoder)

foreach (encoder ${FFMPEG_ENCODERS})
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-encoder=${encoder})
endforeach (encoder)

foreach (muxer ${FFMPEG_MUXERS})
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-muxer=${muxer})
endforeach (muxer)

foreach (parser ${FFMPEG_PARSERS})
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-parser=${parser})
endforeach (parser)

foreach (protocol ${FFMPEG_PROTOCOLS})
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-protocol=${protocol})
endforeach (protocol)

if (WIN32)
	if (ADM_CPU_X86_32)
		set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-memalign-hack)
	endif (ADM_CPU_X86_32)

	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-w32threads)
else (WIN32)
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-pthreads)
endif (WIN32)

if (NOT ADM_DEBUG)
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --disable-debug)
endif (NOT ADM_DEBUG)

if (CMAKE_C_FLAGS)
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --extra-cflags=${CMAKE_C_FLAGS})
endif (CMAKE_C_FLAGS)

if (CMAKE_SHARED_LINKER_FLAGS)
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --extra-ldflags=${CMAKE_SHARED_LINKER_FLAGS})
endif (CMAKE_SHARED_LINKER_FLAGS)

if (XPLATFORM)
	set(XPLATFORM "${XPLATFORM}" CACHE STRING "")
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-cross-compile --arch=${XPLATFORM})
endif (XPLATFORM)

if (NOT "${LAST_FFMPEG_FLAGS}" STREQUAL "${FFMPEG_FLAGS}")
	set(FFMPEG_PERFORM_BUILD 1)
endif (NOT "${LAST_FFMPEG_FLAGS}" STREQUAL "${FFMPEG_FLAGS}")

if (NOT EXISTS "${FFMPEG_BINARY_DIR}/Makefile")
	set(FFMPEG_PERFORM_BUILD 1)
endif (NOT EXISTS "${FFMPEG_BINARY_DIR}/Makefile")

if (FFMPEG_PERFORM_BUILD)
	message(STATUS "Configuring FFmpeg")
	set(LAST_FFMPEG_FLAGS "${FFMPEG_FLAGS}" CACHE STRING "" FORCE)

	file(MAKE_DIRECTORY "${FFMPEG_BINARY_DIR}")
	file(REMOVE "${FFMPEG_BINARY_DIR}/ffmpeg${CMAKE_EXECUTABLE_SUFFIX}")
	file(REMOVE "${FFMPEG_BINARY_DIR}/ffmpeg_g${CMAKE_EXECUTABLE_SUFFIX}")

	execute_process(COMMAND sh ${FFMPEG_SOURCE_DIR}/configure ${FFMPEG_FLAGS}
					WORKING_DIRECTORY "${FFMPEG_BINARY_DIR}"
					${ffmpegBuildOutput})

	if (UNIX AND NOT APPLE)
		find_patch()
		patch_file("${FFMPEG_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/cmake/patches/config.mak.diff")
	endif (UNIX AND NOT APPLE)

	message("")
endif (FFMPEG_PERFORM_BUILD)

# Build FFmpeg
add_custom_command(OUTPUT "${FFMPEG_BINARY_DIR}/ffmpeg${CMAKE_EXECUTABLE_SUFFIX}"
				   COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TOOL=${CMAKE_BUILD_TOOL} -P "${CMAKE_SOURCE_DIR}/cmake/admFFmpegMake.cmake"
				   WORKING_DIRECTORY "${FFMPEG_BINARY_DIR}")

add_custom_target(ffmpeg ALL
				  DEPENDS "${FFMPEG_BINARY_DIR}/ffmpeg${CMAKE_EXECUTABLE_SUFFIX}")

# Add and install libraries
getFfmpegLibNames("${FFMPEG_SOURCE_DIR}")

if (WIN32)
	set(FFMPEG_INSTALL_DIR ${BIN_DIR})
else (WIN32)
	set(FFMPEG_INSTALL_DIR lib${LIB_SUFFIX})
endif (WIN32)

add_library(ADM_libswscale UNKNOWN IMPORTED)
set_property(TARGET ADM_libswscale PROPERTY IMPORTED_LOCATION "${FFMPEG_BINARY_DIR}/libswscale/${LIBSWSCALE_LIB}")
install(FILES "${FFMPEG_BINARY_DIR}/libswscale/${LIBSWSCALE_LIB}" DESTINATION "${FFMPEG_INSTALL_DIR}")

add_library(ADM_libpostproc UNKNOWN IMPORTED)
set_property(TARGET ADM_libpostproc PROPERTY IMPORTED_LOCATION "${FFMPEG_BINARY_DIR}/libpostproc/${LIBPOSTPROC_LIB}")
install(FILES "${FFMPEG_BINARY_DIR}/libpostproc/${LIBPOSTPROC_LIB}" DESTINATION "${FFMPEG_INSTALL_DIR}")

add_library(ADM_libavutil UNKNOWN IMPORTED)
set_property(TARGET ADM_libavutil PROPERTY IMPORTED_LOCATION "${FFMPEG_BINARY_DIR}/libavutil/${LIBAVUTIL_LIB}")
install(FILES "${FFMPEG_BINARY_DIR}/libavutil/${LIBAVUTIL_LIB}" DESTINATION "${FFMPEG_INSTALL_DIR}")

add_library(ADM_libavcodec UNKNOWN IMPORTED)
set_property(TARGET ADM_libavcodec PROPERTY IMPORTED_LOCATION "${FFMPEG_BINARY_DIR}/libavcodec/${LIBAVCODEC_LIB}")
install(FILES "${FFMPEG_BINARY_DIR}/libavcodec/${LIBAVCODEC_LIB}" DESTINATION "${FFMPEG_INSTALL_DIR}")

add_library(ADM_libavformat UNKNOWN IMPORTED)
set_property(TARGET ADM_libavformat PROPERTY IMPORTED_LOCATION "${FFMPEG_BINARY_DIR}/libavformat/${LIBAVFORMAT_LIB}")
install(FILES "${FFMPEG_BINARY_DIR}/libavformat/${LIBAVFORMAT_LIB}" DESTINATION "${FFMPEG_INSTALL_DIR}")

include_directories("${FFMPEG_SOURCE_DIR}")
include_directories("${FFMPEG_SOURCE_DIR}/libavutil")
include_directories("${FFMPEG_SOURCE_DIR}/libpostproc")

# Clean FFmpeg
add_custom_target(cleanffmpeg
				  COMMAND ${CMAKE_COMMAND} -P "${CMAKE_SOURCE_DIR}/cmake/admFFmpegClean.cmake"
				  WORKING_DIRECTORY "${FFMPEG_BINARY_DIR}"
				  COMMENT "Cleaning FFmpeg")
