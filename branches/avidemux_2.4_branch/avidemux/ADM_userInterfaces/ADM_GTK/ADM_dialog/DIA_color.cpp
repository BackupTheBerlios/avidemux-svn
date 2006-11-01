

/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#include "config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>


#include "default.h"


#include "ADM_toolkit_gtk/ADM_gladeSupport.h"
#include "ADM_toolkit_gtk/toolkit_gtk.h"
#include "ADM_toolkit_gtk/toolkit_gtk_include.h"
#include <ADM_assert.h>

 #define WID(x) lookup_widget(dialog,#x)

static GtkWidget	*create_colorselectiondialog1 (void);

int DIA_colorSel(uint8_t *r, uint8_t *g, uint8_t *b)
{
GtkWidget *dialog;
int ret=0;

GdkColor color;

	dialog=create_colorselectiondialog1();

	color.red=*r<<8;
	color.green=*g<<8;
	color.blue=*b<<8;

	gtk_color_selection_set_current_color    (GTK_COLOR_SELECTION(WID(color_selection1)),&color);


	if(GTK_RESPONSE_OK==gtk_dialog_run(GTK_DIALOG(dialog)))
	{

  		gtk_color_selection_get_current_color    (GTK_COLOR_SELECTION(WID(color_selection1)),&color);
		*r=color.red>>8;
		*g=color.green>>8;
		*b=color.blue>>8;
		ret=1;
	}

	gtk_widget_destroy(dialog);
	return ret;

}

GtkWidget*
create_colorselectiondialog1 (void)
{
  GtkWidget *colorselectiondialog1;
  GtkWidget *ok_button1;
  GtkWidget *cancel_button1;
  GtkWidget *help_button1;
  GtkWidget *color_selection1;

  colorselectiondialog1 = gtk_color_selection_dialog_new (_("Select Color"));
  gtk_window_set_resizable (GTK_WINDOW (colorselectiondialog1), FALSE);

  ok_button1 = GTK_COLOR_SELECTION_DIALOG (colorselectiondialog1)->ok_button;
  gtk_widget_show (ok_button1);
  GTK_WIDGET_SET_FLAGS (ok_button1, GTK_CAN_DEFAULT);

  cancel_button1 = GTK_COLOR_SELECTION_DIALOG (colorselectiondialog1)->cancel_button;
  gtk_widget_show (cancel_button1);
  GTK_WIDGET_SET_FLAGS (cancel_button1, GTK_CAN_DEFAULT);

  help_button1 = GTK_COLOR_SELECTION_DIALOG (colorselectiondialog1)->help_button;
  gtk_widget_show (help_button1);
  GTK_WIDGET_SET_FLAGS (help_button1, GTK_CAN_DEFAULT);

  color_selection1 = GTK_COLOR_SELECTION_DIALOG (colorselectiondialog1)->colorsel;
  gtk_widget_show (color_selection1);
  gtk_color_selection_set_has_opacity_control (GTK_COLOR_SELECTION (color_selection1), FALSE);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (colorselectiondialog1, colorselectiondialog1, "colorselectiondialog1");
  GLADE_HOOKUP_OBJECT_NO_REF (colorselectiondialog1, ok_button1, "ok_button1");
  GLADE_HOOKUP_OBJECT_NO_REF (colorselectiondialog1, cancel_button1, "cancel_button1");
  GLADE_HOOKUP_OBJECT_NO_REF (colorselectiondialog1, help_button1, "help_button1");
  GLADE_HOOKUP_OBJECT_NO_REF (colorselectiondialog1, color_selection1, "color_selection1");

  return colorselectiondialog1;
}

