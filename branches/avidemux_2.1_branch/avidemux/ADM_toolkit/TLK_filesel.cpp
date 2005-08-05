/***************************************************************************
                          TLK_filesel.cpp  -  description
                             -------------------
	New version of file selector

    begin                : Fri Sep 20 2002
    copyright            : (C) 2002 by mean
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

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include <dirent.h>
#include <errno.h>
#ifndef CYG_MANGLING
	#include <unistd.h>
#endif

#include "avi_vars.h"
#include "toolkit.hxx"
#include <ADM_assert.h>

#include "filesel.h"
#include "prefs.h"
#define TH_READ 1
#define TH_WRITE 2
#include "ADM_toolkit/toolkit_gtk_include.h"
#include "ADM_toolkit/toolkit_gtk.h"


static void GUI_FileSel(const char *label, SELFILE_CB * cb, int rw, char **name=NULL);
char            *PathCanonize(const char *tmpname);
void            PathStripName(char *str);
uint8_t         initFileSelector(void);

static GtkFileFilter   *filter_avi=NULL,*filter_mpeg=NULL,*filter_image=NULL,*filter_all=NULL;
static uint8_t          setFilter( GtkWidget *dialog);

/**
	Select a file
		Target is the string allocated by caller that will receive the resule, maxlen byte
		Source is a optionnal last file to replace the selector at the last dir used
	@Title@ is the title of the dialog window
	
	Returns : 0 if error, 1 on success
	

*/
uint8_t FileSel_SelectRead(const char *title,char *target,uint32_t max, const char *source)
{
	
GtkWidget *dialog;
uint8_t ret=0;
gchar *selected_filename;
gchar last;
char *dupe=NULL,*tmpname=NULL;
DIR *dir=NULL;
	
	dialog = gtk_file_chooser_dialog_new ("Open File",
                                      NULL,
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                      NULL);
	gtk_window_set_title (GTK_WINDOW (dialog),title);
        initFileSelector();
        setFilter(dialog);
	gtk_transient(dialog);
	if(source)
	{
		dupe=PathCanonize(source);
		PathStripName(dupe);
		if( (dir=opendir(dupe)) )
			{
				closedir(dir);
				gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),(gchar *)source);
			}
		delete [] dupe;
	
	}
	else	//use pref
	{
		if( prefs->get(LASTDIR_READ,&tmpname))
		{
			
	
			dupe=PathCanonize(tmpname);
			PathStripName(dupe);

			if( (dir=opendir(dupe)) )
			{
				closedir(dir);
				gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),(gchar *)dupe);
			}
			delete [] dupe;
		}
	}
	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_ACCEPT)
	{
			selected_filename= (gchar *) 	gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			if(strlen(selected_filename))
			{
			last=selected_filename[strlen(selected_filename) - 1]; 
			 if (last == '/' || last =='\\' )
			 {
      	  					GUI_Alert("Cannot open directory as file !");
						return 0;
			}
			else
			{
				// Check we can read it ..
				
					FILE *fd;
					fd=fopen(selected_filename,"rb");
					if(fd)
					{	
						fclose(fd);
						strncpy(target,(char *)selected_filename,max);
						// Finally we accept it :)
						ret=1;
					}
			}
			}
	}	
	gtk_widget_destroy(dialog);
	return ret;
}

void GUI_FileSelWrite(const char *label, SELFILE_CB * cb)
{				/* Create the selector */
    GUI_FileSel(label, cb, 1);
}

void GUI_FileSelRead(const char *label, SELFILE_CB * cb)
{				/* Create the selector */
    GUI_FileSel(label, cb, 0);
}
void GUI_FileSelRead(const char *label, char * * name)
{				/* Create the selector */
    GUI_FileSel(label, NULL, 0,name);
}
void GUI_FileSelWrite(const char *label, char * * name)
{				/* Create the selector */
    GUI_FileSel(label, NULL, 1,name);
}

// CYB 2005.02.23: DND
void fileReadWrite(SELFILE_CB *cb, int rw, char *name)
{

	if(name)
	{
		if(cb)
		{
			FILE *fd;
			fd=fopen(name,"rb");
			if(rw==0) // read
			{
				// try to open it..
				if(!fd)
				{
					GUI_Alert("Cannot open this file !");
					return;
				}
			}
			else // write
			{
				if(fd)
				{
					fclose(fd);
					if(!GUI_Question("Overwrite file ?"))
						return;
				}
				// check we have right access to it
				fd=fopen(name,"wb");
				if(!fd)
				{
					GUI_Alert("No write access to that file !");
					return;
				}
			}
			fclose(fd);
			cb(name);
			ADM_dealloc(name);
		} // no callback -> return value
	}
}
// CYB 2005.02.23: DND


