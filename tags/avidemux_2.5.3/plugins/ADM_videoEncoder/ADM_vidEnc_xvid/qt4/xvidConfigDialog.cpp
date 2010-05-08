/***************************************************************************
                    xvidConfigDialog.cpp  -  description
                    ------------------------------------

                          GUI for configuring Xvid

    begin                : Fri Jun 13 2008
    copyright            : (C) 2008 by mean/gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
***************************************************************************/
#include <math.h>
#include <QtGui/QFileDialog>

#include "../config.h"
#include "xvidConfigDialog.h"
#include "xvidCustomMatrixDialog.h"

#include "ADM_files.h"
#include "DIA_coreToolkit.h"

// Stay away from ADM_assert.h since it hacks memory functions.
// Duplicating ADM_mkdir declaration here instead.
extern "C" {
	extern uint8_t ADM_mkdir(const char *name);
}

XvidConfigDialog::XvidConfigDialog(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties, vidEncOptions *encodeOptions, XvidOptions *options) :
	QDialog((QWidget*)configParameters->parent, Qt::Dialog)
{
	disableGenericSlots = false;
	static const int _predefinedARs[aspectRatioCount][2] = {{16, 15}, {64, 45}, {8, 9}, {32, 27}};

	memcpy(predefinedARs, _predefinedARs, sizeof(predefinedARs));

	ui.setupUi(this);

	connect(ui.configurationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(configurationComboBox_currentIndexChanged(int)));
	connect(ui.saveAsButton, SIGNAL(pressed()), this, SLOT(saveAsButton_pressed()));
	connect(ui.deleteButton, SIGNAL(pressed()), this, SLOT(deleteButton_pressed()));

	// General tab
	lastBitrate = 1500;
	lastVideoSize = 700;

	connect(ui.encodingModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(encodingModeComboBox_currentIndexChanged(int)));
	connect(ui.quantiserSlider, SIGNAL(valueChanged(int)), this, SLOT(quantiserSlider_valueChanged(int)));
	connect(ui.quantiserSpinBox, SIGNAL(valueChanged(int)), this, SLOT(quantiserSpinBox_valueChanged(int)));
	connect(ui.targetRateControlSpinBox, SIGNAL(valueChanged(int)), this, SLOT(targetRateControlSpinBox_valueChanged(int)));

	ui.sarAsInputLabel->setText(QString("%1:%2").arg(properties->parWidth).arg(properties->parHeight));

	// Quantiser tab
	connect(ui.matrixCustomEditButton, SIGNAL(pressed()), this, SLOT(matrixCustomEditButton_pressed()));

	QWidgetList widgetList = QApplication::allWidgets();

	for (int widgetIndex = 0; widgetIndex < widgetList.size(); widgetIndex++)
	{
		QWidget *widget = widgetList.at(widgetIndex);

		if (widget->parentWidget() != NULL && widget->parentWidget()->parentWidget() != NULL && 
			widget->parentWidget()->parentWidget()->parentWidget() != NULL &&
			widget->parentWidget()->parentWidget()->parentWidget()->parentWidget() == ui.tabWidget)
		{
			if (widget->inherits("QComboBox"))
				connect(widget, SIGNAL(currentIndexChanged(int)), this, SLOT(generic_currentIndexChanged(int)));
			else if (widget->inherits("QSpinBox"))
				connect(widget, SIGNAL(valueChanged(int)), this, SLOT(generic_valueChanged(int)));
			else if (widget->inherits("QDoubleSpinBox"))
				connect(widget, SIGNAL(valueChanged(double)), this, SLOT(generic_valueChanged(double)));
			else if (widget->inherits("QCheckBox"))
				connect(widget, SIGNAL(pressed()), this, SLOT(generic_pressed()));
			else if (widget->inherits("QRadioButton"))
				connect(widget, SIGNAL(pressed()), this, SLOT(generic_pressed()));
			else if (widget->inherits("QLineEdit"))
				connect(widget, SIGNAL(textEdited(QString)), this, SLOT(generic_textEdited(QString)));
		}
	}

	fillConfigurationComboBox();

	if (!loadPresetSettings(encodeOptions, options))
		loadSettings(encodeOptions, options);
}

