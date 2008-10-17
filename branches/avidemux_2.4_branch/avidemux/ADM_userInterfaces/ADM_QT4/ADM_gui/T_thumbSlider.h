#ifndef T_THUMBSLIDER_H
#define T_THUMBSLIDER_H

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

#endif
