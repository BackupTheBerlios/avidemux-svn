/***************************************************************************
                    xvidConfigDialog.cpp  -  description
                    ------------------------------------

                          GUI for configuring Xvid

    begin                : Fri Jun 13 2008
    copyright            : (C) 2008 by mean/gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
***************************************************************************/
#include "xvidConfigDialog.h"

XvidConfigDialog::XvidConfigDialog(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties, vidEncOptions *encodeOptions, XvidOptions *options) :
	QDialog((QWidget*)configParameters->parent, Qt::Dialog)
{
	ui.setupUi(this);
}

extern "C" int showXvidConfigDialog(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties, vidEncOptions *encodeOptions, XvidOptions *options)
{
	XvidConfigDialog dialog(configParameters, properties, encodeOptions, options);

	if (dialog.exec() == QDialog::Accepted)
	{
		//dialog.saveSettings(encodeOptions, options);

		return 1;
	}

	return 0;	
}
