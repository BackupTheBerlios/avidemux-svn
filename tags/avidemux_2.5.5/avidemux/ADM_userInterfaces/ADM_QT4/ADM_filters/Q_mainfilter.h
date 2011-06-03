#ifndef Q_mainfilter_h
#define Q_mainfilter_h

#include <QtGui/QItemDelegate>
#include "ui_mainfilter.h"
#include "Q_seekablePreview.h"

class FilterItemEventFilter : public QObject
{
	Q_OBJECT

protected:
	bool eventFilter(QObject *object, QEvent *event);

public:
	FilterItemEventFilter(QWidget *parent = 0);
};

class FilterItemDelegate : public QItemDelegate
{
	Q_OBJECT

private:
	FilterItemEventFilter *filter;

public:
	FilterItemDelegate(QWidget *parent = 0);
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class filtermainWindow : public QDialog
{
	Q_OBJECT

public:
	filtermainWindow(QWidget *parent);
	~filtermainWindow();
	void buildActiveFilterList(void);

	Ui_mainFilterDialog ui;
	QListWidget *availableList;
	QListWidget *activeList;

public slots:
	void add(bool b);
	void up(bool b);
	void down(bool b);
	void remove(bool b);
	void configure(bool b);
	void partial(bool b);
	void loadScript(bool b);
	void saveScript(bool b);
	void activeDoubleClick( QListWidgetItem  *item);
	void activeItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void allDoubleClick( QListWidgetItem  *item);
	void filterFamilyItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void preview(bool b);
	void closePreview();
    // right click
    void add(void);
    void remove(void);
    void configure(void);

private:
	uint32_t previewFrameIndex;
	int previewDialogX, previewDialogY;
	Ui_seekablePreviewWindow *previewDialog;

	void setSelected(int sel);
	void displayFamily(uint32_t family);
	void setupFilters(void);
};
#endif	// Q_mainfilter_h
