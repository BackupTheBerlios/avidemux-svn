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



#include <string.h>
#include <stdio.h>
#include <math.h>

#include "ADM_default.h"

#include "DIA_factory.h"
#include "ADM_assert.h"




diaElemToggle::diaElemToggle(uint32_t *toggleValue,const char *toggleTitle, const char *tip)
  : diaElemToggleBase()
{
}

diaElemToggle::~diaElemToggle()
{
  
}
void diaElemToggle::setMe(void *dialog, void *opaque,uint32_t l)
{
}
void diaElemToggle::getMe(void)
{
}
void diaElemToggle::enable(uint32_t onoff) {}
void   diaElemToggle::finalize(void)
{
  updateMe(); 
}
void   diaElemToggle::updateMe(void)
{

}

int diaElemToggle::getRequiredLayout(void) { return 0; }

//******************************************************
diaElemInteger::diaElemInteger(int32_t *intValue,const char *toggleTitle, int32_t min, int32_t max,const char *tip)
  : diaElem(ELEM_TOGGLE)
{
 }

diaElemInteger::~diaElemInteger()
{
  
}
void diaElemInteger::setMe(void *dialog, void *opaque,uint32_t line)
{
 
}
void diaElemInteger::getMe(void)
{
 
}
uint8_t   diaElemToggle::link(uint32_t onoff,diaElem *w)
{
  
}
void diaElemInteger::enable(uint32_t onoff) {}
int diaElemInteger::getRequiredLayout(void) { return 0; }
void diaElemInteger::updateMe(void) {}

//******************************************************
diaElemUInteger::diaElemUInteger(uint32_t *intValue,const char *toggleTitle, uint32_t min, uint32_t max,const char *tip)
  : diaElem(ELEM_TOGGLE)
{
 }

diaElemUInteger::~diaElemUInteger()
{
  
}
void diaElemUInteger::setMe(void *dialog, void *opaque,uint32_t line)
{
 
}
void diaElemUInteger::getMe(void)
{
 
}
void diaElemUInteger::enable(uint32_t onoff) {}
int diaElemUInteger::getRequiredLayout(void) { return 0; }
void diaElemUInteger::updateMe(void) {}

//******************************************************

diaElemFloat::diaElemFloat(ELEM_TYPE_FLOAT *intValue,const char *toggleTitle, 
                            ELEM_TYPE_FLOAT min, ELEM_TYPE_FLOAT max,const char *tip, int decimals)
  : diaElem(ELEM_FLOAT)
{
}

diaElemFloat::~diaElemFloat()
{
  
}
void diaElemFloat::setMe(void *dialog, void *opaque,uint32_t line)
{
  
}
void diaElemFloat::getMe(void)
{
 
}
void diaElemFloat::enable(uint32_t onoff) {}
int diaElemFloat::getRequiredLayout(void) { return 0; }
void diaElemFloat::updateMe(void) {}

//******************************************************

diaElemBar::diaElemBar(uint32_t percent,const char *toggleTitle)
  : diaElem(ELEM_BAR)
{
}

diaElemBar::~diaElemBar()
{
  
}
void diaElemBar::setMe(void *dialog, void *opaque,uint32_t line)
{
  
}
void diaElemBar::getMe(void)
{
 
}

int diaElemBar::getRequiredLayout(void) { return 0; }
void diaElemBar::updateMe(void) {}

//******************************************************

diaElemMenu::diaElemMenu(uint32_t *intValue,const char *itle, uint32_t nb, 
               const diaMenuEntry *menu,const char *tip)
  : diaElemMenuBase()
{
}

diaElemMenu::~diaElemMenu()
{
  
}
void diaElemMenu::setMe(void *dialog, void *opaque,uint32_t line)
{
  
}
void diaElemMenu::getMe(void)
{
 
}
uint8_t   diaElemMenu::link(diaMenuEntry *entry,uint32_t onoff,diaElem *w)
{
}
void   diaElemMenu::enable(uint32_t onoff)
{}
void diaElemMenu::finalize(void)
{ 
}

int diaElemMenu::getRequiredLayout(void) { return 0; }

void   diaElemMenu::updateMe(void)
{
}
//*****************
diaElemFile::diaElemFile(uint32_t write,char **filename,const char *toggleTitle,
                         const char *defaultSuffix, const char *tip)
  : diaElemFileBase()
{
 
}

diaElemFile::~diaElemFile()
{
  
}
void diaElemFile::setMe(void *dialog, void *opaque,uint32_t line)
{
 
  
}
void diaElemFile::getMe(void)
{
 
}
void   diaElemFile::enable(uint32_t onoff) {}
int diaElemFile::getRequiredLayout(void) { return 0; }
void diaElemFile::updateMe(void) {}

