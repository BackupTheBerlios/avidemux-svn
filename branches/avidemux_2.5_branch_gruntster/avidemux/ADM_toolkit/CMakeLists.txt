SET(ADM_toolkit_SRCS 
	ADM_audioQueue.cpp  ADM_packetQueue.cpp  automation.cpp)

include_directories("${PTHREAD_INCLUDE_DIR}")
ADD_ADM_LIB_ALL_TARGETS(ADM_toolkit ${ADM_toolkit_SRCS})
