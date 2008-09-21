/***************************************************************************
                                Q_about.cpp
                                -----------

    begin                : Fri May 5 2008
    copyright            : (C) 2008 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ui_about.h"
#include "default.h"
#include "../ADM_toolkit/qtToolkit.h"

extern uint8_t DIA_license(void);

class Ui_aboutWindow : public QDialog
{
	Q_OBJECT

private:
	Ui_aboutDialog ui;

public:
	Ui_aboutWindow(QWidget* parent);

private slots:
	void licenseButton_clicked(bool);
};

Ui_aboutWindow::Ui_aboutWindow(QWidget* parent) : QDialog(parent)
{
	Q_INIT_RESOURCE(about);

	ui.setupUi(this);

	connect(ui.licenseButton, SIGNAL(clicked(bool)), this, SLOT(licenseButton_clicked(bool)));

	char subversion[20];

	if (!ADM_SUBVERSION)
		strcpy(subversion, VERSION);
	else
		sprintf(subversion,"%s (r%04u)", VERSION, ADM_SUBVERSION);

	ui.versionLabel->setText(ui.versionLabel->text() + subversion);
}

void Ui_aboutWindow::licenseButton_clicked(bool)
{
	DIA_license();
}

uint8_t DIA_about(void)
{	
	Ui_aboutWindow dialog(qtLastRegisteredDialog());
	qtRegisterDialog(&dialog);

	dialog.exec();

	qtUnregisterDialog(&dialog);

	return 1;
}