void XvidConfigDialog::fillConfigurationComboBox(void)
{
	XvidOptions options;
	bool origDisableGenericSlots = disableGenericSlots;
	QMap<QString, int> configs;
	QStringList filter("*.xml");
	char* configDir = options.getUserConfigDirectory();
	QStringList list = QDir(configDir).entryList(filter, QDir::Files | QDir::Readable);

	delete [] configDir;
	disableGenericSlots = true;

	for (int item = 0; item < list.size(); item++)
		configs.insert(QFileInfo(list[item]).completeBaseName(), PLUGIN_CONFIG_USER);

	configDir = options.getSystemConfigDirectory();
	list = QDir(configDir).entryList(filter, QDir::Files | QDir::Readable);

	delete [] configDir;

	for (int item = 0; item < list.size(); item++)
		configs.insert(QFileInfo(list[item]).completeBaseName(), PLUGIN_CONFIG_SYSTEM);

	ui.configurationComboBox->clear();
	ui.configurationComboBox->addItem(QT_TR_NOOP("<default>"), PLUGIN_CONFIG_DEFAULT);
	ui.configurationComboBox->addItem(QT_TR_NOOP("<custom>"), PLUGIN_CONFIG_CUSTOM);

	QMap<QString, int>::const_iterator mapIterator = configs.constBegin();

	while (mapIterator != configs.constEnd())
	{
		ui.configurationComboBox->addItem(mapIterator.key(), mapIterator.value());
		mapIterator++;
	}

	disableGenericSlots = origDisableGenericSlots;
}

bool XvidConfigDialog::selectConfiguration(QString *selectFile, PluginConfigType configurationType)
{
	bool success = false;
	bool origDisableGenericSlots = disableGenericSlots;

	disableGenericSlots = true;

	if (configurationType == PLUGIN_CONFIG_DEFAULT)
	{
		ui.configurationComboBox->setCurrentIndex(0);
		success = true;
	}
	else
	{
		for (int index = 0; index < ui.configurationComboBox->count(); index++)
		{
			if (ui.configurationComboBox->itemText(index) == selectFile && ui.configurationComboBox->itemData(index).toInt() == configurationType)
			{
				ui.configurationComboBox->setCurrentIndex(index);
				success = true;
				break;
			}
		}

		if (!success)
			ui.configurationComboBox->setCurrentIndex(1);
	}

	disableGenericSlots = origDisableGenericSlots;

	return success;
}

void XvidConfigDialog::configurationComboBox_currentIndexChanged(int index)
{
	bool origDisableGenericSlots = disableGenericSlots;

	disableGenericSlots = true;

	if (index == 0)		// default
	{
		ui.deleteButton->setEnabled(false);

		XvidOptions defaultOptions;
		vidEncOptions *defaultEncodeOptions = defaultOptions.getEncodeOptions();

		loadSettings(defaultEncodeOptions, &defaultOptions);

		delete defaultEncodeOptions;
	}
	else if (index == 1)	// custom
		ui.deleteButton->setEnabled(false);
	else
	{
		PluginConfigType configType = (PluginConfigType)ui.configurationComboBox->itemData(index).toInt();

		ui.deleteButton->setEnabled(configType == PLUGIN_CONFIG_USER);

		XvidOptions options;
		vidEncOptions *encodeOptions;

		options.setPresetConfiguration(ui.configurationComboBox->itemText(index).toUtf8().constData(), configType);

		if (options.loadPresetConfiguration())
		{
			encodeOptions = options.getEncodeOptions();

			loadSettings(encodeOptions, &options);

			delete encodeOptions;
		}
		else
			ui.configurationComboBox->setCurrentIndex(0);
	}

	disableGenericSlots = origDisableGenericSlots;
}

void XvidConfigDialog::saveAsButton_pressed(void)
{
	char *configDirectory = ADM_getHomeRelativePath("xvid");

	ADM_mkdir(configDirectory);

	QString configFileName = QFileDialog::getSaveFileName(this, QT_TR_NOOP("Save As"), configDirectory, QT_TR_NOOP("Xvid Configuration File (*.xml)"));

	if (!configFileName.isNull())
	{
		QFile configFile(configFileName);
		vidEncOptions encodeOptions;
		XvidOptions presetOptions;

		configFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
		saveSettings(&encodeOptions, &presetOptions);
		presetOptions.setEncodeOptions(&encodeOptions);

		char* xml = presetOptions.toXml(PLUGIN_XML_EXTERNAL);

		configFile.write(xml, strlen(xml));
		configFile.close();

		delete [] xml;

		fillConfigurationComboBox();
		selectConfiguration(&QFileInfo(configFileName).completeBaseName(), PLUGIN_CONFIG_USER);
	}

	delete [] configDirectory;
}

