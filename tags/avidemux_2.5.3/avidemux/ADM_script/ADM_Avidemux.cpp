/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define JSDECLARE
#include "ADM_Avidemux.h"

ADM_Avidemux::ADM_Avidemux(void) : m_pContainer(NULL), m_nCurrentFrame(0), m_dFPS(0)
{
	m_pAudio = ADM_JSAvidemuxAudio::JSInit(g_pCx, g_pObject);
	m_pVideo = ADM_JSAvidemuxVideo::JSInit(g_pCx, g_pObject);
}

ADM_Avidemux::~ADM_Avidemux()
{

}
