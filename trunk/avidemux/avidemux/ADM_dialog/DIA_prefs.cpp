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


# include <config.h>

#include "callbacks.h"
#include "avi_vars.h"
#include "ADM_gui2/support.h"
#include "ADM_toolkit/toolkit_gtk.h"
#include "ADM_toolkit/toolkit_gtk_include.h"
#include "ADM_toolkit/filesel.h"

# include "prefs.h"
#include "ADM_audiodevice/audio_out.h"

#include "ADM_assert.h"
static GtkWidget	*create_dialog1 (void);

static void on_callback_toolame(GtkButton * button, gpointer user_data);
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
AUDIO_DEVICE olddevice,newdevice;
uint32_t	lavcodec_mpeg=0;
uint32_t        use_odml=0;
uint32_t	autosplit=0;
GtkWidget *wids[10];
uint32_t k;
	
	dialog=create_dialog1();
	gtk_transient(dialog);
	
	olddevice=newdevice=AVDM_getCurrentDevice();
	
	#define CONNECT(A,B)  gtk_signal_connect (GTK_OBJECT(lookup_widget(dialog,#A)), "clicked", \
		      GTK_SIGNAL_FUNC (B), (void *) NULL);

	
	CONNECT(buttonToolame,on_callback_toolame);
	// Alsa
        if( prefs->get(DEVICE_AUDIO_ALSA_DEVICE, &str) != RC_OK )
               str = ADM_strdup("plughw:0,0");
        gtk_write_entry_string(WID(entryAlsa), str);
        ADM_dealloc(str);
	// prepare
	
	if(!prefs->get(TOOLAME_PATH, &str))
	{
		str=(char *)nullstring;		
	}
	
	gtk_write_entry_string(WID(entryToolame), str);

	if(!prefs->get(SETTINGS_MPEGSPLIT, &autosplit))
	{
		autosplit=690;		
	}
			
	if(!prefs->get(FEATURE_USE_LAVCODEC_MPEG, &lavcodec_mpeg))
	{
		lavcodec_mpeg=0;		
	}		
	gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(WID(checkbutton_lavcodec)),
			lavcodec_mpeg);
        
        // Open DML (Gmv)
        if(!prefs->get(FEATURE_USE_ODML, &use_odml))
        {
          use_odml=0;                
        }               
        gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(WID(checkbuttonODML)),
                                      use_odml);

	// Audio device
	for(uint32_t i=0;i<sizeof(audioDeviceList)/sizeof(DEVICELIST);i++)
	{
		if(audioDeviceList[i].id==olddevice)
			{
				k=i;
			}
		wids[i] = gtk_menu_item_new_with_mnemonic ( audioDeviceList[i].name);
		gtk_widget_show (wids[i]);
		gtk_container_add (GTK_CONTAINER (WID(menu1)), wids[i]);
	}		
	 gtk_option_menu_set_menu (GTK_OPTION_MENU (WID(optionmenu1)), WID(menu1));
	 gtk_option_menu_set_history(GTK_OPTION_MENU(WID(optionmenu1)), k);
	 // Fill entry
	 gtk_write_entry(WID(entry_mpeg2enc), (int) autosplit);
	// run

	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_OK)
	{
		ret=1;
		str =	gtk_editable_get_chars(GTK_EDITABLE (WID(entryToolame)), 0, -1);			
		prefs->set(TOOLAME_PATH, str);
		// Get device
		k=getRangeInMenu(WID(optionmenu1));
		newdevice=audioDeviceList[k].id;
		if(newdevice!=olddevice)
		{
			AVDM_switch(newdevice);
		}
		lavcodec_mpeg=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(WID(checkbutton_lavcodec)));
		prefs->set(FEATURE_USE_LAVCODEC_MPEG, lavcodec_mpeg);
                
                use_odml=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(WID(checkbuttonODML)));
                prefs->set(FEATURE_USE_ODML, use_odml);
                
                
		autosplit=gtk_read_entry(WID(entry_mpeg2enc));
		prefs->set(SETTINGS_MPEGSPLIT, autosplit);
                
                //alsa device
                str=gtk_editable_get_chars(GTK_EDITABLE (WID(entryAlsa)), 0, -1);
                if(str)
                        prefs->set(DEVICE_AUDIO_ALSA_DEVICE, str);
		
	}

	gtk_widget_destroy(dialog);
	dialog=NULL;
	return ret;
}
/*
void on_callback_lame(GtkButton * button, gpointer user_data)
{
char *lame=NULL;

	 GUI_FileSelRead("Where is lame ?", &lame);
	 if(lame)
	 {
	 	gtk_write_entry_string(WID(entryLame), lame);
	 	
	 }

}
*/
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

GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *frame1;
  GtkWidget *alignment1;
  GtkWidget *table1;
  GtkWidget *buttonToolame;
  GtkWidget *entryToolame;
  GtkWidget *entryLame;
  GtkWidget *label7;
  GtkWidget *label8;
  GtkWidget *buttonLame;
  GtkWidget *label12;
  GtkWidget *checkbutton_lavcodec;
  GtkWidget *label13;
  GtkWidget *label14;
  GtkWidget *checkbuttonODML;
  GtkWidget *label15;
  GtkWidget *label16;
  GtkWidget *label17;
  GtkWidget *entry_mpeg2enc;
  GtkWidget *label1;
  GtkWidget *frame2;
  GtkWidget *vbox2;
  GtkWidget *hbox2;
  GtkWidget *label11;
  GtkWidget *optionmenu1;
  GtkWidget *menu1;
  GtkWidget *hbox3;
  GtkWidget *label19;
  GtkWidget *entryAlsa;
  GtkWidget *label18;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  dialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog), _("Preferences"));
  gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

  dialog_vbox1 = GTK_DIALOG (dialog)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 10);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox1), 5);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox1), frame1, FALSE, FALSE, 0);

  alignment1 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (frame1), alignment1);

  table1 = gtk_table_new (5, 3, FALSE);
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (alignment1), table1);
  gtk_container_set_border_width (GTK_CONTAINER (table1), 5);
  gtk_table_set_row_spacings (GTK_TABLE (table1), 5);
  gtk_table_set_col_spacings (GTK_TABLE (table1), 5);

  buttonToolame = gtk_button_new_with_mnemonic (_("Browse..."));
  gtk_widget_show (buttonToolame);
  gtk_table_attach (GTK_TABLE (table1), buttonToolame, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_sensitive (buttonToolame, FALSE);

  entryToolame = gtk_entry_new ();
  gtk_widget_show (entryToolame);
  gtk_table_attach (GTK_TABLE (table1), entryToolame, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_sensitive (entryToolame, FALSE);
  gtk_entry_set_width_chars (GTK_ENTRY (entryToolame), 25);

  entryLame = gtk_entry_new ();
  gtk_widget_show (entryLame);
  gtk_table_attach (GTK_TABLE (table1), entryLame, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_sensitive (entryLame, FALSE);
  gtk_entry_set_width_chars (GTK_ENTRY (entryLame), 25);

  label7 = gtk_label_new (_("tooLAME:"));
  gtk_widget_show (label7);
  gtk_table_attach (GTK_TABLE (table1), label7, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label7), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label7), 0, 0.5);

  label8 = gtk_label_new (_("LAME:"));
  gtk_widget_show (label8);
  gtk_table_attach (GTK_TABLE (table1), label8, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label8), 0, 0.5);

  buttonLame = gtk_button_new_with_mnemonic (_("Browse..."));
  gtk_widget_show (buttonLame);
  gtk_table_attach (GTK_TABLE (table1), buttonLame, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_sensitive (buttonLame, FALSE);

  label12 = gtk_label_new (_("Mpeg auto split (MB)"));
  gtk_widget_show (label12);
  gtk_table_attach (GTK_TABLE (table1), label12, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label12), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label12), 0, 0.5);

  checkbutton_lavcodec = gtk_check_button_new_with_mnemonic (_("Use lavcodec mpeg decoder"));
  gtk_widget_show (checkbutton_lavcodec);
  gtk_table_attach (GTK_TABLE (table1), checkbutton_lavcodec, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label13 = gtk_label_new ("");
  gtk_widget_show (label13);
  gtk_table_attach (GTK_TABLE (table1), label13, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label13), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label13), 0, 0.5);

  label14 = gtk_label_new ("");
  gtk_widget_show (label14);
  gtk_table_attach (GTK_TABLE (table1), label14, 2, 3, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label14), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label14), 0, 0.5);

  checkbuttonODML = gtk_check_button_new_with_mnemonic (_("Use OpenDML rather than avi 1.0"));
  gtk_widget_show (checkbuttonODML);
  gtk_table_attach (GTK_TABLE (table1), checkbuttonODML, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label15 = gtk_label_new ("");
  gtk_widget_show (label15);
  gtk_table_attach (GTK_TABLE (table1), label15, 2, 3, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label15), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label15), 0, 0.5);

  label16 = gtk_label_new ("");
  gtk_widget_show (label16);
  gtk_table_attach (GTK_TABLE (table1), label16, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label16), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label16), 0, 0.5);

  label17 = gtk_label_new ("");
  gtk_widget_show (label17);
  gtk_table_attach (GTK_TABLE (table1), label17, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label17), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label17), 0, 0.5);

  entry_mpeg2enc = gtk_entry_new ();
  gtk_widget_show (entry_mpeg2enc);
  gtk_table_attach (GTK_TABLE (table1), entry_mpeg2enc, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label1 = gtk_label_new (_("<b>Paths</b>"));
  gtk_widget_show (label1);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label1);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_show (frame2);
  gtk_box_pack_start (GTK_BOX (vbox1), frame2, FALSE, FALSE, 0);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (frame2), vbox2);

  hbox2 = gtk_hbox_new (FALSE, 5);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox2, TRUE, TRUE, 0);

  label11 = gtk_label_new (_("Audio device:"));
  gtk_widget_show (label11);
  gtk_box_pack_start (GTK_BOX (hbox2), label11, FALSE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label11), GTK_JUSTIFY_LEFT);

  optionmenu1 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu1);
  gtk_box_pack_start (GTK_BOX (hbox2), optionmenu1, FALSE, TRUE, 0);

  menu1 = gtk_menu_new ();

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu1), menu1);

  hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox3, TRUE, TRUE, 0);

  label19 = gtk_label_new (_("Device for Alsa :"));
  gtk_widget_show (label19);
  gtk_box_pack_start (GTK_BOX (hbox3), label19, FALSE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label19), GTK_JUSTIFY_LEFT);

  entryAlsa = gtk_entry_new ();
  gtk_widget_show (entryAlsa);
  gtk_box_pack_start (GTK_BOX (hbox3), entryAlsa, FALSE, TRUE, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entryAlsa), 10);

  label18 = gtk_label_new (_("<b>Audio</b>"));
  gtk_widget_show (label18);
  gtk_frame_set_label_widget (GTK_FRAME (frame2), label18);
  gtk_label_set_use_markup (GTK_LABEL (label18), TRUE);
  gtk_label_set_justify (GTK_LABEL (label18), GTK_JUSTIFY_LEFT);

  dialog_action_area1 = GTK_DIALOG (dialog)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog), cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

  okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog), okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog, dialog, "dialog");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (dialog, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (dialog, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (dialog, alignment1, "alignment1");
  GLADE_HOOKUP_OBJECT (dialog, table1, "table1");
  GLADE_HOOKUP_OBJECT (dialog, buttonToolame, "buttonToolame");
  GLADE_HOOKUP_OBJECT (dialog, entryToolame, "entryToolame");
  GLADE_HOOKUP_OBJECT (dialog, entryLame, "entryLame");
  GLADE_HOOKUP_OBJECT (dialog, label7, "label7");
  GLADE_HOOKUP_OBJECT (dialog, label8, "label8");
  GLADE_HOOKUP_OBJECT (dialog, buttonLame, "buttonLame");
  GLADE_HOOKUP_OBJECT (dialog, label12, "label12");
  GLADE_HOOKUP_OBJECT (dialog, checkbutton_lavcodec, "checkbutton_lavcodec");
  GLADE_HOOKUP_OBJECT (dialog, label13, "label13");
  GLADE_HOOKUP_OBJECT (dialog, label14, "label14");
  GLADE_HOOKUP_OBJECT (dialog, checkbuttonODML, "checkbuttonODML");
  GLADE_HOOKUP_OBJECT (dialog, label15, "label15");
  GLADE_HOOKUP_OBJECT (dialog, label16, "label16");
  GLADE_HOOKUP_OBJECT (dialog, label17, "label17");
  GLADE_HOOKUP_OBJECT (dialog, entry_mpeg2enc, "entry_mpeg2enc");
  GLADE_HOOKUP_OBJECT (dialog, label1, "label1");
  GLADE_HOOKUP_OBJECT (dialog, frame2, "frame2");
  GLADE_HOOKUP_OBJECT (dialog, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (dialog, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (dialog, label11, "label11");
  GLADE_HOOKUP_OBJECT (dialog, optionmenu1, "optionmenu1");
  GLADE_HOOKUP_OBJECT (dialog, menu1, "menu1");
  GLADE_HOOKUP_OBJECT (dialog, hbox3, "hbox3");
  GLADE_HOOKUP_OBJECT (dialog, label19, "label19");
  GLADE_HOOKUP_OBJECT (dialog, entryAlsa, "entryAlsa");
  GLADE_HOOKUP_OBJECT (dialog, label18, "label18");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialog, okbutton1, "okbutton1");

  return dialog;
}

