/***************************************************************************
                            Q_pluginManager.cpp
                            -------------------

    begin                : Fri June 11 2010
    copyright            : (C) 2010 by gruntster
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "config.h"
#undef QT_TR_NOOP

#include "Q_pluginManager.h"
#include "ADM_inttype.h"
#include "ADM_toolkitQt.h"
#include "ADM_encoder/ADM_vidEncode.hxx"

#define PLUGIN_ID           Qt::UserRole
#define PLUGIN_IS_DEFAULT   Qt::UserRole + 1

extern CODEC_INFO *internalVideoCodecInfo[];
extern uint32_t ADM_ve_getNbEncoders(void);
extern bool ADM_ve_getEncoderInfo(int filter, const char **id, const char **name, const char **type, const char **desc, uint32_t *major, uint32_t *minor, uint32_t *patch);
extern int getInternalVideoCodecCount();

Ui_pluginManagerWindow::Ui_pluginManagerWindow(QWidget* parent) : QDialog(parent)
{
	UiPluginManager manager;

	ui.setupUi(this);

	QStandardItemModel *model = new QStandardItemModel();
	QStandardItem *nameItem = new QStandardItem(tr("Name"));
	QStandardItem *versionItem = new QStandardItem(tr("Version"));
	QStandardItem *descItem = new QStandardItem(tr("Description"));

	nameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	versionItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	descItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	model->setHorizontalHeaderItem(0, nameItem);
	model->setHorizontalHeaderItem(1, versionItem);
	model->setHorizontalHeaderItem(2, descItem);

	ui.pluginTableView->verticalHeader()->setVisible(false);
	ui.pluginTableView->setModel(model);

	connect(
		ui.pluginTableView->selectionModel(), 
		SIGNAL(currentRowChanged(const QModelIndex, const QModelIndex)), this, 
		SLOT(pluginTableView_rowChanged(const QModelIndex, const QModelIndex)));

	connect(ui.setDefaultButton, SIGNAL(clicked(bool)), this, SLOT(setDefaultButton_clicked(bool)));
	connect(ui.enableAllButton, SIGNAL(clicked(bool)), this, SLOT(enableAllButton_clicked(bool)));
	connect(ui.disableAllButton, SIGNAL(clicked(bool)), this, SLOT(disableAllButton_clicked(bool)));
	connect(ui.moveUpButton, SIGNAL(clicked(bool)), this, SLOT(moveUpButton_clicked(bool)));
	connect(ui.moveDownButton, SIGNAL(clicked(bool)), this, SLOT(moveDownButton_clicked(bool)));

	fillVideoEncoderList(manager);
	ui.pluginTableView->resizeColumnsToContents();

	connect(
		ui.pluginTableView->model(),
		SIGNAL(itemChanged(QStandardItem*)), this,
		SLOT(pluginTableView_itemChanged(QStandardItem*)));
}

void Ui_pluginManagerWindow::fillVideoEncoderList(UiPluginManager manager)
{
	uint32_t pluginCount = ADM_ve_getNbEncoders();
	set<string> pluginIds;
	map<string, UiPlugVersionInfo> plugins;

	for (int i = 0; i < getInternalVideoCodecCount(); i++)
	{
		UiPlugVersionInfo versionInfo;

		versionInfo.name = internalVideoCodecInfo[i]->menuName;
		versionInfo.version = QString("%1.0.%2").arg(VERSION).arg(ADM_SUBVERSION).toUtf8().constData();
		versionInfo.description = tr("Internal video encoder").toUtf8().constData();

		pluginIds.insert(internalVideoCodecInfo[i]->tagName);
		plugins[internalVideoCodecInfo[i]->tagName] = versionInfo;
	}

	for (int i = 0; i < pluginCount; i++)
	{
		const char *id, *name, *type, *desc;
		uint32_t major, minor, patch;
		UiPlugVersionInfo versionInfo;

		ADM_ve_getEncoderInfo(i, &id, &name, &type, &desc, &major, &minor, &patch);

		versionInfo.name = QString("%1 (%2)").arg(type).arg(name).toUtf8().constData();
		versionInfo.version = QString("%1.%2.%3").arg(major).arg(minor, 2, 10, QLatin1Char('0')).arg(patch, 2, 10, QLatin1Char('0')).toUtf8().constData();
		versionInfo.description = desc;

		plugins[id] = versionInfo;
		pluginIds.insert(id);
	}

	list<UiPluginManager::UiPlugInfo> rankedList = manager.getRankedList(UiPluginManager::PLUGINTYPE_VIDEO_ENCODER, pluginIds);
	bool defaultSet = false;

	for (list<UiPluginManager::UiPlugInfo>::const_iterator itRankedPlugin = rankedList.begin(); itRankedPlugin != rankedList.end(); itRankedPlugin++)
	{
		string id = itRankedPlugin->id;
		map<string, UiPlugVersionInfo>::const_iterator itPlugin = plugins.find(id);

		addRow(
			QString(id.c_str()), QString(itPlugin->second.name.c_str()), QString(itPlugin->second.version.c_str()),
			QString(itPlugin->second.description.c_str()), itRankedPlugin->enabled, itRankedPlugin->isDefault);

		if (!defaultSet && itRankedPlugin->isDefault)
			defaultSet = true;
	}

	if (pluginIds.size())
	{
		ui.pluginTableView->selectRow(0);

		if (!defaultSet)
			setDefaultRow(0);
	}

}

void Ui_pluginManagerWindow::pluginTableView_rowChanged(const QModelIndex current, const QModelIndex previous)
{
	ui.moveUpButton->setEnabled(current.row() > 0);
	ui.moveDownButton->setEnabled((current.row() + 1) != ui.pluginTableView->model()->rowCount());
}

void Ui_pluginManagerWindow::pluginTableView_itemChanged(QStandardItem* item)
{
	if (item->column() == 0 && item->checkState() == Qt::Unchecked)
		clearDefaultRow(item->row());
}

void Ui_pluginManagerWindow::setDefaultButton_clicked(bool)
{
	QStandardItemModel *model = (QStandardItemModel*)ui.pluginTableView->model();
	
	setDefaultRow(ui.pluginTableView->selectionModel()->currentIndex().row());
}

void Ui_pluginManagerWindow::enableAllButton_clicked(bool)
{
	checkAllItems(true);
}

void Ui_pluginManagerWindow::disableAllButton_clicked(bool)
{
	checkAllItems(false);
}

void Ui_pluginManagerWindow::moveUpButton_clicked(bool)
{
	moveSelectedItem(true);
}

void Ui_pluginManagerWindow::moveDownButton_clicked(bool)
{
	moveSelectedItem(false);
}

void Ui_pluginManagerWindow::addRow(QString id, QString name, QString version, QString desc, bool enabled, bool isDefault)
{
	QStandardItemModel *model = (QStandardItemModel*)ui.pluginTableView->model();
	QStandardItem *nameItem = new QStandardItem(name);
	QStandardItem *versionItem = new QStandardItem(version);
	QStandardItem *descItem = new QStandardItem(desc);
	int row = model->rowCount();

	nameItem->setCheckable(true);
	nameItem->setData(id, PLUGIN_ID);
	versionItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

	if (enabled)
		nameItem->setCheckState(Qt::Checked);

	model->setItem(row, 0, nameItem);
	model->setItem(row, 1, versionItem);
	model->setItem(row, 2, descItem);

	if (isDefault)
		setDefaultRow(row);
}

void Ui_pluginManagerWindow::setDefaultRow(int defaultRow)
{
	QStandardItemModel *model = (QStandardItemModel*)ui.pluginTableView->model();

	for (int row = 0; row < model->rowCount(); row++)
	{
		QStandardItem *item = model->item(row, 0);

		item->setData(row == defaultRow, PLUGIN_IS_DEFAULT);

		for (int column = 0; column < model->columnCount(); column++)
		{
			QStandardItem *item = model->item(row, column);
			QFont font = item->font();
		
			font.setBold(row == defaultRow);
			item->setFont(font);
		}
	}
}

void Ui_pluginManagerWindow::clearDefaultRow(int defaultRow)
{
	QStandardItemModel *model = (QStandardItemModel*)ui.pluginTableView->model();
	QStandardItem *item = model->item(defaultRow, 0);

	item->setData(false, PLUGIN_IS_DEFAULT);

	for (int column = 0; column < model->columnCount(); column++)
	{
		QStandardItem *item = model->item(defaultRow, column);
		QFont font = item->font();

		font.setBold(false);
		item->setFont(font);
	}
}

void Ui_pluginManagerWindow::checkAllItems(bool checked)
{
	for (int i = 0; i < ui.pluginTableView->model()->rowCount(); i++)
		((QStandardItemModel*)ui.pluginTableView->model())->item(i, 0)->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
}

void Ui_pluginManagerWindow::moveSelectedItem(bool up)
{
	const int sourceRow = ui.pluginTableView->selectionModel()->currentIndex().row();
	const int destRow = (up ? sourceRow - 1 : sourceRow);

	QStandardItemModel *model = (QStandardItemModel*)ui.pluginTableView->model();
	QList<QStandardItem*> sourceItems = model->takeRow(sourceRow);
	QList<QStandardItem*> destItems = model->takeRow(destRow);

	model->insertRow(destRow, sourceItems);
	model->insertRow(sourceRow, destItems);

	ui.pluginTableView->selectRow(up ? destRow : sourceRow + 1);
}

void Ui_pluginManagerWindow::save(void)
{
	UiPluginManager manager;
	QStandardItemModel *model = (QStandardItemModel*)ui.pluginTableView->model();

	for (int i = 0; i < model->rowCount(); i++)
	{
		QStandardItem *item = model->item(i, 0);

		manager.addPlugin(
			UiPluginManager::PLUGINTYPE_VIDEO_ENCODER,
			item->data(PLUGIN_ID).toString().toUtf8().constData(),
			item->checkState() == Qt::Checked,
			item->data(PLUGIN_IS_DEFAULT).toBool());
	}

	manager.save();
}

uint8_t DIA_pluginManager(void)
{
	Ui_pluginManagerWindow dialog(qtLastRegisteredDialog());
	qtRegisterDialog(&dialog);

	if (dialog.exec() == QDialog::Accepted)
		dialog.save();

	qtUnregisterDialog(&dialog);

	return 1;
}
