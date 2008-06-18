################################
#
################################
include(AdmLib)
SET(ADM_LIB ADM_filtersGtk)
#
SET(${ADM_LIB}_SRCS
gui_filtermanager.cpp  gui_filtermanager_dialog.cpp

)
ADD_LIBRARY(${ADM_LIB} STATIC ${${ADM_LIB}_SRCS})
ADD_ADM_LIB(${ADM_LIB} ADM_libraries)


################################
ADD_definitions(${GTK_CFLAGS})
include_directories(.. ../..)
#/../ADM_osSupport ../../../ADM_inputs)
