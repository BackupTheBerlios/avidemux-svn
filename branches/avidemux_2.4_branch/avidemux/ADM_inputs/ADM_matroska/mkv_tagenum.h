/***************************************************************************
 *   Copyright (C) 2007 by mean,    *
 *   fixounet@free.fr   *
 *                                                                         *
 *        EBML Handling code                                               *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#if !defined( MKV_TAG_ENUM) || defined(ADM_BUILD_TAG)
#define MKV_TAG_ENUM

#ifndef ADM_BUILD_TAG
#define MKTAG(a,b,c) a=b
#else
#define MKTAG(a,b,c) {a,c,#a}
#endif
  MKTAG(EBML_HEADER,0x1A45DFA3,ADM_MKV_TYPE_CONTAINER),
  MKTAG(EBML_VERSION,0x4286,ADM_MKV_TYPE_UINTEGER),
  MKTAG(EBML_COMPAT,0x42F7,ADM_MKV_TYPE_UINTEGER),
  MKTAG(EBML_MAXIDLEN,0x42F2,ADM_MKV_TYPE_UINTEGER),
  MKTAG(EBML_MAXSIZELEN,0x42F3,ADM_MKV_TYPE_UINTEGER),
  MKTAG(EBML_DOCTYPE,0x4282,ADM_MKV_TYPE_STRING),
  MKTAG(EBML_DOCTYPE_VERSION,0x4287,ADM_MKV_TYPE_UINTEGER),
  MKTAG(EBML_DOCTYPE_COMPAT,0x4285,ADM_MKV_TYPE_UINTEGER),
 
  MKTAG(MKV_SEGMENT, 0x18538067,ADM_MKV_TYPE_CONTAINER),
  MKTAG(MKV_CLUSTER, 0x1f43b675,ADM_MKV_TYPE_CONTAINER),
  
  MKTAG(MKV_SEEK_HEAD,0x114d9b74,ADM_MKV_TYPE_CONTAINER),
  MKTAG(MKV_SEEK,0x4dbb,ADM_MKV_TYPE_CONTAINER),
  MKTAG(MKV_SEEK_POSITION,0x53AC,ADM_MKV_TYPE_UINTEGER),
  MKTAG(MKV_ID,0x53AB,ADM_MKV_TYPE_BINARY),
  
  MKTAG(MKV_INFO,0x1549a966,ADM_MKV_TYPE_CONTAINER),
  
  MKTAG(MKV_CUES,0x1c53bb6b,ADM_MKV_TYPE_CONTAINER),
  
  MKTAG(MKV_MUXING_APP, 0x4D80,ADM_MKV_TYPE_UTF8),
  MKTAG(MKV_WRITING_APP, 0x5741,ADM_MKV_TYPE_UTF8),
  MKTAG(MKV_TITLE, 0x7BA9,ADM_MKV_TYPE_UTF8),
  
  MKTAG(MKV_TIMECODE_SCALE,0x2AD7B1,ADM_MKV_TYPE_UINTEGER),
  
  MKTAG(MKV_TRACKS,0x1654AE6B,ADM_MKV_TYPE_CONTAINER),
  MKTAG(MKV_TRACK_ENTRY,0xAE,ADM_MKV_TYPE_CONTAINER),
  MKTAG(MKV_TRACK_NUMBER,0xD7,ADM_MKV_TYPE_UINTEGER),
  
  MKTAG(MKV_TRACK_TYPE,0x83,ADM_MKV_TYPE_UINTEGER),
  
  MKTAG(MKV_TRACK_TIMECODESCALE, 0x23314F,ADM_MKV_TYPE_FLOAT),
  
  MKTAG(MKV_CODEC_ID,0x86,ADM_MKV_TYPE_STRING),
  
  MKTAG(MKV_CODEC_NAME,0x258688,ADM_MKV_TYPE_UTF8),
   	
  MKTAG(MKV_VIDEO_SETTINGS,0xE0,ADM_MKV_TYPE_CONTAINER),
  MKTAG(MKV_FRAME_DEFAULT_DURATION,0x23E383,ADM_MKV_TYPE_UINTEGER),
  
  MKTAG(MKV_CODEC_EXTRADATA,0x63A2,ADM_MKV_TYPE_BINARY),
  
  MKTAG(MKV_VIDEO_WIDTH,0xB0,ADM_MKV_TYPE_UINTEGER),
  MKTAG(MKV_VIDEO_HEIGHT,0xBA,ADM_MKV_TYPE_UINTEGER),
  MKTAG(MKV_VIDEO_AR,0x54B3,ADM_MKV_TYPE_UINTEGER),

  MKTAG(MKV_AUDIO_SETTINGS,0xE1,ADM_MKV_TYPE_CONTAINER),
  MKTAG(MKV_AUDIO_FREQUENCY,0xB5,ADM_MKV_TYPE_FLOAT),
  MKTAG(MKV_AUDIO_OUT_FREQUENCY,0x78B5,ADM_MKV_TYPE_FLOAT),
  MKTAG(MKV_AUDIO_CHANNELS,0x9F,ADM_MKV_TYPE_UINTEGER),
  MKTAG(MKV_AUDIO_BITS_PER_SAMPLE,0x6264,ADM_MKV_TYPE_UINTEGER),
  
  /***/
  MKTAG(MKV_TIMECODE,0xE7,ADM_MKV_TYPE_UINTEGER),
  MKTAG(MKV_BLOCK_GROUP,0xA0,ADM_MKV_TYPE_CONTAINER),
  MKTAG(MKV_BLOCK,0xA1,ADM_MKV_TYPE_BINARY),
  /** */
  MKTAG(MKV_CUE_POINT,0xBB,ADM_MKV_TYPE_CONTAINER),
  MKTAG(MKV_CUE_TIME,0xB3,ADM_MKV_TYPE_UINTEGER),
  
  MKTAG(MKV_CUE_TRACK_POSITION,0xB7,ADM_MKV_TYPE_CONTAINER),
  MKTAG(MKV_CUE_CLUSTER_POSITION,0xF1,ADM_MKV_TYPE_UINTEGER),
  MKTAG(MKV_CUE_TRACK,0xF7,ADM_MKV_TYPE_UINTEGER),
  MKTAG(MKV_CUE_BLOCK_NUMBER,0x5378,ADM_MKV_TYPE_UINTEGER),
  
   MKTAG(MKV_VOID,0xEC,ADM_MKV_TYPE_BINARY),
  
         //*************************
   MKTAG(MKV_MAX,0xFFFF,ADM_MKV_TYPE_UNKNOWN)    
       
#undef MKTAG
#endif
