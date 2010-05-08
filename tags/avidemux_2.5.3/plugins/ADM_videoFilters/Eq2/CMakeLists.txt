INCLUDE(vf_plugin)


SET(eq2Common_SRCS 
ADM_vidEq2.cpp  DIA_flyEq2.cpp
)

INCLUDE(vf_plugin_gtk)
INCLUDE(vf_plugin_qt4)

SET(eq2CLI_SRCS cli/DIA_eq2.cpp)
SET(eq2Gtk_SRCS gtk/DIA_eq2.cpp)
SET(eq2QT_SRCS  qt4/Q_eq2.cpp)
SET(eq2QT_HEADERS  qt4/Q_eq2.h)
SET(eq2QT_UI    qt4/eq2)

INIT_VIDEOFILTER_PLUGIN_QT4(ADM_vf_eq2_qt4 
                ${eq2QT_SRCS} ${eq2QT_HEADERS} ${eq2QT_UI} ${eq2Common_SRCS})
INIT_VIDEOFILTER_PLUGIN_GTK(  ADM_vf_eq2_gtk 
                ${eq2Gtk_SRCS} ${eq2Common_SRCS})
#CLI
INCLUDE (vf_plugin_cli)
INIT_VIDEOFILTER_PLUGIN_CLI(ADM_vf_eq2_cli ${eq2Common_SRCS} ${eq2CLI_SRCS})
INSTALL_VIDEOFILTER(ADM_vf_eq2_cli)
#CLI

