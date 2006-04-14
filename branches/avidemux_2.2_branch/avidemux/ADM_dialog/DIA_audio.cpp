/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */
# include <config.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>


#include "ADM_assert.h" 

#include "ADM_library/default.h"
#include "ADM_gui2/support.h"
#include "ADM_toolkit/toolkit_gtk.h"
#include "ADM_toolkit/toolkit_gtk_include.h"
#include "ADM_audiofilter/audioeng_buildfilters.h"


#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)
  
 static GtkWidget	*create_dialogAudio (void);
 
 int DIA_getAudioFilter(int *normalized, RESAMPLING *downsamplingmethod, int *tshifted,
  			 int *shiftvalue, int *drc,int *freqvalue,FILMCONV *filmconv,
                         CHANNEL_CONF *channel);
			 
  int DIA_getAudioFilter(int *normalized, RESAMPLING *downsamplingmethod, int *tshifted,
  			 int *shiftvalue, int *drc,int *freqvalue,FILMCONV *filmconv,
                         CHANNEL_CONF *channel)
  {
  GtkWidget *dialog;
  gint result;
  int ret=0;

  	dialog=create_dialogAudio();
//	gtk_transient(dialog);
        gtk_register_dialog(dialog);

	#define SET(a,b) if(*b) 		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(dialog,#a)),1);
			SET(checkbuttonNormalize,normalized);			
			SET(checkbuttonTimeShift,tshifted);	
			SET(checkbuttonDRC,drc);				
	#undef SET			    
				
	
				
					
				
	gtk_write_entry(WID(entryTimeshift), *shiftvalue);  
	gtk_write_entry(WID(entryFrequency), *freqvalue);      
	
	switch(*downsamplingmethod)
	{	
		case RESAMPLING_NONE:			RADIO_SET(radiobuttonNone,1);break;
		case RESAMPLING_DOWNSAMPLING:		RADIO_SET(radiobutton2,1);break;
		case RESAMPLING_CUSTOM:			RADIO_SET(radiobuttonSox,1);break;
		default:ADM_assert(0);
	}
	switch(*filmconv)
	{	
		case FILMCONV_NONE:		RADIO_SET(radiobutton_fpsnone,1);break;
		case FILMCONV_FILM2PAL:		RADIO_SET(radiobutton_fpsfilm,1);break;
		case FILMCONV_PAL2FILM:		RADIO_SET(radiobutton_fpsPAL,1);break;
		default:ADM_assert(0);
	}
        gtk_combo_box_set_active(GTK_COMBO_BOX(WID(combobox1)),*channel);
	
	result=gtk_dialog_run(GTK_DIALOG(dialog));
			
	switch(result)
	{
		case GTK_RESPONSE_OK:

					ret= 1;
					#define SET(a,b)	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(dialog,#a)))) \
							{ *b=1;;} else *b=0;
					SET(checkbuttonNormalize,normalized);					
					SET(checkbuttonTimeShift,tshifted);	
					SET(checkbuttonDRC,drc);				
					#undef SET	
                                        *channel=(CHANNEL_CONF)gtk_combo_box_get_active(GTK_COMBO_BOX(WID(combobox1)));
					*shiftvalue=gtk_read_entry(WID(entryTimeshift) );  
					*freqvalue=gtk_read_entry(WID(entryFrequency) );  
					if(RADIO_GET(radiobuttonNone)) *downsamplingmethod=RESAMPLING_NONE;
					else if(RADIO_GET(radiobutton2)) 
						*downsamplingmethod=RESAMPLING_DOWNSAMPLING;
						else if(RADIO_GET(radiobuttonSox))
							*downsamplingmethod=RESAMPLING_CUSTOM; 
							else
								ADM_assert(0);
					if(RADIO_GET(radiobutton_fpsnone)) *filmconv=FILMCONV_NONE;
					else if(RADIO_GET(radiobutton_fpsfilm)) 
						*filmconv=FILMCONV_FILM2PAL;
						else if(RADIO_GET(radiobutton_fpsPAL))
							*filmconv=FILMCONV_PAL2FILM; 
							else
								ADM_assert(0);
										
					
					break;
		default:
					ret=0;
					break;	
	}
	gtk_unregister_dialog(dialog);
	gtk_widget_destroy(dialog);
	return ret;	    
  }
  
  GtkWidget*
          create_dialogAudio (void)
  {
      GtkWidget *dialogAudio;
      GtkWidget *dialog_vbox1;
      GtkWidget *vbox3;
      GtkWidget *frame3;
      GtkWidget *vbox6;
      GtkWidget *checkbuttonNormalize;
      GtkWidget *checkbuttonDRC;
      GtkWidget *hbox3;
      GtkWidget *checkbuttonTimeShift;
      GtkWidget *entryTimeshift;
      GtkWidget *label5;
      GtkWidget *frame1;
      GtkWidget *vbox4;
      GtkWidget *radiobuttonNone;
      GSList *radiobuttonNone_group = NULL;
      GtkWidget *radiobutton2;
      GtkWidget *hbox2;
      GtkWidget *radiobuttonSox;
      GtkWidget *entryFrequency;
      GtkWidget *label3;
      GtkWidget *frame2;
      GtkWidget *vbox5;
      GtkWidget *radiobutton_fpsnone;
      GSList *radiobutton_fpsnone_group = NULL;
      GtkWidget *radiobutton_fpsfilm;
      GtkWidget *radiobutton_fpsPAL;
      GtkWidget *label4;
      GtkWidget *frame4;
      GtkWidget *combobox1;
      GtkWidget *label6;
      GtkWidget *dialog_action_area1;
      GtkWidget *cancelbutton1;
      GtkWidget *okbutton1;

      dialogAudio = gtk_dialog_new ();
      gtk_window_set_title (GTK_WINDOW (dialogAudio), _("Audio Processing"));
      gtk_window_set_type_hint (GTK_WINDOW (dialogAudio), GDK_WINDOW_TYPE_HINT_DIALOG);

      dialog_vbox1 = GTK_DIALOG (dialogAudio)->vbox;
      gtk_widget_show (dialog_vbox1);

      vbox3 = gtk_vbox_new (FALSE, 0);
      gtk_widget_show (vbox3);
      gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox3, FALSE, TRUE, 0);

      frame3 = gtk_frame_new (NULL);
      gtk_widget_show (frame3);
      gtk_box_pack_start (GTK_BOX (vbox3), frame3, FALSE, FALSE, 0);

      vbox6 = gtk_vbox_new (FALSE, 0);
      gtk_widget_show (vbox6);
      gtk_container_add (GTK_CONTAINER (frame3), vbox6);

      checkbuttonNormalize = gtk_check_button_new_with_mnemonic (_("Normalize"));
      gtk_widget_show (checkbuttonNormalize);
      gtk_box_pack_start (GTK_BOX (vbox6), checkbuttonNormalize, FALSE, FALSE, 0);

      checkbuttonDRC = gtk_check_button_new_with_mnemonic (_("DRC"));
      gtk_widget_show (checkbuttonDRC);
      gtk_box_pack_start (GTK_BOX (vbox6), checkbuttonDRC, FALSE, FALSE, 0);

      hbox3 = gtk_hbox_new (FALSE, 0);
      gtk_widget_show (hbox3);
      gtk_box_pack_start (GTK_BOX (vbox6), hbox3, TRUE, TRUE, 0);

      checkbuttonTimeShift = gtk_check_button_new_with_mnemonic (_("Timeshift"));
      gtk_widget_show (checkbuttonTimeShift);
      gtk_box_pack_start (GTK_BOX (hbox3), checkbuttonTimeShift, FALSE, FALSE, 0);

      entryTimeshift = gtk_entry_new ();
      gtk_widget_show (entryTimeshift);
      gtk_box_pack_start (GTK_BOX (hbox3), entryTimeshift, TRUE, TRUE, 0);
      gtk_entry_set_max_length (GTK_ENTRY (entryTimeshift), 5);

      label5 = gtk_label_new (_("<b>Misc</b>"));
      gtk_widget_show (label5);
      gtk_frame_set_label_widget (GTK_FRAME (frame3), label5);
      gtk_label_set_use_markup (GTK_LABEL (label5), TRUE);

      frame1 = gtk_frame_new (NULL);
      gtk_widget_show (frame1);
      gtk_box_pack_start (GTK_BOX (vbox3), frame1, FALSE, FALSE, 0);

      vbox4 = gtk_vbox_new (FALSE, 0);
      gtk_widget_show (vbox4);
      gtk_container_add (GTK_CONTAINER (frame1), vbox4);

      radiobuttonNone = gtk_radio_button_new_with_mnemonic (NULL, _("None"));
      gtk_widget_show (radiobuttonNone);
      gtk_box_pack_start (GTK_BOX (vbox4), radiobuttonNone, FALSE, FALSE, 0);
      gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobuttonNone), radiobuttonNone_group);
      radiobuttonNone_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobuttonNone));

      radiobutton2 = gtk_radio_button_new_with_mnemonic (NULL, _("48 to 44.1 khz"));
      gtk_widget_show (radiobutton2);
      gtk_box_pack_start (GTK_BOX (vbox4), radiobutton2, FALSE, FALSE, 0);
      gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton2), radiobuttonNone_group);
      radiobuttonNone_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton2));

      hbox2 = gtk_hbox_new (FALSE, 0);
      gtk_widget_show (hbox2);
      gtk_box_pack_start (GTK_BOX (vbox4), hbox2, TRUE, TRUE, 0);

      radiobuttonSox = gtk_radio_button_new_with_mnemonic (NULL, _("Resample to hz"));
      gtk_widget_show (radiobuttonSox);
      gtk_box_pack_start (GTK_BOX (hbox2), radiobuttonSox, FALSE, FALSE, 0);
      gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobuttonSox), radiobuttonNone_group);
      radiobuttonNone_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobuttonSox));

      entryFrequency = gtk_entry_new ();
      gtk_widget_show (entryFrequency);
      gtk_box_pack_start (GTK_BOX (hbox2), entryFrequency, TRUE, TRUE, 0);

      label3 = gtk_label_new (_("<b>Resampling</b>"));
      gtk_widget_show (label3);
      gtk_frame_set_label_widget (GTK_FRAME (frame1), label3);
      gtk_label_set_use_markup (GTK_LABEL (label3), TRUE);

      frame2 = gtk_frame_new (NULL);
      gtk_widget_show (frame2);
      gtk_box_pack_start (GTK_BOX (vbox3), frame2, FALSE, TRUE, 0);

      vbox5 = gtk_vbox_new (FALSE, 0);
      gtk_widget_show (vbox5);
      gtk_container_add (GTK_CONTAINER (frame2), vbox5);

      radiobutton_fpsnone = gtk_radio_button_new_with_mnemonic (NULL, _("None"));
      gtk_widget_show (radiobutton_fpsnone);
      gtk_box_pack_start (GTK_BOX (vbox5), radiobutton_fpsnone, FALSE, FALSE, 0);
      gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_fpsnone), radiobutton_fpsnone_group);
      radiobutton_fpsnone_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton_fpsnone));

      radiobutton_fpsfilm = gtk_radio_button_new_with_mnemonic (NULL, _("Film -> PAL"));
      gtk_widget_show (radiobutton_fpsfilm);
      gtk_box_pack_start (GTK_BOX (vbox5), radiobutton_fpsfilm, FALSE, FALSE, 0);
      gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_fpsfilm), radiobutton_fpsnone_group);
      radiobutton_fpsnone_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton_fpsfilm));

      radiobutton_fpsPAL = gtk_radio_button_new_with_mnemonic (NULL, _("PAL->Film"));
      gtk_widget_show (radiobutton_fpsPAL);
      gtk_box_pack_start (GTK_BOX (vbox5), radiobutton_fpsPAL, FALSE, FALSE, 0);
      gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_fpsPAL), radiobutton_fpsnone_group);
      radiobutton_fpsnone_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton_fpsPAL));

      label4 = gtk_label_new (_("<b>Fps convert</b>"));
      gtk_widget_show (label4);
      gtk_frame_set_label_widget (GTK_FRAME (frame2), label4);
      gtk_label_set_use_markup (GTK_LABEL (label4), TRUE);

      frame4 = gtk_frame_new (NULL);
      gtk_widget_show (frame4);
      gtk_box_pack_start (GTK_BOX (vbox3), frame4, FALSE, TRUE, 0);

      combobox1 = gtk_combo_box_new_text ();
      gtk_widget_show (combobox1);
      gtk_container_add (GTK_CONTAINER (frame4), combobox1);
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("No change"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("Mono"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("Stereo"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("Stereo+Surround"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("Stereo+Center"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("Stereo+Center+Surround"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("Stereo Front+Stero Rear"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("5 channels"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("5.1"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("Dolby Surround"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("Dolby Prologic"));
      gtk_combo_box_append_text (GTK_COMBO_BOX (combobox1), _("Dolby Prologic 2"));

      label6 = gtk_label_new (_("<b>Mixer</b>"));
      gtk_widget_show (label6);
      gtk_frame_set_label_widget (GTK_FRAME (frame4), label6);
      gtk_label_set_use_markup (GTK_LABEL (label6), TRUE);

      dialog_action_area1 = GTK_DIALOG (dialogAudio)->action_area;
      gtk_widget_show (dialog_action_area1);
      gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

      cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
      gtk_widget_show (cancelbutton1);
      gtk_dialog_add_action_widget (GTK_DIALOG (dialogAudio), cancelbutton1, GTK_RESPONSE_CANCEL);
      GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

      okbutton1 = gtk_button_new_from_stock ("gtk-ok");
      gtk_widget_show (okbutton1);
      gtk_dialog_add_action_widget (GTK_DIALOG (dialogAudio), okbutton1, GTK_RESPONSE_OK);
      GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);

      /* Store pointers to all widgets, for use by lookup_widget(). */
      GLADE_HOOKUP_OBJECT_NO_REF (dialogAudio, dialogAudio, "dialogAudio");
      GLADE_HOOKUP_OBJECT_NO_REF (dialogAudio, dialog_vbox1, "dialog_vbox1");
      GLADE_HOOKUP_OBJECT (dialogAudio, vbox3, "vbox3");
      GLADE_HOOKUP_OBJECT (dialogAudio, frame3, "frame3");
      GLADE_HOOKUP_OBJECT (dialogAudio, vbox6, "vbox6");
      GLADE_HOOKUP_OBJECT (dialogAudio, checkbuttonNormalize, "checkbuttonNormalize");
      GLADE_HOOKUP_OBJECT (dialogAudio, checkbuttonDRC, "checkbuttonDRC");
      GLADE_HOOKUP_OBJECT (dialogAudio, hbox3, "hbox3");
      GLADE_HOOKUP_OBJECT (dialogAudio, checkbuttonTimeShift, "checkbuttonTimeShift");
      GLADE_HOOKUP_OBJECT (dialogAudio, entryTimeshift, "entryTimeshift");
      GLADE_HOOKUP_OBJECT (dialogAudio, label5, "label5");
      GLADE_HOOKUP_OBJECT (dialogAudio, frame1, "frame1");
      GLADE_HOOKUP_OBJECT (dialogAudio, vbox4, "vbox4");
      GLADE_HOOKUP_OBJECT (dialogAudio, radiobuttonNone, "radiobuttonNone");
      GLADE_HOOKUP_OBJECT (dialogAudio, radiobutton2, "radiobutton2");
      GLADE_HOOKUP_OBJECT (dialogAudio, hbox2, "hbox2");
      GLADE_HOOKUP_OBJECT (dialogAudio, radiobuttonSox, "radiobuttonSox");
      GLADE_HOOKUP_OBJECT (dialogAudio, entryFrequency, "entryFrequency");
      GLADE_HOOKUP_OBJECT (dialogAudio, label3, "label3");
      GLADE_HOOKUP_OBJECT (dialogAudio, frame2, "frame2");
      GLADE_HOOKUP_OBJECT (dialogAudio, vbox5, "vbox5");
      GLADE_HOOKUP_OBJECT (dialogAudio, radiobutton_fpsnone, "radiobutton_fpsnone");
      GLADE_HOOKUP_OBJECT (dialogAudio, radiobutton_fpsfilm, "radiobutton_fpsfilm");
      GLADE_HOOKUP_OBJECT (dialogAudio, radiobutton_fpsPAL, "radiobutton_fpsPAL");
      GLADE_HOOKUP_OBJECT (dialogAudio, label4, "label4");
      GLADE_HOOKUP_OBJECT (dialogAudio, frame4, "frame4");
      GLADE_HOOKUP_OBJECT (dialogAudio, combobox1, "combobox1");
      GLADE_HOOKUP_OBJECT (dialogAudio, label6, "label6");
      GLADE_HOOKUP_OBJECT_NO_REF (dialogAudio, dialog_action_area1, "dialog_action_area1");
      GLADE_HOOKUP_OBJECT (dialogAudio, cancelbutton1, "cancelbutton1");
      GLADE_HOOKUP_OBJECT (dialogAudio, okbutton1, "okbutton1");

      return dialogAudio;
  }

