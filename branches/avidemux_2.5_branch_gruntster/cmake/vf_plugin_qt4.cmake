MACRO(INIT_VIDEOFILTER_PLUGIN_QT4  lib  _srcsQt _headersQt _srcQt_ui  )
	INCLUDE(admCheckQt4)

	checkQt4()

	IF (QT4_FOUND)
		INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIR})
		INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/qt4)
		INCLUDE_DIRECTORIES(${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_UIs/ADM_QT4/include/)

		QT4_WRAP_UI(qt4_ui ${_srcQt_ui}.ui)
		QT4_WRAP_CPP(qt4_cpp ${_headersQt})
		#${_srcQt_ui}.h)

		ADD_LIBRARY(${lib} SHARED ${ARGN} ${_srcsQt} ${qt4_cpp} ${qt4_ui})
		ADD_TARGET_CFLAGS(${lib} "-DADM_UI_TYPE_BUILD=4")
	        ADD_TARGET_CFLAGS(${lib} "-I${CMAKE_BINARY_DIR}/config/qt4 ")

		TARGET_LINK_LIBRARIES( ${lib} ADM_UIQT4  ADM_render_qt4)
		TARGET_LINK_LIBRARIES(${lib} ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY})

		INIT_VIDEOFILTER_PLUGIN(${lib})
		INSTALL_VIDEOFILTER(${lib})
	ENDIF (QT4_FOUND)
ENDMACRO(INIT_VIDEOFILTER_PLUGIN_QT4)
