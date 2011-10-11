#ifndef Q_MSWIN_H
#define Q_MSWIN_H

#include <QtGui/QWidget>
#include <shobjidl.h>

class Q_mswin
{
public:
	Q_mswin();
	void init(WId wid);
	bool winEvent(MSG * message, long * result);
	void setProgressValue(int value, int max);
	void setProgressState(TBPFLAG state);

private:
	ITaskbarList3 *_taskbar;
	UINT _taskbarMessageId;
	WId _windowId;
};
#endif