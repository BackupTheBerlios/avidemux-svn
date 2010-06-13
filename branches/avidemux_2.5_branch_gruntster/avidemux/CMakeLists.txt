########################################
# Macros
########################################
MACRO (ADD_ADMLIB_ALL_TARGETS)
	FOREACH (_libName ${ARGV})
		TARGET_LINK_LIBRARIES(avidemux2_cli ${_libName}_cli)

		IF (ADM_UI_GTK)
			TARGET_LINK_LIBRARIES(avidemux2_gtk ${_libName}_gtk)
		ENDIF (ADM_UI_GTK)

		IF (ADM_UI_QT4)
			TARGET_LINK_LIBRARIES(avidemux2_qt4 ${_libName}_qt4)
		ENDIF (ADM_UI_QT4)
	ENDFOREACH (_libName ${ARGV})
ENDMACRO (ADD_ADMLIB_ALL_TARGETS)


MACRO (ADD_LIB_ALL_TARGETS)
	TARGET_LINK_LIBRARIES(avidemux2_cli ${ARGV})

	IF (ADM_UI_GTK)
		TARGET_LINK_LIBRARIES(avidemux2_gtk ${ARGV})
	ENDIF (ADM_UI_GTK)

	IF (ADM_UI_QT4)
		TARGET_LINK_LIBRARIES(avidemux2_qt4 ${ARGV})
	ENDIF (ADM_UI_QT4)
ENDMACRO (ADD_LIB_ALL_TARGETS)


MACRO (ADD_LDFLAGS_ALL_TARGETS)
	ADD_TARGET_LDFLAGS(avidemux2_cli ${ARGV})

	IF (ADM_UI_GTK)
		ADD_TARGET_LDFLAGS(avidemux2_gtk ${ARGV})
	ENDIF (ADM_UI_GTK)

	IF (ADM_UI_QT4)
		ADD_TARGET_LDFLAGS(avidemux2_qt4 ${ARGV})
	ENDIF (ADM_UI_QT4)
ENDMACRO (ADD_LDFLAGS_ALL_TARGETS)

########################################
# Functions
########################################
function(MakeOSXBundleApp target binary_dir)
	SET (OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT "${binary_dir}/${target}_OSX_MakeStandAloneBundle.cmake")
	SET (OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT "${binary_dir}/${target}_OSX_MakeStandAloneBundle.sh")

	#-- Configure the cmake file and the shell script
	CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Apple/CompleteBundle.cmake.in"
		"${OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT}" @ONLY IMMEDIATE)
	CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Apple/CreateBundle.sh.in"
		"${OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT}" @ONLY IMMEDIATE)

	#-- Create the installation code
	install(SCRIPT "${OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT}")
endfunction(MakeOSXBundleApp)

########################################
# Definitions and Includes
########################################
ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64 -D_LARGE_FILES)
ADD_DEFINITIONS(-DHAVE_CONFIG_H)

IF (ADM_DEBUG)
	ADD_DEFINITIONS(-DADM_DEBUG)
ENDIF (ADM_DEBUG)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_BINARY_DIR}/config")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/ADM_core/include")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/ADM_coreUI/include")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/ADM_coreImage/include")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/ADM_coreAudio/include")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/ADM_libraries/ADM_utilities")

IF (GETTEXT_FOUND)
	INCLUDE_DIRECTORIES(${GETTEXT_INCLUDE_DIR})
ENDIF (GETTEXT_FOUND)

