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

#include <config.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "ui_vobsub.h"

#include "default.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_toolkit/filesel.h"
#include "ADM_image.h"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_colorspace/ADM_rgb.h"

#include "ADM_videoFilter/ADM_vobsubinfo.h"
#include "ADM_videoFilter/ADM_vidVobSub.h"

#include "ADM_assert.h"



//
//	Video is in YV12 Colorspace
//
//
class Ui_vobsubWindow : public QDialog
 {
     Q_OBJECT
 protected :
	 vobSubParam *param;
	 void  		 fillLanguage(const char *file);
 public:
     Ui_vobsubWindow(vobSubParam *param);
     ~Ui_vobsubWindow();
     Ui_DialogVobSub ui;
     
     void  gather(void);
 public slots:
 private slots:
 		void idxSel(bool i);
 private:
     
 };
  Ui_vobsubWindow::Ui_vobsubWindow(vobSubParam *parm)
  {
    uint32_t width,height;
    	this->param=parm;
        ui.setupUi(this);
        connect( ui.pushButtonIdx,SIGNAL(clicked(bool)),this,SLOT(idxSel(bool)));
        // Set what was there...
        if(param->subname)
        {
    		ui.lineEditIdx->setText(param->subname);
    		fillLanguage(param->subname);
    		ui.comboBoxLanguage->setCurrentIndex(param->index);
        }
        
  }
  /**
   * 	\fn gather
   * 	\brief Collect info from dialog and fill the param struct with it
   */
void  Ui_vobsubWindow::gather(void)
{
	if(param->subname) ADM_dezalloc(param->subname);
	param->subname=NULL;
	param->subname=ADM_strdup(ui.lineEditIdx->text().toAscii().data());
	param->index=ui.comboBoxLanguage->currentIndex();
}
/**
 * 	\fn fillLanguage
 *  \brief Update the language combobox with what's in the idx file
 *  @param file : idx file (in)
 */
void  Ui_vobsubWindow::fillLanguage(const char *file)
{
	// Grab info also
			 vobSubLanguage *lang=NULL;
			 QComboBox *combo=(ui.comboBoxLanguage);
			 
			  combo->clear();
			  lang=vobSubAllocateLanguage();
			    
			  if(vobSubGetLanguage(file,lang))
			  {
			      // add them
			    for(int i=0;i<lang->nbLanguage;i++)
			    {
			    		combo->addItem(lang->language[i].name);
			    }
			  }
			    // Destroy
			  vobSubDestroyLanguage(lang);
			  // Reset index
			  ui.comboBoxLanguage->setCurrentIndex(0);
}
Ui_vobsubWindow::~Ui_vobsubWindow()
{
}

void Ui_vobsubWindow::idxSel(bool i)
{
	char fileIdx[1024];
	
	if(FileSel_SelectRead(_("Select Idx File"),fileIdx,1023, NULL) && ADM_fileExist(fileIdx))
	{
		// Set entry
		ui.lineEditIdx->setText(fileIdx);
		fillLanguage(fileIdx);
	}
}


/**
      \fn     DIA_vobsub
      \brief  Dialog to select vobsub file, language and shift. 
*/
uint8_t DIA_vobsub(vobSubParam *param)
{
        uint8_t ret=0;
        Ui_vobsubWindow dialog(param);        
        if(dialog.exec()==QDialog::Accepted)
        {
        	dialog.gather();
            ret=1;
        }
        return ret;
}
//____________________________________
// EOF


