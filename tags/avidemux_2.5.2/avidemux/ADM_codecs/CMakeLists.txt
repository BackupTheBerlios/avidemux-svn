SET(ADM_codecs_SRCS 
	ADM_codecs.cpp  ADM_ffmpeg.cpp  ADM_mjpegEncode.cpp
	ADM_ffmp43.cpp  ADM_mjpeg.cpp   ADM_uyvy.cpp)

ADD_ADM_LIB_ALL_TARGETS(ADM_codecs ${ADM_codecs_SRCS})
REMOVE_DEFINITIONS(-DHAVE_CONFIG_H)
