/***************************************************************************
  FAC_frame.cpp
  Handle dialog factory element : Frame
  (C) 2006 Mean Fixounet@free.fr 
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <QDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QGroupBox>

#include "default.h"
#include "ADM_commonUI/DIA_factory.h"
#include "ADM_assert.h"
#include "dialogFactoryQt4.h"

extern const char *shortkey(const char *);

diaElemFrame::diaElemFrame(const char *toggleTitle, const char *tip)
  : diaElem(ELEM_FRAME)
{
  param=NULL;
  paramTitle=shortkey(toggleTitle);
  this->tip=tip;
   nbElems=0;
  frameSize=0;
  setSize(2);
}
void diaElemFrame::swallow(diaElem *widget)
{
   elems[nbElems]=widget;
  frameSize+=widget->getSize();
 // setSize(frameSize);
  nbElems++;
  ADM_assert(nbElems<DIA_MAX_FRAME); 
}
diaElemFrame::~diaElemFrame()
{
  if(paramTitle)
    delete paramTitle;
}

void diaElemFrame::setMe(void *dialog, void *opaque,uint32_t line)
{
	QVBoxLayout *layout = (QVBoxLayout*)opaque;
	QGroupBox *groupBox = new QGroupBox(QString::fromUtf8(paramTitle));
	QVBoxLayout *vboxlayout = new QVBoxLayout(groupBox);
	QLayout *layout2 = NULL;
	int currentLayout = 0;
	int v;

	for (int i = 0; i < nbElems; i++)
	{
		if (elems[i]->getRequiredLayout() != currentLayout)
		{
			if (layout2)
				vboxlayout->addLayout(layout2);

			switch (elems[i]->getRequiredLayout())
			{
				case FAC_QT_GRIDLAYOUT:
					layout2 = new QGridLayout();
					break;
				case FAC_QT_VBOXLAYOUT:
					layout2 = new QVBoxLayout();
					break;
			}

			currentLayout = elems[i]->getRequiredLayout();
			v = 0;
		}

		elems[i]->setMe(groupBox, layout2, v); 
		v += elems[i]->getSize();
	}

	if (layout2)
		vboxlayout->addLayout(layout2);

	layout->addWidget(groupBox);
}

//*****************************
void diaElemFrame::getMe(void)
{
   for(int i=0;i<nbElems;i++)
  {
    elems[i]->getMe(); 
  }
}
void diaElemFrame::finalize(void)
{
   for(int i=0;i<nbElems;i++)
  {
    elems[i]->finalize(); 
  }
}
void diaElemFrame::enable(uint32_t onoff)
{
  
}

int diaElemFrame::getRequiredLayout(void) { return FAC_QT_VBOXLAYOUT; }

//******************************************************

//EOF
