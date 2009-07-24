#ifndef T_configMenu_h
#define T_configMenu_h
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include "DIA_factory.h"

namespace ADM_Qt4Factory
{
	class ADM_QconfigMenu : public QWidget
	{
		Q_OBJECT

	private:
		const char *userConfigDir, *systemConfigDir;
		diaElem **controls;
		unsigned int controlCount;
		bool disableGenericSlots;

		CONFIG_MENU_CHANGED_T *changedFunc;
		CONFIG_MENU_SERIALIZE_T *serializeFunc;

		void fillConfigurationComboBox(void);
		bool selectConfiguration(QString *selectFile, ConfigMenuType configurationType);

	public slots:
		void deleteClicked(bool checked);
		void saveAsClicked(bool checked);
		void comboboxIndexChanged(int index);

	public:
		QLabel *label;
		QComboBox *combobox;
		QPushButton *saveAsButton;
		QPushButton *deleteButton;

		ADM_QconfigMenu(QWidget *widget, QGridLayout *layout, int line,
			const char* userConfigDir, const char* systemConfigDir, CONFIG_MENU_CHANGED_T *changedFunc,
			CONFIG_MENU_SERIALIZE_T *serializeFunc, diaElem **controls, unsigned int controlCount);
		~ADM_QconfigMenu();
	};
}
#endif	// T_configMenu_h
