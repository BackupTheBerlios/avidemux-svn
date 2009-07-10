#include <QtGui/QColorDialog>

#include "ADM_toolkitQt.h"
#include "ADM_default.h"
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
