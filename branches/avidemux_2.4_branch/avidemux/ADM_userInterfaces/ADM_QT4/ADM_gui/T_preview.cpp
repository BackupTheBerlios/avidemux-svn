
/***************************************************************************
    Handle all redraw operation for QT4 
    
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
 ***************************************************************************/

#include "config.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <sys/time.h>

#include <QtGui>

/* Probably on unix/X11 ..*/
#ifdef __APPLE__
#include <Carbon/Carbon.h>
#elif !defined(ADM_WIN32)
#include <QX11Info>
#endif

#include "default.h"
#include "GUI_render.h"
#include "GUI_accelRender.h"
#include "ADM_video/ADM_genvideo.hxx"
#include "Q_preview.h"

static QFrame *hostFrame=NULL;
static AccelRender *accelRender=NULL;
static uint8_t *lastImage=NULL;
extern QWidget *QuiMainWindows;
 ColYuvRgb rgbConverter(640,480,1);
extern void UI_purge( void );

static Ui_previewWindow *previewWindow = NULL;

void DIA_previewInit(uint32_t width, uint32_t height)
{
	previewWindow = new Ui_previewWindow(QuiMainWindows, width, height);
	previewWindow->show();
}

uint8_t DIA_previewStillAlive(void)
{
	return (previewWindow != NULL);
}

uint8_t DIA_previewUpdate(uint8_t *buffer)
{
	if (previewWindow)
	{
		previewWindow->update(buffer);

		return 1;
	}

	return 0;
}

void DIA_previewEnd(void)
{
	if (previewWindow)
	{
		delete previewWindow;
		previewWindow = NULL;
	}
}


//****************************************************************************************************
/*
  Function to display
  Warning the incoming data are YUV!
  They are translated to RGB32 by the colorconv instance.

*/
static uint8_t *rgbDataBuffer=NULL;
static uint32_t displayW=0,displayH=0;

//****************************************************************************************************
/*
  This is the class that will display the video images.
  It is a base QWidget where the image will be put by painter.

*/
int paintEngineType = -1;

class  ADM_Qvideo : public QWidget
{
	Q_OBJECT
    
public:
	ADM_Qvideo(QWidget *z) : QWidget(z) {};
	~ADM_Qvideo() {};

	/**
		\fn paintEvent( QPaintEvent *ev))
		\brief Repaint our "video" widget, ignore when accelRender is on
	*/
	void paintEvent(QPaintEvent *ev)
	{
		if (paintEngineType == -1)
		{
			QPainter painter(this);

			if (painter.isActive())
				paintEngineType = painter.paintEngine()->type();

			painter.end();
		}

		if(!displayW || !displayH || !rgbDataBuffer || accelRender)
			return ;

		if(accelRender) 
		{
			if(lastImage)
			{
				accelRender->display(lastImage,displayW,displayH);
			}
		}
		else
		{
			QImage image(rgbDataBuffer,displayW,displayH,QImage::Format_RGB32);
			QPainter painter(this);
			painter.drawImage(QPoint(0,0),image);
			painter.end();
		}
	}
};

ADM_Qvideo *videoWindow=NULL;
//****************************************************************************************************
void UI_QT4VideoWidget(QFrame *host)
{
   videoWindow=new ADM_Qvideo(host);
   hostFrame=host;
   videoWindow->resize(hostFrame->size());
   videoWindow->show();
   
}
//*************************
/**
    \brief return pointer to the drawing widget that displays video
*/
void *UI_getDrawWidget(void)
{
  return (void *) videoWindow;
}
/**
    \brief Display to widget in RGB32
*/
void UI_rgbDraw(void *widg,uint32_t w, uint32_t h,uint8_t *ptr)
{
      memcpy(rgbDataBuffer,ptr,w*h*4);
      videoWindow->repaint();
    
}

/**
      \brief Resize the window
*/
void  UI_updateDrawWindowSize(void *win,uint32_t w,uint32_t h)
{
	if(displayW == w && displayH == h && rgbDataBuffer)
		return;

	if(rgbDataBuffer)
		delete[] rgbDataBuffer;

	rgbDataBuffer = new uint8_t[w * h * 4]; // 32 bits / color
	displayW = w;
	displayH = h;

	hostFrame->setMinimumSize(displayW, displayH);
	videoWindow->setMinimumSize(displayW, displayH);	

	hostFrame->resize(displayW, displayH);
	videoWindow->resize(displayW, displayH);
	UI_purge();

	QuiMainWindows->adjustSize();
	UI_purge();

// Trolltech need to get their act together.  Resizing doesn't work well or the same on all platforms.
#if defined(__APPLE__)
	// Hack required for Mac to resize properly.  adjustSize() just doesn't cut the mustard.
	QuiMainWindows->resize(QuiMainWindows->width() + 1, QuiMainWindows->height() + 1);
#else
	// resizing doesn't work unless called twice on Windows and Linux.
	QuiMainWindows->adjustSize();
#endif

	UI_purge();

	printf("[RDR] Resizing to %u x %u\n", displayW, displayH);
}
/**
      \brief Retrieve info from window, needed for accel layer
*/
void UI_getWindowInfo(void *draw, GUI_WindowInfo *xinfo)
{
	QWidget* widget = videoWindow->parentWidget();

#if defined(ADM_WIN32)
	xinfo->display=videoWindow->winId();
#elif defined(__APPLE__)
	xinfo->display = HIViewGetWindow(HIViewRef(widget->winId()));
	xinfo->window = 0;
#else
    const QX11Info &info=videoWindow->x11Info();
    xinfo->display=info.display();
    xinfo->window=videoWindow->winId();
#endif

	xinfo->x = widget->x();
	xinfo->y = widget->parentWidget()->height() - (widget->y() + displayH);
	xinfo->width = displayW;
	xinfo->height = displayH;
}


//****************************************************************************************************
//EOF 
