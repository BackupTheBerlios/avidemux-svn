/***************************************************************************
  FAC_bitrate.cpp
  Handle dialog factory element : Bitrate (encoding mode)
  (C) 2006 Mean Fixounet@free.fr 
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ADM_plugin/ADM_plugin_translate.h"
#include "ADM_toolkitGtk.h"
#include "DIA_factory.h"

namespace ADM_GtkFactory
{
struct diaElemBitrateData
{
	GtkWidget *label1, *label2, *combo, *spin;
	COMPRES_PARAMS *param;
	unsigned int maxQ, minQ;
};

class diaElemBitrate : public diaElemBitrateBase
{
protected:
public:
  
  diaElemBitrate(COMPRES_PARAMS *p,const char *toggleTitle,const char *tip=NULL);
  virtual ~diaElemBitrate() ;
  void setMe(void *dialog, void *opaque,uint32_t line);
  void getMe(void);
  void setMaxQz(uint32_t qz);
  void setMinQz(uint32_t qz);
  void updateMe(void);
  int getRequiredLayout(void);
};

static void cb_mod(GtkWidget *widget, gpointer *data);
/**
 * 	\fn 	readPullDown
 * \brief 	Convert the raw read of the combox into the actual compression mode
 */
static COMPRESSION_MODE readPulldown(COMPRES_PARAMS *copy,int rank)
{
	int index=0;
	COMPRESSION_MODE mode=COMPRESS_MAX;
	
#undef LOOKUP
#define LOOKUP(A,B) \
  if(copy->capabilities & ADM_ENC_CAP_##A) \
  { \
	  if(rank==index) mode=COMPRESS_##B; \
	  index++; \
  } 
  
  LOOKUP(CBR,CBR);
  LOOKUP(CQ,CQ);
  LOOKUP(SAME,SAME);
  LOOKUP(AQ,AQ);
  LOOKUP(2PASS,2PASS);
  LOOKUP(2PASS_BR,2PASS_BITRATE);
  
	ADM_assert(mode!=COMPRESS_MAX);
	return mode;
}

static void updatePulldown(COMPRES_PARAMS *copy, GtkComboBox *combo)
{
	int index = 0, set = -1;

#undef LOOKUP
#define LOOKUP(A,B) \
	if(copy->capabilities & ADM_ENC_CAP_##A) \
	{ \
		if (copy->mode == COMPRESS_##B) set = index; \
			index++; \
	} \

	LOOKUP(CBR, CBR);
	LOOKUP(CQ, CQ);
	LOOKUP(SAME, SAME);
	LOOKUP(AQ, AQ);
	LOOKUP(2PASS, 2PASS);
	LOOKUP(2PASS_BR, 2PASS_BITRATE);

	if (set != -1)
		gtk_combo_box_set_active(combo, set);
}

 diaElemBitrate::diaElemBitrate(COMPRES_PARAMS *p,const char *toggleTitle,const char *tip)
  : diaElemBitrateBase()
{
  param=(void *)p;
  memcpy(&copy,p,sizeof(copy));
  paramTitle=toggleTitle;
  this->tip=tip;
  setSize(2);
  minQ=2;
  maxQ=31;
}

void diaElemBitrate::setMinQz(uint32_t qz)
{
  minQ=qz;
}

void diaElemBitrate::setMaxQz(uint32_t qz)
{
  maxQ=qz; 
}
diaElemBitrate::~diaElemBitrate()
{
	diaElemBitrateData *data = (diaElemBitrateData*)myWidget;

	delete data;
	myWidget = NULL;
}
/**
 * \fn setMe
 * @param dialog  Pointer to father dialog
 * @param opaque  Internal, Gtk table to attach to
 * @param line Line were the widget should be displayed
 */
void diaElemBitrate::setMe(void *dialog, void *opaque,uint32_t line)
{
  GtkObject *adj;
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *combo;
  GtkWidget *spin;
  
#define PUT_ARRAY(x,y,widget)  gtk_table_attach (GTK_TABLE (opaque), widget, x, x+1, line+y, line+y+1, \
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), \
                    (GtkAttachOptions) (0), 0, 0);
  
  /* Add text -> encoding mode */
  label1 = gtk_label_new_with_mnemonic (QT_TR_NOOP("_Encoding mode:"));
  gtk_misc_set_alignment (GTK_MISC (label1), 0.0, 0.5);
  gtk_widget_show(label1);
  
  /* put entry in hbox */
 
  PUT_ARRAY(0,0,label1);
  
  
  /* Add text -> encoding mode */
  label2 = gtk_label_new_with_mnemonic (QT_TR_NOOP("_Bitrate (kb/s):"));
  gtk_misc_set_alignment (GTK_MISC (label2), 0.0, 0.5);
  gtk_widget_show(label2);
  /* put entry in hbox */
  PUT_ARRAY(0,1,label2);
 
  /* Add encoding menu combo */
  
  
  combo = gtk_combo_box_new_text ();
  gtk_widget_show (combo);
  
  gtk_label_set_mnemonic_widget (GTK_LABEL(label1), combo);
  if((copy.capabilities & ADM_ENC_CAP_CBR)) 
	  gtk_combo_box_append_text (GTK_COMBO_BOX (combo),QT_TR_NOOP("Single pass - bitrate"));
  if((copy.capabilities & ADM_ENC_CAP_CQ))
	  gtk_combo_box_append_text (GTK_COMBO_BOX (combo),QT_TR_NOOP("Single pass - constant quality"));
  if((copy.capabilities & ADM_ENC_CAP_SAME))
	  gtk_combo_box_append_text (GTK_COMBO_BOX (combo),QT_TR_NOOP("Single pass - same qz as input"));
  if((copy.capabilities & ADM_ENC_CAP_AQ))
	  gtk_combo_box_append_text (GTK_COMBO_BOX (combo),QT_TR_NOOP("Single pass - Average quantiser"));

  if((copy.capabilities & ADM_ENC_CAP_2PASS))
	  gtk_combo_box_append_text (GTK_COMBO_BOX (combo),QT_TR_NOOP("Two pass - video size"));
  if((copy.capabilities & ADM_ENC_CAP_2PASS_BR))
	  gtk_combo_box_append_text (GTK_COMBO_BOX (combo),QT_TR_NOOP("Two pass - average bitrate"));
  
  /**/
  
   
  
  PUT_ARRAY(1,0,combo);
  
  
  /* Now add value */
  spin = gtk_spin_button_new_with_range(0,1,1);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON(spin),TRUE);
  gtk_spin_button_set_digits  (GTK_SPIN_BUTTON(spin),0);
  
  gtk_widget_show (spin);
  
    PUT_ARRAY(1,1,spin);
  /*  add button */
   gtk_label_set_mnemonic_widget (GTK_LABEL(label1), combo);
   gtk_label_set_mnemonic_widget (GTK_LABEL(label2), spin);

   diaElemBitrateData *data = new diaElemBitrateData;

   data->param = &copy;
   data->label1 = label1;
   data->label2 = label2;
   data->combo = combo;
   data->spin = spin;
   data->maxQ = maxQ;
   data->minQ = minQ;

   myWidget = (void*)data;

   gtk_signal_connect(GTK_OBJECT(data->combo), "changed",
	   G_CALLBACK(cb_mod), (void *)data);

   updatePulldown(&copy, GTK_COMBO_BOX(combo));
}

