SET(ADM_oplug_flv_SRCS oplug_flv.cpp)

include_directories("${PTHREAD_INCLUDE_DIR}")
ADD_ADM_LIB_ALL_TARGETS(oplug_flv ${ADM_oplug_flv_SRCS})