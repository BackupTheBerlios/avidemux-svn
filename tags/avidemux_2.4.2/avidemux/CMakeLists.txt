##########################################################
#
#  Main CMake makefile for avidemux2
#  It is not for the faint of heart and pretty ugly
#
##########################################################
SET(adm_EXE_SRCS ADM_preview.cpp  gtk_gui.cpp  gui_autodrive.cpp  GUI_jobs.cpp  gui_navigate.cpp  guiplay.cpp  gui_savenew.cpp  main.cpp  gui_action.cpp)

include_directories(ADM_userInterfaces/ADM_commonUI ADM_outputs)
add_definitions(${GLIB_CFLAGS})

########################################################
# Macros
########################################################
MACRO (add_libs_all_targets _libName)
	TARGET_LINK_LIBRARIES(avidemux2_cli ${_libName})
  
	if(GTK_FOUND)
		TARGET_LINK_LIBRARIES(avidemux2_gtk ${_libName})
	endif(GTK_FOUND)
	
	if(QT4_FOUND)
		TARGET_LINK_LIBRARIES(avidemux2_qt4 ${_libName})
	endif(QT4_FOUND)
ENDMACRO (add_libs_all_targets _libName)

MACRO (add_link_flags _target _flg)
	GET_TARGET_PROPERTY(_flags ${_target} LINK_FLAGS)

	if (_flags)
		set(_flags "${_flags} ${_flg}")
	else (_flags)
		set(_flags "${_flg}")
	endif (_flags)

	SET_TARGET_PROPERTIES(${_target} PROPERTIES LINK_FLAGS "${_flags}")
ENDMACRO (add_link_flags)

MACRO (add_ldflags_all_targets _libName)
	add_link_flags(avidemux2_cli ${_libName})

	if(GTK_FOUND)
		add_link_flags(avidemux2_gtk ${_libName})
	endif(GTK_FOUND)

	if(QT4_FOUND)
		add_link_flags(avidemux2_qt4 ${_libName})
	endif(QT4_FOUND)
ENDMACRO (add_ldflags_all_targets _libName)

MACRO (add_compile_flags _target _flg)
	GET_SOURCE_FILE_PROPERTY(_flags ${_target} COMPILE_FLAGS)

	if (_flags)
		set(_flags "${_flags} ${_flg}")
	else (_flags)
		set(_flags "${_flg}")
	endif (_flags)
   
	SET_SOURCE_FILES_PROPERTIES(${_target} PROPERTIES COMPILE_FLAGS "${_flags}")   
ENDMACRO (add_compile_flags)

########################################################
# Our exectuables
########################################################
ADD_EXECUTABLE(avidemux2_cli ${adm_EXE_SRCS})

if(GTK_FOUND)
	ADD_EXECUTABLE(avidemux2_gtk ${adm_EXE_SRCS})
endif(GTK_FOUND)

if(QT4_FOUND)
	ADD_EXECUTABLE(avidemux2_qt4 ${adm_EXE_SRCS})
endif(QT4_FOUND)

########################################################
# Normal subdirs, compile and link what is inside....
########################################################
SET(ADM_SUBDIR 
ADM_filter
ADM_audiofilter 
ADM_editor
ADM_audiocodec 
ADM_audio
ADM_script 
ADM_videoFilter
ADM_filter
ADM_video 
ADM_audiodevice
ADM_ocr  
ADM_colorspace 
)

FOREACH(_current ${ADM_SUBDIR})
	ADD_SUBDIRECTORY(${_current})
	add_libs_all_targets(${_current})
ENDFOREACH(_current ${ADM_SUBDIR})

###########################################
# Build Regular Subdirs
###########################################
ADD_SUBDIRECTORY(ADM_osSupport)
ADD_SUBDIRECTORY(ADM_encoder)
ADD_SUBDIRECTORY(ADM_toolkit)
ADD_SUBDIRECTORY(ADM_codecs)
ADD_SUBDIRECTORY(ADM_requant)
ADD_SUBDIRECTORY(ADM_infoExtractor)

###########################################
# Construct ADM_outputs
###########################################
ADD_SUBDIRECTORY(ADM_outputs)

SET(ADM_outputs
oplug_avi
oplug_mp4
oplug_mpeg
oplug_mpegFF
oplug_ogm
oplug_flv
oplug_dummy
)

FOREACH(_current ${ADM_outputs})
	add_libs_all_targets(${_current})
ENDFOREACH(_current ${ADM_outputs})

###########################################
# Construct ADM_inputs
###########################################
ADD_SUBDIRECTORY(ADM_inputs)

SET(ADM_inputs
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
ADM_amv
)

FOREACH(_current ${ADM_inputs})
	add_libs_all_targets(${_current})
ENDFOREACH(_current ${ADM_inputs})

#########################################
# Tails
#########################################
add_libs_all_targets(ADM_toolkit)
add_libs_all_targets(ADM_osSupport)
add_libs_all_targets(ADM_encoder)
add_libs_all_targets(ADM_codecs)
add_libs_all_targets(ADM_requant)
add_libs_all_targets(ADM_infoExtractor)

