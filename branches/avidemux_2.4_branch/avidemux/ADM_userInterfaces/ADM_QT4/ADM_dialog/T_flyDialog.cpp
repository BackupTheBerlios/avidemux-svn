/***************************************************************************
    copyright            : (C) 2006 by mean
    email                : fixounet@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************///

#include "config.h"

#include <QtGui>

#include "default.h"
#include "ADM_assert.h"
#include "ADM_video/ADM_genvideo.hxx"
#include "DIA_flyDialog.h"
#include "DIA_flyDialogQt4.h"

extern float UI_calcZoomToFitScreen(QWidget* window, QWidget* canvas, uint32_t imageWidth, uint32_t imageHeight);
extern uint8_t UI_getPhysicalScreenSize(void* window, uint32_t *w, uint32_t *h);

class FlyDialogEventFilter : public QObject
{
	Q_OBJECT

	ADM_flyDialog *flyDialog;
	bool recomputed;

public:
	FlyDialogEventFilter(ADM_flyDialog *flyDialog)
	{
		recomputed = false;
		this->flyDialog = flyDialog;
	};

protected:
	bool eventFilter(QObject *obj, QEvent *event)
	{
		if (event->type() == QEvent::Show && !recomputed)
		{
			recomputed = true;
			QWidget* parent = (QWidget*)obj;
			uint32_t screenWidth, screenHeight;

			UI_getPhysicalScreenSize(parent, &screenWidth, &screenHeight);
			flyDialog->recomputeSize();
			QCoreApplication::processEvents();
			parent->move((screenWidth - parent->frameSize().width()) / 2, (screenHeight - parent->frameSize().height()) / 2);
		}
		
		return QObject::eventFilter(obj, event);
	};
};

void ADM_flyDialog::postInit(uint8_t reInit)
{
	QWidget *graphicsView = ((ADM_QCanvas*)_canvas)->parentWidget();
	QSlider  *slider=(QSlider *)_slider;

	if (!reInit)
	{
		FlyDialogEventFilter *eventFilter = new FlyDialogEventFilter(this);

		if (slider)
			slider->setMaximum(_in->getInfo()->nb_frames);

		graphicsView->parentWidget()->installEventFilter(eventFilter);
	}

	((ADM_QCanvas*)_canvas)->changeSize(_zoomW, _zoomH);
	graphicsView->setMinimumSize(_zoomW, _zoomH);
	graphicsView->resize(_zoomW, _zoomH);
}

float ADM_flyDialog::calcZoomFactor(void)
{
	return UI_calcZoomToFitScreen(((ADM_QCanvas*)_canvas)->parentWidget()->parentWidget(), ((ADM_QCanvas*)_canvas)->parentWidget(), _w, _h);
}

uint8_t  ADM_flyDialog::display(void)
{
   ADM_QCanvas *view=(ADM_QCanvas *)_canvas;
   ADM_assert(view);
   view->dataBuffer=_rgbBufferOut;
   if(!_rgbBufferOut)
   {
      printf("flyDialog: No rgbuffer ??\n"); 
   } 
   view->repaint();
  return 1; 
}
uint32_t ADM_flyDialog::sliderGet(void)
{
  QSlider  *slide=(QSlider *)_slider;
  ADM_assert(slide);
  return slide->value();
  
}
uint8_t     ADM_flyDialog::sliderSet(uint32_t value)
{
  QSlider  *slide=(QSlider *)_slider;
  ADM_assert(slide);
  slide->setValue(value);
  return 1; 
}
uint8_t  ADM_flyDialog::isRgbInverted(void)
{
  return 1; 
}
uint8_t ADM_flyDialog::cleanup2(void)
{
  return 1; 
}

//EOF
