ADD_SUBDIRECTORY(main)
# Install glade file into FOO/lib/ADM_glade/
set(ADM_glade
about.gtkBuilder
calculator.gtkBuilder
DIA_alternate.gtkBuilder
encoding.gtkBuilder
properties.gtkBuilder
${AVIDEMUX_TOP_SOURCE_DIR}/avidemux_icon.png
)

ADM_INSTALL_GLADE_LIB_FILES("" ${ADM_glade})
