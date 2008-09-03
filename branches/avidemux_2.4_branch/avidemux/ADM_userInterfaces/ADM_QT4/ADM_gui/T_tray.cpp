#include <QtGui/QDialog>

extern void UI_deiconify(void);

class ADM_qtray_signalReceiver : public QObject
{
     Q_OBJECT

public:
	QDialog *parent;

public slots:
	void restore(void)
	{
		UI_deiconify();
		parent->showNormal();
	};
};