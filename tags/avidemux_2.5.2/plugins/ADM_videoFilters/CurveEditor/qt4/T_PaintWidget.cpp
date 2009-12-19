/*
                          T_PaintWidget.cpp
                          -----------------
    This module including declaration of PaintWidget class that paints cubic
    Hermite spline curve.
    email: george.janec@gmail.com
    
    Copyright (C) 2009 Jiri Janecek

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <math.h>

#include <QtCore/QString>
#include <QtCore/QSize>
#include <QtGui/QPainter>
#include <QtGui/QSizePolicy>

#include "ADM_default.h"

#include "T_PaintWidget.h"
#include "ADM_Utils.h"


/* ======================== *
 *    Macro declaration     *
 * ======================== */

/*
 * Crops input value in defined bounds (min, max).
 * Result is saved to value.
 */
#define CLAMP(val, min, max) \
    (val) = (val) < (min) ? (min) : (val) > (max) ? (max) : (val)

/*
 * Flips vertical coordinate to convert window coords to real coords.
 */
#define FLIP(y) (255 - (y))

/*
 * Calculates real coordinate by scaling factor.
 */
#define SCALE(x) ROUND((x) * scaleFactor)
#define RESCALE(b) ROUND((b) / scaleFactor)

/*
 * CP = Current point(index)
 * CT = Current table(index)
 */
#define CP(index) p_param->points[currentChannel].get(index)
#define CT(index) p_param->table[currentChannel][index]

/*
 * Calculates one-sided difference from point at index k.
 */
#define DIFF(k) (float)(CP(k+1)->y - CP(k)->y) / (CP(k+1)->x - CP(k)->x)


/* =========================== *
 *    Method implementation    *
 * =========================== */

/*
 * Components are initialized here and input parameters in_param are copied to
 * temporary variable p_param.
 */
PaintWidget::PaintWidget(QWidget *parent, p_ColorCurveParam param)
    : QWidget(parent)
{
    setObjectName(QString::fromUtf8("PaintWidget"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy1);
    setMinimumSize(QSize(256, 256));
    setCursor(Qt::OpenHandCursor);
    setMouseTracking(true);

    in_param = param;   // save address of original parameters
    p_param = new ColorCurveParam();
    p_param->points[0].copy(in_param->points[0]);
    p_param->points[1].copy(in_param->points[1]);
    p_param->points[2].copy(in_param->points[2]);
    memcpy(p_param->table, in_param->table, sizeof(in_param->table));

    scaleFactor = 1.0f;
    currentChannel = 0;
    selectedIndex = -1;
    dragging = false;
    *posText = '\0';        // empty string
}

/*
 * Delete local parameters.
 */
PaintWidget::~PaintWidget()
{
    if (p_param) delete p_param;
}

/*
 * Overwrites original parameters in_param with values stored in p_param.
 */
void PaintWidget::applyChanges() const
{
    in_param->points[0].copy(p_param->points[0]);
    in_param->points[1].copy(p_param->points[1]);
    in_param->points[2].copy(p_param->points[2]);
    memcpy(in_param->table , p_param->table, sizeof(p_param->table));
}

/*
 * Set active channel. Value must be between 0 and 2.
 */
void PaintWidget::setCurrentChannel(const int index)
{
    currentChannel = (index >= 0 && index <= 2) ? index : 0;
    selectedIndex = -1;
    update();
}

/*
 * Returns true if some point is selected else false.
 */
bool PaintWidget::isSelected() const
{
    return selectedIndex != -1;
}

/*
 * Returns index of selected point.
 */
int PaintWidget::getSelectedIndex() const
{
    return selectedIndex;
}

/*
 * Removes point from list at index.
 */
void PaintWidget::removePoint(const int index)
{
    p_param->points[currentChannel].remove(index);
    generateTable();
    if (index == selectedIndex) selectedIndex = -1;
    update();
}

/*
 * Sets default curve.
 */
void PaintWidget::resetPoints()
{
    p_param->points[currentChannel].reset();
    generateTable();
    selectedIndex = -1;
    update();
}

/*
 * Calculates scaling factor for adjusting curve size.
 */
void PaintWidget::resizeEvent(QResizeEvent *event)
{
    QSize s = event->size();
    if (s.width() != s.height())
    {
        int newSize = s.width() < s.height() ? s.width() : s.height();
        resize(newSize, newSize);
        scaleFactor = 256.0f / newSize;
    }
}

/*
 * Handles moving points over painting area.
 */
void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    int x = SCALE(event->x());
    int y = FLIP(SCALE(event->y()));
    int count = p_param->points[currentChannel].count();
    if (dragging && isSelected()) {
        if (selectedIndex == 0) {
            CLAMP(x, 0, CP(selectedIndex + 1)->x - 1);
        } else if (selectedIndex == count-1) {
            CLAMP(x, CP(selectedIndex - 1)->x + 1, 255);
        } else {
            CLAMP(x, CP(selectedIndex - 1)->x + 1, CP(selectedIndex + 1)->x - 1);
        }
        CLAMP(y, 0, 255);
        CP(selectedIndex)->set(x, y);
        generateTable();
        sprintf(posText, "input: %d output: %d", x, y);
    } else {
        QWidget::mouseMoveEvent(event);
    }
    update();
}

