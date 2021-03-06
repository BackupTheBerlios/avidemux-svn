/***************************************************************************
  FAC_bar.cpp
  Handle dialog factory element : Toggle
  (C) 2006 Mean Fixounet@free.fr 
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ADM_toolkit_gtk/toolkit_gtk.h"
#include "ADM_commonUI/DIA_factory.h"



diaElemBar::diaElemBar(uint32_t percent,const char *toggleTitle)
  : diaElem(ELEM_BAR)
{
  per=percent;
  paramTitle=ADM_strdup(toggleTitle);
}

diaElemBar::~diaElemBar()
{
  ADM_dealloc(paramTitle);
}
void diaElemBar::setMe(void *dialog, void *opaque,uint32_t line)
{
  GtkWidget *widget;
  GtkWidget *label;
  GtkWidget *bar;
  
  label = gtk_label_new_with_mnemonic (paramTitle);
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_widget_show(label);
  
  gtk_table_attach (GTK_TABLE (opaque), label, 0, 1, line, line+1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  
  
   bar = gtk_progress_bar_new ();
   gtk_widget_show (bar);
   gfloat p;
   p=per;
   p=p/100.;
   gtk_progress_set_percentage(GTK_PROGRESS(bar),p);

  
  gtk_table_attach (GTK_TABLE (opaque), bar, 1, 2, line, line+1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  
  
}
void diaElemBar::getMe(void)
{
  
}

//EOF
