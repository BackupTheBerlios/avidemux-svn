#include "ui_coloryuv.h"
#include "ADM_vidColorYuv_param.h"

class ColorYuvWindow : public QDialog
{
	Q_OBJECT

private:
	COLOR_YUV_PARAM *param;
	Ui_ColorYuvDialog ui;

public:
	ColorYuvWindow(QWidget* parent, COLOR_YUV_PARAM *param);
	void gather(void);
};
