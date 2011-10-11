#include "Q_mswin.h"

Q_mswin::Q_mswin()
{
    _taskbar = NULL;
}

void Q_mswin::init(WId wid)
{
    _windowId = wid;
    _taskbarMessageId = RegisterWindowMessageW(L"TaskbarButtonCreated");
}

bool Q_mswin::winEvent(MSG *message, long *result)
{
	bool stopEvent = false;

    if (message->message == _taskbarMessageId)
    {
        *result = CoCreateInstance(
			CLSID_TaskbarList, 0, CLSCTX_INPROC_SERVER,
			IID_ITaskbarList3, reinterpret_cast<void**> (&(_taskbar)));

        stopEvent = true;
    }

    return stopEvent;
}

void Q_mswin::setProgressValue(int value, int max)
{
    if (_taskbar)
	{
		_taskbar->SetProgressValue(_windowId, value, max);
	}
}

void Q_mswin::setProgressState(TBPFLAG state)
{
    if (_taskbar)
	{
		_taskbar->SetProgressState(_windowId, state);
	}
}