
// C++ Interface: 
//
// Description: 
//
//
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <dirent.h>

#include <QtCore/QVariant>
#include <QtGui/qfiledialog.h>

#include "ADM_default.h"
#include "DIA_fileSel.h"
#include "DIA_coreToolkit.h"
#include "DIA_coreUI_internal.h"
#include "ADM_toolkitQt.h"

#include "prefs.h"

namespace ADM_QT4_fileSel
{
	/**
	  \fn GUI_FileSelWrite (const char *label, char **name, uint32_t access) 
	  \brief Ask for a file for reading (access=0) or writing (access=1)
	*/
	void GUI_FileSelSelect(const char *label, char **name, uint32_t access)
	{
		char *tmpname = NULL;
		char *str = NULL;
		options pref_entry = LASTDIR_READ;

		*name = NULL;

		if (access)
			pref_entry = LASTDIR_WRITE;

		if (prefs->get(pref_entry,(ADM_filename **)&tmpname))
		{
			DIR *dir;
			str = ADM_PathCanonize(tmpname);
			ADM_PathStripName(str);

			/* LASTDIR may have gone; then do nothing and use current dir instead (implied) */
			if (dir = opendir(str))
				closedir(dir);
		}

		delete [] tmpname;

		QString fileName;

		if(access)  fileName=QFileDialog::getSaveFileName(qtLastRegisteredDialog(), QString::fromUtf8(label), QString::fromUtf8(str));
		else    fileName=QFileDialog::getOpenFileName(qtLastRegisteredDialog(), QString::fromUtf8(label), QString::fromUtf8(str));

		if (!fileName.isNull())
		{
			fileName = QDir::toNativeSeparators(fileName);
			const char *s=fileName.toUtf8().constData();
			*name=ADM_strdup(s);
			prefs->set(pref_entry,(ADM_filename *)*name);
		}

		if (str)
			delete [] str;
	}

	void GUI_FileSelRead(const char *label, char **name)
	{
		GUI_FileSelSelect(label, name, 0);
	}

	void GUI_FileSelWrite(const char *label, char **name)
	{
		GUI_FileSelSelect(label, name, 1);
	}

	void GUI_FileSelRead(const char *label, SELFILE_CB cb)
	{
		char *name;

		GUI_FileSelRead(label, &name);

		if (name)
		{
			cb(name); 
			ADM_dealloc(name);
		}
	}

	void GUI_FileSelWrite(const char *label, SELFILE_CB cb)
	{
		char *name;

		GUI_FileSelWrite(label, &name);

		if (name)
		{
			cb(name); 
			ADM_dealloc(name);
		}
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
	uint8_t FileSel_SelectWrite(const char *title, char *target, uint32_t max, const char *source)
	{
		char *dir=NULL,*tmpname=NULL;
		QString defaultPath, fileName;

		if (source && *source)
			defaultPath = QString::fromUtf8(source);
		else if (prefs->get(LASTDIR_WRITE, (ADM_filename **)&tmpname))
		{
			DIR *dir;
			char *str = ADM_PathCanonize(tmpname);

			ADM_PathStripName(str);

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
	uint8_t FileSel_SelectRead(const char *title, char *target, uint32_t max, const char *source)
	{
		char *dir=NULL,*tmpname=NULL;
		QString defaultPath, fileName;

		if (source && *source)
			defaultPath = QString::fromUtf8(source);
		else if (prefs->get(LASTDIR_READ,(ADM_filename **)&tmpname))
		{
			DIR *dir;
			char *str = ADM_PathCanonize(tmpname);

			ADM_PathStripName(str);

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
	uint8_t FileSel_SelectDir(const char *title, char *target, uint32_t max, const char *source)
	{
		char *dir=NULL,*tmpname=NULL;
		QString defaultPath, fileName;

		if (source && *source)
			defaultPath = QString::fromUtf8(source);
		else if (prefs->get(LASTDIR_READ,(ADM_filename **)&tmpname))
		{
			DIR *dir;
			char *str = ADM_PathCanonize(tmpname);

			ADM_PathStripName(str);

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

	/**
		 * 
	*/
	void init(void)
	{
		// Nothing special to do for QT4 fileselector
	}
} // End of nameSpace

static DIA_FILESEL_DESC_T Qt4FileSelDesc =
{
	ADM_QT4_fileSel::init,
	ADM_QT4_fileSel::GUI_FileSelRead,
	ADM_QT4_fileSel::GUI_FileSelWrite,
	ADM_QT4_fileSel::GUI_FileSelRead,
	ADM_QT4_fileSel::GUI_FileSelWrite,
	ADM_QT4_fileSel::FileSel_SelectRead,
	ADM_QT4_fileSel::FileSel_SelectWrite,
	ADM_QT4_fileSel::FileSel_SelectDir
};

// Hook our functions
void initFileSelector(void)
{
	DIA_fileSelInit(&Qt4FileSelDesc);
}

