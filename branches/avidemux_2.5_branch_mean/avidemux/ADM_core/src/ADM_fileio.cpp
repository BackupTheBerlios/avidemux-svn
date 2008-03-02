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

#ifdef WIN32
#include <glib.h>
#endif

#include "ADM_default.h"



#include "ADM_toolkit/filesel.h"
#include "ADM_toolkit/toolkit.hxx"

#include "ADM_assert.h"
#undef fread
#undef fwrite
#undef fopen
#undef fclose

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
  FILE *f;

#ifndef ADM_WIN32
  return fopen(file,mode); 
#else
  gchar *retval = g_locale_from_utf8 (file, -1, NULL, NULL, NULL);
  f=fopen(retval,mode);
  g_free (retval);
  return f;  
#endif
}

int    ADM_fclose (FILE *file)
{
  return fclose(file); 
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

/**
        \fn ADM_PathCanonize
        \brief Canonize the path, returns a copy of the absolute path given as parameter
*/
char *ADM_PathCanonize(const char *tmpname)
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
/**
        \fn ADM_PathStripName
	\brief Returns path only /foo/bar.avi -> /foo INPLACE, no copy done

*/
void		ADM_PathStripName(char *str)
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

/**
    \fn ADM_GetFileName
    \brief Get the filename without path. /foo/bar.avi -> bar.avi INPLACE, NO COPY

*/
const char *ADM_GetFileName(const char *str)
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

/**
    \fn ADM_PathSplit
    \brief Split path into absolute path+name and extention i.e. /foo/bar/zee.avi -> /foo/bar/zee,avi.             Copy are returned

*/
void ADM_PathSplit(char *str, char **root, char **ext)
{
	char *full;
	uint32_t l;

		full=ADM_PathCanonize(str);
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
