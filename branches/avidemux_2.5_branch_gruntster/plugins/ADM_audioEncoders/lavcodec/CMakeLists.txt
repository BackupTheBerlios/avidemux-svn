	INCLUDE(ae_plugin)
	include(admFFmpegUtil)

	getFfmpegLibNames("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")
	INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux")
	INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")

	SET(ADM_ae_lav_mp2_SRCS audioencoder_lavcodec.cpp)
	
	add_library(ADM_libavcodec UNKNOWN IMPORTED)
	set_property(TARGET ADM_libavcodec PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBAVCODEC_LIB}")	
	add_library(ADM_libavutil UNKNOWN IMPORTED)
	set_property(TARGET ADM_libavutil PROPERTY IMPORTED_LOCATION "${FFMPEG_LIB_DIR}/${LIBAVUTIL_LIB}")	

	ADD_LIBRARY(ADM_ae_lav_mp2 SHARED ${ADM_ae_lav_mp2_SRCS})
	ADD_TARGET_CFLAGS(ADM_ae_lav_mp2 "-DADM_LAV_MP2")
	TARGET_LINK_LIBRARIES(ADM_ae_lav_mp2 ADM_core ADM_coreAudio ADM_coreUI ADM_libavcodec ADM_libavutil)

	INIT_AUDIO_ENCODER(ADM_ae_lav_mp2)
	INSTALL_AUDIOENCODER(ADM_ae_lav_mp2)

	SET(ADM_ae_lav_ac3_SRCS audioencoder_lavcodec.cpp)

	ADD_LIBRARY(ADM_ae_lav_ac3 SHARED ${ADM_ae_lav_ac3_SRCS})
	ADD_TARGET_CFLAGS(ADM_ae_lav_ac3 "-DADM_LAV_AC3")
	TARGET_LINK_LIBRARIES(ADM_ae_lav_ac3 ADM_core ADM_coreAudio ADM_coreUI ADM_libavcodec ADM_libavutil)

	INIT_AUDIO_ENCODER(ADM_ae_lav_ac3)
	INSTALL_AUDIOENCODER(ADM_ae_lav_ac3)
