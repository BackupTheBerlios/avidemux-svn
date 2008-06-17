/***************************************************************************
                    
    copyright            : (C) 2006 by mean
    email                : fixounet@free.fr
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef __WIN32
#include <windows.h>
#endif

#include "default.h"
#include "ADM_misc.h"


#include "ADM_toolkit/filesel.h"
#include "ADM_toolkit/toolkit.hxx"

#include "ADM_assert.h"
#undef fread
#undef fwrite
#undef fopen
#undef fclose

extern char * actual_workbench_file;

size_t ADM_fread (void *ptr, size_t size, size_t n, FILE *sstream)
{
  return fread(ptr,size,n,sstream); 
  
}

size_t ADM_fwrite (void *ptr, size_t size, size_t n, FILE *sstream)
{
  return fwrite(ptr,size,n,sstream); 
  
}
FILE  *ADM_fopen (const char *file, const char *mode)
{
#ifdef __MINGW32__
	int nFileLen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, file, -1, NULL, 0);
	int nModeLen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, mode, -1, NULL, 0);
	wchar_t wFile[nFileLen + 1];
	wchar_t wMode[nModeLen + 1];

	MultiByteToWideChar(CP_UTF8, 0, mode, -1, wMode, nModeLen + 1);
	MultiByteToWideChar(CP_UTF8, 0, file, -1, wFile, nFileLen + 1);

	return _wfopen(wFile, wMode);
#else
	return fopen(file, mode);
#endif
}

int    ADM_fclose (FILE *file)
{
  return fclose(file); 
}
//*****************************
static char basedir[1024]={0};
static char jobdir[1024]={0};
static char customdir[1024]={0};
int baseDirDone=0;
int jobDirDone=0;
int customDirDone=0;
#ifdef ADM_WIN32
const char *ADM_DIR_NAME="\\avidemux";
#else
const char *ADM_DIR_NAME="/.avidemux";
#endif
/*

*/
/*
      Get the  directory where jobs are stored
******************************************************/

char *ADM_getCustomDir(void)
{
  if(customDirDone) return customdir;

  char *rootDir;
  rootDir=ADM_getBaseDir();
  strncpy(customdir,rootDir,1023);
#if defined(ADM_WIN32)
  strcat(customdir,"\\custom"); 
#else
  strcat(customdir,"/custom");
#endif
  if(!ADM_mkdir(customdir))
  {
    GUI_Error_HIG(QT_TR_NOOP("Oops"),QT_TR_NOOP("can't create custom directory (%s)."),customdir);
                return NULL;
  }
  customDirDone=1;
  return customdir;
}
/*
      Get the  directory where jobs are stored
******************************************************/

