/***************************************************************************
                          adm_encffmmatrix.g  -  description
                             -------------------

	Defined mpeg1/2 custom matrices

    begin                : Tue Sep 1 2003
    copyright            : (C) 2002 by mean
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
#define ADM_MATRIX_DEFAULT 	0
#define ADM_MATRIX_TMP	     	1
#define ADM_MATRIX_ANIME	     	2
#define ADM_MATRIX_KVCD 		3
#define ADM_MATRIX_HRTMP	4

 static uint16_t tmpgenc_intra[64]={
		 8,16,19,22,26,27,29,34,
		16,16,22,24,27,29,34,37,
		19,22,26,27,29,34,34,38,
		22,22,26,27,29,34,37,40,
		22,26,27,29,32,35,40,48,
		26,27,29,32,35,40,48,58,
		26,27,29,34,38,46,56,69,
		27,29,35,38,46,56,69,83
 };
 static uint16_t tmpgenc_inter[64]={
		16,17,18,19,20,21,22,23, //0
		17,18,19,20,21,22,23,24,
		18,19,20,21,22,23,24,25,
		19,20,21,22,23,24,26,27,
		20,21,22,23,25,26,27,28, //4
		21,22,23,24,26,27,28,30,
		22,23,24,26,27,28,30,31,
		23,24,25,27,28,30,31,33
 };
static uint16_t anime_intra[64]={
		  8,32,32,32,32,32,32,32, //0
		 32,32,32,32,32,32,32,32,
		 32,32,32,32,32,32,32,32,
		 32,32,32,32,32,32,32,32,
		 32,32,32,32,32,32,32,32, //4
		 32,32,32,32,32,32,32,32,
		 32,32,32,32,32,32,32,32,
		 32,32,32,32,32,32,32,32
};
static uint16_t anime_inter[64]={
		16,16,16,16,16,16,16,16, //0
		16,16,16,16,16,16,16,16,
		16,16,16,16,16,16,16,16,
		16,16,16,16,16,16,16,16,
		16,16,16,16,16,16,16,16, //4
		16,16,16,16,16,16,16,16,
		16,16,16,16,16,16,16,16,
		16,16,16,16,16,16,16,16,
};
//----------------------------------------------
static uint16_t kvcd_intra[64]={
		  8, 9,12,22,26,27,29,34, //0
		  9,10,14,26,27,29,37,37,
		 12,14,18,27,29,34,37,38,
		 22,26,27,31,36,37,38,40,

		 26,27,29,36,39,38,40,48, //4
		 27,29,34,37,38,40,48,58,
		 29,34,37,38,40,48,58,69,
		 34,37,38,40,48,58,69,79
};
static uint16_t kvcd_inter[64]={
		16,18,20,22,24,26,28,30, //0
		18,20,22,24,26,28,30,32, //0
		20,22,24,26,28,30,32,34, //0
		22,24,26,28,30,32,34,36, //0

		24,26,28,30,32,34,36,38, //0
		26,28,30,32,34,36,38,40, //0
		28,30,32,34,36,38,40,42, //0
		30,32,34,36,38,40,42,44 //0
};
//----------------------------------------------
static uint16_t hrtmpgenc_intra[64]={
// High resolution INTRA table
		8,16,18,20,24,25,26,30,
		16,16,20,23,25,26,30,30,
		18,20,22,24,26,28,29,31,
		20,21,23,24,26,28,31,31,
		21,23,24,25,28,30,30,33,
		23,24,25,28,30,30,33,36,
		24,25,26,29,29,31,34,38,
		25,26,28,29,31,34,38,42
};
static uint16_t hrtmpgenc_inter[64]={
// TMPEGEnc INTER table
		16,17,18,19,20,21,22,23,
		17,18,19,20,21,22,23,24,
		18,19,20,21,22,23,24,25,
		19,20,21,22,23,24,26,27,
		20,21,22,23,25,26,27,28,
		21,22,23,24,26,27,28,30,
		22,23,24,26,27,28,30,31,
		23,24,25,27,28,30,31,33
};

// EOF
