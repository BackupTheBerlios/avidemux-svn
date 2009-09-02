SET(ADM_oplug_mp4_SRCS oplug_mp4.cpp)

include_directories("${PTHREAD_INCLUDE_DIR}")
ADD_ADM_LIB_ALL_TARGETS(oplug_mp4 ${ADM_oplug_mp4_SRCS})
