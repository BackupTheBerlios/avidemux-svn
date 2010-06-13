SET(ADM_codecs_SRCS 
	ADM_codecs.cpp  ADM_ffmp43.cpp  ADM_uyvy.cpp
        ADM_vpx.cpp
)

ADD_ADM_LIB_ALL_TARGETS(ADM_codecs ${ADM_codecs_SRCS})
REMOVE_DEFINITIONS(-DHAVE_CONFIG_H)