void diaElemBitrate::getMe(void)
{
  // Read current value
  diaElemBitrateData *data = (diaElemBitrateData*)myWidget;
  int rank = gtk_combo_box_get_active(GTK_COMBO_BOX(data->combo));
  data->param->mode = readPulldown(data->param,rank);

#undef P
#undef M
#undef S
#define P(x) 
#define M(x,y)
#define S(x)   x=(uint32_t)gtk_spin_button_get_value  (GTK_SPIN_BUTTON(data->spin))
  switch(data->param->mode)
  {
    case COMPRESS_CBR: //CBR
          P(_Bitrate (kb/s):);
          M(0,20000);
          S(data->param->bitrate);
          break;
    case COMPRESS_AQ:// CQ
          P(_Quantizer:);
          M(2,31);
          S(data->param->qz);
          break;
    case COMPRESS_CQ:// CQ
          P(_Quantizer:);
          M(2,31);
          S(data->param->qz);
          break;
    case  COMPRESS_2PASS: // 2pass Filesize
          P(_Video size (MB):);
          M(1,8000);
          S(data->param->finalsize);
          break;
    case COMPRESS_2PASS_BITRATE : // 2pass Avg
          P(_Average bitrate (kb/s):);
          M(0,20000);
          S(data->param->avg_bitrate);
          break;
    case COMPRESS_SAME : // Same Qz as input
          P(-);
          M(0,0);
          break;
    default:
		ADM_assert(0);
  }

  memcpy(param, data->param, sizeof(COMPRES_PARAMS));
}