void XvidConfigDialog::deleteButton_pressed(void)
{
	XvidOptions options;
	char *configDir = options.getUserConfigDirectory();
	QString configFileName = QFileInfo(QString(configDir), ui.configurationComboBox->currentText() + ".xml").filePath();
	QFile configFile(configFileName);

	delete [] configDir;

	if (GUI_Question(QT_TR_NOOP("Are you sure you wish to delete the selected configuration?")) && configFile.exists())
	{
		disableGenericSlots = true;
		configFile.remove();
		ui.configurationComboBox->removeItem(ui.configurationComboBox->currentIndex());
		disableGenericSlots = false;
		ui.configurationComboBox->setCurrentIndex(0);	// default
	}
}

// General tab
void XvidConfigDialog::encodingModeComboBox_currentIndexChanged(int index)
{
	bool enable = false;

	switch (index)
	{
		case 0: // Constant Bitrate - 1 pass
			ui.singlePassTab->setEnabled(true);
			ui.twoPassTab->setEnabled(false);
			ui.targetRateControlLabel1->setText(QT_TR_NOOP("Target Bitrate:"));
			ui.targetRateControlLabel2->setText(QT_TR_NOOP("kbit/s"));
			ui.targetRateControlSpinBox->setValue(lastBitrate);
			break;
		case 1: // Constant Quantiser - 1 pass
			ui.singlePassTab->setEnabled(true);
			ui.twoPassTab->setEnabled(false);
			ui.quantiserLabel2->setText(QT_TR_NOOP("Quantiser:"));
			enable = true;
			break;
		case 2: // Video Size - 2 pass
			ui.singlePassTab->setEnabled(false);
			ui.twoPassTab->setEnabled(true);
			ui.targetRateControlLabel1->setText(QT_TR_NOOP("Target Video Size:"));
			ui.targetRateControlLabel2->setText(QT_TR_NOOP("MB"));
			ui.targetRateControlSpinBox->setValue(lastVideoSize);
			break;
		case 3: // Average Bitrate - 2 pass
			ui.singlePassTab->setEnabled(false);
			ui.twoPassTab->setEnabled(true);
			ui.targetRateControlLabel1->setText(QT_TR_NOOP("Average Bitrate:"));
			ui.targetRateControlLabel2->setText(QT_TR_NOOP("kbit/s"));
			ui.targetRateControlSpinBox->setValue(lastBitrate);
			break;
	}

	ui.quantiserLabel1->setEnabled(enable);
	ui.quantiserLabel2->setEnabled(enable);
	ui.quantiserLabel3->setEnabled(enable);
	ui.quantiserSlider->setEnabled(enable);
	ui.quantiserSpinBox->setEnabled(enable);

	ui.targetRateControlLabel1->setEnabled(!enable);
	ui.targetRateControlLabel2->setEnabled(!enable);
	ui.targetRateControlSpinBox->setEnabled(!enable);
}

void XvidConfigDialog::quantiserSlider_valueChanged(int value)
{
	ui.quantiserSpinBox->setValue(value);
}

void XvidConfigDialog::quantiserSpinBox_valueChanged(int value)
{
	ui.quantiserSlider->setValue(value);
}

void XvidConfigDialog::targetRateControlSpinBox_valueChanged(int value)
{
	if (ui.encodingModeComboBox->currentIndex() == 2)	// Video Size - 2 pass
		lastVideoSize = value;
	else
		lastBitrate = value;
}

void XvidConfigDialog::matrixCustomEditButton_pressed()
{
	XvidCustomMatrixDialog dialog(this, intraMatrix, interMatrix);

	if (dialog.exec() == QDialog::Accepted)
	{
		dialog.getMatrix(intraMatrix, interMatrix);
		ui.configurationComboBox->setCurrentIndex(1);
	}
}

