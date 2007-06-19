/***************************************************************************
                          GUI_mux.h  -  description
                             -------------------
    begin                : Wed Jul 3 2002
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
# include "config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

# include <math.h>
#include "default.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_toolkit/filesel.h"
#include "DIA_factory.h"

/**
      \fn DIA_v2v
      \brief  handle vob2vobsub dialog 
*/
uint8_t  DIA_v2v(char **vobname, char **ifoname,char **vobsubname)
{
uint8_t ret=0;
char *tmp=NULL,*tmp2=NULL,*tmp3=NULL;

        
        diaElemFile eVob(0,&tmp,_("_VOB file(s):"),"");
        diaElemFile eIfo(0,&tmp2,_("_IFO file:"),"");
        diaElemFile eVsub(1,&tmp3,_("Vob_Sub file:"),"");
        
        while(1)
        {
           diaElem *tabs[]={&eVob,&eIfo,&eVsub};
          if( diaFactoryRun(_("VOB to VobSub"),3,tabs))
	  {
              if(!ADM_fileExist(tmp))
              {
                GUI_Error_HIG(_("The selected vobfile does not exist"), NULL); 
                continue;
              }
              if(!ADM_fileExist(tmp2))
              {
                GUI_Error_HIG(_("The selected vobfile does not exist"), NULL); 
                continue;
              }
              if(strlen(tmp3)<3)
              {
                 GUI_Error_HIG(_("Please select a correct VobSub path/dir"), NULL); 
                 continue;
              }
                  if(*vobname) ADM_dealloc(*vobname);
                  if(*ifoname) ADM_dealloc(*ifoname);
                  if(*vobsubname) ADM_dealloc(*vobsubname);

                    *vobname=*ifoname=*vobsubname=NULL;

                  *vobname=ADM_strdup(tmp);
                  *ifoname=ADM_strdup(tmp2);
                  *vobsubname=(char *)ADM_alloc(strlen(tmp3)+5); //ADM_strdup(tmp3);
                  strcpy(*vobsubname,tmp3);
                  if(tmp3[strlen(tmp3)-1]!='x'|| tmp3[strlen(tmp3)-2]!='d') 
                          strcat(*vobsubname,".idx");
                  ADM_dealloc(tmp);
                  ADM_dealloc(tmp2);
                  ADM_dealloc(tmp3);
                  return 1;
          } 
          else return 0;
        }
        return 0;
}
//EOF

