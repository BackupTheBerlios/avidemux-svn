#ifndef Q_gui2_h
#define Q_gui2_h

#include <QtCore/QFileInfo>
#include <QtGui/QSlider>
#include <QtGui/QWidget>
#include <QtGui/QMenu>
#include "ADM_default.h"
#include "ADM_qslider.h"
#include "T_thumbSlider.h"
#include "ui_gui2.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();
	void buildCustomMenu(void);
	void buildAutoMenu(void);

	Ui_MainWindow ui;

public slots:
	void timeChanged(int);
	void buttonPressed(void);
	void custom(void);
	void autoMenuHandler(void);
	void toolButtonPressed(bool z);

	void comboChanged(int z);
	void sliderValueChanged(int u);
	void sliderMoved(int value);
	void sliderReleased(void);
	void thumbSlider_valueEmitted(int value);
	void volumeChange( int u );
	void audioToggled(bool checked);
	void previewModeChanged(QAction *action);
	void previousIntraFrame(void);
	void nextIntraFrame(void);
	void timeChangeFinished(void);
	void currentFrameChanged(void);
	void currentTimeChanged(void);

protected:
	ThumbSlider *thumbSlider;

	void clearCustomMenu(void);
	bool eventFilter(QObject* watched, QEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void closeEvent(QCloseEvent *event);
	void addDirEntryToMenu(QMenu *parentMenu, QString path);
};
#endif	// Q_gui2_h
