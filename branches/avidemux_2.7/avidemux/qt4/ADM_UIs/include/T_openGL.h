/***************************************************************************
  \file T_openGL.h
  \brief OpenGL related filters
  \author (C) 2011 Mean Fixounet@free.fr 
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef T_OPENGL_H
#define T_OPENGL_H

#include "ADM_UIQT46_export.h"

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#	include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#	include <OpenGL/glext.h>

#	define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_RECTANGLE_EXT

typedef void (* PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (* PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (* PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef void (* PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef GLvoid* (* PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean (* PFNGLUNMAPBUFFERPROC) (GLenum target);
typedef void (* PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
#else
#	ifdef _WIN32
#		include <windows.h>
#	endif
#	include <GL/gl.h>
#   include <GL/glu.h>
#	include <GL/glext.h>
#endif

#include <QtOpenGL/QtOpenGL>

class QGLFilterWidget : public QGLWidget
{
private:
	PFNGLACTIVETEXTUREPROC _activeTexture;
	PFNGLBINDBUFFERPROC    _bindBuffer;
	PFNGLDELETEBUFFERSPROC _deleteBuffers;
	PFNGLGENBUFFERSPROC    _genBuffers;
	PFNGLMAPBUFFERPROC     _mapBuffer;
	PFNGLUNMAPBUFFERPROC   _unmapBuffer;
	PFNGLBUFFERDATAARBPROC _bufferData;

	bool initialised();
	QGLFilterWidget(QWidget *parent = 0);

public:
	static QGLFilterWidget* create(QWidget *parent = 0);
	bool hasArbFunctions();
	void activeTexture(GLenum texture);
	void bindBuffer(GLenum target, GLuint buffer);
	void genBuffers(GLsizei n, GLuint* buffers);
	void deleteBuffers(GLsizei n, const GLuint* buffers);
	GLvoid* mapBuffer(GLenum target, GLenum access);
	void unmapBuffer(GLenum target);
	void bufferData(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
};

#endif
