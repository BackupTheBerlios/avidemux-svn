#ifndef QT_TOOLKIT_H
#define QT_TOOLKIT_H
#include <QtGui/QWidget>

void UI_purge(void);

void qtRegisterDialog(QWidget *dialog);
void qtUnregisterDialog(QWidget *dialog);
QWidget* qtLastRegisteredDialog();
#endif
