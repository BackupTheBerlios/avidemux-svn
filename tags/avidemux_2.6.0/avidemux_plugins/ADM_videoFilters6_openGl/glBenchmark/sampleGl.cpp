/** *************************************************************************
                    \fn       openGlFragmentSample.cpp  
                    \brief    simple fragment shader

    This one is combining the 3 textures and apply the shader once


    copyright            : (C) 2011 by mean

bench : 1280*720, null shader, 20 ms, 95% of it in download texture.
            Download Texture
                RGB2Y=5ms               (MMX it)
                toQimage=14 ms  <<==    TOO SLOW

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "ADM_openGL.h"
#define ADM_LEGACY_PROGGY
#include "ADM_default.h"
#include "ADM_coreVideoFilterInternal.h"
#include "T_openGL.h"
#include "T_openGLFilter.h"
#include "sampleGl.h"
#include "ADM_clock.h"


/**
    \class openGlBenchmark
*/
class openGlBenchmark : public  ADM_coreVideoFilterQtGl
{
protected:

protected:
                        bool render(ADMImage *image,ADM_PLANE plane,QGLFramebufferObject *fbo);

public:
                             openGlBenchmark(ADM_coreVideoFilter *previous,CONFcouple *conf);
                            ~openGlBenchmark();

        virtual const char   *getConfiguration(void);                   /// Return  current configuration as a human readable string
        virtual bool         getNextFrame(uint32_t *fn,ADMImage *image);    /// Return the next image
        virtual bool         getCoupledConf(CONFcouple **couples) ;   /// Return the current filter configuration
		virtual void setCoupledConf(CONFcouple *couples);
        virtual bool         configure(void) {return true;}             /// Start graphical user interface
};

// Add the hook to make it valid plugin
DECLARE_VIDEO_FILTER(   openGlBenchmark,   // Class
                        1,0,0,              // Version
                        ADM_UI_QT4+ADM_UI_GL,         // UI
                        VF_OPENGL,            // Category
                        "glBenchmark",            // internal name (must be uniq!)
                        "OpenGl ReadBack benchmark",            // Display name
                        "Check how fast readback is." // Description
                    );

// Now implements the interesting parts
/**
    \fn openGlBenchmark
    \brief constructor
*/
openGlBenchmark::openGlBenchmark(  ADM_coreVideoFilter *in,CONFcouple *setup) : ADM_coreVideoFilterQtGl(in,setup)
{
UNUSED_ARG(setup);
        widget->makeCurrent();
        fboY->bind();
        printf("Compiling shader \n");
        glProgramY = new QGLShaderProgram(context);
        ADM_assert(glProgramY);
        if ( !glProgramY->addShaderFromSourceCode(QGLShader::Fragment, myShaderY))
        {
                ADM_error("[GL Render] Fragment log: %s\n", glProgramY->log().toUtf8().constData());
                ADM_assert(0);
        }
        if ( !glProgramY->link())
        {
            ADM_error("[GL Render] Link log: %s\n", glProgramY->log().toUtf8().constData());
            ADM_assert(0);
        }

        if ( !glProgramY->bind())
        {
                ADM_error("[GL Render] Binding FAILED\n");
                ADM_assert(0);
        }

        fboY->release();
        widget->doneCurrent();

}
/**
    \fn openGlBenchmark
    \brief destructor
*/
openGlBenchmark::~openGlBenchmark()
{
    
}

/**
    \fn getFrame
    \brief Get a processed frame
*/
bool openGlBenchmark::getNextFrame(uint32_t *fn,ADMImage *image)
{
    // since we do nothing, just get the output of previous filter
    if(false==previousFilter->getNextFrame(fn,image))
    {
        ADM_warning("FlipFilter : Cannot get frame\n");
        return false;
    }
    widget->makeCurrent();
    glPushMatrix();
    // size is the last one...
    fboY->bind();
    
    float angle=*fn;
    angle=angle/40;
    glProgramY->setUniformValue("teta", angle);     
    glProgramY->setUniformValue("myTextureU", 1); 
    glProgramY->setUniformValue("myTextureV", 2); 
    glProgramY->setUniformValue("myTextureY", 0); 
    glProgramY->setUniformValue("myWidth", (GLfloat)image->GetWidth(PLANAR_Y)); 
    glProgramY->setUniformValue("myHeight", (GLfloat)image->GetHeight(PLANAR_Y)); 

    uploadAllPlanes(image);

    render(image,PLANAR_Y,fboY);

    ADMBenchmark bench;

    for(int i=0;i<10;i++)
    {
        bench.start();
        downloadTexturesQt(image,fboY);
        bench.end();
    }
    ADMBenchmark bench2;
    for(int i=0;i<10;i++)
    {
        bench2.start();
        downloadTexturesDma(image,fboY);
        bench2.end();
    }
    printf("Qt4 Benchmark\n");
    bench.printResult();
    printf("PBO/FBO Benchmark\n");
    bench2.printResult();

    float avg1,avg2;
    int min1,min2,max1,max2;
    bench.getResult(avg1,min1,max1);
    bench2.getResult(avg2,min2,max2);

    char str1[81];
    char str2[81];

    snprintf(str1,80,"Qt  avg=%03.2f ms, min=%d max=%d ms",avg1,(int)min1,(int)max1);
    snprintf(str2,80,"DMA avg=%03.2f ms, min=%d max=%d ms",avg2,(int)min2,(int)max2);
    image->printString(2,4,str1);
    image->printString(2,8,str2);

    
    fboY->release();
    firstRun=false;
    glPopMatrix();
    widget->doneCurrent();
    
    return true;
}

/**
    \fn getCoupledConf
    \brief Return our current configuration as couple name=value
*/
bool         openGlBenchmark::getCoupledConf(CONFcouple **couples)
{
    *couples=new CONFcouple(0); // Even if we dont have configuration we must allocate one 
    return true;
}

void openGlBenchmark::setCoupledConf(CONFcouple *couples)
{
}
/**
    \fn getConfiguration
    \brief Return current setting as a string
*/
const char *openGlBenchmark::getConfiguration(void)
{
    
    return "openGl Sample.";
}


/**
    \fn render
*/
bool openGlBenchmark::render(ADMImage *image,ADM_PLANE plane,QGLFramebufferObject *fbo)
{
    int width=image->GetWidth(plane);
    int height=image->GetHeight(plane);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);

    //
    glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2i(0, 0);
	glTexCoord2i(width, 0);
	glVertex2i(width, 0);
	glTexCoord2i(width, height);
	glVertex2i(width ,height);
	glTexCoord2i(0, height);
	glVertex2i(0, height);
	glEnd();	// draw cube background
    return true;
}

//EOF
