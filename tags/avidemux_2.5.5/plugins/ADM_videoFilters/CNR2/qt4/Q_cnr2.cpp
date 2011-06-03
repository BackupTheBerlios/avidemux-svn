/***************************************************************************
                                 Q_cnr2.cpp
                                 ----------

    begin                : Fri Oct 3 2008
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

#include <math.h>
#include "Q_cnr2.h"
#include "ADM_toolkitQt.h"

Cnr2Window::Cnr2Window(QWidget* parent, CNR2Param *param) : QDialog(parent)
{
	this->param = param;

	ui.setupUi(this);

	connect(ui.sceneChangeSlider, SIGNAL(valueChanged(int)), this, SLOT(sceneChangeSlider_valueChanged(int)));
	connect(ui.sceneChangeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(sceneChangeSpinBox_valueChanged(double)));

	ui.sensibilityYSpinBox->setValue(param->ln);
	ui.sensibilityUSpinBox->setValue(param->un);
	ui.sensibilityVSpinBox->setValue(param->vn);

	ui.maxYSpinBox->setValue(param->lm);
	ui.maxUSpinBox->setValue(param->um);
	ui.maxVSpinBox->setValue(param->vm);

	ui.useChromaCheckBox->setChecked(param->sceneChroma);

	if (param->mode & 0xFF0000)
		ui.modeYComboBox->setCurrentIndex(1);

	if (param->mode & 0xFF00)
		ui.modeUComboBox->setCurrentIndex(1);

	if (param->mode & 0xFF)
		ui.modeVComboBox->setCurrentIndex(1);

	ui.sceneChangeSpinBox->setValue(param->scdthr);
}

void Cnr2Window::gather(void)
{
	param->ln = ui.sensibilityYSpinBox->value();
	param->un = ui.sensibilityUSpinBox->value();
	param->vn = ui.sensibilityVSpinBox->value();

	param->lm = ui.maxYSpinBox->value();
	param->um = ui.maxUSpinBox->value();
	param->vm = ui.maxVSpinBox->value();

	param->sceneChroma = ui.useChromaCheckBox->isChecked();

	param->mode = 0;

	if (ui.modeYComboBox->currentIndex())
		param->mode |= 0xFF0000;

	if (ui.modeUComboBox->currentIndex())
		param->mode |= 0xFF00;

	if (ui.modeVComboBox->currentIndex())
		param->mode |= 0xFF;

	param->scdthr = ui.sceneChangeSpinBox->value();
}

void Cnr2Window::sceneChangeSlider_valueChanged(int value)
{
	ui.sceneChangeSpinBox->setValue(((double)value) / 100);
}

void Cnr2Window::sceneChangeSpinBox_valueChanged(double value)
{
	ui.sceneChangeSlider->setValue(floor((value * 100) + 0.49));
}

uint8_t DIA_cnr2(CNR2Param *param)
{
	int r = 0;
	Cnr2Window cnr2Window(qtLastRegisteredDialog(), param);

	qtRegisterDialog(&cnr2Window);

	if (cnr2Window.exec() == QDialog::Accepted)
	{
		cnr2Window.gather();
		r = 1;
	}

	qtUnregisterDialog(&cnr2Window);

	return r;
}
