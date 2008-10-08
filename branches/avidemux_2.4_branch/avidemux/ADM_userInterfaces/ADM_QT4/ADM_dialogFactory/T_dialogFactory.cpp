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

#include "default.h"
#include "ADM_commonUI/DIA_factory.h"
#include "ADM_assert.h"
#include "../ADM_toolkit/qtToolkit.h"
#include "dialogFactoryQt4.h"

static void insertTab(uint32_t index, diaElemTabs *tab, QTabWidget *wtab);

/**
    \fn diaFactoryRun(const char *title,uint32_t nb,diaElem **elems)
    \brief  Run a dialog made of nb elems, each elem being described in the **elems
    @return 0 on failure, 1 on success
*/

uint8_t diaFactoryRun(const char *title,uint32_t nb,diaElem **elems)
{
  QDialog dialog(qtLastRegisteredDialog());
  qtRegisterDialog(&dialog);
  
  ADM_assert(title);
  ADM_assert(nb);
  ADM_assert(elems);
  
  dialog.setWindowTitle(QString::fromUtf8(title));
  
  QSpacerItem *spacer = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);
  QDialogButtonBox *buttonBox = new QDialogButtonBox();
  QVBoxLayout *vboxLayout = new QVBoxLayout();
  QLayout *layout = NULL;
  int currentLayout = 0;

 int  v=0;

 for(int i=0;i<nb;i++)
 {
	 ADM_assert(elems[i]);

	 if (elems[i]->getRequiredLayout() != currentLayout)
	 {
		 if (layout)
			 vboxLayout->addLayout(layout);

		 switch (elems[i]->getRequiredLayout())
		 {
			 case FAC_QT_GRIDLAYOUT:
				 layout = new QGridLayout();
				 break;
			 case FAC_QT_VBOXLAYOUT:
				 layout = new QVBoxLayout();
				 break;
		 }

		 currentLayout = elems[i]->getRequiredLayout();
		 v = 0;
	 }

	 elems[i]->setMe( (void *)&dialog,layout,v);
	 v+=elems[i]->getSize();
 }

   for(int i=0;i<nb;i++)
  {
    ADM_assert(elems[i]);
     elems[i]->finalize(); 
  }

  // Add buttons
   buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

   QObject::connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
   QObject::connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

   if (layout)
	   vboxLayout->addLayout(layout);

   vboxLayout->addItem(spacer);
   vboxLayout->addWidget(buttonBox);

   dialog.setLayout(vboxLayout);

  if(dialog.exec()==QDialog::Accepted)
  {
     for(int i=0;i<nb;i++)
     {
        ADM_assert(elems[i]);
        elems[i]->getMe(); 
    
      }

	 qtUnregisterDialog(&dialog);

    return 1;
  }

  qtUnregisterDialog(&dialog);

  return 0;
  
}
/**
    \fn shortkey(const char *in)
    \brief translate gtk like accelerator (_) to QT4 like accelerator (&)
*/
const char *shortkey(const char *in)
{
	QString escaped = QString(in);

	escaped.replace("&", "&&");
	escaped.replace("_", "&");

	return ADM_strdup(escaped.toUtf8().constData());
}

uint8_t diaFactoryRunTabs(const char *title,uint32_t nb,diaElemTabs **tabs)
{
    QDialog dialog(qtLastRegisteredDialog());

	qtRegisterDialog(&dialog);
  
  ADM_assert(title);
  ADM_assert(nb);
  ADM_assert(tabs);
  
  dialog.setWindowTitle(QString::fromUtf8(title));

  QVBoxLayout *vboxLayout = new QVBoxLayout();
  QGridLayout *layout = new QGridLayout();
  QSpacerItem *spacer = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);
  QTabWidget *wtabs = new QTabWidget();
  QDialogButtonBox *buttonBox = new QDialogButtonBox();

    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

     QObject::connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
     QObject::connect(buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

     for(int i=0;i<nb;i++)
     {
        ADM_assert(tabs[i]);
        insertTab(i,tabs[i],wtabs);
      }

	 vboxLayout->addLayout(layout);
     vboxLayout->addWidget(wtabs,0,0);
	 vboxLayout->addItem(spacer);
     vboxLayout->addWidget(buttonBox,1,0);

	 dialog.setLayout(vboxLayout);

	 // Expand to see all tabs but still allow the window to be resized smaller
	 wtabs->setUsesScrollButtons(false);
	 dialog.adjustSize();
	 wtabs->setUsesScrollButtons(true);

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
	   qtUnregisterDialog(&dialog);

      return 1;
  }

  qtUnregisterDialog(&dialog);

  return 0;
  
}

void insertTab(uint32_t index, diaElemTabs *tab, QTabWidget *wtab)
{

  QWidget *wid=new QWidget;  
  QSpacerItem *spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  int currentLayout = 0;
  QVBoxLayout *vboxLayout = new QVBoxLayout(wid);
  QLayout *layout = NULL;
  
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

		if (tab->dias[i]->getRequiredLayout() != currentLayout)
		{
			if (layout)
				vboxLayout->addLayout(layout);

			switch (tab->dias[i]->getRequiredLayout())
			{
				case FAC_QT_GRIDLAYOUT:
					layout = new QGridLayout();
					break;
				case FAC_QT_VBOXLAYOUT:
					layout = new QVBoxLayout();
					break;
			}

			currentLayout = tab->dias[i]->getRequiredLayout();
			v = 0;
		}

		tab->dias[i]->setMe( wid,layout,v); 
		v+=tab->dias[i]->getSize();
	}
  
  wtab->addTab(wid,QString::fromUtf8(tab->title));
  for(int i=0;i<tab->nbElems;i++)
  {
    tab->dias[i]->finalize(); 
  }

  if (layout)
	  vboxLayout->addLayout(layout);

  vboxLayout->addItem(spacerItem);
}
//EOF
