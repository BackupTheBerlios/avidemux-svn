/***************************************************************************
                              DIA_mpdelogo.cpp
                              ----------------

                        GUI for MPlayer Delogo filter

    begin                : Tue Sep 30 2008
    copyright            : (C) 2002 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "Q_mpdelogo.h"
#include "ADM_toolkitQt.h"

Ui_mpdelogoWindow::Ui_mpdelogoWindow(QWidget* parent, MPDELOGO_PARAM *param, AVDMGenericVideoStream *in)
{
	ui.setupUi(this);

	lock = 0;

	canvas = new ADM_QCanvas(ui.graphicsView, in->getInfo()->width, in->getInfo()->height);

	myDelogo = new flyMpDelogo(in->getInfo()->width, in->getInfo()->height, in, canvas, ui.horizontalSlider);
	myDelogo->x = param->xoff;
	myDelogo->y = param->yoff;
	myDelogo->width = param->lw;
	myDelogo->height = param->lh;
	myDelogo->_cookie = this;
	myDelogo->upload();
	myDelogo->sliderChanged();

	ui.spinBoxBand->setValue(param->band);

	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderUpdate(int)));
	connect(ui.spinBoxX, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
	connect(ui.spinBoxY, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
	connect(ui.spinBoxWidth, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
	connect(ui.spinBoxHeight, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
}

void Ui_mpdelogoWindow::sliderUpdate(int value)
{
	myDelogo->sliderChanged();
}

void Ui_mpdelogoWindow::gather(MPDELOGO_PARAM *param)
{
	myDelogo->download();

	param->xoff = myDelogo->x;
	param->yoff = myDelogo->y;
	param->lw = myDelogo->width;
	param->lh = myDelogo->height;
	param->band = ui.spinBoxBand->value();
}

Ui_mpdelogoWindow::~Ui_mpdelogoWindow()
{
	delete canvas;
}

void Ui_mpdelogoWindow::valueChanged(int value)
{
	if (lock)
		return;

	lock++;
	myDelogo->download();
	myDelogo->process();
	myDelogo->display();
	lock--;
}

uint8_t flyMpDelogo::download(void)
{
#define SPIN_GET(x, y) x = w->ui.spinBox##y->value();

	Ui_mpdelogoWindow *w = (Ui_mpdelogoWindow*)_cookie;
	int reject = 0;

	SPIN_GET(x, X);
	SPIN_GET(y, Y);
	SPIN_GET(height, Height);
	SPIN_GET(width, Width);

	x &= 0xffffe;
	y &= 0xffffe;
	height &= 0xffffe;
	width &= 0xffffe;

	if ((x + width) > _w)
	{
		if(width >= _w)
			width = _w;

		x = _w - width;
		reject = 1;
	}

	if ((y + height) > _h)
	{
		if(height >= _h)
			height = _h;

		y = _h - height;
		reject = 1;
	}

	if (reject)
		upload();
}

uint8_t flyMpDelogo::upload(void)
{
#define SPIN_SET(x, y) { w->ui.spinBox##y->setValue(x); }

	Ui_mpdelogoWindow *w = (Ui_mpdelogoWindow*)_cookie;

	SPIN_SET(x, X);
	SPIN_SET(y, Y);
	SPIN_SET(width, Width);
	SPIN_SET(height, Height);

	return 1;
}

uint8_t DIA_getMPdelogo(MPDELOGO_PARAM *param, AVDMGenericVideoStream *in)
{
	uint8_t ret = 0;

	Ui_mpdelogoWindow dialog(qtLastRegisteredDialog(), param, in);
	qtRegisterDialog(&dialog);

	if(dialog.exec() == QDialog::Accepted)
	{
		dialog.gather(param); 
		ret = 1;
	}

	qtUnregisterDialog(&dialog);

	return ret;
}
