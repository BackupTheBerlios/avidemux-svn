/***************************************************************************
                          ADM_guiClean.cpp  -  description
                             -------------------
    begin                : Sun Apr 14 2002
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

 /*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <gtk/gtk.h>
#include <time.h>
#include <sys/time.h>

#include <ADM_assert.h>
#include "fourcc.h"
#include "avio.hxx"
#include "avi_vars.h"
#ifdef HAVE_ENCODER


#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_video/ADM_vidClean.h"
//#include "ADM_video/ADM_vidCommonFilter.h"

static void gui_ok(GtkButton * button, gpointer user_data);
static int getSmoothParams(	uint32_t *w,uint32_t 	*w2);
static GtkWidget  *create_dialog1 (void);

static int croplock;
static GtkObject *blend_adj;
static GtkObject *radius_adj;


uint8_t AVDMVideoSmooth::configure( AVDMGenericVideoStream *instream)
{
UNUSED_ARG(instream);

SMOOTH_PARAMS *par;
	
     	par=_param;
     	return((uint8_t)getSmoothParams(&par->radius,&par->blend));

}

//
//      Get crop parameters from GUI
//                         left, right, top, down , initial size
//
int getSmoothParams(	uint32_t *radius,uint32_t 	*stre)
{
//char 	string[50];
//gchar   *str;
static 	GtkWidget *resi;
//int 	ret;
uint32_t 	ir,is;

		ir=*radius;
		is=*stre;

		do
		{
			croplock=0;
			resi=create_dialog1();				
	  		gtk_widget_show(resi);

     		  	gtk_adjustment_set_value( GTK_ADJUSTMENT(blend_adj),(  gdouble  ) is );
    			gtk_signal_emit_by_name (GTK_OBJECT (blend_adj), "changed");
            		  	gtk_adjustment_set_value( GTK_ADJUSTMENT(radius_adj),(  gdouble  ) ir*2 );
    			gtk_signal_emit_by_name (GTK_OBJECT (radius_adj), "changed");

		while(!croplock)
		{
			gtk_main_iteration();
		}
		// now check parameters
		//
		if(1==croplock) //         ok button
		{
    			*radius= (uint32_t)floor(GTK_ADJUSTMENT(radius_adj)->value);
       		*radius=*radius/2;
       		*stre=is;
		}
		// cancel button
		if(croplock!=-1)
		{
		 	gtk_widget_destroy(resi);
		}
	}while(!croplock);
	// exit ok
	if(croplock==1)
	{
		return 1;
	}
  return 0;
}
//
//
//
//
/*void gui_ko(GtkButton * button, gpointer user_data)
{
	if(croplock==0)
		croplock=-1;
}  */


void gui_ok(GtkButton * button, gpointer user_data)
{
    UNUSED_ARG(button);

    int res;

    res = (int) user_data;
    if (croplock)
			return;			// second call back->ignore.
			
    switch (res)
      {
      case 1:
	  croplock = 1;
	  break;
      case 0:
	  croplock = -2;
	  break;
      }
    return;

};

GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *accellabel1;
  GtkWidget *hscale1;
  GtkWidget *label1;
  GtkWidget *hscale2;
  GtkWidget *dialog_action_area1;
  GtkWidget *hbox1;
  GtkWidget *button_ok;
  GtkWidget *button_ko;
  GtkWidget *button_preview;

  dialog1 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog1", dialog1);
  gtk_window_set_title (GTK_WINDOW (dialog1), "Clean smoother");
  gtk_window_set_policy (GTK_WINDOW (dialog1), TRUE, TRUE, FALSE);
   gtk_window_set_modal(GTK_WINDOW(dialog1), TRUE);
  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog_vbox1", dialog_vbox1);

  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  accellabel1 = gtk_accel_label_new ("Diameter");
  gtk_widget_ref (accellabel1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "accellabel1", accellabel1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (accellabel1);
  gtk_box_pack_start (GTK_BOX (vbox1), accellabel1, FALSE, FALSE, 0);

  radius_adj=gtk_adjustment_new (2, 2, 10, 1, 1, 1);

  hscale1 = gtk_hscale_new (GTK_ADJUSTMENT (radius_adj));
  gtk_widget_ref (hscale1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hscale1", hscale1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hscale1);
  gtk_box_pack_start (GTK_BOX (vbox1), hscale1, TRUE, TRUE, 0);

  gtk_scale_set_digits(GTK_SCALE(hscale1),0);

  label1 = gtk_label_new ("Blend amount");
  gtk_widget_ref (label1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label1", label1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox1), label1, FALSE, FALSE, 0);

  blend_adj= gtk_adjustment_new (1, 1, 10, 1, 1, 1);
  hscale2 = gtk_hscale_new (GTK_ADJUSTMENT (blend_adj));
  gtk_widget_ref (hscale2);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hscale2", hscale2,
                            (GtkDestroyNotify) gtk_widget_unref);
   gtk_scale_set_digits(GTK_SCALE(hscale2),0);

  gtk_widget_show (hscale2);
  gtk_box_pack_start (GTK_BOX (vbox1), hscale2, TRUE, TRUE, 0);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog_action_area1", dialog_action_area1);
  gtk_widget_show (dialog_action_area1);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area1), 10);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (dialog_action_area1), hbox1, TRUE, TRUE, 0);

  button_ok = gtk_button_new_with_label ("OK");
  gtk_widget_ref (button_ok);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "button_ok", button_ok,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button_ok);
  gtk_box_pack_start (GTK_BOX (hbox1), button_ok, FALSE, FALSE, 0);

  button_ko = gtk_button_new_with_label ("Cancel");
  gtk_widget_ref (button_ko);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "button_ko", button_ko,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button_ko);
  gtk_box_pack_start (GTK_BOX (hbox1), button_ko, FALSE, FALSE, 0);

  button_preview = gtk_button_new_with_label ("Preview");
  gtk_widget_ref (button_preview);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "button_preview", button_preview,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button_preview);
  gtk_box_pack_start (GTK_BOX (hbox1), button_preview, TRUE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (button_ok), "clicked",
                      GTK_SIGNAL_FUNC (gui_ok),
                      (void *)1);
  gtk_signal_connect (GTK_OBJECT (button_ko), "clicked",
                      GTK_SIGNAL_FUNC (gui_ok),
                      (void *)0);



  return dialog1;
}

#endif