###########################################
# Construct ADM_libraries
###########################################
ADD_SUBDIRECTORY(ADM_libraries)

SET(ADM_LIBRARIES
ADM_lavformat
ADM_lavcodec
ADM_lavutil
ADM_liba52
ADM_libMad
ADM_mplex
ADM_libMpeg2Dec
ADM_libmpeg2enc
ADM_libswscale
ADM_libtwolame
ADM_libwrapper
ADM_lvemux
ADM_smjs
ADM_utilities
ADM_xvidratectl
ADM_libpostproc
ADM_colorspace
)

if (USE_FREETYPE)
	SET(ADM_LIBRARIES ${ADM_LIBRARIES} ADM_libass)
endif (USE_FREETYPE)

if (ADM_DEBUG AND FIND_LEAKS)
	SET(ADM_LIBRARIES ${ADM_LIBRARIES} ADM_nvwa)
endif (ADM_DEBUG AND FIND_LEAKS)

if (ADM_OS_APPLE AND USE_SDL)
	SET(ADM_LIBRARIES ${ADM_LIBRARIES} ADM_libsdl)
endif (ADM_OS_APPLE AND USE_SDL)

FOREACH(_current ${ADM_LIBRARIES})
	add_libs_all_targets(${_current})
ENDFOREACH(_current ${ADM_SUBDIR})

###########################################
# Construct ADM_userInterface
###########################################
ADD_SUBDIRECTORY(ADM_userInterfaces)

TARGET_LINK_LIBRARIES(avidemux2_cli ADM_UI_None)
TARGET_LINK_LIBRARIES(avidemux2_cli ADM_commonUI)
TARGET_LINK_LIBRARIES(avidemux2_cli ADM_ocr)
TARGET_LINK_LIBRARIES(avidemux2_cli ADM_dialogNone)
TARGET_LINK_LIBRARIES(avidemux2_cli ADM_dialogFactoryNone)
TARGET_LINK_LIBRARIES(avidemux2_cli ADM_dialogFiltersNone)
TARGET_LINK_LIBRARIES(avidemux2_cli ADM_dialogGuiNone)

if(GTK_FOUND)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_UI_GTK)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_commonUI)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_ocr)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_ocrGtk)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_dialogGtk)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_filtersGtk)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_dialogFactoryGtk)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_gui2Gtk)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_toolkitGtk)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ADM_commonUI)
endif(GTK_FOUND)

if(QT4_FOUND)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ADM_UI_QT4)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ADM_commonUI)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ADM_ocr)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ADM_ocrQt4)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ADM_dialogQt4)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ADM_dialogFactoryQt4)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ADM_guiQt4)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ADM_filtersQt4)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ADM_commonUI)
endif(QT4_FOUND)

add_libs_all_targets(ADM_utilities)

###########################################
# Add external libs
###########################################
#iconv
if(NEED_LIBICONV)
	add_libs_all_targets(iconv)
endif(NEED_LIBICONV)

# Pulse audio
if(USE_PULSE_SIMPLE)
	add_libs_all_targets(pulse-simple)
endif(USE_PULSE_SIMPLE)

#gettext
if(ADM_GETTEXT_LIB)
	INCLUDE_DIRECTORIES(${ADM_GETTEXT_INCLUDE})
	TARGET_LINK_LIBRARIES(avidemux2_cli ${ADM_GETTEXT_LIB})

	if(GTK_FOUND)
		TARGET_LINK_LIBRARIES(avidemux2_gtk ${ADM_GETTEXT_LIB})
	endif(GTK_FOUND)
endif(ADM_GETTEXT_LIB)

#freetype
if(USE_FREETYPE)
	add_libs_all_targets(${FT_LDFLAGS})
endif(USE_FREETYPE)

#fontconfig
if(USE_FONTCONFIG)
	add_libs_all_targets(fontconfig)
endif(USE_FONTCONFIG)

# png
if(USE_PNG)
	TARGET_LINK_LIBRARIES(avidemux2_cli png)
	
	if(GTK_FOUND)
		TARGET_LINK_LIBRARIES(avidemux2_gtk png)
	endif(GTK_FOUND)
	
	if (QT4_FOUND)
		if (ADM_OS_APPLE AND NOT QT_USE_FRAMEWORKS)
			TARGET_LINK_LIBRARIES(avidemux2_qt4 png)
		elseif (NOT ADM_OS_APPLE)
			TARGET_LINK_LIBRARIES(avidemux2_qt4 png)
		endif (ADM_OS_APPLE AND NOT QT_USE_FRAMEWORKS)
	endif (QT4_FOUND)
endif(USE_PNG)

# Xml2
add_libs_all_targets(xml2)

#XVideo 
if(USE_XV)
	add_libs_all_targets("${X11_LIBRARIES}")
	add_libs_all_targets(Xv)
endif(USE_XV)

# SDL
if(USE_SDL)
	add_libs_all_targets("${SDL_LIBRARY}")
endif(USE_SDL)

# ARTS
if(USE_ARTS)
	add_libs_all_targets("${ARTS_LDFLAGS}")