#include "ADM_encoder/ADM_vidEncode.hxx"
  diaElemBitrate::diaElemBitrate(COMPRES_PARAMS *p,const char *toggleTitle,const char *tip)
  : diaElemBitrateBase()
{
  
}

diaElemBitrate::~diaElemBitrate()
{
  
}
void diaElemBitrate::setMe(void *dialog, void *opaque,uint32_t line)
{
 
  
}
void diaElemBitrate::getMe(void)
{
 
}

int diaElemBitrate::getSize(void)
{ 
}

void diaElemBitrate::setSize(int size)
{ 
}

int diaElemBitrate::getRequiredLayout(void) { return 0; }

void diaElemFile::changeFile(void)
{
}
void diaElemBitrate::setMaxQz(uint32_t qz)
{
}
void diaElemBitrate::updateMe(void) {}
//******************************************************
diaElemReadOnlyText::diaElemReadOnlyText(const char *readyOnly,const char *toggleTitle,const char *tip)
  : diaElem(ELEM_ROTEXT)
{
 
} 
diaElemReadOnlyText::~diaElemReadOnlyText()
{
  
}
void diaElemReadOnlyText::setMe(void *dialog, void *opaque,uint32_t line)
{
 
  
}
void diaElemReadOnlyText::getMe(void)
{
 
}

int diaElemReadOnlyText::getRequiredLayout(void) { return 0; }
void diaElemReadOnlyText::updateMe(void) {}

//******************************************************
diaElemNotch::diaElemNotch(uint32_t yes,const char *toggleTitle, const char *tip)
  : diaElem(ELEM_NOTCH)
{
 
}

diaElemNotch::~diaElemNotch()
{
  
}
void diaElemNotch::setMe(void *dialog, void *opaque,uint32_t line)
{
  
}
 void diaElemNotch::getMe(void) {};

int diaElemNotch::getRequiredLayout(void) { return 0; }
void diaElemNotch::updateMe(void) {}

//******************************************************
diaElemDirSelect::diaElemDirSelect(char **filename,const char *toggleTitle,const char *tip)  : diaElemDirSelectBase() {}
diaElemDirSelect::~diaElemDirSelect() {}
void diaElemDirSelect::setMe(void *dialog, void *opaque,uint32_t line) {}
void diaElemDirSelect::getMe(void) {}
  
void diaElemDirSelect::changeFile(void) {}
void   diaElemDirSelect::enable(uint32_t onoff){}
int diaElemDirSelect::getRequiredLayout(void) { return 0; }
void diaElemDirSelect::updateMe(void) {}
//******************************************************
diaElemText::diaElemText(char **text,const char *toggleTitle,const char *tip)
    : diaElem(ELEM_NOTCH) {}
diaElemText::~diaElemText() {}
void diaElemText::setMe(void *dialog, void *opaque,uint32_t line) {}
void diaElemText::getMe(void) {}  
void diaElemText::enable(uint32_t onoff) {};
int diaElemText::getRequiredLayout(void) { return 0; }
void diaElemText::updateMe(void) {}
//******************************************************
diaElemMenuDynamic::diaElemMenuDynamic(uint32_t *intValue,const char *itle, uint32_t nb, 
                diaMenuEntryDynamic **menu,const char *tip)
  : diaElemMenuDynamicBase()
{
}

diaElemMenuDynamic::~diaElemMenuDynamic()
{ 
}
void      diaElemMenuDynamic::updateMe(void)
{

}
void diaElemMenuDynamic::setMe(void *dialog, void *opaque,uint32_t line)
{
}

void diaElemMenuDynamic::getMe(void)
{ 
}
uint8_t   diaElemMenuDynamic::link(diaMenuEntryDynamic *entry,uint32_t onoff,diaElem *w)
{
}
void   diaElemMenuDynamic::enable(uint32_t onoff)
{}
void diaElemMenuDynamic::finalize(void)
{ 
}
int diaElemMenuDynamic::getRequiredLayout(void) { return 0; }

//******************************************************
diaElemFrame::diaElemFrame(const char *toggleTitle, const char *tip)
  : diaElemFrameBase()
{
  
}
void diaElemFrame::swallow(diaElem *widget)
{
 
}
diaElemFrame::~diaElemFrame()
{
  
}
void diaElemFrame::setMe(void *dialog, void *opaque,uint32_t line)
{
 
}
void diaElemFrame::getMe(void)
{
  
}
void diaElemFrame::enable(uint32_t onoff)
{

}
void diaElemFrame::finalize(void)
{
}

