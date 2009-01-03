#ifndef XvidConfigDialog_h
#define XvidConfigDialog_h

#include "ui_xvidConfigDialog.h"
#include "../options.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

class XvidConfigDialog : public QDialog
{
	Q_OBJECT

private:
	Ui_XvidConfigDialog ui;

public:
	XvidConfigDialog(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties, vidEncOptions *encodeOptions, XvidOptions *options);
};
#endif	// XvidConfigDialog_h