/*
 * Handles creating and selecting points by mouse clicking.
 */
void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    int insPos;
    int max = p_param->points[currentChannel].count() - 1;
    int x = SCALE(event->x());
    int y = SCALE(event->y());
    if (event->button() == Qt::LeftButton
            && x >= 0 && x <= 255 && y >= 0 && y <= 255) {
        y = FLIP(y);
        insPos = p_param->points[currentChannel].search(x);
        insPos = insPos < 0 ? -(insPos + 1) : insPos;
        // testing at most 3 points around middle point
        int i = -1;
        while (true)
        {
            if (BOUNDS(insPos + i, max) && CP(insPos + i)->contains(x, y))
            {
                selectedIndex = insPos + i;
                break;
            }
            // terminating condition
            if (i == 1)
            {
                if (p_param->points[currentChannel].insert(insPos, x, y))
                {
                    generateTable();
                    selectedIndex = insPos;
                }
                else
                {
                    selectedIndex = -1;
                }
                break;
            }
            i++;
        }
        sprintf(posText, "input: %d output: %d", x, y);
        dragging = true;
        this->setCursor(Qt::ClosedHandCursor);
        update();
    } else if (event->button() == Qt::RightButton) {
        selectedIndex = -1;
        update();
    } else {
        QWidget::mousePressEvent(event);
    }
}

/*
 * Handles mouse release event.
 */
void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        *posText = '\0';
        dragging = false;
        this->setCursor(Qt::OpenHandCursor);
    }
    else {
        QWidget::mouseReleaseEvent(event);
    }
}

/*
 * Draws spline on widget.
 */
void PaintWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter;

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::blue);
    painter.fillRect(QRect(0, 0, this->width(), this->height()), QBrush(Qt::white));

    painter.setPen(QColor::fromRgb(192, 192, 192));
    painter.drawLine(RESCALE(63), RESCALE(0), RESCALE(63), RESCALE(255));
    painter.drawLine(RESCALE(127), RESCALE(0), RESCALE(127), RESCALE(255));
    painter.drawLine(RESCALE(191), RESCALE(0), RESCALE(191), RESCALE(255));

    painter.drawLine(RESCALE(0), RESCALE(63), RESCALE(255), RESCALE(63));
    painter.drawLine(RESCALE(0), RESCALE(127), RESCALE(255), RESCALE(127));
    painter.drawLine(RESCALE(0), RESCALE(191), RESCALE(255), RESCALE(191));

    painter.drawLine(RESCALE(0), RESCALE(255), RESCALE(255), RESCALE(0));

    // drawing spline
    painter.setPen(Qt::black);
    for (int i = 1; i < 256; i++)
    {
        painter.drawLine(
            RESCALE(i-1), RESCALE(FLIP(CT(i-1))),
            RESCALE(i), RESCALE(FLIP(CT(i)))
        );
    }

    // drawing control points
    for (int i = 0; i < p_param->points[currentChannel].count(); i++) {
        if (selectedIndex == i) {
            painter.fillRect(
                RESCALE(CP(i)->x) - 3,
                RESCALE(FLIP(CP(i)->y)) - 3,
                7, 7, QBrush(Qt::red)
            );
        }
        else {
            painter.fillRect(
                RESCALE(CP(i)->x) - 3,
                RESCALE(FLIP(CP(i)->y)) - 3,
                7, 7, painter.brush()
            );
        }
    }

    if (posText[0] != '\0') {
        painter.setFont(QFont("Arial", 10));
        painter.drawText(6, 16, QString::fromUtf8(posText));
    }

    painter.end();
}

