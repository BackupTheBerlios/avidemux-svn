#ifndef Q_MPDELOGO_H
#define Q_MPDELOGO_H

#include "ui_mpdelogo.h"
#include "ADM_vidMPdelogo.h"
#include "DIA_flyDialog.h"
#include "DIA_flyMpDelogo.h"

class Ui_mpdelogoWindow : public QDialog
{
	Q_OBJECT

protected:
	int lock;

public:
	flyMpDelogo *myDelogo;
	ADM_QCanvas *canvas;
	Ui_mpdelogoWindow(QWidget* parent, MPDELOGO_PARAM *param, AVDMGenericVideoStream *in);
	~Ui_mpdelogoWindow();
	Ui_MPDelogoDialog ui;

public slots:
	void gather(MPDELOGO_PARAM *param);

private slots:
	void sliderUpdate(int value);
	void valueChanged(int value);
};
#endif
