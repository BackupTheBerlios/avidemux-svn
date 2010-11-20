/**
    \file   ADM_jobControl
    \author mean fixounet@free.Fr (c) 2010

*/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "T_jobs.h"
#include "ADM_default.h"
#include "ADM_coreJobs.h"


static QTableWidgetItem *fromText(const string &t,int id)
{
    QString s(QString::fromUtf8(t.c_str()));
    QTableWidgetItem *w=new QTableWidgetItem(s,id);
    Qt::ItemFlags flags=w->flags();
    flags&=~Qt::ItemIsEditable;
    w->setFlags(flags);
    return w;
}
string date2String(uint64_t date)
{
    if(!date) return string("N/A");
    return string(ADM_epochToString(date));
}
/**
    \fn refreshList
*/
void jobWindow::refreshList(void)
{
      QTableWidget *list=ui.tableWidget;
      list->clear();
      listOfJob.clear();
      
      
      
      
// set titles
     QTableWidgetItem *jb=fromText("Job",255);
     QTableWidgetItem *outputFile=fromText("Output",255);
     QTableWidgetItem *status=fromText("Status",255);
     QTableWidgetItem *start=fromText("Start Time",255);
     QTableWidgetItem *end=fromText("End Time",255);
     ui.tableWidget->setHorizontalHeaderItem(0,jb);
     ui.tableWidget->setHorizontalHeaderItem(1,outputFile);
     ui.tableWidget->setHorizontalHeaderItem(2,start);
     ui.tableWidget->setHorizontalHeaderItem(3,end);
     ui.tableWidget->setHorizontalHeaderItem(4,status);


      if(false==ADM_jobGet(listOfJob)) return ;

      int n=listOfJob.size();
      ADM_info("Found %d jobs\n",(int)n);
      if(!n) return;
      list->setRowCount(n);
      for(int i=0;i<n;i++)
      {
           QTableWidgetItem *nm=fromText(listOfJob[i].jobName,i);
           QTableWidgetItem *out=fromText(listOfJob[i].outputFileName,i);
           string s;
           string start="X";
           string end="X";
            switch(listOfJob[i].status)
            {
                case ADM_JOB_IDLE:
                            s=string("Ready");
                            break;
                case ADM_JOB_RUNNING:
                            s=string("Running....");
                            break;
                case ADM_JOB_OK:
                            s=string("Success");
                            start=date2String(listOfJob[i].startTime);
                            end=date2String(listOfJob[i].endTime);
                            break;
                case ADM_JOB_KO:
                            s=string("Failed");
                            start=date2String(listOfJob[i].startTime);
                            end=date2String(listOfJob[i].endTime);
                            break;
                default:
                            s=string("???");
                            break;
            }
        QTableWidgetItem *status=fromText (s,i);
        QTableWidgetItem *startItem=fromText (start,i);
        QTableWidgetItem *endItem=fromText (end,i);
        list->setItem(i,0,nm);
        list->setItem(i,1,out);
        list->setItem(i,2,startItem);
        list->setItem(i,3,endItem);
        list->setItem(i,4,status);
      }
      list->resizeColumnsToContents();
}


/**
    \fn ctor
*/
#define QT_NOOP(x) x
jobWindow::jobWindow(void) : QDialog()
{
    ui.setupUi(this);
    ui.tableWidget->setColumnCount(5); // Job name, fileName, Status

    // Add some right click menu...
    ui.tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    // Add some right click menu...
   QAction *del = new  QAction(QString("Delete"),this);
   QAction *runNow = new  QAction(QString("Run Now"),this);
   QAction *setOk = new  QAction(QString("Force Status to success"),this);
   QAction *setReady = new  QAction(QString("Force Status to ready"),this);
   ui.tableWidget->addAction(del);
   ui.tableWidget->addAction(runNow);
   ui.tableWidget->addAction(setOk);
   ui.tableWidget->addAction(setReady);
   connect(del,SIGNAL(activated()),this,    SLOT(del()));
   connect(runNow,SIGNAL(activated()),this, SLOT(runNow()));
   connect(setOk,SIGNAL(activated()),this,  SLOT(setOk()));
   connect(setReady,SIGNAL(activated()),this,SLOT(setReady()));

   connect(ui.pushButtonQuit,SIGNAL(pressed()),this,SLOT(quit()));
   connect(ui.pushButtonRunAll,SIGNAL(pressed()),this,SLOT(runAllJob()));

   refreshList();

}
/**
    \fn dtor
*/
jobWindow::~jobWindow()
{

}

