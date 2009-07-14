# OpenCORE AMR-NB decoder
INCLUDE(admCheckOpencore)
checkOpencore()

IF (OPENCORE_AMRNB_FOUND)
	INCLUDE(ad_plugin)
	
	SET(ADM_ad_opencore_SRCS ADM_ad_opencore.cpp)

	ADD_LIBRARY(ADM_ad_opencore SHARED ${ADM_ad_opencore_SRCS})
	TARGET_LINK_LIBRARIES(ADM_ad_opencore ADM_core ${OPENCORE_AMRNB_LIBRARY_DIR})

	ADD_TARGET_CFLAGS(ADM_ad_opencore "-I${OPENCORE_AMRNB_INCLUDE_DIR}")

	INIT_AUDIO_PLUGIN(ADM_ad_opencore)
	INSTALL_AUDIODECODER(ADM_ad_opencore)
ENDIF (OPENCORE_AMRNB_FOUND)
