INCLUDE(admCheckQt4)
INCLUDE(admPluginVideoEncoder)

checkQt4()

IF (QT4_FOUND)
	QT4_WRAP_UI(xvidQt_headers
		xvidConfigDialog.ui)
	QT4_WRAP_CPP(xvidQt_source
		xvidConfigDialog.h)
	SET(ADM_vidEnc_xvid_Qt_SRCS
		${xvidQt_headers} ${xvidQt_source} xvidConfigDialog.cpp)

	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIR})
	ADD_LIBRARY(${QT_PLUGIN_NAME} SHARED ${ADM_vidEnc_xvid_Qt_SRCS})
	TARGET_LINK_LIBRARIES(${QT_PLUGIN_NAME} ADM_vidEnc_xvid ADM_coreUI ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY})

	INIT_VIDEO_ENCODER_PLUGIN(${QT_PLUGIN_NAME})
	INSTALL(TARGETS ${QT_PLUGIN_NAME} DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/ADM_plugins/videoEncoder/${PLUGIN_SUBDIR}")
ENDIF (QT4_FOUND)