/***************************************************************************
                               Q_equaliser.cpp
							   ---------------

    begin                : Tue Oct 7 2008
    copyright            : (C) 2008 by mean/gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//#include "config.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "Q_equaliser.h"
#include "ADM_toolkitQt.h"

Ui_equaliserWindow::Ui_equaliserWindow(QWidget* parent, EqualizerParam *param, AVDMGenericVideoStream *in) : QDialog(parent)
{
	ui.setupUi(this);

	_width = in->getInfo()->width;
	_height = in->getInfo()->height;
	lock = 0;

	canvas = new ADM_QCanvas(ui.frame, _width, _height);
	histInCanvas = new ADM_QCanvas(ui.histInFrame, 256, 128);
	histOutCanvas = new ADM_QCanvas(ui.histOutFrame, 256, 128);

	flyDialog = new flyEqualiser(_width, _height, in, canvas, ui.horizontalSlider);
	path = new EqualiserPath(ui.frame_4, flyDialog->crossSettings);
	path->setMinimumSize(ui.frame_4->width(), ui.frame_4->height());
	path->resize(ui.frame_4->width(), ui.frame_4->height());

	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderUpdate(int)));
	connect(path, SIGNAL(pointChanged(int, int)), this, SLOT(pointChanged(int, int)));

	connect(ui.horizontalSlider1, SIGNAL(valueChanged(int)), this, SLOT(slider1Changed(int)));
	connect(ui.horizontalSlider2, SIGNAL(valueChanged(int)), this, SLOT(slider2Changed(int)));
	connect(ui.horizontalSlider3, SIGNAL(valueChanged(int)), this, SLOT(slider3Changed(int)));
	connect(ui.horizontalSlider4, SIGNAL(valueChanged(int)), this, SLOT(slider4Changed(int)));
	connect(ui.horizontalSlider5, SIGNAL(valueChanged(int)), this, SLOT(slider5Changed(int)));
	connect(ui.horizontalSlider6, SIGNAL(valueChanged(int)), this, SLOT(slider6Changed(int)));
	connect(ui.horizontalSlider7, SIGNAL(valueChanged(int)), this, SLOT(slider7Changed(int)));
	connect(ui.horizontalSlider8, SIGNAL(valueChanged(int)), this, SLOT(slider8Changed(int)));

	connect(ui.spinBox1, SIGNAL(valueChanged(int)), this, SLOT(spinBox1Changed(int)));
	connect(ui.spinBox2, SIGNAL(valueChanged(int)), this, SLOT(spinBox2Changed(int)));
	connect(ui.spinBox3, SIGNAL(valueChanged(int)), this, SLOT(spinBox3Changed(int)));
	connect(ui.spinBox4, SIGNAL(valueChanged(int)), this, SLOT(spinBox4Changed(int)));
	connect(ui.spinBox5, SIGNAL(valueChanged(int)), this, SLOT(spinBox5Changed(int)));
	connect(ui.spinBox6, SIGNAL(valueChanged(int)), this, SLOT(spinBox6Changed(int)));
	connect(ui.spinBox7, SIGNAL(valueChanged(int)), this, SLOT(spinBox7Changed(int)));
	connect(ui.spinBox8, SIGNAL(valueChanged(int)), this, SLOT(spinBox8Changed(int)));

	ui.horizontalSlider->setMaximum(in->getInfo()->nb_frames);

	flyDialog->_cookie = this;

	for (int i = 0; i < 8; i++)
		pointChanged(i, param->_scaler[flyDialog->crossSettings[i]]);

	flyDialog->sliderChanged();
}

Ui_equaliserWindow::~Ui_equaliserWindow()
{
	delete flyDialog;
	delete path;
	delete canvas;
	delete histInCanvas;
	delete histOutCanvas;
}

void Ui_equaliserWindow::sliderUpdate(int value)
{
	flyDialog->sliderChanged();
	updateDisplay();
}

void Ui_equaliserWindow::pointChanged(int pointIndex, int value)
{
	QSlider *slider[] = {ui.horizontalSlider1, ui.horizontalSlider2, ui.horizontalSlider3, ui.horizontalSlider4,
		ui.horizontalSlider5, ui.horizontalSlider6, ui.horizontalSlider7, ui.horizontalSlider8};

	slider[pointIndex]->setValue(value);
	updateDisplay();
}

void Ui_equaliserWindow::slider1Changed(int value)
{
	ui.spinBox1->setValue(value);
	path->updatePoint(0, value);
}

void Ui_equaliserWindow::slider2Changed(int value)
{
	ui.spinBox2->setValue(value);
	path->updatePoint(1, value);
}

void Ui_equaliserWindow::slider3Changed(int value)
{
	ui.spinBox3->setValue(value);
	path->updatePoint(2, value);
}

void Ui_equaliserWindow::slider4Changed(int value)
{
	ui.spinBox4->setValue(value);
	path->updatePoint(3, value);
}

void Ui_equaliserWindow::slider5Changed(int value)
{
	ui.spinBox5->setValue(value);
	path->updatePoint(4, value);
}

void Ui_equaliserWindow::slider6Changed(int value)
{
	ui.spinBox6->setValue(value);
	path->updatePoint(5, value);
}

void Ui_equaliserWindow::slider7Changed(int value)
{
	ui.spinBox7->setValue(value);
	path->updatePoint(6, value);
}

void Ui_equaliserWindow::slider8Changed(int value)
{
	ui.spinBox8->setValue(value);
	path->updatePoint(7, value);
}

void Ui_equaliserWindow::spinBox1Changed(int value)
{
	ui.horizontalSlider1->setValue(value);
}

void Ui_equaliserWindow::spinBox2Changed(int value)
{
	ui.horizontalSlider2->setValue(value);
}

void Ui_equaliserWindow::spinBox3Changed(int value)
{
	ui.horizontalSlider3->setValue(value);
}

void Ui_equaliserWindow::spinBox4Changed(int value)
{
	ui.horizontalSlider4->setValue(value);
}

void Ui_equaliserWindow::spinBox5Changed(int value)
{
	ui.horizontalSlider5->setValue(value);
}

void Ui_equaliserWindow::spinBox6Changed(int value)
{
	ui.horizontalSlider6->setValue(value);
}

void Ui_equaliserWindow::spinBox7Changed(int value)
{
	ui.horizontalSlider7->setValue(value);
}

void Ui_equaliserWindow::spinBox8Changed(int value)
{
	ui.horizontalSlider8->setValue(value);
}

void Ui_equaliserWindow::updateDisplay()
{
	if (lock)
		return;

	lock++;

	flyDialog->download();
	flyDialog->process();
	flyDialog->display();

	histInCanvas->dataBuffer = (uint8_t*)flyDialog->histogramIn;
	histInCanvas->repaint();

	histOutCanvas->dataBuffer = (uint8_t*)flyDialog->histogramOut;
	histOutCanvas->repaint();

	lock--;
}

void Ui_equaliserWindow::gather(EqualizerParam *param)
{
	flyDialog->download();
	memcpy(param->_scaler, flyDialog->scaler, 256 * sizeof(int));
}

uint8_t flyEqualiser::upload(void)
{
	Ui_equaliserWindow *window = (Ui_equaliserWindow*)_cookie;

	window->ui.spinBox1->setValue(points[0]);
	window->ui.spinBox2->setValue(points[1]);
	window->ui.spinBox3->setValue(points[2]);
	window->ui.spinBox4->setValue(points[3]);
	window->ui.spinBox5->setValue(points[4]);
	window->ui.spinBox6->setValue(points[5]);
	window->ui.spinBox7->setValue(points[6]);
	window->ui.spinBox8->setValue(points[7]);

	buildScaler(points, scaler);
}

uint8_t flyEqualiser::download(void)
{
	Ui_equaliserWindow *window = (Ui_equaliserWindow*)_cookie;

	points[0] = window->ui.spinBox1->value();
	points[1] = window->ui.spinBox2->value();
	points[2] = window->ui.spinBox3->value();
	points[3] = window->ui.spinBox4->value();
	points[4] = window->ui.spinBox5->value();
	points[5] = window->ui.spinBox6->value();
	points[6] = window->ui.spinBox7->value();
	points[7] = window->ui.spinBox8->value();

	upload();
}

uint8_t DIA_getEqualizer(EqualizerParam *param, AVDMGenericVideoStream *in)
{
	uint8_t ret = 0;

	Ui_equaliserWindow dialog(qtLastRegisteredDialog(), param, in);
	qtRegisterDialog(&dialog);

	if(dialog.exec() == QDialog::Accepted)
	{
		dialog.gather(param); 
		ret = 1;
	}

	qtUnregisterDialog(&dialog);

	return ret;
}
