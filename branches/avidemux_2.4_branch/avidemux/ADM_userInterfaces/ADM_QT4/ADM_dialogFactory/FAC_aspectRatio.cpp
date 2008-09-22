/***************************************************************************
  FAC_aspectRatio.cpp
  Handle dialog factory element : Aspect Ratio
  (C) 2008 Gruntster
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

#include <QDialog>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>

#include "default.h"
#include "ADM_commonUI/DIA_factory.h"
#include "ADM_assert.h"
#include "dialogFactoryQt4.h"

extern const char *shortkey(const char *);

diaElemAspectRatio::diaElemAspectRatio(uint32_t *num, uint32_t *den, const char *title, const char *tip) : diaElem(ELEM_TOGGLE)
{
	param = (void *)num;
	this->den = den;
	paramTitle = shortkey(title);
	this->tip = tip;
}

diaElemAspectRatio::~diaElemAspectRatio()
{
	if(paramTitle)
		delete paramTitle;
}

void diaElemAspectRatio::setMe(void *dialog, void *opaque, uint32_t line)
{
	QLabel *text = new QLabel(QString::fromUtf8(paramTitle));
	QSpinBox *numBox = new QSpinBox();
	QLabel *label = new QLabel(":");
	QSpinBox *denBox = new QSpinBox();
	QGridLayout *layout = (QGridLayout*) opaque;
	QHBoxLayout *hboxLayout = new QHBoxLayout();

	myWidget = (void*)numBox;
	this->label = (void*)label;
	this->denControl = (void*)denBox;

	text->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	text->setBuddy(numBox);

	numBox->setMinimum(1);
	numBox->setMaximum(255);

	denBox->setMinimum(1);
	denBox->setMaximum(255);

	numBox->setValue(*(uint32_t*)param);
	denBox->setValue(*(uint32_t*)den);

	QSpacerItem *spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	hboxLayout->addWidget(numBox);
	hboxLayout->addWidget(label);
	hboxLayout->addWidget(denBox);
	hboxLayout->addItem(spacer);

	layout->addWidget(text,line,0);
	layout->addLayout(hboxLayout,line,1);
}

void diaElemAspectRatio::getMe(void)
{
	*(uint32_t*)param = ((QSpinBox*)myWidget)->value();
	*(uint32_t*)den = ((QSpinBox*)denControl)->value();
}

void diaElemAspectRatio::enable(uint32_t onoff) 
{
	QSpinBox *numBox = (QSpinBox*)myWidget;
	QSpinBox *denBox = (QSpinBox*)denControl;
	QLabel *label = (QLabel*)this->label;

	numBox->setEnabled(onoff);
	denBox->setEnabled(onoff);
	label->setEnabled(onoff);
}

int diaElemAspectRatio::getRequiredLayout(void) { return FAC_QT_GRIDLAYOUT; }
