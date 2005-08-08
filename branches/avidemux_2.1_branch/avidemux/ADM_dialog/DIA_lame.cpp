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

#include "ADM_assert.h" 


#include "ADM_audiofilter/audioeng_buildfilters.h"
#include "ADM_audiofilter/audioeng_lame.h"


#define NB_BITRATE 11
static int BTR[] = {		// see interface.c if you change this !
		48,
		56,
		64,
		80,
    		96,
    		112,
    		128,
   		 160,
    		192,
   		 224,
		 384
};
  


 static GtkWidget	*create_dialog1 (void);
  
			 
int DIA_getLameSettings(int *pmode, int *pbitrate,ADM_LAME_PRESET *preset)
  {
  GtkWidget *dialog;
  gint result;
  int ret=0;
  char string[400];
  GtkWidget *fq[20];
  GtkWidget *Tpreset[10];

  	dialog=create_dialog1();
	gtk_transient(dialog);

	// Set mode
	int mindex;
	mindex=1;
	switch(*pmode)
	{
		case ADM_MONO:mindex=0;break;
		default:
		case ADM_STEREO:mindex=1;break;
		case ADM_JSTEREO:mindex=2;break;	
	}
	gtk_option_menu_set_history(GTK_OPTION_MENU(WID(optionmenu_mode)), mindex);
	// set bitrate
	for(unsigned int i=0;i<NB_BITRATE;i++)
	{
		sprintf(string,"%d kbits",BTR[i]);
		fq[i]=gtk_menu_item_new_with_mnemonic (string);
		gtk_widget_show (fq[i]);
		gtk_container_add (GTK_CONTAINER (WID(menu2)), fq[i]);
		if(*pbitrate==BTR[i])
			{
				// set 
				gtk_option_menu_set_history(GTK_OPTION_MENU(WID(optionmenu_bitrate)), i);
			}
	}
	for(int j=0;j<3;j++)
	{
		
		Tpreset[j]=gtk_menu_item_new_with_mnemonic (presetDefinition[j].name);
		gtk_widget_show (Tpreset[j]);
		gtk_container_add (GTK_CONTAINER (WID(menu3)), Tpreset[j]);
		if(*preset==presetDefinition[j].preset)
			{
				// set 
				gtk_option_menu_set_history(GTK_OPTION_MENU(WID(optionmenu_preset)), j);
			}
		
	
	}
	// Fill bitrate
	
	
	gtk_widget_show(dialog);
	
	if(GTK_RESPONSE_OK==gtk_dialog_run(GTK_DIALOG(dialog)))
	{
		// Grab result
		int index;
		
		index=getRangeInMenu(WID(optionmenu_bitrate));
		*pbitrate=BTR[index];
		index=getRangeInMenu(WID(optionmenu_preset));
		*preset=presetDefinition[index].preset;
		index=getRangeInMenu(WID(optionmenu_mode));
		switch(index)
		{
			case 0: *pmode=ADM_MONO;break;
			case 1: *pmode=ADM_STEREO;break;
			case 2: *pmode=ADM_JSTEREO;break;
			default:ADM_assert(0);
		}
		
		
	}
	
	gtk_widget_destroy(dialog);
	return ret;	    
  }
  
//___________ glade stuff below ____________

GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *frame1;
  GtkWidget *vbox2;
  GtkWidget *optionmenu_mode;
  GtkWidget *menu1;
  GtkWidget *mono1;
  GtkWidget *stereo1;
  GtkWidget *jointstereo1;
  GtkWidget *label2;
  GtkWidget *label1;
  GtkWidget *frame2;
  GtkWidget *vbox3;
  GtkWidget *optionmenu_bitrate;
  GtkWidget *menu2;
  //GtkWidget *dummy1;
  GtkWidget *optionmenu_preset;
  GtkWidget *menu3;
