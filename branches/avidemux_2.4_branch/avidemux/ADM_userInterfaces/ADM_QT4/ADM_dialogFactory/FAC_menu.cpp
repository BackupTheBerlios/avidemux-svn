/***************************************************************************
  FAC_toggle.cpp
  Handle dialog factory element : Toggle
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

#include <config.h>


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>

#include "default.h"
#include "ADM_commonUI/DIA_factory.h"
#include "ADM_assert.h"

#include <QCheckBox>


extern const char *shortkey(const char *);


diaElemMenu::diaElemMenu(uint32_t *intValue,const char *itle, uint32_t nb, 
               const diaMenuEntry *menu,const char *tip)
  : diaElem(ELEM_MENU)
{
  param=(void *)intValue;
  paramTitle=itle;
  this->tip=tip;
  this->menu=menu;
  this->nbMenu=nb;
  nbLink=0;
  
  menus=new diaMenuEntryDynamic * [nb];
  for(int i=0;i<nb;i++)
  {
    menus[i]=new  diaMenuEntryDynamic(menu[i].val,menu[i].text,menu[i].desc);
  }
  dyna=new diaElemMenuDynamic(intValue,itle,nb,menus,tip);
}

diaElemMenu::~diaElemMenu()
{
  for(int i=0;i<nbMenu;i++)
      delete menus[i];
  delete [] menus;
  
}
void diaElemMenu::setMe(void *dialog, void *opaque,uint32_t line)
{
  dyna->setMe(dialog,opaque,line);
}

void diaElemMenu::getMe(void)
{
 dyna->getMe();
}
void   diaElemMenu::updateMe(void)
{
  dyna->updateMe();
}
uint8_t   diaElemMenu::link(diaMenuEntry *entry,uint32_t onoff,diaElem *w)
{
    for(int i=0;i<nbMenu;i++)
    {
        if(entry->val==menus[i]->val)
            return dyna->link(menus[i],onoff,w);
    }
    ADM_assert(0);
}
void   diaElemMenu::enable(uint32_t onoff)
{
  
    dyna->enable(onoff);
}
void   diaElemMenu::finalize(void)
{
  dyna->finalize();
}
//*********************************

diaElemMenuDynamic::diaElemMenuDynamic(uint32_t *intValue,const char *itle, uint32_t nb, 
                diaMenuEntryDynamic **menu,const char *tip)
  : diaElem(ELEM_MENU)
{
  param=(void *)intValue;
  paramTitle=shortkey(itle);
  this->tip=tip;
  this->menu=menu;
  this->nbMenu=nb;
  nbLink=0;
}

diaElemMenuDynamic::~diaElemMenuDynamic()
{
  if(paramTitle)
    delete paramTitle;
}
void diaElemMenuDynamic::setMe(void *dialog, void *opaque,uint32_t line)
{
  QComboBox *combo=new QComboBox( (QWidget *)dialog);
  QGridLayout *layout=(QGridLayout*) opaque;
     myWidget=(void *)combo; 

  QLabel *text=new QLabel( this->paramTitle,(QWidget *)dialog);
  
  // Fill in combobox
  int mem=0;
  diaMenuEntryDynamic **entries=menu;
  for(int i=0;i<nbMenu;i++)
  {
    combo->addItem(entries[i]->text);
    if( *(uint32_t *)param==entries[i]->val) mem=i;
  }
   combo->setCurrentIndex(mem);
   text->setBuddy(combo);
   layout->addWidget(text,line,0);
   layout->addWidget(combo,line,1);
}

void diaElemMenuDynamic::getMe(void)
{
  QComboBox *combo=(QComboBox *)myWidget;
  int r;
  r=combo->currentIndex();
  if(!nbMenu) return;
  ADM_assert(r<nbMenu);
  *(uint32_t *)param=menu[r]->val;
      
}
void diaElemMenuDynamic::finalize(void)
{ 
   QComboBox *combo=(QComboBox *)myWidget;
  uint32_t val;
  uint32_t rank;
  if(!nbMenu) return;
  ADM_assert(combo);
  
  
  rank=combo->currentIndex();
  ADM_assert(rank<this->nbMenu);
  val=this->menu[rank]->val;
  /* Now search through the linked list to see if something happens ...*/
 
   /* 1 disable everything */
  for(int i=0;i<nbLink;i++)
  {
    dialElemLink *l=&(links[i]);
    if(l->value==val)
    {
      if(!l->onoff)  l->widget->enable(FALSE);
    }else
    {
       if(l->onoff)  l->widget->enable(FALSE);
    }
    
  }
  /* Then enable */
  for(int i=0;i<nbLink;i++)
  {
    dialElemLink *l=&(links[i]);
    if(l->value==val)
    {
      if(l->onoff)  l->widget->enable(TRUE);
    }else
    {
       if(!l->onoff)  l->widget->enable(TRUE);
    }
    
  }
}
void diaElemMenuDynamic::enable(uint32_t onoff)
{ 
   QComboBox *combo=(QComboBox *)myWidget;
  ADM_assert(combo);
  if(onoff)
    combo->setEnabled(true);
  else
    combo->setDisabled(true);
}

uint8_t   diaElemMenuDynamic::link(diaMenuEntryDynamic *entry,uint32_t onoff,diaElem *w)
{
    ADM_assert(nbLink<MENU_MAX_lINK);
    links[nbLink].value=entry->val;
    links[nbLink].onoff=onoff;
    links[nbLink].widget=w;
    nbLink++;
    return 1;
}
void   diaElemMenuDynamic::updateMe(void)
{
  finalize();
}
//EOF
