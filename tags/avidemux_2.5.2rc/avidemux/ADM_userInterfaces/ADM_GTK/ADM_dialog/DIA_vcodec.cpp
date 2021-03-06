/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */


#include "ADM_toolkitGtk.h"
//___________________________________
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_videoFilter.h"

#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_encoder/adm_encoder.h"

//___________________________________
 
extern int encoderGetEncoderCount(void);
extern const char* encoderGetIndexedName(uint32_t i);
extern int videoCodecGetIndex(void);

static GtkWidget *create_dialog1 (void);

//___________________________________________________________
uint8_t DIA_videoCodec(int *codecIndex)
{
#define CONFIGURE 99
	uint8_t ret = 0;
	uint32_t nb = encoderGetEncoderCount();
	GtkWidget *dialog = create_dialog1();

	gtk_register_dialog(dialog);

	for (int i = 0; i < nb; i++)
		gtk_combo_box_append_text(GTK_COMBO_BOX(WID(combobox1)), encoderGetIndexedName(i));

	gtk_combo_box_set_active(GTK_COMBO_BOX (WID(combobox1)), videoCodecGetIndex());
	gtk_dialog_add_action_widget(GTK_DIALOG(dialog), WID(buttonConf), CONFIGURE);

	int running = 1;

	while (running)
	{
		switch (gtk_dialog_run(GTK_DIALOG(dialog)))
		{
		case GTK_RESPONSE_OK:
			*codecIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(WID(combobox1)));

			ret = 1;
			running = 0;
			break;
		case CONFIGURE:
			videoCodecConfigureUI(gtk_combo_box_get_active(GTK_COMBO_BOX(WID(combobox1))));

			break;
		default:
			running = 0;
			break;
		}
	}

	gtk_widget_destroy(dialog);
	gtk_unregister_dialog(dialog);

	return ret;
} 

GtkWidget *create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *hbox1;
  GtkWidget *combobox1;
  GtkWidget *buttonConf;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), QT_TR_NOOP("Video encoder"));
  gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), hbox1, TRUE, TRUE, 0);

  combobox1 = gtk_combo_box_new_text ();
  gtk_widget_show (combobox1);
  gtk_box_pack_start (GTK_BOX (hbox1), combobox1, TRUE, TRUE, 0);

  buttonConf = gtk_button_new_with_mnemonic (QT_TR_NOOP("Configure"));
  gtk_widget_show (buttonConf);
  gtk_box_pack_start (GTK_BOX (hbox1), buttonConf, FALSE, FALSE, 0);

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
  GLADE_HOOKUP_OBJECT (dialog1, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (dialog1, combobox1, "combobox1");
  GLADE_HOOKUP_OBJECT (dialog1, buttonConf, "buttonConf");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");

  return dialog1;
}

