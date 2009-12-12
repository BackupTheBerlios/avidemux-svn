INCLUDE(vf_plugin)


SET(curveEditorCommon_SRCS
    ADM_vidCurveEditor.cpp  ADM_PointArrayList.cpp
)

INCLUDE(vf_plugin_qt4)

SET(curveEditorQT_SRCS  qt4/Q_CurveDialog.cpp qt4/T_PaintWidget.cpp)
SET(curveEditorQT_HEADERS   qt4/Q_CurveDialog.h qt4/T_PaintWidget.h)
SET(curveEditorQT_UI    qt4/CurveDialog)

INIT_VIDEOFILTER_PLUGIN_QT4(ADM_vf_curveEditor_qt4
    "${curveEditorQT_SRCS}" "${curveEditorQT_HEADERS}" "${curveEditorQT_UI}" "${curveEditorCommon_SRCS}")

