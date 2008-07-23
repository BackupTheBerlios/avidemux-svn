/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#include "config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <ADM_assert.h>

#include "fourcc.h"
#include "ADM_audio/aviaudio.hxx"
#include "ADM_audiofilter/audioprocess.hxx"
#include "ADM_toolkit_gtk/toolkit_gtk.h"
#include "ADM_toolkit_gtk/toolkit_gtk_include.h"
#include "ADM_audiofilter/audioeng_buildfilters.h"
#include "ADM_toolkit_gtk/ADM_gladeSupport.h"

#include "ADM_video/ADM_vidColorYuv_param.h"

static GtkWidget    *        create_dialog1 (void);

#define COMBO_SET(x,y) {gtk_combo_box_set_active(GTK_COMBO_BOX(WID(combobox##x)),param->y);}
#define COMBO_GET(x,y) {param->y=gtk_combo_box_get_active(GTK_COMBO_BOX(WID(combobox##x)));}

#define CHECK_GET(x,y) {param->y=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(WID(x)));}
#define CHECK_SET(x,y) {gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(WID(x)),param->y);}	

#define SPIN_GET(x,y) {param->y= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(WID(x))) ;}
#define SPIN_SET(x,y)  {gtk_spin_button_set_value(GTK_SPIN_BUTTON(WID(x)),(gfloat)param->y) ;}

#define DO_LUMA(x,y) { SPIN(spinbuttonL##x,y_##y);}
#define DO_CHROU(x,y) { SPIN(spinbuttonU##x,u_##y);}
#define DO_CHROV(x,y) { SPIN(spinbuttonV##x,v_##y);}

#define DO_ONE(x,y)   {DO_LUMA(x,y);\
                        DO_CHROU(x,y)\
                        DO_CHROV(x,y);}

#define DO_ALL() DO_ONE(C,contrast) ;\
                 DO_ONE(B,bright) ;\
                 DO_ONE(G,gamma) ;\
                 DO_ONE(N,gain)

