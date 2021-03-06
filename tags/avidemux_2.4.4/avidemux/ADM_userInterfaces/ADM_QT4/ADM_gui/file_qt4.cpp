
// C++ Interface: 
//
// Description: 
//
//
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "default.h"
#include <QtCore/QVariant>
#include <QtGui/qfiledialog.h>

#include "ADM_toolkit/toolkit.hxx"
#include "ADM_toolkit/filesel.h"
#include "prefs.h"
#include "ADM_assert.h"
#include "../ADM_toolkit/qtToolkit.h"
    
static void GUI_FileSelSelect(const char *label, char **name, uint32_t access) ;

//****************************************************************************************************
void GUI_FileSelRead(const char *label,SELFILE_CB *cb) 
{
  char *name;
  GUI_FileSelRead(label,&name);
  if(name)
  {
    cb(name); 
    ADM_dealloc(name);
  }
}
//****************************************************************************************************
void GUI_FileSelWrite(const char *label,SELFILE_CB *cb) 
{
  char *name;
  GUI_FileSelWrite(label,&name);
  if(name)
  {
    cb(name); 
    ADM_dealloc(name);
  }

}
//****************************************************************************************************
void GUI_FileSelRead(const char *label, char * * name)
{
 GUI_FileSelSelect(label, name, 0) ;
}
//****************************************************************************************************
void GUI_FileSelWrite(const char *label, char * * name)
{
 GUI_FileSelSelect(label, name, 1) ;

}
/**
  \fn GUI_FileSelWrite (const char *label, char **name, uint32_t access) 
  \brief Ask for a file for reading (access=0) or writing (access=1)

*/
void GUI_FileSelSelect(const char *label, char **name, uint32_t access) 
{
  char *dir=NULL,*tmpname=NULL;
  *name=NULL;
  char *str=NULL;
  options pref_entry=LASTDIR_READ;
  if(access) pref_entry=LASTDIR_WRITE;
  
  if(prefs->get(pref_entry,(ADM_filename **)&tmpname))
  {
     DIR *dir;
        str=PathCanonize(tmpname);
        PathStripName(str);
        /* LASTDIR may have gone; then do nothing and use current dir instead (implied) */
        if( (dir=opendir(str)) )
                closedir(dir);
  }

  delete [] tmpname;

  QString fileName;

   if(access)  fileName=QFileDialog::getSaveFileName(qtLastRegisteredDialog(), QString::fromUtf8(label), QString::fromUtf8(str));
       else    fileName=QFileDialog::getOpenFileName(qtLastRegisteredDialog(), QString::fromUtf8(label), QString::fromUtf8(str));

  if(!fileName.isNull())
  {
	fileName = QDir::toNativeSeparators(fileName);
    const char *s=fileName.toUtf8().constData();
    *name=ADM_strdup(s);
    prefs->set(pref_entry,(ADM_filename *)*name);
  }
  if(str) delete [] str;
  
}
/**
      \fn FileSel_SelectWrite
      \brief select file, write mode
      @param title window title
      @param target where to store result
      @param max Max buffer size in bytes
      @param source Original value
      @return 1 on success, 0 on failure

*/
uint8_t FileSel_SelectWrite(const char *title,char *target,uint32_t max, const char *source)
{
  char *dir=NULL,*tmpname=NULL;
  QString defaultPath, fileName;

  if (source && *source)
	  defaultPath = QString::fromUtf8(source);
  else if (prefs->get(LASTDIR_WRITE, (ADM_filename **)&tmpname))
  {
	  DIR *dir;
	  char *str = PathCanonize(tmpname);

	  PathStripName(str);

	  if (dir = opendir(str))
		  closedir(dir);

	  defaultPath = QString::fromUtf8(str);
	  delete [] str;
  }

  delete [] tmpname;

  fileName=QFileDialog::getSaveFileName(qtLastRegisteredDialog(), QString::fromUtf8(title), defaultPath);

  if(!fileName.isNull() )
  {
	fileName = QDir::toNativeSeparators(fileName);
    const char *s=fileName.toUtf8().constData();
    strncpy(target,s,max);
	prefs->set(LASTDIR_WRITE, (ADM_filename *)s);
  }
  
  return !fileName.isNull();
}
/**
      \fn FileSel_SelectRead
      \brief select file, read mode
      @param title window title
      @param target where to store result
      @param max Max buffer size in bytes
      @param source Original value
      @return 1 on success, 0 on failure

*/
uint8_t FileSel_SelectRead(const char *title,char *target,uint32_t max, const char *source)
{
  char *dir=NULL,*tmpname=NULL;
  QString defaultPath, fileName;

  if (source && *source)
	  defaultPath = QString::fromUtf8(source);
  else if (prefs->get(LASTDIR_READ,(ADM_filename **)&tmpname))
  {
	  DIR *dir;
	  char *str = PathCanonize(tmpname);

	  PathStripName(str);

	  if (dir = opendir(str))
		  closedir(dir);

	  defaultPath = QString::fromUtf8(str);
	  delete [] str;
  }

  delete [] tmpname;

  fileName = QFileDialog::getOpenFileName(qtLastRegisteredDialog(), QString::fromUtf8(title), defaultPath);

  if(!fileName.isNull() )
  {
	fileName = QDir::toNativeSeparators(fileName);
    const char *s=fileName.toUtf8().constData();
    strncpy(target,s,max);
    prefs->set(LASTDIR_READ, (ADM_filename *)s);
  }
  
  return !fileName.isNull();
}
/**
      \fn FileSel_SelectDir
      \brief select directory
      @param title window title
      @param target where to store result
      @param max Max buffer size in bytes
      @param source Original value
      @return 1 on success, 0 on failure

*/

uint8_t FileSel_SelectDir(const char *title,char *target,uint32_t max, const char *source)
{
   char *dir=NULL,*tmpname=NULL;
   QString defaultPath, fileName;

   if (source && *source)
	   defaultPath = QString::fromUtf8(source);
   else if (prefs->get(LASTDIR_READ,(ADM_filename **)&tmpname))
   {
	   DIR *dir;
	   char *str = PathCanonize(tmpname);

	   PathStripName(str);

	   if (dir = opendir(str))
		   closedir(dir);

	   defaultPath = QString::fromUtf8(str);
	   delete [] str;
   }

   delete [] tmpname;

   fileName=QFileDialog::getExistingDirectory(qtLastRegisteredDialog(), QString::fromUtf8(title), defaultPath, QFileDialog::ShowDirsOnly);

  if(!fileName.isNull() )
  {
	fileName = QDir::toNativeSeparators(fileName);
    const char *s=fileName.toUtf8().constData();
    strncpy(target,s,max);
	prefs->set(LASTDIR_READ, (ADM_filename *)s);
  }

  return !fileName.isNull();
}



//****************************************************************************************************
//EOF