int diaElemFrame::getRequiredLayout(void) { return 0; }
void diaElemFrame::updateMe(void) {}
//**************************

  
  diaElemHex::diaElemHex(const char *toggleTitle, uint32_t dataSize,uint8_t *data) :diaElem(ELEM_HEXDUMP){};
  diaElemHex::~diaElemHex() {};
  void diaElemHex::setMe(void *dialog, void *opaque,uint32_t line) {};
  void diaElemHex::getMe(void) {} ;
  void diaElemHex::finalize(void) {};
  int diaElemHex::getRequiredLayout(void) { return 0; }
  void diaElemHex::updateMe(void) {}
//**************************
diaElemToggleUint::diaElemToggleUint(uint32_t *toggleValue,const char *toggleTitle, uint32_t *uintval, const char *name,uint32_t min,uint32_t max,const char *tip)
  : diaElem(ELEM_TOGGLE_UINT)
{
 
}
diaElemToggleUint::~diaElemToggleUint()
{
  
}
void diaElemToggleUint::setMe(void *dialog, void *opaque,uint32_t line)
{
 
  
}
void diaElemToggleUint::getMe(void)
{
  
}
void   diaElemToggleUint::finalize(void)
{
  updateMe();
}
void   diaElemToggleUint::updateMe(void)
{
  
    
}
void   diaElemToggleUint::enable(uint32_t onoff)
{
   
}

int diaElemToggleUint::getRequiredLayout(void) { return 0; }

//**************************
diaElemToggleInt::diaElemToggleInt(uint32_t *toggleValue,const char *toggleTitle, int32_t *uintval, const char *name,int32_t min,int32_t max,const char *tip)
  :diaElem(ELEM_TOGGLE_INT)
{
 
}
diaElemToggleInt::~diaElemToggleInt()
{
  
}
void diaElemToggleInt::setMe(void *dialog, void *opaque,uint32_t line)
{
 
  
}
void diaElemToggleInt::getMe(void)
{
  
}

int diaElemToggleInt::getRequiredLayout(void) { return 0; }

void   diaElemToggleInt::finalize(void)
{
}
void   diaElemToggleInt::enable(uint32_t onoff)
{
}
void diaElemToggleInt::updateMe(void) {}
//*********************************************************
diaElemButton:: diaElemButton(const char *toggleTitle, ADM_FAC_CALLBACK *cb,void *cookie,const char *tip)
  : diaElem(ELEM_BUTTON)
{
}

diaElemButton::~diaElemButton()
{
  
}

void diaElemButton::setMe(void *dialog, void *opaque,uint32_t line)
{
  
}
void diaElemButton::getMe(void)
{
  
}
void   diaElemButton::enable(uint32_t onoff)
{

}

int diaElemButton::getRequiredLayout(void) { return 0; }
void diaElemButton::updateMe(void) {}

//***
#if 0
 template <typename T>
 diaElemGenericSlider<T>::diaElemGenericSlider(T *value,const char *toggleTitle, T min,T max,T incr,const char *tip)
     : diaElem(ELEM_SLIDER)
  {
 }
  
 template <typename T>
 diaElemGenericSlider<T>::~diaElemGenericSlider()
  {
  }
 
 template <typename T>
 void diaElemGenericSlider<T>::setMe(void *dialog, void *opaque,uint32_t line)
  {
  }
 
 template <typename T>
 void diaElemGenericSlider<T>::getMe(void)
  {
  }
  
 template <typename T>
 void diaElemGenericSlider<T>::enable(uint32_t onoff) 
  {
  }
  
template <typename T>
int diaElemGenericSlider<T>::getRequiredLayout(void) { return 0; }

 template class diaElemGenericSlider <int32_t>;
 template class diaElemGenericSlider <uint32_t>;
 template class diaElemGenericSlider <ELEM_TYPE_FLOAT>;
//****
#endif


 diaElemMatrix::diaElemMatrix(uint8_t *trix,const char *toggleTitle, uint32_t trixSize,const char *tip)
   : diaElem(ELEM_MATRIX)
 {
 }

 diaElemMatrix::~diaElemMatrix()
 {
 }
 void diaElemMatrix::setMe(void *dialog, void *opaque,uint32_t line)
 {

 }
 void diaElemMatrix::getMe(void)
 {
   
 }
 void diaElemMatrix::enable(uint32_t onoff)
 {
 }

int diaElemMatrix::getRequiredLayout(void) { return 0; }
void diaElemMatrix::updateMe(void) {}

//***
diaElemThreadCount::diaElemThreadCount(uint32_t *value, const char *title, const char *tip) : diaElem(ELEM_THREAD_COUNT) {}
diaElemThreadCount::~diaElemThreadCount() {}
void diaElemThreadCount::setMe(void *dialog, void *opaque, uint32_t line) {}
void diaElemThreadCount::getMe(void) {}
int diaElemThreadCount::getRequiredLayout(void) { return 0; }
void diaElemThreadCount::updateMe(void) {}

//EOF
