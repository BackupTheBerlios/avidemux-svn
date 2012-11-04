    /***************************************************************************
  \file T_openGL.h
  \brief OpenGL related filters
  \author (C) 2011 Mean Fixounet@free.fr 
***************************************************************************/
#include "T_openGL.h"
#include "T_openGLFilter.h"
#include "ADM_default.h"
#include "DIA_coreToolkit.h"

bool QGLFilterWidget::initialised()
{
	return this->_activeTexture != NULL;
}

QGLFilterWidget::QGLFilterWidget(QWidget *parent) : QGLWidget(parent)
{
	this->makeCurrent();

	// OpenGL 1.3 core
	this->_activeTexture = reinterpret_cast<PFNGLACTIVETEXTUREPROC>(this->context()->getProcAddress(QLatin1String("glActiveTexture")));
	
	// OpenGL 1.4 extension / OpenGL 1.5 core
	this->_bindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(this->context()->getProcAddress(QLatin1String("glBindBufferARB")));
	this->_bufferData = reinterpret_cast<PFNGLBUFFERDATAARBPROC>(this->context()->getProcAddress(QLatin1String("glBufferDataARB")));
	this->_deleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(this->context()->getProcAddress(QLatin1String("glDeleteBuffersARB")));
	this->_genBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(this->context()->getProcAddress(QLatin1String("glGenBuffersARB")));
	this->_mapBuffer = reinterpret_cast<PFNGLMAPBUFFERPROC>(this->context()->getProcAddress(QLatin1String("glMapBufferARB")));
	this->_unmapBuffer = reinterpret_cast<PFNGLUNMAPBUFFERPROC>(this->context()->getProcAddress(QLatin1String("glUnmapBufferARB")));
}

QGLFilterWidget* QGLFilterWidget::create(QWidget *parent)
{
	QGLFilterWidget* filterWidget = new QGLFilterWidget(parent);

	if (!filterWidget->initialised())
	{
		delete filterWidget;
		filterWidget = NULL;
	}

	return filterWidget;
}

bool QGLFilterWidget::hasArbFunctions()
{
	return
		this->_bindBuffer != NULL && this->_deleteBuffers != NULL &&
		this->_genBuffers != NULL && this->_mapBuffer != NULL && this->_unmapBuffer != NULL &&
		this->_bufferData != NULL;
}

void QGLFilterWidget::activeTexture(GLenum texture)
{
	this->_activeTexture(texture);
}

void QGLFilterWidget::bindBuffer(GLenum target, GLuint buffer)
{
	this->_bindBuffer(target, buffer);
}

void QGLFilterWidget::genBuffers(GLsizei n, GLuint* buffers)
{
	this->_genBuffers(n, buffers);
}

void QGLFilterWidget::deleteBuffers(GLsizei n, const GLuint* buffers)
{
	this->_deleteBuffers(n, buffers);
}

GLvoid* QGLFilterWidget::mapBuffer(GLenum target, GLenum access)
{
	return this->_mapBuffer(target, access);
}

void QGLFilterWidget::unmapBuffer(GLenum target)
{
	this->_unmapBuffer(target);
}

void QGLFilterWidget::bufferData(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage)
{
	this->_bufferData(target, size, data, usage);
}

/**
     \fn checkGlError
     \brief pop an error if an operation failed
*/
bool ADM_coreVideoFilterQtGl::checkGlError(const char *op)
{
    GLenum er=glGetError();
    if(!er) return true;
    ADM_error("[GLERROR]%s: %d => %s\n",op,er,gluErrorString(er));
    return false;
}          