void XvidConfigDialog::generic_currentIndexChanged(int index)
{
	if (!disableGenericSlots)
		ui.configurationComboBox->setCurrentIndex(1);
}

void XvidConfigDialog::generic_valueChanged(int value)
{
	if (!disableGenericSlots)
		ui.configurationComboBox->setCurrentIndex(1);
}

void XvidConfigDialog::generic_valueChanged(double value)
{
	if (!disableGenericSlots)
		ui.configurationComboBox->setCurrentIndex(1);
}

void XvidConfigDialog::generic_pressed(void)
{
	if (!disableGenericSlots)
		ui.configurationComboBox->setCurrentIndex(1);
}

void XvidConfigDialog::generic_textEdited(QString text)
{
	if (!disableGenericSlots)
		ui.configurationComboBox->setCurrentIndex(1);
}

bool XvidConfigDialog::loadPresetSettings(vidEncOptions *encodeOptions, XvidOptions *options)
{
	bool origDisableGenericSlots = disableGenericSlots;
	char *configurationName;
	PluginConfigType configurationType;

	disableGenericSlots = true;
	options->getPresetConfiguration(&configurationName, &configurationType);		

	bool foundConfig = selectConfiguration(&QString(configurationName), configurationType);

	if (!foundConfig)
		printf("Configuration %s (type %d) could not be found.  Using snapshot.\n", configurationName, configurationType);

	if (configurationName)
		delete [] configurationName;

	disableGenericSlots = origDisableGenericSlots;

	return (foundConfig && configurationType != PLUGIN_CONFIG_CUSTOM);
}

