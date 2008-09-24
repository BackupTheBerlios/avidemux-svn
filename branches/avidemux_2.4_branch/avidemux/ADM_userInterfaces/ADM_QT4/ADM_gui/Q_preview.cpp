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

#include "ui_preview.h"

#include "default.h"
#include "ADM_assert.h"
#include "ADM_video/ADM_genvideo.hxx"
#include "DIA_flyDialog.h"
#include "DIA_flyPreview.h"
#include "../ADM_dialog/DIA_flyDialogQt4.h"

class Ui_previewWindow : public QDialog
{
	Q_OBJECT

public:
	ADM_QCanvas *canvas;
	flyPreview *preview;
	Ui_previewDialog ui;
	Ui_previewWindow(QWidget *parent, int width, int height);
	~Ui_previewWindow();
	void update(uint8_t *buffer);
};

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
