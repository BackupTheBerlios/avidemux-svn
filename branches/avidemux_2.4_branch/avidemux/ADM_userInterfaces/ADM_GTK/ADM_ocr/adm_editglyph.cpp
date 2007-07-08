/***************************************************************************
    copyright            : (C) 2007 by mean
    email                : fixounet@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "default.h"
#include "gtk/gtk.h"
#include <ADM_assert.h>



#include "ADM_toolkit/toolkit.hxx"
#include "ADM_toolkit/filesel.h"

#include "../ADM_GTK/ADM_toolkit_gtk/toolkit_gtk.h"
#include "../ADM_GTK/ADM_toolkit_gtk/toolkit_gtk_include.h"

#include "ADM_ocr/adm_glyph.h"

static GtkWidget *create_dialog1 (void);
static admGlyph *currentGlyph=NULL;
static GtkWidget *dialog;

static uint8_t loadGlyph(char *name,admGlyph *head,uint32_t *outNb);
static uint8_t saveGlyph(char *name,admGlyph *head,uint32_t nb);
static gboolean glyphDraw( void );
static void glyphUpdate(void );
/**
    \fn DIA_glyphEdit
    \brief Dialog to edit glyph
*/
uint8_t DIA_glyphEdit(void)
{
  char *glyphName;
  admGlyph head(1,1);
  
  
  uint32_t nbGlyph=0;
  uint8_t ret=0;
  // First select a file
  
   GUI_FileSelRead(_("Select GlyphFile to edit"), &glyphName);
  if(!glyphName) return 0;
  
  // Try to load it
  if(!loadGlyph(glyphName,&head,&nbGlyph) || !nbGlyph)
  {
    destroyGlyphTree(&head);
    return 0;
  }
  // Convert the linear glyph to glyph array
  admGlyph *glyphArray[nbGlyph];
  admGlyph *cur=head.next;
  uint32_t idx=0;
  while(cur)
  {
     glyphArray[idx++]=cur;
     cur=cur->next;
  }
  ADM_assert(idx<=nbGlyph);
  nbGlyph=idx;
  // Glyph loaded, here we go
  currentGlyph=head.next;
  dialog=create_dialog1 ();
  gtk_register_dialog(dialog);
  
  // Register callbacks
#define ASSOCIATE(x,y)   gtk_dialog_add_action_widget (GTK_DIALOG (dialog), WID(x),y)
#define CONNECT(x,y,z) 	gtk_signal_connect(GTK_OBJECT(WID(x)), #y,GTK_SIGNAL_FUNC(z),   NULL);
#define ACTION_PREV 10
#define ACTION_NEXT 20
#define ACTION_PREV_EMPTY 30
#define ACTION_NEXT_EMPTY 40
#define ACTION_SAVE 50
#define ACTION_DELETE 60
  
  ASSOCIATE(buttonPrev,ACTION_PREV);
  ASSOCIATE(buttonNext,ACTION_NEXT);
  ASSOCIATE(buttonEmptyPrev,ACTION_PREV_EMPTY);
  ASSOCIATE(buttonNextEmpty,ACTION_NEXT_EMPTY);
  ASSOCIATE(buttonSave,ACTION_SAVE);
  ASSOCIATE(buttonDelete,ACTION_DELETE);
  
  CONNECT(drawingarea1,expose_event,glyphDraw);
  gtk_widget_show(dialog);
  glyphUpdate();
  while(1)
  {
   gint b=gtk_dialog_run(GTK_DIALOG(dialog));
    switch(b)
    {
      case ACTION_PREV: 
                        printf("PREV\n");
                        if(currentGlyph!=head.next)
                        {
                           admGlyph *father;
                           father=glyphSearchFather(currentGlyph,&head);
                           if(father)
                           {
                               currentGlyph=father;
                               glyphUpdate();
                           }
                          
                        }
                        continue;break;
      case ACTION_NEXT: 
                        printf("NEXT\n");
                        if(currentGlyph->next)
                        {
                          currentGlyph=currentGlyph->next; 
                          glyphUpdate();
                        }
                        continue;break; 
      case ACTION_NEXT_EMPTY: 
                        printf("NEXT EMPTY\n");
                        while(1)
                        {
                            if(currentGlyph->next)
                            {
                              currentGlyph=currentGlyph->next; 
                              glyphUpdate();
                              if(!currentGlyph->code || !*(currentGlyph->code))
                              {
                                break;
                              }
                            }
                            else 
                            {
                              GUI_Error_HIG(_("End reached"),_("No more glyphs"));
                              break;
                            }
                        }
                        continue;break; 
        case ACTION_PREV_EMPTY: 
                        printf("PREV EMPTY\n");
                        while(1)
                        {
                            if(currentGlyph!=head.next)
                            {
                              admGlyph *father;
                              father=glyphSearchFather(currentGlyph,&head);
                              if(father)
                              {
                                  currentGlyph=father;
                                  glyphUpdate();
                                   if(!currentGlyph->code || !*(currentGlyph->code))
                                    {
                                      break;
                                    }
                                  continue;
                              } 
                            } 
                            GUI_Error_HIG(_("Head reached"),_("No more glyphs"));
                            break;
                        }
                        continue;break;
      case ACTION_SAVE:
                    saveGlyph(glyphName,&head,nbGlyph);
                    continue;break;
      case ACTION_DELETE:
                  {
                      admGlyph *father;
                      father=glyphSearchFather(currentGlyph,&head);
                      ADM_assert(father);
                      father->next=currentGlyph->next;
                      delete currentGlyph;
                      
                      currentGlyph=father;
                        if(father==&head && head.next)
                          currentGlyph=head.next;
                      nbGlyph--;
                      glyphUpdate();
                      continue;break; 
                  }
    }
    break; // exit while(1)
  }
  
  gtk_unregister_dialog(dialog);
  gtk_widget_destroy(dialog);
  
  destroyGlyphTree(&head);
  return ret;

}
/**
    \fn       glyphSave
    \brief    Save the glypset
*/
uint8_t saveGlyph(char *name,admGlyph *head,uint32_t nb)
{
  FILE *out;
  uint32_t slen;
    
  admGlyph *glyph=head->next;
    
    
  out=fopen(name,"wb");
  if(!out)
  {
    GUI_Error_HIG(_("Could not write the file"), NULL);
    return 0;
  }
#define WRITE(x) fwrite(&(x),sizeof(x),1,out);
    WRITE(nb);
    
    while(glyph)
    {
      WRITE(glyph->width);
      WRITE(glyph->height);
      fwrite(glyph->data,glyph->width*glyph->height,1,out);
      if(glyph->code) slen=strlen(glyph->code);
      else slen=0;
      WRITE(slen);
      fwrite(glyph->code,slen,1,out);
      glyph=glyph->next;
    }
    
    fclose(out);
    return 1;
  
}
/**
    \fn glyphUpdate
    \brief Update all fields in the dialog wrt currentGlyph
*/
void glyphUpdate(void )
{
  if(!currentGlyph) return;
  gtk_widget_set_usize(WID(drawingarea1), currentGlyph->width+4, currentGlyph->height+4);
  glyphDraw();
  gtk_editable_delete_text(GTK_EDITABLE(WID(entry1)), 0,-1); 
  if(currentGlyph->code)
  {
    // Set our text
    gtk_entry_set_text (GTK_ENTRY (WID(entry1)), currentGlyph->code);
  }
}
/**
    \fn     glyphDraw
    \brief  display glyph
*/
gboolean glyphDraw( void )
{
    if(!currentGlyph) return true;
    gdk_draw_gray_image(WID(drawingarea1)->window, WID(drawingarea1)->style->fg_gc[GTK_STATE_SELECTED /*GTK_STATE_NORMAL*/],
                        2,                          // X
                        2,                          // y
                        currentGlyph->width,                          //width
                        currentGlyph->height,                          //h*2, // heigth
                        GDK_RGB_DITHER_NONE,
                        currentGlyph->data, // buffer
                        currentGlyph->width );
    return true;
}

