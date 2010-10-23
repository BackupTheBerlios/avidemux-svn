/*
                          Q_CurveDialog.cpp
                          -----------------
    This module including declaration of CurveDialog class that shows main
    dialog window of this filter. Curve painting mechanism is located in
    PaintWidget class (T_PaintWidget.cpp).
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

#include <string.h>

#include <QtGui/QMessageBox>
#include <QtGui/QApplication>

#include "ADM_toolkitQt.h"

#include "Q_CurveDialog.h"
#include "ADM_Utils.h"
#include "ADM_vidCurveEditor_param.h"


/* =========================== *
 *    Method implementation    *
 * =========================== */

/*
 * Initialization of all window components (widgets) and saving input filter
 * parameters.
 */
CurveDialog::CurveDialog(QWidget *parent, p_ColorCurveParam param,
    AVDMGenericVideoStream *in) : QDialog(parent)
{
    setupUi(this);
    ADM_assert(param);
    ADM_assert(in);
    paintWidget = new PaintWidget(this, param);
    mainVerticalLayout->insertWidget(mainVerticalLayout->count()-1, paintWidget);
}

/*
 * Handles key pressing events.
 */
void CurveDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete && paintWidget->isSelected())
    {
        paintWidget->removePoint(paintWidget->getSelectedIndex());
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

/*
 * Handles key releasing events.
 */
void CurveDialog::keyReleaseEvent(QKeyEvent *event)
{
    QWidget::keyReleaseEvent(event);
}

/*
 * Changes active color channel.
 */
void CurveDialog::on_channelComboBox_currentIndexChanged(int index)
{
    paintWidget->setCurrentChannel(index);
}

/*
 * Sets default curve on current channel.
 */
void CurveDialog::on_defaultButton_clicked()
{
    paintWidget->resetPoints();
}

/*
 * Shows help dialog.
 */
void CurveDialog::on_helpButton_clicked()
{
    static char msg[] = "<h2>Color Curve Editor for Avidemux 2.5</h2>\n"
    "<b>email: george.janec@gmail.com</b>\n"
    "<p>This program is creating spline curves that can be used for colour "
    "adjustment. You can edit three curves in YUV colour space.</p>\n"
    "<i>Copyright (C) 2009 Jiri Janecek</i>\n"
    "<p><i>This program is distributed in the hope that it will be useful, "
    "but WITHOUT ANY WARRANTY; without even the implied warranty of "
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
    "GNU General Public License for more details.</i></p>\n"
    "<h3>Program usage</h3>\n"
    "<p>Colour channel can be selected by pop-up list located at top-left part "
    "of dialog window. Each channel is edited separately. New points can be "
    "created by clicking on marked area. Moving points can be done by dragging "
    "a point to a different location. Refresh button loads default curve. "
    "Selected points (marked by red colour) can by erased by pressing Delete key.</p>";
    QString qtitle = QString::fromUtf8("About");
    QString qmsg = QString::fromUtf8(msg, sizeof(msg));
    QMessageBox::information(this, qtitle, qmsg, QMessageBox::Ok, QMessageBox::Ok);
}


/* ============================================================== *
 *     Implementation of function from ADM_vidCurveEditor.cpp     *
 * ============================================================== */

/*
 * Shows filter dialog window. If dialog is confirmed by Ok button then local
 * changes are applied on input parameters.
 */
uint8_t DIA_RunCurveDialog(p_ColorCurveParam param, AVDMGenericVideoStream *in)
{
    uint8_t retcode = 0;
    CurveDialog mainDiag(qtLastRegisteredDialog(), param, in);
    qtRegisterDialog(&mainDiag);
    if (mainDiag.exec() == QDialog::Accepted)
    {
        mainDiag.paintWidget->applyChanges();
        retcode = 1;
    }
    qtUnregisterDialog(&mainDiag);
    return retcode;
}

