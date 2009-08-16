#ifndef XvidConfigDialog_h
#define XvidConfigDialog_h

#include "ui_xvidConfigDialog.h"
#define ADM_LEGACY_PROGGY
#include "ADM_default.h"
#include "../xvidOptions.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

class XvidConfigDialog : public QDialog
{
	Q_OBJECT

private:
	Ui_XvidConfigDialog ui;
	bool disableGenericSlots;

	static const int aspectRatioCount = 4;
	int predefinedARs[aspectRatioCount][2];

	int lastBitrate, lastVideoSize;
	unsigned char intraMatrix[64], interMatrix[64];

	void fillConfigurationComboBox(void);
	bool selectConfiguration(QString *selectFile, PluginConfigType configurationType);
	bool loadPresetSettings(vidEncOptions *encodeOptions, XvidOptions *options);
	void loadSettings(vidEncOptions *encodeOptions, XvidOptions *options);

public:
	XvidConfigDialog(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties, vidEncOptions *encodeOptions, XvidOptions *options);
	void saveSettings(vidEncOptions *encodeOptions, XvidOptions *options);

private slots:
	void generic_currentIndexChanged(int index);
	void generic_valueChanged(int value);
	void generic_valueChanged(double value);
	void generic_pressed(void);
	void generic_textEdited(QString text);

	void configurationComboBox_currentIndexChanged(int index);
	void saveAsButton_pressed(void);
	void deleteButton_pressed(void);

	// General tab
	void encodingModeComboBox_currentIndexChanged(int index);
	void quantiserSlider_valueChanged(int value);
	void quantiserSpinBox_valueChanged(int value);
	void targetRateControlSpinBox_valueChanged(int value);

	// Quantiser tab
	void matrixCustomEditButton_pressed();
};
#endif	// XvidConfigDialog_h