/**
    \fn loadGlyph
    \brief Load a glyph set
*/
uint8_t loadGlyph(char *name,admGlyph *head,uint32_t *outNb)
{
  FILE *out;
  admGlyph *glyph,*nw;
  uint32_t N,w,h,slen;
  uint32_t nbGlyphs;
 
  glyph=head;
  out=fopen(name,"rb");
  if(!out)
  {
    GUI_Error_HIG(_("File error"), _("Could not read \"%s\"."), name);
    return 0;
  }
#define READ(x) fread(&(x),sizeof(x),1,out);
    nbGlyphs=0;
    READ(N);
    while(N--)
    {
        
      READ(w);
      READ(h);
      nw=new admGlyph(w,h);
      fread(nw->data,w*h,1,out);
      READ(slen);
      if(slen)
      {
        nw->code=new char[slen+1];
        fread(nw->code,slen,1,out);
        nw->code[slen]=0;
      }
      glyph->next=nw;
      glyph=nw;
      nbGlyphs++;
    }
    
    fclose(out);
    *outNb=nbGlyphs;
    return 1;

}


//**********************************

GtkWidget*
create_dialog1 (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *vbox2;
  GtkWidget *drawingarea1;
  GtkWidget *entry1;
  GtkWidget *table1;
  GtkWidget *buttonNext;
  GtkWidget *buttonPrev;
  GtkWidget *buttonEmptyPrev;
  GtkWidget *alignment2;
  GtkWidget *hbox4;
  GtkWidget *image2;
  GtkWidget *label2;
  GtkWidget *buttonNextEmpty;
  GtkWidget *alignment3;
  GtkWidget *hbox5;
  GtkWidget *image3;
  GtkWidget *label3;
  GtkWidget *buttonDelete;
  GtkWidget *buttonSave;
  GtkWidget *dialog_action_area1;
  GtkWidget *closebutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), _("dialog1"));
  gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox2);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox2, TRUE, TRUE, 0);

  drawingarea1 = gtk_drawing_area_new ();
  gtk_widget_show (drawingarea1);
  gtk_box_pack_start (GTK_BOX (vbox2), drawingarea1, TRUE, TRUE, 0);

  entry1 = gtk_entry_new ();
  gtk_widget_show (entry1);
  gtk_box_pack_start (GTK_BOX (vbox2), entry1, FALSE, FALSE, 0);

  table1 = gtk_table_new (3, 2, FALSE);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (vbox2), table1, TRUE, TRUE, 0);

  buttonNext = gtk_button_new_from_stock ("gtk-media-next");
  gtk_widget_show (buttonNext);
  gtk_table_attach (GTK_TABLE (table1), buttonNext, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK), 0, 0);

  buttonPrev = gtk_button_new_from_stock ("gtk-media-previous");
  gtk_widget_show (buttonPrev);
  gtk_table_attach (GTK_TABLE (table1), buttonPrev, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL), 0, 0);

  buttonEmptyPrev = gtk_button_new ();
  gtk_widget_show (buttonEmptyPrev);
  gtk_table_attach (GTK_TABLE (table1), buttonEmptyPrev, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK), 0, 0);

  alignment2 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (buttonEmptyPrev), alignment2);

  hbox4 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox4);
  gtk_container_add (GTK_CONTAINER (alignment2), hbox4);

  image2 = gtk_image_new_from_stock ("gtk-media-previous", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image2);
  gtk_box_pack_start (GTK_BOX (hbox4), image2, FALSE, FALSE, 0);

  label2 = gtk_label_new_with_mnemonic (_("Prev. Empty"));
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (hbox4), label2, FALSE, FALSE, 0);

  buttonNextEmpty = gtk_button_new ();
  gtk_widget_show (buttonNextEmpty);
  gtk_table_attach (GTK_TABLE (table1), buttonNextEmpty, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK), 0, 0);

  alignment3 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment3);
  gtk_container_add (GTK_CONTAINER (buttonNextEmpty), alignment3);

  hbox5 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox5);
  gtk_container_add (GTK_CONTAINER (alignment3), hbox5);

  image3 = gtk_image_new_from_stock ("gtk-media-next", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image3);
  gtk_box_pack_start (GTK_BOX (hbox5), image3, FALSE, FALSE, 0);

  label3 = gtk_label_new_with_mnemonic (_("Next Empty"));
  gtk_widget_show (label3);
  gtk_box_pack_start (GTK_BOX (hbox5), label3, FALSE, FALSE, 0);

  buttonDelete = gtk_button_new_from_stock ("gtk-delete");
  gtk_widget_show (buttonDelete);
  gtk_table_attach (GTK_TABLE (table1), buttonDelete, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  buttonSave = gtk_button_new_from_stock ("gtk-save");
  gtk_widget_show (buttonSave);
  gtk_table_attach (GTK_TABLE (table1), buttonSave, 1, 2, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  closebutton1 = gtk_button_new_from_stock ("gtk-close");
  gtk_widget_show (closebutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), closebutton1, GTK_RESPONSE_CLOSE);
  GTK_WIDGET_SET_FLAGS (closebutton1, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog1, "dialog1");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT (dialog1, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (dialog1, drawingarea1, "drawingarea1");
  GLADE_HOOKUP_OBJECT (dialog1, entry1, "entry1");
  GLADE_HOOKUP_OBJECT (dialog1, table1, "table1");
  GLADE_HOOKUP_OBJECT (dialog1, buttonNext, "buttonNext");
  GLADE_HOOKUP_OBJECT (dialog1, buttonPrev, "buttonPrev");
  GLADE_HOOKUP_OBJECT (dialog1, buttonEmptyPrev, "buttonEmptyPrev");
  GLADE_HOOKUP_OBJECT (dialog1, alignment2, "alignment2");
  GLADE_HOOKUP_OBJECT (dialog1, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (dialog1, image2, "image2");
  GLADE_HOOKUP_OBJECT (dialog1, label2, "label2");
  GLADE_HOOKUP_OBJECT (dialog1, buttonNextEmpty, "buttonNextEmpty");
  GLADE_HOOKUP_OBJECT (dialog1, alignment3, "alignment3");
  GLADE_HOOKUP_OBJECT (dialog1, hbox5, "hbox5");
  GLADE_HOOKUP_OBJECT (dialog1, image3, "image3");
  GLADE_HOOKUP_OBJECT (dialog1, label3, "label3");
  GLADE_HOOKUP_OBJECT (dialog1, buttonDelete, "buttonDelete");
  GLADE_HOOKUP_OBJECT (dialog1, buttonSave, "buttonSave");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (dialog1, closebutton1, "closebutton1");

  return dialog1;
}

