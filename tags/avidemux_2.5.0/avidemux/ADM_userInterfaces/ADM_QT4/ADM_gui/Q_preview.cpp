/***************************************************************************
                                Q_preview.cpp
                                -------------

    begin                : Mon Sep 8 2008
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

#include "Q_preview.h"

Ui_previewWindow::Ui_previewWindow(QWidget *parent, int width, int height) : QDialog(parent)
{
	ui.setupUi(this);

	canvas = new ADM_QCanvas(ui.frame, width, height);
	preview = new flyPreview(width, height, canvas);
}

Ui_previewWindow::~Ui_previewWindow()
{
	delete preview;
	delete canvas;
}

void Ui_previewWindow::update(uint8_t *buffer)
{
	preview->setData(buffer);
	preview->display();
}