/**
    \fn ctor
*/
ADM_coreVideoFilterQtGl::ADM_coreVideoFilterQtGl(ADM_coreVideoFilter *previous, CONFcouple *conf) : ADM_coreVideoFilter(previous, conf)
{
    bufferARB = 0;
    context = NULL;
    glProgramY = NULL;
    glProgramUV = NULL;
    fboY = NULL;
    fboUV = NULL;

	widget = QGLFilterWidget::create();

	if (widget == NULL)
	{
		return;
	}

    widget->makeCurrent();

    firstRun=0;
    ADM_info("Gl : Allocating context and frameBufferObjects\n");
    context = widget->context();
    ADM_assert(context);

    fboY = new QGLFramebufferObject(info.width, info.height);
    ADM_assert(fboY);
    fboUV = new QGLFramebufferObject(info.width / 2, info.height / 2);
    ADM_assert(fboUV);

    glGenTextures(3, texName);
    checkGlError("GenTex");

    if (widget->hasArbFunctions())
	{
        widget->genBuffers(1, &bufferARB);
	}

    checkGlError("GenBuffer");
    widget->doneCurrent();
    // glTexture TODO
}
/**
    \fn resizeFBO
*/
bool ADM_coreVideoFilterQtGl::resizeFBO(uint32_t w, uint32_t h)
{
    widget->makeCurrent();

    if(fboY)
	{
		delete fboY;
	}

    fboY = new QGLFramebufferObject(w,h);
    widget->doneCurrent();
    checkGlError("resizeFBO");

    return true;
}
/**
    \fn dtor
*/
ADM_coreVideoFilterQtGl::~ADM_coreVideoFilterQtGl()
{
	ADM_info("Gl filter : Destroying..\n");

	if (widget)
	{
		glDeleteTextures(3,texName);
	}

    if (glProgramY)
	{
		delete glProgramY;
		glProgramY = NULL;
	}

    if (glProgramUV)
	{
		delete glProgramUV;
		glProgramUV = NULL;
	}

    if (fboY)
	{
		delete fboY;
		fboY = NULL;
	}

    if (fboUV)
	{
		delete fboUV;
		fboUV = NULL;
	}

	if (this->widget)
	{
		if (this->widget->hasArbFunctions())
		{
			this->widget->deleteBuffers(1,&bufferARB);
		}

		delete widget;
		widget = NULL;
	}

    bufferARB = 0;
}

#define TEX_Y_OFFSET 2
#define TEX_U_OFFSET 1
#define TEX_V_OFFSET 0  
#define TEX_A_OFFSET 3   

/**
    \fn downloadTexture
*/
bool ADM_coreVideoFilterQtGl::downloadTexture(ADMImage *image, ADM_PLANE plane, QGLFramebufferObject *fbo)
{
#ifdef BENCH_READTEXTURE
    {
    ADMBenchmark bench;
    for(int i=0;i<100;i++)
    {
        bench.start();
        QImage qimg(fbo->toImage());
        bench.end();
     }
    ADM_warning("convert to Qimage\n");
    bench.printResult();
    }
#endif

    QImage qimg(fbo->toImage()); // this is slow ! ~ 15 ms for a 720 picture (Y only).

    // Assume RGB32, read R or A
#ifdef BENCH_READTEXTURE
    ADMBenchmark bench;
    for(int i=0;i<100;i++)
    {
        bench.start();
#endif
    int stride = image->GetPitch(plane);
    uint8_t *to = image->GetWritePtr(plane);
    int width = image->GetWidth(plane);
    int height = image->GetHeight(plane);

    for (int y = 0; y < height; y++)
    {
        const uchar *src = qimg.constScanLine(height - y - 1);

        if (!src)
        {
            ADM_error("Can t get pointer to openGl texture\n");
            return false;
        }

        for (int x = 0; x < width; x++)
		{
            to[x] = src[x * 4];
		}

        to += stride;
    }

#ifdef BENCH_READTEXTURE
        bench.end();
    }
    bench.printResult();

#endif

    return true;
}

typedef void typeGlYv444(const uint8_t *src,uint8_t *dst,const int width);

#ifdef ADM_CPU_X86
static inline void glYUV444_MMXInit(void)
{
   static uint64_t __attribute__((used)) FUNNY_MANGLE(mask) = 0x00ff000000ff0000LL;

    __asm__(" movq "Mangle(mask)", %%mm7\n" ::);
}
static inline void glYUV444_MMX(const uint8_t *src, uint8_t *dst, const int width)
{
 
    int count=width/8;
                    __asm__(
                        "1:\n"
                        "movq           (%0),%%mm0 \n"
                        "pand           %%mm7,%%mm0\n"
                        "movq           8(%0),%%mm1 \n"
                        "pand           %%mm7,%%mm1\n"

                        "movq           16(%0),%%mm2 \n"
                        "pand           %%mm7,%%mm2\n"
                        "movq           24(%0),%%mm3 \n"
                        "pand           %%mm7,%%mm3\n"

                        "packuswb       %%mm1,%%mm0\n"
                        "packuswb       %%mm3,%%mm2\n"
                        "psrlw          $8,%%mm0\n"
                        "psrlw          $8,%%mm2\n"
                        "packuswb       %%mm2,%%mm0\n"

                        "movq           %%mm0,(%1)  \n"  
                        "add            $32,%0      \n"
                        "add            $8,%1       \n"
                        "sub            $1,%2        \n"
                        "jnz             1b         \n"
                        
                        :: "r"(src),"r"(dst),"r"(count)
                        );
    if(width&7)
    {
        for(int i=count*8;i<width;i++)
            dst[i]  = src[i*4+TEX_Y_OFFSET];
    }
}
#endif

