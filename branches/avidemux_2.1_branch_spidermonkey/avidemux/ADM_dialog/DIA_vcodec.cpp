/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#include "config.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "ADM_toolkit/toolkit_gtk_include.h"
#include "ADM_toolkit/toolkit_gtk.h"

#include "ADM_assert.h" 

#include "ADM_library/default.h"

//___________________________________
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"

#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_encoder/adm_encoder.h"


//___________________________________

 static GtkWidget	*create_dialogVideoCodec (void) ;
 static SelectCodecType findCodec( void );
 static GtkWidget *dialog;
 static void okCallback(GtkButton * button, gpointer user_data);
 static void callBackCalc( void );
extern void DIA_Calculator(uint32_t *sizeInMeg, uint32_t *avgBitrate );
 typedef struct myVideoCodec
 {
 		SelectCodecType codec;
		char		*name;
 }myVideoCodec;
 static myVideoCodec myTab[]=
 {
 	{CodecFF,"Lavcodec Mpeg4"},
	{CodecH263,"Lavcodec H263"},
	{CodecH263P,"Lavcodec H263+"},
	{CodecHuff,"Huffyuv"},
        {CodecFFhuff,"FF Huffyuv"},
	{CodecFFV1,"FFV1"},
	{CodecSnow,"Snow"},
#ifdef USE_XVID_4	
 	{CodecXvid4,"Xvid4"},
#endif	
#ifdef USE_XX_XVID	
 	{CodecXvid,"Xvid"},
#endif
#ifdef USE_DIVX
	{CodecDivx,"Divx"},
#endif

	{CodecMjpeg,"Mjpeg"},
#ifdef USE_MJPEG
	{CodecVCD,"VCD"},
	{CodecSVCD,"SVCD"},
	{CodecDVD,"DVD"},
#endif
	{CodecXVCD,"XVCD (lav)"},
	{CodecXSVCD,"XSVCD (lav)"},
	{CodecXDVD,"DVD (lavcodec)"}
	
 };
 void okCallback(GtkButton * button, gpointer user_data)
{
	SelectCodecType cur;

		UNUSED_ARG(button);
		UNUSED_ARG(user_data);
	//	printf("Callback called\n");
		cur=findCodec();
		videoCodecSetcodec(cur);
		videoCodecConfigureUI();

}

//___________________________________________________________
void callBackCalc( void )
{
uint32_t sz,br;
	DIA_Calculator(&sz,&br);
}
//___________________________________________________________
uint8_t DIA_videoCodec( SelectCodecType *codec )
{

	uint8_t ret=0;
	SelectCodecType old=*codec;
	GtkWidget *widget[sizeof(myTab)/sizeof(myVideoCodec)]; 
	dialog=create_dialogVideoCodec();
	//gtk_transient(dialog);	
	gtk_register_dialog(dialog);

	
	// now set the input one
	for(uint32_t i=0;i<sizeof(myTab)/sizeof(myVideoCodec);i++)
	{
		widget[i]=gtk_menu_item_new_with_mnemonic (myTab[i].name);
  		gtk_widget_show (widget[i]);
  		gtk_container_add (GTK_CONTAINER (WID(menu1)), widget[i]);
		if(*codec==myTab[i].codec)
			{
				// set
				gtk_option_menu_set_history(GTK_OPTION_MENU(WID(optionmenu_CodecList)), i);
			}
	}
	gtk_signal_connect(GTK_OBJECT(WID(buttonConfigure)), "clicked",
		       GTK_SIGNAL_FUNC(okCallback), (void *) 0);
	gtk_signal_connect(GTK_OBJECT(WID(buttonCalc)), "clicked",
		       GTK_SIGNAL_FUNC(callBackCalc), (void *) 0);

	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_OK)
	{
		
			*codec=findCodec();
			ret=1;
	
	}
	else
	{
		*codec=old;
	}
	gtk_widget_destroy(dialog);
	gtk_unregister_dialog(dialog);
	return ret;
} 

SelectCodecType findCodec( void )
{
uint8_t j;
			j=getRangeInMenu(WID(optionmenu_CodecList));

			if(j>=sizeof(myTab)/sizeof(myVideoCodec)) ADM_assert(0);
			return myTab[j].codec;
			

}


