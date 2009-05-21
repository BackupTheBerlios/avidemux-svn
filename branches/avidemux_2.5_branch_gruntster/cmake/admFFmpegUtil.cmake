MACRO (getFfmpegVersionFromHeader description headerFile definition ffmpegVersion)
	FILE(READ "${headerFile}" FFMPEG_H)
	STRING(REGEX MATCH "#define[ ]+${definition}[ ]+([0-9]+)" FFMPEG_H "${FFMPEG_H}")
	STRING(REGEX REPLACE ".*[ ]([0-9]+).*" "\\1" ${ffmpegVersion} "${FFMPEG_H}")
ENDMACRO (getFfmpegVersionFromHeader)

MACRO (getFfmpegLibNames sourceDir)
	getFfmpegVersionFromHeader("libavcodec" "${sourceDir}/libavcodec/avcodec.h" LIBAVCODEC_VERSION_MAJOR LIBAVCODEC_VERSION)
	getFfmpegVersionFromHeader("libavformat" "${sourceDir}/libavformat/avformat.h" LIBAVFORMAT_VERSION_MAJOR LIBAVFORMAT_VERSION)
	getFfmpegVersionFromHeader("libavutil" "${sourceDir}/libavutil/avutil.h" LIBAVUTIL_VERSION_MAJOR LIBAVUTIL_VERSION)
	getFfmpegVersionFromHeader("libpostproc" "${sourceDir}/libpostproc/postprocess.h" LIBPOSTPROC_VERSION_MAJOR LIBPOSTPROC_VERSION)
	getFfmpegVersionFromHeader("libswscale" "${sourceDir}/libswscale/swscale.h" LIBSWSCALE_VERSION_MAJOR LIBSWSCALE_VERSION)
        
        set(LIBAVCODEC_ADM ADM5)

	if (APPLE)
		set(LIBAVCODEC_LIB lib${LIBAVCODEC_ADM}avcodec.${LIBAVCODEC_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
		set(LIBAVFORMAT_LIB lib${LIBAVCODEC_ADM}avformat.${LIBAVFORMAT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
		set(LIBAVUTIL_LIB lib${LIBAVCODEC_ADM}avutil.${LIBAVUTIL_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
		set(LIBPOSTPROC_LIB lib${LIBAVCODEC_ADM}postproc.${LIBPOSTPROC_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
		set(LIBSWSCALE_LIB lib${LIBAVCODEC_ADM}swscale.${LIBSWSCALE_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})	
	elseif (UNIX)
		set(LIBAVCODEC_LIB lib${LIBAVCODEC_ADM}avcodec${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBAVCODEC_VERSION})
		set(LIBAVFORMAT_LIB lib${LIBAVCODEC_ADM}avformat${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBAVFORMAT_VERSION})
		set(LIBAVUTIL_LIB lib${LIBAVCODEC_ADM}avutil${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBAVUTIL_VERSION})
		set(LIBPOSTPROC_LIB lib${LIBAVCODEC_ADM}postproc${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBPOSTPROC_VERSION})
		set(LIBSWSCALE_LIB lib${LIBAVCODEC_ADM}swscale${CMAKE_SHARED_LIBRARY_SUFFIX}.${LIBSWSCALE_VERSION})
	else (APPLE)
		set(LIBAVCODEC_LIB ${LIBAVCODEC_ADM}avcodec-${LIBAVCODEC_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
		set(LIBAVFORMAT_LIB ${LIBAVCODEC_ADM}avformat-${LIBAVFORMAT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
		set(LIBAVUTIL_LIB ${LIBAVCODEC_ADM}avutil-${LIBAVUTIL_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
		set(LIBPOSTPROC_LIB ${LIBAVCODEC_ADM}postproc-${LIBPOSTPROC_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
		set(LIBSWSCALE_LIB ${LIBAVCODEC_ADM}swscale-${LIBSWSCALE_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
	endif (APPLE)
ENDMACRO (getFfmpegLibNames)
