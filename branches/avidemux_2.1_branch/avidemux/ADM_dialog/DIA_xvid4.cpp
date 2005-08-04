# include <config.h>

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>


#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "default.h"


#include "ADM_gui2/support.h"
#include "ADM_toolkit/toolkit.hxx"
#include "ADM_toolkit/toolkit_gtk.h"
#include "ADM_toolkit/toolkit_gtk_include.h"

#include "ADM_assert.h" 

#ifdef USE_XVID_4
#include "ADM_encoder/ADM_vidEncode.hxx"

#include "ADM_codecs/ADM_xvid4param.h"


static xvid4EncParam localParam;
static uint32_t mQ,mB,mS;
static GtkWidget *dialog=NULL;
static COMPRESSION_MODE mMode;


static GtkWidget	*create_dialog1 (void);
static void xvid4otherUpload(GtkWidget *dialog);
static void xvid4otherDownload(GtkWidget *dialog);
static void updateMode( void );
static int cb_mod(GtkObject * object, gpointer user_data);
/*uint8_t DIA_xvid4(COMPRESSION_MODE * mode, uint32_t * qz,  uint32_t * br,uint32_t *fsize,
		xvid4EncParam *param);
*/		
		
		
uint8_t DIA_xvid4(COMPRES_PARAMS *incoming)
{
int b;
int ret=0;
		ADM_assert(incoming->extraSettingsLen==sizeof(localParam));
		memcpy(&localParam,incoming->extraSettings,sizeof(localParam));
		dialog=create_dialog1();
		gtk_register_dialog(dialog);
		
#define HIST_SET(x) gtk_option_menu_set_history(GTK_OPTION_MENU(WID(optionmenuType)), x)
#define VAL_SET(x) gtk_write_entry(WID(entryEntry), x)

		mQ=incoming->qz;
		mB=incoming->bitrate;
		mS=incoming->finalsize;	
		mMode=incoming->mode;
		updateMode();	

		xvid4otherUpload(dialog);
 		gtk_signal_connect(GTK_OBJECT(WID(optionmenuType)), "changed",
                      GTK_SIGNAL_FUNC(cb_mod),                   (void *) 0);
	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_OK)
	{

		
		int r,value=0;
		ret=1;
		r=getRangeInMenu(WID(optionmenuType));
		xvid4otherDownload(dialog);
		memcpy(incoming->extraSettings,&localParam,sizeof(localParam));
		switch(r)
			{
				case 0:
					incoming->mode = COMPRESS_CBR;				      
		      			value = (uint32_t) gtk_read_entry(WID(entryEntry));
		      			if (value < 3000)
			  			value *= 1000;
		      			if (value > 16 && value < 6000000)
					{
			    			incoming->bitrate = value;
			    			ret = 1;						
		      			}
					gtk_label_set_text(GTK_LABEL(WID(label11)),"Bitrate (kbps):");
					break;
				case 1:
					incoming->mode = COMPRESS_CQ;		      			
					value = (uint32_t) gtk_spin_button_get_value_as_int(
								GTK_SPIN_BUTTON(WID(spinbuttonQuant)));
		      			if (value >= 2 && value <= 32)
						{
			    			incoming->qz = value;
		      				}
		      			break;

				case 2:
		     				incoming->mode = COMPRESS_2PASS;		       				
						value = (uint32_t) gtk_read_entry(WID(entryEntry));
        					if((value>0)&&(value<4000))
          					{
       							incoming->finalsize=value;
				      			
           					}
						gtk_label_set_text(GTK_LABEL(WID(label11)),"Size (MBytes):");
            					break;
				case 3:
						incoming->mode=COMPRESS_SAME;
						break;
		  		default:
		      				ADM_assert(0);
				}
	}
	gtk_unregister_dialog(dialog);
	gtk_widget_destroy(dialog);
	return ret;

}
// Read the localParam struct and set the ui fields
// accordingly
#define SPIN_GET(x,y) {localParam.y= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(WID(x))) ;\
				printf(#x":%d\n",localParam.y);}

#define SPIN_SET(x,y)  {gtk_spin_button_set_value(GTK_SPIN_BUTTON(WID(x)),(gfloat)localParam.y) ; \
				printf(#x":%d\n",localParam.y);}

#define MENU_SET(x,y) { gtk_option_menu_set_history (GTK_OPTION_MENU(WID(x)),localParam.y);}
#define MENU_GET(x,y) { localParam.y	= getRangeInMenu(WID(x));}
#define TOGGLE_SET(x,y) {gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(WID(x)),localParam.y);}
#define TOGGLE_GET(x,y) {localParam.y=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(WID(x)));\
				printf(#y":%d\n",localParam.y);}
				
#define ENTRY_SET(x,y) {gtk_write_entry(WID(x),(int)localParam.y);}
#define ENTRY_GET(x,y) {localParam.y=gtk_read_entry(WID(x));\
				printf(#y":%d\n",localParam.y);}

#define CHECK_GET(x,y) {localParam.y=gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(WID(x)));}
#define CHECK_SET(x,y) {gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(WID(x)),localParam.y);}							
void xvid4otherUpload(GtkWidget *dialog)
{
	MENU_SET(optionmenuMS,guiLevel);
	MENU_SET(optionmenuVHQ,vhqmode);

	if(localParam.mpegQuantizer)
		RADIO_SET(radiobuttonMpeg,		1);
	else
		RADIO_SET(radiobuttonH263,		1);		
	CHECK_SET(checkbuttonInterlaced,	interlaced);
	CHECK_SET(checkbutton4MV,		inter4mv);
	CHECK_SET(checkbuttonTrellis,		trellis);
	CHECK_SET(checkbuttonCartoon,		cartoon);
	CHECK_SET(checkbuttonGreyScale,		greyscale);
	CHECK_SET(checkbuttonChroma,		chroma_opt);
	CHECK_SET(checkbuttonChromaMotion,	chroma_me);
	CHECK_SET(checkbuttonGMC,		gmc);
	CHECK_SET(checkbuttonQPel,		qpel);
	CHECK_SET(checkbuttonHQAC,		hqac);
	
	CHECK_SET(checkbuttonTurbo,		turbo);
	CHECK_SET(checkbuttonPacked,		packed);
		
  	SPIN_SET(spinbuttonIMaxPeriod,  max_key_interval);
	ENTRY_SET(entryIInterv,		min_key_interval);
	SPIN_SET(spinbuttonBFrame,	bframes);
	
	SPIN_SET(spinbuttonIMax,	qmax[0]);
	SPIN_SET(spinbuttonPMax,	qmax[1]);
	SPIN_SET(spinbuttonBMax,	qmax[2]);
	
	SPIN_SET(spinbuttonIMin,	qmin[0]);
	SPIN_SET(spinbuttonPMin,	qmin[1]);
	SPIN_SET(spinbuttonBMin,	qmin[2]);
	
	
  	ENTRY_SET(entryIBoost		,	keyframe_boost		);
	
  	ENTRY_SET(entryHiPass		,	curve_compression_high	);
  	ENTRY_SET(entryLowPass		,	curve_compression_low	);
  	ENTRY_SET(entryOvrControl,		overflow_control_strength	);
  	
	ENTRY_SET(entryMaxOvrImp	,	max_overflow_improvement	);
  	ENTRY_SET(entryMaxOvrDeg	,	max_overflow_degradation	);
	
  	ENTRY_SET(entryIreduction	,	kfreduction			);
  	ENTRY_SET(entryIInterv	,		kfthreshold			);
	//TOGGLE_SET(toggleHPEL,			halfpel);
	
}
// Read the ui fields and set localParam
// accordingly


void xvid4otherDownload(GtkWidget *dialog)
{
	MENU_GET(optionmenuMS,guiLevel);
	MENU_GET(optionmenuVHQ,vhqmode);
	
	localParam.mpegQuantizer=RADIO_GET(radiobuttonMpeg);
	CHECK_GET(checkbuttonInterlaced,	interlaced);
	CHECK_GET(checkbutton4MV,		inter4mv);
	CHECK_GET(checkbuttonTrellis,		trellis);
	CHECK_GET(checkbuttonCartoon,		cartoon);
	CHECK_GET(checkbuttonGreyScale,		greyscale);
	CHECK_GET(checkbuttonChroma,		chroma_opt);
	CHECK_GET(checkbuttonChromaMotion,	chroma_me);
	CHECK_GET(checkbuttonGMC,		gmc);
	CHECK_GET(checkbuttonQPel,		qpel);
	CHECK_GET(checkbuttonHQAC,		hqac);
	CHECK_GET(checkbuttonTurbo,		turbo);
	CHECK_GET(checkbuttonPacked,		packed);

	
	SPIN_GET(spinbuttonIMaxPeriod,  max_key_interval);	
  	ENTRY_GET(entryIInterv,			kfthreshold);
	SPIN_GET(spinbuttonBFrame,		bframes);
	
  	ENTRY_GET(entryIBoost		,	keyframe_boost		);
	
  	ENTRY_GET(entryHiPass		,	curve_compression_high	);
  	ENTRY_GET(entryLowPass		,	curve_compression_low	);
  	ENTRY_GET(entryOvrControl,		overflow_control_strength	);
  	
	ENTRY_GET(entryMaxOvrImp	,	max_overflow_improvement	);
  	ENTRY_GET(entryMaxOvrDeg	,	max_overflow_degradation	);
	
  	ENTRY_GET(entryIreduction	,	kfreduction			);
  	ENTRY_GET(entryIInterv	,	kfthreshold			);
	//TOGGLE_SET(toggleHPEL,			halfpel);
	SPIN_GET(spinbuttonIMax,	qmax[0]);
	SPIN_GET(spinbuttonPMax,	qmax[1]);
	SPIN_GET(spinbuttonBMax,	qmax[2]);
	
	SPIN_GET(spinbuttonIMin,	qmin[0]);
	SPIN_GET(spinbuttonPMin,	qmin[1]);
	SPIN_GET(spinbuttonBMin,	qmin[2]);
	
	
}

void updateMode( void )
{
uint32_t b;
		// set the right select button
 		switch (mMode)
	    {
	    	case COMPRESS_CBR:
			HIST_SET(0);			
			b=mB/1000;
			VAL_SET(b);
			gtk_widget_set_sensitive(WID(spinbuttonQuant),0);
			gtk_widget_set_sensitive(WID(entryEntry),1);
			gtk_label_set_text(GTK_LABEL(WID(label11)),"Target bitrate (kb/s):");
			break;

		case COMPRESS_2PASS:
			HIST_SET(2);
			VAL_SET(mS);
			gtk_widget_set_sensitive(WID(spinbuttonQuant),0);
			gtk_widget_set_sensitive(WID(entryEntry),1);
			gtk_label_set_text(GTK_LABEL(WID(label11)),"Target size (MBytes):");
			break;

	    	case COMPRESS_CQ:
			HIST_SET(1);
			gtk_widget_set_sensitive(WID(entryEntry),0);
			gtk_widget_set_sensitive(WID(spinbuttonQuant),1);
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(WID(spinbuttonQuant)),(gfloat)mQ);
			break;
		case COMPRESS_SAME:
			HIST_SET(3);
			gtk_widget_set_sensitive(WID(entryEntry),0);
			gtk_widget_set_sensitive(WID(spinbuttonQuant),0);
			
			break;

		}
}		
int cb_mod(GtkObject * object, gpointer user_data)
{
int r;
	r=getRangeInMenu(WID(optionmenuType));
	switch(r)
	{
		case 0: mMode=COMPRESS_CBR ;break;
		case 1: mMode=COMPRESS_CQ ;break;
		case 2: mMode=COMPRESS_2PASS ;break;
		case 3: mMode=COMPRESS_SAME;break;
	
	}
	updateMode();
	printf("Changed!!!\n");

}
//-------------------- Glade-2 here----------		
//_____KK_______

GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *notebook1;
  GtkWidget *vbox2;
  GtkWidget *frame1;
  GtkWidget *vbox3;
  GtkWidget *hbox19;
  GtkWidget *vbox21;
  GtkWidget *label10;
  GtkWidget *label11;
  GtkWidget *label12;
  GtkWidget *vbox22;
  GtkWidget *optionmenuType;
  GtkWidget *menu1;
  GtkWidget *one_pass_cbr1;
  GtkWidget *one_pass_quantizer1;
  GtkWidget *two_pass1;
  GtkWidget *same_qz_as_input1;
  GtkWidget *entryEntry;
  GtkObject *spinbuttonQuant_adj;
  GtkWidget *spinbuttonQuant;
  GtkWidget *label9;
  GtkWidget *hbox7;
  GtkWidget *vbox4;
  GtkWidget *checkbuttonInterlaced;
  GtkWidget *checkbuttonGreyScale;
  GtkWidget *vbox5;
  GtkWidget *checkbuttonCartoon;
  GtkWidget *checkbuttonChroma;
  GtkWidget *vbox23;
  GtkWidget *checkbuttonTurbo;
  GtkWidget *checkbuttonPacked;
  GtkWidget *label8;
  GtkWidget *vbox6;
  GtkWidget *hbox8;
  GtkWidget *vbox8;
  GtkWidget *label13;
  GtkWidget *label14;
  GtkWidget *vbox7;
  GtkWidget *optionmenuMS;
  GtkWidget *menu2;
  GtkWidget *_0___none1;
  GtkWidget *_1___very_low1;
  GtkWidget *_2___low2;
  GtkWidget *_3___medium1;
  GtkWidget *_4___high1;
  GtkWidget *_5___very_high1;
  GtkWidget *_6___ultra_high1;
  GtkWidget *optionmenuVHQ;
  GtkWidget *menu3;
  GtkWidget *_0___off1;
  GtkWidget *_1___mode_decision1;
  GtkWidget *_2___limited_search1;
  GtkWidget *_3___medium_search1;
  GtkWidget *_4___wide_search1;
  GtkWidget *hbox10;
  GtkWidget *checkbuttonChromaMotion;
  GtkWidget *checkbutton4MV;
  GtkWidget *checkbuttonHQAC;
  GtkWidget *hbox9;
  GtkWidget *label15;
  GtkObject *spinbuttonIMinPeriod_adj;
  GtkWidget *spinbuttonIMinPeriod;
  GtkWidget *label16;
  GtkObject *spinbuttonIMaxPeriod_adj;
  GtkWidget *spinbuttonIMaxPeriod;
  GtkWidget *frame2;
  GtkWidget *vbox9;
  GtkWidget *hbox11;
  GtkWidget *label18;
  GtkObject *spinbuttonBFrame_adj;
  GtkWidget *spinbuttonBFrame;
  GtkWidget *hbox12;
  GtkWidget *checkbuttonQPel;
  GtkWidget *checkbuttonGMC;
  GtkWidget *label17;
  GtkWidget *label2;
  GtkWidget *vbox10;
  GtkWidget *hbox13;
  GtkWidget *label19;
  GtkWidget *radiobuttonH263;
  GSList *radiobuttonH263_group = NULL;
  GtkWidget *radiobuttonMpeg;
  GtkWidget *checkbuttonTrellis;
  GtkWidget *frame3;
  GtkWidget *hbox14;
  GtkWidget *vbox11;
  GtkWidget *label20;
  GtkWidget *label21;
  GtkWidget *label22;
  GtkWidget *vbox12;
  GtkObject *spinbuttonIMin_adj;
  GtkWidget *spinbuttonIMin;
  GtkObject *spinbuttonPMin_adj;
  GtkWidget *spinbuttonPMin;
  GtkObject *spinbuttonBMin_adj;
  GtkWidget *spinbuttonBMin;
  GtkWidget *vbox13;
  GtkWidget *label24;
  GtkWidget *label25;
  GtkWidget *label26;
  GtkWidget *vbox14;
  GtkObject *spinbuttonIMax_adj;
  GtkWidget *spinbuttonIMax;
  GtkObject *spinbuttonPMax_adj;
  GtkWidget *spinbuttonPMax;
  GtkObject *spinbuttonBMax_adj;
  GtkWidget *spinbuttonBMax;

  GtkWidget *label3;
  GtkWidget *vbox15;
  GtkWidget *frame4;
  GtkWidget *vbox16;
  GtkWidget *hbox;
  GtkWidget *vbox17;
  GtkWidget *label30;
  GtkWidget *label31;
  GtkWidget *label32;
  GtkWidget *label28;
  GtkWidget *label29;
  GtkWidget *vbox18;
  GtkWidget *entryIBoost;
  GtkWidget *entryIInterv;
  GtkWidget *entryIreduction;
  GtkWidget *entryMaxOvrImp;
  GtkWidget *entryMaxOvrDeg;
  GtkWidget *label27;
  GtkWidget *frame5;
  GtkWidget *hbox18;
  GtkWidget *vbox19;
  GtkWidget *label37;
  GtkWidget *label38;
  GtkWidget *label39;
  GtkWidget *vbox20;
  GtkWidget *entryHiPass;
  GtkWidget *entryLowPass;
  GtkWidget *entryOvrControl;
  GtkWidget *label36;
  GtkWidget *label4;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  dialog1 = gtk_dialog_new ();
  gtk_container_set_border_width (GTK_CONTAINER (dialog1), 2);
  gtk_window_set_title (GTK_WINDOW (dialog1), _("XviD encoding options"));

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  notebook1 = gtk_notebook_new ();
  gtk_widget_show (notebook1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), notebook1, TRUE, TRUE, 0);

  vbox2 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox2);
  gtk_notebook_set_tab_label_packing (GTK_NOTEBOOK (notebook1), vbox2,
                                      FALSE, FALSE, GTK_PACK_START);
  gtk_container_set_border_width (GTK_CONTAINER (vbox2), 5);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox2), frame1, FALSE, FALSE, 0);

  vbox3 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox3);
  gtk_container_add (GTK_CONTAINER (frame1), vbox3);
  gtk_container_set_border_width (GTK_CONTAINER (vbox3), 5);

  hbox19 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox19);
  gtk_box_pack_start (GTK_BOX (vbox3), hbox19, TRUE, TRUE, 0);

  vbox21 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox21);
  gtk_box_pack_start (GTK_BOX (hbox19), vbox21, FALSE, FALSE, 0);

  label10 = gtk_label_new (_("Encoding type:"));
  gtk_widget_show (label10);
  gtk_box_pack_start (GTK_BOX (vbox21), label10, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label10), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label10), 0, 0.5);

  label11 = gtk_label_new (_("Bitrate (kb/s):"));
  gtk_widget_show (label11);
  gtk_box_pack_start (GTK_BOX (vbox21), label11, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label11), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label11), 0, 0.5);

  label12 = gtk_label_new (_("Quantizer:"));
  gtk_widget_show (label12);
  gtk_box_pack_start (GTK_BOX (vbox21), label12, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label12), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label12), 0, 0.5);

  vbox22 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox22);
  gtk_box_pack_start (GTK_BOX (hbox19), vbox22, FALSE, FALSE, 0);

  optionmenuType = gtk_option_menu_new ();
  gtk_widget_show (optionmenuType);
  gtk_box_pack_start (GTK_BOX (vbox22), optionmenuType, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, optionmenuType, _("Select 1-pass or 2-pass encoding"), NULL);

  menu1 = gtk_menu_new ();

  one_pass_cbr1 = gtk_menu_item_new_with_mnemonic (_("Single pass - bitrate"));
  gtk_widget_show (one_pass_cbr1);
  gtk_container_add (GTK_CONTAINER (menu1), one_pass_cbr1);

  one_pass_quantizer1 = gtk_menu_item_new_with_mnemonic (_("Single pass - quantizer"));
  gtk_widget_show (one_pass_quantizer1);
  gtk_container_add (GTK_CONTAINER (menu1), one_pass_quantizer1);

  two_pass1 = gtk_menu_item_new_with_mnemonic (_("Two pass"));
  gtk_widget_show (two_pass1);
  gtk_container_add (GTK_CONTAINER (menu1), two_pass1);

  same_qz_as_input1 = gtk_menu_item_new_with_mnemonic (_("Same Qz as input"));
  gtk_widget_show (same_qz_as_input1);
  gtk_container_add (GTK_CONTAINER (menu1), same_qz_as_input1);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenuType), menu1);

  entryEntry = gtk_entry_new ();
  gtk_widget_show (entryEntry);
  gtk_box_pack_start (GTK_BOX (vbox22), entryEntry, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, entryEntry, _("Target video bitrate"), NULL);
  gtk_entry_set_width_chars (GTK_ENTRY (entryEntry), 10);

  spinbuttonQuant_adj = gtk_adjustment_new (4, 1, 31, 0.1, 10, 10);
  spinbuttonQuant = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonQuant_adj), 1, 1);
  gtk_widget_show (spinbuttonQuant);
  gtk_box_pack_start (GTK_BOX (vbox22), spinbuttonQuant, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinbuttonQuant, _("Constant quantizer - each frame will get the same compression"), NULL);

  label9 = gtk_label_new (_("Variable bitrate mode"));
  gtk_widget_show (label9);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label9);
  gtk_label_set_justify (GTK_LABEL (label9), GTK_JUSTIFY_LEFT);

  hbox7 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox7);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox7, FALSE, FALSE, 0);

  vbox4 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox4);
  gtk_box_pack_start (GTK_BOX (hbox7), vbox4, FALSE, FALSE, 0);

  checkbuttonInterlaced = gtk_check_button_new_with_mnemonic (_("_Interlaced"));
  gtk_widget_show (checkbuttonInterlaced);
  gtk_box_pack_start (GTK_BOX (vbox4), checkbuttonInterlaced, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonInterlaced, _("Enables interlaced frame support - use only if your source contains interlacing artifacts (i.e. fields instead of progressive frames)"), NULL);

  checkbuttonGreyScale = gtk_check_button_new_with_mnemonic (_("_Greyscale"));
  gtk_widget_show (checkbuttonGreyScale);
  gtk_box_pack_start (GTK_BOX (vbox4), checkbuttonGreyScale, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonGreyScale, _("Encode in black & white"), NULL);

  vbox5 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox5);
  gtk_box_pack_start (GTK_BOX (hbox7), vbox5, FALSE, FALSE, 0);

  checkbuttonCartoon = gtk_check_button_new_with_mnemonic (_("Ca_rtoon mode"));
  gtk_widget_show (checkbuttonCartoon);
  gtk_box_pack_start (GTK_BOX (vbox5), checkbuttonCartoon, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonCartoon, _("Enables special motion estimation features for cartoons/anime"), NULL);

  checkbuttonChroma = gtk_check_button_new_with_mnemonic (_("C_hroma optimizer"));
  gtk_widget_show (checkbuttonChroma);
  gtk_box_pack_start (GTK_BOX (vbox5), checkbuttonChroma, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonChroma, _("Enables a chroma optimizer prefilter"), NULL);

  vbox23 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox23);
  gtk_box_pack_start (GTK_BOX (hbox7), vbox23, TRUE, TRUE, 0);

  checkbuttonTurbo = gtk_check_button_new_with_mnemonic (_("Turbo Mode"));
  gtk_widget_show (checkbuttonTurbo);
  gtk_box_pack_start (GTK_BOX (vbox23), checkbuttonTurbo, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonTurbo, _("Enables fast 1st pass encoding"), NULL);

  checkbuttonPacked = gtk_check_button_new_with_mnemonic (_("Packed bitstream"));
  gtk_widget_show (checkbuttonPacked);
  gtk_box_pack_start (GTK_BOX (vbox23), checkbuttonPacked, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonPacked, _("Enables Divx like packed vop, DO NOT SET, it is ugly"), NULL);

  label8 = gtk_label_new (_("Main"));
  gtk_widget_show (label8);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label8);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);

  vbox6 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox6);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox6);
  gtk_notebook_set_tab_label_packing (GTK_NOTEBOOK (notebook1), vbox6,
                                      FALSE, FALSE, GTK_PACK_START);
  gtk_container_set_border_width (GTK_CONTAINER (vbox6), 5);

  hbox8 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox8);
  gtk_box_pack_start (GTK_BOX (vbox6), hbox8, FALSE, FALSE, 0);

  vbox8 = gtk_vbox_new (FALSE, 10);
  gtk_widget_show (vbox8);
  gtk_box_pack_start (GTK_BOX (hbox8), vbox8, FALSE, FALSE, 0);

  label13 = gtk_label_new (_("Motion search precision:"));
  gtk_widget_show (label13);
  gtk_box_pack_start (GTK_BOX (vbox8), label13, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label13), GTK_JUSTIFY_LEFT);

  label14 = gtk_label_new (_("VHQ mode:"));
  gtk_widget_show (label14);
  gtk_box_pack_start (GTK_BOX (vbox8), label14, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label14), GTK_JUSTIFY_FILL);
  gtk_misc_set_alignment (GTK_MISC (label14), 0, 0.5);

  vbox7 = gtk_vbox_new (FALSE, 10);
  gtk_widget_show (vbox7);
  gtk_box_pack_start (GTK_BOX (hbox8), vbox7, FALSE, FALSE, 0);

  optionmenuMS = gtk_option_menu_new ();
  gtk_widget_show (optionmenuMS);
  gtk_box_pack_start (GTK_BOX (vbox7), optionmenuMS, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, optionmenuMS, _("Higher settings give higher quality results, at the cost of slower encoding"), NULL);

  menu2 = gtk_menu_new ();

  _0___none1 = gtk_menu_item_new_with_mnemonic (_("0 - None"));
  gtk_widget_show (_0___none1);
  gtk_container_add (GTK_CONTAINER (menu2), _0___none1);

  _1___very_low1 = gtk_menu_item_new_with_mnemonic (_("1 - Very low"));
  gtk_widget_show (_1___very_low1);
  gtk_container_add (GTK_CONTAINER (menu2), _1___very_low1);

  _2___low2 = gtk_menu_item_new_with_mnemonic (_("2 - Low"));
  gtk_widget_show (_2___low2);
  gtk_container_add (GTK_CONTAINER (menu2), _2___low2);

  _3___medium1 = gtk_menu_item_new_with_mnemonic (_("3 - Medium"));
  gtk_widget_show (_3___medium1);
  gtk_container_add (GTK_CONTAINER (menu2), _3___medium1);

  _4___high1 = gtk_menu_item_new_with_mnemonic (_("4 - High"));
  gtk_widget_show (_4___high1);
  gtk_container_add (GTK_CONTAINER (menu2), _4___high1);

  _5___very_high1 = gtk_menu_item_new_with_mnemonic (_("5 - Very high"));
  gtk_widget_show (_5___very_high1);
  gtk_container_add (GTK_CONTAINER (menu2), _5___very_high1);

  _6___ultra_high1 = gtk_menu_item_new_with_mnemonic (_("6 - Ultra high"));
  gtk_widget_show (_6___ultra_high1);
  gtk_container_add (GTK_CONTAINER (menu2), _6___ultra_high1);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenuMS), menu2);

  optionmenuVHQ = gtk_option_menu_new ();
  gtk_widget_show (optionmenuVHQ);
  gtk_box_pack_start (GTK_BOX (vbox7), optionmenuVHQ, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, optionmenuVHQ, _("VHQ enables an additional search process to increase quality"), NULL);

  menu3 = gtk_menu_new ();

  _0___off1 = gtk_menu_item_new_with_mnemonic (_("0 - Off"));
  gtk_widget_show (_0___off1);
  gtk_container_add (GTK_CONTAINER (menu3), _0___off1);

  _1___mode_decision1 = gtk_menu_item_new_with_mnemonic (_("1 - Mode decision"));
  gtk_widget_show (_1___mode_decision1);
  gtk_container_add (GTK_CONTAINER (menu3), _1___mode_decision1);

  _2___limited_search1 = gtk_menu_item_new_with_mnemonic (_("2 - Limited search"));
  gtk_widget_show (_2___limited_search1);
  gtk_container_add (GTK_CONTAINER (menu3), _2___limited_search1);

  _3___medium_search1 = gtk_menu_item_new_with_mnemonic (_("3 - Medium search"));
  gtk_widget_show (_3___medium_search1);
  gtk_container_add (GTK_CONTAINER (menu3), _3___medium_search1);

  _4___wide_search1 = gtk_menu_item_new_with_mnemonic (_("4 - Wide search"));
  gtk_widget_show (_4___wide_search1);
  gtk_container_add (GTK_CONTAINER (menu3), _4___wide_search1);

  gtk_option_menu_set_menu (GTK_OPTION_MENU (optionmenuVHQ), menu3);

  hbox10 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox10);
  gtk_box_pack_start (GTK_BOX (vbox6), hbox10, FALSE, FALSE, 0);

  checkbuttonChromaMotion = gtk_check_button_new_with_mnemonic (_("C_hroma motion"));
  gtk_widget_show (checkbuttonChromaMotion);
  gtk_box_pack_start (GTK_BOX (hbox10), checkbuttonChromaMotion, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonChromaMotion, _("Use chroma information to estimate motion"), NULL);

  checkbutton4MV = gtk_check_button_new_with_mnemonic (_("_4MV"));
  gtk_widget_show (checkbutton4MV);
  gtk_box_pack_start (GTK_BOX (hbox10), checkbutton4MV, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbutton4MV, _("Use 4 motion vectors per macroblock, might give better compression"), NULL);

  checkbuttonHQAC = gtk_check_button_new_with_mnemonic (_("HQ _AC"));
  gtk_widget_show (checkbuttonHQAC);
  gtk_box_pack_start (GTK_BOX (hbox10), checkbuttonHQAC, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonHQAC, _("Enable a better prediction of AC component"), NULL);

  hbox9 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox9);
  gtk_box_pack_start (GTK_BOX (vbox6), hbox9, FALSE, FALSE, 0);

  label15 = gtk_label_new (_("I-frame interval - Min:"));
  gtk_widget_show (label15);
  gtk_box_pack_start (GTK_BOX (hbox9), label15, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label15), GTK_JUSTIFY_LEFT);

  spinbuttonIMinPeriod_adj = gtk_adjustment_new (1, 1, 9, 1, 10, 10);
  spinbuttonIMinPeriod = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonIMinPeriod_adj), 1, 0);
  gtk_widget_show (spinbuttonIMinPeriod);
  gtk_box_pack_start (GTK_BOX (hbox9), spinbuttonIMinPeriod, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinbuttonIMinPeriod, _("Minimum space between I-frames, 1 will disable forced I-frame spacing"), NULL);

  label16 = gtk_label_new (_("Max:"));
  gtk_widget_show (label16);
  gtk_box_pack_start (GTK_BOX (hbox9), label16, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label16), GTK_JUSTIFY_LEFT);

  spinbuttonIMaxPeriod_adj = gtk_adjustment_new (250, 1, 500, 1, 10, 10);
  spinbuttonIMaxPeriod = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonIMaxPeriod_adj), 1, 0);
  gtk_widget_show (spinbuttonIMaxPeriod);
  gtk_box_pack_start (GTK_BOX (hbox9), spinbuttonIMaxPeriod, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinbuttonIMaxPeriod, _("Maximum number of frames allowed between I-frames"), NULL);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_show (frame2);
  gtk_box_pack_start (GTK_BOX (vbox6), frame2, FALSE, FALSE, 0);

  vbox9 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox9);
  gtk_container_add (GTK_CONTAINER (frame2), vbox9);
  gtk_container_set_border_width (GTK_CONTAINER (vbox9), 5);

  hbox11 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox11);
  gtk_box_pack_start (GTK_BOX (vbox9), hbox11, FALSE, FALSE, 0);

  label18 = gtk_label_new (_("Number of B-frames:"));
  gtk_widget_show (label18);
  gtk_box_pack_start (GTK_BOX (hbox11), label18, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label18), GTK_JUSTIFY_LEFT);

  spinbuttonBFrame_adj = gtk_adjustment_new (1, 0, 3, 1, 1, 1);
  spinbuttonBFrame = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonBFrame_adj), 1, 0);
  gtk_widget_show (spinbuttonBFrame);
  gtk_box_pack_start (GTK_BOX (hbox11), spinbuttonBFrame, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinbuttonBFrame, _("Maximum number of sequential B-frames (when set to 0, the original I/P encoder is used)"), NULL);

  hbox12 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox12);
  gtk_box_pack_start (GTK_BOX (vbox9), hbox12, FALSE, FALSE, 0);

  checkbuttonQPel = gtk_check_button_new_with_mnemonic (_("_Qpel"));
  gtk_widget_show (checkbuttonQPel);
  gtk_box_pack_start (GTK_BOX (hbox12), checkbuttonQPel, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonQPel, _("Use Quarter pixel resolution for a more precise motion estimation"), NULL);

  checkbuttonGMC = gtk_check_button_new_with_mnemonic (_("_GMC"));
  gtk_widget_show (checkbuttonGMC);
  gtk_box_pack_start (GTK_BOX (hbox12), checkbuttonGMC, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonGMC, _("Global Motion Compensation can save bits on camera pans, zooming and rotation"), NULL);

  label17 = gtk_label_new (_("Advanced Simple Profile"));
  gtk_widget_show (label17);
  gtk_frame_set_label_widget (GTK_FRAME (frame2), label17);
  gtk_label_set_justify (GTK_LABEL (label17), GTK_JUSTIFY_LEFT);

  label2 = gtk_label_new (_("Motion Estimation"));
  gtk_widget_show (label2);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label2);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);

  vbox10 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox10);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox10);
  gtk_container_set_border_width (GTK_CONTAINER (vbox10), 5);

  hbox13 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox13);
  gtk_box_pack_start (GTK_BOX (vbox10), hbox13, FALSE, FALSE, 0);

  label19 = gtk_label_new (_("Quantization type:"));
  gtk_widget_show (label19);
  gtk_box_pack_start (GTK_BOX (hbox13), label19, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label19), GTK_JUSTIFY_LEFT);

  radiobuttonH263 = gtk_radio_button_new_with_mnemonic (NULL, _("_H.263"));
  gtk_widget_show (radiobuttonH263);
  gtk_box_pack_start (GTK_BOX (hbox13), radiobuttonH263, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, radiobuttonH263, _("H.263 quantizers smooth the picture (useful at lower bitrates)"), NULL);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobuttonH263), radiobuttonH263_group);
  radiobuttonH263_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobuttonH263));

  radiobuttonMpeg = gtk_radio_button_new_with_mnemonic (NULL, _("_MPEG"));
  gtk_widget_show (radiobuttonMpeg);
  gtk_box_pack_start (GTK_BOX (hbox13), radiobuttonMpeg, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, radiobuttonMpeg, _("MPEG quantizers (slightly slower) produce sharper picture (useful at higher bitrates)"), NULL);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobuttonMpeg), radiobuttonH263_group);
  radiobuttonH263_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobuttonMpeg));

  checkbuttonTrellis = gtk_check_button_new_with_mnemonic (_("_Trellis quantization"));
  gtk_widget_show (checkbuttonTrellis);
  gtk_box_pack_start (GTK_BOX (vbox10), checkbuttonTrellis, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, checkbuttonTrellis, _("Rate distortion quantization, will find optimal encoding for each block"), NULL);

  frame3 = gtk_frame_new (NULL);
  gtk_widget_show (frame3);
  gtk_box_pack_start (GTK_BOX (vbox10), frame3, FALSE, FALSE, 0);

  hbox14 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox14);
  gtk_container_add (GTK_CONTAINER (frame3), hbox14);
  gtk_container_set_border_width (GTK_CONTAINER (hbox14), 5);

  vbox11 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox11);
  gtk_box_pack_start (GTK_BOX (hbox14), vbox11, FALSE, FALSE, 0);

  label20 = gtk_label_new (_("I-frame quantizer - Min:"));
  gtk_widget_show (label20);
  gtk_box_pack_start (GTK_BOX (vbox11), label20, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label20), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label20), 0, 0.5);

  label21 = gtk_label_new (_("P-frame quantizer - Min:"));
  gtk_widget_show (label21);
  gtk_box_pack_start (GTK_BOX (vbox11), label21, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label21), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label21), 0, 0.5);

  label22 = gtk_label_new (_("B-frame quantizer - Min:"));
  gtk_widget_show (label22);
  gtk_box_pack_start (GTK_BOX (vbox11), label22, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label22), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label22), 0, 0.5);

  vbox12 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox12);
  gtk_box_pack_start (GTK_BOX (hbox14), vbox12, FALSE, FALSE, 0);

  spinbuttonIMin_adj = gtk_adjustment_new (2, 1, 31, 1, 10, 10);
  spinbuttonIMin = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonIMin_adj), 1, 0);
  gtk_widget_show (spinbuttonIMin);
  gtk_box_pack_start (GTK_BOX (vbox12), spinbuttonIMin, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinbuttonIMin, _("Minimum quantizer allowed for I-frames"), NULL);

  spinbuttonPMin_adj = gtk_adjustment_new (2, 1, 31, 1, 10, 10);
  spinbuttonPMin = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonPMin_adj), 1, 0);
  gtk_widget_show (spinbuttonPMin);
  gtk_box_pack_start (GTK_BOX (vbox12), spinbuttonPMin, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinbuttonPMin, _("Minimum quantizer allowed for P-frames"), NULL);

  spinbuttonBMin_adj = gtk_adjustment_new (2, 1, 31, 1, 10, 10);
  spinbuttonBMin = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonBMin_adj), 1, 0);
  gtk_widget_show (spinbuttonBMin);
  gtk_box_pack_start (GTK_BOX (vbox12), spinbuttonBMin, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinbuttonBMin, _("Minimum quantizer allowed for B-frames"), NULL);

  vbox13 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox13);
  gtk_box_pack_start (GTK_BOX (hbox14), vbox13, FALSE, FALSE, 0);

  label24 = gtk_label_new (_("Max:"));
  gtk_widget_show (label24);
  gtk_box_pack_start (GTK_BOX (vbox13), label24, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label24), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label24), 0, 0.5);

  label25 = gtk_label_new (_("Max:"));
  gtk_widget_show (label25);
  gtk_box_pack_start (GTK_BOX (vbox13), label25, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label25), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label25), 0, 0.5);

  label26 = gtk_label_new (_("Max:"));
  gtk_widget_show (label26);
  gtk_box_pack_start (GTK_BOX (vbox13), label26, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label26), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label26), 0, 0.5);

  vbox14 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox14);
  gtk_box_pack_start (GTK_BOX (hbox14), vbox14, FALSE, FALSE, 0);

  spinbuttonIMax_adj = gtk_adjustment_new (31, 1, 31, 1, 10, 10);
  spinbuttonIMax = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonIMax_adj), 1, 0);
  gtk_widget_show (spinbuttonIMax);
  gtk_box_pack_start (GTK_BOX (vbox14), spinbuttonIMax, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinbuttonIMax, _("Maximum quantizer allowed for I-frames"), NULL);

  spinbuttonPMax_adj = gtk_adjustment_new (31, 1, 31, 1, 10, 10);
  spinbuttonPMax = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonPMax_adj), 1, 0);
  gtk_widget_show (spinbuttonPMax);
  gtk_box_pack_start (GTK_BOX (vbox14), spinbuttonPMax, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinbuttonPMax, _("Maximum quantizer allowed for P-frames"), NULL);

  spinbuttonBMax_adj = gtk_adjustment_new (31, 1, 31, 1, 10, 10);
  spinbuttonBMax = gtk_spin_button_new (GTK_ADJUSTMENT (spinbuttonBMax_adj), 1, 0);
  gtk_widget_show (spinbuttonBMax);
  gtk_box_pack_start (GTK_BOX (vbox14), spinbuttonBMax, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, spinbuttonBMax, _("Maximum quantizer allowed for B-frames"), NULL);

  label26 = gtk_label_new (_("Quantizer restrictions"));
  gtk_widget_show (label26);
  gtk_frame_set_label_widget (GTK_FRAME (frame3), label26);
  gtk_label_set_justify (GTK_LABEL (label26), GTK_JUSTIFY_LEFT);

  label3 = gtk_label_new (_("Quantization"));
  gtk_widget_show (label3);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), label3);
  gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_LEFT);

  vbox15 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox15);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox15);
  gtk_notebook_set_tab_label_packing (GTK_NOTEBOOK (notebook1), vbox15,
                                      FALSE, FALSE, GTK_PACK_START);
  gtk_container_set_border_width (GTK_CONTAINER (vbox15), 5);

  frame4 = gtk_frame_new (NULL);
  gtk_widget_show (frame4);
  gtk_box_pack_start (GTK_BOX (vbox15), frame4, FALSE, FALSE, 0);

  vbox16 = gtk_vbox_new (FALSE, 10);
  gtk_widget_show (vbox16);
  gtk_container_add (GTK_CONTAINER (frame4), vbox16);
  gtk_container_set_border_width (GTK_CONTAINER (vbox16), 5);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox);
  gtk_box_pack_start (GTK_BOX (vbox16), hbox, FALSE, FALSE, 0);

  vbox17 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox17);
  gtk_box_pack_start (GTK_BOX (hbox), vbox17, TRUE, TRUE, 0);

  label30 = gtk_label_new (_("I-frame boost (%):"));
  gtk_widget_show (label30);
  gtk_box_pack_start (GTK_BOX (vbox17), label30, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label30), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label30), 0, 0.5);

  label31 = gtk_label_new (_("I-frames closer than ... frames..."));
  gtk_widget_show (label31);
  gtk_box_pack_start (GTK_BOX (vbox17), label31, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label31), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label31), 0, 0.5);

  label32 = gtk_label_new (_("...are reduced by (%):"));
  gtk_widget_show (label32);
  gtk_box_pack_start (GTK_BOX (vbox17), label32, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label32), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label32), 0, 0.5);

  label28 = gtk_label_new (_("Max overflow improvement (%):"));
  gtk_widget_show (label28);
  gtk_box_pack_start (GTK_BOX (vbox17), label28, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label28), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label28), 0, 0.5);

  label29 = gtk_label_new (_("Max overflow degradation (%):"));
  gtk_widget_show (label29);
  gtk_box_pack_start (GTK_BOX (vbox17), label29, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label29), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label29), 0, 0.5);

  vbox18 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox18);
  gtk_box_pack_start (GTK_BOX (hbox), vbox18, FALSE, FALSE, 0);

  entryIBoost = gtk_entry_new ();
  gtk_widget_show (entryIBoost);
  gtk_box_pack_start (GTK_BOX (vbox18), entryIBoost, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, entryIBoost, _("A value of 20 will give 20% more bits to every I-frame"), NULL);

  entryIInterv = gtk_entry_new ();
  gtk_widget_show (entryIInterv);
  gtk_box_pack_start (GTK_BOX (vbox18), entryIInterv, FALSE, FALSE, 0);
  gtk_widget_set_size_request (entryIInterv, 158, -2);
  gtk_tooltips_set_tip (tooltips, entryIInterv, _("I-frames appearing in the range below this value will be treated as consecutive keyframes"), NULL);

  entryIreduction = gtk_entry_new ();
  gtk_widget_show (entryIreduction);
  gtk_box_pack_start (GTK_BOX (vbox18), entryIreduction, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, entryIreduction, _("Reduction of bitrate for the first consecutive I-frames. The last I-frame will get treated normally"), NULL);

  entryMaxOvrImp = gtk_entry_new ();
  gtk_widget_show (entryMaxOvrImp);
  gtk_box_pack_start (GTK_BOX (vbox18), entryMaxOvrImp, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, entryMaxOvrImp, _("How much of the overflow the codec can eat into during undersized sections - larger values will bridge the gap faster"), NULL);

  entryMaxOvrDeg = gtk_entry_new ();
  gtk_widget_show (entryMaxOvrDeg);
  gtk_box_pack_start (GTK_BOX (vbox18), entryMaxOvrDeg, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, entryMaxOvrDeg, _("How much of the overflow the codec can eat into during oversized sections - larger values will bridge the gap faster"), NULL);

  label27 = gtk_label_new (_("Two pass tuning"));
  gtk_widget_show (label27);
  gtk_frame_set_label_widget (GTK_FRAME (frame4), label27);
  gtk_label_set_justify (GTK_LABEL (label27), GTK_JUSTIFY_LEFT);

  frame5 = gtk_frame_new (NULL);
  gtk_widget_show (frame5);
  gtk_box_pack_start (GTK_BOX (vbox15), frame5, FALSE, FALSE, 0);

  hbox18 = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox18);
  gtk_container_add (GTK_CONTAINER (frame5), hbox18);
  gtk_container_set_border_width (GTK_CONTAINER (hbox18), 5);

  vbox19 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox19);
  gtk_box_pack_start (GTK_BOX (hbox18), vbox19, TRUE, TRUE, 0);

  label37 = gtk_label_new (_("High bitrate scenes (%):"));
  gtk_widget_show (label37);
  gtk_box_pack_start (GTK_BOX (vbox19), label37, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label37), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label37), 0, 0.5);

  label38 = gtk_label_new (_("Low bitrate scenes (%):"));
  gtk_widget_show (label38);
  gtk_box_pack_start (GTK_BOX (vbox19), label38, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label38), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label38), 0, 0.5);

  label39 = gtk_label_new (_("Overflow control strength (%):"));
  gtk_widget_show (label39);
  gtk_box_pack_start (GTK_BOX (vbox19), label39, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label39), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label39), 0, 0.5);

  vbox20 = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox20);
  gtk_box_pack_start (GTK_BOX (hbox18), vbox20, FALSE, FALSE, 0);

  entryHiPass = gtk_entry_new ();
  gtk_widget_show (entryHiPass);
  gtk_box_pack_start (GTK_BOX (vbox20), entryHiPass, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, entryHiPass, _("The higher this value, the more bits get taken from frames larger than the average size, and redistributed to others"), NULL);

  entryLowPass = gtk_entry_new ();
  gtk_widget_show (entryLowPass);
  gtk_box_pack_start (GTK_BOX (vbox20), entryLowPass, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, entryLowPass, _("The higher this value, the more bits get assigned to frames below the average frame size"), NULL);

  entryOvrControl = gtk_entry_new ();
  gtk_widget_show (entryOvrControl);
  gtk_box_pack_start (GTK_BOX (vbox20), entryOvrControl, FALSE, FALSE, 0);
  gtk_tooltips_set_tip (tooltips, entryOvrControl, _("0 = default from core (let XviD decide), else overflow payback percent per frame"), NULL);

  label36 = gtk_label_new (_("Curve compression"));
  gtk_widget_show (label36);
  gtk_frame_set_label_widget (GTK_FRAME (frame5), label36);
  gtk_label_set_justify (GTK_LABEL (label36), GTK_JUSTIFY_LEFT);

  label4 = gtk_label_new (_("Second Pass"));
  gtk_widget_show (label4);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 3), label4);
  gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_LEFT);

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
  GLADE_HOOKUP_OBJECT (dialog1, notebook1, "notebook1");
  GLADE_HOOKUP_OBJECT (dialog1, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (dialog1, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (dialog1, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (dialog1, hbox19, "hbox19");
  GLADE_HOOKUP_OBJECT (dialog1, vbox21, "vbox21");
  GLADE_HOOKUP_OBJECT (dialog1, label10, "label10");
  GLADE_HOOKUP_OBJECT (dialog1, label11, "label11");
  GLADE_HOOKUP_OBJECT (dialog1, label12, "label12");
  GLADE_HOOKUP_OBJECT (dialog1, vbox22, "vbox22");
  GLADE_HOOKUP_OBJECT (dialog1, optionmenuType, "optionmenuType");
  GLADE_HOOKUP_OBJECT (dialog1, menu1, "menu1");
  GLADE_HOOKUP_OBJECT (dialog1, one_pass_cbr1, "one_pass_cbr1");
  GLADE_HOOKUP_OBJECT (dialog1, one_pass_quantizer1, "one_pass_quantizer1");
  GLADE_HOOKUP_OBJECT (dialog1, two_pass1, "two_pass1");
  GLADE_HOOKUP_OBJECT (dialog1, same_qz_as_input1, "same_qz_as_input1");
  GLADE_HOOKUP_OBJECT (dialog1, entryEntry, "entryEntry");
  GLADE_HOOKUP_OBJECT (dialog1, spinbuttonQuant, "spinbuttonQuant");
  GLADE_HOOKUP_OBJECT (dialog1, label9, "label9");
  GLADE_HOOKUP_OBJECT (dialog1, hbox7, "hbox7");
  GLADE_HOOKUP_OBJECT (dialog1, vbox4, "vbox4");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonInterlaced, "checkbuttonInterlaced");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonGreyScale, "checkbuttonGreyScale");
  GLADE_HOOKUP_OBJECT (dialog1, vbox5, "vbox5");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonCartoon, "checkbuttonCartoon");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonChroma, "checkbuttonChroma");
  GLADE_HOOKUP_OBJECT (dialog1, vbox23, "vbox23");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonTurbo, "checkbuttonTurbo");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonPacked, "checkbuttonPacked");
  GLADE_HOOKUP_OBJECT (dialog1, label8, "label8");
  GLADE_HOOKUP_OBJECT (dialog1, vbox6, "vbox6");
  GLADE_HOOKUP_OBJECT (dialog1, hbox8, "hbox8");
  GLADE_HOOKUP_OBJECT (dialog1, vbox8, "vbox8");
  GLADE_HOOKUP_OBJECT (dialog1, label13, "label13");
  GLADE_HOOKUP_OBJECT (dialog1, label14, "label14");
  GLADE_HOOKUP_OBJECT (dialog1, vbox7, "vbox7");
  GLADE_HOOKUP_OBJECT (dialog1, optionmenuMS, "optionmenuMS");
  GLADE_HOOKUP_OBJECT (dialog1, menu2, "menu2");
  GLADE_HOOKUP_OBJECT (dialog1, _0___none1, "_0___none1");
  GLADE_HOOKUP_OBJECT (dialog1, _1___very_low1, "_1___very_low1");
  GLADE_HOOKUP_OBJECT (dialog1, _2___low2, "_2___low2");
  GLADE_HOOKUP_OBJECT (dialog1, _3___medium1, "_3___medium1");
  GLADE_HOOKUP_OBJECT (dialog1, _4___high1, "_4___high1");
  GLADE_HOOKUP_OBJECT (dialog1, _5___very_high1, "_5___very_high1");
  GLADE_HOOKUP_OBJECT (dialog1, _6___ultra_high1, "_6___ultra_high1");
  GLADE_HOOKUP_OBJECT (dialog1, optionmenuVHQ, "optionmenuVHQ");
  GLADE_HOOKUP_OBJECT (dialog1, menu3, "menu3");
  GLADE_HOOKUP_OBJECT (dialog1, _0___off1, "_0___off1");
  GLADE_HOOKUP_OBJECT (dialog1, _1___mode_decision1, "_1___mode_decision1");
  GLADE_HOOKUP_OBJECT (dialog1, _2___limited_search1, "_2___limited_search1");
  GLADE_HOOKUP_OBJECT (dialog1, _3___medium_search1, "_3___medium_search1");
  GLADE_HOOKUP_OBJECT (dialog1, _4___wide_search1, "_4___wide_search1");
  GLADE_HOOKUP_OBJECT (dialog1, hbox10, "hbox10");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonChromaMotion, "checkbuttonChromaMotion");
  GLADE_HOOKUP_OBJECT (dialog1, checkbutton4MV, "checkbutton4MV");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonHQAC, "checkbuttonHQAC");
  GLADE_HOOKUP_OBJECT (dialog1, hbox9, "hbox9");
  GLADE_HOOKUP_OBJECT (dialog1, label15, "label15");
  GLADE_HOOKUP_OBJECT (dialog1, spinbuttonIMinPeriod, "spinbuttonIMinPeriod");
  GLADE_HOOKUP_OBJECT (dialog1, label16, "label16");
  GLADE_HOOKUP_OBJECT (dialog1, spinbuttonIMaxPeriod, "spinbuttonIMaxPeriod");
  GLADE_HOOKUP_OBJECT (dialog1, frame2, "frame2");
  GLADE_HOOKUP_OBJECT (dialog1, vbox9, "vbox9");
  GLADE_HOOKUP_OBJECT (dialog1, hbox11, "hbox11");
  GLADE_HOOKUP_OBJECT (dialog1, label18, "label18");
  GLADE_HOOKUP_OBJECT (dialog1, spinbuttonBFrame, "spinbuttonBFrame");
  GLADE_HOOKUP_OBJECT (dialog1, hbox12, "hbox12");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonQPel, "checkbuttonQPel");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonGMC, "checkbuttonGMC");
  GLADE_HOOKUP_OBJECT (dialog1, label17, "label17");
  GLADE_HOOKUP_OBJECT (dialog1, label2, "label2");
  GLADE_HOOKUP_OBJECT (dialog1, vbox10, "vbox10");
  GLADE_HOOKUP_OBJECT (dialog1, hbox13, "hbox13");
  GLADE_HOOKUP_OBJECT (dialog1, label19, "label19");
  GLADE_HOOKUP_OBJECT (dialog1, radiobuttonH263, "radiobuttonH263");
  GLADE_HOOKUP_OBJECT (dialog1, radiobuttonMpeg, "radiobuttonMpeg");
  GLADE_HOOKUP_OBJECT (dialog1, checkbuttonTrellis, "checkbuttonTrellis");
  GLADE_HOOKUP_OBJECT (dialog1, frame3, "frame3");
  GLADE_HOOKUP_OBJECT (dialog1, hbox14, "hbox14");
  GLADE_HOOKUP_OBJECT (dialog1, vbox11, "vbox11");
  GLADE_HOOKUP_OBJECT (dialog1, label20, "label20");
  GLADE_HOOKUP_OBJECT (dialog1, label21, "label21");
  GLADE_HOOKUP_OBJECT (dialog1, label22, "label22");
  GLADE_HOOKUP_OBJECT (dialog1, vbox12, "vbox12");
  GLADE_HOOKUP_OBJECT (dialog1, spinbuttonIMin, "spinbuttonIMin");
  GLADE_HOOKUP_OBJECT (dialog1, spinbuttonPMin, "spinbuttonPMin");
  GLADE_HOOKUP_OBJECT (dialog1, spinbuttonBMin, "spinbuttonBMin");
  GLADE_HOOKUP_OBJECT (dialog1, vbox13, "vbox13");
  GLADE_HOOKUP_OBJECT (dialog1, label24, "label24");
  GLADE_HOOKUP_OBJECT (dialog1, label25, "label25");
  GLADE_HOOKUP_OBJECT (dialog1, label26, "label26");
  GLADE_HOOKUP_OBJECT (dialog1, vbox14, "vbox14");
  GLADE_HOOKUP_OBJECT (dialog1, spinbuttonIMax, "spinbuttonIMax");
  GLADE_HOOKUP_OBJECT (dialog1, spinbuttonPMax, "spinbuttonPMax");
  GLADE_HOOKUP_OBJECT (dialog1, spinbuttonBMax, "spinbuttonBMax");
  GLADE_HOOKUP_OBJECT (dialog1, label26, "label26");
  GLADE_HOOKUP_OBJECT (dialog1, label3, "label3");
  GLADE_HOOKUP_OBJECT (dialog1, vbox15, "vbox15");
  GLADE_HOOKUP_OBJECT (dialog1, frame4, "frame4");
  GLADE_HOOKUP_OBJECT (dialog1, vbox16, "vbox16");
  GLADE_HOOKUP_OBJECT (dialog1, hbox, "hbox");
  GLADE_HOOKUP_OBJECT (dialog1, vbox17, "vbox17");
  GLADE_HOOKUP_OBJECT (dialog1, label30, "label30");
  GLADE_HOOKUP_OBJECT (dialog1, label31, "label31");
  GLADE_HOOKUP_OBJECT (dialog1, label32, "label32");
  GLADE_HOOKUP_OBJECT (dialog1, label28, "label28");
  GLADE_HOOKUP_OBJECT (dialog1, label29, "label29");
  GLADE_HOOKUP_OBJECT (dialog1, vbox18, "vbox18");
  GLADE_HOOKUP_OBJECT (dialog1, entryIBoost, "entryIBoost");
  GLADE_HOOKUP_OBJECT (dialog1, entryIInterv, "entryIInterv");
  GLADE_HOOKUP_OBJECT (dialog1, entryIreduction, "entryIreduction");
  GLADE_HOOKUP_OBJECT (dialog1, entryMaxOvrImp, "entryMaxOvrImp");
  GLADE_HOOKUP_OBJECT (dialog1, entryMaxOvrDeg, "entryMaxOvrDeg");
  GLADE_HOOKUP_OBJECT (dialog1, label27, "label27");
  GLADE_HOOKUP_OBJECT (dialog1, frame5, "frame5");
  GLADE_HOOKUP_OBJECT (dialog1, hbox18, "hbox18");
  GLADE_HOOKUP_OBJECT (dialog1, vbox19, "vbox19");
  GLADE_HOOKUP_OBJECT (dialog1, label37, "label37");
  GLADE_HOOKUP_OBJECT (dialog1, label38, "label38");
  GLADE_HOOKUP_OBJECT (dialog1, label39, "label39");
  GLADE_HOOKUP_OBJECT (dialog1, vbox20, "vbox20");
  GLADE_HOOKUP_OBJECT (dialog1, entryHiPass, "entryHiPass");
  GLADE_HOOKUP_OBJECT (dialog1, entryLowPass, "entryLowPass");
  GLADE_HOOKUP_OBJECT (dialog1, entryOvrControl, "entryOvrControl");
  GLADE_HOOKUP_OBJECT (dialog1, label36, "label36");
  GLADE_HOOKUP_OBJECT (dialog1, label4, "label4");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (dialog1, okbutton1, "okbutton1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, tooltips, "tooltips");

  return dialog1;
}


#endif
