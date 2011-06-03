#ifndef Q_working_h
#define Q_working_h

#include "ui_working.h"
#include "ADM_default.h"
#include "DIA_working.h"

class workWindow : public QDialog
{
	Q_OBJECT

private:
	DIA_working *_working;

public:
	workWindow(QWidget *parent, DIA_working *working);
	Ui_workingDialog ui;

private slots:
	void closeDialog(void);
};
#endif	// Q_working_h
