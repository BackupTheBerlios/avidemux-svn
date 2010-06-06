/***************************************************************************
                 xvidCustomMatrixDialog.cpp  -  description
                 ------------------------------------------

                      GUI for configuring Xvid matrix

    begin                : Thu Jan 08 2009
    copyright            : (C) 2009 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "xvidCustomMatrixDialog.h"
#include "ADM_default.h"
#include "DIA_fileSel.h"
#include "DIA_coreToolkit.h"

XvidCustomMatrixDialog::XvidCustomMatrixDialog(QWidget *parent, const unsigned char intra8x8Luma[64], 
											   const unsigned char inter8x8Luma[64]) : QDialog(parent, Qt::Dialog)
{
	ui.setupUi(this);

	connect(ui.loadFileButton, SIGNAL(pressed()), this, SLOT(loadFileButton_pressed()));

	setIntra8x8Luma(intra8x8Luma);
	setInter8x8Luma(inter8x8Luma);
}

void XvidCustomMatrixDialog::loadFileButton_pressed()
{
	char cqmFileName[1024];
	unsigned char intra8x8Luma[64], inter8x8Luma[64];

	if (FileSel_SelectRead(tr("Select Matrix File").toUtf8().constData(), cqmFileName, 1023, NULL) && ADM_fileExist(cqmFileName))
	{
		if (parseCqmFile(cqmFileName, intra8x8Luma, inter8x8Luma) == 0)
		{
			setIntra8x8Luma(intra8x8Luma);
			setInter8x8Luma(inter8x8Luma);
		}
		else
			GUI_Error_HIG(tr("Read Error").toUtf8().constData(), tr("Error reading custom matrix file.").toUtf8().constData());
	}
}

bool XvidCustomMatrixDialog::parseCqmFile(const char cqmFileName[1024], unsigned char intra8x8Luma[64], unsigned char inter8x8Luma[64])
{
	FILE *file = fopen(cqmFileName, "rb");
	bool result = false;

	if (file)
	{
		if (fread(intra8x8Luma, 1, 64, file) == 64)
		{
			if (fread(inter8x8Luma, 1, 64, file) != 64)
				result = false;
		}
		else
			result = false;

		fclose(file);
	}

	return result;
}

void XvidCustomMatrixDialog::getMatrix(unsigned char intra8x8Luma[64], unsigned char inter8x8Luma[64])
{
	intra8x8Luma[0] = ui.intra8x8Luma1SpinBox->value();
	intra8x8Luma[1] = ui.intra8x8Luma2SpinBox->value();
	intra8x8Luma[2] = ui.intra8x8Luma3SpinBox->value();
	intra8x8Luma[3] = ui.intra8x8Luma4SpinBox->value();
	intra8x8Luma[4] = ui.intra8x8Luma5SpinBox->value();
	intra8x8Luma[5] = ui.intra8x8Luma6SpinBox->value();
	intra8x8Luma[6] = ui.intra8x8Luma7SpinBox->value();
	intra8x8Luma[7] = ui.intra8x8Luma8SpinBox->value();
	intra8x8Luma[8] = ui.intra8x8Luma9SpinBox->value();
	intra8x8Luma[9] = ui.intra8x8Luma10SpinBox->value();
	intra8x8Luma[10] = ui.intra8x8Luma11SpinBox->value();
	intra8x8Luma[11] = ui.intra8x8Luma12SpinBox->value();
	intra8x8Luma[12] = ui.intra8x8Luma13SpinBox->value();
	intra8x8Luma[13] = ui.intra8x8Luma14SpinBox->value();
	intra8x8Luma[14] = ui.intra8x8Luma15SpinBox->value();
	intra8x8Luma[15] = ui.intra8x8Luma16SpinBox->value();
	intra8x8Luma[16] = ui.intra8x8Luma17SpinBox->value();
	intra8x8Luma[17] = ui.intra8x8Luma18SpinBox->value();
	intra8x8Luma[18] = ui.intra8x8Luma19SpinBox->value();
	intra8x8Luma[19] = ui.intra8x8Luma20SpinBox->value();
	intra8x8Luma[20] = ui.intra8x8Luma21SpinBox->value();
	intra8x8Luma[21] = ui.intra8x8Luma22SpinBox->value();
	intra8x8Luma[22] = ui.intra8x8Luma23SpinBox->value();
	intra8x8Luma[23] = ui.intra8x8Luma24SpinBox->value();
	intra8x8Luma[24] = ui.intra8x8Luma25SpinBox->value();
	intra8x8Luma[25] = ui.intra8x8Luma26SpinBox->value();
	intra8x8Luma[26] = ui.intra8x8Luma27SpinBox->value();
	intra8x8Luma[27] = ui.intra8x8Luma28SpinBox->value();
	intra8x8Luma[28] = ui.intra8x8Luma29SpinBox->value();
	intra8x8Luma[29] = ui.intra8x8Luma30SpinBox->value();
	intra8x8Luma[30] = ui.intra8x8Luma31SpinBox->value();
	intra8x8Luma[31] = ui.intra8x8Luma32SpinBox->value();
	intra8x8Luma[32] = ui.intra8x8Luma33SpinBox->value();
	intra8x8Luma[33] = ui.intra8x8Luma34SpinBox->value();
	intra8x8Luma[34] = ui.intra8x8Luma35SpinBox->value();
	intra8x8Luma[35] = ui.intra8x8Luma36SpinBox->value();
	intra8x8Luma[36] = ui.intra8x8Luma37SpinBox->value();
	intra8x8Luma[37] = ui.intra8x8Luma38SpinBox->value();
	intra8x8Luma[38] = ui.intra8x8Luma39SpinBox->value();
	intra8x8Luma[39] = ui.intra8x8Luma40SpinBox->value();
	intra8x8Luma[40] = ui.intra8x8Luma41SpinBox->value();
	intra8x8Luma[41] = ui.intra8x8Luma42SpinBox->value();
	intra8x8Luma[42] = ui.intra8x8Luma43SpinBox->value();
	intra8x8Luma[43] = ui.intra8x8Luma44SpinBox->value();
	intra8x8Luma[44] = ui.intra8x8Luma45SpinBox->value();
	intra8x8Luma[45] = ui.intra8x8Luma46SpinBox->value();
	intra8x8Luma[46] = ui.intra8x8Luma47SpinBox->value();
	intra8x8Luma[47] = ui.intra8x8Luma48SpinBox->value();
	intra8x8Luma[48] = ui.intra8x8Luma49SpinBox->value();
	intra8x8Luma[49] = ui.intra8x8Luma50SpinBox->value();
	intra8x8Luma[50] = ui.intra8x8Luma51SpinBox->value();
	intra8x8Luma[51] = ui.intra8x8Luma52SpinBox->value();
	intra8x8Luma[52] = ui.intra8x8Luma53SpinBox->value();
	intra8x8Luma[53] = ui.intra8x8Luma54SpinBox->value();
	intra8x8Luma[54] = ui.intra8x8Luma55SpinBox->value();
	intra8x8Luma[55] = ui.intra8x8Luma56SpinBox->value();
	intra8x8Luma[56] = ui.intra8x8Luma57SpinBox->value();
	intra8x8Luma[57] = ui.intra8x8Luma58SpinBox->value();
	intra8x8Luma[58] = ui.intra8x8Luma59SpinBox->value();
	intra8x8Luma[59] = ui.intra8x8Luma60SpinBox->value();
	intra8x8Luma[60] = ui.intra8x8Luma61SpinBox->value();
	intra8x8Luma[61] = ui.intra8x8Luma62SpinBox->value();
	intra8x8Luma[62] = ui.intra8x8Luma63SpinBox->value();
	intra8x8Luma[63] = ui.intra8x8Luma64SpinBox->value();

	inter8x8Luma[0] = ui.inter8x8Luma1SpinBox->value();
	inter8x8Luma[1] = ui.inter8x8Luma2SpinBox->value();
	inter8x8Luma[2] = ui.inter8x8Luma3SpinBox->value();
	inter8x8Luma[3] = ui.inter8x8Luma4SpinBox->value();
	inter8x8Luma[4] = ui.inter8x8Luma5SpinBox->value();
	inter8x8Luma[5] = ui.inter8x8Luma6SpinBox->value();
	inter8x8Luma[6] = ui.inter8x8Luma7SpinBox->value();
	inter8x8Luma[7] = ui.inter8x8Luma8SpinBox->value();
	inter8x8Luma[8] = ui.inter8x8Luma9SpinBox->value();
	inter8x8Luma[9] = ui.inter8x8Luma10SpinBox->value();
	inter8x8Luma[10] = ui.inter8x8Luma11SpinBox->value();
	inter8x8Luma[11] = ui.inter8x8Luma12SpinBox->value();
	inter8x8Luma[12] = ui.inter8x8Luma13SpinBox->value();
	inter8x8Luma[13] = ui.inter8x8Luma14SpinBox->value();
	inter8x8Luma[14] = ui.inter8x8Luma15SpinBox->value();
	inter8x8Luma[15] = ui.inter8x8Luma16SpinBox->value();
	inter8x8Luma[16] = ui.inter8x8Luma17SpinBox->value();
	inter8x8Luma[17] = ui.inter8x8Luma18SpinBox->value();
	inter8x8Luma[18] = ui.inter8x8Luma19SpinBox->value();
	inter8x8Luma[19] = ui.inter8x8Luma20SpinBox->value();
	inter8x8Luma[20] = ui.inter8x8Luma21SpinBox->value();
	inter8x8Luma[21] = ui.inter8x8Luma22SpinBox->value();
	inter8x8Luma[22] = ui.inter8x8Luma23SpinBox->value();
	inter8x8Luma[23] = ui.inter8x8Luma24SpinBox->value();
	inter8x8Luma[24] = ui.inter8x8Luma25SpinBox->value();
	inter8x8Luma[25] = ui.inter8x8Luma26SpinBox->value();
	inter8x8Luma[26] = ui.inter8x8Luma27SpinBox->value();
	inter8x8Luma[27] = ui.inter8x8Luma28SpinBox->value();
	inter8x8Luma[28] = ui.inter8x8Luma29SpinBox->value();
	inter8x8Luma[29] = ui.inter8x8Luma30SpinBox->value();
	inter8x8Luma[30] = ui.inter8x8Luma31SpinBox->value();
	inter8x8Luma[31] = ui.inter8x8Luma32SpinBox->value();
	inter8x8Luma[32] = ui.inter8x8Luma33SpinBox->value();
	inter8x8Luma[33] = ui.inter8x8Luma34SpinBox->value();
	inter8x8Luma[34] = ui.inter8x8Luma35SpinBox->value();
	inter8x8Luma[35] = ui.inter8x8Luma36SpinBox->value();
	inter8x8Luma[36] = ui.inter8x8Luma37SpinBox->value();
	inter8x8Luma[37] = ui.inter8x8Luma38SpinBox->value();
	inter8x8Luma[38] = ui.inter8x8Luma39SpinBox->value();
	inter8x8Luma[39] = ui.inter8x8Luma40SpinBox->value();
	inter8x8Luma[40] = ui.inter8x8Luma41SpinBox->value();
	inter8x8Luma[41] = ui.inter8x8Luma42SpinBox->value();
	inter8x8Luma[42] = ui.inter8x8Luma43SpinBox->value();
	inter8x8Luma[43] = ui.inter8x8Luma44SpinBox->value();
	inter8x8Luma[44] = ui.inter8x8Luma45SpinBox->value();
	inter8x8Luma[45] = ui.inter8x8Luma46SpinBox->value();
	inter8x8Luma[46] = ui.inter8x8Luma47SpinBox->value();
	inter8x8Luma[47] = ui.inter8x8Luma48SpinBox->value();
	inter8x8Luma[48] = ui.inter8x8Luma49SpinBox->value();
	inter8x8Luma[49] = ui.inter8x8Luma50SpinBox->value();
	inter8x8Luma[50] = ui.inter8x8Luma51SpinBox->value();
	inter8x8Luma[51] = ui.inter8x8Luma52SpinBox->value();
	inter8x8Luma[52] = ui.inter8x8Luma53SpinBox->value();
	inter8x8Luma[53] = ui.inter8x8Luma54SpinBox->value();
	inter8x8Luma[54] = ui.inter8x8Luma55SpinBox->value();
	inter8x8Luma[55] = ui.inter8x8Luma56SpinBox->value();
	inter8x8Luma[56] = ui.inter8x8Luma57SpinBox->value();
	inter8x8Luma[57] = ui.inter8x8Luma58SpinBox->value();
	inter8x8Luma[58] = ui.inter8x8Luma59SpinBox->value();
	inter8x8Luma[59] = ui.inter8x8Luma60SpinBox->value();
	inter8x8Luma[60] = ui.inter8x8Luma61SpinBox->value();
	inter8x8Luma[61] = ui.inter8x8Luma62SpinBox->value();
	inter8x8Luma[62] = ui.inter8x8Luma63SpinBox->value();
	inter8x8Luma[63] = ui.inter8x8Luma64SpinBox->value();
}

void XvidCustomMatrixDialog::setIntra8x8Luma(const unsigned char intra8x8Luma[64])
{
	ui.intra8x8Luma1SpinBox->setValue(intra8x8Luma[0]);
	ui.intra8x8Luma2SpinBox->setValue(intra8x8Luma[1]);
	ui.intra8x8Luma3SpinBox->setValue(intra8x8Luma[2]);
	ui.intra8x8Luma4SpinBox->setValue(intra8x8Luma[3]);
	ui.intra8x8Luma5SpinBox->setValue(intra8x8Luma[4]);
	ui.intra8x8Luma6SpinBox->setValue(intra8x8Luma[5]);
	ui.intra8x8Luma7SpinBox->setValue(intra8x8Luma[6]);
	ui.intra8x8Luma8SpinBox->setValue(intra8x8Luma[7]);
	ui.intra8x8Luma9SpinBox->setValue(intra8x8Luma[8]);
	ui.intra8x8Luma10SpinBox->setValue(intra8x8Luma[9]);
	ui.intra8x8Luma11SpinBox->setValue(intra8x8Luma[10]);
	ui.intra8x8Luma12SpinBox->setValue(intra8x8Luma[11]);
	ui.intra8x8Luma13SpinBox->setValue(intra8x8Luma[12]);
	ui.intra8x8Luma14SpinBox->setValue(intra8x8Luma[13]);
	ui.intra8x8Luma15SpinBox->setValue(intra8x8Luma[14]);
	ui.intra8x8Luma16SpinBox->setValue(intra8x8Luma[15]);
	ui.intra8x8Luma17SpinBox->setValue(intra8x8Luma[16]);
	ui.intra8x8Luma18SpinBox->setValue(intra8x8Luma[17]);
	ui.intra8x8Luma19SpinBox->setValue(intra8x8Luma[18]);
	ui.intra8x8Luma20SpinBox->setValue(intra8x8Luma[19]);
	ui.intra8x8Luma21SpinBox->setValue(intra8x8Luma[20]);
	ui.intra8x8Luma22SpinBox->setValue(intra8x8Luma[21]);
	ui.intra8x8Luma23SpinBox->setValue(intra8x8Luma[22]);
	ui.intra8x8Luma24SpinBox->setValue(intra8x8Luma[23]);
	ui.intra8x8Luma25SpinBox->setValue(intra8x8Luma[24]);
	ui.intra8x8Luma26SpinBox->setValue(intra8x8Luma[25]);
	ui.intra8x8Luma27SpinBox->setValue(intra8x8Luma[26]);
	ui.intra8x8Luma28SpinBox->setValue(intra8x8Luma[27]);
	ui.intra8x8Luma29SpinBox->setValue(intra8x8Luma[28]);
	ui.intra8x8Luma30SpinBox->setValue(intra8x8Luma[29]);
	ui.intra8x8Luma31SpinBox->setValue(intra8x8Luma[30]);
	ui.intra8x8Luma32SpinBox->setValue(intra8x8Luma[31]);
	ui.intra8x8Luma33SpinBox->setValue(intra8x8Luma[32]);
	ui.intra8x8Luma34SpinBox->setValue(intra8x8Luma[33]);
	ui.intra8x8Luma35SpinBox->setValue(intra8x8Luma[34]);
	ui.intra8x8Luma36SpinBox->setValue(intra8x8Luma[35]);
	ui.intra8x8Luma37SpinBox->setValue(intra8x8Luma[36]);
	ui.intra8x8Luma38SpinBox->setValue(intra8x8Luma[37]);
	ui.intra8x8Luma39SpinBox->setValue(intra8x8Luma[38]);
	ui.intra8x8Luma40SpinBox->setValue(intra8x8Luma[39]);
	ui.intra8x8Luma41SpinBox->setValue(intra8x8Luma[40]);
	ui.intra8x8Luma42SpinBox->setValue(intra8x8Luma[41]);
	ui.intra8x8Luma43SpinBox->setValue(intra8x8Luma[42]);
	ui.intra8x8Luma44SpinBox->setValue(intra8x8Luma[43]);
	ui.intra8x8Luma45SpinBox->setValue(intra8x8Luma[44]);
	ui.intra8x8Luma46SpinBox->setValue(intra8x8Luma[45]);
	ui.intra8x8Luma47SpinBox->setValue(intra8x8Luma[46]);
	ui.intra8x8Luma48SpinBox->setValue(intra8x8Luma[47]);
	ui.intra8x8Luma49SpinBox->setValue(intra8x8Luma[48]);
	ui.intra8x8Luma50SpinBox->setValue(intra8x8Luma[49]);
	ui.intra8x8Luma51SpinBox->setValue(intra8x8Luma[50]);
	ui.intra8x8Luma52SpinBox->setValue(intra8x8Luma[51]);
	ui.intra8x8Luma53SpinBox->setValue(intra8x8Luma[52]);
	ui.intra8x8Luma54SpinBox->setValue(intra8x8Luma[53]);
	ui.intra8x8Luma55SpinBox->setValue(intra8x8Luma[54]);
	ui.intra8x8Luma56SpinBox->setValue(intra8x8Luma[55]);
	ui.intra8x8Luma57SpinBox->setValue(intra8x8Luma[56]);
	ui.intra8x8Luma58SpinBox->setValue(intra8x8Luma[57]);
	ui.intra8x8Luma59SpinBox->setValue(intra8x8Luma[58]);
	ui.intra8x8Luma60SpinBox->setValue(intra8x8Luma[59]);
	ui.intra8x8Luma61SpinBox->setValue(intra8x8Luma[60]);
	ui.intra8x8Luma62SpinBox->setValue(intra8x8Luma[61]);
	ui.intra8x8Luma63SpinBox->setValue(intra8x8Luma[62]);
	ui.intra8x8Luma64SpinBox->setValue(intra8x8Luma[63]);
}

void XvidCustomMatrixDialog::setInter8x8Luma(const unsigned char inter8x8Luma[64])
{
	ui.inter8x8Luma1SpinBox->setValue(inter8x8Luma[0]);
	ui.inter8x8Luma2SpinBox->setValue(inter8x8Luma[1]);
	ui.inter8x8Luma3SpinBox->setValue(inter8x8Luma[2]);
	ui.inter8x8Luma4SpinBox->setValue(inter8x8Luma[3]);
	ui.inter8x8Luma5SpinBox->setValue(inter8x8Luma[4]);
	ui.inter8x8Luma6SpinBox->setValue(inter8x8Luma[5]);
	ui.inter8x8Luma7SpinBox->setValue(inter8x8Luma[6]);
	ui.inter8x8Luma8SpinBox->setValue(inter8x8Luma[7]);
	ui.inter8x8Luma9SpinBox->setValue(inter8x8Luma[8]);
	ui.inter8x8Luma10SpinBox->setValue(inter8x8Luma[9]);
	ui.inter8x8Luma11SpinBox->setValue(inter8x8Luma[10]);
	ui.inter8x8Luma12SpinBox->setValue(inter8x8Luma[11]);
	ui.inter8x8Luma13SpinBox->setValue(inter8x8Luma[12]);
	ui.inter8x8Luma14SpinBox->setValue(inter8x8Luma[13]);
	ui.inter8x8Luma15SpinBox->setValue(inter8x8Luma[14]);
	ui.inter8x8Luma16SpinBox->setValue(inter8x8Luma[15]);
	ui.inter8x8Luma17SpinBox->setValue(inter8x8Luma[16]);
	ui.inter8x8Luma18SpinBox->setValue(inter8x8Luma[17]);
	ui.inter8x8Luma19SpinBox->setValue(inter8x8Luma[18]);
	ui.inter8x8Luma20SpinBox->setValue(inter8x8Luma[19]);
	ui.inter8x8Luma21SpinBox->setValue(inter8x8Luma[20]);
	ui.inter8x8Luma22SpinBox->setValue(inter8x8Luma[21]);
	ui.inter8x8Luma23SpinBox->setValue(inter8x8Luma[22]);
	ui.inter8x8Luma24SpinBox->setValue(inter8x8Luma[23]);
	ui.inter8x8Luma25SpinBox->setValue(inter8x8Luma[24]);
	ui.inter8x8Luma26SpinBox->setValue(inter8x8Luma[25]);
	ui.inter8x8Luma27SpinBox->setValue(inter8x8Luma[26]);
	ui.inter8x8Luma28SpinBox->setValue(inter8x8Luma[27]);
	ui.inter8x8Luma29SpinBox->setValue(inter8x8Luma[28]);
	ui.inter8x8Luma30SpinBox->setValue(inter8x8Luma[29]);
	ui.inter8x8Luma31SpinBox->setValue(inter8x8Luma[30]);
	ui.inter8x8Luma32SpinBox->setValue(inter8x8Luma[31]);
	ui.inter8x8Luma33SpinBox->setValue(inter8x8Luma[32]);
	ui.inter8x8Luma34SpinBox->setValue(inter8x8Luma[33]);
	ui.inter8x8Luma35SpinBox->setValue(inter8x8Luma[34]);
	ui.inter8x8Luma36SpinBox->setValue(inter8x8Luma[35]);
	ui.inter8x8Luma37SpinBox->setValue(inter8x8Luma[36]);
	ui.inter8x8Luma38SpinBox->setValue(inter8x8Luma[37]);
	ui.inter8x8Luma39SpinBox->setValue(inter8x8Luma[38]);
	ui.inter8x8Luma40SpinBox->setValue(inter8x8Luma[39]);
	ui.inter8x8Luma41SpinBox->setValue(inter8x8Luma[40]);
	ui.inter8x8Luma42SpinBox->setValue(inter8x8Luma[41]);
	ui.inter8x8Luma43SpinBox->setValue(inter8x8Luma[42]);
	ui.inter8x8Luma44SpinBox->setValue(inter8x8Luma[43]);
	ui.inter8x8Luma45SpinBox->setValue(inter8x8Luma[44]);
	ui.inter8x8Luma46SpinBox->setValue(inter8x8Luma[45]);
	ui.inter8x8Luma47SpinBox->setValue(inter8x8Luma[46]);
	ui.inter8x8Luma48SpinBox->setValue(inter8x8Luma[47]);
	ui.inter8x8Luma49SpinBox->setValue(inter8x8Luma[48]);
	ui.inter8x8Luma50SpinBox->setValue(inter8x8Luma[49]);
	ui.inter8x8Luma51SpinBox->setValue(inter8x8Luma[50]);
	ui.inter8x8Luma52SpinBox->setValue(inter8x8Luma[51]);
	ui.inter8x8Luma53SpinBox->setValue(inter8x8Luma[52]);
	ui.inter8x8Luma54SpinBox->setValue(inter8x8Luma[53]);
	ui.inter8x8Luma55SpinBox->setValue(inter8x8Luma[54]);
	ui.inter8x8Luma56SpinBox->setValue(inter8x8Luma[55]);
	ui.inter8x8Luma57SpinBox->setValue(inter8x8Luma[56]);
	ui.inter8x8Luma58SpinBox->setValue(inter8x8Luma[57]);
	ui.inter8x8Luma59SpinBox->setValue(inter8x8Luma[58]);
	ui.inter8x8Luma60SpinBox->setValue(inter8x8Luma[59]);
	ui.inter8x8Luma61SpinBox->setValue(inter8x8Luma[60]);
	ui.inter8x8Luma62SpinBox->setValue(inter8x8Luma[61]);
	ui.inter8x8Luma63SpinBox->setValue(inter8x8Luma[62]);
	ui.inter8x8Luma64SpinBox->setValue(inter8x8Luma[63]);
}
