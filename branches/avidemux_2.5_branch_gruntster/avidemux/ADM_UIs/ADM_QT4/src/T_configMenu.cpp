/***************************************************************************
T_configMenu.cpp
Handle dialog factory element : Config Menu
(C) 2009 Gruntster 
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>

#include "T_configMenu.h"
#include "ADM_dialogFactoryQt4.h"
#include "ADM_files.h"
#include "DIA_coreToolkit.h"

extern "C"
{
#include "ADM_plugin/ADM_vidEnc_plugin.h"
}

namespace ADM_Qt4Factory
{
	ADM_QconfigMenu::ADM_QconfigMenu(QWidget *widget, QGridLayout *layout, int line, const char* userConfigDir,
		const char* systemConfigDir, CONFIG_MENU_CHANGED_T *changedFunc, CONFIG_MENU_SERIALIZE_T *serializeFunc,
		diaElem **controls,	unsigned int controlCount) : QWidget(widget) 
	{
		disableGenericSlots = false;

		this->userConfigDir = userConfigDir;
		this->systemConfigDir = systemConfigDir;

		this->changedFunc = changedFunc;
		this->serializeFunc = serializeFunc;

		this->controls = controls;
		this->controlCount = controlCount;

		label = new QLabel(tr("Configuration:"), widget);
		combobox = new QComboBox(widget);
		saveAsButton = new QPushButton(tr("Save As"), widget);
		deleteButton = new QPushButton(tr("Delete"), widget);

		QSpacerItem *spacer1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		QSpacerItem *spacer2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

		layout->addItem(spacer1, line, 0);
		layout->addWidget(label, line, 1);
		layout->addWidget(combobox, line, 2);
		layout->addWidget(saveAsButton, line, 3);
		layout->addWidget(deleteButton, line, 4);
		layout->addItem(spacer2, line, 5);

		fillConfigurationComboBox();

		QObject::connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteClicked(bool)));
		QObject::connect(saveAsButton, SIGNAL(clicked(bool)), this, SLOT(saveAsClicked(bool)));
		QObject::connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboboxIndexChanged(int)));
	}

	ADM_QconfigMenu::~ADM_QconfigMenu() 
	{
	}

	void ADM_QconfigMenu::generic_currentIndexChanged(int index)
	{
		if (!disableGenericSlots)
			combobox->setCurrentIndex(1);
	}

	void ADM_QconfigMenu::generic_valueChanged(int value)
	{
		if (!disableGenericSlots)
			combobox->setCurrentIndex(1);
	}

	void ADM_QconfigMenu::generic_valueChanged(double value)
	{
		if (!disableGenericSlots)
			combobox->setCurrentIndex(1);
	}

	void ADM_QconfigMenu::generic_pressed(void)
	{
		if (!disableGenericSlots)
			combobox->setCurrentIndex(1);
	}

	void ADM_QconfigMenu::generic_textEdited(QString text)
	{
		if (!disableGenericSlots)
			combobox->setCurrentIndex(1);
	}

	void ADM_QconfigMenu::fillConfigurationComboBox(void)
	{
		bool origDisableGenericSlots = disableGenericSlots;
		QMap<QString, int> configs;
		QStringList filter("*.xml");
		QStringList list = QDir(this->userConfigDir).entryList(filter, QDir::Files | QDir::Readable);

		disableGenericSlots = true;

		for (int item = 0; item < list.size(); item++)
			configs.insert(QFileInfo(list[item]).completeBaseName(), CONFIG_MENU_USER);

		list = QDir(this->systemConfigDir).entryList(filter, QDir::Files | QDir::Readable);

		for (int item = 0; item < list.size(); item++)
			configs.insert(QFileInfo(list[item]).completeBaseName(), CONFIG_MENU_SYSTEM);

		combobox->clear();
		combobox->addItem(tr("<default>"), CONFIG_MENU_DEFAULT);
		combobox->addItem(tr("<custom>"), CONFIG_MENU_CUSTOM);

		QMap<QString, int>::const_iterator mapIterator = configs.constBegin();

		while (mapIterator != configs.constEnd())
		{
			combobox->addItem(mapIterator.key(), mapIterator.value());
			mapIterator++;
		}

		disableGenericSlots = origDisableGenericSlots;
	}

	void ADM_QconfigMenu::getConfiguration(char *configName, ConfigMenuType *configType)
	{
		strcpy(configName, combobox->currentText().toUtf8().constData());

		*configType = (ConfigMenuType)combobox->itemData(combobox->currentIndex()).toInt();
	}

	bool ADM_QconfigMenu::selectConfiguration(QString *selectFile, ConfigMenuType configurationType)
	{
		bool success = false;
		bool origDisableGenericSlots = disableGenericSlots;

		disableGenericSlots = true;

		if (configurationType == CONFIG_MENU_DEFAULT)
		{
			combobox->setCurrentIndex(0);
			success = true;
		}
		else
		{
			for (int index = 0; index < combobox->count(); index++)
			{
				if (combobox->itemText(index) == selectFile && combobox->itemData(index).toInt() == configurationType)
				{
					combobox->setCurrentIndex(index);
					success = true;
					break;
				}
			}

			if (!success)
				combobox->setCurrentIndex(1);
		}

		disableGenericSlots = origDisableGenericSlots;

		return success;
	}

	void ADM_QconfigMenu::deleteClicked(bool checked)
	{
		if (combobox->itemData(combobox->currentIndex()).toInt() == CONFIG_MENU_USER)
		{
			QString configFileName = QFileInfo(QString(this->userConfigDir), combobox->currentText() + ".xml").filePath();
			QFile configFile(configFileName);

			if (GUI_Question(tr("Are you sure you wish to delete the selected configuration?").toUtf8().constData()) && configFile.exists())
			{
				configFile.remove();
				combobox->removeItem(combobox->currentIndex());
				combobox->setCurrentIndex(0);	// default
			}
		}
	}

	void ADM_QconfigMenu::saveAsClicked(bool checked)
	{
		if (this->serializeFunc)
		{
			ADM_mkdir(this->userConfigDir);

			QString configFileName = QFileDialog::getSaveFileName(this, tr("Save As"), this->userConfigDir, tr("Configuration File (*.xml)"));

			if (!configFileName.isNull())
			{
				for (int i = 0; i < this->controlCount; i++)
					this->controls[i]->getMe();

				char *configData = this->serializeFunc();
				QFile configFile(configFileName);

				configFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
				configFile.write(configData, strlen(configData));
				configFile.close();

				delete [] configData;

				fillConfigurationComboBox();
				selectConfiguration(&QFileInfo(configFileName).completeBaseName(), CONFIG_MENU_USER);
			}
		}
	}

	void ADM_QconfigMenu::comboboxIndexChanged(int index)
	{
		bool origDisableGenericSlots = disableGenericSlots;
		ConfigMenuType configType = (ConfigMenuType)combobox->itemData(index).toInt();

		disableGenericSlots = true;
		deleteButton->setEnabled(configType == CONFIG_MENU_USER);

		for (int i = 0; i < this->controlCount; i++)
			this->controls[i]->getMe();

		if (changedFunc)
		{
			if (changedFunc(combobox->itemText(index).toUtf8().constData(), configType))
			{
				for (int i = 0; i < this->controlCount; i++)
					this->controls[i]->updateMe();
			}
			else
				combobox->setCurrentIndex(0);
		}

		disableGenericSlots = origDisableGenericSlots;
	}

	class diaElemConfigMenu : public diaElem
	{
	protected:
		char *configName;
		ConfigMenuType *configType;

		const char *userConfigDir, *systemConfigDir;
		diaElem **controls;
		unsigned int controlCount;

		CONFIG_MENU_CHANGED_T *changedFunc;
		CONFIG_MENU_SERIALIZE_T *serializeFunc;

	public:
		diaElemConfigMenu(char *configName, ConfigMenuType *configType, const char* userConfigDir,
			const char* systemConfigDir, CONFIG_MENU_CHANGED_T *changedFunc, CONFIG_MENU_SERIALIZE_T *serializeFunc,
			diaElem **controls, unsigned int controlCount);
		~diaElemConfigMenu();
		void setMe(void *dialog, void *opaque, uint32_t line);
		void getMe(void);
		void enable(uint32_t onoff);
		int getRequiredLayout(void);
		void updateMe(void);
		void finalize(void);
	};

	diaElemConfigMenu::diaElemConfigMenu(char *configName, ConfigMenuType *configType, const char* userConfigDir,
		const char* systemConfigDir, CONFIG_MENU_CHANGED_T *changedFunc, CONFIG_MENU_SERIALIZE_T *serializeFunc,
		diaElem **controls, unsigned int controlCount) : diaElem(ELEM_CONFIG_MENU)
	{
		this->configName = configName;
		this->configType = configType;

		this->userConfigDir = userConfigDir;
		this->systemConfigDir = systemConfigDir;

		this->changedFunc = changedFunc;
		this->serializeFunc = serializeFunc;

		this->controls = controls;
		this->controlCount = controlCount;
	}

	diaElemConfigMenu::~diaElemConfigMenu()
	{
	}

	void diaElemConfigMenu::setMe(void *dialog, void *opaque, uint32_t line)
	{
		QGridLayout *layout = (QGridLayout*)opaque;
		ADM_QconfigMenu *configMenu = new ADM_QconfigMenu((QWidget*)dialog, layout, line, this->userConfigDir, 
			this->systemConfigDir, this->changedFunc, this->serializeFunc, this->controls, this->controlCount);

		myWidget = (void*)configMenu;
	}

	void diaElemConfigMenu::getMe(void)
	{
		ADM_QconfigMenu *configMenu = (ADM_QconfigMenu*)myWidget;

		configMenu->getConfiguration(configName, configType);
	}

	void diaElemConfigMenu::enable(uint32_t onoff)
	{
	}

	int diaElemConfigMenu::getRequiredLayout(void)
	{
		return FAC_QT_GRIDLAYOUT;
	}

	void diaElemConfigMenu::updateMe(void)
	{
		ADM_QconfigMenu *configMenu = (ADM_QconfigMenu*)myWidget;

		configMenu->selectConfiguration(&QString(configName), *configType);
	}

	void diaElemConfigMenu::finalize(void)
	{
		ADM_QconfigMenu *configMenu = (ADM_QconfigMenu*)myWidget;
		QWidgetList widgetList = QApplication::allWidgets();

		for (int widgetIndex = 0; widgetIndex < widgetList.size(); widgetIndex++)
		{
			QWidget *widget = widgetList.at(widgetIndex);
			QWidget *parentWidget = widget;

			if (widget != configMenu->combobox && widget != configMenu->label && 
				widget != configMenu->deleteButton && widget != configMenu->saveAsButton)
			{
				do
				{
					if (parentWidget == configMenu->combobox->parent())
					{
						if (widget->inherits("QComboBox"))
							QObject::connect(widget, SIGNAL(currentIndexChanged(int)), configMenu, SLOT(generic_currentIndexChanged(int)));
						else if (widget->inherits("QSpinBox"))
							QObject::connect(widget, SIGNAL(valueChanged(int)), configMenu, SLOT(generic_valueChanged(int)));
						else if (widget->inherits("QDoubleSpinBox"))
							QObject::connect(widget, SIGNAL(valueChanged(double)), configMenu, SLOT(generic_valueChanged(double)));
						else if (widget->inherits("QCheckBox"))
							QObject::connect(widget, SIGNAL(pressed()), configMenu, SLOT(generic_pressed()));
						else if (widget->inherits("QRadioButton"))
							QObject::connect(widget, SIGNAL(pressed()), configMenu, SLOT(generic_pressed()));
						else if (widget->inherits("QLineEdit"))
							QObject::connect(widget, SIGNAL(textEdited(QString)), configMenu, SLOT(generic_textEdited(QString)));
					}

					parentWidget = (QWidget*)parentWidget->parent();
				}
				while (parentWidget != NULL);
			}
		}

		configMenu->selectConfiguration(&QString(configName), *configType);
	}
}

diaElem* qt4CreateConfigMenu(char *configName, ConfigMenuType *configType, const char* userConfigDir, const char* systemConfigDir,
							 CONFIG_MENU_CHANGED_T *changedFunc, CONFIG_MENU_SERIALIZE_T *serializeFunc, diaElem **controls,
							 unsigned int controlCount)
{
	return new ADM_Qt4Factory::diaElemConfigMenu(configName, configType, userConfigDir, systemConfigDir, changedFunc, 
		serializeFunc, controls, controlCount);
}

void qt4DestroyConfigMenu(diaElem *e)
{
	ADM_Qt4Factory::diaElemConfigMenu *a = (ADM_Qt4Factory::diaElemConfigMenu*)e;

	delete a;
}
