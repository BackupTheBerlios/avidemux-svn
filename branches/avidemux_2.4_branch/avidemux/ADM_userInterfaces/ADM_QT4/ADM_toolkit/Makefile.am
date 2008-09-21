
noinst_LIBRARIES = libADM_toolitQt4.a 
libADM_toolitQt4_a_SOURCES = qtToolkit.cpp

INCLUDES = $(all_includes)  $(XML_CFLAGS)  $(FREETYPE_CFLAGS) -DADM_SUBVERSION=@ADM_SUBVERSION@ \
		-I../../../ADM_libraries/ADM_utilities \
		-I../../ADM_commonUI -I../../.. -I../../../ADM_libraries/ADM_lavutil \
		-I../../../ADM_inputs -I../../../ADM_outputs @ADM_QTINCLUDES@

libADM_toolitQt4_a_METASOURCES = AUTO


include ../Makefile.adm
filter_rsc.cpp : filter.qrc
	$(RCC)  $< -o $@   -name filter


EXTRA_DIST =  

