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

#include "ADM_library/default.h"
#include "ADM_gui2/support.h"
#include "ADM_toolkit/toolkit_gtk.h"
#include "ADM_toolkit/toolkit_gtk_include.h"
#include "ADM_toolkit/filesel.h"

# include "prefs.h"
#include "ADM_audiodevice/audio_out.h"

#include "ADM_assert.h"
#include "ADM_gui2/GUI_render.h"
#include "ADM_toolkit/ADM_cpuCap.h"
static GtkWidget	*create_dialog1 (void);
static void setpp(void);

static GtkWidget *dialog=NULL;

extern void 		AVDM_audioPref( void );

typedef struct stVideoDevice
{
        ADM_RENDER_TYPE type;
        char            *name;

}stVideoDevice;

static stVideoDevice myVideoDevice[]
={
        {RENDER_GTK,"Gtk (no accel)"},
#ifdef USE_XV
        {RENDER_XV,"Xvideo accel (best)"},
#endif
#ifdef USE_SDL
        {RENDER_SDL,"SDL accel"},
#endif

};

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
GtkWidget *videowids[10];
uint32_t k;
unsigned int renderI;
ADM_RENDER_TYPE render;
	
	dialog=create_dialog1();
//	gtk_transient(dialog);
        gtk_register_dialog(dialog);
//****************************	
#define SET_CPU(x,y) gtk_widget_set_sensitive(WID(check##x),0); \
        if(CpuCaps::has##y()) gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(WID(check##x)),1);

        SET_CPU(MMX,MMX);
        SET_CPU(MMXEXT,MMXEXT);
        SET_CPU(3DNOW,3DNOW);
        SET_CPU(SSE,SSE);
        SET_CPU(SSE2,SSE2);
        gtk_widget_set_sensitive( (WID(checkAltivec)),0);
//****************************
	olddevice=newdevice=AVDM_getCurrentDevice();
	
	#define CONNECT(A,B)  gtk_signal_connect (GTK_OBJECT(lookup_widget(dialog,#A)), "clicked", \
		      GTK_SIGNAL_FUNC (B), (void *) NULL);

	