GtkWidget*
create_dialogVideoCodec (void)
{
  GtkWidget *dialogVideoCodec;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *hbox2;
  GtkWidget *optionmenu_CodecList;
  GtkWidget *menu1;
#if 0
  GtkWidget *ffmpeg_mpeg4;
  GtkWidget *ffmpeg_h263;
  GtkWidget *ffmpeg_h263p;
  GtkWidget *ffmpeg_huffyuv;
  GtkWidget *xvid;
  GtkWidget *divx;
  GtkWidget *mjpeg;
#endif  
  GtkWidget *buttonConfigure;
  GtkWidget *hbox1;
  GtkWidget *buttonCalc;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  dialogVideoCodec = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialogVideoCodec), _("VideoCodec"));

  dialog_vbox1 = GTK_DIALOG (dialogVideoCodec)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox2, FALSE, FALSE, 0);

  optionmenu_CodecList = gtk_option_menu_new ();
  gtk_widget_show (optionmenu_CodecList);
  gtk_box_pack_start (GTK_BOX (hbox2), optionmenu_CodecList, FALSE, FALSE, 0);

  menu1 = gtk_menu_new ();
#if 0
  ffmpeg_mpeg4 = gtk_menu_item_new_with_mnemonic (_("FFmpeg Mpeg4"));
  gtk_widget_show (ffmpeg_mpeg4);
  gtk_container_add (GTK_CONTAINER (menu1), ffmpeg_mpeg4);

  ffmpeg_h263 = gtk_menu_item_new_with_mnemonic (_("FFmpeg H263"));
  gtk_widget_show (ffmpeg_h263);
  gtk_container_add (GTK_CONTAINER (menu1), ffmpeg_h263);

  ffmpeg_h263p = gtk_menu_item_new_with_mnemonic (_("FFmpeg H263+"));
  gtk_widget_show (ffmpeg_h263p);
  gtk_container_add (GTK_CONTAINER (menu1), ffmpeg_h263p);

  ffmpeg_huffyuv = gtk_menu_item_new_with_mnemonic (_("FFmpeg Huffyuv"));
  gtk_widget_show (ffmpeg_huffyuv);
  gtk_container_add (GTK_CONTAINER (menu1), ffmpeg_huffyuv);

  xvid = gtk_menu_item_new_with_mnemonic (_("Xvid"));
  gtk_widget_show (xvid);
  gtk_container_add (GTK_CONTAINER (menu1), xvid);

  divx = gtk_menu_item_new_with_mnemonic (_("Divx"));
  gtk_widget_show (divx);
  gtk_container_add (GTK_CONTAINER (menu1), divx);

  mjpeg = gtk_menu_item_new_with_mnemonic (_("Mjpeg"));
  gtk_widget_show (mjpeg);
  gtk_container_add (GTK_CONTAINER (menu1), mjpeg);
#endif
  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_CodecList), menu1);

  buttonConfigure = gtk_button_new_with_mnemonic (_("Configure Codec"));
  gtk_widget_show (buttonConfigure);
  gtk_box_pack_start (GTK_BOX (hbox2), buttonConfigure, FALSE, FALSE, 0);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);

  buttonCalc = gtk_button_new_with_mnemonic (_("Calculator"));
  gtk_widget_show (buttonCalc);
  gtk_box_pack_end (GTK_BOX (hbox1), buttonCalc, FALSE, FALSE, 0);

  dialog_action_area1 = GTK_DIALOG (dialogVideoCodec)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialogVideoCodec), cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

  okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialogVideoCodec), okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialogVideoCodec, dialogVideoCodec, "dialogVideoCodec");
  GLADE_HOOKUP_OBJECT_NO_REF (dialogVideoCodec, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, optionmenu_CodecList, "optionmenu_CodecList");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, menu1, "menu1");
#if 0  
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, ffmpeg_mpeg4, "ffmpeg_mpeg4");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, ffmpeg_h263, "ffmpeg_h263");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, ffmpeg_h263p, "ffmpeg_h263p");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, ffmpeg_huffyuv, "ffmpeg_huffyuv");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, xvid, "xvid");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, divx, "divx");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, mjpeg, "mjpeg");
#endif  
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, buttonConfigure, "buttonConfigure");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, buttonCalc, "buttonCalc");
  GLADE_HOOKUP_OBJECT_NO_REF (dialogVideoCodec, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, okbutton1, "okbutton1");

  return dialogVideoCodec;
}


