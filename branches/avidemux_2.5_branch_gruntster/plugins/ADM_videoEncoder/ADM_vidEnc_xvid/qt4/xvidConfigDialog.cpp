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
#include <QtGui/QFileDialog>

#include "../config.h"
#include "../presetOptions.h"
#include "xvidConfigDialog.h"

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
			else if (widget->inherits("QAbstractButton"))
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
	bool origDisableGenericSlots = disableGenericSlots;
	QMap<QString, int> configs;
	QStringList filter("*.xml");
	QStringList list = QDir(getUserConfigDirectory()).entryList(filter, QDir::Files | QDir::Readable);

	disableGenericSlots = true;

	for (int item = 0; item < list.size(); item++)
		configs.insert(QFileInfo(list[item]).baseName(), CONFIG_USER);

	list = QDir(getSystemConfigDirectory()).entryList(filter, QDir::Files | QDir::Readable);

	for (int item = 0; item < list.size(); item++)
		configs.insert(QFileInfo(list[item]).baseName(), CONFIG_SYSTEM);

	ui.configurationComboBox->clear();
	ui.configurationComboBox->addItem(QT_TR_NOOP("<default>"), CONFIG_DEFAULT);
	ui.configurationComboBox->addItem(QT_TR_NOOP("<custom>"), CONFIG_CUSTOM);

	QMap<QString, int>::const_iterator mapIterator = configs.constBegin();

	while (mapIterator != configs.constEnd())
	{
		ui.configurationComboBox->addItem(mapIterator.key(), mapIterator.value());
		mapIterator++;
	}

	disableGenericSlots = origDisableGenericSlots;
}

bool XvidConfigDialog::selectConfiguration(QString *selectFile, ConfigType configurationType)
{
	bool success = false;
	bool origDisableGenericSlots = disableGenericSlots;

	disableGenericSlots = true;

	if (configurationType == CONFIG_DEFAULT)
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

		XvidPresetOptions defaultOptions;
		vidEncOptions *defaultEncodeOptions = defaultOptions.getEncodeOptions();

		loadSettings(defaultEncodeOptions, &defaultOptions);

		delete defaultEncodeOptions;
	}
	else if (index == 1)	// custom
		ui.deleteButton->setEnabled(false);
	else
	{
		int configType = ui.configurationComboBox->itemData(index).toInt();
		QString configFileName;

		ui.deleteButton->setEnabled(configType == CONFIG_USER);

		if (configType == CONFIG_SYSTEM)
			configFileName = QFileInfo(getSystemConfigDirectory(), ui.configurationComboBox->itemText(index) + ".xml").filePath();
		else	// CONFIG_USER
			configFileName = QFileInfo(getUserConfigDirectory(), ui.configurationComboBox->itemText(index) + ".xml").filePath();

		QFile configFile(configFileName);

		if (configFile.exists())
		{
			configFile.open(QIODevice::ReadOnly | QIODevice::Text);

			QByteArray fileContents = configFile.readAll();
			XvidPresetOptions options;
			vidEncOptions *encodeOptions;

			configFile.close();
			options.fromXml(fileContents.constData());
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
		XvidPresetOptions presetOptions;

		configFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
		saveSettings(&encodeOptions, &presetOptions);
		presetOptions.setEncodeOptions(&encodeOptions);

		char* xml = presetOptions.toXml();

		configFile.write(xml, strlen(xml));
		configFile.close();

		delete [] xml;

		fillConfigurationComboBox();
		selectConfiguration(&QFileInfo(configFileName).baseName(), CONFIG_USER);
	}

	delete [] configDirectory;
}

void XvidConfigDialog::deleteButton_pressed(void)
{
	QString configFileName = QFileInfo(getUserConfigDirectory(), ui.configurationComboBox->currentText() + ".xml").filePath();
	QFile configFile(configFileName);

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
		case 0:
			ui.targetRateControlLabel1->setText(QT_TR_NOOP("Target Bitrate:"));
			ui.targetRateControlLabel2->setText(QT_TR_NOOP("kbit/s"));
			ui.targetRateControlSpinBox->setValue(lastBitrate);
			break;
		case 1: // Constant Quality - 1 pass
			ui.quantiserLabel2->setText(QT_TR_NOOP("Quantiser:"));
			enable = true;
			break;
		case 2: // Video Size - 2 pass
			ui.targetRateControlLabel1->setText(QT_TR_NOOP("Target Video Size:"));
			ui.targetRateControlLabel2->setText(QT_TR_NOOP("MB"));
			ui.targetRateControlSpinBox->setValue(lastVideoSize);
			break;
		case 3: // Average Bitrate - 2 pass
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
	ConfigType configurationType;

	disableGenericSlots = true;
	options->getPresetConfiguration(&configurationName, &configurationType);		

	bool foundConfig = selectConfiguration(&QString(configurationName), configurationType);

	if (!foundConfig)
		printf("Configuration %s (type %d) could not be found.  Using snapshot.\n", configurationName, configurationType);

	if (configurationName)
		free(configurationName);

	disableGenericSlots = origDisableGenericSlots;

	return (foundConfig && configurationType != CONFIG_CUSTOM);
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

	ConfigType configurationType = (ConfigType)ui.configurationComboBox->itemData(ui.configurationComboBox->currentIndex()).toInt();

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
}

QString XvidConfigDialog::getUserConfigDirectory(void)
{
	char *userConfigDirectory = ADM_getHomeRelativePath("xvid");
	QString qstring = QString(userConfigDirectory);

	delete [] userConfigDirectory;

	return qstring;
}

QString XvidConfigDialog::getSystemConfigDirectory(void)
{
	char* pluginPath = ADM_getPluginPath();
	QString qstring = QString(pluginPath).append("/").append(PLUGIN_SUBDIR);

	delete [] pluginPath;

	return qstring;
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
