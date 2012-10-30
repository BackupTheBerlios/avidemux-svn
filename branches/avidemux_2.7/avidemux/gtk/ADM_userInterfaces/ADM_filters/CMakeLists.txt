SET(ADM_LIB ADM_filtersGtk)

SET(${ADM_LIB}_SRCS
	gui_filtermanager.cpp  
)

ADD_LIBRARY(${ADM_LIB} STATIC ${${ADM_LIB}_SRCS})
FILE(GLOB ADM_filterIcons ${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/common/ADM_icons/videoFilter/*.png)
ADM_INSTALL_GLADE_LIB_FILES(videoFilter videoFilter.gtkBuilder ${ADM_filterIcons})
