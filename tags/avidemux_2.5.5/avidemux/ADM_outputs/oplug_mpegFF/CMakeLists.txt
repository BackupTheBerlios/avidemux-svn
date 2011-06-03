SET(ADM_oplug_mpegFF_SRCS 
	oplug_vcdff.cpp)

include_directories("${PTHREAD_INCLUDE_DIR}")
ADD_ADM_LIB_ALL_TARGETS(oplug_mpegFF ${ADM_oplug_mpegFF_SRCS})
