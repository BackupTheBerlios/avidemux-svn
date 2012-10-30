# Install glade file into FOO/lib/ADM_glade/
set(ADM_glade
gtk2_build.gtkBuilder
avidemux_icon_small.png
)

FILE(GLOB ADM_icons ${AVIDEMUX_TOP_SOURCE_DIR}/avidemux/common/ADM_icons/*.png)
ADM_INSTALL_GLADE_LIB_FILES(main ${ADM_glade} ${ADM_icons})