void XvidConfigDialog::loadSettings(vidEncOptions *encodeOptions, XvidOptions *options)
{
	bool origDisableGenericSlots = disableGenericSlots;

	disableGenericSlots = true;

	// General tab
	switch (encodeOptions->encodeMode)
	{
		case ADM_VIDENC_MODE_CBR:	// Constant Bitrate (Single Pass)
			ui.encodingModeComboBox->setCurrentIndex(0);
			ui.targetRateControlSpinBox->setValue(encodeOptions->encodeModeParameter);
			break;
		case ADM_VIDENC_MODE_CQP:	// Constant Quality (Single Pass)
			ui.encodingModeComboBox->setCurrentIndex(1);
			ui.quantiserSpinBox->setValue(encodeOptions->encodeModeParameter);
			break;
		case ADM_VIDENC_MODE_2PASS_SIZE:	// Video Size (Two Pass)
			ui.encodingModeComboBox->setCurrentIndex(2);
			ui.targetRateControlSpinBox->setValue(encodeOptions->encodeModeParameter);
			break;
		case ADM_VIDENC_MODE_2PASS_ABR:	// Average Bitrate (Two Pass)
			ui.encodingModeComboBox->setCurrentIndex(3);
			ui.targetRateControlSpinBox->setValue(encodeOptions->encodeModeParameter);
			break;
	}

	if (options->getParAsInput())
		ui.sarAsInputRadioButton->setChecked(true);
	else
	{
		bool predefined = false;
		unsigned int width, height;

		options->getPar(&width, &height);

		for (int ratioIndex = 0; ratioIndex < aspectRatioCount; ratioIndex++)
		{
			if (width == predefinedARs[ratioIndex][0] && height == predefinedARs[ratioIndex][1])
			{
				ui.sarPredefinedRadioButton->setChecked(true);
				ui.sarPredefinedComboBox->setCurrentIndex(ratioIndex);
				predefined = true;
				break;
			}
		}

		if (!predefined)
		{
			ui.sarCustomRadioButton->setChecked(true);
			ui.sarCustomSpinBox1->setValue(width);
			ui.sarCustomSpinBox2->setValue(height);
		}
	}

	switch (options->getThreads())
	{
		case 0:
			ui.threadAutoDetectRadioButton->setChecked(true);
			break;
		case 1:
			ui.threadDisableRadioButton->setChecked(true);
			break;
		default:
			ui.threadCustomRadioButton->setChecked(true);
			ui.threadCustomSpinBox->setValue(options->getThreads());
	}

	// Motion tab
	ui.meCheckBox->setChecked((options->getMotionEstimation() != ME_NONE));

	switch (options->getMotionEstimation())
	{
		case ME_LOW:
			ui.meComboBox->setCurrentIndex(0);
			break;
		case ME_MEDIUM:
			ui.meComboBox->setCurrentIndex(1);
			break;
		case ME_HIGH:
			ui.meComboBox->setCurrentIndex(2);
			break;
	}

	ui.rdoCheckBox->setChecked((options->getRateDistortion() != RD_NONE));

	switch (options->getRateDistortion())
	{
		case RD_DCT_ME:
			ui.rdoComboBox->setCurrentIndex(0);
			break;
		case RD_HPEL_QPEL_16:
			ui.rdoComboBox->setCurrentIndex(1);
			break;
		case RD_HPEL_QPEL_8:
			ui.rdoComboBox->setCurrentIndex(2);
			break;
		case RD_SQUARE:
			ui.rdoComboBox->setCurrentIndex(3);
			break;
	}

	ui.rdoBframeCheckBox->setChecked(options->getBframeRdo());
	ui.chromaMeCheckBox->setChecked(options->getChromaMotionEstimation());
	ui.qPelCheckBox->setChecked(options->getQpel());
	ui.gmcCheckBox->setChecked(options->getGmc());
	ui.turboCheckBox->setChecked(options->getTurboMode());

	// Frame tab
	ui.chromaOptimiserCheckBox->setChecked(options->getChromaOptimisation());
	ui.fourMvCheckBox->setChecked(options->getInterMotionVector());
	ui.cartoonCheckBox->setChecked(options->getCartoon());
	ui.greyscaleCheckBox->setChecked(options->getGreyscale());

	ui.interlacedCheckBox->setChecked((options->getInterlaced() != INTERLACED_NONE));

	switch (options->getInterlaced())
	{
		case INTERLACED_BFF:
			ui.interlacedComboBox->setCurrentIndex(0);
			break;
		case INTERLACED_TFF:
			ui.interlacedComboBox->setCurrentIndex(1);
			break;
	}

	ui.frameDropSpinBox->setValue(options->getFrameDropRatio());
	ui.maxIframeIntervalSpinBox->setValue(options->getMaxKeyInterval());
	ui.maxBframesSpinBox->setValue(options->getMaxBframes());
	ui.bFrameSensitivitySpinBox->setValue(options->getBframeSensitivity());
	ui.closedGopCheckBox->setChecked(options->getClosedGop());
	ui.packedCheckBox->setChecked(options->getPacked());

	// Quantiser tab
	unsigned int minI, minP, minB;
	unsigned int maxI, maxP, maxB;

	options->getMinQuantiser(&minI, &minP, &minB);
	options->getMaxQuantiser(&maxI, &maxP, &maxB);

	ui.quantIminSpinBox->setValue(minI);
	ui.quantPminSpinBox->setValue(minP);
	ui.quantBminSpinBox->setValue(minB);

	ui.quantImaxSpinBox->setValue(maxI);
	ui.quantPmaxSpinBox->setValue(maxP);
	ui.quantBmaxSpinBox->setValue(maxB);

	ui.quantBratioSpinBox->setValue((float)options->getBframeQuantiserRatio() / 100);
	ui.quantBoffsetSpinBox->setValue((float)options->getBframeQuantiserOffset() / 100);

	switch (options->getCqmPreset())
	{
		case CQM_MPEG:
			ui.matrixMpegRadioButton->setChecked(true);
			break;
		case CQM_CUSTOM:
			ui.matrixCustomRadioButton->setChecked(true);
			break;
		default:
			ui.matrixH263RadioButton->setChecked(true);
			break;
	}

	options->getIntraMatrix(intraMatrix);
	options->getInterMatrix(interMatrix);

	ui.trellisCheckBox->setChecked(options->getTrellis());

	// Single pass tab
	ui.reactionDelaySpinBox->setValue(options->getReactionDelayFactor());
	ui.averagingQuantiserSpinBox->setValue(options->getAveragingQuantiserPeriod());
	ui.smootherSpinBox->setValue(options->getSmoother());

	// Two pass tab
	ui.keyFrameBoostSpinBox->setValue(options->getKeyFrameBoost());
	ui.maxKeyFrameReduceBitrateSpinBox->setValue(options->getMaxKeyFrameReduceBitrate());
	ui.keyFrameBitrateThresholdSpinBox->setValue(options->getKeyFrameBitrateThreshold());
	ui.overflowStrengthSpinBox->setValue(options->getOverflowControlStrength());
	ui.maxOverflowImprovmentSpinBox->setValue(options->getMaxOverflowImprovement());
	ui.maxOverflowDegradationSpinBox->setValue(options->getMaxOverflowDegradation());
	ui.aboveAvgCurveCompSpinBox->setValue(options->getAboveAverageCurveCompression());
	ui.belowAvgCurveCompSpinBox->setValue(options->getBelowAverageCurveCompression());
	ui.vbvBufferSizeSpinBox->setValue(options->getVbvBufferSize());
	ui.maxVbvBitrateSpinBox->setValue(options->getMaxVbvBitrate());
	ui.vbvPeakBitrateSpinBox->setValue(options->getVbvPeakBitrate());

	disableGenericSlots = origDisableGenericSlots;
}