//   GtkWidget *dummy2;
  GtkWidget *label3;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), _("Lame settings"));

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox1), frame1, TRUE, TRUE, 0);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (frame1), vbox2);

  optionmenu_mode = gtk_option_menu_new ();
  gtk_widget_show (optionmenu_mode);
  gtk_box_pack_start (GTK_BOX (vbox2), optionmenu_mode, FALSE, FALSE, 0);

  menu1 = gtk_menu_new ();

  mono1 = gtk_menu_item_new_with_mnemonic (_("Mono"));
  gtk_widget_show (mono1);
  gtk_container_add (GTK_CONTAINER (menu1), mono1);

  stereo1 = gtk_menu_item_new_with_mnemonic (_("Stereo"));
  gtk_widget_show (stereo1);
  gtk_container_add (GTK_CONTAINER (menu1), stereo1);

  jointstereo1 = gtk_menu_item_new_with_mnemonic (_("JointStereo"));
  gtk_widget_show (jointstereo1);
  gtk_container_add (GTK_CONTAINER (menu1), jointstereo1);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_mode), menu1);

  label2 = gtk_label_new (_("This entry is ignored \nif the stream is mono."));
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (vbox2), label2, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);

  label1 = gtk_label_new (_("<b>Mode</b>"));
  gtk_widget_show (label1);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label1);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_show (frame2);
  gtk_box_pack_start (GTK_BOX (vbox1), frame2, TRUE, TRUE, 0);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox3);
  gtk_container_add (GTK_CONTAINER (frame2), vbox3);

  optionmenu_bitrate = gtk_option_menu_new ();
  gtk_widget_show (optionmenu_bitrate);
  gtk_box_pack_start (GTK_BOX (vbox3), optionmenu_bitrate, FALSE, FALSE, 0);

  menu2 = gtk_menu_new ();

//   dummy1 = gtk_menu_item_new_with_mnemonic (_("dummy1"));
//   gtk_widget_show (dummy1);
//   gtk_container_add (GTK_CONTAINER (menu2), dummy1);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_bitrate), menu2);

  optionmenu_preset = gtk_option_menu_new ();
  gtk_widget_show (optionmenu_preset);
  gtk_box_pack_start (GTK_BOX (vbox3), optionmenu_preset, FALSE, FALSE, 0);

  menu3 = gtk_menu_new ();

//   dummy2 = gtk_menu_item_new_with_mnemonic (_("dummy2"));
//   gtk_widget_show (dummy2);
//   gtk_container_add (GTK_CONTAINER (menu3), dummy2);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_preset), menu3);

  label3 = gtk_label_new (_("<b>Rate</b>"));
  gtk_widget_show (label3);
  gtk_frame_set_label_widget (GTK_FRAME (frame2), label3);
  gtk_label_set_use_markup (GTK_LABEL (label3), TRUE);
  gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_LEFT);

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
  GLADE_HOOKUP_OBJECT (dialog1, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (dialog1, optionmenu_mode, "optionmenu_mode");
  GLADE_HOOKUP_OBJECT (dialog1, menu1, "menu1");
  GLADE_HOOKUP_OBJECT (dialog1, mono1, "mono1");
  GLADE_HOOKUP_OBJECT (dialog1, stereo1, "stereo1");
  GLADE_HOOKUP_OBJECT (dialog1, jointstereo1, "jointstereo1");
  GLADE_HOOKUP_OBJECT (dialog1, label2, "label2");
  GLADE_HOOKUP_OBJECT (dialog1, label1, "label1");
  GLADE_HOOKUP_OBJECT (dialog1, frame2, "frame2");
  GLADE_HOOKUP_OBJECT (dialog1, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (dialog1, optionmenu_bitrate, "optionmenu_bitrate");
  GLADE_HOOKUP_OBJECT (dialog1, menu2, "menu2");
 // GLADE_HOOKUP_OBJECT (dialog1, dummy1, "dummy1");
  GLADE_HOOKUP_OBJECT (dialog1, optionmenu_preset, "optionmenu_preset");
  GLADE_HOOKUP_OBJECT (dialog1, menu3, "menu3");
  //GLADE_HOOKUP_OBJECT (dialog1, dummy2, "dummy2");
  GLADE_HOOKUP_OBJECT (dialog1, label3, "label3");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");

  return dialog1;
}

