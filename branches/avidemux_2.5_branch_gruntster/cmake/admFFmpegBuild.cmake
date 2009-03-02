include(admFFmpegUtil)
find_package(Subversion)

set(FFMPEG_VERSION 17480)
set(SWSCALE_VERSION 28690)
set(FFMPEG_SOURCE_DIR "${CMAKE_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")
set(FFMPEG_BINARY_DIR "${CMAKE_BINARY_DIR}/avidemux/ADM_libraries/ffmpeg")

set(FFMPEG_DECODERS  adpcm_ima_amv  amv  bmp  cinepak  dnxhd  dvbsub  dvvideo  ffv1  ffvhuff  flv  fourxm  fraps  h263  h264  huffyuv  indeo2  indeo3
					 interplay_video  mjpeg  mjpegb  mpeg1video  mpeg2video  mpeg4  mpegaudio_hp  mpegvideo  msmpeg4v1  msmpeg4v2  msmpeg4v3  msvideo1
					 nellymoser  png  qdm2  rv10  rv20  snow  svq1  svq3  tscc  vc1  vcr1  vp3  vp5  vp6  vp6a  vp6f  wmav2  wmv1  wmv2  wmv3)
set(FFMPEG_ENCODERS  ac3  dvbsub  dvvideo  ffv1  ffvhuff  flv  flv1  h263  h263p  huffyuv  mjpeg  mpeg1video  mpeg2video  mp2  mpeg4  msmpeg4v3  snow)
set(FFMPEG_MUXERS  flv  ipod  matroska  mov  mp4  psp  tg2  tgp)
set(FFMPEG_PARSERS  h263  h264  mpeg4video)
set(FFMPEG_PROTOCOLS  file)
set(FFMPEG_FLAGS  --enable-shared --disable-static --disable-filters --disable-protocols --disable-indevs --disable-outdevs --disable-bsfs
				  --disable-parsers --disable-decoders --disable-encoders --disable-demuxers --disable-muxers --enable-postproc --enable-swscale --enable-gpl 
				  --prefix=${CMAKE_INSTALL_PREFIX})

if (NOT VERBOSE)
	set(ffmpegSvnOutput OUTPUT_VARIABLE FFMPEG_SVN_OUTPUT)
	set(swscaleSvnOutput OUTPUT_VARIABLE SWSCALE_SVN_OUTPUT)
	set(ffmpegBuildOutput OUTPUT_VARIABLE FFMPEG_CONFIGURE_OUTPUT)
	set(unix2dosOutput OUTPUT_VARIABLE UNIX2DOS_OUTPUT)
endif (NOT VERBOSE)

# Checkout FFmpeg source and patch it
if (NOT IS_DIRECTORY "${FFMPEG_SOURCE_DIR}/.svn")
	find_package(Patch)

	if (WIN32)
		find_package(Unix2Dos)
	endif (WIN32)

	message(STATUS "Checking out FFmpeg")
	execute_process(COMMAND ${Subversion_SVN_EXECUTABLE} co svn://svn.ffmpeg.org/ffmpeg/trunk -r ${FFMPEG_VERSION} --ignore-externals "${FFMPEG_SOURCE_DIR}"
					${ffmpegSvnOutput})

	message(STATUS "Checking out libswscale")
	execute_process(COMMAND ${Subversion_SVN_EXECUTABLE} co svn://svn.ffmpeg.org/mplayer/trunk/libswscale -r ${SWSCALE_VERSION} "${FFMPEG_SOURCE_DIR}/libswscale"
					${swscaleSvnOutput})

	message("")

	set(FFMPEG_PERFORM_PATCH 1)
endif (NOT IS_DIRECTORY "${FFMPEG_SOURCE_DIR}/.svn")

# Check version
Subversion_WC_INFO(${FFMPEG_SOURCE_DIR} ffmpeg)
message(STATUS "FFmpeg revision: ${ffmpeg_WC_REVISION}")

