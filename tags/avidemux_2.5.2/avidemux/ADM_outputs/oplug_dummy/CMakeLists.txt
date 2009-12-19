SET(ADM_oplug_dummy_SRCS oplug_dummy.cpp)

include_directories("${PTHREAD_INCLUDE_DIR}")
ADD_ADM_LIB_ALL_TARGETS(oplug_dummy ${ADM_oplug_dummy_SRCS})