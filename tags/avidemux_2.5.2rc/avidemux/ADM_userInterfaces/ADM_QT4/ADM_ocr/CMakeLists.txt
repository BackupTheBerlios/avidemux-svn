SET(ADM_LIB ADM_ocrQt4)

QT4_WRAP_UI(${ADM_LIB}_header  ocr.ui  glypheditor.ui)
QT4_WRAP_CPP(${ADM_LIB}_source  Q_ocr.h  Q_glypheditor.h)

SET(${ADM_LIB}_SRCS
	Q_ocr.cpp  Q_glypheditor.cpp  ${${ADM_LIB}_header}  ${${ADM_LIB}_source})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/avidemux/ADM_UIs/ADM_QT4/include")
ADD_ADM_LIB_QT4_TARGET(${ADM_LIB} ${${ADM_LIB}_SRCS})
