#ifndef Q_gui2_h
#define Q_gui2_h

#define MKICON(x) ":/new/prefix1/pics/"#x".png"

#include <vector>
#include <QtCore/QUrl>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

#include "ADM_qslider.h"
#include "T_thumbSlider.h"
#include "ui_gui2.h"
#include "gui_action.hxx"
#include "myOwnMenu.h"
#include "IScriptEngine.h"
#include "prefs.h"

/**
    \class MainWindow
*/
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	Ui_MainWindow ui;

	MainWindow(const std::vector<IScriptEngine*>& scriptEngines);
	virtual ~MainWindow();

	void buildCustomMenu(void);
	void buildRecentMenu(void);
	void buildRecentProjectMenu(void);

protected:
    QMenu *jsMenu;
    QMenu *pyMenu;
    QMenu *autoMenu;
    QMenu *recentFiles;
    QMenu *recentProjects;
    QAction *recentFileAction[NB_LAST_FILES];
	QAction *recentProjectAction[NB_LAST_FILES];
    ThumbSlider *thumbSlider;

public slots:
	void timeChanged(int);
        void checkChanged(int);
	void buttonPressed(void);
	void toolButtonPressed(bool z);

	void comboChanged(int z);
	void sliderValueChanged(int u);
	void sliderMoved(int value);
	void sliderReleased(void);
	void volumeChange( int u );
	void audioToggled(bool checked);
	void previewModeChanged(QAction *action);
	void previousIntraFrame(void);
	void nextIntraFrame(void);
	void timeChangeFinished(void);
	void currentFrameChanged(void);
	void currentTimeChanged(void);

    void thumbSlider_valueEmitted(int value);

    void searchFileMenu(QAction * action);
    void searchEditMenu(QAction * action);
    void searchVideoMenu(QAction * action);
    void searchAudioMenu(QAction * action);
    void searchHelpMenu(QAction * action);
    void searchToolMenu(QAction * action);
    void searchViewMenu(QAction * action);
    void searchGoMenu(QAction * action);
    void searchRecentFiles(QAction * action);
    void searchRecentProjects(QAction * action);
    void searchToolBar(QAction *);

	void scriptFileActionHandler();
	void scriptReferenceActionHandler();

protected:
	const std::vector<IScriptEngine*>& _scriptEngines;

	void addScriptDirToMenu(QMenu* scriptMenu, const QString& dir, const QStringList& fileExts);
	void addScriptEnginesToFileMenu(std::vector<MenuEntry>& fileMenu);
	void addScriptShellsToToolsMenu(vector<MenuEntry>& toolMenu);
	void addScriptReferencesToHelpMenu();
        bool buildMyMenu(void);
        bool buildMenu(QMenu *root,MenuEntry *menu, int nb);
	void buildRecentMenu(QMenu *menu, const char **files, QAction **actions);
        void searchMenu(QAction * action,MenuEntry *menu, int nb);
	void searchRecentFiles(QAction *action, QAction **actionList, int firstEventId);
	bool eventFilter(QObject* watched, QEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void openFiles(QList<QUrl>);
};
#endif	// Q_gui2_h
