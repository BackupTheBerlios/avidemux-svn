/***************************************************************************
                          GUI_encoder.cpp  -  description
                             -------------------
    begin                : Fri Apr 19 2002
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

/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */


#  include <config.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "ADM_library/default.h"
#include "GUI_encoder.h"
#include "ADM_gui2/GUI_ui.h"

#include "ADM_toolkit/toolkit.hxx"

static   GtkWidget *label_eta;
static   GtkWidget *label_codec;
static   GtkWidget *button_cancel;
static   GtkWidget *label_frame;
static   GtkWidget *label_phasis;
static   GtkWidget *label_fps;
static   GtkWidget *label_size;

static 	GtkWidget *window;
static GtkWidget *progressbar1;
static GtkWidget	*create_dialog1 (void);
static void cb_destroy(GtkObject * object, gpointer user_data);
static void cb_abort(GtkObject * object, gpointer user_data);

static int   	tobedestroyed=1;

static uint32_t lastTime;
static uint32_t lastFrame;

static uint32_t fps_average;

extern void UI_purge(void );
extern uint32_t getTime(int called);

static uint32_t timestart;
  void GUI_encoderStop( void )
  {
  	UI_deiconify();
	if(window)
 	{
		gtk_widget_destroy(window);
   		window=NULL;
     }

    }

void GUI_encoderStart( void )
{
      	window=create_dialog1();
       	tobedestroyed=0;
       gtk_widget_show(window);
       timestart=getTime(0);
       lastTime=timestart;
       lastFrame=0;
       fps_average=0;
       UI_purge();
       UI_iconify();
}
uint8_t GUI_encoderIsAlive(void)
{
 	if(window&&(	tobedestroyed==0))  return 1;
	// Ask if we are sure ?

	if(GUI_Question("Are you sure ?")==0)
	{
		if( tobedestroyed)
		{
			tobedestroyed=0;
		}
		else
		{
			GUI_encoderStart();
		}
		return 1;

	}
  return 0;

}
void GUI_encoderSetSize(int size)
{
 	if(window)
  	{
		 				char s[200];
		     		sprintf(s,"%03d MBytes",size);
      	   gtk_label_set_text(GTK_LABEL(label_size),s);
     }

}
void GUI_encoderSetPhasis(const char *n)
{
 	if(window)
  	{
      	   gtk_label_set_text(GTK_LABEL(label_phasis),n);
     }

}
void GUI_encoderSetCodec(const char *n)
{
 	if(window)
  	{
      	   gtk_label_set_text(GTK_LABEL(label_codec),n);
     }

}
void GUI_encoderSetFrame(uint32_t nb,uint32_t total)
{
uint32_t tim;
static char str[100];
//float ratio;
//uint32_t mm,ss;
 	if(window)
  	{
     		sprintf(str,"%08lu / %08lu ",nb,total);
      	   gtk_label_set_text(GTK_LABEL(label_frame),str);
           if(nb==0) // restart ?
           {
						   	timestart=getTime(0);
       					lastTime=timestart;
       					lastFrame=0;
       					fps_average=0;						 
						}
           tim=getTime(1)-timestart;
           //
           //	nb/total=timestart/totaltime -> total time =timestart*total/nb
           //
           //
           if(nb&&tim)
           	{

              //---
              	uint32_t sectogo,secdone;
                double d;
//                float f;
			      int  mm,ss;

	  				secdone = getTime(1) - timestart;;	
			 	  	d = secdone;
	  				d /= nb;
       			d*=total;
	  				d -= secdone;
     				// d is in ms, divide by 1000 to get seconds
	  				sectogo = (uint32_t) floor(d / 1000.);
     				mm=sectogo/60;
      				ss=sectogo%60;
                  sprintf(str,"%03u m %02u s left ",mm,ss);
      	   			gtk_label_set_text(GTK_LABEL(label_eta),str);
              }

					// update progress bar
						 float f=nb;
						 f=f/total;
					   gtk_progress_set_percentage(GTK_PROGRESS(progressbar1),(gfloat)f);              
        /*
        		Compute FPS
              
        */      
        
        	if(getTime(1)>(lastTime+1000)) // one second has elapsed
         	{
						
						 /* fps = 1000* delta frame / delta ms  */
						 double fps;
						 double framepersec;
								fps=getTime(1)-lastTime;
								if(fps)
									{
										framepersec=(nb-lastFrame);
										framepersec/=fps;
										framepersec*=1000.;
										if(lastTime==0)
										{
												fps_average=(uint32_t)floor(framepersec);
										}
										fps_average=fps_average*9+(uint32_t)floor(framepersec);
										fps_average=(fps_average+4)/10;
										
//										printf("\n fps= %02.2f",fps);
                sprintf(str," %02ld ",fps_average);
      	   			gtk_label_set_text(GTK_LABEL(label_fps),str);

										lastTime=getTime(1);
										lastFrame=nb;									
									}						
						
						}
              
              
          	while (gtk_events_pending())
	  {
	      gtk_main_iteration();
	  }
     }

}
void cb_destroy(GtkObject * object, gpointer user_data)
{
   UNUSED_ARG(object);
   UNUSED_ARG(user_data);
   window=NULL;
};
void cb_abort(GtkObject * object, gpointer user_data)
{
    UNUSED_ARG(object);
    UNUSED_ARG(user_data);
//	gtk_widget_destroy(window);       modal
  	tobedestroyed=1;
};

