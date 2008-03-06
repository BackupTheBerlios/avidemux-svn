
SET(ADM_core_SRCS 
ADM_cpuCap.cpp     ADM_memcpy.cpp  ADM_memsupport.cpp  ADM_threads.cpp  ADM_win32.cpp
ADM_memory.cpp  ADM_misc.cpp        TLK_clock.cpp
ADM_crashdump.cpp
ADM_fileio.cpp
ADM_dynamicLoading.cpp
)

ADD_LIBRARY(ADM_core STATIC ${ADM_core_SRCS})
