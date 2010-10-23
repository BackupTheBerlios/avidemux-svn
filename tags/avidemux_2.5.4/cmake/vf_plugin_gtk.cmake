MACRO(INIT_VIDEOFILTER_PLUGIN_GTK  lib  _srcsGtk )
INCLUDE(admCheckGtk)
INCLUDE(admCheckGettext)


checkGtk()

IF (GTK_FOUND AND GTHREAD_FOUND)
	checkGettext()

	IF (GETTEXT_FOUND)
		ADD_DEFINITIONS("-DHAVE_GETTEXT")
		INCLUDE_DIRECTORIES(${GETTEXT_INCLUDE_DIR})
	ENDIF (GETTEXT_FOUND)

        ADD_DEFINITIONS(${GTK_CFLAGS})

        ADD_LIBRARY(${lib} SHARED ${ARGN} ${_srcsGtk})
        INCLUDE_DIRECTORIES(${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_UIs/ADM_GTK/include/)
        TARGET_LINK_LIBRARIES( ${lib} ADM_UIGtk ADM_render_gtk)
        TARGET_LINK_LIBRARIES(${lib} ${GTK_LDFLAGS} ${GTHREAD_LDFLAGS})

	IF (GETTEXT_FOUND)
		TARGET_LINK_LIBRARIES(${lib} ${GETTEXT_LIBRARY_DIR})
	ENDIF(GETTEXT_FOUND)
		ADD_TARGET_CFLAGS(${lib} "-DADM_UI_TYPE_BUILD=2")
        INIT_VIDEOFILTER_PLUGIN(${lib})
        INSTALL_VIDEOFILTER(${lib})
ENDIF (GTK_FOUND AND GTHREAD_FOUND)
ENDMACRO(INIT_VIDEOFILTER_PLUGIN_GTK)
