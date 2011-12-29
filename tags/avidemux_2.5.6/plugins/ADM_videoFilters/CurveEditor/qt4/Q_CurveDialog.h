#ifndef CURVEDIALOG_H
#define CURVEDIALOG_H

#include <QtGui/QWidget>
#include <QtGui/QKeyEvent>

#include "ADM_default.h"
#include "ADM_videoFilterDynamic.h"

#include "ui_CurveDialog.h"
#include "T_PaintWidget.h"
#include "ADM_vidCurveEditor_param.h"


class CurveDialog : public QDialog, public Ui::CurveDialog {
    Q_OBJECT

public:
    PaintWidget *paintWidget;
    CurveDialog(QWidget *parent, p_ColorCurveParam param, AVDMGenericVideoStream *in);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void on_channelComboBox_currentIndexChanged(int index);
    void on_defaultButton_clicked();
    void on_helpButton_clicked();
};

#endif // CURVEDIALOG_H

