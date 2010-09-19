/***************************************************************************
    copyright            : (C) 2010 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef GUI_QTGLRENDER_H
#define GUI_QTGLRENDER_H

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLShader>

#include "GUI_render.h"
#include "GUI_accelRender.h"
#include "ADM_colorspace.h"

class QtGlAccelWidget : public QGLWidget
{
private:
	int imageWidth, imageHeight;
	bool firstRun;

	QGLShaderProgram *glProgram;
	GLsizei textureWidths[3];
	GLsizei textureHeights[3];
	uint8_t *textureOffsets[3];

#ifndef QT_OPENGL_ES
	typedef void (APIENTRY *_glActiveTexture) (GLenum);
	_glActiveTexture glActiveTexture;
#endif

protected:
	void initializeGL();
	void paintGL();

public:
	QtGlAccelWidget(QWidget *parent, int widgetWidth, int widgetHeight);
	void setBuffer(uint8_t *buffer, int imageWidth, int imageHeight);
};


class QtGlAccelRender : public AccelRender
{
private:
	QtGlAccelWidget *glWidget;

public:
	QtGlAccelRender(void);
	virtual	uint8_t init(GUI_WindowInfo *window, uint32_t w, uint32_t h);
	virtual	uint8_t end(void);
	virtual uint8_t display(uint8_t *ptr, uint32_t w, uint32_t h, renderZoom zoom);
	uint8_t hasHwZoom(void);
};
#endif
