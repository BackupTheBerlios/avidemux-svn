#ifndef Q_working_h
#define Q_working_h

#include "ui_working.h"

class workWindow : public QDialog
{
	Q_OBJECT

public:
	workWindow(QWidget *parent);
	Ui_workingDialog ui;
};
#endif	// Q_working_h