/*
 * Calculates spline curve in segment between points pid1 and pid2 with
 * tangents m1 and m2. Description of Cubic Hermite splines you can find at:
 * http://en.wikipedia.org/wiki/Cubic_Hermite_spline
 */
inline void PaintWidget::hermiteInterp(int pid1, int pid2, float m1, float m2)
{
    float F1, F2, F3, F4;
    int Q;
    float Qf;
    int intervalWidth = CP(pid2)->x - CP(pid1)->x;
    float norm = 1.0f / intervalWidth;
    float t, t2, t3;

    CT(CP(pid1)->x) = CP(pid1)->y;
    for (int i = 1; i <= intervalWidth; i++) {
        t = i * norm;
        t2 = t * t;
        t3 = t2 * t;

        F1 = 2*t3 - 3*t2 + 1;
        F2 = -2*t3 + 3*t2;
        F3 = t3 - 2*t2 + t;
        F4 = t3 - t2;

        // Calculates interpolation
        Qf = CP(pid1)->y*F1 + CP(pid2)->y*F2 + m1*intervalWidth*F3 + m2*intervalWidth*F4;

        Q = ROUND(Qf);      // rounding to integer
        CLAMP(Q, 0, 255);   // cropping values <0, 255>

        CT(CP(pid1)->x + i) = (uint8_t) Q;
    }
}

/*
 * Creates transformation table. First tangents are calculated.
 * Finally interpolation method is called.
 * Tangents are generated by algorithm described at Wikipedia:
 * Monotone cubic Hermite interpolation
 * http://en.wikipedia.org/wiki/Monotone_cubic_interpolation
 */
void PaintWidget::generateTable()
{
    int count = p_param->points[currentChannel].count();
    float *m = new float[count];
    float *sec = new float[count-1];

    for (int i = 0; i < CP(0)->x; i++) {
        CT(i) = CP(0)->y;
    }

    // data preprocessing
    for (int i = 0; i < count - 1; i++) sec[i] = DIFF(i);

    m[0] = DIFF(0);
    m[count - 1] = DIFF(count - 2);
    for (int i = 1; i < count - 1; i++) m[i] = (sec[i-1] + sec[i]) / 2.0f;

    for (int i = 0; i < count - 1; i++)
    {
        float tmpSec = sec[i];
        if (tmpSec == 0)
        {
            m[i] = m[i + 1] = 0;
        }
        else
        {
            float ak, bk;
            float *m1 = &m[i];
            float *m2 = &m[i + 1];
            ak = *m1 / tmpSec;
            bk = *m2 / tmpSec;
            float cirEq = ak*ak + bk*bk;
            if (cirEq > 9.0f)
            {
                float tk = 3.0f / sqrt(cirEq);
                *m1 = *m1 * tk;
                *m2 = *m2 * tk;
            }
        }
    }

    // cubic interpolation
    for (int i = 0; i < count - 1; i++)
    {
        hermiteInterp(i, i + 1, m[i], m[i + 1]);
    }

    for (int i = CP(count-1)->x + 1; i < 256; i++) {
        CT(i) = CP(count-1)->y;
    }
    
    delete [] m;
    delete [] sec;
}

