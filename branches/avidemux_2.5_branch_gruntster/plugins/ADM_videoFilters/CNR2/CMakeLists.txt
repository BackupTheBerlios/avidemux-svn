INCLUDE(vf_plugin)


SET(cnr2Common_SRCS 
ADM_vidCNR2.cpp
)

INCLUDE(vf_plugin_gtk)
INCLUDE(vf_plugin_qt4)

SET(cnr2CLI_SRCS cli/DIA_cnr2.cpp)
SET(cnr2Gtk_SRCS gtk/DIA_cnr2.cpp)
SET(cnr2QT_SRCS  qt4/Q_cnr2.cpp)
SET(cnr2QT_HEADERS  qt4/Q_cnr2.h)
SET(cnr2QT_UI    qt4/cnr2)

INIT_VIDEOFILTER_PLUGIN_GTK(  ADM_vf_cnr2_gtk 
                ${cnr2Gtk_SRCS} ${cnr2Common_SRCS})
INIT_VIDEOFILTER_PLUGIN_QT4(ADM_vf_cnr2_qt4 
                ${cnr2QT_SRCS} ${cnr2QT_HEADERS} ${cnr2QT_UI} ${cnr2Common_SRCS})
#CLI
INCLUDE (vf_plugin_cli)
INIT_VIDEOFILTER_PLUGIN_CLI(ADM_vf_cnr2_cli ${cnr2Common_SRCS} ${cnr2CLI_SRCS})
INSTALL_VIDEOFILTER(ADM_vf_cnr2_cli)
#CLI