endif(USE_ARTS)

#alsa
if(ALSA_FOUND)
	add_libs_all_targets("${ASOUND_LIBRARY}")
endif(ALSA_FOUND)

# ESD
if(USE_ESD)
	add_libs_all_targets(esd)
endif(USE_ESD)

# JACK
if(USE_JACK)
	add_libs_all_targets(jack)
endif(USE_JACK)

# libAMRNB
if(USE_AMR_NB AND NOT USE_LATE_BINDING)
	add_libs_all_targets(amrnb)
endif(USE_AMR_NB AND NOT USE_LATE_BINDING)

#libdts
if(USE_LIBDCA AND NOT USE_LATE_BINDING)
	add_libs_all_targets(dts)
endif(USE_LIBDCA AND NOT USE_LATE_BINDING)

# Lame
if(HAVE_LIBMP3LAME)
	add_libs_all_targets(mp3lame)
endif(HAVE_LIBMP3LAME)

# Vorbis
if(USE_VORBIS)
	add_libs_all_targets(vorbis)
	add_libs_all_targets(vorbisenc)
endif(USE_VORBIS)

#faac
if(USE_FAAC)
	add_libs_all_targets(faac)
endif(USE_FAAC)

#aften
if(USE_AFTEN)
	add_libs_all_targets(aften)
endif(USE_AFTEN)

#faad
if(USE_FAAD)
	add_libs_all_targets(faad)
endif(USE_FAAD)

#libsamplerate
if(USE_SRC)
	add_libs_all_targets(samplerate)
endif(USE_SRC)

# xvid
if(USE_XVID_4)
	add_libs_all_targets(xvidcore)
endif(USE_XVID_4)

# x264
if(USE_X264)
	add_libs_all_targets(x264)
endif(USE_X264)

###########################################
# Specific
###########################################
TARGET_LINK_LIBRARIES(avidemux2_cli ${GLIB_LDFLAGS})
LINK_DIRECTORIES(avidemux2_cli ${GLIB_LIB})

if(GTK_FOUND)
	TARGET_LINK_LIBRARIES(avidemux2_gtk ${GLIB_LDFLAGS})
	LINK_DIRECTORIES(avidemux2_gtk ${GLIB_LIB})
	TARGET_LINK_LIBRARIES(avidemux2_gtk ${GTK_LDFLAGS})
	TARGET_LINK_LIBRARIES(avidemux2_gtk ${GTHREAD_LDFLAGS}) # FIXME
endif(GTK_FOUND)

if(QT4_FOUND)
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ${GLIB_LDFLAGS})
	LINK_DIRECTORIES(avidemux2_qt4 ${GLIB_LIB})
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ${QT_QTGUI_LIBRARY}) # FIXME
	TARGET_LINK_LIBRARIES(avidemux2_qt4 ${QT_QTCORE_LIBRARY}) # FIXME
endif(QT4_FOUND)

########################
# Win32
########################
IF (ADM_OS_WINDOWS)
	add_libs_all_targets(z)
	add_libs_all_targets(ADM_osSupport)
	add_libs_all_targets(pthreadGC2)
	add_libs_all_targets(winmm)
	add_libs_all_targets(wsock32)
	add_libs_all_targets(imagehlp)

	add_libs_all_targets("-mms-bitfield -mno-cygwin ${CMAKE_SOURCE_DIR}/avidemux/xpm/adm.o -Wl,--export-all-symbols")

	TARGET_LINK_LIBRARIES(avidemux2_cli "-Wl,-subsystem,console")
	
	if (GTK_FOUND)
		TARGET_LINK_LIBRARIES(avidemux2_gtk "-mwindows")
	endif(GTK_FOUND)
	
	if (QT4_FOUND)
		TARGET_LINK_LIBRARIES(avidemux2_qt4 "-mwindows")
	endif(QT4_FOUND)

	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		add_ldflags_all_targets("-Wl,-s")
	endif(CMAKE_BUILD_TYPE STREQUAL "Release")
ENDIF (ADM_OS_WINDOWS)

########################
# MacOsX
########################
if (ADM_OS_APPLE)
	add_libs_all_targets(z)
	add_libs_all_targets("-framework CoreServices -framework CoreAudio -framework AudioUnit -framework Carbon -framework Cocoa")

	# for Leopard but it doesn't hurt Tiger
	add_libs_all_targets("-Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib")
endif (ADM_OS_APPLE)

########################
# all
########################
SDLify(main.cpp)
add_compile_flags(main.cpp "-DADM_SUBVERSION=${ADM_SUBVERSION}")

###############
# INSTALL
###############
SET(ADM_EXES avidemux2_cli)

if(GTK_FOUND)
	SET(ADM_EXES ${ADM_EXES} avidemux2_gtk)
endif(GTK_FOUND)

if(QT4_FOUND)
	SET(ADM_EXES ${ADM_EXES} avidemux2_qt4)
endif(QT4_FOUND)

INSTALL(TARGETS ${ADM_EXES} RUNTIME DESTINATION bin LIBRARY DESTINATION lib)

# EOF
