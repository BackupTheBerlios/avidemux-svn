#include "config.h"

#ifdef HAVE_GETTEXT
#  include <libintl.h>
#endif

#include <QMessageBox>

#include "default.h"
#include "prefs.h"
#include "ADM_toolkit/toolkit.hxx"
#include "../ADM_toolkit/qtToolkit.h"

static int beQuiet=0;
extern QWidget *QuiMainWindows;

//****************************************************************************************************
void            GUI_Alert(const char *alertstring)
{
    QMessageBox::StandardButton reply;
              reply = QMessageBox::critical(qtLastRegisteredDialog(), QString::fromUtf8(QT_TR_NOOP("Alert")),
                                        QString::fromUtf8(alertstring),
                                        QMessageBox::Ok );
}
//****************************************************************************************************
void            GUI_Info(const char *alertstring)
{
    QMessageBox::StandardButton reply;
              reply = QMessageBox::information(qtLastRegisteredDialog(), QString::fromUtf8(QT_TR_NOOP("Info")),
                                        QString::fromUtf8(alertstring),
                                        QMessageBox::Ok );
}

//****************************************************************************************************
void GUI_Info_HIG(const ADM_LOG_LEVEL level,const char *primary, const char *secondary_format, ...)
{
	uint32_t msglvl = 2;
	QString alertString;

	prefs->get(MESSAGE_LEVEL, &msglvl);

	if (msglvl<level)
	{
		printf("Info: %s\n",primary);
		return;
	}

	if (!secondary_format)
		alertString = "<big><b>" + QString::fromUtf8(primary) + "</b></big>";
	else
	{
		va_list ap;
		va_start(ap, secondary_format);

		alertString.vsprintf(secondary_format, ap);
		alertString = QString::fromUtf8(alertString.toAscii().constData());
		alertString = "<big><b>" + QString::fromUtf8(primary) + "</b></big><br><br>" + alertString;
		alertString.replace("\n", "<br>");

		va_end(ap);
	}

	QMessageBox::StandardButton reply;

	reply = QMessageBox::information(qtLastRegisteredDialog(), QString::fromUtf8(QT_TR_NOOP("Info")),
		alertString, QMessageBox::Ok);
}
//****************************************************************************************************
void GUI_Error_HIG(const char *primary, const char *secondary_format, ...)
{
	uint32_t msglvl = 2;
	QString alertString;

	prefs->get(MESSAGE_LEVEL, &msglvl);

	if (msglvl == ADM_LOG_NONE)
	{
		printf("Error: %s\n", primary);
		return;
	}

	if (!secondary_format)
		alertString = "<big><b>" + QString::fromUtf8(primary) + "</b></big>";
	else
	{
		va_list ap;
		va_start(ap, secondary_format);

		alertString.vsprintf(secondary_format, ap);
		alertString = QString::fromUtf8(alertString.toAscii().constData());
		alertString = "<big><b>" + QString::fromUtf8(primary) + "</b></big><br><br>" + alertString;
		alertString.replace("\n", "<br>");

		va_end(ap);
	}

	QMessageBox::StandardButton reply;

	reply = QMessageBox::critical(qtLastRegisteredDialog(), QString::fromUtf8(QT_TR_NOOP("Info")),
		alertString, QMessageBox::Ok);
}
//****************************************************************************************************
int GUI_Confirmation_HIG(const char *button_confirm, const char *primary, const char *secondary_format, ...)
{
	QString alertString;

	if (beQuiet)
	{
		printf("Info: %s\n", primary);
		return 0;
	}

	if (!secondary_format)
		alertString = "<big><b>" + QString::fromUtf8(primary) + "</b></big>";
	else
	{
		va_list ap;
		va_start(ap, secondary_format);

		alertString.vsprintf(secondary_format, ap);
		alertString = QString::fromUtf8(alertString.toAscii().constData());
		alertString = "<big><b>" + QString::fromUtf8(primary) + "</b></big><br><br>" + alertString;
		alertString.replace("\n", "<br>");

		va_end(ap);
	}

	QMessageBox::StandardButton reply;

	reply = QMessageBox::question(qtLastRegisteredDialog(), QString::fromUtf8(QT_TR_NOOP("Confirmation")),
		alertString, QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
		return 1;

	return 0;
}
//****************************************************************************************************
int GUI_YesNo(const char *primary, const char *secondary_format, ...)
{
	uint32_t msglvl = 2;
	QString alertString;

	if (beQuiet)
	{
		printf("Info: %s\n", primary);
		return 0;
	}

	if (!secondary_format)
		alertString = "<big><b>" + QString::fromUtf8(primary) + "</b></big>";
	else
	{
		va_list ap;
		va_start(ap, secondary_format);

		alertString.vsprintf(secondary_format, ap);
		alertString = QString::fromUtf8(alertString.toAscii().constData());
		alertString = "<big><b>" + QString::fromUtf8(primary) + "</b></big><br><br>" + alertString;
		alertString.replace("\n", "<br>");

		va_end(ap);
	}

	QMessageBox::StandardButton reply;

	reply = QMessageBox::question(qtLastRegisteredDialog(), QString::fromUtf8(QT_TR_NOOP("Confirmation")),
		alertString, QMessageBox::Yes | QMessageBox::No);

	if (reply == QMessageBox::Yes)
		return 1;

	return 0; 
}
//****************************************************************************************************
int             GUI_Question(const char *alertstring)
{
 QMessageBox::StandardButton reply;
          if (beQuiet)
              {
                      printf("Question: %s\n", alertstring);
                      return 0;
              }
          reply = QMessageBox::question(qtLastRegisteredDialog(), QString::fromUtf8(QT_TR_NOOP("Question")),
                                    QString::fromUtf8(alertstring),
                                    QMessageBox::Yes | QMessageBox::No );
          if(reply==QMessageBox::Yes) return 1;
          return 0;
}
/**
    \fn GUI_Alternate(char *title,char *choice1,char *choice2)
    \brief Popup a dialog box name title with 2 buttons (choice1/choice2)
    \return 0 if first is selected, 1 if second
*/
int      GUI_Alternate(char *title,char *choice1,char *choice2)
{
	if (beQuiet)
	{
		printf("Alternate<%s>: %s or %s\n", title,choice1,choice2);
		return 0;
	}

	QMessageBox box(qtLastRegisteredDialog());

	box.setWindowTitle(QString::fromUtf8(QT_TR_NOOP("Question ?")));
	box.addButton(QString::fromUtf8(choice1),QMessageBox::YesRole);
	box.addButton(QString::fromUtf8(choice2),QMessageBox::NoRole);

	if (title)
		box.setText(QString::fromUtf8(title));
	else
		box.setText(QString::fromUtf8(QT_TR_NOOP("Question")));

	box.setIcon(QMessageBox::Question);

	if (box.exec() == 0)
		return 1;
	else
		return 0;
}
//****************************************************************************************************
uint8_t  GUI_getDoubleValue(double *valye, float min, float max, const char *title)
{
  
  
  return 0; 
}
//****************************************************************************************************
uint8_t		isQuiet(void)
{
    return beQuiet;
}
//****************************************************************************************************
void            GUI_Verbose(void)
{
    beQuiet=0;
}
//****************************************************************************************************
void            GUI_Quiet(void)
{
  beQuiet=1;
}
//****************************************************************************************************
//EOF
