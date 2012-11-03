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
***************************************************************************///
#define GL_GLEXT_PROTOTYPES

#include <QtGui/QPainter>

#ifdef __APPLE__
#	include <OpenGL/gl.h>
#	include <OpenGL/glu.h>
#	include <OpenGL/glext.h>
#	define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_RECTANGLE_EXT
#else
#	ifdef _WIN32
#		include <windows.h>
#	endif
#	include <GL/gl.h>
#	include <GL/glu.h>
#	include <GL/glext.h>
#endif
#define ADM_LEGACY_PROGGY // Dont clash with free/malloc etc..
#include "ADM_default.h"
#include "GUI_render.h"

#include "GUI_accelRender.h"
#include "GUI_qtGlRender.h"

const char *QtGlAccelWidget::yuvToRgb =
	"#extension GL_ARB_texture_rectangle: enable\n"
	"uniform sampler2DRect texY, texU, texV;\n"
	"uniform float height;\n"
    "const mat4 mytrix=mat4( 1,   0,         1.5958,   0,"
                            "1,  -0.39173,  -0.81290,  0,"
                            "1,   2.017,      0,       0,"
                            "0,        0,     0,       1);\n"
                            
    "const vec4 offsetx=vec4(-0.07276875,-0.5,-0.5,0);\n"
    "const vec4 factorx=vec4(1.1643,1,1,1);\n"
    "const vec2 factorTex=vec2(0.5,0.5);"

	"void main(void) {\n"
	"  float nx = gl_TexCoord[0].x;\n"
	"  float ny = height - gl_TexCoord[0].y;\n"
    "  vec2 coord=vec2(nx,ny);"
    "  vec2 coord2=coord*factorTex;"
	"  float y = texture2DRect(texY, coord).r;\n"
	"  float u = texture2DRect(texU, coord2).r;\n"
	"  float v = texture2DRect(texV, coord2).r;\n"

    "  vec4 inx=vec4(y,u,v,1.0);\n"
    "  vec4 inx2=(factorx*inx)+offsetx;\n"
    "  vec4 outx=inx2*mytrix;\n"
	"  gl_FragColor = outx;\n"
	"}\n";

/**
    \fn checkGlError
*/
bool QtGlAccelWidget::checkGlError(const char *op)
{
    GLenum er = glGetError();

	if(!er)
	{
		return true;
	}

    ADM_error("[GLERROR]%s: %d => %s\n", op, er, gluErrorString(er));

    return false;
}          

