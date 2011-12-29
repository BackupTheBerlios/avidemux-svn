INCLUDE(vf_plugin)


SET(mpdelogoCommon_SRCS 
ADM_vidMPdelogo.cpp  DIA_flyMpDelogo.cpp
)

INCLUDE(vf_plugin_gtk)
INCLUDE(vf_plugin_qt4)

SET(mpdelogoCLI_SRCS cli/DIA_mpdelogo.cpp )
SET(mpdelogoGtk_SRCS gtk/DIA_mpdelogo.cpp )
SET(mpdelogoQT_SRCS  qt4/Q_mpdelogo.cpp)
SET(mpdelogoQT_HEADERS  qt4/Q_mpdelogo.h)
SET(mpdelogoQT_UI    qt4/mpdelogo)

INIT_VIDEOFILTER_PLUGIN_QT4(ADM_vf_mpdelogo_qt4 
                ${mpdelogoQT_SRCS} ${mpdelogoQT_HEADERS} ${mpdelogoQT_UI} ${mpdelogoCommon_SRCS})
INIT_VIDEOFILTER_PLUGIN_GTK(  ADM_vf_mpdelogo_gtk 
                ${mpdelogoGtk_SRCS} ${mpdelogoCommon_SRCS})
#CLI
INCLUDE (vf_plugin_cli)
INIT_VIDEOFILTER_PLUGIN_CLI(ADM_vf_mpdelogo_cli ${mpdelogoCommon_SRCS} ${mpdelogoCLI_SRCS})
INSTALL_VIDEOFILTER(ADM_vf_mpdelogo_cli)
#CLI

