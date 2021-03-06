/***************************************************************************
  DIA_dialogFactory.cpp
  (C) 2007 Mean Fixounet@free.fr 
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
#include <stdlib.h>
#include <math.h>

#include <QDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTabWidget>

#include "ADM_default.h"
#include "ADM_commonUI/DIA_factory.h"
#include "ADM_assert.h"

class factoryWindow : public QDialog
{
     Q_OBJECT

 public:
     factoryWindow();
 public slots:
 private slots:
 private:
};

static void insertTab(uint32_t index, diaElemTabs *tab, QTabWidget *wtab);
factoryWindow::factoryWindow()     : QDialog()
 {
//     ui.setupUi(this);
     //connect( (ui.pushButton),SIGNAL(pressed()),this,SLOT(buttonPressed()));
 }


/**
    \fn diaFactoryRun(const char *title,uint32_t nb,diaElem **elems)
    \brief  Run a dialog made of nb elems, each elem being described in the **elems
    @return 0 on failure, 1 on success
*/

uint8_t diaFactoryRun(const char *title,uint32_t nb,diaElem **elems)
{
  factoryWindow dialog;
  
  ADM_assert(title);
  ADM_assert(nb);
  ADM_assert(elems);
  
  dialog.setWindowTitle(QString::fromUtf8(title));
  
  QGridLayout layout(&dialog);
  
  /* First compute the size of our window */
  int vsize=0;
  for(int i=0;i<nb;i++)
  {
    ADM_assert(elems[i]);
     vsize+=elems[i]->getSize(); 
  }

 int  v=0;
  for(int i=0;i<nb;i++)
  {
    ADM_assert(elems[i]);
     elems[i]->setMe( (void *)&dialog,&layout,v); 
     v+=elems[i]->getSize();
    
  }
   for(int i=0;i<nb;i++)
  {
    ADM_assert(elems[i]);
     elems[i]->finalize(); 
  }
  // Add buttons
   QDialogButtonBox buttonBox((QWidget *)&dialog);
    buttonBox.setStandardButtons(QDialogButtonBox::Ok
                            | QDialogButtonBox::Cancel);
     QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
     QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
     layout.addWidget(&buttonBox,vsize,0);
  // run
  dialog.setLayout(&layout);
  if(dialog.exec()==QDialog::Accepted)
  {
     for(int i=0;i<nb;i++)
     {
        ADM_assert(elems[i]);
        elems[i]->getMe(); 
    
      }
    return 1;
  }
  return 0;
  
}
/**
    \fn shortkey(const char *in)
    \brief translate gtk like accelerator (_) to QT4 like accelerator (&)
*/
const char *shortkey(const char *in)
{
 char *t=ADM_strdup(in);
 for(int i=0;i<strlen(t);i++)
 {
    if(t[i]=='_') t[i]='&'; 
 }
  return t;
}
uint8_t diaFactoryRunTabs(const char *title,uint32_t nb,diaElemTabs **tabs)
{
    factoryWindow dialog;
  
  ADM_assert(title);
  ADM_assert(nb);
  ADM_assert(tabs);
  
  dialog.setWindowTitle(QString::fromUtf8(title));
  
  QGridLayout layout(&dialog);

  // Add tabs
  QTabWidget wtabs((QWidget *)&dialog);
  // Add buttons
   QDialogButtonBox buttonBox((QWidget *)&dialog);
    buttonBox.setStandardButtons(QDialogButtonBox::Ok
                            | QDialogButtonBox::Cancel);
     QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
     QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
     
     for(int i=0;i<nb;i++)
     {
        ADM_assert(tabs[i]);
        insertTab(i,tabs[i],&wtabs); 
    
      }
     
     layout.addWidget(&wtabs,0,0);
     layout.addWidget(&buttonBox,1,0);
  // run
  dialog.setLayout(&layout);
  if(dialog.exec()==QDialog::Accepted)
  {
      // Read tabs
       for(int tab=0;tab<nb;tab++)
     {
        ADM_assert(tabs[tab]);
        diaElemTabs *myTab=tabs[tab];
        for(int i=0;i<myTab->nbElems;i++)
        {
          myTab->dias[i]->getMe();
        }
    
      }
      return 1;
  }
  return 0;
  
}

void insertTab(uint32_t index, diaElemTabs *tab, QTabWidget *wtab)
{

  QWidget *wid=new QWidget;
  QGridLayout *layout=new QGridLayout(wid);
  
  /* First compute the size of our window */
  int vsize=0;
  for(int i=0;i<tab->nbElems;i++)
  {
    ADM_assert(tab->dias[i]);
     vsize+=tab->dias[i]->getSize(); 
  }

 int  v=0;
  for(int i=0;i<tab->nbElems;i++)
  {
     ADM_assert(tab->dias[i]);
     tab->dias[i]->setMe( wid,layout,v); 
     v+=tab->dias[i]->getSize();
    
  }
  
  wtab->addTab(wid,QString::fromUtf8(tab->title));
  for(int i=0;i<tab->nbElems;i++)
  {
    tab->dias[i]->finalize(); 
  }
}
//EOF
