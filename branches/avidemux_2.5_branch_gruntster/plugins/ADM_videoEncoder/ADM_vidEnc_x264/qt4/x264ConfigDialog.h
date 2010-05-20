#ifndef x264ConfigDialog_h
#define x264ConfigDialog_h
#include "ui_x264ConfigDialog.h"
#define ADM_LEGACY_PROGGY
#include "ADM_default.h"
#include "../x264Options.h"
#include "x264ZoneTableModel.h"
#include "x264ZoneTableDelegate.h"

extern "C"
{
#include "ADM_vidEnc_plugin.h"
}

class x264ConfigDialog : public QDialog
{
	Q_OBJECT

private:
	Ui_x264ConfigDialog ui;
	x264ZoneTableModel zoneTableModel;
	x264ZoneTableDelegate zoneTableDelegate;
	bool disableGenericSlots;

	static const int aspectRatioCount = 4;
	int predefinedARs[aspectRatioCount][2];

	static const int idcLevelCount = 16;
	int8_t idcLevel[idcLevelCount];

	static const int videoFormatCount = 6;
	uint8_t videoFormat[videoFormatCount];

	static const int colourPrimariesCount = 7;
	uint8_t colourPrimaries[colourPrimariesCount];

	static const int transferCharacteristicsCount = 9;
	uint8_t transferCharacteristics[transferCharacteristicsCount];

	static const int colourMatrixCount = 8;
	uint8_t colourMatrix[colourMatrixCount];

	int lastBitrate, lastVideoSize;
	uint8_t intra4x4Luma[16], intraChroma[16];
	uint8_t inter4x4Luma[16], interChroma[16];
	uint8_t intra8x8Luma[64], inter8x8Luma[64];

	void fillConfigurationComboBox(void);
	bool selectConfiguration(QString *selectFile, PluginConfigType configurationType);
	bool loadPresetSettings(vidEncOptions *encodeOptions, x264Options *options);
	void loadSettings(vidEncOptions *encodeOptions, x264Options *options);
	int getValueIndexInArray(uint8_t value, const uint8_t valueArray[], int elementCount);
	int getValueIndexInArray(int8_t value, const int8_t valueArray[], int elementCount);

public:
	x264ConfigDialog(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties, vidEncOptions *encodeOptions, x264Options *options);
	void saveSettings(vidEncOptions *encodeOptions, x264Options *options);

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
	void mbTreeCheckBox_toggled(bool checked);

	// Motion Estimation tab
	void meSlider_valueChanged(int value);
	void meSpinBox_valueChanged(int value);
	void dct8x8CheckBox_toggled(bool checked);
	void p8x8CheckBox_toggled(bool checked);

	// Frame tab
	void loopFilterCheckBox_toggled(bool checked);
	void cabacCheckBox_toggled(bool checked);

	// Analysis tab
	void trellisCheckBox_toggled(bool checked);
	void matrixCustomEditButton_pressed();

	// Quantiser tab
	void aqVarianceCheckBox_toggled(bool checked);

	// Advanced Rate Control tab
	void zoneAddButton_pressed();
	void zoneEditButton_pressed();
	void zoneDeleteButton_pressed();
};
#endif	// x264ConfigDialog_h
