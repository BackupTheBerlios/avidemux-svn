/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "ADM_toolkit_gtk/ADM_gladeSupport.h"
#include "ADM_toolkit_gtk/toolkit_gtk.h"
#include "ADM_toolkit_gtk/toolkit_gtk_include.h"


GtkWidget*
DIA_ocr (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *frame5;
  GtkWidget *table1;
  GtkWidget *label8;
  GtkWidget *label9;
  GtkWidget *labelNbGlyphs;
  GtkWidget *labelNbLines;
  GtkWidget *label7;
  GtkWidget *frameBitmap;
  GtkWidget *vbox7;
  GtkWidget *drawingareaBitmap;
  GtkWidget *table2;
  GtkWidget *label13;
  GtkWidget *drawingareaSmall;
  GtkWidget *label14;
  GtkWidget *labelText;
  GtkWidget *entry;
  GtkWidget *hbuttonbox1;
  GtkWidget *buttonCalibrate;
  GtkWidget *buttonSkipAll;
  GtkWidget *buttonSkip;
  GtkWidget *buttonIgnore;
  GtkWidget *buttonOk;
  GtkWidget *hseparator1;
  GtkWidget *label12;
  GtkWidget *dialog_action_area1;
  GtkWidget *closebutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), _("Mini OCR"));
  gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  frame5 = gtk_frame_new (NULL);
  gtk_widget_show (frame5);
  gtk_box_pack_start (GTK_BOX (vbox1), frame5, FALSE, FALSE, 0);

  table1 = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (frame5), table1);

  label8 = gtk_label_new (_("# of Glyphs :"));
  gtk_widget_show (label8);
  gtk_table_attach (GTK_TABLE (table1), label8, 0, 1, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);

  label9 = gtk_label_new (_("# of lines"));
  gtk_widget_show (label9);
  gtk_table_attach (GTK_TABLE (table1), label9, 0, 1, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label9), 0, 0.5);

  labelNbGlyphs = gtk_label_new (_("0"));
  gtk_widget_show (labelNbGlyphs);
  gtk_table_attach (GTK_TABLE (table1), labelNbGlyphs, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (labelNbGlyphs), 0, 0.5);

  labelNbLines = gtk_label_new (_("0"));
  gtk_widget_show (labelNbLines);
  gtk_table_attach (GTK_TABLE (table1), labelNbLines, 1, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (labelNbLines), 0, 0.5);

  label7 = gtk_label_new (_("<b>Stats</b>"));
  gtk_widget_show (label7);
  gtk_frame_set_label_widget (GTK_FRAME (frame5), label7);
  gtk_label_set_use_markup (GTK_LABEL (label7), TRUE);

  frameBitmap = gtk_frame_new (NULL);
  gtk_widget_show (frameBitmap);
  gtk_box_pack_start (GTK_BOX (vbox1), frameBitmap, FALSE, FALSE, 0);

  vbox7 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox7);
  gtk_container_add (GTK_CONTAINER (frameBitmap), vbox7);

  drawingareaBitmap = gtk_drawing_area_new ();
  gtk_widget_show (drawingareaBitmap);
  gtk_box_pack_start (GTK_BOX (vbox7), drawingareaBitmap, FALSE, FALSE, 0);
  gtk_widget_set_size_request (drawingareaBitmap, 5, 11);

  table2 = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (table2);
  gtk_box_pack_start (GTK_BOX (vbox7), table2, TRUE, TRUE, 0);

  label13 = gtk_label_new (_("Current Glyph"));
  gtk_widget_show (label13);
  gtk_table_attach (GTK_TABLE (table2), label13, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label13), 0, 0.5);

  drawingareaSmall = gtk_drawing_area_new ();
  gtk_widget_show (drawingareaSmall);
  gtk_table_attach (GTK_TABLE (table2), drawingareaSmall, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  label14 = gtk_label_new (_(" Text:"));
  gtk_widget_show (label14);
  gtk_table_attach (GTK_TABLE (table2), label14, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label14), 0, 0.5);

  labelText = gtk_label_new (_(" "));
  gtk_widget_show (labelText);
  gtk_table_attach (GTK_TABLE (table2), labelText, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (labelText), 0, 0.5);

  entry = gtk_entry_new ();
  gtk_widget_show (entry);
  gtk_box_pack_start (GTK_BOX (vbox7), entry, FALSE, FALSE, 0);

  hbuttonbox1 = gtk_hbutton_box_new ();
  gtk_widget_show (hbuttonbox1);
  gtk_box_pack_start (GTK_BOX (vbox7), hbuttonbox1, TRUE, TRUE, 0);

  buttonCalibrate = gtk_button_new_with_mnemonic (_("Calibrate"));
  gtk_widget_show (buttonCalibrate);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), buttonCalibrate);
  GTK_WIDGET_SET_FLAGS (buttonCalibrate, GTK_CAN_DEFAULT);

  buttonSkipAll = gtk_button_new_with_mnemonic (_("Skip all"));
  gtk_widget_show (buttonSkipAll);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), buttonSkipAll);
  GTK_WIDGET_SET_FLAGS (buttonSkipAll, GTK_CAN_DEFAULT);

  buttonSkip = gtk_button_new_with_mnemonic (_("Skip Glyph"));
  gtk_widget_show (buttonSkip);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), buttonSkip);
  GTK_WIDGET_SET_FLAGS (buttonSkip, GTK_CAN_DEFAULT);

  buttonIgnore = gtk_button_new_with_mnemonic (_("Ignore glyph"));
  gtk_widget_show (buttonIgnore);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), buttonIgnore);
  GTK_WIDGET_SET_FLAGS (buttonIgnore, GTK_CAN_DEFAULT);

  buttonOk = gtk_button_new_with_mnemonic (_("Ok"));
  gtk_widget_show (buttonOk);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), buttonOk);
  GTK_WIDGET_SET_FLAGS (buttonOk, GTK_CAN_DEFAULT);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_show (hseparator1);
  gtk_box_pack_start (GTK_BOX (vbox7), hseparator1, TRUE, TRUE, 0);

  label12 = gtk_label_new (_("<b>Bitmap</b>"));
  gtk_widget_show (label12);
  gtk_frame_set_label_widget (GTK_FRAME (frameBitmap), label12);
  gtk_label_set_use_markup (GTK_LABEL (label12), TRUE);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  closebutton1 = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_show (closebutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), closebutton1, GTK_RESPONSE_CLOSE);
  GTK_WIDGET_SET_FLAGS (closebutton1, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog1, "dialog1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, frame5, "frame5");
  GLADE_HOOKUP_OBJECT (dialog1, table1, "table1");
  GLADE_HOOKUP_OBJECT (dialog1, label8, "label8");
  GLADE_HOOKUP_OBJECT (dialog1, label9, "label9");
  GLADE_HOOKUP_OBJECT (dialog1, labelNbGlyphs, "labelNbGlyphs");
  GLADE_HOOKUP_OBJECT (dialog1, labelNbLines, "labelNbLines");
  GLADE_HOOKUP_OBJECT (dialog1, label7, "label7");
  GLADE_HOOKUP_OBJECT (dialog1, frameBitmap, "frameBitmap");
  GLADE_HOOKUP_OBJECT (dialog1, vbox7, "vbox7");
  GLADE_HOOKUP_OBJECT (dialog1, drawingareaBitmap, "drawingareaBitmap");
  GLADE_HOOKUP_OBJECT (dialog1, table2, "table2");
  GLADE_HOOKUP_OBJECT (dialog1, label13, "label13");
  GLADE_HOOKUP_OBJECT (dialog1, drawingareaSmall, "drawingareaSmall");
  GLADE_HOOKUP_OBJECT (dialog1, label14, "label14");
  GLADE_HOOKUP_OBJECT (dialog1, labelText, "labelText");
  GLADE_HOOKUP_OBJECT (dialog1, entry, "entry");
  GLADE_HOOKUP_OBJECT (dialog1, hbuttonbox1, "hbuttonbox1");
  GLADE_HOOKUP_OBJECT (dialog1, buttonCalibrate, "buttonCalibrate");
  GLADE_HOOKUP_OBJECT (dialog1, buttonSkipAll, "buttonSkipAll");
  GLADE_HOOKUP_OBJECT (dialog1, buttonSkip, "buttonSkip");
  GLADE_HOOKUP_OBJECT (dialog1, buttonIgnore, "buttonIgnore");
  GLADE_HOOKUP_OBJECT (dialog1, buttonOk, "buttonOk");
  GLADE_HOOKUP_OBJECT (dialog1, hseparator1, "hseparator1");
  GLADE_HOOKUP_OBJECT (dialog1, label12, "label12");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, closebutton1, "closebutton1");

  return dialog1;
}