char *ADM_getJobDir(void)
{
  if(jobDirDone) return jobdir;

  char *rootDir;
  rootDir=ADM_getBaseDir();
  strncpy(jobdir,rootDir,1023);
#if defined(ADM_WIN32)
  strcat(jobdir,"\\jobs"); 
#else
  strcat(jobdir,"/jobs");
#endif
  if(!ADM_mkdir(jobdir))
  {
    GUI_Error_HIG(QT_TR_NOOP("Oops"),QT_TR_NOOP("can't create job directory (%s)."),jobdir);
                return NULL;
  }
  jobDirDone=1;
  return jobdir;
}
/*
      Get the root directory for .avidemux stuff
******************************************************/
char *ADM_getBaseDir(void)
{
char *dirname=NULL;
DIR *dir=NULL;
char *home;
//
        if(baseDirDone) return basedir;
// Get the base directory
#if defined(ADM_WIN32)
        if( ! (home=getenv("USERPROFILE")) )
        {
          GUI_Error_HIG(QT_TR_NOOP("Oops"),QT_TR_NOOP("can't determine $USERPROFILE."));
                    home="c:\\";
        }

#else
        if( ! (home=getenv("HOME")) )
        {
          GUI_Error_HIG(QT_TR_NOOP("Oops"),QT_TR_NOOP("can't determine $HOME."));
                return NULL;
        }
#endif

 // Try to open the .avidemux directory
        dirname=new char[strlen(home)+strlen(ADM_DIR_NAME)+2];
        strcpy(dirname,home);
        strcat(dirname,ADM_DIR_NAME);
        if(!ADM_mkdir(dirname))
        {
          GUI_Error_HIG(QT_TR_NOOP("Oops"),QT_TR_NOOP("Cannot create the .avidemux directory"), NULL);
                        delete [] dirname;
                        return NULL;
        }
        delete [] dirname;

        // Now built the filename
        strncpy(basedir,home,1023);
        strncat(basedir,ADM_DIR_NAME,1023-strlen(basedir));
        baseDirDone=1;
        printf("Using %s as base directory for prefs/jobs/...\n",basedir);
        return basedir;
}
/*----------------------------------------
      Create a directory
      If it already exists, do nothing
------------------------------------------*/
uint8_t ADM_mkdir(const char *dirname)
{
DIR *dir=NULL;
              // Check it already exists ?
              dir=opendir(dirname);
              if(dir)
              { 
                  printf("Directory %s exists.Good.\n",dirname);
                  closedir(dir);
                  return 1;
              }
#if defined(ADM_WIN32)
                if(mkdir(dirname))
                {
                    printf("Oops: mkdir failed on %s\n",dirname);   
                    return 0;
                }
                
#else    
                char *sys=new char[strlen(dirname)+strlen("mkdir ")+2];
                strcpy(sys,"mkdir ");
                strcat(sys,dirname);
                printf("Creating dir :%s\n",sys);
                system(sys);
                delete [] sys;
#endif		
              if((dir=opendir(dirname))==NULL)
                {
                        return 0;
                }
              closedir(dir); 
              return 1;
}
uint8_t buildDirectoryContent(uint32_t *outnb,const char *base, char *jobName[],int maxElems,const char *ext)
{

DIR *dir;
struct dirent *direntry;
int dirmax=0,len;
int extlen=strlen(ext);
    ADM_assert(extlen);
    
         dir=opendir(base);
        if(!dir)
        {
                return 0;
        }
        while((direntry=readdir(dir)))
        {
                len=strlen(direntry->d_name);
                if(len<(extlen+1)) continue;
                int xbase=len-extlen;
                if(memcmp(direntry->d_name+xbase,ext,extlen))
                //if(direntry->d_name[len-1]!='s' || direntry->d_name[len-2]!='j' || direntry->d_name[len-3]!='.')
                {
                        printf("ignored:%s\n",direntry->d_name);
                        continue;
                }
                jobName[dirmax]=(char *)ADM_alloc(strlen(base)+strlen(direntry->d_name)+2);
                strcpy(jobName[dirmax],base);
                strcat(jobName[dirmax],"/");
                strcat(jobName[dirmax],direntry->d_name);
                dirmax++;
                if(dirmax>=maxElems)
                {
                        printf("[jobs]: Max # of jobs exceeded\n");
                         break;
                }
        }
        closedir(dir);
        *outnb=dirmax;
        return 1;
}
//------------------------------------------------------------------

/*

** note: it modifies it's first argument
*/
void simplify_path(char **buf){
   unsigned int last1slash = 0;
   unsigned int last2slash = 0;
	while( !strncmp(*buf,"/../",4) )
		memmove(*buf,*buf+3,strlen(*buf+3)+1);
	for(unsigned int i=0;i<strlen(*buf)-2;i++)
		while( !strncmp(*buf+i,"/./",3) )
			memmove(*buf+i,*buf+i+2,strlen(*buf+i+2)+1);
	for(unsigned int i=0;i<strlen(*buf)-3;i++){
		if( *(*buf+i) == '/' ){
			last2slash = last1slash;
			last1slash = i;
		}
		if( !strncmp(*buf+i,"/../",4) ){
			memmove(*buf+last2slash,*buf+i+3,strlen(*buf+i+3)+1);
			return simplify_path(buf);
		}
	}
}

//
//	Make it absolute
//
char *PathCanonize(const char *tmpname)
{
	char path[300];
	char *out ;

	if( ! getcwd(path,300) ){
		fprintf(stderr,"\ngetcwd() failed with: %s (%u)\n",
		               strerror(errno), errno );
		path[0] = '\0';
	}
	if(!tmpname || tmpname[0]==0)
	{
		out=new char [strlen(path)+2];
		strcpy(out,path);
#ifndef ADM_WIN32		
		strcat(out,"/");
#else
		strcat(out,"\\");
#endif	
		printf("\n Canonizing null string ??? (%s)\n",out);
	}else if(tmpname[0]=='/'
#if defined(ADM_WIN32)
		|| tmpname[1]==':'
#endif	
	
	)
	{
		out=new char[strlen(tmpname)+1];
		strcpy(out,tmpname);
		return out;
	}else{
		out=new char[strlen(path)+strlen(tmpname)+6];
		strcpy(out,path);
#ifndef ADM_WIN32		
		strcat(out,"/");
#else
		strcat(out,"\\");
#endif		
		strcat(out,tmpname);
	}
	simplify_path(&out);
	return out;
}
/*
	Strip the path and only keep the name

*/
void		PathStripName(char *str)
{
		int len=strlen(str);
		if(len<=1) return;
		len--;
#ifndef ADM_WIN32		
		while( *(str+len)!='/' && len)
#else
	while( *(str+len)!='\\' && len)
#endif		
		{
			 *(str+len)=0;
			 len--;
		}
}

