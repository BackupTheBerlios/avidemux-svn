/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "ADM_assert.h" 

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "config.h"
#include "ADM_gui2/support.h"
#include "ADM_library/default.h"

//___________________________________
#include "avi_vars.h"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_encoder/ADM_vidEncode.hxx"
#include "ADM_encoder/adm_encoder.h"
#include "ADM_toolkit/toolkit_gtk.h"
//___________________________________
#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

 static GtkWidget	*create_dialogVideoCodec (void) ;
 static SelectCodecType findCodec( void );
 static GtkWidget *dialog;
 static void okCallback(GtkButton * button, gpointer user_data);
 static SelectCodecType myTab[]=
 {
 	CodecFF,
	CodecH263,
	CodecH263P,
	CodecHuff,
	CodecFFV1,
	CodecSnow,
#ifdef USE_XVID_4	
 	CodecXvid4	,
#endif	
#ifdef USE_XX_XVID	
 	CodecXvid	,
#endif
#ifdef USE_DIVX
	CodecDivx	,
#endif

	CodecMjpeg	,
#ifdef USE_MJPEG
	CodecVCD,
	CodecSVCD,
	CodecDVD,
#endif
	CodecXVCD,
	CodecXSVCD,
	
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
uint8_t DIA_videoCodec( SelectCodecType *codec )
{

	uint8_t ret=0;
	SelectCodecType old=*codec;
	
	dialog=create_dialogVideoCodec();
	gtk_transient(dialog);	

	
	// now set the input one
	for(uint32_t i=0;i<sizeof(myTab)/sizeof(SelectCodecType);i++)
		if(*codec==myTab[i])
			{
				// set
				gtk_option_menu_set_history(GTK_OPTION_MENU(lookup_widget(dialog,"optionmenu_CodecList")), i);
			}
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
	
	return ret;
} 

SelectCodecType findCodec( void )
{
uint8_t j;
			j=getRangeInMenu(lookup_widget(dialog,"optionmenu_CodecList"));

			if(j>=sizeof(myTab)/sizeof(SelectCodecType)) ADM_assert(0);
			return myTab[j];
			

}

GtkWidget	*create_dialogVideoCodec (void)
{
  GtkWidget *dialogVideoCodec;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox1;
  GtkWidget *optionmenu_CodecList;
  GtkWidget *menu1;
  GtkWidget *ffmpeg_mpeg4;
  GtkWidget *ffmpeg_h263;
  GtkWidget *ffmpeg_h263p;
  GtkWidget *ffmpeg_huffyuv;
  GtkWidget *ffmpeg_ffv1;
   GtkWidget *ffmpeg_snow;
#ifdef USE_XX_XVID
  GtkWidget *xvid;
#endif
#ifdef USE_XVID_4
  GtkWidget *xvid4;
#endif
#ifdef USE_DIVX
  GtkWidget *divx;
#endif

  GtkWidget *mjpeg;
#ifdef USE_MJPEG
   GtkWidget *vcd;
   GtkWidget *svcd;
   GtkWidget *dvd;
#endif
GtkWidget *xvcd,*xsvcd;;
  GtkWidget *buttonConfigure;
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

  optionmenu_CodecList = gtk_option_menu_new ();
  gtk_widget_show (optionmenu_CodecList);
  gtk_box_pack_start (GTK_BOX (vbox1), optionmenu_CodecList, FALSE, FALSE, 0);

  menu1 = gtk_menu_new ();

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

ffmpeg_ffv1 = gtk_menu_item_new_with_mnemonic (_("FFmpeg FFV1"));
  gtk_widget_show (ffmpeg_ffv1);
  gtk_container_add (GTK_CONTAINER (menu1), ffmpeg_ffv1);
  
  ffmpeg_snow = gtk_menu_item_new_with_mnemonic (_("FFmpeg Snow"));
  gtk_widget_show (ffmpeg_snow);
  gtk_container_add (GTK_CONTAINER (menu1), ffmpeg_snow);


#ifdef USE_XX_XVID
  xvid = gtk_menu_item_new_with_mnemonic (_("Xvid"));
  gtk_widget_show (xvid);
  gtk_container_add (GTK_CONTAINER (menu1), xvid);
#endif
#ifdef USE_XVID_4
  xvid4 = gtk_menu_item_new_with_mnemonic (_("Xvid API 4"));
  gtk_widget_show (xvid4);
  gtk_container_add (GTK_CONTAINER (menu1), xvid4);
#endif
#ifdef USE_DIVX
  divx = gtk_menu_item_new_with_mnemonic (_("Divx"));
  gtk_widget_show (divx);
  gtk_container_add (GTK_CONTAINER (menu1), divx);
#endif



  mjpeg = gtk_menu_item_new_with_mnemonic (_("Mjpeg"));
  gtk_widget_show (mjpeg);
  gtk_container_add (GTK_CONTAINER (menu1), mjpeg);
#ifdef USE_MJPEG
vcd = gtk_menu_item_new_with_mnemonic (_("VCD"));
  gtk_widget_show (vcd);
  gtk_container_add (GTK_CONTAINER (menu1), vcd);

svcd = gtk_menu_item_new_with_mnemonic (_("SVCD"));
  gtk_widget_show (svcd);
  gtk_container_add (GTK_CONTAINER (menu1), svcd);

dvd = gtk_menu_item_new_with_mnemonic (_("DVD"));
  gtk_widget_show (dvd);
  gtk_container_add (GTK_CONTAINER (menu1), dvd);

#endif

xvcd = gtk_menu_item_new_with_mnemonic (_("XVCD"));
  gtk_widget_show (xvcd);
  gtk_container_add (GTK_CONTAINER (menu1), xvcd);

xsvcd = gtk_menu_item_new_with_mnemonic (_("DVD (ffmpeg)"));
  gtk_widget_show (xsvcd);
  gtk_container_add (GTK_CONTAINER (menu1), xsvcd);


  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenu_CodecList), menu1);

  buttonConfigure = gtk_button_new_with_mnemonic (_("Configure codec"));
  gtk_widget_show (buttonConfigure);
  gtk_box_pack_start (GTK_BOX (vbox1), buttonConfigure, FALSE, FALSE, 0);

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
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, optionmenu_CodecList, "optionmenu_CodecList");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, menu1, "menu1");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, ffmpeg_mpeg4, "ffmpeg_mpeg4");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, ffmpeg_h263, "ffmpeg_h263");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, ffmpeg_h263p, "ffmpeg_h263p");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, ffmpeg_huffyuv, "ffmpeg_huffyuv");
#ifdef USE_XX_XVID  
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, xvid, "xvid");
#endif
#ifdef USE_XVID_4
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, xvid4, "xvid4");
#endif

#ifdef USE_DIVX  
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, divx, "divx");
#endif

  GLADE_HOOKUP_OBJECT (dialogVideoCodec, mjpeg, "mjpeg");
#ifdef USE_MJPEG    
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, vcd, "vcd");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, svcd, "svcd");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, dvd, "dvd");
#endif
GLADE_HOOKUP_OBJECT (dialogVideoCodec, xvcd, "xvcd");
GLADE_HOOKUP_OBJECT (dialogVideoCodec, xsvcd, "xsvcd");

  GLADE_HOOKUP_OBJECT (dialogVideoCodec, buttonConfigure, "buttonConfigure");
  GLADE_HOOKUP_OBJECT_NO_REF (dialogVideoCodec, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialogVideoCodec, okbutton1, "okbutton1");

  	 gtk_signal_connect(GTK_OBJECT(buttonConfigure), "clicked",
                      GTK_SIGNAL_FUNC(okCallback),                   (void *) 0);

  return dialogVideoCodec;
}