int diaElemBitrate::getRequiredLayout(void) { return 0; }

void updateCombo(diaElemBitrateData *data)
{
#undef M
#undef S
#define M(x,y) gtk_spin_button_set_range  (GTK_SPIN_BUTTON(data->spin),x,y)
#define S(x)   gtk_spin_button_set_value  (GTK_SPIN_BUTTON(data->spin),x)

  updatePulldown(data->param, GTK_COMBO_BOX(data->combo));

  switch (data->param->mode)
  {
    case COMPRESS_CBR:
          gtk_label_set_text_with_mnemonic(GTK_LABEL(data->label2),QT_TR_NOOP("_Bitrate (kb/s):"));
          M(0,20000);
          S(data->param->bitrate);
          break; 
    case COMPRESS_CQ:
          gtk_label_set_text_with_mnemonic(GTK_LABEL(data->label2),QT_TR_NOOP("_Quantiser:"));
          M(data->minQ,data->maxQ);
          S(data->param->qz);
          break;
    case COMPRESS_AQ:
          gtk_label_set_text_with_mnemonic(GTK_LABEL(data->label2),QT_TR_NOOP("A_vg Quantiser:"));
          M(2,64);
          S(data->param->qz);
          break;
    case COMPRESS_2PASS:
          gtk_label_set_text_with_mnemonic(GTK_LABEL(data->label2),QT_TR_NOOP("_Video size (MB):"));
          M(1,8000);
          S(data->param->finalsize);
          break;
    case COMPRESS_2PASS_BITRATE:
          gtk_label_set_text_with_mnemonic(GTK_LABEL(data->label2),QT_TR_NOOP("_Average bitrate (kb/s):"));
          M(0,20000);
          S(data->param->avg_bitrate);
          break;
    case COMPRESS_SAME : // Same Qz as input
          gtk_label_set_text_with_mnemonic(GTK_LABEL(data->label2),QT_TR_NOOP("-"));
          M(0,0);
          break;
    default:
		ADM_assert(0);
  }
}

void diaElemBitrate::updateMe(void)
{
	diaElemBitrateData *data = (diaElemBitrateData*)myWidget;

	memcpy(data->param, param, sizeof(COMPRES_PARAMS));
	updateCombo(data);
}

void cb_mod(GtkWidget *widget, gpointer *d)
{
	diaElemBitrateData *data = (diaElemBitrateData*)d;

	data->param->mode = readPulldown(data->param, gtk_combo_box_get_active(GTK_COMBO_BOX(data->combo)));
	updateCombo(data);
}

} // End of namespace
//****************************Hoook*****************

diaElem  *gtkCreateBitrate(COMPRES_PARAMS *p,const char *toggleTitle,const char *tip)
{
	return new  ADM_GtkFactory::diaElemBitrate(p,toggleTitle,tip);
}
void gtkDestroyBitrate(diaElem *e)
{
	ADM_GtkFactory::diaElemBitrate *a=(ADM_GtkFactory::diaElemBitrate *)e;
	delete a;
}
//EOF
