INCLUDE(ad_plugin)

IF (ADM_CPU_X86_32)
	ADD_DEFINITIONS("-DFPM_INTEL")
ELSEIF (ADM_CPU_X86_64)
	ADD_DEFINITIONS("-DFPM_DEFAULT")
ELSEIF (ADM_CPU_PPC)
	ADD_DEFINITIONS("-DFPM_PPC")
ENDIF (ADM_CPU_X86_32)

ADD_SUBDIRECTORY(ADM_libMad)

ADD_DEFINITIONS("-DHAVE_ASSERT_H")
SET(ADM_ad_Mad_SRCS ADM_ad_mad.cpp)

ADD_AUDIO_DECODER( ADM_ad_Mad ${ADM_ad_Mad_SRCS})
TARGET_LINK_LIBRARIES(ADM_ad_Mad ADM_libMad)

INIT_AUDIO_PLUGIN(ADM_ad_Mad)
INSTALL_AUDIODECODER(ADM_ad_Mad)
