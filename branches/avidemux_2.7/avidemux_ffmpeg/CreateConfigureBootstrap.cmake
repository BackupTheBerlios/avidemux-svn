MACRO (ffmpegAddFlag opt)
	if ("${ARGV1}" STREQUAL "")
		set(FFMPEG_FLAGS "${FFMPEG_FLAGS} ${opt}")
	else ("${ARGV1}" STREQUAL "")
		string(STRIP ${ARGV1} arg)
		set(FFMPEG_FLAGS "${FFMPEG_FLAGS} ${opt}=\"${arg}\"")
	endif ("${ARGV1}" STREQUAL "")
ENDMACRO (ffmpegAddFlag)

option(FF_INHERIT_BUILD_ENV "" ON)

set(FFMPEG_DECODERS  aac ac3 eac3 adpcm_ima_amv  amv  bmp  cinepak  cyuv  dca  dvbsub  dvvideo  ffv1  ffvhuff  flv  fraps  h263  h264  huffyuv  mjpeg
					 mjpegb  mpeg2video  mpeg4  msmpeg4v2  msmpeg4v3  msvideo1  nellymoser  png  qdm2  rawvideo  snow
					 svq3  theora  tscc  mp2 mp3 mp2_float mp3_float
					 vc1  vp3  vp6  vp6a  vp6f  vp8 wmapro wmav2  wmv1  wmv2  wmv3)
set(FFMPEG_ENCODERS  ac3  ac3_float dvvideo  ffv1  ffvhuff  flv  h263  huffyuv  mjpeg  mp2  mpeg1video  mpeg2video  mpeg4  snow aac)
set(FFMPEG_MUXERS  flv  matroska  mpeg1vcd  mpeg2dvd  mpeg2svcd  mpegts  mov  mp4  psp)
set(FFMPEG_PARSERS  ac3  h263  h264  mpeg4video)
set(FFMPEG_PROTOCOLS  file)
ffmpegAddFlag("--enable-shared --disable-static --disable-everything --disable-avfilter --enable-hwaccels --enable-postproc --enable-gpl")
ffmpegAddFlag("--enable-runtime-cpudetect --disable-network --disable-ffplay --disable-ffprobe")
ffmpegAddFlag("--enable-swscale --disable-swresample")

if (NOT CROSS)
	ffmpegAddFlag(--prefix ${CMAKE_INSTALL_PREFIX})
endif (NOT CROSS)

if (USE_VDPAU)
	ffmpegAddFlag(--enable-vdpau)
	set(FFMPEG_DECODERS ${FFMPEG_DECODERS} h264_vdpau  vc1_vdpau  mpeg1_vdpau  mpeg_vdpau  wmv3_vdpau)
endif (USE_VDPAU)

ffmpegAddFlag(--enable-bsf aac_adtstoasc)

# Configure FFmpeg, if required
foreach (decoder ${FFMPEG_DECODERS})
	ffmpegAddFlag(--enable-decoder ${decoder})
endforeach (decoder)

foreach (encoder ${FFMPEG_ENCODERS})
	ffmpegAddFlag(--enable-encoder ${encoder})
endforeach (encoder)

foreach (muxer ${FFMPEG_MUXERS})
	ffmpegAddFlag(--enable-muxer ${muxer})
endforeach (muxer)

foreach (parser ${FFMPEG_PARSERS})
	ffmpegAddFlag(--enable-parser ${parser})
endforeach (parser)

foreach (protocol ${FFMPEG_PROTOCOLS})
	ffmpegAddFlag(--enable-protocol ${protocol})
endforeach (protocol)

if (WIN32)
	if (ADM_CPU_X86_32)
		ffmpegAddFlag(--enable-memalign-hack)
	endif (ADM_CPU_X86_32)

	ffmpegAddFlag(--enable-w32threads)
else (WIN32)
	ffmpegAddFlag(--enable-pthreads)
endif (WIN32)

if (NOT ADM_DEBUG)
	ffmpegAddFlag(--disable-debug)
endif (NOT ADM_DEBUG)

#  Cross compiler override (win32 & win64)
if (CROSS)
	if(APPLE)
		ffmpegAddFlag(--prefix /opt/mac)
		ffmpegAddFlag(--host-cc gcc)
		ffmpegAddFlag(--nm ${CMAKE_CROSS_PREFIX}-nm) 
		ffmpegAddFlag(--strip ${CMAKE_CROSS_PREFIX}-strip) 

		set(CROSS_OS darwin)
		set(CROSS_ARCH i386)
	else(APPLE)
		ffmpegAddFlag(--prefix /mingw)
		ffmpegAddFlag(--host-cc gcc)
		ffmpegAddFlag(--nm ${CMAKE_CROSS_PREFIX}-nm) 
		ffmpegAddFlag(--sysroot /mingw/include)

		set(CROSS_OS mingw32)	

		if (ADM_CPU_64BIT)
			set(CROSS_ARCH x86_64)
		else (ADM_CPU_64BIT)
			set(CROSS_ARCH i386)
		endif (ADM_CPU_64BIT)
	endif(APPLE)

	message(STATUS "Using cross compilation flag: ${FFMPEG_FLAGS}")
endif (CROSS)

if (FF_INHERIT_BUILD_ENV)
	ffmpegAddFlag(--cc "${CMAKE_C_COMPILER}")
	ffmpegAddFlag(--ld "${CMAKE_C_COMPILER}")
	ffmpegAddFlag(--ar "${CMAKE_AR}")
	# nm should be ok if we do not cross compile

	if (CMAKE_C_FLAGS)
		ffmpegAddFlag(--extra-cflags ${CMAKE_C_FLAGS})
	endif (CMAKE_C_FLAGS)

	if (CMAKE_SHARED_LINKER_FLAGS)
		ffmpegAddFlag(--extra-ldflags ${CMAKE_SHARED_LINKER_FLAGS})
	endif (CMAKE_SHARED_LINKER_FLAGS)

	if (VERBOSE)
		# for ffmpeg to use the same  compiler as others
		MESSAGE(STATUS "Configuring FFmpeg with CC=${CMAKE_C_COMPILER}")
		MESSAGE(STATUS "Configuring FFmpeg with LD=${CMAKE_C_COMPILER}")
		MESSAGE(STATUS "Configuring FFmpeg with AR=${CMAKE_AR}")
		MESSAGE(STATUS "Configuring FFmpeg with CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
		MESSAGE(STATUS "Configuring FFmpeg with CFLAGS=${FF_FLAGS}")
		MESSAGE(STATUS "Configuring FFmpeg with ${FFMPEG_FLAGS}")
		message("")
	endif (VERBOSE)
endif (FF_INHERIT_BUILD_ENV)

if (CROSS_ARCH OR CROSS_OS)
	ffmpegAddFlag(--enable-cross-compile)
endif (CROSS_ARCH OR CROSS_OS)

if (CROSS_ARCH)
	set(CROSS_ARCH "${CROSS_ARCH}" CACHE STRING "")
	ffmpegAddFlag(--arch ${CROSS_ARCH})
endif (CROSS_ARCH)

if (CROSS_OS)
	set(CROSS_OS "${CROSS_OS}" CACHE STRING "")
	ffmpegAddFlag(--target-os ${CROSS_OS})
endif (CROSS_OS)

if (FF_FLAGS)
	set(FF_FLAGS "${FF_FLAGS}" CACHE STRING "")
	ffmpegAddFlag(${FF_FLAGS})
endif (FF_FLAGS)

configure_file(ffmpeg_configure.sh.cmake ffmpeg_configure.sh)