/*
	Get the filename without path

*/
const char *GetFileName(const char *str)
{
	char *filename;
        char *filename2;
#ifndef ADM_WIN32		
	filename = strrchr(str, '/');
        
#else
	filename = strrchr(str, '\\');
        filename2 = strrchr(str, '/');
        if(filename2 && filename)
        {
          if(filename2>filename) filename=filename2; 
        }
#endif
	if (filename)
		return filename+1;
	else
		return str;
}

/*
	Split patch into absolute path+name and extention

*/
void PathSplit(const char *str, char **root, char **ext)
{
	char *full;
	uint32_t l;

		full=PathCanonize(str);
		// Search the last .
		l=strlen(full);
		l--;
		ADM_assert(l>0);
		while( *(full+l)!='.' && l) l--;
		if(!l || l==(strlen(full)-1))
		{
			if(l==(strlen(full)-1))
			{
				*(full+l)=0;  // remove trailing .
			}
			*ext=new char[2];
			*root=full;
			strcpy(*ext,"");
			return ;
		}
		// else we do get an extension
		// starting at l+1
		uint32_t suff;

		suff=strlen(full)-l-1;
		*ext=new char[suff+1];
		strcpy(*ext,full+l+1);
		*(full+l)=0;
		*root=full;
		return ;
}

// CYB 2005.02.23: DND
void fileReadWrite(SELFILE_CB *cb, int rw, const char *name)
{

	if(name)
	{
		if(cb)
		{
			FILE *fd;
			fd=ADM_fopen(name,"rb");
			if(rw==0) // read
			{
				// try to open it..
				if(!fd)
				{
                                  GUI_Error_HIG(QT_TR_NOOP("File error"), QT_TR_NOOP("Cannot open \"%s\"."), name);
					return;
				}
			}
			else // write
			{
				if(fd){
				  struct stat buf;
				  int fdino;
					ADM_fclose(fd);

					char msg[300];

					snprintf(msg, 300, QT_TR_NOOP("%s already exists.\n\nDo you want to replace it?"), GetFileName(name));

                                        if(!GUI_Question(msg))
						return;
	                                /*
	                                ** JSC Fri Feb 10 00:07:30 CET 2006
	                                ** compare existing output file inode against each current open files inode
	                                ** i'm ignoring st_dev, so we may get false positives
	                                ** i'm testing until fd=1024, should be MAXFD computed by configure
	                                ** keep in mind:
	                                ** you can overwrite .idx files, they are loaded into memory and closed soon
	                                ** you cannot overwrite segment data files, all files are kept open and
	                                ** are detected here
	                                */
#ifndef ADM_WIN32
					if( stat(name,&buf) == -1 ){
						fprintf(stderr,"stat(%s) failed\n",name);
						return;
					}
#endif
					fdino = buf.st_ino;
					for(int i=0;i<1024;i++){
						if( fstat(i,&buf) != -1 ){
							if( buf.st_ino == fdino ){
							  char str[512];
								snprintf(str,512,"File \"%s\" exists and is opened by Avidemux",name);
								GUI_Error_HIG(str,
                                                                    QT_TR_NOOP("It is possible that you are trying to overwrite an input file!"));
								return;
							}
						}
					}
					/*
	                                ** compare output file against actual EMCAscript file
					** need to stat() to avoid symlink (/home/x.js) vs. real file (/export/home/x.js) case
	                                */
					if( actual_workbench_file ){
						if( stat(actual_workbench_file,&buf) != -1 ){
							if( buf.st_ino == fdino ){
							  char str[512];
								snprintf(str,512,"File \"%s\" exists and is the actual ECMAscript file",name);
                                                                GUI_Error_HIG(str,QT_TR_NOOP("It is possible that you are trying to overwrite an input file!"));
								return;
							}
						}
					}
				}

				// check we have right access to it
				fd=ADM_fopen(name,"wb");
				if(!fd)
				{
                                  GUI_Error_HIG(QT_TR_NOOP("Cannot write the file"),QT_TR_NOOP( "No write access to \"%s\"."), name);
					return;
				}
			}
			ADM_fclose(fd);
			cb(name);
		} // no callback -> return value
	}
}
// CYB 2005.02.23: DND
