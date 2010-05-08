/***************************************************************************
FAC_configMenu.cpp
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

#include <string>
#include <map>

#include "ADM_toolkitGtk.h"
#include "DIA_factory.h"
#include "DIA_coreToolkit.h"
#include "DIA_fileSel.h"

using namespace std;

extern "C"
{
#include "ADM_plugin/ADM_vidEnc_plugin.h"
}

namespace ADM_GtkFactory
{
	struct diaElemConfigMenuData
	{
		bool disableSignals;

		GtkDialog *dialog;
		GtkComboBox *combo;
		GtkButton *deleteButton;

		diaElem **controls;
		unsigned int controlCount;

		map<string, int> *configs;
		const char *userConfigDir;
		const char *systemConfigDir;

		CONFIG_MENU_CHANGED_T *changedFunc;
		CONFIG_MENU_SERIALIZE_T *serializeFunc;
	};

	static map<string, int>* fillConfigurationComboBox(diaElemConfigMenuData *menuData)
	{
#define MAX_CONFIG 100

		uint32_t fileCount = 0;
		char *files[MAX_CONFIG];
		map<string, int> *configs = new map<string, int>();
		bool origDisableSignals = menuData->disableSignals;

		buildDirectoryContent(&fileCount, menuData->userConfigDir, files, MAX_CONFIG, "xml");

		for (int i = 0; i < fileCount; i++)
		{
			files[i][strlen(files[i]) - 4] = 0;	// clip extension
			configs->insert(pair<string, ConfigMenuType>(string(ADM_GetFileName(files[i])), CONFIG_MENU_USER));

			ADM_dealloc(files[i]);
		}

		fileCount = 0;
		buildDirectoryContent(&fileCount, menuData->systemConfigDir, files, MAX_CONFIG, "xml");

		for (int i = 0; i < fileCount; i++)
		{
			files[i][strlen(files[i]) - 4] = 0;	// clip extension
			configs->insert(pair<string, ConfigMenuType>(string(ADM_GetFileName(files[i])), CONFIG_MENU_USER));

			ADM_dealloc(files[i]);
		}

		GtkTreeModel* model = gtk_combo_box_get_model(menuData->combo);

		gtk_list_store_clear(GTK_LIST_STORE(model));
		gtk_combo_box_append_text(menuData->combo, QT_TR_NOOP("<default>"));
		gtk_combo_box_append_text(menuData->combo, QT_TR_NOOP("<custom>"));

		for (map<string, int>::iterator it = configs->begin(); it != configs->end(); it++)
			gtk_combo_box_append_text(menuData->combo, it->first.c_str());

		configs->insert(pair<string, ConfigMenuType>(string(QT_TR_NOOP("<default>")), CONFIG_MENU_DEFAULT));
		configs->insert(pair<string, ConfigMenuType>(string(QT_TR_NOOP("<custom>")), CONFIG_MENU_CUSTOM));

		gtk_combo_box_set_active(menuData->combo, 0);

		menuData->disableSignals = origDisableSignals;

		return configs;
	}

	static bool selectConfiguration(diaElemConfigMenuData *menuData, const char *selectFile, ConfigMenuType configurationType)
	{
		bool success = false;
		bool origDisableSignals = menuData->disableSignals;

		if (configurationType == CONFIG_MENU_DEFAULT)
		{
			gtk_combo_box_set_active(menuData->combo, 0);
			success = true;
		}
		else
		{
			GtkTreeModel* model = gtk_combo_box_get_model(menuData->combo);
			GtkTreeIter iter;
			const char *config;
			int index = 0;

			if (gtk_tree_model_get_iter_first(model, &iter) && selectFile)
			{
				do
				{
					map<string, int>::iterator it = menuData->configs->find(string(selectFile));

					gtk_tree_model_get(model, &iter, 0, &config, -1);

					if (strcmp(config, selectFile) == 0 && it->second == configurationType)
					{
						gtk_combo_box_set_active(menuData->combo, index);
						success = true;
						break;
					}

					index++;
				}
				while (gtk_tree_model_iter_next(model, &iter));
			}

			if (!success)
				gtk_combo_box_set_active(menuData->combo, 1);
		}

		menuData->disableSignals = origDisableSignals;

		return success;
	}

	static void saveAsClicked(GtkWidget *widget, gpointer *data)
	{
		diaElemConfigMenuData *menuData = (diaElemConfigMenuData*)data;

		if (menuData->serializeFunc)
		{
			ADM_mkdir(menuData->userConfigDir);

			char filename[1024];

			if (FileSel_SelectWrite(QT_TR_NOOP("Save As"), filename, 1023, menuData->userConfigDir))
			{
				for (int i = 0; i < menuData->controlCount; i++)
					menuData->controls[i]->getMe();

				char *configData = menuData->serializeFunc();

				FILE *fd = fopen(filename, "w");

				fwrite(configData, 1, strlen(configData), fd);
				fclose(fd);

				delete menuData->configs;
				menuData->configs = fillConfigurationComboBox(menuData);

				char* baseName = (char *)ADM_GetFileName(filename);
				char *ext = strrchr(baseName, '.');

				if (ext)
					*ext = 0;

				selectConfiguration(menuData, baseName, CONFIG_MENU_USER);

				delete [] configData;
			}
		}
	}

	static void deleteClicked(GtkWidget *widget, gpointer *data)
	{
		diaElemConfigMenuData *menuData = (diaElemConfigMenuData*)data;
		const char* selectedConfig = gtk_combo_box_get_active_text(menuData->combo);
		map<string, int>::iterator it = menuData->configs->find(string((selectedConfig)));

		if (it->second == CONFIG_MENU_USER)
		{
			char path[strlen(menuData->userConfigDir) + strlen(selectedConfig) + 5];

			strcpy(path, menuData->userConfigDir);
			strcat(path, selectedConfig);
			strcat(path, ".xml");

			if (GUI_Question(QT_TR_NOOP("Are you sure you wish to delete the selected configuration?")) && ADM_fileExist(path))
			{
				ADM_unlink(path);
				menuData->configs->erase(it);
				gtk_combo_box_remove_text(menuData->combo, gtk_combo_box_get_active(menuData->combo));
				gtk_combo_box_set_active(menuData->combo, 0);	// default
			}
		}
	}

	void comboChanged(GtkWidget *widget, gpointer *data)
	{
		diaElemConfigMenuData *menuData = (diaElemConfigMenuData*)data;
		const char* selectedConfig = gtk_combo_box_get_active_text(menuData->combo);
		bool origDisableSignals = menuData->disableSignals;

		menuData->disableSignals = true;

		if (selectedConfig)
		{
			map<string, int>::iterator it = menuData->configs->find(string((selectedConfig)));

			gtk_widget_set_sensitive(GTK_WIDGET(menuData->deleteButton), it->second == CONFIG_MENU_USER);

			for (int i = 0; i < menuData->controlCount; i++)
				menuData->controls[i]->getMe();

			if (menuData->changedFunc)
			{
				if (menuData->changedFunc(selectedConfig, (ConfigMenuType)it->second))
				{
					for (int i = 0; i < menuData->controlCount; i++)
						menuData->controls[i]->updateMe();
				}
				else
					gtk_combo_box_set_active(menuData->combo, 0);
			}
		}

		menuData->disableSignals = origDisableSignals;
	}

	void genericControlChanged(GtkWidget *widget, gpointer *data)
	{
		diaElemConfigMenuData *menuData = (diaElemConfigMenuData*)data;

		if (!menuData->disableSignals)
			gtk_combo_box_set_active(menuData->combo, 1);
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
		diaElemConfigMenu(char *configName, ConfigMenuType *configType, const char* userConfigDir, const char* systemConfigDir, 
			CONFIG_MENU_CHANGED_T *changedFunc, CONFIG_MENU_SERIALIZE_T *serializeFunc, diaElem **controls, unsigned int controlCount);
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
		diaElemConfigMenuData *data = (diaElemConfigMenuData*)myWidget;

		delete data->configs;
		delete data;
	}

	void diaElemConfigMenu::setMe(void *dialog, void *opaque, uint32_t line)
	{
		GtkWidget *label = gtk_label_new_with_mnemonic(QT_TR_NOOP("Configuration:"));
		gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
		gtk_widget_show(label);
		gtk_table_attach(GTK_TABLE(opaque), label, 0, 1, line, line + 1, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)0, 0, 0);

		GtkComboBox *combo = GTK_COMBO_BOX(gtk_combo_box_new_text());
		gtk_widget_show(GTK_WIDGET(combo));
		gtk_label_set_mnemonic_widget(GTK_LABEL(label), GTK_WIDGET(combo));
		gtk_table_attach(GTK_TABLE(opaque), GTK_WIDGET(combo), 1, 2, line, line + 1, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)0, 0, 0);

		GtkWidget *button1 = gtk_button_new_from_stock(QT_TR_NOOP("Save As"));
		gtk_widget_show(button1);
		gtk_table_attach(GTK_TABLE(opaque), button1, 2, 3, line, line + 1, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)0, 0, 0);

		GtkButton *button2 = GTK_BUTTON(gtk_button_new_from_stock(QT_TR_NOOP("Delete")));
		gtk_widget_show(GTK_WIDGET(button2));
		gtk_table_attach(GTK_TABLE(opaque), GTK_WIDGET(button2), 3, 4, line, line + 1, (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), (GtkAttachOptions)0, 0, 0);

		diaElemConfigMenuData *data = new diaElemConfigMenuData();

		data->dialog = (GtkDialog*)dialog;
		data->combo = combo;
		data->deleteButton = button2;
		data->controls = controls;
		data->controlCount = controlCount;
		data->userConfigDir = userConfigDir;
		data->systemConfigDir = systemConfigDir;
		data->changedFunc = changedFunc;
		data->serializeFunc = serializeFunc;
		data->disableSignals = false;
		data->configs = fillConfigurationComboBox(data);

		myWidget = (void*)data;

		g_signal_connect(GTK_OBJECT(button1), "clicked", G_CALLBACK(saveAsClicked), myWidget);
		g_signal_connect(GTK_OBJECT(button2), "clicked", G_CALLBACK(deleteClicked), myWidget);
		g_signal_connect(GTK_OBJECT(combo), "changed", G_CALLBACK(comboChanged), myWidget);
	}

	void diaElemConfigMenu::getMe(void)
	{
		diaElemConfigMenuData *menuData = (diaElemConfigMenuData*)myWidget;
		const char* selectedConfig = gtk_combo_box_get_active_text(menuData->combo);
		map<string, int>::iterator it = menuData->configs->find(string((selectedConfig)));

		strcpy(this->configName, selectedConfig);
		*this->configType = (ConfigMenuType)it->second;
	}

	void diaElemConfigMenu::enable(uint32_t onoff) { }
	int diaElemConfigMenu::getRequiredLayout(void) { return 0; }

	void diaElemConfigMenu::updateMe(void)
	{
		diaElemConfigMenuData *menuData = (diaElemConfigMenuData*)myWidget;

		selectConfiguration(menuData, this->configName, *this->configType);
	}

	void traverseChildren(GtkContainer *container, diaElemConfigMenuData *menuData)
	{
		GList *list = gtk_container_get_children(container);

		for (GList *listItem = g_list_first(list); listItem != NULL; listItem = g_list_next(listItem))
		{
			const char* typeName = g_type_name(GTK_OBJECT_TYPE(listItem->data));

			if (GTK_IS_CONTAINER(listItem->data))
				traverseChildren(GTK_CONTAINER(listItem->data), menuData);

			if (strcmp(typeName, "GtkComboBox") == 0 && GTK_COMBO_BOX(listItem->data) != menuData->combo)
				g_signal_connect(GTK_OBJECT(listItem->data), "changed", G_CALLBACK(genericControlChanged), menuData);
			else if (strcmp(typeName, "GtkSpinButton") == 0)
				g_signal_connect(GTK_OBJECT(listItem->data), "value-changed", G_CALLBACK(genericControlChanged), menuData);
			else if (strcmp(typeName, "GtkCheckButton") == 0)
				g_signal_connect(GTK_OBJECT(listItem->data), "toggled", G_CALLBACK(genericControlChanged), menuData);
			else if (strcmp(typeName, "GtkRadioButton") == 0)
				g_signal_connect(GTK_OBJECT(listItem->data), "toggled", G_CALLBACK(genericControlChanged), menuData);
			else if (strcmp(typeName, "GtkEntry") == 0)
				g_signal_connect(GTK_OBJECT(listItem->data), "changed", G_CALLBACK(genericControlChanged), menuData);
		}

		g_list_free(list);
	}

	void diaElemConfigMenu::finalize(void)
	{
		diaElemConfigMenuData *menuData = (diaElemConfigMenuData*)myWidget;
		traverseChildren(GTK_CONTAINER(menuData->dialog), menuData);

		this->updateMe();
	}
}

diaElem* gtkCreateConfigMenu(char *configName, ConfigMenuType *configType, const char* userConfigDir, const char* systemConfigDir,
							 CONFIG_MENU_CHANGED_T *changedFunc, CONFIG_MENU_SERIALIZE_T *serializeFunc, diaElem **controls, 
							 unsigned int controlCount)
{
	return new ADM_GtkFactory::diaElemConfigMenu(configName, configType, userConfigDir, systemConfigDir, changedFunc,
		serializeFunc, controls, controlCount);
}

void gtkDestroyConfigMenu(diaElem *e)
{
	ADM_GtkFactory::diaElemConfigMenu *a = (ADM_GtkFactory::diaElemConfigMenu*)e;

	delete a;
}