#define SPIN_GET(x,y) {x= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(WID(y))) ;}
#define SPIN_SET(x,y)  {gtk_spin_button_set_value(GTK_SPIN_BUTTON(WID(y)),(gfloat)x) ;}

	// Alsa
        if( prefs->get(DEVICE_AUDIO_ALSA_DEVICE, &str) != RC_OK )
               str = ADM_strdup("plughw:0,0");
        gtk_write_entry_string(WID(entryAlsa), str);
        ADM_dealloc(str);

	// VCD/SVCD split point		
	if(!prefs->get(SETTINGS_MPEGSPLIT, &autosplit))
	{
		autosplit=690;		
	}
	// Fill entry
         SPIN_SET(autosplit,spinbuttonMpegSplit);
        		
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
        gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(WID(checkbuttonOpenDML)),
                                      use_odml);
        // Video accel device ______________________________________________
        int vd=0;
        if(prefs->get(DEVICE_VIDEODEVICE,&renderI)!=RC_OK)
        {       
                render=RENDER_GTK;
        }else
        {
                render=(ADM_RENDER_TYPE)renderI;
        }
        for(uint32_t i=0;i<sizeof(myVideoDevice)/sizeof(stVideoDevice);i++)
        {
                if(myVideoDevice[i].type==render)
                        {
                                vd=i;
                        }
                videowids[i] = gtk_menu_item_new_with_mnemonic ( myVideoDevice[i].name);
                gtk_widget_show (videowids[i]);
                gtk_container_add (GTK_CONTAINER (WID(menu3)), videowids[i]);
        }               
         gtk_option_menu_set_menu (GTK_OPTION_MENU (WID(optionmenuVideo)), WID(menu3));
         gtk_option_menu_set_history(GTK_OPTION_MENU(WID(optionmenuVideo)), vd);
                        

	// Audio accel device ______________________________________________
	for(uint32_t i=0;i<sizeof(audioDeviceList)/sizeof(DEVICELIST);i++)
	{
		if(audioDeviceList[i].id==olddevice)
			{
				k=i;
			}
		wids[i] = gtk_menu_item_new_with_mnemonic ( audioDeviceList[i].name);
		gtk_widget_show (wids[i]);
		gtk_container_add (GTK_CONTAINER (WID(menu2)), wids[i]);
	}		
	 gtk_option_menu_set_menu (GTK_OPTION_MENU (WID(optionmenuAudio)), WID(menu2));
	 gtk_option_menu_set_history(GTK_OPTION_MENU(WID(optionmenuAudio)), k);
        // Callback for button
        gtk_signal_connect(GTK_OBJECT(WID(buttonPP)), "clicked",GTK_SIGNAL_FUNC(setpp),   NULL);
	 // run

	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_OK)
	{
		ret=1;
		// Get device
		k=getRangeInMenu(WID(optionmenuAudio));
		newdevice=audioDeviceList[k].id;
		if(newdevice!=olddevice)
		{
			AVDM_switch(newdevice);
		}
                // video device

                k=getRangeInMenu(WID(optionmenuVideo));
                render=myVideoDevice[k].type;
                renderI=(int)render;
                prefs->set(DEVICE_VIDEODEVICE,renderI);
                
                ///*********
		lavcodec_mpeg=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(WID(checkbutton_lavcodec)));
		prefs->set(FEATURE_USE_LAVCODEC_MPEG, lavcodec_mpeg);
                
                use_odml=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(WID(checkbuttonOpenDML)));
                prefs->set(FEATURE_USE_ODML, use_odml);
                
                SPIN_GET(autosplit,spinbuttonMpegSplit);
		prefs->set(SETTINGS_MPEGSPLIT, autosplit);
                
                //alsa device
                str=gtk_editable_get_chars(GTK_EDITABLE (WID(entryAlsa)), 0, -1);
                if(str)
                        prefs->set(DEVICE_AUDIO_ALSA_DEVICE, str);
		
	}
        gtk_unregister_dialog(dialog);
	gtk_widget_destroy(dialog);
	dialog=NULL;
	return ret;
}
extern int DIA_getMPParams( uint32_t *pplevel, uint32_t *ppstrength,uint32_t *swap);
void setpp(void)
{
        uint32_t type,strength,uv=0;

        if(!prefs->get(DEFAULT_POSTPROC_TYPE,&type)) type=3;
        if(!prefs->get(DEFAULT_POSTPROC_VALUE,&strength)) strength=3;
        if( DIA_getMPParams( &type,&strength,&uv))
        {
                prefs->set(DEFAULT_POSTPROC_TYPE,type);
                prefs->set(DEFAULT_POSTPROC_VALUE,strength);

        }
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
  GtkWidget *frame3;
  GtkWidget *table2;
  GtkWidget *label21;
  GtkWidget *label22;
  GtkWidget *label12;
  GtkWidget *checkbutton_lavcodec;
  GtkWidget *checkbuttonOpenDML;
  GtkObject *spinbuttonMpegSplit_adj;
  GtkWidget *spinbuttonMpegSplit;
  GtkWidget *label20;
  GtkWidget *frame4;
  GtkWidget *table3;
  GtkWidget *label24;
  GtkWidget *optionmenuVideo;
  GtkWidget *menu3;
  GtkWidget *label39;
  GtkWidget *buttonPP;
  GtkWidget *label23;
  GtkWidget *frame6;
  GtkWidget *table4;
  GtkWidget *label29;
  GtkWidget *label30;
  GtkWidget *entryAlsa;
  GtkWidget *optionmenuAudio;
  GtkWidget *menu2;
  GtkWidget *label28;
  GtkWidget *frame8;
  GtkWidget *table6;
  GtkWidget *label33;
  GtkWidget *label36;
  GtkWidget *checkMMX;
  GtkWidget *checkMMXEXT;
  GtkWidget *check3DNOW;
  GtkWidget *checkSSE;
  GtkWidget *checkSSE2;
  GtkWidget *checkAltivec;
  GtkWidget *label32;
  GtkWidget *label35;
  GtkWidget *label37;
  GtkWidget *label34;
  GtkWidget *label38;
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

  frame3 = gtk_frame_new (NULL);
  gtk_widget_show (frame3);
  gtk_box_pack_start (GTK_BOX (vbox1), frame3, FALSE, FALSE, 0);

  table2 = gtk_table_new (3, 2, FALSE);
  gtk_widget_show (table2);
  gtk_container_add (GTK_CONTAINER (frame3), table2);

  label21 = gtk_label_new (_("Use lavcodec mpg 1/2 decoder "));
  gtk_widget_show (label21);
  gtk_table_attach (GTK_TABLE (table2), label21, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label21), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label21), 0, 0.5);

  label22 = gtk_label_new (_("Use openDML rather\n      than split for big files "));
  gtk_widget_show (label22);
  gtk_table_attach (GTK_TABLE (table2), label22, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label22), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label22), 0, 0.5);

  label12 = gtk_label_new (_("Mpeg auto split (MB)"));
  gtk_widget_show (label12);
  gtk_table_attach (GTK_TABLE (table2), label12, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label12), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label12), 0, 0.5);

  checkbutton_lavcodec = gtk_check_button_new_with_mnemonic (_(" "));
  gtk_widget_show (checkbutton_lavcodec);
  gtk_table_attach (GTK_TABLE (table2), checkbutton_lavcodec, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkbuttonOpenDML = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (checkbuttonOpenDML);
  gtk_table_attach (GTK_TABLE (table2), checkbuttonOpenDML, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  spinbuttonMpegSplit_adj = gtk_adjustment_new (690, 10, 5000, 10, 100, 100);
  spinbuttonMpegSplit = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonMpegSplit_adj), 1, 0);
  gtk_widget_show (spinbuttonMpegSplit);
  gtk_table_attach (GTK_TABLE (table2), spinbuttonMpegSplit, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbuttonMpegSplit), TRUE);

  label20 = gtk_label_new (_("<b>Option</b>"));
  gtk_widget_show (label20);
  gtk_frame_set_label_widget (GTK_FRAME (frame3), label20);
  gtk_label_set_use_markup (GTK_LABEL (label20), TRUE);
  gtk_label_set_justify (GTK_LABEL (label20), GTK_JUSTIFY_LEFT);

  frame4 = gtk_frame_new (NULL);
  gtk_widget_show (frame4);
  gtk_box_pack_start (GTK_BOX (vbox1), frame4, FALSE, FALSE, 0);

  table3 = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (table3);
  gtk_container_add (GTK_CONTAINER (frame4), table3);

  label24 = gtk_label_new (_("Use accelerated video "));
  gtk_widget_show (label24);
  gtk_table_attach (GTK_TABLE (table3), label24, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label24), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label24), 0, 0.5);

  optionmenuVideo = gtk_option_menu_new ();
  gtk_widget_show (optionmenuVideo);
  gtk_table_attach (GTK_TABLE (table3), optionmenuVideo, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  menu3 = gtk_menu_new ();

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenuVideo), menu3);

  label39 = gtk_label_new (_("Set default Post Proc"));
  gtk_widget_show (label39);
  gtk_table_attach (GTK_TABLE (table3), label39, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label39), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label39), 0, 0.5);

  buttonPP = gtk_button_new_with_mnemonic (_("Change"));
  gtk_widget_show (buttonPP);
  gtk_table_attach (GTK_TABLE (table3), buttonPP, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label23 = gtk_label_new (_("<b>Video</b>"));
  gtk_widget_show (label23);
  gtk_frame_set_label_widget (GTK_FRAME (frame4), label23);
  gtk_label_set_use_markup (GTK_LABEL (label23), TRUE);
  gtk_label_set_justify (GTK_LABEL (label23), GTK_JUSTIFY_LEFT);

  frame6 = gtk_frame_new (NULL);
  gtk_widget_show (frame6);
  gtk_box_pack_start (GTK_BOX (vbox1), frame6, FALSE, TRUE, 0);

  table4 = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (table4);
  gtk_container_add (GTK_CONTAINER (frame6), table4);

  label29 = gtk_label_new (_("Audio device"));
  gtk_widget_show (label29);
  gtk_table_attach (GTK_TABLE (table4), label29, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label29), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label29), 0, 0.5);

  label30 = gtk_label_new (_("Alsa Device :"));
  gtk_widget_show (label30);
  gtk_table_attach (GTK_TABLE (table4), label30, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label30), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label30), 0, 0.5);

  entryAlsa = gtk_entry_new ();
  gtk_widget_show (entryAlsa);
  gtk_table_attach (GTK_TABLE (table4), entryAlsa, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_entry_set_max_length (GTK_ENTRY (entryAlsa), 12);

  optionmenuAudio = gtk_option_menu_new ();
  gtk_widget_show (optionmenuAudio);
  gtk_table_attach (GTK_TABLE (table4), optionmenuAudio, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  menu2 = gtk_menu_new ();

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenuAudio), menu2);

  label28 = gtk_label_new (_("<b>Audio</b>"));
  gtk_widget_show (label28);
  gtk_frame_set_label_widget (GTK_FRAME (frame6), label28);
  gtk_label_set_use_markup (GTK_LABEL (label28), TRUE);
  gtk_label_set_justify (GTK_LABEL (label28), GTK_JUSTIFY_LEFT);

  frame8 = gtk_frame_new (NULL);
  gtk_widget_show (frame8);
  gtk_box_pack_start (GTK_BOX (vbox1), frame8, TRUE, TRUE, 0);

  table6 = gtk_table_new (3, 4, FALSE);
  gtk_widget_show (table6);
  gtk_container_add (GTK_CONTAINER (frame8), table6);

  label33 = gtk_label_new (_("MMXEXT"));
  gtk_widget_show (label33);
  gtk_table_attach (GTK_TABLE (table6), label33, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label33), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label33), 0, 0.5);

  label36 = gtk_label_new (_("SSE2"));
  gtk_widget_show (label36);
  gtk_table_attach (GTK_TABLE (table6), label36, 2, 3, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label36), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label36), 0, 0.5);

  checkMMX = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (checkMMX);
  gtk_table_attach (GTK_TABLE (table6), checkMMX, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkMMXEXT = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (checkMMXEXT);
  gtk_table_attach (GTK_TABLE (table6), checkMMXEXT, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  check3DNOW = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (check3DNOW);
  gtk_table_attach (GTK_TABLE (table6), check3DNOW, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkSSE = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (checkSSE);
  gtk_table_attach (GTK_TABLE (table6), checkSSE, 3, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkSSE2 = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (checkSSE2);
  gtk_table_attach (GTK_TABLE (table6), checkSSE2, 3, 4, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  checkAltivec = gtk_check_button_new_with_mnemonic ("");
  gtk_widget_show (checkAltivec);
  gtk_table_attach (GTK_TABLE (table6), checkAltivec, 3, 4, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label32 = gtk_label_new (_("MMX"));
  gtk_widget_show (label32);
  gtk_table_attach (GTK_TABLE (table6), label32, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label32), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label32), 0, 0.5);

  label35 = gtk_label_new (_("SSE"));
  gtk_widget_show (label35);
  gtk_table_attach (GTK_TABLE (table6), label35, 2, 3, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label35), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label35), 0, 0.5);

  label37 = gtk_label_new (_("Altivec"));
  gtk_widget_show (label37);
  gtk_table_attach (GTK_TABLE (table6), label37, 2, 3, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label37), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label37), 0, 0.5);

  label34 = gtk_label_new (_("3DNOW"));
  gtk_widget_show (label34);
  gtk_table_attach (GTK_TABLE (table6), label34, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label34), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label34), 0, 0.5);

  label38 = gtk_label_new (_("<b>Cpu</b>"));
  gtk_widget_show (label38);
  gtk_frame_set_label_widget (GTK_FRAME (frame8), label38);
  gtk_label_set_use_markup (GTK_LABEL (label38), TRUE);
  gtk_label_set_justify (GTK_LABEL (label38), GTK_JUSTIFY_LEFT);

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
  GLADE_HOOKUP_OBJECT (dialog, frame3, "frame3");
  GLADE_HOOKUP_OBJECT (dialog, table2, "table2");
  GLADE_HOOKUP_OBJECT (dialog, label21, "label21");
  GLADE_HOOKUP_OBJECT (dialog, label22, "label22");
  GLADE_HOOKUP_OBJECT (dialog, label12, "label12");
  GLADE_HOOKUP_OBJECT (dialog, checkbutton_lavcodec, "checkbutton_lavcodec");
  GLADE_HOOKUP_OBJECT (dialog, checkbuttonOpenDML, "checkbuttonOpenDML");
  GLADE_HOOKUP_OBJECT (dialog, spinbuttonMpegSplit, "spinbuttonMpegSplit");
  GLADE_HOOKUP_OBJECT (dialog, label20, "label20");
  GLADE_HOOKUP_OBJECT (dialog, frame4, "frame4");
  GLADE_HOOKUP_OBJECT (dialog, table3, "table3");
  GLADE_HOOKUP_OBJECT (dialog, label24, "label24");
  GLADE_HOOKUP_OBJECT (dialog, optionmenuVideo, "optionmenuVideo");
  GLADE_HOOKUP_OBJECT (dialog, menu3, "menu3");
  GLADE_HOOKUP_OBJECT (dialog, label39, "label39");
  GLADE_HOOKUP_OBJECT (dialog, buttonPP, "buttonPP");
  GLADE_HOOKUP_OBJECT (dialog, label23, "label23");
  GLADE_HOOKUP_OBJECT (dialog, frame6, "frame6");
  GLADE_HOOKUP_OBJECT (dialog, table4, "table4");
  GLADE_HOOKUP_OBJECT (dialog, label29, "label29");
  GLADE_HOOKUP_OBJECT (dialog, label30, "label30");
  GLADE_HOOKUP_OBJECT (dialog, entryAlsa, "entryAlsa");
  GLADE_HOOKUP_OBJECT (dialog, optionmenuAudio, "optionmenuAudio");
  GLADE_HOOKUP_OBJECT (dialog, menu2, "menu2");
  GLADE_HOOKUP_OBJECT (dialog, label28, "label28");
  GLADE_HOOKUP_OBJECT (dialog, frame8, "frame8");
  GLADE_HOOKUP_OBJECT (dialog, table6, "table6");
  GLADE_HOOKUP_OBJECT (dialog, label33, "label33");
  GLADE_HOOKUP_OBJECT (dialog, label36, "label36");
  GLADE_HOOKUP_OBJECT (dialog, checkMMX, "checkMMX");
  GLADE_HOOKUP_OBJECT (dialog, checkMMXEXT, "checkMMXEXT");
  GLADE_HOOKUP_OBJECT (dialog, check3DNOW, "check3DNOW");
  GLADE_HOOKUP_OBJECT (dialog, checkSSE, "checkSSE");
  GLADE_HOOKUP_OBJECT (dialog, checkSSE2, "checkSSE2");
  GLADE_HOOKUP_OBJECT (dialog, checkAltivec, "checkAltivec");
  GLADE_HOOKUP_OBJECT (dialog, label32, "label32");
  GLADE_HOOKUP_OBJECT (dialog, label35, "label35");
  GLADE_HOOKUP_OBJECT (dialog, label37, "label37");
  GLADE_HOOKUP_OBJECT (dialog, label34, "label34");
  GLADE_HOOKUP_OBJECT (dialog, label38, "label38");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialog, okbutton1, "okbutton1");

  return dialog;
}