########################################
# Embed resources for executables
########################################
IF (APPLE)
	# add icon
	SET(ADM_EXE_SRCS ${ADM_EXE_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/ADM_icons/xpm/avidemux.icns)
	SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/ADM_icons/xpm/avidemux.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

	# bundle version info
	SET(ADM_OSX_VERSION "${VERSION}")

	IF (Subversion_FOUND)
		SET(ADM_OSX_VERSION "${ADM_OSX_VERSION} (r${ADM_SUBVERSION})")
	ENDIF (Subversion_FOUND)

	SET(MACOSX_BUNDLE_ICON_FILE avidemux.icns)
	SET(MACOSX_BUNDLE_INFO_STRING "Avidemux ${ADM_OSX_VERSION}")
	SET(MACOSX_BUNDLE_LONG_VERSION_STRING "${ADM_OSX_VERSION}") 
	SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${ADM_OSX_VERSION}")
	SET(MACOSX_BUNDLE_VERSION "${ADM_OSX_VERSION}")
	SET(MACOSX_BUNDLE_BUNDLE_NAME "Avidemux" )
	SET(MACOSX_BUNDLE_COPYRIGHT "Copyright 2001-2010 Mean" )
	SET(MACOSX_BUNDLE_SIGNATURE "Avdx" )
	SET(MACOSX_BUNDLE_GUI_IDENTIFIER www.avidemux.org)

	# include translation files in bundle
	# this isn't the cleanest approach but there's not much we can do until the po directory is under the avidemux directory
	FILE(GLOB PO_FILES ${CMAKE_SOURCE_DIR}/po/*.po)

	FOREACH(_file ${PO_FILES})
		GET_FILENAME_COMPONENT(_filename ${_file} NAME_WE)
		SET(_mofile "${CMAKE_BINARY_DIR}/po/${_filename}/avidemux.mo")

		SET(MO_FILES ${MO_FILES} ${_mofile})
		SET_SOURCE_FILES_PROPERTIES(${_mofile} PROPERTIES GENERATED TRUE MACOSX_PACKAGE_LOCATION "Resources/locale/${_filename}/LC_MESSAGES")
	ENDFOREACH(_file)

	FILE(GLOB TS_FILES ${CMAKE_SOURCE_DIR}/po/*.ts)

	FOREACH(_file ${TS_FILES})
		GET_FILENAME_COMPONENT(_filename ${_file} NAME_WE)
		SET(_qmfile "${CMAKE_BINARY_DIR}/po/${_filename}.qm")

		SET(QM_FILES ${QM_FILES} ${_qmfile})
		SET_SOURCE_FILES_PROPERTIES(${_qmfile} PROPERTIES GENERATED TRUE MACOSX_PACKAGE_LOCATION "Resources/locale")
	ENDFOREACH(_file)
ENDIF (APPLE)

IF (WIN32)
	# add icon and version info
	SET(FILEVERSION_STRING "${VERSION}.${ADM_SUBVERSION}")
	SET(PRODUCTVERSION_STRING "${VERSION}")
	STRING(REPLACE "." "," FILEVERSION ${FILEVERSION_STRING})
	STRING(REPLACE "." "," PRODUCTVERSION ${PRODUCTVERSION_STRING})
	SET(PRODUCTVERSION "${PRODUCTVERSION},0")

	SET(AVIDEMUX_ICON "adm.ico")
	SET(WIN_RES_GTK "admWinGtk.obj")
	CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake/admWin32.rc.in" ${CMAKE_CURRENT_BINARY_DIR}/admWinGtk.rc IMMEDIATE)
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${WIN_RES_GTK} COMMAND windres.exe -i ${CMAKE_CURRENT_BINARY_DIR}/admWinGtk.rc -o ${CMAKE_CURRENT_BINARY_DIR}/${WIN_RES_GTK} -O coff --define VS_VERSION_INFO=1)

	SET(AVIDEMUX_ICON "avidemux.ico")
	SET(WIN_RES_QT "admWinQt.obj")
	CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/cmake/admWin32.rc.in" ${CMAKE_CURRENT_BINARY_DIR}/admWinQt.rc IMMEDIATE)
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${WIN_RES_QT} COMMAND windres.exe -i ${CMAKE_CURRENT_BINARY_DIR}/admWinQt.rc -o ${CMAKE_CURRENT_BINARY_DIR}/${WIN_RES_QT} -O coff --define VS_VERSION_INFO=1)
ENDIF (WIN32)

########################################
# Our exectuables
########################################
SET(ADM_EXE_SRCS ${ADM_EXE_SRCS} ADM_preview.cpp  gtk_gui.cpp  GUI_jobs.cpp  gui_navigate.cpp  guiplay.cpp  gui_savenew.cpp  main.cpp  gui_action.cpp)

ADD_EXECUTABLE(avidemux2_cli MACOSX_BUNDLE ${ADM_EXE_SRCS} ${MO_FILES} ${WIN_RES_GTK})
ADD_TARGET_CFLAGS(avidemux2_cli "-I${CMAKE_BINARY_DIR}/config/cli")
ADD_DEPENDENCIES(avidemux2_cli mofiles  ffmpeg)

IF (ADM_UI_GTK)
	ADD_EXECUTABLE(avidemux2_gtk MACOSX_BUNDLE ${ADM_EXE_SRCS} ${MO_FILES} ${WIN_RES_GTK})
	ADD_TARGET_CFLAGS(avidemux2_gtk "-I${CMAKE_BINARY_DIR}/config/gtk")
	ADD_DEPENDENCIES(avidemux2_gtk mofiles  ffmpeg)
ENDIF (ADM_UI_GTK)

IF (ADM_UI_QT4)
	ADD_EXECUTABLE(avidemux2_qt4 MACOSX_BUNDLE ${ADM_EXE_SRCS} ${QM_FILES} ${WIN_RES_QT})
	
	IF (WIN32 OR APPLE)
		set_property(TARGET avidemux2_qt4 PROPERTY OUTPUT_NAME avidemux2)
	ENDIF (WIN32 OR APPLE)
	
	ADD_TARGET_CFLAGS(avidemux2_qt4 "-I${CMAKE_BINARY_DIR}/config/qt4")
	ADD_DEPENDENCIES(avidemux2_qt4 qmfiles  ffmpeg)
ENDIF (ADM_UI_QT4)

SDLify(main.cpp)
ADD_SOURCE_CFLAGS(main.cpp "-DADM_SUBVERSION=${ADM_SUBVERSION} -I${PTHREAD_INCLUDE_DIR}")
ADD_SOURCE_CFLAGS(gui_savenew.cpp "-I${PTHREAD_INCLUDE_DIR}")

########################################
# FFmpeg
########################################
include(admFFmpegBuild)

########################################
# Compile and link general subdirs
########################################
SET(ADM_SUBDIR 
	ADM_audiofilter
	#ADM_audioEncoder
	ADM_editor
	ADM_audiocodec
	ADM_audio
	ADM_script
	ADM_filter
	ADM_videoFilter
	ADM_video
	ADM_audiodevice
	ADM_ocr
)
FOREACH(_current ${ADM_SUBDIR})
	ADD_SUBDIRECTORY(${_current})
	ADD_ADMLIB_ALL_TARGETS(${_current})
ENDFOREACH(_current ${ADM_SUBDIR})

###########################################
# Construct ADM_outputs
###########################################
ADD_SUBDIRECTORY(ADM_outputs)

SET(ADM_OUTPUTS	
	oplug_avi
	oplug_mp4
	oplug_mpeg
	oplug_mpegFF
	oplug_ogm
	oplug_flv
	oplug_dummy
	ADM_outputs)

ADD_ADMLIB_ALL_TARGETS(${ADM_OUTPUTS})

###########################################
# Construct ADM_inputs
###########################################
ADD_SUBDIRECTORY(ADM_inputs)
ADD_SUBDIRECTORY(ADM_UIs)

SET(ADM_INPUTS
	ADM_asf
	ADM_avsproxy
	ADM_h263
	ADM_inpics
	ADM_matroska
	ADM_mp4
	ADM_mpegdemuxer
	ADM_nuv
	ADM_ogm
	ADM_openDML
	ADM_flv
	ADM_amv)

ADD_ADMLIB_ALL_TARGETS(${ADM_INPUTS})

#########################################
# Tails
#########################################
SET(ADM_TAILS
	ADM_osSupport
	ADM_encoder
	ADM_toolkit
	ADM_codecs
	ADM_requant
	ADM_infoExtractor)

FOREACH(_current ${ADM_TAILS})
	ADD_SUBDIRECTORY(${_current})
	ADD_ADMLIB_ALL_TARGETS(${_current})
ENDFOREACH(_current ${ADM_TAILS})
###########################################
# Construct ADM_libraries
###########################################
ADD_SUBDIRECTORY(ADM_libraries)

ADD_ADMLIB_ALL_TARGETS(
	ADM_mplex
	ADM_lvemux
	ADM_utilities
)

ADD_LIB_ALL_TARGETS(
	ADM_libavutil
	ADM_libpostproc
	ADM_libswscale
	ADM_libavcodec
	ADM_libavformat)

if (USE_SYSTEM_SPIDERMONKEY)
	ADD_LIB_ALL_TARGETS(${SPIDERMONKEY_LIBRARY_DIR})
else (USE_SYSTEM_SPIDERMONKEY)
	ADD_LIB_ALL_TARGETS(ADM_smjs)
endif (USE_SYSTEM_SPIDERMONKEY)

IF (ADM_DEBUG AND FIND_LEAKS)
	ADD_ADMLIB_ALL_TARGETS(ADM_nvwa)
ENDIF (ADM_DEBUG AND FIND_LEAKS)

IF (APPLE AND USE_SDL)
	IF (ADM_UI_GTK)
		TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_libsdl_gtk)
	ENDIF (ADM_UI_GTK)

	IF (ADM_UI_QT4)
		TARGET_LINK_LIBRARIES(avidemux2_qt4 ADM_libsdl_qt4)
	ENDIF (ADM_UI_QT4)
ENDIF (APPLE AND USE_SDL)

###########################################
# Construct ADM_userInterface
###########################################
ADD_SUBDIRECTORY(ADM_userInterfaces)

TARGET_LINK_LIBRARIES(avidemux2_cli
	ADM_UI_None_cli
	ADM_commonUI_cli
	ADM_ocr_cli
	ADM_dialogNone_cli
	ADM_dialogFiltersNone_cli
	ADM_dialogGuiNone_cli
	ADM_UICli
	ADM_render_cli
)

IF (ADM_UI_GTK)
	TARGET_LINK_LIBRARIES(avidemux2_gtk
		ADM_UI_GTK_gtk
		ADM_commonUI_gtk
		ADM_ocr_gtk
		ADM_ocrGtk_gtk
		ADM_dialogGtk_gtk
		ADM_filtersGtk_gtk
		ADM_gui2Gtk_gtk
		ADM_commonUI_gtk
		ADM_toolkitGtk_gtk
		ADM_UIGtk
		ADM_render_gtk
	)
ENDIF (ADM_UI_GTK)

IF (ADM_UI_QT4)
	TARGET_LINK_LIBRARIES(avidemux2_qt4
		ADM_UI_QT4_qt4
		ADM_commonUI_qt4
		ADM_ocr_qt4
		ADM_ocrQt4_qt4
		ADM_dialogQt4_qt4
		ADM_guiQt4_qt4
		ADM_filtersQt4_qt4
		ADM_commonUI_qt4
		ADM_dialogQt4_qt4
		ADM_UIQT4
		ADM_render_qt4
	)
ENDIF (ADM_UI_QT4)

ADD_ADMLIB_ALL_TARGETS(ADM_utilities)

###########################################
# ADM_core/ADM_coreUI
###########################################
ADD_SUBDIRECTORY(ADM_core)
ADD_LIB_ALL_TARGETS(ADM_core)
ADD_SUBDIRECTORY(ADM_coreUI)
ADD_LIB_ALL_TARGETS(ADM_coreUI)
ADD_SUBDIRECTORY(ADM_coreImage)
ADD_LIB_ALL_TARGETS(ADM_coreImage)
ADD_SUBDIRECTORY(ADM_coreAudio)
ADD_LIB_ALL_TARGETS(ADM_coreAudio)

# To be removed
#ADD_SUBDIRECTORY(ADM_audioEncoder)

###########################################
# External libs
###########################################
# gettext
IF (GETTEXT_FOUND)
	TARGET_LINK_LIBRARIES(avidemux2_cli ${GETTEXT_LIBRARY_DIR})

	IF (ADM_UI_GTK)
		TARGET_LINK_LIBRARIES(avidemux2_gtk ${GETTEXT_LIBRARY_DIR})
	ENDIF (ADM_UI_GTK)
ENDIF (GETTEXT_FOUND)

# pthreads
IF (UNIX AND PTHREAD_FOUND)
	ADD_LIB_ALL_TARGETS("${PTHREAD_LIBRARIES}")
ENDIF (UNIX AND PTHREAD_FOUND)

# Libxml2
ADD_LIB_ALL_TARGETS("${LIBXML2_LIBRARIES}")

# XVideo
IF (USE_XV)
	IF (ADM_UI_GTK)
		TARGET_LINK_LIBRARIES(avidemux2_gtk ${XVIDEO_LIBRARY_DIR})
	ENDIF (ADM_UI_GTK)

	IF (ADM_UI_QT4)
		TARGET_LINK_LIBRARIES(avidemux2_qt4 ${XVIDEO_LIBRARY_DIR})
	ENDIF (ADM_UI_QT4)
ENDIF (USE_XV)

# VPX
IF (USE_VPX)
	IF (ADM_UI_GTK)
		TARGET_LINK_LIBRARIES(avidemux2_gtk ${VPX_LIBRARY_DIR})
	ENDIF (ADM_UI_GTK)

	IF (ADM_UI_QT4)
		TARGET_LINK_LIBRARIES(avidemux2_qt4 ${VPX_LIBRARY_DIR})
	ENDIF (ADM_UI_QT4)
	TARGET_LINK_LIBRARIES(avidemux2_cli ${VPX_LIBRARY_DIR})
ENDIF (USE_VPX)

# SDL
IF (USE_SDL)
	IF (ADM_UI_GTK)
		TARGET_LINK_LIBRARIES(avidemux2_gtk ${SDL_LIBRARY})
	ENDIF (ADM_UI_GTK)

	IF (ADM_UI_QT4)
		TARGET_LINK_LIBRARIES(avidemux2_qt4 ${SDL_LIBRARY})
	ENDIF (ADM_UI_QT4)
ENDIF (USE_SDL)

###########################################
# UI Specific
###########################################
IF (ADM_UI_GTK)
	ADD_TARGET_LDFLAGS(avidemux2_gtk ${GTK_LDFLAGS} ${GTHREAD_LDFLAGS})
ENDIF (ADM_UI_GTK)

IF (ADM_UI_QT4)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY})
ENDIF (ADM_UI_QT4)

###########################################
# OS Specific
###########################################
IF (WIN32)
	ADD_ADMLIB_ALL_TARGETS(ADM_osSupport)

	IF (MINGW)
		ADD_LIB_ALL_TARGETS(-mno-cygwin)
	ENDIF (MINGW)

	ADD_LIB_ALL_TARGETS(winmm)

	ADD_TARGET_LDFLAGS(avidemux2_cli "-Wl,-subsystem,console")

	IF (ADM_UI_GTK)
		ADD_TARGET_LDFLAGS(avidemux2_gtk "-mwindows")
	ENDIF (ADM_UI_GTK)

	IF (ADM_UI_QT4)
		ADD_TARGET_LDFLAGS(avidemux2_qt4 "-mwindows")
	ENDIF (ADM_UI_QT4)
ENDIF (WIN32)

IF (APPLE)
	ADD_LIB_ALL_TARGETS("-framework CoreServices -framework CoreAudio -framework AudioUnit -framework Carbon")

	# for Leopard but it doesn't hurt Tiger
	ADD_LIB_ALL_TARGETS("-Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib")
ENDIF (APPLE)

###########################################
# Install
###########################################
SET(ADM_EXES avidemux2_cli)


IF (ADM_UI_QT4)
	SET(ADM_EXES ${ADM_EXES} avidemux2_qt4)

	IF (APPLE AND MAC_SELF_CONTAINED_BUNDLE)
		MakeOSXBundleApp(avidemux2_qt4 ${PROJECT_BINARY_DIR})
	ENDIF (APPLE AND MAC_SELF_CONTAINED_BUNDLE)
ENDIF (ADM_UI_QT4)

IF (ADM_UI_GTK)
	SET(ADM_EXES ${ADM_EXES} avidemux2_gtk)

	IF (APPLE AND MAC_SELF_CONTAINED_BUNDLE)
		MakeOSXBundleApp(avidemux2_gtk ${PROJECT_BINARY_DIR})
	ENDIF (APPLE AND MAC_SELF_CONTAINED_BUNDLE)
ENDIF (ADM_UI_GTK)

IF (APPLE)
	set_target_properties(${ADM_EXES} PROPERTIES INSTALL_NAME_DIR ${CMAKE_INSTALL_NAME_DIR})

	IF (MAC_BUNDLE_DIR)
		INSTALL(TARGETS ${ADM_EXES} RUNTIME DESTINATION ${BIN_DIR} BUNDLE DESTINATION ${MAC_BUNDLE_DIR} LIBRARY DESTINATION lib)
	ELSE (MAC_BUNDLE_DIR)
		INSTALL(TARGETS ${ADM_EXES} RUNTIME DESTINATION ${BIN_DIR} BUNDLE DESTINATION "/Applications" LIBRARY DESTINATION lib)
		SET (MAC_BUNDLE_DIR "/Applications")
	ENDIF (MAC_BUNDLE_DIR)
ELSE (APPLE)
	INSTALL(TARGETS ${ADM_EXES} RUNTIME DESTINATION ${BIN_DIR} LIBRARY DESTINATION lib${LIB_SUFFIX})
ENDIF (APPLE)

MACRO (ADM_SYMLINK_LIBS)
       FOREACH (t ${ARGV})
             ADD_CUSTOM_COMMAND(TARGET ${t}
                      POST_BUILD
                      COMMAND mkdir -p ${CMAKE_BINARY_DIR}/lib${LIB_SUFFIX}
                      COMMAND find ${CMAKE_BINARY_DIR}/avidemux -name '*.so*' | xargs ln -sft ${CMAKE_BINARY_DIR}/lib${LIB_SUFFIX}
                      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                      COMMENT "Creating temporary lib directory for proper plugins linking")
       ENDFOREACH (t )
ENDMACRO (ADM_SYMLINK_LIBS)
IF (UNIX AND NOT APPLE)
   # Some plugins link to ADM_coreImage (e.g., ADM_vf_addborders) and thus need the symlink in ${CMAKE_BINARY_DIR}/lib${LIB_SUFFIX}
   ADM_SYMLINK_LIBS(ffmpeg ADM_coreImage)
ENDIF(UNIX AND NOT APPLE)
