#include <QtGui>

#include "../ADM_toolkit/qtToolkit.h"

int DIA_colorSel(uint8_t *r, uint8_t *g, uint8_t *b)
{
	QColor initialColor = QColor(*r, *g, *b);
	QColor color = QColorDialog::getColor(initialColor, qtLastRegisteredDialog());

	if (color.isValid())
	{
		*r = color.red();
		*g = color.green();
		*b = color.blue();

		return 1;
	}

	return 0;
}