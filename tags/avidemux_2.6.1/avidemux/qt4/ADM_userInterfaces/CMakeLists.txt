SET(ADM_LIB ADM_UI_QT46)
SET(${ADM_LIB}_SRCS ui_support.cpp)
ADD_LIBRARY(${ADM_LIB} STATIC ${${ADM_LIB}_SRCS})

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR}  ${CMAKE_CURRENT_SOURCE_DIR}/../../ADM_UIs/ADM_QT4/include)
INCLUDE_DIRECTORIES(ADM_UIs/include/)
subdirs (PREORDER ADM_filters)
ADD_SUBDIRECTORY(ADM_dialog)
ADD_SUBDIRECTORY(ADM_gui)
ADD_SUBDIRECTORY(ADM_shell)