int DIA_coloryuv(COLOR_YUV_PARAM *param)
{
    GtkWidget *dialog;
    int ret=0;

    dialog=create_dialog1();
	gtk_dialog_set_alternative_button_order(GTK_DIALOG(dialog),
								GTK_RESPONSE_OK,
								GTK_RESPONSE_CANCEL,
								-1);
    gtk_register_dialog(dialog);
    
    COMBO_SET(Matrix,matrix);
    COMBO_SET(3,levels);
    COMBO_SET(2,opt);
        
    CHECK_SET(checkbuttonAnalyze,analyze);
    CHECK_SET(checkbuttonAutoGain,autogain);
    CHECK_SET(checkbuttonAutoWhite,autowhite);
    
#define SPIN SPIN_SET
    DO_ALL();
    
    if(GTK_RESPONSE_OK==gtk_dialog_run(GTK_DIALOG(dialog)))			
    {
        COMBO_GET(Matrix,matrix);
        COMBO_GET(3,levels);
        COMBO_GET(2,opt);
        
        CHECK_GET(checkbuttonAnalyze,analyze);
        CHECK_GET(checkbuttonAutoGain,autogain);
        CHECK_GET(checkbuttonAutoWhite,autowhite);
        
#undef SPIN
#define SPIN SPIN_GET
        DO_ALL();

        ret=1;
    }
    gtk_unregister_dialog(dialog);
    gtk_widget_destroy(dialog);
    return ret;	    

}
GtkWidget*
        create_dialog1 (void)
{
    GtkWidget *dialog1;
    GtkWidget *dialog_vbox1;
    GtkWidget *vbox1;
    GtkWidget *frame1;
    GtkWidget *alignment1;
    GtkWidget *table1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *label5;
    GtkWidget *label6;
    GtkWidget *label7;
    GtkWidget *label8;
    GtkObject *spinbuttonUC_adj;
    GtkWidget *spinbuttonUC;
    GtkObject *spinbuttonVC_adj;
    GtkWidget *spinbuttonVC;
    GtkObject *spinbuttonLB_adj;
    GtkWidget *spinbuttonLB;
    GtkObject *spinbuttonUB_adj;
    GtkWidget *spinbuttonUB;
    GtkObject *spinbuttonVB_adj;
    GtkWidget *spinbuttonVB;
    GtkObject *spinbuttonUG_adj;
    GtkWidget *spinbuttonUG;
    GtkObject *spinbuttonVG_adj;
    GtkWidget *spinbuttonVG;
    GtkObject *spinbuttonLN_adj;
    GtkWidget *spinbuttonLN;
    GtkObject *spinbuttonUN_adj;
    GtkWidget *spinbuttonUN;
    GtkObject *spinbuttonVN_adj;
    GtkWidget *spinbuttonVN;
    GtkObject *spinbutton23_adj;
    GtkWidget *spinbutton23;
    GtkObject *spinbutton24_adj;
    GtkWidget *spinbutton24;
    GtkWidget *Color_Correction;
    GtkWidget *Settings;
    GtkWidget *alignment2;
    GtkWidget *hbox1;
    GtkWidget *table2;
    GtkWidget *label10;
    GtkWidget *label11;
    GtkWidget *label12;
    GtkWidget *comboboxMatrix;
    GtkWidget *combobox2;
    GtkWidget *combobox3;
    GtkWidget *vbox2;
    GtkWidget *label13;
    GtkWidget *checkbuttonAutoGain;
    GtkWidget *checkbuttonAnalyze;
    GtkWidget *checkbuttonAutoWhite;
    GtkWidget *label9;
    GtkWidget *dialog_action_area1;
    GtkWidget *cancelbutton1;
    GtkWidget *okbutton1;

    dialog1 = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (dialog1), QT_TR_NOOP("ColorYuv (From avisynth)"));
    gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

    dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
    gtk_widget_show (dialog_vbox1);

    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox1);
    gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

    frame1 = gtk_frame_new (NULL);
    gtk_widget_show (frame1);
    gtk_box_pack_start (GTK_BOX (vbox1), frame1, TRUE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_NONE);

    alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment1);
    gtk_container_add (GTK_CONTAINER (frame1), alignment1);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment1), 0, 0, 12, 0);

    table1 = gtk_table_new (4, 5, FALSE);
    gtk_widget_show (table1);
    gtk_container_add (GTK_CONTAINER (alignment1), table1);

    label2 = gtk_label_new (QT_TR_NOOP("Contrast"));
    gtk_widget_show (label2);
    gtk_table_attach (GTK_TABLE (table1), label2, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label2), 0, 0.5);

    label3 = gtk_label_new (QT_TR_NOOP("Brightness"));
    gtk_widget_show (label3);
    gtk_table_attach (GTK_TABLE (table1), label3, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label3), 0, 0.5);

    label4 = gtk_label_new (QT_TR_NOOP("Gamma"));
    gtk_widget_show (label4);
    gtk_table_attach (GTK_TABLE (table1), label4, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);

    label5 = gtk_label_new (QT_TR_NOOP("Gain"));
    gtk_widget_show (label5);
    gtk_table_attach (GTK_TABLE (table1), label5, 4, 5, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label5), 0, 0.5);

    label6 = gtk_label_new (QT_TR_NOOP("Luma Y"));
    gtk_widget_show (label6);
    gtk_table_attach (GTK_TABLE (table1), label6, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label6), 0, 0.5);

    label7 = gtk_label_new (QT_TR_NOOP("Chroma U"));
    gtk_widget_show (label7);
    gtk_table_attach (GTK_TABLE (table1), label7, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label7), 0, 0.5);

    label8 = gtk_label_new (QT_TR_NOOP("Chroma V"));
    gtk_widget_show (label8);
    gtk_table_attach (GTK_TABLE (table1), label8, 0, 1, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);

    spinbuttonUC_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbuttonUC = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonUC_adj), 1, 0);
    gtk_widget_show (spinbuttonUC);
    gtk_table_attach (GTK_TABLE (table1), spinbuttonUC, 1, 2, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonUC), TRUE);

    spinbuttonVC_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbuttonVC = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonVC_adj), 1, 0);
    gtk_widget_show (spinbuttonVC);
    gtk_table_attach (GTK_TABLE (table1), spinbuttonVC, 1, 2, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonVC), TRUE);

    spinbuttonLB_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbuttonLB = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonLB_adj), 1, 0);
    gtk_widget_show (spinbuttonLB);
    gtk_table_attach (GTK_TABLE (table1), spinbuttonLB, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonLB), TRUE);

    spinbuttonUB_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbuttonUB = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonUB_adj), 1, 0);
    gtk_widget_show (spinbuttonUB);
    gtk_table_attach (GTK_TABLE (table1), spinbuttonUB, 2, 3, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonUB), TRUE);

    spinbuttonVB_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbuttonVB = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonVB_adj), 1, 0);
    gtk_widget_show (spinbuttonVB);
    gtk_table_attach (GTK_TABLE (table1), spinbuttonVB, 2, 3, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonVB), TRUE);

    spinbuttonUG_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbuttonUG = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonUG_adj), 1, 0);
    gtk_widget_show (spinbuttonUG);
    gtk_table_attach (GTK_TABLE (table1), spinbuttonUG, 3, 4, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonUG), TRUE);

    spinbuttonVG_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbuttonVG = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonVG_adj), 1, 0);
    gtk_widget_show (spinbuttonVG);
    gtk_table_attach (GTK_TABLE (table1), spinbuttonVG, 3, 4, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonVG), TRUE);

    spinbuttonLN_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbuttonLN = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonLN_adj), 1, 0);
    gtk_widget_show (spinbuttonLN);
    gtk_table_attach (GTK_TABLE (table1), spinbuttonLN, 4, 5, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonLN), TRUE);

    spinbuttonUN_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbuttonUN = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonUN_adj), 1, 0);
    gtk_widget_show (spinbuttonUN);
    gtk_table_attach (GTK_TABLE (table1), spinbuttonUN, 4, 5, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonUN), TRUE);

    spinbuttonVN_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbuttonVN = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonVN_adj), 1, 0);
    gtk_widget_show (spinbuttonVN);
    gtk_table_attach (GTK_TABLE (table1), spinbuttonVN, 4, 5, 3, 4,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonVN), TRUE);

    spinbutton23_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbutton23 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton23_adj), 1, 0);
    gtk_widget_show (spinbutton23);
    gtk_table_attach (GTK_TABLE (table1), spinbutton23, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton23), TRUE);

    spinbutton24_adj = gtk_adjustment_new (0, -10000, 10000, 1, 1, 1);
    spinbutton24 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton24_adj), 1, 0);
    gtk_widget_show (spinbutton24);
    gtk_table_attach (GTK_TABLE (table1), spinbutton24, 3, 4, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton24), TRUE);

    Color_Correction = gtk_label_new (QT_TR_NOOP("<b>Color Correction Matrix</b>"));
    gtk_widget_show (Color_Correction);
    gtk_frame_set_label_widget (GTK_FRAME (frame1), Color_Correction);
    gtk_label_set_use_markup (GTK_LABEL (Color_Correction), TRUE);

    Settings = gtk_frame_new (NULL);
    gtk_widget_show (Settings);
    gtk_box_pack_start (GTK_BOX (vbox1), Settings, TRUE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (Settings), GTK_SHADOW_NONE);

    alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (alignment2);
    gtk_container_add (GTK_CONTAINER (Settings), alignment2);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 12, 0);

    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox1);
    gtk_container_add (GTK_CONTAINER (alignment2), hbox1);

    table2 = gtk_table_new (3, 2, FALSE);
    gtk_widget_show (table2);
    gtk_box_pack_start (GTK_BOX (hbox1), table2, TRUE, TRUE, 0);

    label10 = gtk_label_new (QT_TR_NOOP("Matrix"));
    gtk_widget_show (label10);
    gtk_table_attach (GTK_TABLE (table2), label10, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label10), 0, 0.5);

    label11 = gtk_label_new (QT_TR_NOOP("Opt"));
    gtk_widget_show (label11);
    gtk_table_attach (GTK_TABLE (table2), label11, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label11), 0, 0.5);

    label12 = gtk_label_new (QT_TR_NOOP("Level"));
    gtk_widget_show (label12);
    gtk_table_attach (GTK_TABLE (table2), label12, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (label12), 0, 0.5);

    comboboxMatrix = gtk_combo_box_new_text ();
    gtk_widget_show (comboboxMatrix);
    gtk_table_attach (GTK_TABLE (table2), comboboxMatrix, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (comboboxMatrix), QT_TR_NOOP("None"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (comboboxMatrix), QT_TR_NOOP("Rec.709"));

    combobox2 = gtk_combo_box_new_text ();
    gtk_widget_show (combobox2);
    gtk_table_attach (GTK_TABLE (table2), combobox2, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (combobox2), QT_TR_NOOP("None"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combobox2), QT_TR_NOOP("Coring"));

    combobox3 = gtk_combo_box_new_text ();
    gtk_widget_show (combobox3);
    gtk_table_attach (GTK_TABLE (table2), combobox3, 1, 2, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (combobox3), QT_TR_NOOP("None"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combobox3), QT_TR_NOOP("TV->PC"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combobox3), QT_TR_NOOP("PC->TV"));
    gtk_combo_box_append_text (GTK_COMBO_BOX (combobox3), QT_TR_NOOP("PC->TV.Y"));

    vbox2 = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (vbox2);
    gtk_box_pack_start (GTK_BOX (hbox1), vbox2, TRUE, TRUE, 0);

    label13 = gtk_label_new (QT_TR_NOOP("<b>Matrix Warning :</b>\nall above values are normalized to 256!"));
    gtk_widget_show (label13);
    gtk_box_pack_start (GTK_BOX (vbox2), label13, FALSE, FALSE, 0);
    gtk_label_set_use_markup (GTK_LABEL (label13), TRUE);
    gtk_label_set_justify (GTK_LABEL (label13), GTK_JUSTIFY_CENTER);

    checkbuttonAutoGain = gtk_check_button_new_with_mnemonic (QT_TR_NOOP("AutoGain"));
    gtk_widget_show (checkbuttonAutoGain);
    gtk_box_pack_start (GTK_BOX (vbox2), checkbuttonAutoGain, FALSE, FALSE, 0);

    checkbuttonAnalyze = gtk_check_button_new_with_mnemonic (QT_TR_NOOP("Display Stats"));
    gtk_widget_show (checkbuttonAnalyze);
    gtk_box_pack_start (GTK_BOX (vbox2), checkbuttonAnalyze, FALSE, FALSE, 0);

    checkbuttonAutoWhite = gtk_check_button_new_with_mnemonic (QT_TR_NOOP("AutoWhite"));
    gtk_widget_show (checkbuttonAutoWhite);
    gtk_box_pack_start (GTK_BOX (vbox2), checkbuttonAutoWhite, FALSE, FALSE, 0);

    label9 = gtk_label_new (QT_TR_NOOP("<b>frame2</b>"));
    gtk_widget_show (label9);
    gtk_frame_set_label_widget (GTK_FRAME (Settings), label9);
    gtk_label_set_use_markup (GTK_LABEL (label9), TRUE);

    dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
    gtk_widget_show (dialog_action_area1);
    gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

    cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
    gtk_widget_show (cancelbutton1);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), cancelbutton1, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

    okbutton1 = gtk_button_new_from_stock ("gtk-ok");
    gtk_widget_show (okbutton1);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), okbutton1, GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);

    /* Store pointers to all widgets, for use by lookup_widget(). */
    GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog1, "dialog1");
    GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_vbox1, "dialog_vbox1");
    GLADE_HOOKUP_OBJECT (dialog1, vbox1, "vbox1");
    GLADE_HOOKUP_OBJECT (dialog1, frame1, "frame1");
    GLADE_HOOKUP_OBJECT (dialog1, alignment1, "alignment1");
    GLADE_HOOKUP_OBJECT (dialog1, table1, "table1");
    GLADE_HOOKUP_OBJECT (dialog1, label2, "label2");
    GLADE_HOOKUP_OBJECT (dialog1, label3, "label3");
    GLADE_HOOKUP_OBJECT (dialog1, label4, "label4");
    GLADE_HOOKUP_OBJECT (dialog1, label5, "label5");
    GLADE_HOOKUP_OBJECT (dialog1, label6, "label6");
    GLADE_HOOKUP_OBJECT (dialog1, label7, "label7");
    GLADE_HOOKUP_OBJECT (dialog1, label8, "label8");
    GLADE_HOOKUP_OBJECT (dialog1, spinbuttonUC, "spinbuttonUC");
    GLADE_HOOKUP_OBJECT (dialog1, spinbuttonVC, "spinbuttonVC");
    GLADE_HOOKUP_OBJECT (dialog1, spinbuttonLB, "spinbuttonLB");
    GLADE_HOOKUP_OBJECT (dialog1, spinbuttonUB, "spinbuttonUB");
    GLADE_HOOKUP_OBJECT (dialog1, spinbuttonVB, "spinbuttonVB");
    GLADE_HOOKUP_OBJECT (dialog1, spinbuttonUG, "spinbuttonUG");
    GLADE_HOOKUP_OBJECT (dialog1, spinbuttonVG, "spinbuttonVG");
    GLADE_HOOKUP_OBJECT (dialog1, spinbuttonLN, "spinbuttonLN");
    GLADE_HOOKUP_OBJECT (dialog1, spinbuttonUN, "spinbuttonUN");
    GLADE_HOOKUP_OBJECT (dialog1, spinbuttonVN, "spinbuttonVN");
    GLADE_HOOKUP_OBJECT (dialog1, spinbutton23, "spinbutton23");
    GLADE_HOOKUP_OBJECT (dialog1, spinbutton24, "spinbutton24");
    GLADE_HOOKUP_OBJECT (dialog1, Color_Correction, "Color_Correction");
    GLADE_HOOKUP_OBJECT (dialog1, Settings, "Settings");
    GLADE_HOOKUP_OBJECT (dialog1, alignment2, "alignment2");
    GLADE_HOOKUP_OBJECT (dialog1, hbox1, "hbox1");
    GLADE_HOOKUP_OBJECT (dialog1, table2, "table2");
    GLADE_HOOKUP_OBJECT (dialog1, label10, "label10");
    GLADE_HOOKUP_OBJECT (dialog1, label11, "label11");
    GLADE_HOOKUP_OBJECT (dialog1, label12, "label12");
    GLADE_HOOKUP_OBJECT (dialog1, comboboxMatrix, "comboboxMatrix");
    GLADE_HOOKUP_OBJECT (dialog1, combobox2, "combobox2");
    GLADE_HOOKUP_OBJECT (dialog1, combobox3, "combobox3");
    GLADE_HOOKUP_OBJECT (dialog1, vbox2, "vbox2");
    GLADE_HOOKUP_OBJECT (dialog1, label13, "label13");
    GLADE_HOOKUP_OBJECT (dialog1, checkbuttonAutoGain, "checkbuttonAutoGain");
    GLADE_HOOKUP_OBJECT (dialog1, checkbuttonAnalyze, "checkbuttonAnalyze");
    GLADE_HOOKUP_OBJECT (dialog1, checkbuttonAutoWhite, "checkbuttonAutoWhite");
    GLADE_HOOKUP_OBJECT (dialog1, label9, "label9");
    GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
    GLADE_HOOKUP_OBJECT (dialog1, cancelbutton1, "cancelbutton1");
    GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");

    return dialog1;
}

