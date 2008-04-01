/***************************************************************************
                          DIA_coreUI_internal.h
  Handles univeral dialog
  (C) Mean 2006 fixounet@free.fr

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef DIA_COREUI_INTERNAL_H
#define DIA_COREUI_INTERNAL_H
#include "DIA_enter.h"
#include "DIA_fileSel.h"
#include "DIA_factory.h"
 
// Dia enter
typedef struct
{
	DIA_FILE_INIT	  *fileInit;
	DIA_FILE_SEL_CB   *fileReadCb;
	DIA_FILE_SEL_CB   *fileWriteCb;
	DIA_FILE_SEL_NAME *fileReadName;
	DIA_FILE_SEL_NAME *fileWriteName;
	DIA_FILE_SELECT   *fileSelectRead;
	DIA_FILE_SELECT   *fileSelectWrite;
	DIA_FILE_SELECT   *fileSelectDirectory;
}DIA_FILESEL_DESC_T;

// Call this to hook your fileSelector functions
uint8_t DIA_fileSelInit(DIA_FILESEL_DESC_T *d);


//
typedef uint8_t DIA_FACTORY_RUN_TABS(const char *title,uint32_t nb,diaElemTabs **tabs);
typedef uint8_t DIA_FACTORY_RUN(const char *title,uint32_t nb,diaElem **elems);
typedef struct
{
	COREUI_GET_VERSION   *FactoryGetVersion;
	DIA_FACTORY_RUN      *FactoryRun;
	DIA_FACTORY_RUN_TABS *FactoryRunTab;
//	
	CREATE_BUTTON_T 	 *CreareButton;
	DELETE_DIA_ELEM_T    *DestroyButton;
//
	CREATE_BAR_T		 *CreateBar;
	DELETE_DIA_ELEM_T    *DestroyBar;
// Float
	CREATE_FLOAT_T       *CreateFloat;
	DELETE_DIA_ELEM_T    *DestroyFloat;
// Integer	
	CREATE_INTEGER_T     *CreateInteger;
	DELETE_DIA_ELEM_T    *DestroyInteger;
// UInteger	
	CREATE_UINTEGER_T    *CreateUInteger;
	DELETE_DIA_ELEM_T    *DestroyUInteger;
// Notch	
	CREATE_NOTCH_T       *CreateNotch;
	DELETE_DIA_ELEM_T    *DestroyNotch;
// ReadonlyText
	CREATE_READONLYTEXT_T *CreateReadonlyText;
	DELETE_DIA_ELEM_T     *DestroyReadonlyText;
// Text
	CREATE_TEXT_T        *CreateText;
	DELETE_DIA_ELEM_T    *DestroyText;
// Hex
	CREATE_HEX_T         *CreateHex;
	DELETE_DIA_ELEM_T    *DestroyHex;
// Matrix
	CREATE_MATRIX_T      *CreateMatrix;
	DELETE_DIA_ELEM_T    *DestroyMatrix;
// Menu
	CREATE_MENU_T        *CreateMenu;
	DELETE_DIA_ELEM_T    *DestroyMenu;
// MenuDynamic
	CREATE_MENUDYNAMIC_T *CreateMenuDynamic;
	DELETE_DIA_ELEM_T    *DestroyMenuDynamic;
// ThreadCount
	CREATE_THREADCOUNT_T *CreateThreadCount;
	DELETE_DIA_ELEM_T    *DestroyThreadCount;
// Bitrate
	CREATE_BITRATE_T     *CreateBitrate;
	DELETE_DIA_ELEM_T    *DestroyBitrate;
// File
	CREATE_FILE_T        *CreateFile;
	DELETE_DIA_ELEM_T    *DestroyFile;
// Dir
	CREATE_DIR_T         *CreateDir;
	DELETE_DIA_ELEM_T    *DestroyDir;
// Frame
	CREATE_FRAME_T       *CreateFrame;
	DELETE_DIA_ELEM_T    *DestroyFrame;
// Toggle uint
	CREATE_TOGGLE_UINT   *CreateToggleUint;
	DELETE_DIA_ELEM_T    *DestroyToggleUint;
// Toggle int
	CREATE_TOGGLE_INT    *CreateToggleInt;
	DELETE_DIA_ELEM_T    *DestroyToggleInt;
// Regular Toggle	
	CREATE_TOGGLE_T      *CreateToggle;
	DELETE_DIA_ELEM_T    *DestroyToggle;
// Uslider
	CREATE_USLIDER_T     *CreateUSlider;
	DELETE_DIA_ELEM_T    *DestroyUSlider;
// Slider
	CREATE_SLIDER_T      *CreateSlider;
	DELETE_DIA_ELEM_T    *DestroySlider;
}FactoryDescriptor;
//
uint8_t DIA_factoryInit(FactoryDescriptor *d);
#endif
