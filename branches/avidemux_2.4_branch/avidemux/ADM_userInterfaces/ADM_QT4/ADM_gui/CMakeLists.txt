################################
#
################################
include(AdmLib)
SET(ADM_LIB ADM_guiQt4)

SET(${ADM_LIB}_SRCS
avidemux_rsc.cpp  file_qt4.cpp  gui_none.cpp  ADM_qslider.cpp  ADM_qtray.cpp
)

SET(${ADM_LIB}_T
tray
preview
)

SET(${ADM_LIB}_Q
gui2
preview
)

Q_RCC(avidemux)

include(adm_qt4)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories(${QT_INCLUDES})

