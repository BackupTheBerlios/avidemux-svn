/***************************************************************************
                          gtkgui.h  -  description
                             -------------------
    begin                : Mon Dec 10 2001
    copyright            : (C) 2001 by mean
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

int 			A_openAvi		(char *name);
void 			A_saveAudio	(char *name);
void 			A_saveAudioDecoded	(char *name);
void 			A_saveAVI		(char *name);
void 			A_playAvi		(void);
void 			update_status_bar(ADMImage *image );
void 			rebuild_status_bar(void );



void GUI_NextFrame( void );
void GUI_NextKeyFrame( void ) ;
void GUI_NextPrevBlackFrame( int ) ;
void GUI_PreviousKeyFrame( void );
uint8_t A_ListAllBlackFrames( char *name);
void GUI_PlayAvi( void );
uint32_t GUI_GetScale( void );
void GUI_detransient(void );
void GUI_retransient(void );

typedef enum pipID
{
	P_TOOLAME,
	P_LAME,
	P_OTHER
};
 void A_Pipe(pipID who,char *file=NULL );

//EOF