/**
    \fn ctor
*/
QtGlAccelWidget::QtGlAccelWidget(QWidget *parent, int w, int h) : QGLWidget(parent)
{
	this->_initialised = true;

	memset(this->_textureRealWidths, 0, sizeof(this->_textureRealWidths));
	memset(this->_textureStrides, 0, sizeof(this->_textureStrides));
	memset(this->_textureHeights, 0, sizeof(this->_textureHeights));
	memset(this->_textureOffsets, 0, sizeof(this->_textureOffsets));

	this->_imageWidth = w;
	this->_imageHeight = h;
	this->_firstRun = true;
	this->_glProgram = NULL;
	this->_textureName[0] = this->_textureName[1] = this->_textureName[2] = 0;

	if (!QGLFormat::hasOpenGL())
    {
		ADM_warning("OpenGL not supported on this system\n");
        this->_initialised = false;
    }

	this->makeCurrent();

	printf("[GL Render] OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
	printf("[GL Render] OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("[GL Render] OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("[GL Render] OpenGL Extensions: %s\n", glGetString(GL_EXTENSIONS));

	if (this->_initialised)
	{
		this->_activeTexture = reinterpret_cast<PFNGLACTIVETEXTUREPROC>(
			this->context()->getProcAddress(QLatin1String("glActiveTexture")));

		if (this->_activeTexture == NULL)
		{
			ADM_warning("OpenGL implementation doesn't support ActiveTexture\n");
			this->_initialised = false;
		}
	}
	else
	{
		this->_activeTexture = NULL;
	}

	if (this->_initialised && !QGLShaderProgram::hasOpenGLShaderPrograms(this->context()))
	{
		ADM_warning("OpenGL implementation doesn't support OpenGL shader programs\n");
		this->_initialised = false;
	}

	if (this->_initialised)
	{
		this->_glProgram = new QGLShaderProgram(this);

		ADM_info("Creating glWidget\n");
		glGenTextures(3, this->_textureName);
	}
}

bool QtGlAccelWidget::initialised()
{
	return this->_initialised;
}

QtGlAccelWidget* QtGlAccelWidget::create(QWidget *parent, int imagew, int imageh)
{
	QtGlAccelWidget* widget = new QtGlAccelWidget(parent, imagew, imageh);

	if (!widget->initialised())
	{
		delete widget;
		widget = NULL;
	}

	return widget;
}

/**
    \fn setDisplaySize
*/
bool QtGlAccelWidget::setDisplaySize(int width, int height)
{
    this->_displayWidth = width;
    this->_displayHeight = height;
    this->resize(this->_displayWidth, this->_displayHeight);
    this->_firstRun = true;

    return true;
}
/**
        \fn dtor
*/
QtGlAccelWidget::~QtGlAccelWidget()
{
    ADM_info("\t Deleting glWidget\n");

    if (this->_initialised)
	{
        glDeleteTextures(3, this->_textureName);
	}

    this->_textureName[0] = 0;
}

/**
    \fn setImage
*/

bool QtGlAccelWidget::setImage(ADMImage *pic)
{
	this->_imageWidth = pic->_width;
	this->_imageHeight = pic->_height;

    for (int i = 0; i < 3; i++)
    {
        ADM_PLANE plane=(ADM_PLANE)i;

        this->_textureRealWidths[i] = pic->GetWidth(plane);
        this->_textureStrides[i] = pic->GetPitch(plane);
        this->_textureHeights[i] = pic->GetHeight(plane);
        this->_textureOffsets[i] = pic->GetReadPtr(plane);
    }

    updateTexture();

    return true;
}
/**
    \fn initializeGL
*/
void QtGlAccelWidget::initializeGL()
{
	bool success = true;

	if (!this->_glProgram->addShaderFromSourceCode(QGLShader::Fragment, yuvToRgb))
	{
		success = false;
		printf("[GL Render] Fragment log: %s\n", this->_glProgram->log().toUtf8().constData());
	}

	if (success && !this->_glProgram->link())
	{
		success = false;
		printf("[GL Render] Link log: %s\n", this->_glProgram->log().toUtf8().constData());
	}

	if (success && !this->_glProgram->bind())
	{
		success = false;
		printf("[GL Render] Binding FAILED\n");
	}

	this->_glProgram->setUniformValue("texY", 0);
	this->_glProgram->setUniformValue("texU", 2);
	this->_glProgram->setUniformValue("texV", 1);

    if (success)
	{
        printf("[GL Render] Init successful\n");
	}
}
/**
    \fn updateTexture
*/
void QtGlAccelWidget::updateTexture()
{
	checkGlError("Entering UpdateTexture");

	if (!this->_textureOffsets[0])
	{
		printf("[Render] Buffer not set\n");
		return;
	}

	if (this->_firstRun)
	{
		glViewport(0, 0, width(), height());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width(), 0, height(), -1, 1);
		this->_glProgram->setUniformValue("height", (float)this->_imageHeight);
	}

	// Activate texture unit "tex"
	for(int xplane=2;xplane>=0;xplane--)
	{
		this->_activeTexture(GL_TEXTURE0+xplane);
		ADM_PLANE plane=(ADM_PLANE)xplane;
		glBindTexture(GL_TEXTURE_RECTANGLE_NV, this->_textureName[xplane]); // Use tex engine "texNum"
		glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		if (this->_firstRun)
		{
			glTexImage2D(
				GL_TEXTURE_RECTANGLE_NV, 0, GL_LUMINANCE, this->_textureStrides[xplane],
				this->_textureHeights[xplane], 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, this->_textureOffsets[xplane]);

			this->checkGlError("texImage2D");
		}
		else
		{
			glTexSubImage2D(
				GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, this->_textureStrides[xplane], this->_textureHeights[xplane],
				GL_LUMINANCE, GL_UNSIGNED_BYTE, this->_textureOffsets[xplane]);

			this->checkGlError("subImage2D");
		}
	}

	if (this->_firstRun)
	{
		this->_firstRun = false;
	}    
}

/**
    \fn paintGL
*/
void QtGlAccelWidget::paintGL()
{
	this->_glProgram->setUniformValue("texY", 0);
	this->_glProgram->setUniformValue("texU", 2);
	this->_glProgram->setUniformValue("texV", 1);
    this->_glProgram->setUniformValue("height", (float)this->_imageHeight);
    this->checkGlError("setUniformValue");
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2i(0, 0);
	glTexCoord2i(this->_imageWidth, 0);
	glVertex2i(width(), 0);
	glTexCoord2i(this->_imageWidth, this->_imageHeight);
	glVertex2i(this->width(), this->height());
	glTexCoord2i(0, this->_imageHeight);
	glVertex2i(0, this->height());
	glEnd();

    this->checkGlError("draw");
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/**
    \fn ctor
*/
QtGlRender::QtGlRender(void)
{
    ADM_info("Creating GL Renderer\n");
	glWidget = NULL;
}
/**
    \fn dtor
*/
QtGlRender::~QtGlRender(void)
{
    ADM_info("Destroying GL Renderer\n");
	this->stop();
}

/**
    \fn stop
*/

bool QtGlRender::stop(void)
{
	ADM_info("[GL Render] Renderer closed\n");

	if (glWidget)
    {
        glWidget->setParent(NULL);
		delete glWidget;
		glWidget=NULL;
    }

    return true;
}
/**
    \fn init
*/
bool QtGlRender::init(GUI_WindowInfo* window, uint32_t w, uint32_t h, renderZoom zoom)
{
	printf("[GL Render] Initialising renderer\n");
    baseInit(w, h, zoom);

	glWidget = QtGlAccelWidget::create((QWidget*)window->widget, w, h);

    if (glWidget == NULL)
    {
		ADM_warning("[GL Render] Init failed\n");
        return false;
    }

	printf("[GL Render] Setting widget display size to %d x %d\n",imageWidth,imageHeight);
    glWidget->setDisplaySize(displayWidth,displayHeight);
	glWidget->show();

    return true;
}

/**
    \fn displayImage
*/
bool QtGlRender::displayImage(ADMImage *pic)
{
	glWidget->setImage(pic);
	glWidget->repaint();

	return true;
}
/**
    \fn changeZoom
*/
bool QtGlRender::changeZoom(renderZoom newZoom)
{
    ADM_info("changing zoom, qtGl render.\n");

    calcDisplayFromZoom(newZoom);
    currentZoom=newZoom;

	glWidget->setDisplaySize(displayWidth,displayHeight);
    glWidget->repaint();

    return true;
}
/**
    \fn refresh
*/      
bool    QtGlRender::refresh(void)   
{
    glWidget->repaint();
	return true;
}

VideoRenderBase *RenderSpawnQtGl(void)
{
    return new QtGlRender();
}
// EOF
