/**
    Jobs dialog
    (c) Mean 2007
*/

#include "Q_jobs.h"
#include "DIA_coreToolkit.h"
#include "ADM_toolkitQt.h"

static void updateStatus(void);
extern bool parseECMAScript(const char *name);
static QString StringStatus[]={jobsWindow::tr("Ready"), jobsWindow::tr("Succeeded"), jobsWindow::tr("Failed"), jobsWindow::tr("Deleted"), jobsWindow::tr("Running")};

ADM_Job_Descriptor::ADM_Job_Descriptor(void) 
{
	status = STATUS_READY;
	memset(&startDate, 0, sizeof(startDate));
	memset(&endDate, 0, sizeof(startDate));
}

 /**
          \fn jobsWindow
 */
jobsWindow::jobsWindow(QWidget *parent, uint32_t n,char **j) : QDialog(parent)
{
	ui.setupUi(this);
	_nbJobs=n;
	_jobsName=j;
	desc=new ADM_Job_Descriptor[n];
	// Setup display
	ui.tableWidget->setRowCount(_nbJobs);
	ui.tableWidget->setColumnCount(4);

	// Set headers
	QStringList headers;
	headers << tr("Job Name") << tr("Status") << tr("Start Time") << tr("End Time"); 

	ui.tableWidget->setVerticalHeaderLabels(headers);
	updateRows();

#define CNX(x) connect( ui.pushButton##x,SIGNAL(clicked(bool)),this,SLOT(x(bool)))
	//connect( ui.pushButtonRunOne,SIGNAL(buttonPressed(const char *)),this,SLOT(runOne(const char *)));
	CNX(RunOne);
	CNX(RunAll);
	CNX(DeleteAll);
	CNX(DeleteOne);
}
 /**
    \fn ~jobsWindow
 */
jobsWindow::~jobsWindow()
{
	delete [] desc;
}
 
static void ADM_setText(QString txt,uint32_t col, uint32_t row,QTableWidget *w)
{
	QTableWidgetItem *newItem = new QTableWidgetItem(txt);

	w->setItem(row, col, newItem);  
}
 /**
      \fn updateRaw
      \brief update display for raw x
 */
void jobsWindow::updateRows(void)
{
   ui.tableWidget->clear();
   ADM_Job_Descriptor *j;
   char str[20];
   for(int i=0;i<_nbJobs;i++)
   {
      j=&(desc[i]);
      ADM_setText(QString(ADM_GetFileName(_jobsName[i])),0,i,ui.tableWidget);
      ADM_setText(StringStatus[j->status],1,i,ui.tableWidget);
      ADM_setText(tr("%1:%2:%3").arg((uint)j->startDate.hours, 2, 10, QLatin1Char('0')).arg((uint)j->startDate.minutes, 2, 10, QLatin1Char('0')).arg((uint)j->startDate.seconds, 2, 10, QLatin1Char('0')), 2, i, ui.tableWidget);
	  ADM_setText(tr("%1:%2:%3").arg((uint)j->endDate.hours, 2, 10, QLatin1Char('0')).arg((uint)j->endDate.minutes, 2, 10, QLatin1Char('0')).arg((uint)j->endDate.seconds, 2, 10, QLatin1Char('0')), 3, i, ui.tableWidget);
   }
}

                                                                 
                                                                 
/**
      \fn deleteOne
      \brief delete one job
*/
void jobsWindow::DeleteOne(bool b)
{
	int sel = ui.tableWidget->currentRow();

	if (sel >= 0 && sel < _nbJobs)
	{
		if (GUI_Confirmation_HIG(tr("Sure!").toUtf8().constData(), tr("Delete job").toUtf8().constData(), tr("Are you sure you want to delete %s job?").toUtf8().constData(), ADM_GetFileName(_jobsName[sel])))
		{
			desc[sel].status = STATUS_DELETED;
			unlink(_jobsName[sel]);
			updateRows();
		}
	}
}
/**
      \fn deleteAll
      \brief delete all job
*/
void jobsWindow::DeleteAll(bool b)
{
	if (GUI_Confirmation_HIG(tr("Sure!").toUtf8().constData(), tr("Delete *all* job").toUtf8().constData(), tr("Are you sure you want to delete ALL jobs?").toUtf8().constData()))
	{
		for(int sel = 0; sel < _nbJobs; sel++)
		{
			desc[sel].status = STATUS_DELETED;
			unlink(_jobsName[sel]);
		}

		updateRows();
	}
}
                                                        
/**
      \fn runOne
      \brief Run one job
*/
void jobsWindow::RunOne(bool b)
{
	int sel = ui.tableWidget->currentRow();
	printf("Selected %d\n", sel);

	if(sel >= 0 && sel < _nbJobs)
	{
		if(desc[sel].status == STATUS_SUCCEED)
			GUI_Info_HIG(ADM_LOG_INFO,tr("Already done").toUtf8().constData(),tr("This script has already been successfully executed.").toUtf8().constData());
		else
		{
			desc[sel].status=STATUS_RUNNING;
			updateRows();
			GUI_Quiet();
			TLK_getDate(&(desc[sel].startDate));

			if(parseECMAScript(_jobsName[sel]))
				desc[sel].status=STATUS_SUCCEED;
			else
				desc[sel].status=STATUS_FAILED;

			TLK_getDate(&(desc[sel].endDate));
			updateRows();
			GUI_Verbose();
		}
	}
}
/**
      \fn RunAll
      \brief Run all jobs
*/
void jobsWindow::RunAll(bool b)
{
	for(int sel=0;sel<_nbJobs;sel++)
	{
		if(desc[sel].status == STATUS_SUCCEED || desc[sel].status == STATUS_DELETED)
			continue;

		desc[sel].status=STATUS_RUNNING;
		updateRows();
		GUI_Quiet();
		TLK_getDate(&(desc[sel].startDate));

		if(parseECMAScript(_jobsName[sel]))
			desc[sel].status=STATUS_SUCCEED;
		else
			desc[sel].status=STATUS_FAILED;

		TLK_getDate(&(desc[sel].endDate));
		updateRows();
		GUI_Verbose();
	}
}

/**
    \fn     DIA_job
    \brief  
*/
uint8_t  DIA_job(uint32_t nb, char **name)
{
  uint8_t r=0;
  jobsWindow jobswindow(qtLastRegisteredDialog(), nb,name);

  qtRegisterDialog(&jobswindow);
     
     if(jobswindow.exec()==QDialog::Accepted)
     {
       r=1;
     }

	 qtUnregisterDialog(&jobswindow);

     return r;
}

//EOF