GtkWidget	*create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *table1;
  GtkWidget *label1;
  GtkWidget *label3;
  GtkWidget *label5;
  GtkWidget *label_19;
  GtkWidget *label_label;
  GtkWidget *label_labelfps;
  GtkWidget *dialog_action_area1;

  dialog1 = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog1", dialog1);
  gtk_window_set_title (GTK_WINDOW (dialog1), "Encoding");
  gtk_window_set_policy (GTK_WINDOW (dialog1), TRUE, TRUE, FALSE);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog_vbox1", dialog_vbox1);
  gtk_widget_show (dialog_vbox1);
     gtk_window_set_modal(GTK_WINDOW(dialog1), 1);
  table1 = gtk_table_new (4, 2, FALSE);
  gtk_widget_ref (table1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "table1", table1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), table1, TRUE, TRUE, 0);

  label1 = gtk_label_new ("  Phasis :");
  gtk_widget_ref (label1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label1", label1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label1);
  gtk_table_attach (GTK_TABLE (table1), label1, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 0, 0);
  gtk_label_set_line_wrap (GTK_LABEL (label1), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label1), 0, 0.5);

  label_phasis = gtk_label_new ("Idling");
  gtk_widget_ref (label_phasis);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label_phasis", label_phasis,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_phasis);
  gtk_table_attach (GTK_TABLE (table1), label_phasis, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_phasis), 0, 0.5);

  label3 = gtk_label_new ("  Frame # :");
  gtk_widget_ref (label3);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label3", label3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label3);
  gtk_table_attach (GTK_TABLE (table1), label3, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label3), 0, 0.5);

  label_frame = gtk_label_new ("0");
  gtk_widget_ref (label_frame);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label_frame", label_frame,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_frame);
  gtk_table_attach (GTK_TABLE (table1), label_frame, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_frame), 0, 0.5);

  label5 = gtk_label_new ("  Estimated time left :");
  gtk_widget_ref (label5);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label5", label5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label5);
  gtk_table_attach (GTK_TABLE (table1), label5, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label5), 0, 0.5);

  label_eta = gtk_label_new ("0");
  gtk_widget_ref (label_eta);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label_eta", label_eta,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_eta);
  gtk_table_attach (GTK_TABLE (table1), label_eta, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_eta), 0, 0.5);

  label_19 = gtk_label_new ("  Codec used :");
  gtk_widget_ref (label_19);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label_19", label_19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_19);
  gtk_table_attach (GTK_TABLE (table1), label_19, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_19), 0, 0.5);
    //--
  label_codec = gtk_label_new ("None");
  gtk_widget_ref (label_codec);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label_codec", label_codec,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_codec);  
  gtk_table_attach (GTK_TABLE (table1), label_codec, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_codec), 0, 0.5);

 
  // fps
  label_labelfps = gtk_label_new (" Fps");
  gtk_widget_ref (label_labelfps);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label_labelfps", label_labelfps,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_labelfps);  
  gtk_table_attach (GTK_TABLE (table1), label_labelfps, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_labelfps), 0, 0.5);
 
   
  label_fps = gtk_label_new ("0");
  gtk_widget_ref (label_fps);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label_fps", label_fps,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_fps);  
  gtk_table_attach (GTK_TABLE (table1), label_fps, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_fps), 0, 0.5);
 // size label_label
   label_label = gtk_label_new (" Size MB");
  gtk_widget_ref (label_label);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label_label", label_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_label);  
  gtk_table_attach (GTK_TABLE (table1), label_label, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_label), 0, 0.5);
  
   label_size = gtk_label_new ("None");
  gtk_widget_ref (label_size);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "label_size", label_size,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_size);  
  gtk_table_attach (GTK_TABLE (table1), label_size, 1, 2, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_size), 0, 0.5);
 
 
 //
  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_object_set_data (GTK_OBJECT (dialog1), "dialog_action_area1", dialog_action_area1);
  gtk_widget_show (dialog_action_area1);
  
   // progress bar
  //________________
   progressbar1 = gtk_progress_bar_new ();
  gtk_widget_ref (progressbar1);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "progressbar1", progressbar1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (progressbar1);
  gtk_box_pack_start (GTK_BOX (dialog_action_area1), progressbar1, FALSE, FALSE, 0);
  
	// / progress bar
  
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area1), 10);
                                       
 
  //
  
  button_cancel = gtk_button_new_with_label ("Abort");
  gtk_widget_ref (button_cancel);
  gtk_object_set_data_full (GTK_OBJECT (dialog1), "button_cancel", button_cancel,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button_cancel);
  gtk_box_pack_start (GTK_BOX (dialog_action_area1), button_cancel, FALSE, FALSE, 0);
   // Call back
    gtk_object_set_data_full(GTK_OBJECT(dialog1),
			     "dialog1", dialog1,
			     (GtkDestroyNotify) cb_destroy);

    gtk_signal_connect(GTK_OBJECT(button_cancel),
		       "button_press_event", GTK_SIGNAL_FUNC(cb_abort),
		       0);

  return dialog1;
}

