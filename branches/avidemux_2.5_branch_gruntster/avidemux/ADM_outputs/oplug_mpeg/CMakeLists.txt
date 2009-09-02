SET(ADM_oplug_mpeg_SRCS 
	op_mpegpass.cpp)

include_directories("${PTHREAD_INCLUDE_DIR}")
ADD_ADM_LIB_ALL_TARGETS(oplug_mpeg ${ADM_oplug_mpeg_SRCS})