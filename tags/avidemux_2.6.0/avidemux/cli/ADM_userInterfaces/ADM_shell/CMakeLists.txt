SET(ADM_LIB ADM_shellCli)

SET(${ADM_LIB}_SRCS 
        cliShell.cpp
)

INCLUDE_DIRECTORIES("${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/common/ADM_script2/include/")
ADD_LIBRARY(${ADM_LIB} STATIC ${${ADM_LIB}_SRCS})