/**
    \fn jobRun
*/
bool jobRun(int ac,char **av)
{
    QApplication *app=new QApplication(ac,av,0);
    jobWindow *jWindow=new jobWindow();
    jWindow->exec();
    delete jWindow;
    delete app;
    return true;
}
/**
    \fn del
    \brief delete the currently selected jobs
*/
int jobWindow::getActiveIndex(void)
{
        QTableWidgetItem *item=ui.tableWidget->currentItem();
        if(!item) return -1;
        int index=item->type(); 
        return index;
}
/**
    \fn runAction
*/
void jobWindow::runAction(JobAction action)
{
        int index=getActiveIndex();
        ADM_info("%d command for index %d\n",action,index);
        if(index==-1) return;
        // get the job
        if(index>listOfJob.size())
        {
            ADM_error("index out of bound : %d/%d\n",index,listOfJob.size());
            return ;
        }
        ADMJob *j=&(listOfJob[index]);
        switch(action)
        {
            case   JobAction_setReady:          
                        j->status=ADM_JOB_IDLE;
                        ADM_jobUpdate(*j);
                        refreshList();
                        break;
            case   JobAction_setOk:
                        j->status=ADM_JOB_OK;
                        ADM_jobUpdate(*j);
                        refreshList();
                        break;
            case   JobAction_runNow:
                        {
                            runOneJob(*j);
                        }
                        break;
                        
            default:
                        ADM_warning("Command %d no handled\n",action);
                        break;
        }
}

void jobWindow::del(void)
{
    runAction(JobAction_delete);
}
void jobWindow::setOk(void)
{
    runAction(JobAction_setOk);
}
void jobWindow::setReady(void)
{
    runAction(JobAction_setReady);
}
void jobWindow::runNow(void)
{
    runAction(JobAction_runNow);
}
 
/**
    \fn quit
    \brief run selected jobs whatever its status
*/

void jobWindow::quit(void)
{
    done(1);
}
/**
    \fn runAllJob
*/
void jobWindow::runAllJob(void)
{
    int n=listOfJob.size();
    for(int i=0;i<n;i++)
    {
            ADMJob *j=&(listOfJob[i]);
            if(j->status==ADM_JOB_IDLE)
                runOneJob(*j);
    }
    return ;
}
#if 0
bool jobRun(void)
{

        ADM_jobDropAllJobs();

        vector <ADMJob> jobs;
        ADM_jobGet(jobs);

        int n=jobs.size();
        printf("Found %d jobs...\n",n);

        ADMJob job;
        job.endTime=2;
        job.startTime=3;
        job.scriptName="myscript1";
        job.jobName="myjob1";
        job.outputFileName="output1";
        
        ADM_jobAdd(job);


        
        ADM_jobGet(jobs);

        n=jobs.size();
        printf("Found %d jobs...\n",n);
        for(int i=0;i<n;i++)
        {
            printf("%d/%d ",i,n);
            ADM_jobDump(jobs[i]);
        }
        printf("\n***************************\n");
        jobs[0].status=ADM_JOB_KO;
        ADM_jobUpdate(jobs[0]);
        n=jobs.size();
        printf("Found %d jobs...\n",n);
        for(int i=0;i<n;i++)
        {
            printf("%d/%d ",i,n);
            ADM_jobDump(jobs[i]);
        }
        printf("\n***************************\n");
        // Delete
        ADM_jobDelete(jobs[0]);
        ADM_jobGet(jobs);

        n=jobs.size();
        printf("Found %d jobs...\n",n);
        for(int i=0;i<n;i++)
        {
            printf("%d/%d ",i,n);
            ADM_jobDump(jobs[i]);
        }
        return true;
}
#endif
