/***************************************************************************
                          ADM_vidBlend.cpp  -  description
                             -------------------
    begin                : Tue Jan 7 2003
    copyright            : (C) 2003 by mean
    email                : fixounet@free.fr

    Slighlty faster ASM deinterlace
    Blend later

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ADM_assert.h>

#include "config.h"
#include "fourcc.h"
#include "avio.hxx"
#include "config.h"
#include "avi_vars.h"



#include "ADM_toolkit/toolkit.hxx"
#include "ADM_editor/ADM_edit.hxx"
#include "ADM_video/ADM_genvideo.hxx"

#include"ADM_video/ADM_vidField.h"

#include "admmangle.h"


 static int32_t _l_w,_l_h;
 static uint8_t *_l_p,*_l_c,*_l_n;
 static uint8_t *_l_e,*_l_e2;
#define EXPAND(x) (x)+((x)<<16)+((x)<<32) +((x)<<48)
static uint64_t  __attribute__((used)) _mmTHRESH1=EXPAND((uint64_t )THRES1);
static uint64_t  __attribute__((used)) full_ones=0xFFFFFFFFFFFFFFFFLL;


static void myBlendASM(void);

void ADMVideoFields::blend_C(uint8_t *p,uint8_t *c,
							uint8_t *n,
							uint8_t *e,
							uint8_t *f
							)
{
uint32_t x,y;
 for(y=_info.height-2;y>0;y--)
           	{
              for(x=_info.width;x>0;x--)
              {

                    if(*e)
                    	{
                       	*f=(*c>>1) + ((*n+*p)>>2);
                       //	*f=255;

                       }
                       else
                       		*f=*c;
						//			*f=0;

                       c++;n++;p++;e++;f++;
                 }
              }
}
#if defined( ARCH_X86)  || defined(ARCH_X86_64)
void ADMVideoFields::blend_MMX(uint8_t *p,uint8_t *c,
																							uint8_t *n,
																							uint8_t *e,
																							uint8_t *f
																							)
{
      _l_h=_info.height-2;
			_l_w=_info.width>>2;
			_l_p=p;
			_l_c=c;
			_l_n=n;
			_l_e=e;
			_l_e2=f;
			myBlendASM();
}

void myBlendASM(void)
{
	__asm__ (
//"StartASM4: \n\t"
"push 				"REG_di"\n\t"
"push 				"REG_si"\n\t"
"push 				"REG_ax"\n\t"

"mov (%0), "REG_ax" \n\t"
"mov "Mangle(_l_p)", "REG_ax" \n\t"
"mov "Mangle(_l_c)", "REG_bx" \n\t"
"mov "Mangle(_l_n)", "REG_cx" \n\t"
"mov "Mangle(_l_e)", "REG_si" \n\t"
"mov "Mangle(_l_e2)", "REG_di" \n\t"
"movq "Mangle(full_ones)",%%mm7 \n\t"
"pxor	   %%mm6,%%mm6 \n\t"

"DHCOLB%=: \n\t" // loop
"mov "Mangle(_l_w)", "REG_dx" \n\t"                // loop one line

"DHLineB%=:  \n\t"

"movd ("REG_bx"),%%mm0 \n\t"     // mm0 <- c
"movd ("REG_ax"),%%mm1 \n\t"     // mm1 <- p
"movd ("REG_cx"),%%mm2 \n\t"     // mm2 <- n
"movd ("REG_si"),%%mm3 \n\t"     // mm2 <- e


"punpcklbw %%mm6, %%mm0 \n\t"
"punpcklbw %%mm6, %%mm1 \n\t"
"punpcklbw %%mm6, %%mm2 \n\t"
"punpcklbw %%mm6, %%mm3 \n\t"
"movq      %%mm6, %%mm4 \n\t"

//"movq %%mm3,S0 \n\t"

// make 16 bits mask
"psllw     $8, %%mm4 \n\t"
"por	   %%mm4,%%mm3 \n\t"  // mask in m3
//"movq %%mm3,S1 \n\t"
//"movq %%mm2,S2 \n\t"
//"movq %%mm1,S3 \n\t"


// compute average of p & n in mm4
"paddw     %%mm1,%%mm2 \n\t"
"paddw     %%mm0,%%mm2 \n\t"
"paddw     %%mm0,%%mm2 \n\t"
//"movq 	   %%mm2,S4 \n\t"

"psrlw     $2,%%mm2 \n\t"     //mm2 is averafe of p & n & c -> replacement value
//"movq      %%mm2,S5 \n\t"

"pand     %%mm3,%%mm2 \n\t"
//"movq     %%mm2,S6 \n\t"

// compute inverse value
"pxor     %%mm7,%%mm3 \n\t" // inverse mask in m3
//"movq     %%mm3,S7 \n\t"

"pand     %%mm3,%%mm0 \n\t" // right value in m0
//"movq     %%mm0,S8 \n\t"

"por     %%mm2,%%mm0 \n\t" // ok, now right
//"movq     %%mm0,S9 \n\t"

"packuswb  %%mm6, %%mm0 \n\t"          // pack
//"movq %%mm0,S10 \n\t"

"movd	   %%mm0, ("REG_di") \n\t" // store

//
//
// next
"add       $4,"REG_ax" \n\t"
"add       $4,"REG_bx" \n\t"
"add       $4,"REG_cx" \n\t"
"add       $4,"REG_si" \n\t"
"add       $4,"REG_di" \n\t"
"sub       $1,"REG_dx"	\n\t"
"jne        DHLineB%= \n\t"   // next
"sub       $1,"Mangle(_l_h)"  \n\t" // next line
"jne        DHCOLB%= \n\t"
"pop 				"REG_ax"\n\t"
"pop 				"REG_si"\n\t"
"pop 				"REG_di"\n\t"
" emms       \n\t"
 : /* no output */
 :  "r"(&_mmTHRESH1)
 :   "ebx", "ecx", "edx");
}
#endif
