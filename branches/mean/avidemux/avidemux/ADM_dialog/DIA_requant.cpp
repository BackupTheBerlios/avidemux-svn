//
/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>


#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "config.h"
#include "callbacks.h"
#include "avi_vars.h"
#include "ADM_gui2/support.h"
#include "ADM_toolkit/toolkit_gtk.h"
#include "ADM_toolkit/toolkit_gtk_include.h"
#include "ADM_toolkit/filesel.h"


# include "prefs.h"

static GtkWidget	*create_dialog1 (void);
static GtkWidget 	*dialog;
static uint64_t		_init;
static void callback( void );

uint8_t DIA_Requant(float *perce,uint32_t *quality,uint64_t init);
uint8_t DIA_Requant(float *perce,uint32_t *quality, uint64_t init)
{
uint8_t ret=0;
char string[1025];
	
	dialog=create_dialog1();
	gtk_transient(dialog);		
	sprintf(string,"Initial size : %lu MB",init>>20);
	_init=init;
  	gtk_label_set_text(GTK_LABEL(WID(labelInit)),string);
	gtk_signal_connect(GTK_OBJECT(WID(hscale1)),"value_changed",GTK_SIGNAL_FUNC(callback),0);
	



	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_OK)
	{
		GtkAdjustment *sliderAdjustment;
		ret=1;
		// Read value & speed
		sliderAdjustment=gtk_range_get_adjustment (GTK_RANGE(WID(hscale1)));
		*perce=(float)GTK_ADJUSTMENT(sliderAdjustment)->value;
		*quality=getRangeInMenu(WID(optionmenu1));
		if(*quality==2) *quality=3;
	}

	gtk_widget_destroy(dialog);
	dialog=NULL;
	return ret;
}
void callback( void )
{
float per;
uint64_t final;
char string[1024];

		GtkAdjustment *sliderAdjustment;		
		sliderAdjustment=gtk_range_get_adjustment (GTK_RANGE(WID(hscale1)));
		per=(float)GTK_ADJUSTMENT(sliderAdjustment)->value;
		if(per<1.0) per=1.0;
		final=(uint64_t)floor(_init/per);
		sprintf(string,"Final size : %lu MB",(uint32_t)(final>>20));		
  		gtk_label_set_text(GTK_LABEL(WID(labelEnd)),string);		
	
}
GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *label1;
  GtkWidget *hscale1;
  GtkWidget *optionmenu1;
  GtkWidget *menu1;
  GtkWidget *fast;
  GtkWidget *medium;
  GtkWidget *slow;
  GtkWidget *labelInit;
  GtkWidget *labelEnd;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), _("Requantize Option"));

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  label1 = gtk_label_new (_("Shrink factor"));
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox1), label1, FALSE, FALSE, 0);

  hscale1 = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (2, 1.0, 4.0, 0.2, 0.2, 0)));
  gtk_widget_show (hscale1);
  gtk_box_pack_start (GTK_BOX (vbox1), hscale1, FALSE, FALSE, 0);
  gtk_scale_set_digits (GTK_SCALE (hscale1), 2);

  optionmenu1 = gtk_option_menu_new ();
  gtk_widget_show (optionmenu1);
  gtk_box_pack_start (GTK_BOX (vbox1), optionmenu1, FALSE, FALSE, 0);

  menu1 = gtk_menu_new ();

  fast = gtk_menu_item_new_with_mnemonic (_("Fast / Low quality"));
  gtk_widget_show (fast);
  gtk_container_add (GTK_CONTAINER (menu1), fast);

  medium = gtk_menu_item_new_with_mnemonic (_("Medium/Medium"));
  gtk_widget_show (medium);
  gtk_container_add (GTK_CONTAINER (menu1), medium);

  slow = gtk_menu_item_new_with_mnemonic (_("Slow/High quality"));
  gtk_widget_show (slow);
  gtk_container_add (GTK_CONTAINER (menu1), slow);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu1), menu1);

  labelInit = gtk_label_new (_("Initial size : 0"));
  gtk_widget_show (labelInit);
  gtk_box_pack_start (GTK_BOX (vbox1), labelInit, FALSE, FALSE, 0);

  labelEnd = gtk_label_new (_("Final size: 0"));
  gtk_widget_show (labelEnd);
  gtk_box_pack_start (GTK_BOX (vbox1), labelEnd, FALSE, FALSE, 0);

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
  GLADE_HOOKUP_OBJECT (dialog1, hscale1, "hscale1");
  GLADE_HOOKUP_OBJECT (dialog1, optionmenu1, "optionmenu1");
  GLADE_HOOKUP_OBJECT (dialog1, menu1, "menu1");
  GLADE_HOOKUP_OBJECT (dialog1, fast, "fast");
  GLADE_HOOKUP_OBJECT (dialog1, medium, "medium");
  GLADE_HOOKUP_OBJECT (dialog1, slow, "slow");
  GLADE_HOOKUP_OBJECT (dialog1, labelInit, "labelInit");
  GLADE_HOOKUP_OBJECT (dialog1, labelEnd, "labelEnd");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");

  return dialog1;
}

