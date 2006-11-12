/***************************************************************************
                          ADM_guiContrast.cpp  -  description
                             -------------------
    begin                : Mon Sep 23 2002
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
#include <math.h>


#include "fourcc.h"
#include "avio.hxx"

#include "avi_vars.h"
#ifdef HAVE_ENCODER


#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_video/ADM_vidContrast.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_colorspace/ADM_rgb.h"
#include "ADM_toolkit_gtk/ADM_gladeSupport.h"
#include "ADM_toolkit_gtk/toolkit_gtk.h"
#include "ADM_toolkit_gtk/toolkit_gtk_include.h"
#include <ADM_assert.h>


#define CHECKBOX(x,y) if(TRUE==gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(dialog,#x))))  \
			y=1; else y=0;
#define SCHECKBOX(x,y) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dialog,#x)),y)
void    GUI_RGBDisplay(uint8_t *dis,uint32_t w,uint32_t h,void *widg);

static int getContrastParams (void);

static GtkWidget *create_dialog1 (void);
static GtkWidget *dialog = NULL;
static GtkObject *adj_offset = NULL, *adj_coeff = NULL;

static gboolean gui_draw (GtkWidget * widget,
			  GdkEventExpose * event, gpointer user_data);

//static int croplock;
static ColYuvRgb    *rgbConv=NULL;
static void gui_update (GtkButton * button, gpointer user_data);
static void draw(void);
static void calculate(void);

// Ugly !
static ADMImage *video_src,*video_shifted;
static AVDMGenericVideoStream *sstream;
static uint8_t *video_rgb;
static uint32_t ww, hh;
static CONTRAST_PARAM par;

static uint8_t tableluma[256], tablechroma[256];

uint8_t DIA_contrast(AVDMGenericVideoStream *astream,CONTRAST_PARAM *param)
{
  uint8_t ret=0;
        sstream=astream;
        ww=astream->getInfo()->width;
        hh=astream->getInfo()->height;
        video_src=new ADMImage(ww,hh);
        video_shifted=new ADMImage(ww,hh);
        video_rgb=new uint8_t[ww*hh*4];
        rgbConv=new ColYuvRgb(ww,hh);
        rgbConv->reset(ww,hh);
        
        dialog=create_dialog1();
        gtk_register_dialog(dialog);
        gtk_widget_set_usize (WID (drawingarea1), ww, hh);
        
        memcpy(&par,param,sizeof(par));
        SCHECKBOX (checkLuma, par.doLuma);
        SCHECKBOX (checkbuttonU, par.doChromaU);
        SCHECKBOX (checkbuttonV, par.doChromaV);

        GTK_ADJUSTMENT (adj_offset)->value = par.offset;
        GTK_ADJUSTMENT (adj_coeff)->value = par.coef;

  
  // and value changed
#define CNX(x,y) gtk_signal_connect(GTK_OBJECT(WID(x)), y, GTK_SIGNAL_FUNC(gui_update), (void *) (1));

        CNX (hscaleContrast, "value_changed");
        CNX (hscaleContrast, "drag_data_received");
      
        CNX (hscaleBright, "value_changed");
        CNX (hscaleBright, "drag_data_received");
        uint32_t curImage=0,len,flags;
        if(!astream->getFrameNumberNoAlloc(curImage,&len,video_src,&flags))
        {
          GUI_Error_HIG("Frame error","Cannnot read frame to display");

        }
        else
        {
          calculate();
          // add display callback
          rgbConv->scale(video_src->data,video_rgb);
          //GUI_RGBDisplay (video_rgb, ww, hh, (void *) WID (drawingarea1));
          gtk_signal_connect (GTK_OBJECT (WID (drawingarea1)), "expose_event",
                  GTK_SIGNAL_FUNC (gui_draw), NULL);

          if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK)
            {
              CHECKBOX (checkLuma, par.doLuma);
              CHECKBOX (checkbuttonU, par.doChromaU);
              CHECKBOX (checkbuttonV, par.doChromaV);

              par.coef = GTK_ADJUSTMENT (adj_coeff)->value;
              par.offset = (int8_t) floor (GTK_ADJUSTMENT (adj_offset)->value);

              memcpy(param,&par,sizeof(par));
              ret = 1;
            }
        }
        gtk_unregister_dialog(dialog);
        gtk_widget_destroy(dialog);
        dialog=NULL;
        delete video_src;
        delete video_shifted;
        delete [] video_rgb;
        delete rgbConv;
        return ret;
}
void calculate(void)
{
  buildContrastTable (par.coef, par.offset, tableluma, tablechroma);

  memcpy (video_shifted->data, video_src->data, (hh * ww * 3)>>1);
  if (par.doLuma)
    {
      doContrast (YPLANE(video_src), YPLANE(video_shifted), tableluma, ww, hh);
    }
  if (par.doChromaU)
    {
      doContrast (UPLANE(video_src), UPLANE(video_shifted), tablechroma, ww >> 1, hh >> 1);
    }

  if (par.doChromaV)
    {
     doContrast (VPLANE(video_src), VPLANE(video_shifted), tablechroma, ww >> 1, hh >> 1);
    }
}
void draw(void)
{
  //COL_yv12rgb (sw, sh, video2, video3);
  rgbConv->scale(video_shifted->data,video_rgb);
  GUI_RGBDisplay (video_rgb, ww, hh, (void *) WID (drawingarea1));
}
//
//        Check entered values and green-out the selected portion on the screen
//        Each value must be even
//
void gui_update (GtkButton * button, gpointer user_data)
{
  UNUSED_ARG (button);
  UNUSED_ARG (user_data);
  uint32_t    sz;

  sz = ww * hh;
  CHECKBOX (checkLuma, par.doLuma);
  CHECKBOX (checkbuttonU, par.doChromaU);
  CHECKBOX (checkbuttonV, par.doChromaV);

  par.coef = GTK_ADJUSTMENT (adj_coeff)->value;
  par.offset = (int8_t) floor (GTK_ADJUSTMENT (adj_offset)->value);

  // printf("\n Luma : %d,u %d, v:%d",par.doLuma,par.doChromaU,par.doChromaV);
  // printf("\n off : %d, coeff %f\n",par.offset,par.coef);
  calculate();
  draw();
  
}

gboolean gui_draw (GtkWidget * widget, GdkEventExpose * event, gpointer user_data)
{
  UNUSED_ARG (widget);
  UNUSED_ARG (event);
  UNUSED_ARG (user_data);

  GUI_RGBDisplay (video_rgb, ww, hh, (void *) WID (drawingarea1));
  return TRUE;
}


GtkWidget *
create_dialog1 (void)
{
  GtkWidget *
    dialog1;
  GtkWidget *
    dialog_vbox1;
  GtkWidget *
    vbox1;
  GtkWidget *
    hbox1;
  GtkWidget *
    vbox3;
  GtkWidget *
    label1;
  GtkWidget *
    hscaleContrast;
  GtkWidget *
    label2;
  GtkWidget *
    hscaleBright;
  GtkWidget *
    vbox2;
  GtkWidget *
    checkLuma;
  GtkWidget *
    checkbuttonU;
  GtkWidget *
    checkbuttonV;
  GtkWidget *
    drawingarea1;
  GtkWidget *
    dialog_action_area1;
  GtkWidget *
    cancelbutton1;
  GtkWidget *
    okbutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), _("Contrast"));

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox3);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox3, TRUE, TRUE, 0);

  label1 = gtk_label_new (_("Contrast"));
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox3), label1, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);



  adj_coeff = gtk_adjustment_new (1, 0.5, 1.5, 0.1, 0.1, 0);
  hscaleContrast = gtk_hscale_new (GTK_ADJUSTMENT (adj_coeff));
  gtk_widget_show (hscaleContrast);
  gtk_box_pack_start (GTK_BOX (vbox3), hscaleContrast, FALSE, TRUE, 0);
  gtk_scale_set_value_pos (GTK_SCALE (hscaleContrast), GTK_POS_LEFT);

  label2 = gtk_label_new (_("Brightness"));
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (vbox3), label2, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);


  adj_offset = gtk_adjustment_new (0, -127, 127, 1, 1, 0);
  hscaleBright = gtk_hscale_new (GTK_ADJUSTMENT (adj_offset));
  gtk_widget_show (hscaleBright);
  gtk_box_pack_start (GTK_BOX (vbox3), hscaleBright, FALSE, TRUE, 0);
  gtk_scale_set_value_pos (GTK_SCALE (hscaleBright), GTK_POS_LEFT);
  gtk_scale_set_digits (GTK_SCALE (hscaleBright), 0);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox2, TRUE, TRUE, 0);

  checkLuma = gtk_check_button_new_with_mnemonic (_("Luma"));
  gtk_widget_show (checkLuma);
  gtk_box_pack_start (GTK_BOX (vbox2), checkLuma, FALSE, FALSE, 0);

  checkbuttonU = gtk_check_button_new_with_mnemonic (_("Chroma U"));
  gtk_widget_show (checkbuttonU);
  gtk_box_pack_start (GTK_BOX (vbox2), checkbuttonU, FALSE, FALSE, 0);

  checkbuttonV = gtk_check_button_new_with_mnemonic (_("Chroma V"));
  gtk_widget_show (checkbuttonV);
  gtk_box_pack_start (GTK_BOX (vbox2), checkbuttonV, FALSE, FALSE, 0);

  drawingarea1 = gtk_drawing_area_new ();
  gtk_widget_show (drawingarea1);
  gtk_box_pack_start (GTK_BOX (vbox1), drawingarea1, TRUE, TRUE, 0);
  gtk_widget_set_size_request (drawingarea1, -1, 300);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1),
			     GTK_BUTTONBOX_END);

  cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), cancelbutton1,
				GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

  okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), okbutton1,
				GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog1, "dialog1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (dialog1, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (dialog1, label1, "label1");
  GLADE_HOOKUP_OBJECT (dialog1, hscaleContrast, "hscaleContrast");
  GLADE_HOOKUP_OBJECT (dialog1, label2, "label2");
  GLADE_HOOKUP_OBJECT (dialog1, hscaleBright, "hscaleBright");
  GLADE_HOOKUP_OBJECT (dialog1, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (dialog1, checkLuma, "checkLuma");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonU, "checkbuttonU");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonV, "checkbuttonV");
  GLADE_HOOKUP_OBJECT (dialog1, drawingarea1, "drawingarea1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1,
			      "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");

  return dialog1;
}


#endif
