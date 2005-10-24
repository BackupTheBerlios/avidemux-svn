/***************************************************************************
                          ADM_vidpreview.cpp  -  description
                             -------------------

       Previewer, we switch to RGB (only one Xv at a time)   and display the incoming
       YV12 buffer in a nice windowd

    begin                : Mon Mar 25 2002
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
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <time.h>
#include <sys/time.h>

#include "config.h"
#include "fourcc.h"
#include "avio.hxx"

#include "avi_vars.h"
#include "ADM_gui2/support.h"
#include "ADM_toolkit/toolkit_gtk.h"
#include "ADM_toolkit/toolkit_gtk_include.h"
#include "ADM_colorspace/ADM_rgb.h"
#include "prototype.h"
#include <ADM_assert.h>
#include "gui_action.hxx"

extern void HandleAction (Action action);
void GUI_detransient(void );
void GUI_retransient(void );

static GtkWidget	*create_dialog1 (void);
static uint8_t      stacked=0;
static void 			previewRender(void);
static gboolean		preview_exit(GtkButton * button, gpointer user_data);
static gboolean         cb_prev(GtkButton * button, gpointer user_data);
static gboolean         cb_stack(GtkButton * button, gpointer user_data);
static gboolean         cb_next(GtkButton * button, gpointer user_data);
static gboolean 		preview_exit_short (GtkWidget * widget,GdkEvent * event, gpointer user_data);

static uint8_t *rgb_render=NULL,*rgb_alternate=NULL;
static GtkWidget *dialog=NULL;
static uint32_t 	uw, uh;
int 			lock;
uint8_t		needDestroy=0;
static int      needResponse=0;
extern void UI_setPreviewToggleStatus(uint8_t s);
static ColYuvRgb    rgbConv(100,100);
static int          isModal=0;
// Init previewer
//
uint8_t GUI_StillAlive( void )
{
 	if(dialog==NULL) return 0;
  	return 1;

}
void GUI_PreviewInit(uint32_t w , uint32_t h,uint32_t modal)
{
		if(rgb_render)
		{
			printf("\n Warning rgb render not null...\n");
			delete [] rgb_render;
			delete [] rgb_alternate;
			rgb_alternate=NULL;
			rgb_render=NULL;
		}
	   stacked=0;
           isModal=modal;
       ADM_assert(rgb_render=new uint8_t [w*h*4]);
       ADM_assert(rgb_alternate=new uint8_t [w*h*4]);
       uw=w;
       uh=h;
       rgbConv.reset(w,h); 
       dialog=create_dialog1();
       gtk_widget_set_usize (lookup_widget(dialog,"drawingarea1"),w,h);

       // add callback for destroy
       if(!isModal)
        {
 	        gtk_object_set_data_full(GTK_OBJECT(dialog),
			     "dialog",
			     dialog,
			     (GtkDestroyNotify) preview_exit_short);
                gtk_signal_connect(GTK_OBJECT(WID(buttonNext)), "clicked",
                       GTK_SIGNAL_FUNC(cb_next),                   (void *) NULL);
                gtk_signal_connect(GTK_OBJECT(WID(buttonPrev)), "clicked",
                       GTK_SIGNAL_FUNC(cb_prev),                   (void *) NULL);
                gtk_signal_connect(GTK_OBJECT(WID(buttonStack)), "clicked",
                       GTK_SIGNAL_FUNC(cb_stack),                   (void *) NULL);

        }
        // add callback for redraw
        gtk_signal_connect(GTK_OBJECT(WID(drawingarea1)), "expose_event",
                       GTK_SIGNAL_FUNC(previewRender),    NULL);
       lock=0;
       needDestroy=1;
       gtk_widget_show(  dialog);
}

static uint8_t stack(uint8_t *out, uint8_t *in, int width,int height)
{
 uint8_t *o1,*o2,*i;
 int x,y;
        o1=out;
        o2=out+((width*height)>>1);
        i=in;
        for(y=0;y<height>>1;y++)
        {
            memcpy(o1,i,width); 
            memcpy(o2,i+width,width);  
            o1+=width;
            o2+=width;
            i+=2*width;        
        }   
    return 1;
}
// return 1 if preview is still running
uint8_t  GUI_PreviewUpdate(uint8_t *data)
{
      if(  dialog)
       {

		// First convert YV12 to RGB
	      // COL_yv12rgb( uw, uh,data, rgb_render);
	      if(!stacked)
	      {
	            rgbConv.scale(data,rgb_render);
          }else
          {
                 stack(rgb_alternate,data, uw,uh);
                 stack(rgb_alternate+uw*uh,data+uw*uh, uw>>1,uh>>1);
                 stack(rgb_alternate+((5*uw*uh)>>2),data+((5*uw*uh)>>2), uw>>1,uh>>1);
                 rgbConv.scale(rgb_alternate,rgb_render);
          }
	       previewRender();
	       if(lock==1)
	    	{
	    		dialog=NULL;
	    		return 0;
	    	}
	    	else
	    		return 1;
	    }
	    return 0;

}
uint8_t  GUI_PreviewRun(uint8_t *data)
{
      if(  dialog)
       {

		// First convert YV12 to RGB
	       	//COL_yv12rgb( uw, uh,data, rgb_render);
	       	rgbConv.scale(data,rgb_render);
	       	previewRender();
		// add a new handler

		gtk_dialog_run(GTK_DIALOG(dialog));

               // GUI_detransient();
		return 1;

	    }
	    return 0;

}

void GUI_PreviewEnd(void)
{
 	if(dialog && needDestroy)
 		{
			//printf("+X\n");
                        
 		 	gtk_widget_destroy(dialog);
                      //  GUI_retransient();
			//printf("-X\n");

 		}
 	if(rgb_render)
 		{
 		 	delete [] rgb_render;
 		 	rgb_render=NULL;
 		 	delete [] rgb_alternate;
			rgb_alternate=NULL;
 		}
	dialog=NULL;
  	needDestroy=0;
}
void previewRender(void)
{

if(!dialog) return;
GUI_RGBDisplay(rgb_render, uw, uh ,lookup_widget(dialog,"drawingarea1"));


}
// Called when the exit button is pushed
//
gboolean preview_exit(GtkButton * button, gpointer user_data)
{
    UNUSED_ARG(button);
    UNUSED_ARG(user_data);
//	printf("button\n");

 	if(lock==0)
       	{
	   		lock=-1;
			GUI_PreviewEnd();
            		UI_setPreviewToggleStatus( 0);
                        
       	}
	return FALSE;
}
gboolean         cb_stack(GtkButton * button, gpointer user_data)
{

        stacked^=1;
        return FALSE;

}
// Called when window is destroyed
gboolean preview_exit_short (GtkWidget * widget, GdkEvent * event, gpointer user_data)
{
    UNUSED_ARG(widget);
    UNUSED_ARG(event);
    UNUSED_ARG(user_data);
//	printf("destroyed\n");

    if(lock==0)
    {
    		lock=-1;
		//printf(">\n");
		needDestroy=0;
    		GUI_PreviewEnd();
		//printf("<\n");
       		UI_setPreviewToggleStatus( 0);
                printf("Destroyed\n");
                
    }
	return FALSE;
}
static gboolean         cb_prev(GtkButton * button, gpointer user_data)
{
        HandleAction(ACT_PreviousFrame);
        return TRUE;
}
static gboolean         cb_next(GtkButton * button, gpointer user_data)
{
        HandleAction(ACT_NextFrame);
        return FALSE;
}
GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *toolbar1;
  GtkIconSize tmp_toolbar_icon_size;
  GtkWidget *toolitem1;
  GtkWidget *buttonPrev;
  GtkWidget *toolitem2;
  GtkWidget *buttonNext;
  GtkWidget *toolitem3;
  GtkWidget *buttonStack;
  GtkWidget *drawingarea1;
  GtkWidget *dialog_action_area1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), _("Preview"));
  gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  toolbar1 = gtk_toolbar_new ();
  gtk_widget_show (toolbar1);
  gtk_box_pack_start (GTK_BOX (vbox1), toolbar1, FALSE, FALSE, 0);
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar1), GTK_TOOLBAR_BOTH);
  tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar1));

  toolitem1 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem1);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem1);

  buttonPrev = gtk_button_new_from_stock ("gtk-media-previous");
  gtk_widget_show (buttonPrev);
  gtk_container_add (GTK_CONTAINER (toolitem1), buttonPrev);

  toolitem2 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem2);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem2);

  buttonNext = gtk_button_new_from_stock ("gtk-media-next");
  gtk_widget_show (buttonNext);
  gtk_container_add (GTK_CONTAINER (toolitem2), buttonNext);

  toolitem3 = (GtkWidget*) gtk_tool_item_new ();
  gtk_widget_show (toolitem3);
  gtk_container_add (GTK_CONTAINER (toolbar1), toolitem3);

  buttonStack = gtk_button_new_with_mnemonic (_("Stack Field"));
  gtk_widget_show (buttonStack);
  gtk_container_add (GTK_CONTAINER (toolitem3), buttonStack);

  drawingarea1 = gtk_drawing_area_new ();
  gtk_widget_show (drawingarea1);
  gtk_box_pack_start (GTK_BOX (vbox1), drawingarea1, TRUE, TRUE, 0);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog1, "dialog1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, toolbar1, "toolbar1");
  GLADE_HOOKUP_OBJECT (dialog1, toolitem1, "toolitem1");
  GLADE_HOOKUP_OBJECT (dialog1, buttonPrev, "buttonPrev");
  GLADE_HOOKUP_OBJECT (dialog1, toolitem2, "toolitem2");
  GLADE_HOOKUP_OBJECT (dialog1, buttonNext, "buttonNext");
  GLADE_HOOKUP_OBJECT (dialog1, toolitem3, "toolitem3");
  GLADE_HOOKUP_OBJECT (dialog1, buttonStack, "buttonStack");
  GLADE_HOOKUP_OBJECT (dialog1, drawingarea1, "drawingarea1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");

  return dialog1;
}



