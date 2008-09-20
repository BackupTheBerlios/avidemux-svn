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


#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>

#include "ADM_default.h"
#include "DIA_factory.h"

extern const char *shortkey(const char *);


namespace ADM_qt4Factory
{
class diaElemFrame : public diaElemFrameBase
{
  
public:
  
  diaElemFrame(const char *toggleTitle, const char *tip=NULL);
  virtual ~diaElemFrame() ;
  void setMe(void *dialog, void *opaque,uint32_t line);
  void getMe(void) ;
  void swallow(diaElem *widget);
  void enable(uint32_t onoff);
  void finalize(void);
};

diaElemFrame::diaElemFrame(const char *toggleTitle, const char *tip)
  : diaElemFrameBase()
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
	QGridLayout *layout = (QGridLayout*)opaque;
	QGroupBox *groupBox = new QGroupBox(QString::fromUtf8(paramTitle));
	QGridLayout *layout2 = new QGridLayout(groupBox);
	int v = 0;

	for (int i = 0; i < nbElems; i++)
	{
		elems[i]->setMe(groupBox, layout2, v); 
		v += elems[i]->getSize();
	}

	layout->addWidget(groupBox, line, 0);
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
} // End of namespace
//****************************Hoook*****************

diaElem  *qt4CreateFrame(const char *toggleTitle, const char *tip)
{
	return new  ADM_qt4Factory::diaElemFrame(toggleTitle,tip);
}
void qt4DestroyFrame(diaElem *e)
{
	ADM_qt4Factory::diaElemFrame *a=(ADM_qt4Factory::diaElemFrame *)e;
	delete a;
}
//EOF
