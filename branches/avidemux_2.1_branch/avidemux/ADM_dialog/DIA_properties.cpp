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
#include "config.h"
#include "ADM_library/default.h"

#include "ADM_gui2/support.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_toolkit/toolkit_gtk.h"
#include "ADM_toolkit/toolkit_gtk_include.h"


#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)


#define CHECK_SET(x,y) {gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(WID(x)),y);}	

extern const char *getStrFromAudioCodec( uint32_t codec);
static GtkWidget	*create_dialog (void);

void DIA_properties( void )
{

 char text[80];
 uint16_t hh, mm, ss, ms;
 GtkWidget *dialog;
 uint8_t gmc, qpel,vop;
 uint32_t info=0;
 
    if (playing)
	return;

    text[0] = 0;
    if (!avifileinfo)
	return;

	// Fetch info
	info=video_body->getSpecificMpeg4Info();
	vop=info & ADM_VOP_ON;
	qpel=info & ADM_QPEL_ON;
	gmc=info & ADM_GMC_ON;
	
	dialog = create_dialog();
//	gtk_transient(dialog);
        gtk_register_dialog(dialog);
#define FILL_ENTRY(x) gtk_label_set_text((GtkLabel *) lookup_widget(dialog,#x),text);
	sprintf(text, "%lu x %lu  ", avifileinfo->width,		avifileinfo->height);

	FILL_ENTRY(label_size);

	sprintf(text, "%2.3f fps", (float) avifileinfo->fps1000 / 1000.F);
	FILL_ENTRY(label_fps);
		
	sprintf(text, "%ld frames", avifileinfo->nb_frames);
	FILL_ENTRY(label_number);
		
      	sprintf(text, "%s ",      fourCC::tostring(avifileinfo->fcc));
	FILL_ENTRY(label_videofourcc);

	if (avifileinfo->nb_frames)
	  {
	      	frame2time(avifileinfo->nb_frames, avifileinfo->fps1000,
			 &hh, &mm, &ss, &ms);
	      	sprintf(text, "%02d:%02d:%02d.%03d", hh, mm, ss, ms);
	   	FILL_ENTRY(label_duration);	

	  }
	// Fill in vop, gmc & qpel
	if(vop) CHECK_SET(checkbutton_vop,1);
	if(gmc) CHECK_SET(checkbutton_gmc,1);
	if(qpel) CHECK_SET(checkbutton_qpel,1);
	// Now audio
	WAVHeader *wavinfo=NULL;
	if (currentaudiostream) wavinfo=currentaudiostream->getInfo();
	  if(wavinfo)
	  {
	      
	      switch (wavinfo->channels)
		{
		case 1:
		    sprintf(text, "MONO");
		    break;
		case 2:
		    sprintf(text, "STEREO");
		    break;
		default:
		    sprintf(text, "????");
		    break;
		}
	     	FILL_ENTRY(label1_audiomode);
	     
	      	sprintf(text, "%lu Hz", wavinfo->frequency);
	     	FILL_ENTRY(label_fq);
		sprintf(text, "%lu Bps / %lu kbps", wavinfo->byterate,      wavinfo->byterate * 8 / 1000);
		FILL_ENTRY(label_bitrate);
		sprintf(text, "%s ", getStrFromAudioCodec(wavinfo->encoding));
	     	FILL_ENTRY(label1_audiofourcc);
		// Duration in seconds too
		if(currentaudiostream && wavinfo->byterate>1)
		{
			double du;
			du=currentaudiostream->getLength();
			du*=1000;
			du/=wavinfo->byterate;
			ms2time((uint32_t)floor(du),
				 &hh, &mm, &ss, &ms);
	      		sprintf(text, "%02d:%02d:%02d.%03d (%lu MBytes)", hh, mm, ss, ms
				,currentaudiostream->getLength()>>20);
		}
		FILL_ENTRY(label_audioduration);
                if(currentaudiostream->isVBR() ) CHECK_SET(checkbutton_vbr,1);
	} else
	  {
	      sprintf(text, "NONE");
	      	FILL_ENTRY(label_fq);
	     	FILL_ENTRY(label1_audiomode);
	        FILL_ENTRY(label_bitrate);
	  	FILL_ENTRY(label1_audiofourcc);
	  }

	gtk_dialog_run(GTK_DIALOG(dialog));	
        gtk_unregister_dialog(dialog);
	gtk_widget_destroy(dialog);
		
}  


