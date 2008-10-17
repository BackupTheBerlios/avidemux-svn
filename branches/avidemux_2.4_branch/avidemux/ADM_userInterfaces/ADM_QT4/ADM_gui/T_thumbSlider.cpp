//#include "T_thumbSlider.h"

#include <QtGui/QSlider>

class ThumbSlider : public QSlider
{
	Q_OBJECT

private:
	int timerId, count, lock;

public:
	ThumbSlider(QWidget *parent = 0);

protected:
	void timerEvent(QTimerEvent *event);

private slots:
	void sliderPressed();
	void sliderReleased();

signals:
	void valueChanged(int value);
};


ThumbSlider::ThumbSlider(QWidget *parent) : QSlider(parent)
{
	setMaximum(100);
	setMinimum(-100);
	setValue(0);

	count = 0;
	lock = 0;

	connect(this, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
	connect(this, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
}

void ThumbSlider::timerEvent(QTimerEvent *event)
{
	static const int jogScale[10] = {20, 15, 11, 8, 6, 5, 4, 3, 2, 0};
	int r = abs(value()) / 10;

	if (!r)
		return;

	if (count)
		count--;

	if (count)
		return;

	count = (r > 9 ? jogScale[9] : jogScale[r]);

	if (lock)
		return;

	lock++;

	emit valueChanged(value());

	lock--;
}

void ThumbSlider::sliderPressed()
{
	timerId = startTimer(20);
}

void ThumbSlider::sliderReleased()
{
	killTimer(timerId);
	setValue(0);

	count = 0;
}
