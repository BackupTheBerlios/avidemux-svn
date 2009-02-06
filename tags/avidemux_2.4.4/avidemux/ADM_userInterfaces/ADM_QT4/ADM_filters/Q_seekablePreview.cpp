/***************************************************************************
                           Q_seekablePreview.cpp
                           ---------------------

    begin                : Fri Sep 5 2008
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

#include "ui_seekablePreview.h"

#include "default.h"
#include "ADM_assert.h"
#include "ADM_video/ADM_genvideo.hxx"
#include "DIA_flyDialog.h"
#include "DIA_flyPreview.h"
#include "../ADM_dialog/DIA_flyDialogQt4.h"

class Ui_seekablePreviewWindow : public QDialog
{
	Q_OBJECT

public:
	ADM_QCanvas *canvas;
	flySeekablePreview *seekablePreview;
	Ui_seekablePreviewDialog ui;
	Ui_seekablePreviewWindow(QWidget *parent, AVDMGenericVideoStream *videoStream, uint32_t defaultFrame = 0);
	~Ui_seekablePreviewWindow();
	void resetVideoStream(AVDMGenericVideoStream *videoStream);
	uint32_t frameIndex();

public slots:
	void sliderChanged(int value);
};

Ui_seekablePreviewWindow::Ui_seekablePreviewWindow(QWidget *parent, AVDMGenericVideoStream *videoStream, uint32_t defaultFrame) : QDialog(parent)
{
	ui.setupUi(this);

	seekablePreview = NULL;
	canvas = NULL;

	resetVideoStream(videoStream);
	seekablePreview->sliderSet(defaultFrame);
	seekablePreview->sliderChanged();

	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));
}

Ui_seekablePreviewWindow::~Ui_seekablePreviewWindow()
{
	delete seekablePreview;
	delete canvas;
}

void Ui_seekablePreviewWindow::resetVideoStream(AVDMGenericVideoStream *videoStream)
{
	if (seekablePreview)
		delete seekablePreview;

	if (canvas)
		delete canvas;

	uint32_t canvasWidth = videoStream->getInfo()->width;
	uint32_t canvasHeight = videoStream->getInfo()->height;

	canvas = new ADM_QCanvas(ui.frame, canvasWidth, canvasHeight);
	canvas->show();
	seekablePreview = new flySeekablePreview(canvasWidth, canvasHeight, videoStream, canvas, ui.horizontalSlider);	

	seekablePreview->process();
	seekablePreview->sliderChanged();
}

void Ui_seekablePreviewWindow::sliderChanged(int value)
{
	seekablePreview->sliderChanged();
}

uint32_t Ui_seekablePreviewWindow::frameIndex()
{
	return seekablePreview->sliderGet();
}