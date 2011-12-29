#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>

#include "ADM_Point.h"
#include "ADM_vidCurveEditor_param.h"


class PaintWidget : public QWidget {
    Q_OBJECT

public:
    PaintWidget(QWidget *parent, p_ColorCurveParam param);
    ~PaintWidget();
    void applyChanges() const;
    void setCurrentChannel(const int index);
    bool isSelected() const;
    int getSelectedIndex() const;
    void removePoint(const int index);
    void resetPoints();

protected:
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    p_ColorCurveParam in_param; // input parameters
    p_ColorCurveParam p_param;  // local copy of input parameters
    float scaleFactor;
    int currentChannel;
    int selectedIndex;
    bool dragging;
    char posText[48];
    void hermiteInterp(int pid1, int pid2, float m1, float m2);
    void generateTable();
};

#endif // PAINTWIDGET_H

