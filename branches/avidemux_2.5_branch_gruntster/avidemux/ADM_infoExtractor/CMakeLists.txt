SET(ADM_infoExtractor_SRCS 
	ADM_infoextractor.cpp)

ADD_ADM_LIB_ALL_TARGETS(ADM_infoExtractor ${ADM_infoExtractor_SRCS})
ADD_SOURCE_CFLAGS(ADM_infoextractor.cpp "-I${CMAKE_BINARY_DIR}/avidemux -DHAVE_AV_CONFIG_H")