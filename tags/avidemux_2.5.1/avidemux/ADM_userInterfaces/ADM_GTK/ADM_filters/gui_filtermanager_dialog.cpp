/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "config.h"
#include "ADM_toolkitGtk.h"
#include "ADM_default.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *vbox2;
  GtkWidget *tmp_image;
  GtkWidget *hbox1;
  GtkWidget *alignment1;
  GtkWidget *table1;
  GtkWidget *vbox3;
  GtkWidget *table2;
  GtkWidget *scrolledwindowAvailable;
  GtkWidget *treeviewAvailable;
  GtkWidget *buttonVCDRes;
  GtkWidget *buttonSVCDRes;
  GtkWidget *buttonHalfD1Res;
  GtkWidget *hbox2;
  GtkWidget *label1;
  GtkWidget *buttonDVDRes;
  GtkWidget *frame1;
  GtkWidget *mainTree;
  GtkWidget *hboxActionAvailable;
  GtkWidget *buttonAdd;
  GtkWidget *image1;
  GtkWidget *labelAvailable;
  GtkWidget *alignment2;
  GtkWidget *vbox4;
  GtkWidget *scrolledwindowActive;
  GtkWidget *treeviewActive;
  GtkWidget *hboxActionActive;
  GtkWidget *buttonRemove;
  GtkWidget *image2;
  GtkWidget *buttonDown;
  GtkWidget *image3;
  GtkWidget *buttonUp;
  GtkWidget *image4;
  GtkWidget *buttonOpen;
  GtkWidget *buttonSave;
  GtkWidget *buttonPartial;
  GtkWidget *buttonProperties;
  GtkWidget *alignment3;
  GtkWidget *hbox3;
  GtkWidget *label2;
  GtkWidget *labelActive;
  GtkWidget *dialog_action_area1;
  GtkWidget *buttonPreview;
  GtkWidget *alignment4;
  GtkWidget *image5;
  GtkWidget *label3;
  GtkWidget *buttonClose;
  GtkAccelGroup *accel_group;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  accel_group = gtk_accel_group_new ();

  dialog1 = gtk_dialog_new ();
  gtk_container_set_border_width (GTK_CONTAINER (dialog1), 6);
  gtk_window_set_title (GTK_WINDOW (dialog1), QT_TR_NOOP("Video Filter Manager"));
  gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);
  gtk_dialog_set_has_separator (GTK_DIALOG (dialog1), FALSE);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_box_set_spacing (GTK_BOX(dialog_vbox1), 12);
  gtk_widget_show (dialog_vbox1);

  hbox1 = gtk_hbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (hbox1), 6);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), hbox1, TRUE, TRUE, 0);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox1, TRUE, TRUE, 0);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox2, TRUE, TRUE, 0);

  labelAvailable = gtk_label_new (QT_TR_NOOP("<b>Available Filters</b>"));
  gtk_label_set_use_markup (GTK_LABEL (labelAvailable), TRUE);
  gtk_misc_set_alignment (GTK_MISC (labelAvailable), 0, 0);
  gtk_box_pack_start (GTK_BOX (vbox1), labelAvailable, FALSE, FALSE, 0);
  gtk_widget_show (labelAvailable);

  alignment1 = gtk_alignment_new (0, 0, 1, 1);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment1), 6, 0, 18, 0);
  gtk_box_pack_start (GTK_BOX (vbox1), alignment1, TRUE, TRUE, 0);
  gtk_widget_show (alignment1);

  table1 = gtk_table_new (2, 2, FALSE);
  gtk_table_set_row_spacings (GTK_TABLE (table1), 6);
  gtk_table_set_col_spacings (GTK_TABLE (table1), 6);
  gtk_container_add (GTK_CONTAINER (alignment1), table1);
  gtk_widget_show (table1);

  vbox3 = gtk_vbox_new (FALSE, 6);
  gtk_table_attach (GTK_TABLE(table1), vbox3, 0, 1, 0, 1, GTK_SHRINK, GTK_FILL, 0, 0);
  gtk_widget_show (vbox3);

  frame1 = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME(frame1), GTK_SHADOW_IN);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox3), frame1, TRUE, TRUE, 0);

  mainTree =  gtk_tree_view_new ();
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(mainTree), FALSE);
  gtk_tree_view_set_enable_search (GTK_TREE_VIEW (mainTree), FALSE);
  gtk_widget_show (mainTree);
  gtk_container_add (GTK_CONTAINER (frame1), mainTree);
  
  table2 = gtk_table_new (2, 2, TRUE);
  gtk_table_set_row_spacings (GTK_TABLE(table2), 6);
  gtk_table_set_col_spacings (GTK_TABLE(table2), 6);
  gtk_widget_show (table2);
  gtk_box_pack_start (GTK_BOX (vbox3), table2, FALSE, FALSE, 0);

  buttonVCDRes = gtk_button_new_with_mnemonic (QT_TR_NOOP("_VCD"));
  gtk_tooltips_set_tip (tooltips, buttonVCDRes, QT_TR_NOOP("VCD resolution"), NULL);
  gtk_widget_show (buttonVCDRes);
  gtk_table_attach_defaults (GTK_TABLE(table2), buttonVCDRes, 0, 1, 0, 1);

  buttonSVCDRes = gtk_button_new_with_mnemonic (QT_TR_NOOP("_SVCD"));
  gtk_tooltips_set_tip (tooltips, buttonSVCDRes, QT_TR_NOOP("SVCD resolution"), NULL);
  gtk_widget_show (buttonSVCDRes);
  gtk_table_attach_defaults (GTK_TABLE(table2), buttonSVCDRes, 1, 2, 0, 1);

  buttonHalfD1Res = gtk_button_new ();
  gtk_tooltips_set_tip (tooltips, buttonHalfD1Res, QT_TR_NOOP("Half D1 resolution"), NULL);
  gtk_widget_show (buttonHalfD1Res);
  hbox2 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox2);
  gtk_container_add (GTK_CONTAINER (buttonHalfD1Res), hbox2);
  label1 = gtk_label_new_with_mnemonic (QT_TR_NOOP("<sup>1</sup>/<sub>2</sub> D_1"));
  gtk_label_set_use_markup (GTK_LABEL(label1), TRUE);
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (hbox2), label1, FALSE, FALSE, 0);
  gtk_table_attach_defaults (GTK_TABLE(table2), buttonHalfD1Res, 0, 1, 1, 2);

  buttonDVDRes = gtk_button_new_with_mnemonic (QT_TR_NOOP("_DVD"));
  gtk_tooltips_set_tip (tooltips, buttonDVDRes, QT_TR_NOOP("DVD resolution"), NULL);
  gtk_widget_show (buttonDVDRes);
  gtk_table_attach_defaults (GTK_TABLE(table2), buttonDVDRes, 1, 2, 1, 2);

  scrolledwindowAvailable = gtk_scrolled_window_new (NULL, NULL);
  gtk_table_attach_defaults (GTK_TABLE(table1), scrolledwindowAvailable, 1, 2, 0, 1);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindowAvailable), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindowAvailable), GTK_SHADOW_IN);
  gtk_widget_show (scrolledwindowAvailable);

  treeviewAvailable = gtk_tree_view_new ();
  gtk_widget_show (treeviewAvailable);
  gtk_container_add (GTK_CONTAINER (scrolledwindowAvailable), treeviewAvailable);
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeviewAvailable), FALSE);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (treeviewAvailable), TRUE);
  gtk_tree_view_set_enable_search (GTK_TREE_VIEW (treeviewAvailable), FALSE);

  hboxActionAvailable = gtk_hbox_new (FALSE, 6);
  gtk_widget_show (hboxActionAvailable);
  gtk_table_attach (GTK_TABLE(table1), hboxActionAvailable, 1, 2, 1, 2, GTK_FILL, GTK_FILL, 0, 0);

  buttonAdd = gtk_button_new ();
  gtk_widget_show (buttonAdd);
  gtk_box_pack_end (GTK_BOX (hboxActionAvailable), buttonAdd, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, buttonAdd, QT_TR_NOOP("Add selected filter to the Active Filters list"), NULL);

  image1 = gtk_image_new_from_stock ("gtk-add", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image1);
  gtk_container_add (GTK_CONTAINER (buttonAdd), image1);

  labelActive = gtk_label_new (QT_TR_NOOP("<b>Active Filters</b>"));
  gtk_label_set_use_markup (GTK_LABEL (labelActive), TRUE);
  gtk_misc_set_alignment (GTK_MISC (labelActive), 0, 0);
  gtk_box_pack_start (GTK_BOX (vbox2), labelActive, FALSE, FALSE, 0);
  gtk_widget_show (labelActive);

  alignment2 = gtk_alignment_new (0, 0, 1, 1);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 6, 0, 18, 0);
  gtk_box_pack_start (GTK_BOX (vbox2), alignment2, TRUE, TRUE, 0);
  gtk_widget_show (alignment2);

  vbox4 = gtk_vbox_new (FALSE, 6);
  gtk_widget_show (vbox4);
  gtk_container_add (GTK_CONTAINER (alignment2), vbox4);

  scrolledwindowActive = gtk_scrolled_window_new (NULL, NULL);
  gtk_box_pack_start (GTK_BOX (vbox4), scrolledwindowActive, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindowActive), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindowActive), GTK_SHADOW_IN);
  gtk_widget_show (scrolledwindowActive);

  treeviewActive = gtk_tree_view_new ();
  gtk_widget_show (treeviewActive);
  gtk_container_add (GTK_CONTAINER (scrolledwindowActive), treeviewActive);
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (treeviewActive), FALSE);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (treeviewActive), TRUE);
  gtk_tree_view_set_enable_search (GTK_TREE_VIEW (treeviewActive), FALSE);

  hboxActionActive = gtk_hbox_new (FALSE, 6);
  gtk_widget_show (hboxActionActive);
  gtk_box_pack_start (GTK_BOX (vbox4), hboxActionActive, FALSE, FALSE, 0);

  buttonRemove = gtk_button_new ();
  gtk_widget_show (buttonRemove);
  gtk_box_pack_end (GTK_BOX (hboxActionActive), buttonRemove, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, buttonRemove, QT_TR_NOOP("Remove filter"), NULL);

  image2 = gtk_image_new_from_stock ("gtk-remove", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image2);
  gtk_container_add (GTK_CONTAINER (buttonRemove), image2);

  buttonDown = gtk_button_new ();
  gtk_widget_show (buttonDown);
  gtk_box_pack_end (GTK_BOX (hboxActionActive), buttonDown, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, buttonDown, QT_TR_NOOP("Move filter down"), NULL);

  image3 = gtk_image_new_from_icon_name ("gtk-go-down", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image3);
  gtk_container_add (GTK_CONTAINER (buttonDown), image3);

  buttonUp = gtk_button_new ();
  gtk_widget_show (buttonUp);
  gtk_box_pack_end (GTK_BOX (hboxActionActive), buttonUp, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, buttonUp, QT_TR_NOOP("Move filter up"), NULL);

  image4 = gtk_image_new_from_icon_name ("gtk-go-up", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image4);
  gtk_container_add (GTK_CONTAINER (buttonUp), image4);

  buttonOpen = gtk_button_new ();
  gtk_widget_show (buttonOpen);
  gtk_box_pack_end (GTK_BOX (hboxActionActive), buttonOpen, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, buttonOpen, QT_TR_NOOP("Open filter list"), NULL);

  tmp_image = gtk_image_new_from_icon_name ("gtk-open", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (tmp_image);
  gtk_container_add (GTK_CONTAINER (buttonOpen), tmp_image);

  buttonSave = gtk_button_new ();
  gtk_widget_show (buttonSave);
  gtk_box_pack_end (GTK_BOX (hboxActionActive), buttonSave, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, buttonSave, QT_TR_NOOP("Save filter list"), NULL);

  tmp_image= gtk_image_new_from_icon_name ("gtk-save", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (tmp_image);
  gtk_container_add (GTK_CONTAINER (buttonSave), tmp_image);

  buttonPartial = gtk_button_new_with_mnemonic (QT_TR_NOOP("P_artial"));
  gtk_widget_show (buttonPartial);
  gtk_box_pack_end (GTK_BOX (hboxActionActive), buttonPartial, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, buttonPartial, QT_TR_NOOP("Apply the current filter only to a part of the file"), NULL);

  buttonProperties = gtk_button_new ();
  gtk_widget_show (buttonProperties);
  gtk_box_pack_end (GTK_BOX (hboxActionActive), buttonProperties, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, buttonProperties, QT_TR_NOOP("Configure filter"), NULL);

  alignment3 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment3);
  gtk_container_add (GTK_CONTAINER (buttonProperties), alignment3);

  hbox3 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox3);
  gtk_container_add (GTK_CONTAINER (alignment3), hbox3);

  label2 = gtk_label_new_with_mnemonic (QT_TR_NOOP("C_onfigure"));
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (hbox3), label2, FALSE, FALSE, 0);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  buttonPreview = gtk_button_new ();
  gtk_widget_show (buttonPreview);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), buttonPreview, GTK_RESPONSE_APPLY);
  GTK_WIDGET_SET_FLAGS (buttonPreview, GTK_CAN_DEFAULT);

  alignment4 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment4);
  gtk_container_add (GTK_CONTAINER (buttonPreview), alignment4);

  hbox2 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox2);
  gtk_container_add (GTK_CONTAINER (alignment4), hbox2);

  image5 = create_pixmap (dialog1, "preview-button.png");
  gtk_widget_show (image5);
  gtk_box_pack_start (GTK_BOX (hbox2), image5, FALSE, FALSE, 0);

  label3 = gtk_label_new_with_mnemonic (QT_TR_NOOP("_Preview"));
  gtk_widget_show (label3);
  gtk_box_pack_start (GTK_BOX (hbox2), label3, FALSE, FALSE, 0);

  buttonClose = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_show (buttonClose);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), buttonClose, GTK_RESPONSE_CLOSE);
  GTK_WIDGET_SET_FLAGS (buttonClose, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog1, "dialog1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (dialog1, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (dialog1, alignment1, "alignment1");
  GLADE_HOOKUP_OBJECT (dialog1, table1, "table1");
  GLADE_HOOKUP_OBJECT (dialog1, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (dialog1, table2, "table2");
  GLADE_HOOKUP_OBJECT (dialog1, scrolledwindowAvailable, "scrolledwindowAvailable");
  GLADE_HOOKUP_OBJECT (dialog1, treeviewAvailable, "treeviewAvailable");
  GLADE_HOOKUP_OBJECT (dialog1, buttonVCDRes, "buttonVCDRes");
  GLADE_HOOKUP_OBJECT (dialog1, buttonSVCDRes, "buttonSVCDRes");
  GLADE_HOOKUP_OBJECT (dialog1, buttonHalfD1Res, "buttonHalfD1Res");
  GLADE_HOOKUP_OBJECT (dialog1, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (dialog1, label1, "label1");
  GLADE_HOOKUP_OBJECT (dialog1, buttonDVDRes, "buttonDVDRes");
  GLADE_HOOKUP_OBJECT (dialog1, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (dialog1, mainTree, "mainTree");
  GLADE_HOOKUP_OBJECT (dialog1, hboxActionAvailable, "hboxActionAvailable");
  GLADE_HOOKUP_OBJECT (dialog1, buttonAdd, "buttonAdd");
  GLADE_HOOKUP_OBJECT (dialog1, image1, "image1");
  GLADE_HOOKUP_OBJECT (dialog1, labelAvailable, "labelAvailable");
  GLADE_HOOKUP_OBJECT (dialog1, alignment2, "alignment2");
  GLADE_HOOKUP_OBJECT (dialog1, vbox4, "vbox4");
  GLADE_HOOKUP_OBJECT (dialog1, scrolledwindowActive, "scrolledwindowActive");
  GLADE_HOOKUP_OBJECT (dialog1, treeviewActive, "treeviewActive");
  GLADE_HOOKUP_OBJECT (dialog1, hboxActionActive, "hboxActionActive");
  GLADE_HOOKUP_OBJECT (dialog1, buttonRemove, "buttonRemove");
  GLADE_HOOKUP_OBJECT (dialog1, image2, "image2");
  GLADE_HOOKUP_OBJECT (dialog1, buttonDown, "buttonDown");
  GLADE_HOOKUP_OBJECT (dialog1, image3, "image3");
  GLADE_HOOKUP_OBJECT (dialog1, buttonUp, "buttonUp");
  GLADE_HOOKUP_OBJECT (dialog1, image4, "image4");
  GLADE_HOOKUP_OBJECT (dialog1, buttonOpen, "buttonOpen");
  GLADE_HOOKUP_OBJECT (dialog1, buttonSave, "buttonSave");
  GLADE_HOOKUP_OBJECT (dialog1, buttonPartial, "buttonPartial");
  GLADE_HOOKUP_OBJECT (dialog1, buttonProperties, "buttonProperties");
  GLADE_HOOKUP_OBJECT (dialog1, alignment3, "alignment3");
  GLADE_HOOKUP_OBJECT (dialog1, hbox3, "hbox3");
  GLADE_HOOKUP_OBJECT (dialog1, label2, "label2");
  GLADE_HOOKUP_OBJECT (dialog1, labelActive, "labelActive");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, buttonPreview, "buttonPreview");
  GLADE_HOOKUP_OBJECT (dialog1, alignment4, "alignment4");
  GLADE_HOOKUP_OBJECT (dialog1, image5, "image5");
  GLADE_HOOKUP_OBJECT (dialog1, label3, "label3");
  GLADE_HOOKUP_OBJECT (dialog1, buttonClose, "buttonClose");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, tooltips, "tooltips");

  gtk_window_add_accel_group (GTK_WINDOW (dialog1), accel_group);

  return dialog1;
}

