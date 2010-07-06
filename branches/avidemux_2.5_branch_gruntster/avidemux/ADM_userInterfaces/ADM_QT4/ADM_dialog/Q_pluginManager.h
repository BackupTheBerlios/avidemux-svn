#ifndef Q_pluginManager_h
#define Q_pluginManager_h

#include <QtGui/QStandardItemModel>
#include "ui_pluginManager.h"
#include "UiPluginManager.h"

using namespace std;

class Ui_pluginManagerWindow : public QDialog
{
	Q_OBJECT

private:
	typedef struct
	{
		string name;
		string version;
		string description;
	} UiPlugVersionInfo;

	Ui_pluginManagerDialog ui;

	void fillVideoEncoderList(UiPluginManager manager);
	void addRow(QString id, QString name, QString version, QString desc, bool enabled, bool isDefault);
	void checkAllItems(bool check);
	void setDefaultRow(int defaultRow);
	void clearDefaultRow(int defaultRow);
	void moveSelectedItem(bool up);

public:
	Ui_pluginManagerWindow(QWidget* parent);
	void save(void);

private slots:
	void pluginTableView_rowChanged(const QModelIndex current, const QModelIndex previous);
	void pluginTableView_itemChanged(QStandardItem *item);
	void setDefaultButton_clicked(bool);
	void enableAllButton_clicked(bool);
	void disableAllButton_clicked(bool);
	void moveUpButton_clicked(bool);
	void moveDownButton_clicked(bool);
};

#endif	// Q_pluginManager_h
