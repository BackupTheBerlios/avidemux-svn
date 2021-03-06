INCLUDE(admCheckX264)
INCLUDE(admCheckLibxml2)
INCLUDE(admPluginVideoEncoder)

checkX264()

IF (X264_FOUND)
	checkLibxml2()

	IF (LIBXML2_FOUND)
		SET(QT_PLUGIN_NAME "ADM_vidEnc_x264_Qt")
		SET(GTK_PLUGIN_NAME "ADM_vidEnc_x264_Gtk")
		
		SET(PLUGIN_SCHEMA_DIR "x264")
		SET(PLUGIN_CONFIG_DIR "x264")
		
		INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${X264_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ../common)
		ADD_DEFINITIONS(${LIBXML2_DEFINITIONS} -DPLUGIN_SCHEMA_DIR="${PLUGIN_SCHEMA_DIR}" -DPLUGIN_CONFIG_DIR="${PLUGIN_CONFIG_DIR}")
		
		SET(ADM_vidEnc_x264_SRCS interface.c configGuiLoader.cpp encoder.cpp guiHelper.cpp zoneOptions.cpp x264Options.cpp)

		CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/config.h")

		ADD_SUBDIRECTORY(gtk)
		ADD_SUBDIRECTORY(qt4)
		ADD_SUBDIRECTORY(xml)
		ADD_LIBRARY(ADM_vidEnc_x264 SHARED ${ADM_vidEnc_x264_SRCS})
		TARGET_LINK_LIBRARIES(ADM_vidEnc_x264 ADM_core ${X264_LIBRARY_DIR} ${LIBXML2_LIBRARIES})

		INIT_VIDEO_ENCODER_PLUGIN(ADM_vidEnc_x264)
		INSTALL_VIDEO_ENCODER(ADM_vidEnc_x264)
		INSTALL(FILES x264Param.xsd DESTINATION "${VIDENC_INSTALL_DIR}${PLUGIN_SCHEMA_DIR}")
	ENDIF (LIBXML2_FOUND)
ENDIF (X264_FOUND)