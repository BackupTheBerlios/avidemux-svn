#include "ui_cnr2.h"
#include "ADM_default.h"
#include "ADM_vidCNR2_param.h"

class Cnr2Window : public QDialog
{
	Q_OBJECT

private:
	CNR2Param *param;
	Ui_Cnr2Dialog ui;

private slots:
	void sceneChangeSlider_valueChanged(int value);
	void sceneChangeSpinBox_valueChanged(double value);

public:
	Cnr2Window(QWidget* parent, CNR2Param *param);
	void gather(void);
};
