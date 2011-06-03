INCLUDE(vf_plugin)


SET(equalizerCommon_SRCS 
ADM_vidEqualizer.cpp
)

INCLUDE(vf_plugin_gtk)
INCLUDE(vf_plugin_qt4)
INCLUDE_DIRECTORIES("${AVIDEMUX_SOURCE_DIR}/avidemux/ADM_libraries/ffmpeg")

SET(equalizerGtk_SRCS gtk/DIA_equalizer.cpp)
SET(equalizerCLI_SRCS cli/DIA_equalizer.cpp)
SET(equalizerQT_SRCS  DIA_flyEqualiser.cpp  qt4/Q_equaliser.cpp  qt4/T_gridFrame.cpp  qt4/T_EqualiserPath.cpp)
SET(equalizerQT_HEADERS  qt4/Q_equaliser.h  qt4/T_gridFrame.h  qt4/T_EqualiserPath.h)
SET(equalizerQT_UI    qt4/equaliser)

INIT_VIDEOFILTER_PLUGIN_GTK(  ADM_vf_equalizer_gtk 
                ${equalizerGtk_SRCS} ${equalizerCommon_SRCS})
INIT_VIDEOFILTER_PLUGIN_QT4(ADM_vf_equalizer_qt4 
                "${equalizerQT_SRCS}" "${equalizerQT_HEADERS}" "${equalizerQT_UI}" "${equalizerCommon_SRCS}")

#CLI
INCLUDE (vf_plugin_cli)
INIT_VIDEOFILTER_PLUGIN_CLI(ADM_vf_equalizer_cli ${equalizerCommon_SRCS} ${equalizerCLI_SRCS})
INSTALL_VIDEOFILTER(ADM_vf_equalizer_cli)
#CLI

