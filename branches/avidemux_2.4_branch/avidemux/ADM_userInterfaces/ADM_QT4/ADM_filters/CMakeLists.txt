################################
#
################################
include(AdmLib)
SET(ADM_LIB ADM_filtersQt4)
#
SET(${ADM_LIB}_SRCS
filter_none.cpp  filter_rsc.cpp
)
SET(${ADM_LIB}_Q
mainfilter
seekablePreview
)

SET_SOURCE_FILES_PROPERTIES(filter_rsc.cpp PROPERTIES GENERATED TRUE)

include(adm_qt4)
Q_RCC(filter)

include_directories(.. ../../../ADM_osSupport ../../../ADM_inputs)
include_directories(${QT_INCLUDES})