GtkWidget*
create_dialog (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *frame1;
  GtkWidget *table1;
  GtkWidget *label4;
  GtkWidget *label5;
  GtkWidget *label6;
  GtkWidget *label7;
  GtkWidget *label_fps;
  GtkWidget *label_size;
  GtkWidget *label3;
  GtkWidget *label_number;
  GtkWidget *label_videofourcc;
  GtkWidget *label_duration;
  GtkWidget *hseparator1;
  GtkWidget *hseparator2;
  GtkWidget *label18;
  GtkWidget *label19;
  GtkWidget *label20;
  GtkWidget *checkbutton_vop;
  GtkWidget *checkbutton_qpel;
  GtkWidget *checkbutton_gmc;
  GtkWidget *label1;
  GtkWidget *frame2;
  GtkWidget *table2;
  GtkWidget *label13;
  GtkWidget *label14;
  GtkWidget *label15;
  GtkWidget *label16;
  GtkWidget *label17;
  GtkWidget *label1_audiofourcc;
  GtkWidget *label1_audiomode;
  GtkWidget *label_bitrate;
  GtkWidget *label_fq;
  GtkWidget *label_audioduration;
  GtkWidget *label21;
  GtkWidget *checkbutton_vbr;
  GtkWidget *label2;
  GtkWidget *dialog_action_area1;
  GtkWidget *okbutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), _("Informations"));
  gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox1), frame1, TRUE, TRUE, 0);

  table1 = gtk_table_new (9, 2, FALSE);
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (frame1), table1);

  label4 = gtk_label_new (_("Frame Rate"));
  gtk_widget_show (label4);
  gtk_table_attach (GTK_TABLE (table1), label4, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);

  label5 = gtk_label_new (_("# of frames"));
  gtk_widget_show (label5);
  gtk_table_attach (GTK_TABLE (table1), label5, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label5), 0, 0.5);

  label6 = gtk_label_new (_("Codec 4CC"));
  gtk_widget_show (label6);
  gtk_table_attach (GTK_TABLE (table1), label6, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label6), 0, 0.5);

  label7 = gtk_label_new (_("Video Duration  "));
  gtk_widget_show (label7);
  gtk_table_attach (GTK_TABLE (table1), label7, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label7), 0, 0.5);

  label_fps = gtk_label_new ("");
  gtk_widget_show (label_fps);
  gtk_table_attach (GTK_TABLE (table1), label_fps, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_fps), 0, 0.5);

  label_size = gtk_label_new (_(" "));
  gtk_widget_show (label_size);
  gtk_table_attach (GTK_TABLE (table1), label_size, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_size), 0, 0.5);

  label3 = gtk_label_new (_("Image size"));
  gtk_widget_show (label3);
  gtk_table_attach (GTK_TABLE (table1), label3, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label3), 0, 0.5);

  label_number = gtk_label_new ("");
  gtk_widget_show (label_number);
  gtk_table_attach (GTK_TABLE (table1), label_number, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_number), 0, 0.5);

  label_videofourcc = gtk_label_new ("");
  gtk_widget_show (label_videofourcc);
  gtk_table_attach (GTK_TABLE (table1), label_videofourcc, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_videofourcc), 0, 0.5);

  label_duration = gtk_label_new (_(" "));
  gtk_widget_show (label_duration);
  gtk_table_attach (GTK_TABLE (table1), label_duration, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_duration), 0, 0.5);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_show (hseparator1);
  gtk_table_attach (GTK_TABLE (table1), hseparator1, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  hseparator2 = gtk_hseparator_new ();
  gtk_widget_show (hseparator2);
  gtk_table_attach (GTK_TABLE (table1), hseparator2, 1, 2, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);

  label18 = gtk_label_new (_("Vop packed"));
  gtk_widget_show (label18);
  gtk_table_attach (GTK_TABLE (table1), label18, 0, 1, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label18), 0, 0.5);

  label19 = gtk_label_new (_("Quarter Pixel"));
  gtk_widget_show (label19);
  gtk_table_attach (GTK_TABLE (table1), label19, 0, 1, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label19), 0, 0.5);

  label20 = gtk_label_new (_("GMC"));
  gtk_widget_show (label20);
  gtk_table_attach (GTK_TABLE (table1), label20, 0, 1, 8, 9,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label20), 0, 0.5);

  checkbutton_vop = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (checkbutton_vop);
  gtk_table_attach (GTK_TABLE (table1), checkbutton_vop, 1, 2, 6, 7,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton_qpel = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (checkbutton_qpel);
  gtk_table_attach (GTK_TABLE (table1), checkbutton_qpel, 1, 2, 7, 8,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbutton_gmc = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (checkbutton_gmc);
  gtk_table_attach (GTK_TABLE (table1), checkbutton_gmc, 1, 2, 8, 9,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label1 = gtk_label_new (_("<b>Video</b>"));
  gtk_widget_show (label1);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label1);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_show (frame2);
  gtk_box_pack_start (GTK_BOX (vbox1), frame2, TRUE, TRUE, 0);

  table2 = gtk_table_new (6, 2, FALSE);
  gtk_widget_show (table2);
  gtk_container_add (GTK_CONTAINER (frame2), table2);

  label13 = gtk_label_new (_("Codec "));
  gtk_widget_show (label13);
  gtk_table_attach (GTK_TABLE (table2), label13, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label13), 0, 0.5);

  label14 = gtk_label_new (_("Channels  "));
  gtk_widget_show (label14);
  gtk_table_attach (GTK_TABLE (table2), label14, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label14), 0, 0.5);

  label15 = gtk_label_new (_("Bitrate "));
  gtk_widget_show (label15);
  gtk_table_attach (GTK_TABLE (table2), label15, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label15), 0, 0.5);

  label16 = gtk_label_new (_("Frequency"));
  gtk_widget_show (label16);
  gtk_table_attach (GTK_TABLE (table2), label16, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label16), 0, 0.5);

  label17 = gtk_label_new (_("Audio duration  "));
  gtk_widget_show (label17);
  gtk_table_attach (GTK_TABLE (table2), label17, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label17), 0, 0.5);

  label1_audiofourcc = gtk_label_new ("");
  gtk_widget_show (label1_audiofourcc);
  gtk_table_attach (GTK_TABLE (table2), label1_audiofourcc, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label1_audiofourcc), 0, 0.5);

  label1_audiomode = gtk_label_new ("");
  gtk_widget_show (label1_audiomode);
  gtk_table_attach (GTK_TABLE (table2), label1_audiomode, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label1_audiomode), 0, 0.5);

  label_bitrate = gtk_label_new ("");
  gtk_widget_show (label_bitrate);
  gtk_table_attach (GTK_TABLE (table2), label_bitrate, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_bitrate), 0, 0.5);

  label_fq = gtk_label_new ("");
  gtk_widget_show (label_fq);
  gtk_table_attach (GTK_TABLE (table2), label_fq, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_fq), 0, 0.5);

  label_audioduration = gtk_label_new ("");
  gtk_widget_show (label_audioduration);
  gtk_table_attach (GTK_TABLE (table2), label_audioduration, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label_audioduration), 0, 0.5);

  label21 = gtk_label_new (_("VBR"));
  gtk_widget_show (label21);
  gtk_table_attach (GTK_TABLE (table2), label21, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label21), 0, 0.5);

  checkbutton_vbr = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (checkbutton_vbr);
  gtk_table_attach (GTK_TABLE (table2), checkbutton_vbr, 1, 2, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label2 = gtk_label_new (_("<b>Audio</b>"));
  gtk_widget_show (label2);
  gtk_frame_set_label_widget (GTK_FRAME (frame2), label2);
  gtk_label_set_use_markup (GTK_LABEL (label2), TRUE);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog1, "dialog1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (dialog1, table1, "table1");
  GLADE_HOOKUP_OBJECT (dialog1, label4, "label4");
  GLADE_HOOKUP_OBJECT (dialog1, label5, "label5");
  GLADE_HOOKUP_OBJECT (dialog1, label6, "label6");
  GLADE_HOOKUP_OBJECT (dialog1, label7, "label7");
  GLADE_HOOKUP_OBJECT (dialog1, label_fps, "label_fps");
  GLADE_HOOKUP_OBJECT (dialog1, label_size, "label_size");
  GLADE_HOOKUP_OBJECT (dialog1, label3, "label3");
  GLADE_HOOKUP_OBJECT (dialog1, label_number, "label_number");
  GLADE_HOOKUP_OBJECT (dialog1, label_videofourcc, "label_videofourcc");
  GLADE_HOOKUP_OBJECT (dialog1, label_duration, "label_duration");
  GLADE_HOOKUP_OBJECT (dialog1, hseparator1, "hseparator1");
  GLADE_HOOKUP_OBJECT (dialog1, hseparator2, "hseparator2");
  GLADE_HOOKUP_OBJECT (dialog1, label18, "label18");
  GLADE_HOOKUP_OBJECT (dialog1, label19, "label19");
  GLADE_HOOKUP_OBJECT (dialog1, label20, "label20");
  GLADE_HOOKUP_OBJECT (dialog1, checkbutton_vop, "checkbutton_vop");
  GLADE_HOOKUP_OBJECT (dialog1, checkbutton_qpel, "checkbutton_qpel");
  GLADE_HOOKUP_OBJECT (dialog1, checkbutton_gmc, "checkbutton_gmc");
  GLADE_HOOKUP_OBJECT (dialog1, label1, "label1");
  GLADE_HOOKUP_OBJECT (dialog1, frame2, "frame2");
  GLADE_HOOKUP_OBJECT (dialog1, table2, "table2");
  GLADE_HOOKUP_OBJECT (dialog1, label13, "label13");
  GLADE_HOOKUP_OBJECT (dialog1, label14, "label14");
  GLADE_HOOKUP_OBJECT (dialog1, label15, "label15");
  GLADE_HOOKUP_OBJECT (dialog1, label16, "label16");
  GLADE_HOOKUP_OBJECT (dialog1, label17, "label17");
  GLADE_HOOKUP_OBJECT (dialog1, label1_audiofourcc, "label1_audiofourcc");
  GLADE_HOOKUP_OBJECT (dialog1, label1_audiomode, "label1_audiomode");
  GLADE_HOOKUP_OBJECT (dialog1, label_bitrate, "label_bitrate");
  GLADE_HOOKUP_OBJECT (dialog1, label_fq, "label_fq");
  GLADE_HOOKUP_OBJECT (dialog1, label_audioduration, "label_audioduration");
  GLADE_HOOKUP_OBJECT (dialog1, label21, "label21");
  GLADE_HOOKUP_OBJECT (dialog1, checkbutton_vbr, "checkbutton_vbr");
  GLADE_HOOKUP_OBJECT (dialog1, label2, "label2");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");

  gtk_widget_grab_focus (okbutton1);
  gtk_widget_grab_default (okbutton1);
  return dialog1;
}

