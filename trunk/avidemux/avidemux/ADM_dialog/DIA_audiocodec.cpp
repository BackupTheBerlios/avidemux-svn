/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ADM_assert.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "config.h"
#include "fourcc.h"
#include "ADM_audio/aviaudio.hxx"
#include "ADM_audiofilter/audioprocess.hxx"
#include "ADM_toolkit/toolkit_gtk.h"
#include "ADM_audiofilter/audioeng_buildfilters.h"
#include "ADM_gui2/support.h"


#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

static GtkWidget	*create_dialog1 (void);
  
#define NB_BITRATE (sizeof(BTR)/sizeof(int))

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
int DIA_audioEncoder(int *pmode, int *pbitrate,const char *title)
{
GtkWidget *dialog;
int ret=0;

	UNUSED_ARG(title);
	dialog=create_dialog1();
	gtk_transient(dialog);		
	for(uint32_t i=0;i<NB_BITRATE;i++)
		if(*pbitrate==BTR[i])
			{
				// set 
				gtk_option_menu_set_history(GTK_OPTION_MENU(lookup_widget(dialog,"optionmenu1")), i);
			}	
			
			
	switch(*pmode)
	{
		case ADM_STEREO: 
					gtk_option_menu_set_history(GTK_OPTION_MENU(lookup_widget(dialog,"optionmenu2")),0);		
					break;
		case ADM_JSTEREO: 
					gtk_option_menu_set_history(GTK_OPTION_MENU(lookup_widget(dialog,"optionmenu2")),1);		
					break;					
	}			
	if(GTK_RESPONSE_OK==gtk_dialog_run(GTK_DIALOG(dialog)))			
	{
					if(getRangeInMenu(lookup_widget(dialog,"optionmenu2"))==1)
							*pmode=ADM_JSTEREO;
					else
							*pmode=ADM_STEREO;
					*pbitrate=BTR[getRangeInMenu(lookup_widget(dialog,"optionmenu1"))];
					ret=1;
																						
	}
	
	gtk_widget_destroy(dialog);
	return ret;	    

}
  
GtkWidget	*create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *label1;
  GtkWidget *optionmenu1;
  GtkWidget *menu2;
 // GtkWidget *_48_khz1;
  GtkWidget *label2;
  GtkWidget *optionmenu2;
  GtkWidget *menu1;
  GtkWidget *stereo1;
  GtkWidget *joint_stereo1;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), _("Audio settings"));

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  label1 = gtk_label_new (_("Bitrate"));
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox1), label1, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);

  optionmenu1 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu1);
  gtk_box_pack_start (GTK_BOX (vbox1), optionmenu1, FALSE, FALSE, 0);

  menu2 = gtk_menu_new ();
/*
  _48_khz1 = gtk_menu_item_new_with_mnemonic (_("48 khz"));
  gtk_widget_show (_48_khz1);
  gtk_container_add (GTK_CONTAINER (menu2), _48_khz1);
  

*/
	GtkWidget **fq=(GtkWidget **)ADM_alloc(sizeof(GtkWidget *)*NB_BITRATE);
	char string[100];
	for(unsigned int i=0;i<NB_BITRATE;i++)
	{
		sprintf(string,"%d kbits",BTR[i]);
		fq[i]=gtk_menu_item_new_with_mnemonic (string);
		gtk_widget_show (fq[i]);
		gtk_container_add (GTK_CONTAINER (menu2), fq[i]);
	}
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu1), menu2);
  
  label2 = gtk_label_new (_("Mode (ignored for mono)"));
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (vbox1), label2, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);

  optionmenu2 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu2);
  gtk_box_pack_start (GTK_BOX (vbox1), optionmenu2, FALSE, FALSE, 0);

  menu1 = gtk_menu_new ();

  stereo1 = gtk_menu_item_new_with_mnemonic (_("Stereo"));
  gtk_widget_show (stereo1);
  gtk_container_add (GTK_CONTAINER (menu1), stereo1);

  joint_stereo1 = gtk_menu_item_new_with_mnemonic (_("Joint Stereo"));
  gtk_widget_show (joint_stereo1);
  gtk_container_add (GTK_CONTAINER (menu1), joint_stereo1);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu2), menu1);

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
  GLADE_HOOKUP_OBJECT (dialog1, label1, "label1");
  GLADE_HOOKUP_OBJECT (dialog1, optionmenu1, "optionmenu1");
  GLADE_HOOKUP_OBJECT (dialog1, menu2, "menu2");
  //GLADE_HOOKUP_OBJECT (dialog1, _48_khz1, "_48_khz1");
  GLADE_HOOKUP_OBJECT (dialog1, label2, "label2");
  GLADE_HOOKUP_OBJECT (dialog1, optionmenu2, "optionmenu2");
  GLADE_HOOKUP_OBJECT (dialog1, menu1, "menu1");
  GLADE_HOOKUP_OBJECT (dialog1, stereo1, "stereo1");
  GLADE_HOOKUP_OBJECT (dialog1, joint_stereo1, "joint_stereo1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");

  return dialog1;
}

