/***************************************************************************
  FAC_toggle.cpp
  Handle dialog factory element : Toggle
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

#include <config.h>


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <QtGui>

#include "default.h"
#include "ADM_commonUI/DIA_factory.h"
#include "ADM_assert.h"
#include "dialogFactoryQt4.h"

extern const char *shortkey(const char *);

class QCheckBoxReadOnly : public QObject
{
	Q_OBJECT

private:
	QCheckBox *box;
	bool state;

public:
	QCheckBoxReadOnly(QCheckBox *box, bool state)
	{
		this->box = box;
		this->state = state;
	};

public slots:
	void stateChanged(int state)
	{
		box->setCheckState(this->state ? Qt::Checked : Qt::Unchecked);
	};
};

diaElemNotch::diaElemNotch(uint32_t yes,const char *toggleTitle, const char *tip)
  : diaElem(ELEM_NOTCH)
{
  yesno=yes;
  paramTitle=toggleTitle;
  this->tip=tip;
}

diaElemNotch::~diaElemNotch()
{
  
}
void diaElemNotch::setMe(void *dialog, void *opaque,uint32_t line)
{
  QCheckBox *box=new QCheckBox(QString::fromUtf8(paramTitle),(QWidget *)dialog);
  QCheckBoxReadOnly *readOnlyReceiver = new QCheckBoxReadOnly(box, yesno);
 QGridLayout *layout=(QGridLayout*) opaque;
 myWidget=(void *)box; 
 if( yesno)
 {
    box->setCheckState(Qt::Checked); 
 }

 QObject::connect(box, SIGNAL(stateChanged(int)), readOnlyReceiver, SLOT(stateChanged(int)));
 layout->addWidget(box,line,0);
}

int diaElemNotch::getRequiredLayout(void) { return FAC_QT_GRIDLAYOUT; }

//EOF
