
noinst_LIBRARIES = libADM_dialogFactory.a 

INCLUDES = $(all_includes) $(GTK_CFLAGS) $(XML_CFLAGS)  $(FREETYPE_CFLAGS) -DADM_SUBVERSION=@ADM_SUBVERSION@ \
		-I../../../ADM_libraries/ADM_utilities \
		-I../../ADM_commonUI -I../../.. -I../../../ADM_libraries/ADM_lavutil \
		-I../../../ADM_inputs -I../../../ADM_outputs

libADM_dialogFactory_a_METASOURCES = AUTO

libADM_dialogFactory_a_SOURCES = DIA_dialogFactory.cpp \
FAC_toggle.cpp \
FAC_integer.cpp \
FAC_float.cpp \
FAC_menu.cpp \
DIA_filesel.cpp \
FAC_bitrate.cpp \
FAC_bar.cpp \
FAC_readOnlyText.cpp \
FAC_notch.cpp \
FAC_frame.cpp \
FAC_hex.cpp \
FAC_button.cpp \
DIA_color.cpp \
FAC_matrix.cpp \
FAC_threadCount.cpp