void GUI_FileSel(const char *label, SELFILE_CB * cb, int rw,char **rname)
{				/* Create the selector */

    	GtkWidget *dialog;
	char *name=NULL;
	char *tmpname;
	gchar *selected_filename;
        uint8_t res;

       
	if(rname)
		*rname=NULL;

        if(rw)
        {
                        dialog=dialog = gtk_file_chooser_dialog_new ("Save",
                                      NULL,
                                      GTK_FILE_CHOOSER_ACTION_SAVE,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                      NULL);
                        
        }
        else
        {
          dialog=dialog = gtk_file_chooser_dialog_new ("Open File",
                                      NULL,
                                      GTK_FILE_CHOOSER_ACTION_OPEN,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                      NULL);
        }
        /**********/
        initFileSelector();
        setFilter(dialog);
        /**********/
        gtk_window_set_title (GTK_WINDOW (dialog),label);
        gtk_register_dialog(dialog);
        if(rw)
                res=prefs->get(LASTDIR_WRITE,&tmpname); 
        else
               res=prefs->get(LASTDIR_READ,&tmpname); 
        if(res)
	{
                DIR *dir;
                char *str=PathCanonize(tmpname);
                PathStripName(str);

                /* LASTDIR may have gone; then do nothing and use current dir instead (implied) */
                if( (dir=opendir(str)) )
                {
                        closedir(dir);
                        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),(gchar *)str);
                }
                delete [] str;
        }
        if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_ACCEPT)
	{

                        selected_filename= (gchar *) 	gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
#ifdef CYG_MANGLING
                        if (*(selected_filename + strlen(selected_filename) - 1) == '\\'){
#else			
                        if (*(selected_filename + strlen(selected_filename) - 1) == '/'){
#endif	 
                        GUI_Alert("Cannot open directory as file !");
                }
                else
                {
                        name=ADM_strdup(selected_filename);

                        char *str=PathCanonize(name);
                        PathStripName(str);
                        if(rw)
                                prefs->set(LASTDIR_WRITE,str);			
                        else
                                prefs->set(LASTDIR_READ,str);                        
                        delete [] str;

                }
        }
        gtk_unregister_dialog(dialog);
        gtk_widget_destroy(dialog);

// CYB 2005.02.23
        if(cb)
        {
                fileReadWrite(cb, rw, name);
        }
        else
        {
                *rname=name;  
        }
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
#ifndef CYG_MANGLING		
		strcat(out,"/");
#else
		strcat(out,"\\");
#endif	
		printf("\n Canonizing null string ??? (%s)\n",out);
	}else if(tmpname[0]=='/'
#if defined(CYG_MANGLING)
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
#ifndef CYG_MANGLING		
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
#ifndef CYG_MANGLING		
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
char *GetFileName(char *str)
{
	char *filename;
#ifndef CYG_MANGLING		
	filename = strrchr(str, '/');
#else
	filename = strrchr(str, '\\');
#endif
	if (filename)
		return filename+1;
	else
		return str;
}

/*
	Split patch into absolute path+name and extention

*/
void PathSplit(char *str, char **root, char **ext)
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
/* Mean:It seems it is attached to the dialog & destroyed with it
   As it leads to crash if we don't recreate them each time....*/
uint8_t         initFileSelector(void)
{

#define ADD_PAT(x,y) gtk_file_filter_add_pattern(x,"*."#y);
        
        filter_avi=gtk_file_filter_new();
        gtk_file_filter_set_name(filter_avi,"Avi (*.avi)");
        ADD_PAT(filter_avi,avi);
        ADD_PAT(filter_avi,AVI);
        
        filter_mpeg=gtk_file_filter_new();
        gtk_file_filter_set_name(filter_mpeg,"Mpeg (*.m*,*.vob)");
        ADD_PAT(filter_mpeg,[mM][12][Vv]);
        ADD_PAT(filter_mpeg,[Mm][pP][gG]);
        ADD_PAT(filter_mpeg,[Vv][Oo][Bb]);
        ADD_PAT(filter_mpeg,ts);
        ADD_PAT(filter_mpeg,TS);

       
        filter_image=gtk_file_filter_new();
        gtk_file_filter_set_name(filter_image,"Images");
        ADD_PAT(filter_image,png);
        ADD_PAT(filter_image,bmp);
        ADD_PAT(filter_image,jpg);

        ADD_PAT(filter_image,PNG);
        ADD_PAT(filter_image,BMP);
        ADD_PAT(filter_image,JPG);


        
        filter_all=gtk_file_filter_new();
        gtk_file_filter_set_name(filter_all,"All");
        gtk_file_filter_add_pattern (filter_all, "*");

        return 1;
}

uint8_t setFilter( GtkWidget *dialog)
{
        gtk_file_chooser_add_filter     (GTK_FILE_CHOOSER(dialog), filter_avi);
        gtk_file_chooser_add_filter     (GTK_FILE_CHOOSER(dialog), filter_mpeg);
        gtk_file_chooser_add_filter     (GTK_FILE_CHOOSER(dialog), filter_image);
        gtk_file_chooser_add_filter     (GTK_FILE_CHOOSER(dialog), filter_all);
        return 1;
}