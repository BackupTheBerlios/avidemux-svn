/***************************************************************************
                          Q_SRT.cpp  -  description
                             -------------------

    Handle the QT specific part of the fontsize & position dialog box
    copyright            : (C) 2002/2007 by mean
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

#define Ui_Dialog Ui_srtDialog
#include "ui_srt.h"
#undef Ui_Dialog

#include "config.h"
#include "ADM_default.h"


#include "ADM_image.h"
#include "ADM_videoFilter.h"

#include "DIA_flyDialog.h"
#include "DIA_flyDialogQt4.h"

//#include "ADM_video/ADM_vidFont.h"
class ADMfont;
#include "ADM_videoFilter/ADM_vidSRT.h"

#include "DIA_flySrtPos.h"

//
//	Video is in YV12 Colorspace
//
//
class Ui_srtWindow : public QDialog
 {
     Q_OBJECT
 protected : 
    int lock;
 public:
     flySrtPos *myCrop;
     ADM_QCanvas *canvas;
     Ui_srtWindow(SRT_POS_PARAM *param,AVDMGenericVideoStream *in);
     ~Ui_srtWindow();
     Ui_srtDialog ui;
 public slots:
      void gather(SRT_POS_PARAM *param);
      //void update(int i);
 private slots:
   void sliderUpdate(int foo);
   void valueChanged(int foo);

 private:
     
 };
  Ui_srtWindow::Ui_srtWindow(SRT_POS_PARAM *param,AVDMGenericVideoStream *in)
  {
    uint32_t width,height;
        ui.setupUi(this);
        lock=0;
        // Allocate space for green-ised video
        width=in->getInfo()->width;
        height=in->getInfo()->height;

        canvas=new ADM_QCanvas(ui.graphicsView,width,height);
        
        myCrop=new flySrtPos( width, height,in,canvas,ui.horizontalSlider);
        memcpy(&(myCrop->param),param,sizeof(SRT_POS_PARAM));
        myCrop->_cookie=&ui;
        myCrop->upload();
        myCrop->sliderChanged();

        ui.verticalSlider->setMaximum(height-1);
        
        connect( ui.horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(sliderUpdate(int)));
        connect( ui.verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
        connect( ui.spinBox,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));  
        
          

  }
  void Ui_srtWindow::sliderUpdate(int foo)
  {
    myCrop->sliderChanged();
  }
  void Ui_srtWindow::gather(SRT_POS_PARAM *param)
  {
    
        myCrop->download();
        memcpy(param,&(myCrop->param),sizeof(SRT_POS_PARAM));
  }
Ui_srtWindow::~Ui_srtWindow()
{
  if(myCrop) delete myCrop;
  myCrop=NULL; 
  if(canvas) delete canvas;
  canvas=NULL;
}
void Ui_srtWindow::valueChanged( int f )
{
  if(lock) return;
  lock++;
  myCrop->update();
  lock--;
}

#define MYSPIN(x) w->x
#define MYCHECK(x) w->checkBox##x
//************************
uint8_t flySrtPos::upload(void)
{
      Ui_srtDialog *w=(Ui_srtDialog *)_cookie;

        MYSPIN(spinBox)->setValue(param.fontSize);
        int32_t max=_h;
        max-=(SRT_MAX_LINE)*param.fontSize;
        if(max<0) max=0;
        if(param.position>=max)
        {
          param.position=max;
        }
        QSlider  *slide=w->verticalSlider;
        slide->setValue(param.position);
        return 1;
}
uint8_t flySrtPos::download(void)
{
       Ui_srtDialog *w=(Ui_srtDialog *)_cookie;
         param.fontSize=MYSPIN(spinBox)->value();
         int32_t max=_h;
        max-=(SRT_MAX_LINE)*param.fontSize;
        if(max<0) max=0;
        
        QSlider  *slide=w->verticalSlider;
        param.position=slide->value();
        
        if(param.position>=max)
        {
          param.position=max;
          upload();
        }
         
return 1;
}

/**
      \fn     DIA_srtPos
      \brief  Handle srt fontsize/position dialog
*/
int DIA_srtPos(AVDMGenericVideoStream *in,uint32_t *size,uint32_t *position)
{
        uint8_t ret=0;
        SRT_POS_PARAM param;
        param.fontSize=*size;
        param.position=*position;
        Ui_srtWindow dialog(&param,in);        
        if(dialog.exec()==QDialog::Accepted)
        {
            dialog.gather(&param);
            *size=param.fontSize;
            *position=param.position;
            ret=1;
        }
        return ret;
}
//____________________________________
// EOF