if (NOT ${ffmpeg_WC_REVISION} EQUAL ${FFMPEG_VERSION})
	find_package(Patch)

	if (WIN32)
		find_package(Unix2Dos)
	endif (WIN32)

	MESSAGE(STATUS "Updating to revision ${FFMPEG_VERSION}")
	set(FFMPEG_PERFORM_BUILD 1)
	set(FFMPEG_PERFORM_PATCH 1)

	execute_process(COMMAND ${Subversion_SVN_EXECUTABLE} revert -R "${FFMPEG_SOURCE_DIR}"
					${ffmpegSvnOutput})
	execute_process(COMMAND ${Subversion_SVN_EXECUTABLE} up -r ${FFMPEG_VERSION} --ignore-externals "${FFMPEG_SOURCE_DIR}"
					${ffmpegSvnOutput})	

	message("")
endif (NOT ${ffmpeg_WC_REVISION} EQUAL ${FFMPEG_VERSION})

message("")

Subversion_WC_INFO(${FFMPEG_SOURCE_DIR}/libswscale swscale)
message(STATUS "libswscale revision: ${swscale_WC_REVISION}")

if (NOT ${swscale_WC_REVISION} EQUAL ${SWSCALE_VERSION})
	find_package(Patch)

	if (WIN32)
		find_package(Unix2Dos)
	endif (WIN32)

	message(STATUS "Updating to revision ${SWSCALE_VERSION}")
	set(FFMPEG_PERFORM_BUILD 1)
	set(FFMPEG_PERFORM_PATCH 1)

	execute_process(COMMAND ${Subversion_SVN_EXECUTABLE} revert -R "${FFMPEG_SOURCE_DIR}/libswscale"
					${swscaleSvnOutput})
	execute_process(COMMAND ${Subversion_SVN_EXECUTABLE} up -r ${SWSCALE_VERSION} "${FFMPEG_SOURCE_DIR}/libswscale"
					${swscaleSvnOutput})
endif (NOT ${swscale_WC_REVISION} EQUAL ${SWSCALE_VERSION})

message("")

if (FFMPEG_PERFORM_PATCH)
	file(GLOB patchFiles "${CMAKE_SOURCE_DIR}/cmake/patches/*.patch")

	foreach(patchFile ${patchFiles})
		if (WIN32)
			file(MAKE_DIRECTORY "${FFMPEG_BINARY_DIR}/patches")
			get_filename_component(fileName "${patchFile}" NAME)
			execute_process(COMMAND ${UNIX2DOS_EXECUTABLE} -n ${patchFile} ${FFMPEG_BINARY_DIR}/patches/${fileName}
							${unix2dosOutput})

			set(patchFile "${FFMPEG_BINARY_DIR}/patches/${fileName}")
		endif (WIN32)

		execute_process(COMMAND ${PATCH_EXECUTABLE} -p0 -i "${patchFile}"
						WORKING_DIRECTORY "${FFMPEG_SOURCE_DIR}")
	endforeach(patchFile)

	message("")
endif (FFMPEG_PERFORM_PATCH)

# Configure FFmpeg, if required
foreach (decoder ${FFMPEG_DECODERS})
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-decoder=${decoder})
endforeach (decoder)

foreach (encoder ${FFMPEG_DECODERS})
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
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-memalign-hack --enable-w32threads)
else (WIN32)
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --enable-pthreads)
endif (WIN32)

if (NOT ADM_DEBUG)
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --disable-debug)
endif (NOT ADM_DEBUG)

if (CMAKE_C_FLAGS)
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --extra-cflags=${CMAKE_C_FLAGS})
endif (CMAKE_C_FLAGS)

if (CMAKE_LINKER_FLAGS)
	set(FFMPEG_FLAGS ${FFMPEG_FLAGS} --extra-ldflags=${CMAKE_LINKER_FLAGS})
endif (CMAKE_LINKER_FLAGS)

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
	set(FFMPEG_INSTALL_DIR lib)
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