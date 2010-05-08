INCLUDE(vf_plugin)


SET(colorYUVCommon_SRCS 
ADM_vidColorYuv.cpp
)

INCLUDE(vf_plugin_gtk)
INCLUDE(vf_plugin_qt4)

SET(colorYUVGtk_SRCS gtk/DIA_coloryuv.cpp)
SET(colorYUVCLI_SRCS cli/DIA_coloryuv.cpp)
SET(colorYUVQT_SRCS  qt4/Q_coloryuv.cpp)
SET(colorYUVQT_HEADERS  qt4/Q_coloryuv.h)
SET(colorYUVQT_UI    qt4/coloryuv)

INIT_VIDEOFILTER_PLUGIN_GTK(  ADM_vf_colorYUV_gtk 
                ${colorYUVGtk_SRCS} ${colorYUVCommon_SRCS})
INIT_VIDEOFILTER_PLUGIN_QT4(ADM_vf_colorYUV_qt4 
                ${colorYUVQT_SRCS} ${colorYUVQT_HEADERS} ${colorYUVQT_UI} ${colorYUVCommon_SRCS})
#CLI
INCLUDE (vf_plugin_cli)
INIT_VIDEOFILTER_PLUGIN_CLI(ADM_vf_colorYUV_cli ${colorYUVCommon_SRCS} ${colorYUVCLI_SRCS})
INSTALL_VIDEOFILTER(ADM_vf_colorYUV_cli)
#CLI
