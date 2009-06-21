/***************************************************************************
                               Q_coloryuv.cpp
                               --------------

    begin                : Thu Oct 2 2008
    copyright            : (C) 2008 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "Q_coloryuv.h"
#include "ADM_toolkitQt.h"

ColorYuvWindow::ColorYuvWindow(QWidget* parent, COLOR_YUV_PARAM *param) : QDialog(parent)
{
	this->param = param;

	ui.setupUi(this);

	ui.contrastYSpinBox->setValue(param->y_contrast);
	ui.contrastUSpinBox->setValue(param->u_contrast);
	ui.contrastVSpinBox->setValue(param->v_contrast);

	ui.brightnessYSpinBox->setValue(param->y_bright);
	ui.brightnessUSpinBox->setValue(param->u_bright);
	ui.brightnessVSpinBox->setValue(param->v_bright);

	ui.gammaYSpinBox->setValue(param->y_gamma);
	ui.gammaUSpinBox->setValue(param->u_gamma);
	ui.gammaVSpinBox->setValue(param->v_gamma);

	ui.gainYSpinBox->setValue(param->y_gain);
	ui.gainUSpinBox->setValue(param->u_gain);
	ui.gainVSpinBox->setValue(param->v_gain);

	ui.matrixComboBox->setCurrentIndex(param->matrix);
	ui.optComboBox->setCurrentIndex(param->opt);
	ui.levelComboBox->setCurrentIndex(param->levels);

	ui.autoGainCheckBox->setChecked(param->autogain);
	ui.colourStatsCheckBox->setChecked(param->analyze);
	ui.centreOffsetsCheckBox->setChecked(param->autowhite);
}

void ColorYuvWindow::gather(void)
{
	param->y_contrast = ui.contrastYSpinBox->value();
	param->u_contrast = ui.contrastUSpinBox->value();
	param->v_contrast = ui.contrastVSpinBox->value();

	param->y_bright = ui.brightnessYSpinBox->value();
	param->u_bright = ui.brightnessUSpinBox->value();
	param->v_bright = ui.brightnessVSpinBox->value();

	param->y_gamma = ui.gammaYSpinBox->value();
	param->u_gamma = ui.gammaUSpinBox->value();
	param->v_gamma = ui.gammaVSpinBox->value();

	param->y_gain = ui.gainYSpinBox->value();
	param->u_gain = ui.gainUSpinBox->value();
	param->v_gain = ui.gainVSpinBox->value();

	param->matrix = ui.matrixComboBox->currentIndex();
	param->opt = ui.optComboBox->currentIndex();
	param->levels = ui.levelComboBox->currentIndex();

	param->autogain = ui.autoGainCheckBox->isChecked();
	param->analyze = ui.colourStatsCheckBox->isChecked();
	param->autowhite = ui.centreOffsetsCheckBox->isChecked();
}

int DIA_coloryuv(COLOR_YUV_PARAM *param)
{
	int r = 0;
	ColorYuvWindow yuvWindow(qtLastRegisteredDialog(), param);

	qtRegisterDialog(&yuvWindow);

	if (yuvWindow.exec() == QDialog::Accepted)
	{
		yuvWindow.gather();
		r = 1;
	}

	qtUnregisterDialog(&yuvWindow);

	return r;
}
