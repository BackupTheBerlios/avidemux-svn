SET(VERBATIM "VERBATIM")

# Cygwin and MSYS require escaping but standalone MinGW doesn't.
# Since CMake doesn't provide a way of detecting MSYS, check for
# sh.exe in the path.  MinGW makefiles can't be genereated if sh
# is in the path.
IF(MINGW)
	FIND_PROGRAM(SH_EXECUTABLE NAMES sh)
	
	IF(NOT SH_EXECUTABLE)   # if not Cygwin or MSYS
		SET(VERBATIM "")
	ENDIF(NOT SH_EXECUTABLE)
ENDIF(MINGW)

##############################################################"
# FOR AQ
# Q_foo.cpp and foo.ui => AQ_foo.cpp, ui_foo.h and Q_foo.moc
##############################################################"
MACRO(Q_UIMOCIFY _source)
  ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/AQ_${_source}.cpp
                     COMMAND  ${QT_UIC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${_source}.ui -o ui_${_source}.h
                     COMMAND  ${QT_MOC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Q_${_source}.cpp -o Q_${_source}.moc
                     COMMAND  echo "#include \"Q_${_source}.cpp\"" > AQ_${_source}.cpp
                     COMMAND  echo "#include \"Q_${_source}.moc\"" >> AQ_${_source}.cpp
                     DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/${_source}.ui  ${CMAKE_CURRENT_SOURCE_DIR}/Q_${_source}.cpp
                     ${VERBATIM}
                )
ENDMACRO(Q_UIMOCIFY _source)

##############################################################"
# FOR 
# foo.qrc->foo_rsc.cpp
##############################################################"
# $(RCC)  $< -o $@   -name filter
MACRO(Q_RCC _source)
  ADD_CUSTOM_COMMAND(OUTPUT   ${CMAKE_CURRENT_BINARY_DIR}/${_source}_rsc.cpp
                     COMMAND  ${QT_RCC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${_source}.qrc -o ${_source}_rsc.cpp -name ${_source}
                     DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/${_source}.qrc
                     ${VERBATIM}
                )
ENDMACRO(Q_RCC _source)

##############################################################"
# FOR AT
# T_foo.cpp i => AT_foo.cpp and T_foo.moc
##############################################################"
MACRO(Q_MOCIFY _source)
  ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/AT_${_source}.cpp
                     COMMAND  ${QT_MOC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/T_${_source}.cpp -o T_${_source}.moc 
                     COMMAND  echo "#include \"T_${_source}.cpp\"" > AT_${_source}.cpp
                     COMMAND  echo "#include \"T_${_source}.moc\"" >> AT_${_source}.cpp
                     DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/T_${_source}.cpp
                     ${VERBATIM}
                )
ENDMACRO(Q_MOCIFY _source)

# Add to our Targets..
FOREACH(_current ${${ADM_LIB}_Q})
 Q_UIMOCIFY(${_current})
 SET(${ADM_LIB}_SRCS ${${ADM_LIB}_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/AQ_${_current}.cpp)
ENDFOREACH(_current ${${ADM_LIB}_Q})

FOREACH(_current ${${ADM_LIB}_T})
 Q_MOCIFY(${_current})
 SET(${ADM_LIB}_SRCS ${${ADM_LIB}_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/AT_${_current}.cpp)
ENDFOREACH(_current ${${ADM_LIB}_T})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(${ADM_LIB} STATIC ${${ADM_LIB}_SRCS})
ADD_ADM_LIB(${ADM_LIB} ADM_libraries)

# EOF
