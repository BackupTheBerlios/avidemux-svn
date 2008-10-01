#ifndef T_preview_h
#define T_preview_h

#include <QtGui/QPaintEvent>
#include <QtGui/QWidget>

class PreviewEventReceiver : public QObject
{
	Q_OBJECT

public slots:
	void previewClose();
};

class  ADM_Qvideo : public QWidget
{
	Q_OBJECT

public:
	ADM_Qvideo(QWidget *z);
	~ADM_Qvideo();
	void paintEvent(QPaintEvent *ev);
};
#endif	// T_preview_h
