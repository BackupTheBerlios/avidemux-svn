#include <QtCore/QStack>

#include "ADM_toolkitQt.h"
#include "ADM_assert.h"

static QStack<QWidget*> widgetStack;

void qtRegisterDialog(QWidget *dialog)
{
	if (widgetStack.count())
		dialog->setParent(widgetStack.top(), Qt::Dialog);

	widgetStack.push(dialog);
}

void qtUnregisterDialog(QWidget *dialog)
{
	ADM_assert(widgetStack.top() == dialog);
	widgetStack.pop();
}

QWidget* qtLastRegisteredDialog()
{
	if (widgetStack.count())
		return widgetStack.top();
	else
		return NULL;
}