static inline void glYUV444_C(const uint8_t *src, uint8_t *dst, const int width)
{
	for (int x = 0; x < width; x++)
	{
		dst[x] = src[x * 4 + TEX_Y_OFFSET];
	}
}

bool ADM_coreVideoFilterQtGl::downloadTextures(ADMImage *image, QGLFramebufferObject *fbo)
{
	if (this->widget->hasArbFunctions())
	{
		return downloadTexturesDma(image, fbo);
	}

	return downloadTexturesQt(image, fbo);
}

/**
    \fn downloadTexture
    \brief Download YUVA texture into a YV12 image
*/
bool ADM_coreVideoFilterQtGl::downloadTexturesQt(ADMImage *image,  QGLFramebufferObject *fbo)
{
    QImage qimg(fbo->toImage()); // this is slow ! ~ 15 ms for a 720 picture (Y only).
    // Assume RGB32, read R or A
    int strideY = image->GetPitch(PLANAR_Y);
    uint8_t *toY = image->GetWritePtr(PLANAR_Y);
    uint8_t *toU = image->GetWritePtr(PLANAR_U);
    uint8_t *toV = image->GetWritePtr(PLANAR_V);
    int strideU = image->GetPitch(PLANAR_U);
    int strideV = image->GetPitch(PLANAR_V);
    int width = image->GetWidth(PLANAR_Y);
    int height = image->GetHeight(PLANAR_Y);
    typeGlYv444 *luma = glYUV444_C;

#ifdef ADM_CPU_X86
	if(CpuCaps::hasMMX())
	{
		glYUV444_MMXInit();
		luma = glYUV444_MMX;
	}
#endif

    // Do Y
    for (int y = 1; y <= height; y++)
    {
		const uchar *src = qimg.constScanLine(height - y);

		if (!src)
		{
			ADM_error("Can t get pointer to openGl texture\n");
			return false;
		}

		luma(src, toY, width);
		toY += strideY;

		if (y & 1)
		{
			for (int x = 0; x < width; x += 2) // Stupid subsample: 1 out of 4
			{
				const uchar *p = src + x * 4;
				uint32_t v = *(uint32_t *)p;

				if (!v)
				{
					toU[x / 2] = 128;
					toV[x / 2] = 128;
				}
				else
				{
					toU[x / 2] = p[TEX_U_OFFSET];
					toV[x / 2] = p[TEX_V_OFFSET];
				}
			}

			toU += strideU;
			toV += strideV;
		}
	}

#ifdef ADM_CPU_X86
    __asm__( "emms\n"::  );
#endif

    return true;
}
/**
    \fn downloadTexture
    \brief Download YUVA texture into a YV12 image
*/
bool ADM_coreVideoFilterQtGl::downloadTexturesDma(ADMImage *image,  QGLFramebufferObject *fbo)
{
    int width = image->GetWidth(PLANAR_Y);
    int height = image->GetHeight(PLANAR_Y);
    bool r = true;

	this->widget->bindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
    // that one might fail : checkGlError("BindARB-00");

    this->widget->bindBuffer(GL_PIXEL_PACK_BUFFER_ARB, bufferARB);
    checkGlError("BindARB");
    this->widget->bufferData(GL_PIXEL_PACK_BUFFER_ARB, info.width * info.height * sizeof(uint32_t), NULL, GL_STREAM_READ_ARB);
    checkGlError("BufferDataRB");

    glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
    checkGlError("ReadBuffer (fbo)");
    this->widget->bindBuffer(GL_PIXEL_PACK_BUFFER_ARB, bufferARB);
    checkGlError("Bind Buffer (arb)");

    glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0);
    checkGlError("glReadPixel");

    // DMA call done, we can do something else here
    ADM_usleep(1*1000);

    GLubyte* ptr = (GLubyte*)this->widget->mapBuffer(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);
    checkGlError("MapBuffer");

    if (!ptr)
    {
        ADM_error("Cannot map output buffer!\n");
        r = false;
    }
    else
    {
		// Assume RGB32, read R or A
		int strideY = image->GetPitch(PLANAR_Y);
		uint8_t *toY = image->GetWritePtr(PLANAR_Y);
		uint8_t *toU = image->GetWritePtr(PLANAR_U);
		uint8_t *toV = image->GetWritePtr(PLANAR_V);
		int strideU = image->GetPitch(PLANAR_U);
		int strideV = image->GetPitch(PLANAR_V);
		int width = image->GetWidth(PLANAR_Y);
		int height = image->GetHeight(PLANAR_Y);
		typeGlYv444 *luma = glYUV444_C;

    #ifdef ADM_CPU_X86
          if(CpuCaps::hasMMX())
          {
                glYUV444_MMXInit();
                luma=glYUV444_MMX;
          }
    #endif
        // Do Y
		  for (int y = 0; y < height; y++)
		  {
			  const uchar *src = 4 * width * y + ptr;

			  if (!src)
			  {
				  ADM_error("Can t get pointer to openGl texture\n");
				  return false;
			  }

			  luma(src, toY, width);
			  toY += strideY;

			  if (y & 1)
			  {
				  for(int x = 0; x < width; x += 2) // Stupid subsample: 1 out of 4
				  {
					  const uchar *p = src + x * 4;
					  uint32_t v = *(uint32_t *)p;
					  if (!v)
					  {
						  toU[x / 2] = 128;
						  toV[x / 2] = 128;
					  }
					  else
					  {
						  toU[x / 2] = p[TEX_U_OFFSET];
						  toV[x / 2] = p[TEX_V_OFFSET];
					  }
				  }

				  toU += strideU;
				  toV += strideV;
			  }
		  }

    #ifdef ADM_CPU_X86
        __asm__( "emms\n"::  );
    #endif
        this->widget->unmapBuffer(GL_PIXEL_PACK_BUFFER_ARB);
    }

    this->widget->bindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);

    return r;
}
/**
    \fn uploadTexture
*/
void ADM_coreVideoFilterQtGl::uploadOnePlane(ADMImage *image, ADM_PLANE plane, GLuint tex,int texNum )
{
	this->widget->activeTexture(tex);  // Activate texture unit "tex"
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, texNum); // Use texture "texNum"

	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (!firstRun)
	{
		glTexImage2D(
			GL_TEXTURE_RECTANGLE_NV, 0, GL_LUMINANCE, 
			image->GetPitch(plane),
			image->GetHeight(plane), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 
			image->GetReadPtr(plane));
	}
	else
	{
		glTexSubImage2D(
			GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 
			image->GetPitch(plane),
			image->GetHeight(plane),
			GL_LUMINANCE, GL_UNSIGNED_BYTE, 
			image->GetReadPtr(plane));
	}
}

/**
    \fn uploadTexture
*/
void ADM_coreVideoFilterQtGl::uploadAllPlanes(ADMImage *image)
{
	// Activate texture unit "tex"
	for (int xplane = 2; xplane >= 0; xplane--)
	{
		this->widget->activeTexture(GL_TEXTURE0 + xplane);

		ADM_PLANE plane = (ADM_PLANE)xplane;

		glBindTexture(GL_TEXTURE_RECTANGLE_NV, texName[xplane]); // Use tex engine "texNum"
		glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		if (!firstRun)
		{
			glTexImage2D(
				GL_TEXTURE_RECTANGLE_NV, 0, GL_LUMINANCE, 
				image->GetPitch(plane),
				image->GetHeight(plane), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 
				image->GetReadPtr(plane));
		}
		else
		{
			glTexSubImage2D(
				GL_TEXTURE_RECTANGLE_NV, 0, 0, 0, 
				image->GetPitch(plane),
				image->GetHeight(plane),
				GL_LUMINANCE, GL_UNSIGNED_BYTE, 
				image->GetReadPtr(plane));
		}
	}
}

// EOF