void XvidConfigDialog::saveSettings(vidEncOptions *encodeOptions, XvidOptions *options)
{
	encodeOptions->structSize = sizeof(vidEncOptions);

	// General tab
	switch (ui.encodingModeComboBox->currentIndex())
	{
		case 0:	// Constant Bitrate (Single Pass)
			encodeOptions->encodeMode = ADM_VIDENC_MODE_CBR;
			encodeOptions->encodeModeParameter = ui.targetRateControlSpinBox->value();
			break;
		case 1: // Constant Quality (Single Pass)
			encodeOptions->encodeMode = ADM_VIDENC_MODE_CQP;
			encodeOptions->encodeModeParameter = ui.quantiserSpinBox->value();
			break;
		case 2: // Video Size (Two Pass)
			encodeOptions->encodeMode = ADM_VIDENC_MODE_2PASS_SIZE;
			encodeOptions->encodeModeParameter = ui.targetRateControlSpinBox->value();
			break;
		case 3: // Average Bitrate (Two Pass)
			encodeOptions->encodeMode = ADM_VIDENC_MODE_2PASS_ABR;
			encodeOptions->encodeModeParameter = ui.targetRateControlSpinBox->value();
			break;
	}

	PluginConfigType configurationType = (PluginConfigType)ui.configurationComboBox->itemData(ui.configurationComboBox->currentIndex()).toInt();

	options->setPresetConfiguration(ui.configurationComboBox->currentText().toUtf8().constData(), configurationType);
	options->setParAsInput(ui.sarAsInputRadioButton->isChecked());

	if (ui.sarCustomRadioButton->isChecked())
		options->setPar(ui.sarCustomSpinBox1->value(), ui.sarCustomSpinBox2->value());
	else if (ui.sarPredefinedRadioButton->isChecked())
		options->setPar(predefinedARs[ui.sarPredefinedComboBox->currentIndex()][0], predefinedARs[ui.sarPredefinedComboBox->currentIndex()][1]);
	else
		// clear variables
		options->setPar(1, 1);

	if (ui.threadAutoDetectRadioButton->isChecked())
		options->setThreads(0);
	else if (ui.threadDisableRadioButton->isChecked())
		options->setThreads(1);
	else
		options->setThreads(ui.threadCustomSpinBox->value());

	// Motion tab
	if (ui.meCheckBox->isChecked())
	{
		switch (ui.meComboBox->currentIndex())
		{
			case 0:
				options->setMotionEstimation(ME_LOW);
				break;
			case 1:
				options->setMotionEstimation(ME_MEDIUM);
				break;
			case 2:
				options->setMotionEstimation(ME_HIGH);
				break;
		}
	}
	else
		options->setMotionEstimation(ME_NONE);

	if (ui.rdoCheckBox->isChecked())
	{
		switch (ui.rdoComboBox->currentIndex())
		{
			case 0:
				options->setRateDistortion(RD_DCT_ME);
				break;
			case 1:
				options->setRateDistortion(RD_HPEL_QPEL_16);
				break;
			case 2:
				options->setRateDistortion(RD_HPEL_QPEL_8);
				break;
			case 3:
				options->setRateDistortion(RD_SQUARE);
				break;
		}
	}
	else
		options->setRateDistortion(RD_NONE);

	options->setBframeRdo(ui.rdoBframeCheckBox->isChecked());
	options->setChromaMotionEstimation(ui.chromaMeCheckBox->isChecked());
	options->setQpel(ui.qPelCheckBox->isChecked());
	options->setGmc(ui.gmcCheckBox->isChecked());
	options->setTurboMode(ui.turboCheckBox->isChecked());

	// Frame tab
	options->setChromaOptimisation(ui.chromaOptimiserCheckBox->isChecked());
	options->setInterMotionVector(ui.fourMvCheckBox->isChecked());
	options->setCartoon(ui.cartoonCheckBox->isChecked());
	options->setGreyscale(ui.greyscaleCheckBox->isChecked());

	if (ui.interlacedCheckBox->isChecked())
	{
		switch (ui.interlacedComboBox->currentIndex())
		{
			case 0:
				options->setInterlaced(INTERLACED_BFF);
				break;
			case 1:
				options->setInterlaced(INTERLACED_TFF);
				break;
		}
	}
	else
		options->setInterlaced(INTERLACED_NONE);

	options->setFrameDropRatio(ui.frameDropSpinBox->value());
	options->setMaxKeyInterval(ui.maxIframeIntervalSpinBox->value());
	options->setMaxBframes(ui.maxBframesSpinBox->value());
	options->setBframeSensitivity(ui.bFrameSensitivitySpinBox->value());
	options->setClosedGop(ui.closedGopCheckBox->isChecked());
	options->setPacked(ui.packedCheckBox->isChecked());

	// Quantiser tab
	options->setMinQuantiser(ui.quantIminSpinBox->value(), ui.quantPminSpinBox->value(), ui.quantBminSpinBox->value());
	options->setMaxQuantiser(ui.quantImaxSpinBox->value(), ui.quantPmaxSpinBox->value(), ui.quantBmaxSpinBox->value());
	options->setBframeQuantiserRatio((unsigned int)floor(ui.quantBratioSpinBox->value() * 100 + .05));
	options->setBframeQuantiserOffset((unsigned int)floor(ui.quantBoffsetSpinBox->value() * 100 + .05));

	if (ui.matrixH263RadioButton->isChecked())
		options->setCqmPreset(CQM_H263);
	else if (ui.matrixMpegRadioButton->isChecked())
		options->setCqmPreset(CQM_MPEG);
	else if (ui.matrixCustomRadioButton->isChecked())
	{
		options->setCqmPreset(CQM_CUSTOM);
		options->setIntraMatrix(intraMatrix);
		options->setInterMatrix(interMatrix);
	}

	options->setTrellis(ui.trellisCheckBox->isChecked());

	// Single pass tab
	options->setReactionDelayFactor(ui.reactionDelaySpinBox->value());
	options->setAveragingQuantiserPeriod(ui.averagingQuantiserSpinBox->value());
	options->setSmoother(ui.smootherSpinBox->value());

	// Two pass tab
	options->setKeyFrameBoost(ui.keyFrameBoostSpinBox->value());
	options->setMaxKeyFrameReduceBitrate(ui.maxKeyFrameReduceBitrateSpinBox->value());
	options->setKeyFrameBitrateThreshold(ui.keyFrameBitrateThresholdSpinBox->value());
	options->setOverflowControlStrength(ui.overflowStrengthSpinBox->value());
	options->setMaxOverflowImprovement(ui.maxOverflowImprovmentSpinBox->value());
	options->setMaxOverflowDegradation(ui.maxOverflowDegradationSpinBox->value());
	options->setAboveAverageCurveCompression(ui.aboveAvgCurveCompSpinBox->value());
	options->setBelowAverageCurveCompression(ui.belowAvgCurveCompSpinBox->value());
	options->setVbvBufferSize(ui.vbvBufferSizeSpinBox->value());
	options->setMaxVbvBitrate(ui.maxVbvBitrateSpinBox->value());
	options->setVbvPeakBitrate(ui.vbvPeakBitrateSpinBox->value());
}

extern "C" int showXvidConfigDialog(vidEncConfigParameters *configParameters, vidEncVideoProperties *properties, vidEncOptions *encodeOptions, XvidOptions *options)
{
	XvidConfigDialog dialog(configParameters, properties, encodeOptions, options);

	if (dialog.exec() == QDialog::Accepted)
	{
		dialog.saveSettings(encodeOptions, options);

		return 1;
	}

	return 0;	
}
