#ifndef Q_encoding_h
#define Q_encoding_h

#include "ui_encoding.h"

class encodingWindow : public QDialog
{
     Q_OBJECT

 private:
	 bool useTray;

 protected:
	void changeEvent(QEvent *event);

 public:
     encodingWindow(QWidget *parent, bool useTray);
     Ui_encodingDialog ui;

 public slots:
	void buttonPressed(void);
	void priorityChanged(int priorityLevel);
	void shutdownChanged(int state);
};
#endif	// Q_encoding_h
