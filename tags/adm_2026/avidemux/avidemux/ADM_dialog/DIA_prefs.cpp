//
/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>


#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

# include <config.h>
#include "callbacks.h"
#include "avi_vars.h"
#include "ADM_gui2/support.h"
#include "ADM_toolkit/toolkit_gtk.h"
#include "ADM_toolkit/toolkit_gtk_include.h"
#include "ADM_toolkit/filesel.h"

# include "prefs.h"


static GtkWidget	*create_dialog1 (void);
static void on_callback_lame(GtkButton * button, gpointer user_data);
static void on_callback_toolame(GtkButton * button, gpointer user_data);
static void on_callback_lvemux(GtkButton * button, gpointer user_data);
static void on_callback_requant(GtkButton * button, gpointer user_data);
static void on_callback_audio(GtkButton * button, gpointer user_data);
static GtkWidget *dialog=NULL;

extern void 		AVDM_audioPref( void );

uint8_t DIA_Preferences(void);
uint8_t DIA_Preferences(void)
{
uint8_t ret=0;
gint r;
char *str;
const char *nullstring="";
	
	
	dialog=create_dialog1();
	gtk_transient(dialog);
	
	
	#define CONNECT(A,B)  gtk_signal_connect (GTK_OBJECT(lookup_widget(dialog,#A)), "clicked", \
		      GTK_SIGNAL_FUNC (B), (void *) NULL);

	CONNECT(buttonLame,on_callback_lame);
	CONNECT(buttonToolame,on_callback_toolame);
	CONNECT(buttonLvemux,on_callback_lvemux);
	CONNECT(buttonRequant,on_callback_requant);	
	CONNECT(button_audioDevice,on_callback_audio);	

	// prepare
	
	if(!prefs->get(TOOLAME_PATH, &str))
	{
		str=(char *)nullstring;		
	}
	
	gtk_write_entry_string(WID(entryToolame), str);
	
		
	if(!prefs->get(LAME_PATH, &str))
	{
		str=(char *)nullstring;		
	}
	gtk_write_entry_string(WID(entryLame), str);
	
	
	if(!prefs->get(LVEMUX_PATH, &str))
	{
		str=(char *)nullstring;		
	}
	gtk_write_entry_string(WID(entryLvemux), str);
	
	if(!prefs->get(REQUANT_PATH, &str))
	{
		str=(char *)nullstring;		
	}
	gtk_write_entry_string(WID(entryRequant), str);
	


	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_OK)
	{
		ret=1;
		str =	gtk_editable_get_chars(GTK_EDITABLE (WID(entryToolame)), 0, -1);			
		prefs->set(TOOLAME_PATH, str);
		str =	gtk_editable_get_chars(GTK_EDITABLE (WID(entryLame)), 0, -1);			
		prefs->set(LAME_PATH, str);
		str =	gtk_editable_get_chars(GTK_EDITABLE (WID(entryLvemux)), 0, -1);			
		prefs->set(LVEMUX_PATH, str);
		str =	gtk_editable_get_chars(GTK_EDITABLE (WID(entryRequant)), 0, -1);			
		prefs->set(REQUANT_PATH, str);
	}

	gtk_widget_destroy(dialog);
	dialog=NULL;
	return ret;
}
void on_callback_audio(GtkButton * button, gpointer user_data)
{
	printf("Audio device\n");
	AVDM_audioPref();
}
void on_callback_lame(GtkButton * button, gpointer user_data)
{
char *lame=NULL;

	 GUI_FileSelRead("Where is lame ?", &lame);
	 if(lame)
	 {
	 	gtk_write_entry_string(WID(entryLame), lame);
	 	
	 }

}

void on_callback_toolame(GtkButton * button, gpointer user_data)
{
char *lame=NULL;
gint r;
	 GUI_FileSelRead("Where is toolame ?", &lame);
	 if(lame)
	 {
	 	gtk_write_entry_string(WID(entryToolame), lame);		
	 }


}
void on_callback_lvemux(GtkButton * button, gpointer user_data)
{
char *lame=NULL;
gint r;
	 GUI_FileSelRead("Where is lvemux ?", &lame);
	 if(lame)
	 {
	 	gtk_write_entry_string(WID(entryLvemux), lame);
	 }


}
    
void on_callback_requant(GtkButton * button, gpointer user_data)
{
char *lame=NULL;

	 GUI_FileSelRead("Where is M2VRequantizer ?", &lame);
	 if(lame)
	 {
	 	gtk_write_entry_string(WID(entryRequant), lame);
	 	
	 }

}

GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *table1;
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *label4;
  GtkWidget *entryToolame;
  GtkWidget *buttonToolame;
  GtkWidget *label5;
  GtkWidget *entryLame;
  GtkWidget *buttonLame;
  GtkWidget *label6;
  GtkWidget *entryLvemux;
  GtkWidget *buttonLvemux;
  GtkWidget *label7;
  GtkWidget *entryRequant;
  GtkWidget *buttonRequant;
  GtkWidget *label8;
  GtkWidget *button_audioDevice;
  GtkWidget *hseparator1;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), _("Preferences"));

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  table1 = gtk_table_new (6, 3, FALSE);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), table1, TRUE, TRUE, 0);

  label1 = gtk_label_new (_("Name"));
  gtk_widget_show (label1);
  gtk_table_attach (GTK_TABLE (table1), label1, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label1), 0, 0.5);

  label2 = gtk_label_new (_("Path"));
  gtk_widget_show (label2);
  gtk_table_attach (GTK_TABLE (table1), label2, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label2), 0, 0.5);

  label3 = gtk_label_new (_("Select"));
  gtk_widget_show (label3);
  gtk_table_attach (GTK_TABLE (table1), label3, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label3), 0, 0.5);

  label4 = gtk_label_new (_("Toolame"));
  gtk_widget_show (label4);
  gtk_table_attach (GTK_TABLE (table1), label4, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);

  entryToolame = gtk_entry_new ();
  gtk_widget_show (entryToolame);
  gtk_table_attach (GTK_TABLE (table1), entryToolame, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  buttonToolame = gtk_button_new_with_mnemonic (_("Select"));
  gtk_widget_show (buttonToolame);
  gtk_table_attach (GTK_TABLE (table1), buttonToolame, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label5 = gtk_label_new (_("Lame"));
  gtk_widget_show (label5);
  gtk_table_attach (GTK_TABLE (table1), label5, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label5), 0, 0.5);

  entryLame = gtk_entry_new ();
  gtk_widget_show (entryLame);
  gtk_table_attach (GTK_TABLE (table1), entryLame, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  buttonLame = gtk_button_new_with_mnemonic (_("Select"));
  gtk_widget_show (buttonLame);
  gtk_table_attach (GTK_TABLE (table1), buttonLame, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label6 = gtk_label_new (_("LveMux"));
  gtk_widget_show (label6);
  gtk_table_attach (GTK_TABLE (table1), label6, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label6), 0, 0.5);

  entryLvemux = gtk_entry_new ();
  gtk_widget_show (entryLvemux);
  gtk_table_attach (GTK_TABLE (table1), entryLvemux, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  buttonLvemux = gtk_button_new_with_mnemonic (_("Select"));
  gtk_widget_show (buttonLvemux);
  gtk_table_attach (GTK_TABLE (table1), buttonLvemux, 2, 3, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label7 = gtk_label_new (_("M2VRequantizer"));
  gtk_widget_show (label7);
  gtk_table_attach (GTK_TABLE (table1), label7, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label7), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label7), 0, 0.5);

  entryRequant = gtk_entry_new ();
  gtk_widget_show (entryRequant);
  gtk_table_attach (GTK_TABLE (table1), entryRequant, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  buttonRequant = gtk_button_new_with_mnemonic (_("Select"));
  gtk_widget_show (buttonRequant);
  gtk_table_attach (GTK_TABLE (table1), buttonRequant, 2, 3, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label8 = gtk_label_new (_("AudioDevice"));
  gtk_widget_show (label8);
  gtk_table_attach (GTK_TABLE (table1), label8, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);

  button_audioDevice = gtk_button_new_with_mnemonic (_("Select Device"));
  gtk_widget_show (button_audioDevice);
  gtk_table_attach (GTK_TABLE (table1), button_audioDevice, 1, 2, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_show (hseparator1);
  gtk_table_attach (GTK_TABLE (table1), hseparator1, 2, 3, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

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
  GLADE_HOOKUP_OBJECT (dialog1, table1, "table1");
  GLADE_HOOKUP_OBJECT (dialog1, label1, "label1");
  GLADE_HOOKUP_OBJECT (dialog1, label2, "label2");
  GLADE_HOOKUP_OBJECT (dialog1, label3, "label3");
  GLADE_HOOKUP_OBJECT (dialog1, label4, "label4");
  GLADE_HOOKUP_OBJECT (dialog1, entryToolame, "entryToolame");
  GLADE_HOOKUP_OBJECT (dialog1, buttonToolame, "buttonToolame");
  GLADE_HOOKUP_OBJECT (dialog1, label5, "label5");
  GLADE_HOOKUP_OBJECT (dialog1, entryLame, "entryLame");
  GLADE_HOOKUP_OBJECT (dialog1, buttonLame, "buttonLame");
  GLADE_HOOKUP_OBJECT (dialog1, label6, "label6");
  GLADE_HOOKUP_OBJECT (dialog1, entryLvemux, "entryLvemux");
  GLADE_HOOKUP_OBJECT (dialog1, buttonLvemux, "buttonLvemux");
  GLADE_HOOKUP_OBJECT (dialog1, label7, "label7");
  GLADE_HOOKUP_OBJECT (dialog1, entryRequant, "entryRequant");
  GLADE_HOOKUP_OBJECT (dialog1, buttonRequant, "buttonRequant");
  GLADE_HOOKUP_OBJECT (dialog1, label8, "label8");
  GLADE_HOOKUP_OBJECT (dialog1, button_audioDevice, "button_audioDevice");
  GLADE_HOOKUP_OBJECT (dialog1, hseparator1, "hseparator1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");

  return dialog1;
}

