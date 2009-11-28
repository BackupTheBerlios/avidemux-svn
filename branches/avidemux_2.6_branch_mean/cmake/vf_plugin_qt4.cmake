MACRO(INIT_VIDEO_FILTER_QT4  lib  _srcsQt _headersQt _srcQt_ui  )
	INCLUDE(admCheckQt4)

	checkQt4()

	IF (QT4_FOUND)
		INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIR})
		INCLUDE_DIRECTORIES(${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/qt4/ADM_UIs/include/)
		QT4_WRAP_UI(qt4_ui ${_srcQt_ui}.ui)
		QT4_WRAP_CPP(qt4_cpp ${_headersQt})

		ADD_LIBRARY(${lib} SHARED ${ARGN} ${_srcsQt} ${qt4_cpp} ${qt4_ui})
		ADD_TARGET_CFLAGS(${lib} "-DADM_UI_TYPE_BUILD=4")
		TARGET_LINK_LIBRARIES( ${lib} ADM_UIQT4  ADM_render6_qt4)
		TARGET_LINK_LIBRARIES(${lib} ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY})

		INIT_VIDEO_FILTER(${lib})
		INSTALL_VIDEO_FILTER(${lib})
	ENDIF (QT4_FOUND)
ENDMACRO(INIT_VIDEO_FILTER_QT4)
