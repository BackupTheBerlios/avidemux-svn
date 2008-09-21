SET(ADM_LIB ADM_toolkitQt4)
SET(${ADM_LIB}_SRCS qtToolkit.cpp)

include(adm_qt4)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${QT_INCLUDES})
