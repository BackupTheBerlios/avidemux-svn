#include "ui_equaliser.h"
#include "DIA_flyEqualiser.h"
#include "T_EqualiserPath.h"
#include "ADM_vidEqualizer.h"

class Ui_equaliserWindow : public QDialog
{
	Q_OBJECT

private:
	int lock;
	flyEqualiser *flyDialog;
	ADM_QCanvas *canvas, *histInCanvas, *histOutCanvas;
	EqualiserPath *path;
	int _width, _height;

	void updateDisplay();

public:
	Ui_equaliserWindow(QWidget* parent, EqualizerParam *param, AVDMGenericVideoStream *in);
	~Ui_equaliserWindow();
	void gather(EqualizerParam *param);

	Ui_EqualiserDialog ui;

private slots:
	void sliderUpdate(int value);
	void pointChanged(int pointIndex, int value);

	void slider1Changed(int value);
	void slider2Changed(int value);
	void slider3Changed(int value);
	void slider4Changed(int value);
	void slider5Changed(int value);
	void slider6Changed(int value);
	void slider7Changed(int value);
	void slider8Changed(int value);

	void spinBox1Changed(int value);
	void spinBox2Changed(int value);
	void spinBox3Changed(int value);
	void spinBox4Changed(int value);
	void spinBox5Changed(int value);
	void spinBox6Changed(int value);
	void spinBox7Changed(int value);
	void spinBox8Changed(int value);
};
