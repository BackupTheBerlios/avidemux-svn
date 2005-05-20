/*
    Copyright (C) 2001-2003 Michael Niedermayer <michaelni@gmx.at>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#undef REAL_MOVNTQ
#undef MOVNTQ
#undef PAVGB
#undef PREFETCH
#undef PREFETCHW
#undef EMMS
#undef SFENCE

#ifdef HAVE_3DNOW
/* On K6 femms is faster of emms. On K7 femms is directly mapped on emms. */
#define EMMS     "femms"
#else
#define EMMS     "emms"
#endif

#ifdef HAVE_3DNOW
#define PREFETCH  "prefetch"
#define PREFETCHW "prefetchw"
#elif defined ( HAVE_MMX2 )
#define PREFETCH "prefetchnta"
#define PREFETCHW "prefetcht0"
#else
#define PREFETCH "/nop"
#define PREFETCHW "/nop"
#endif

#ifdef HAVE_MMX2
#define SFENCE "sfence"
#else
#define SFENCE "/nop"
#endif

#ifdef HAVE_MMX2
#define PAVGB(a,b) "pavgb " #a ", " #b " \n\t"
#elif defined (HAVE_3DNOW)
#define PAVGB(a,b) "pavgusb " #a ", " #b " \n\t"
#endif

#ifdef HAVE_MMX2
#define REAL_MOVNTQ(a,b) "movntq " #a ", " #b " \n\t"
#else
#define REAL_MOVNTQ(a,b) "movq " #a ", " #b " \n\t"
#endif
#define MOVNTQ(a,b)  REAL_MOVNTQ(a,b)

#ifdef HAVE_ALTIVEC
#include "swscale_altivec_template.c"
#endif

#define YSCALEYUV2YV12X(x, offset) \
			"xor %%"REG_a", %%"REG_a"	\n\t"\
			"movq "VROUNDER_OFFSET"(%0), %%mm3\n\t"\
			"movq %%mm3, %%mm4		\n\t"\
			"lea " offset "(%0), %%"REG_d"	\n\t"\
			"mov (%%"REG_d"), %%"REG_S"	\n\t"\
			".balign 16			\n\t" /* FIXME Unroll? */\
			"1:				\n\t"\
			"movq 8(%%"REG_d"), %%mm0	\n\t" /* filterCoeff */\
			"movq " #x "(%%"REG_S", %%"REG_a", 2), %%mm2\n\t" /* srcData */\
			"movq 8+" #x "(%%"REG_S", %%"REG_a", 2), %%mm5\n\t" /* srcData */\
			"add $16, %%"REG_d"		\n\t"\
			"mov (%%"REG_d"), %%"REG_S"	\n\t"\
			"test %%"REG_S", %%"REG_S"	\n\t"\
			"pmulhw %%mm0, %%mm2		\n\t"\
			"pmulhw %%mm0, %%mm5		\n\t"\
			"paddw %%mm2, %%mm3		\n\t"\
			"paddw %%mm5, %%mm4		\n\t"\
			" jnz 1b			\n\t"\
			"psraw $3, %%mm3		\n\t"\
			"psraw $3, %%mm4		\n\t"\
			"packuswb %%mm4, %%mm3		\n\t"\
			MOVNTQ(%%mm3, (%1, %%REGa))\
			"add $8, %%"REG_a"		\n\t"\
			"cmp %2, %%"REG_a"		\n\t"\
			"movq "VROUNDER_OFFSET"(%0), %%mm3\n\t"\
			"movq %%mm3, %%mm4		\n\t"\
			"lea " offset "(%0), %%"REG_d"	\n\t"\
			"mov (%%"REG_d"), %%"REG_S"	\n\t"\
			"jb 1b				\n\t"

#define YSCALEYUV2YV121 \
			"mov %2, %%"REG_a"		\n\t"\
			".balign 16			\n\t" /* FIXME Unroll? */\
			"1:				\n\t"\
			"movq (%0, %%"REG_a", 2), %%mm0	\n\t"\
			"movq 8(%0, %%"REG_a", 2), %%mm1\n\t"\
			"psraw $7, %%mm0		\n\t"\
			"psraw $7, %%mm1		\n\t"\
			"packuswb %%mm1, %%mm0		\n\t"\
			MOVNTQ(%%mm0, (%1, %%REGa))\
			"add $8, %%"REG_a"		\n\t"\
			"jnc 1b				\n\t"

/*
			:: "m" (-lumFilterSize), "m" (-chrFilterSize),
			   "m" (lumMmxFilter+lumFilterSize*4), "m" (chrMmxFilter+chrFilterSize*4),
			   "r" (dest), "m" (dstW),
			   "m" (lumSrc+lumFilterSize), "m" (chrSrc+chrFilterSize)
			: "%eax", "%ebx", "%ecx", "%edx", "%esi"
*/
#define YSCALEYUV2PACKEDX \
		"xor %%"REG_a", %%"REG_a"	\n\t"\
		".balign 16			\n\t"\
		"nop				\n\t"\
		"1:				\n\t"\
		"lea "CHR_MMX_FILTER_OFFSET"(%0), %%"REG_d"\n\t"\
		"mov (%%"REG_d"), %%"REG_S"	\n\t"\
		"movq "VROUNDER_OFFSET"(%0), %%mm3\n\t"\
		"movq %%mm3, %%mm4		\n\t"\
		".balign 16			\n\t"\
		"2:				\n\t"\
		"movq 8(%%"REG_d"), %%mm0	\n\t" /* filterCoeff */\
		"movq (%%"REG_S", %%"REG_a"), %%mm2	\n\t" /* UsrcData */\
		"movq 4096(%%"REG_S", %%"REG_a"), %%mm5	\n\t" /* VsrcData */\
		"add $16, %%"REG_d"		\n\t"\
		"mov (%%"REG_d"), %%"REG_S"	\n\t"\
		"pmulhw %%mm0, %%mm2		\n\t"\
		"pmulhw %%mm0, %%mm5		\n\t"\
		"paddw %%mm2, %%mm3		\n\t"\
		"paddw %%mm5, %%mm4		\n\t"\
		"test %%"REG_S", %%"REG_S"	\n\t"\
		" jnz 2b			\n\t"\
\
		"lea "LUM_MMX_FILTER_OFFSET"(%0), %%"REG_d"\n\t"\
		"mov (%%"REG_d"), %%"REG_S"	\n\t"\
		"movq "VROUNDER_OFFSET"(%0), %%mm1\n\t"\
		"movq %%mm1, %%mm7		\n\t"\
		".balign 16			\n\t"\
		"2:				\n\t"\
		"movq 8(%%"REG_d"), %%mm0	\n\t" /* filterCoeff */\
		"movq (%%"REG_S", %%"REG_a", 2), %%mm2	\n\t" /* Y1srcData */\
		"movq 8(%%"REG_S", %%"REG_a", 2), %%mm5	\n\t" /* Y2srcData */\
		"add $16, %%"REG_d"		\n\t"\
		"mov (%%"REG_d"), %%"REG_S"	\n\t"\
		"pmulhw %%mm0, %%mm2		\n\t"\
		"pmulhw %%mm0, %%mm5		\n\t"\
		"paddw %%mm2, %%mm1		\n\t"\
		"paddw %%mm5, %%mm7		\n\t"\
		"test %%"REG_S", %%"REG_S"	\n\t"\
		" jnz 2b			\n\t"\


#define YSCALEYUV2RGBX \
		YSCALEYUV2PACKEDX\
		"psubw "U_OFFSET"(%0), %%mm3	\n\t" /* (U-128)8*/\
		"psubw "V_OFFSET"(%0), %%mm4	\n\t" /* (V-128)8*/\
		"movq %%mm3, %%mm2		\n\t" /* (U-128)8*/\
		"movq %%mm4, %%mm5		\n\t" /* (V-128)8*/\
		"pmulhw "UG_COEFF"(%0), %%mm3	\n\t"\
		"pmulhw "VG_COEFF"(%0), %%mm4	\n\t"\
	/* mm2=(U-128)8, mm3=ug, mm4=vg mm5=(V-128)8 */\
		"pmulhw "UB_COEFF"(%0), %%mm2	\n\t"\
		"pmulhw "VR_COEFF"(%0), %%mm5	\n\t"\
		"psubw "Y_OFFSET"(%0), %%mm1	\n\t" /* 8(Y-16)*/\
		"psubw "Y_OFFSET"(%0), %%mm7	\n\t" /* 8(Y-16)*/\
		"pmulhw "Y_COEFF"(%0), %%mm1	\n\t"\
		"pmulhw "Y_COEFF"(%0), %%mm7	\n\t"\
	/* mm1= Y1, mm2=ub, mm3=ug, mm4=vg mm5=vr, mm7=Y2 */\
		"paddw %%mm3, %%mm4		\n\t"\
		"movq %%mm2, %%mm0		\n\t"\
		"movq %%mm5, %%mm6		\n\t"\
		"movq %%mm4, %%mm3		\n\t"\
		"punpcklwd %%mm2, %%mm2		\n\t"\
		"punpcklwd %%mm5, %%mm5		\n\t"\
		"punpcklwd %%mm4, %%mm4		\n\t"\
		"paddw %%mm1, %%mm2		\n\t"\
		"paddw %%mm1, %%mm5		\n\t"\
		"paddw %%mm1, %%mm4		\n\t"\
		"punpckhwd %%mm0, %%mm0		\n\t"\
		"punpckhwd %%mm6, %%mm6		\n\t"\
		"punpckhwd %%mm3, %%mm3		\n\t"\
		"paddw %%mm7, %%mm0		\n\t"\
		"paddw %%mm7, %%mm6		\n\t"\
		"paddw %%mm7, %%mm3		\n\t"\
		/* mm0=B1, mm2=B2, mm3=G2, mm4=G1, mm5=R1, mm6=R2 */\
		"packuswb %%mm0, %%mm2		\n\t"\
		"packuswb %%mm6, %%mm5		\n\t"\
		"packuswb %%mm3, %%mm4		\n\t"\
		"pxor %%mm7, %%mm7		\n\t"
#if 0
#define FULL_YSCALEYUV2RGB \
		"pxor %%mm7, %%mm7		\n\t"\
		"movd %6, %%mm6			\n\t" /*yalpha1*/\
		"punpcklwd %%mm6, %%mm6		\n\t"\
		"punpcklwd %%mm6, %%mm6		\n\t"\
		"movd %7, %%mm5			\n\t" /*uvalpha1*/\
		"punpcklwd %%mm5, %%mm5		\n\t"\
		"punpcklwd %%mm5, %%mm5		\n\t"\
		"xor %%"REG_a", %%"REG_a"		\n\t"\
		".balign 16			\n\t"\
		"1:				\n\t"\
		"movq (%0, %%"REG_a", 2), %%mm0	\n\t" /*buf0[eax]*/\
		"movq (%1, %%"REG_a", 2), %%mm1	\n\t" /*buf1[eax]*/\
		"movq (%2, %%"REG_a",2), %%mm2	\n\t" /* uvbuf0[eax]*/\
		"movq (%3, %%"REG_a",2), %%mm3	\n\t" /* uvbuf1[eax]*/\
		"psubw %%mm1, %%mm0		\n\t" /* buf0[eax] - buf1[eax]*/\
		"psubw %%mm3, %%mm2		\n\t" /* uvbuf0[eax] - uvbuf1[eax]*/\
		"pmulhw %%mm6, %%mm0		\n\t" /* (buf0[eax] - buf1[eax])yalpha1>>16*/\
		"pmulhw %%mm5, %%mm2		\n\t" /* (uvbuf0[eax] - uvbuf1[eax])uvalpha1>>16*/\
		"psraw $4, %%mm1		\n\t" /* buf0[eax] - buf1[eax] >>4*/\
		"movq 4096(%2, %%"REG_a",2), %%mm4	\n\t" /* uvbuf0[eax+2048]*/\
		"psraw $4, %%mm3		\n\t" /* uvbuf0[eax] - uvbuf1[eax] >>4*/\
		"paddw %%mm0, %%mm1		\n\t" /* buf0[eax]yalpha1 + buf1[eax](1-yalpha1) >>16*/\
		"movq 4096(%3, %%"REG_a",2), %%mm0	\n\t" /* uvbuf1[eax+2048]*/\
		"paddw %%mm2, %%mm3		\n\t" /* uvbuf0[eax]uvalpha1 - uvbuf1[eax](1-uvalpha1)*/\
		"psubw %%mm0, %%mm4		\n\t" /* uvbuf0[eax+2048] - uvbuf1[eax+2048]*/\
		"psubw "MANGLE(w80)", %%mm1	\n\t" /* 8(Y-16)*/\
		"psubw "MANGLE(w400)", %%mm3	\n\t" /* 8(U-128)*/\
		"pmulhw "MANGLE(yCoeff)", %%mm1	\n\t"\
\
\
		"pmulhw %%mm5, %%mm4		\n\t" /* (uvbuf0[eax+2048] - uvbuf1[eax+2048])uvalpha1>>16*/\
		"movq %%mm3, %%mm2		\n\t" /* (U-128)8*/\
		"pmulhw "MANGLE(ubCoeff)", %%mm3\n\t"\
		"psraw $4, %%mm0		\n\t" /* uvbuf0[eax+2048] - uvbuf1[eax+2048] >>4*/\
		"pmulhw "MANGLE(ugCoeff)", %%mm2\n\t"\
		"paddw %%mm4, %%mm0		\n\t" /* uvbuf0[eax+2048]uvalpha1 - uvbuf1[eax+2048](1-uvalpha1)*/\
		"psubw "MANGLE(w400)", %%mm0	\n\t" /* (V-128)8*/\
\
\
		"movq %%mm0, %%mm4		\n\t" /* (V-128)8*/\
		"pmulhw "MANGLE(vrCoeff)", %%mm0\n\t"\
		"pmulhw "MANGLE(vgCoeff)", %%mm4\n\t"\
		"paddw %%mm1, %%mm3		\n\t" /* B*/\
		"paddw %%mm1, %%mm0		\n\t" /* R*/\
		"packuswb %%mm3, %%mm3		\n\t"\
\
		"packuswb %%mm0, %%mm0		\n\t"\
		"paddw %%mm4, %%mm2		\n\t"\
		"paddw %%mm2, %%mm1		\n\t" /* G*/\
\
		"packuswb %%mm1, %%mm1		\n\t"
#endif

#define REAL_YSCALEYUV2PACKED(index, c) \
		"movq "CHR_MMX_FILTER_OFFSET"+8("#c"), %%mm0\n\t"\
		"movq "LUM_MMX_FILTER_OFFSET"+8("#c"), %%mm1\n\t"\
		"psraw $3, %%mm0		\n\t"\
		"psraw $3, %%mm1		\n\t"\
		"movq %%mm0, "CHR_MMX_FILTER_OFFSET"+8("#c")\n\t"\
		"movq %%mm1, "LUM_MMX_FILTER_OFFSET"+8("#c")\n\t"\
		"xor "#index", "#index"		\n\t"\
		".balign 16			\n\t"\
		"1:				\n\t"\
		"movq (%2, "#index"), %%mm2	\n\t" /* uvbuf0[eax]*/\
		"movq (%3, "#index"), %%mm3	\n\t" /* uvbuf1[eax]*/\
		"movq 4096(%2, "#index"), %%mm5	\n\t" /* uvbuf0[eax+2048]*/\
		"movq 4096(%3, "#index"), %%mm4	\n\t" /* uvbuf1[eax+2048]*/\
		"psubw %%mm3, %%mm2		\n\t" /* uvbuf0[eax] - uvbuf1[eax]*/\
		"psubw %%mm4, %%mm5		\n\t" /* uvbuf0[eax+2048] - uvbuf1[eax+2048]*/\
		"movq "CHR_MMX_FILTER_OFFSET"+8("#c"), %%mm0\n\t"\
		"pmulhw %%mm0, %%mm2		\n\t" /* (uvbuf0[eax] - uvbuf1[eax])uvalpha1>>16*/\
		"pmulhw %%mm0, %%mm5		\n\t" /* (uvbuf0[eax+2048] - uvbuf1[eax+2048])uvalpha1>>16*/\
		"psraw $7, %%mm3		\n\t" /* uvbuf0[eax] - uvbuf1[eax] >>4*/\
		"psraw $7, %%mm4		\n\t" /* uvbuf0[eax+2048] - uvbuf1[eax+2048] >>4*/\
		"paddw %%mm2, %%mm3		\n\t" /* uvbuf0[eax]uvalpha1 - uvbuf1[eax](1-uvalpha1)*/\
		"paddw %%mm5, %%mm4		\n\t" /* uvbuf0[eax+2048]uvalpha1 - uvbuf1[eax+2048](1-uvalpha1)*/\
		"movq (%0, "#index", 2), %%mm0	\n\t" /*buf0[eax]*/\
		"movq (%1, "#index", 2), %%mm1	\n\t" /*buf1[eax]*/\
		"movq 8(%0, "#index", 2), %%mm6	\n\t" /*buf0[eax]*/\
		"movq 8(%1, "#index", 2), %%mm7	\n\t" /*buf1[eax]*/\
		"psubw %%mm1, %%mm0		\n\t" /* buf0[eax] - buf1[eax]*/\
		"psubw %%mm7, %%mm6		\n\t" /* buf0[eax] - buf1[eax]*/\
		"pmulhw "LUM_MMX_FILTER_OFFSET"+8("#c"), %%mm0\n\t" /* (buf0[eax] - buf1[eax])yalpha1>>16*/\
		"pmulhw "LUM_MMX_FILTER_OFFSET"+8("#c"), %%mm6\n\t" /* (buf0[eax] - buf1[eax])yalpha1>>16*/\
		"psraw $7, %%mm1		\n\t" /* buf0[eax] - buf1[eax] >>4*/\
		"psraw $7, %%mm7		\n\t" /* buf0[eax] - buf1[eax] >>4*/\
		"paddw %%mm0, %%mm1		\n\t" /* buf0[eax]yalpha1 + buf1[eax](1-yalpha1) >>16*/\
		"paddw %%mm6, %%mm7		\n\t" /* buf0[eax]yalpha1 + buf1[eax](1-yalpha1) >>16*/\
                
#define YSCALEYUV2PACKED(index, c)  REAL_YSCALEYUV2PACKED(index, c)
                
#define REAL_YSCALEYUV2RGB(index, c) \
		"xor "#index", "#index"	\n\t"\
		".balign 16			\n\t"\
		"1:				\n\t"\
		"movq (%2, "#index"), %%mm2	\n\t" /* uvbuf0[eax]*/\
		"movq (%3, "#index"), %%mm3	\n\t" /* uvbuf1[eax]*/\
		"movq 4096(%2, "#index"), %%mm5\n\t" /* uvbuf0[eax+2048]*/\
		"movq 4096(%3, "#index"), %%mm4\n\t" /* uvbuf1[eax+2048]*/\
		"psubw %%mm3, %%mm2		\n\t" /* uvbuf0[eax] - uvbuf1[eax]*/\
		"psubw %%mm4, %%mm5		\n\t" /* uvbuf0[eax+2048] - uvbuf1[eax+2048]*/\
		"movq "CHR_MMX_FILTER_OFFSET"+8("#c"), %%mm0\n\t"\
		"pmulhw %%mm0, %%mm2		\n\t" /* (uvbuf0[eax] - uvbuf1[eax])uvalpha1>>16*/\
		"pmulhw %%mm0, %%mm5		\n\t" /* (uvbuf0[eax+2048] - uvbuf1[eax+2048])uvalpha1>>16*/\
		"psraw $4, %%mm3		\n\t" /* uvbuf0[eax] - uvbuf1[eax] >>4*/\
		"psraw $4, %%mm4		\n\t" /* uvbuf0[eax+2048] - uvbuf1[eax+2048] >>4*/\
		"paddw %%mm2, %%mm3		\n\t" /* uvbuf0[eax]uvalpha1 - uvbuf1[eax](1-uvalpha1)*/\
		"paddw %%mm5, %%mm4		\n\t" /* uvbuf0[eax+2048]uvalpha1 - uvbuf1[eax+2048](1-uvalpha1)*/\
		"psubw "U_OFFSET"("#c"), %%mm3	\n\t" /* (U-128)8*/\
		"psubw "V_OFFSET"("#c"), %%mm4	\n\t" /* (V-128)8*/\
		"movq %%mm3, %%mm2		\n\t" /* (U-128)8*/\
		"movq %%mm4, %%mm5		\n\t" /* (V-128)8*/\
		"pmulhw "UG_COEFF"("#c"), %%mm3\n\t"\
		"pmulhw "VG_COEFF"("#c"), %%mm4\n\t"\
	/* mm2=(U-128)8, mm3=ug, mm4=vg mm5=(V-128)8 */\
		"movq (%0, "#index", 2), %%mm0	\n\t" /*buf0[eax]*/\
		"movq (%1, "#index", 2), %%mm1	\n\t" /*buf1[eax]*/\
		"movq 8(%0, "#index", 2), %%mm6\n\t" /*buf0[eax]*/\
		"movq 8(%1, "#index", 2), %%mm7\n\t" /*buf1[eax]*/\
		"psubw %%mm1, %%mm0		\n\t" /* buf0[eax] - buf1[eax]*/\
		"psubw %%mm7, %%mm6		\n\t" /* buf0[eax] - buf1[eax]*/\
		"pmulhw "LUM_MMX_FILTER_OFFSET"+8("#c"), %%mm0\n\t" /* (buf0[eax] - buf1[eax])yalpha1>>16*/\
		"pmulhw "LUM_MMX_FILTER_OFFSET"+8("#c"), %%mm6\n\t" /* (buf0[eax] - buf1[eax])yalpha1>>16*/\
		"psraw $4, %%mm1		\n\t" /* buf0[eax] - buf1[eax] >>4*/\
		"psraw $4, %%mm7		\n\t" /* buf0[eax] - buf1[eax] >>4*/\
		"paddw %%mm0, %%mm1		\n\t" /* buf0[eax]yalpha1 + buf1[eax](1-yalpha1) >>16*/\
		"paddw %%mm6, %%mm7		\n\t" /* buf0[eax]yalpha1 + buf1[eax](1-yalpha1) >>16*/\
		"pmulhw "UB_COEFF"("#c"), %%mm2\n\t"\
		"pmulhw "VR_COEFF"("#c"), %%mm5\n\t"\
		"psubw "Y_OFFSET"("#c"), %%mm1	\n\t" /* 8(Y-16)*/\
		"psubw "Y_OFFSET"("#c"), %%mm7	\n\t" /* 8(Y-16)*/\
		"pmulhw "Y_COEFF"("#c"), %%mm1	\n\t"\
		"pmulhw "Y_COEFF"("#c"), %%mm7	\n\t"\
	/* mm1= Y1, mm2=ub, mm3=ug, mm4=vg mm5=vr, mm7=Y2 */\
		"paddw %%mm3, %%mm4		\n\t"\
		"movq %%mm2, %%mm0		\n\t"\
		"movq %%mm5, %%mm6		\n\t"\
		"movq %%mm4, %%mm3		\n\t"\
		"punpcklwd %%mm2, %%mm2		\n\t"\
		"punpcklwd %%mm5, %%mm5		\n\t"\
		"punpcklwd %%mm4, %%mm4		\n\t"\
		"paddw %%mm1, %%mm2		\n\t"\
		"paddw %%mm1, %%mm5		\n\t"\
		"paddw %%mm1, %%mm4		\n\t"\
		"punpckhwd %%mm0, %%mm0		\n\t"\
		"punpckhwd %%mm6, %%mm6		\n\t"\
		"punpckhwd %%mm3, %%mm3		\n\t"\
		"paddw %%mm7, %%mm0		\n\t"\
		"paddw %%mm7, %%mm6		\n\t"\
		"paddw %%mm7, %%mm3		\n\t"\
		/* mm0=B1, mm2=B2, mm3=G2, mm4=G1, mm5=R1, mm6=R2 */\
		"packuswb %%mm0, %%mm2		\n\t"\
		"packuswb %%mm6, %%mm5		\n\t"\
		"packuswb %%mm3, %%mm4		\n\t"\
		"pxor %%mm7, %%mm7		\n\t"
#define YSCALEYUV2RGB(index, c)  REAL_YSCALEYUV2RGB(index, c)
                
#define REAL_YSCALEYUV2PACKED1(index, c) \
		"xor "#index", "#index"		\n\t"\
		".balign 16			\n\t"\
		"1:				\n\t"\
		"movq (%2, "#index"), %%mm3	\n\t" /* uvbuf0[eax]*/\
		"movq 4096(%2, "#index"), %%mm4	\n\t" /* uvbuf0[eax+2048]*/\
		"psraw $7, %%mm3		\n\t" \
		"psraw $7, %%mm4		\n\t" \
		"movq (%0, "#index", 2), %%mm1	\n\t" /*buf0[eax]*/\
		"movq 8(%0, "#index", 2), %%mm7	\n\t" /*buf0[eax]*/\
		"psraw $7, %%mm1		\n\t" \
		"psraw $7, %%mm7		\n\t" \
                
#define YSCALEYUV2PACKED1(index, c)  REAL_YSCALEYUV2PACKED1(index, c)
                
#define REAL_YSCALEYUV2RGB1(index, c) \
		"xor "#index", "#index"	\n\t"\
		".balign 16			\n\t"\
		"1:				\n\t"\
		"movq (%2, "#index"), %%mm3	\n\t" /* uvbuf0[eax]*/\
		"movq 4096(%2, "#index"), %%mm4	\n\t" /* uvbuf0[eax+2048]*/\
		"psraw $4, %%mm3		\n\t" /* uvbuf0[eax] - uvbuf1[eax] >>4*/\
		"psraw $4, %%mm4		\n\t" /* uvbuf0[eax+2048] - uvbuf1[eax+2048] >>4*/\
		"psubw "U_OFFSET"("#c"), %%mm3	\n\t" /* (U-128)8*/\
		"psubw "V_OFFSET"("#c"), %%mm4	\n\t" /* (V-128)8*/\
		"movq %%mm3, %%mm2		\n\t" /* (U-128)8*/\
		"movq %%mm4, %%mm5		\n\t" /* (V-128)8*/\
		"pmulhw "UG_COEFF"("#c"), %%mm3\n\t"\
		"pmulhw "VG_COEFF"("#c"), %%mm4\n\t"\
	/* mm2=(U-128)8, mm3=ug, mm4=vg mm5=(V-128)8 */\
		"movq (%0, "#index", 2), %%mm1	\n\t" /*buf0[eax]*/\
		"movq 8(%0, "#index", 2), %%mm7	\n\t" /*buf0[eax]*/\
		"psraw $4, %%mm1		\n\t" /* buf0[eax] - buf1[eax] >>4*/\
		"psraw $4, %%mm7		\n\t" /* buf0[eax] - buf1[eax] >>4*/\
		"pmulhw "UB_COEFF"("#c"), %%mm2\n\t"\
		"pmulhw "VR_COEFF"("#c"), %%mm5\n\t"\
		"psubw "Y_OFFSET"("#c"), %%mm1	\n\t" /* 8(Y-16)*/\
		"psubw "Y_OFFSET"("#c"), %%mm7	\n\t" /* 8(Y-16)*/\
		"pmulhw "Y_COEFF"("#c"), %%mm1	\n\t"\
		"pmulhw "Y_COEFF"("#c"), %%mm7	\n\t"\
	/* mm1= Y1, mm2=ub, mm3=ug, mm4=vg mm5=vr, mm7=Y2 */\
		"paddw %%mm3, %%mm4		\n\t"\
		"movq %%mm2, %%mm0		\n\t"\
		"movq %%mm5, %%mm6		\n\t"\
		"movq %%mm4, %%mm3		\n\t"\
		"punpcklwd %%mm2, %%mm2		\n\t"\
		"punpcklwd %%mm5, %%mm5		\n\t"\
		"punpcklwd %%mm4, %%mm4		\n\t"\
		"paddw %%mm1, %%mm2		\n\t"\
		"paddw %%mm1, %%mm5		\n\t"\
		"paddw %%mm1, %%mm4		\n\t"\
		"punpckhwd %%mm0, %%mm0		\n\t"\
		"punpckhwd %%mm6, %%mm6		\n\t"\
		"punpckhwd %%mm3, %%mm3		\n\t"\
		"paddw %%mm7, %%mm0		\n\t"\
		"paddw %%mm7, %%mm6		\n\t"\
		"paddw %%mm7, %%mm3		\n\t"\
		/* mm0=B1, mm2=B2, mm3=G2, mm4=G1, mm5=R1, mm6=R2 */\
		"packuswb %%mm0, %%mm2		\n\t"\
		"packuswb %%mm6, %%mm5		\n\t"\
		"packuswb %%mm3, %%mm4		\n\t"\
		"pxor %%mm7, %%mm7		\n\t"
#define YSCALEYUV2RGB1(index, c)  REAL_YSCALEYUV2RGB1(index, c)

#define REAL_YSCALEYUV2PACKED1b(index, c) \
		"xor "#index", "#index"		\n\t"\
		".balign 16			\n\t"\
		"1:				\n\t"\
		"movq (%2, "#index"), %%mm2	\n\t" /* uvbuf0[eax]*/\
		"movq (%3, "#index"), %%mm3	\n\t" /* uvbuf1[eax]*/\
		"movq 4096(%2, "#index"), %%mm5	\n\t" /* uvbuf0[eax+2048]*/\
		"movq 4096(%3, "#index"), %%mm4	\n\t" /* uvbuf1[eax+2048]*/\
		"paddw %%mm2, %%mm3		\n\t" /* uvbuf0[eax] + uvbuf1[eax]*/\
		"paddw %%mm5, %%mm4		\n\t" /* uvbuf0[eax+2048] + uvbuf1[eax+2048]*/\
		"psrlw $8, %%mm3		\n\t" \
		"psrlw $8, %%mm4		\n\t" \
		"movq (%0, "#index", 2), %%mm1	\n\t" /*buf0[eax]*/\
		"movq 8(%0, "#index", 2), %%mm7	\n\t" /*buf0[eax]*/\
		"psraw $7, %%mm1		\n\t" \
		"psraw $7, %%mm7		\n\t" 
#define YSCALEYUV2PACKED1b(index, c)  REAL_YSCALEYUV2PACKED1b(index, c)
                
// do vertical chrominance interpolation
#define REAL_YSCALEYUV2RGB1b(index, c) \
		"xor "#index", "#index"		\n\t"\
		".balign 16			\n\t"\
		"1:				\n\t"\
		"movq (%2, "#index"), %%mm2	\n\t" /* uvbuf0[eax]*/\
		"movq (%3, "#index"), %%mm3	\n\t" /* uvbuf1[eax]*/\
		"movq 4096(%2, "#index"), %%mm5	\n\t" /* uvbuf0[eax+2048]*/\
		"movq 4096(%3, "#index"), %%mm4	\n\t" /* uvbuf1[eax+2048]*/\
		"paddw %%mm2, %%mm3		\n\t" /* uvbuf0[eax] + uvbuf1[eax]*/\
		"paddw %%mm5, %%mm4		\n\t" /* uvbuf0[eax+2048] + uvbuf1[eax+2048]*/\
		"psrlw $5, %%mm3		\n\t" /*FIXME might overflow*/\
		"psrlw $5, %%mm4		\n\t" /*FIXME might overflow*/\
		"psubw "U_OFFSET"("#c"), %%mm3	\n\t" /* (U-128)8*/\
		"psubw "V_OFFSET"("#c"), %%mm4	\n\t" /* (V-128)8*/\
		"movq %%mm3, %%mm2		\n\t" /* (U-128)8*/\
		"movq %%mm4, %%mm5		\n\t" /* (V-128)8*/\
		"pmulhw "UG_COEFF"("#c"), %%mm3\n\t"\
		"pmulhw "VG_COEFF"("#c"), %%mm4\n\t"\
	/* mm2=(U-128)8, mm3=ug, mm4=vg mm5=(V-128)8 */\
		"movq (%0, "#index", 2), %%mm1	\n\t" /*buf0[eax]*/\
		"movq 8(%0, "#index", 2), %%mm7	\n\t" /*buf0[eax]*/\
		"psraw $4, %%mm1		\n\t" /* buf0[eax] - buf1[eax] >>4*/\
		"psraw $4, %%mm7		\n\t" /* buf0[eax] - buf1[eax] >>4*/\
		"pmulhw "UB_COEFF"("#c"), %%mm2\n\t"\
		"pmulhw "VR_COEFF"("#c"), %%mm5\n\t"\
		"psubw "Y_OFFSET"("#c"), %%mm1	\n\t" /* 8(Y-16)*/\
		"psubw "Y_OFFSET"("#c"), %%mm7	\n\t" /* 8(Y-16)*/\
		"pmulhw "Y_COEFF"("#c"), %%mm1	\n\t"\
		"pmulhw "Y_COEFF"("#c"), %%mm7	\n\t"\
	/* mm1= Y1, mm2=ub, mm3=ug, mm4=vg mm5=vr, mm7=Y2 */\
		"paddw %%mm3, %%mm4		\n\t"\
		"movq %%mm2, %%mm0		\n\t"\
		"movq %%mm5, %%mm6		\n\t"\
		"movq %%mm4, %%mm3		\n\t"\
		"punpcklwd %%mm2, %%mm2		\n\t"\
		"punpcklwd %%mm5, %%mm5		\n\t"\
		"punpcklwd %%mm4, %%mm4		\n\t"\
		"paddw %%mm1, %%mm2		\n\t"\
		"paddw %%mm1, %%mm5		\n\t"\
		"paddw %%mm1, %%mm4		\n\t"\
		"punpckhwd %%mm0, %%mm0		\n\t"\
		"punpckhwd %%mm6, %%mm6		\n\t"\
		"punpckhwd %%mm3, %%mm3		\n\t"\
		"paddw %%mm7, %%mm0		\n\t"\
		"paddw %%mm7, %%mm6		\n\t"\
		"paddw %%mm7, %%mm3		\n\t"\
		/* mm0=B1, mm2=B2, mm3=G2, mm4=G1, mm5=R1, mm6=R2 */\
		"packuswb %%mm0, %%mm2		\n\t"\
		"packuswb %%mm6, %%mm5		\n\t"\
		"packuswb %%mm3, %%mm4		\n\t"\
		"pxor %%mm7, %%mm7		\n\t"
#define YSCALEYUV2RGB1b(index, c)  REAL_YSCALEYUV2RGB1b(index, c)

#define REAL_WRITEBGR32(dst, dstw, index) \
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */\
			"movq %%mm2, %%mm1		\n\t" /* B */\
			"movq %%mm5, %%mm6		\n\t" /* R */\
			"punpcklbw %%mm4, %%mm2		\n\t" /* GBGBGBGB 0 */\
			"punpcklbw %%mm7, %%mm5		\n\t" /* 0R0R0R0R 0 */\
			"punpckhbw %%mm4, %%mm1		\n\t" /* GBGBGBGB 2 */\
			"punpckhbw %%mm7, %%mm6		\n\t" /* 0R0R0R0R 2 */\
			"movq %%mm2, %%mm0		\n\t" /* GBGBGBGB 0 */\
			"movq %%mm1, %%mm3		\n\t" /* GBGBGBGB 2 */\
			"punpcklwd %%mm5, %%mm0		\n\t" /* 0RGB0RGB 0 */\
			"punpckhwd %%mm5, %%mm2		\n\t" /* 0RGB0RGB 1 */\
			"punpcklwd %%mm6, %%mm1		\n\t" /* 0RGB0RGB 2 */\
			"punpckhwd %%mm6, %%mm3		\n\t" /* 0RGB0RGB 3 */\
\
			MOVNTQ(%%mm0, (dst, index, 4))\
			MOVNTQ(%%mm2, 8(dst, index, 4))\
			MOVNTQ(%%mm1, 16(dst, index, 4))\
			MOVNTQ(%%mm3, 24(dst, index, 4))\
\
			"add $8, "#index"		\n\t"\
			"cmp "#dstw", "#index"		\n\t"\
			" jb 1b				\n\t"
#define WRITEBGR32(dst, dstw, index)  REAL_WRITEBGR32(dst, dstw, index)

#define REAL_WRITEBGR16(dst, dstw, index) \
			"pand "MANGLE(bF8)", %%mm2	\n\t" /* B */\
			"pand "MANGLE(bFC)", %%mm4	\n\t" /* G */\
			"pand "MANGLE(bF8)", %%mm5	\n\t" /* R */\
			"psrlq $3, %%mm2		\n\t"\
\
			"movq %%mm2, %%mm1		\n\t"\
			"movq %%mm4, %%mm3		\n\t"\
\
			"punpcklbw %%mm7, %%mm3		\n\t"\
			"punpcklbw %%mm5, %%mm2		\n\t"\
			"punpckhbw %%mm7, %%mm4		\n\t"\
			"punpckhbw %%mm5, %%mm1		\n\t"\
\
			"psllq $3, %%mm3		\n\t"\
			"psllq $3, %%mm4		\n\t"\
\
			"por %%mm3, %%mm2		\n\t"\
			"por %%mm4, %%mm1		\n\t"\
\
			MOVNTQ(%%mm2, (dst, index, 2))\
			MOVNTQ(%%mm1, 8(dst, index, 2))\
\
			"add $8, "#index"		\n\t"\
			"cmp "#dstw", "#index"		\n\t"\
			" jb 1b				\n\t"
#define WRITEBGR16(dst, dstw, index)  REAL_WRITEBGR16(dst, dstw, index)

#define REAL_WRITEBGR15(dst, dstw, index) \
			"pand "MANGLE(bF8)", %%mm2	\n\t" /* B */\
			"pand "MANGLE(bF8)", %%mm4	\n\t" /* G */\
			"pand "MANGLE(bF8)", %%mm5	\n\t" /* R */\
			"psrlq $3, %%mm2		\n\t"\
			"psrlq $1, %%mm5		\n\t"\
\
			"movq %%mm2, %%mm1		\n\t"\
			"movq %%mm4, %%mm3		\n\t"\
\
			"punpcklbw %%mm7, %%mm3		\n\t"\
			"punpcklbw %%mm5, %%mm2		\n\t"\
			"punpckhbw %%mm7, %%mm4		\n\t"\
			"punpckhbw %%mm5, %%mm1		\n\t"\
\
			"psllq $2, %%mm3		\n\t"\
			"psllq $2, %%mm4		\n\t"\
\
			"por %%mm3, %%mm2		\n\t"\
			"por %%mm4, %%mm1		\n\t"\
\
			MOVNTQ(%%mm2, (dst, index, 2))\
			MOVNTQ(%%mm1, 8(dst, index, 2))\
\
			"add $8, "#index"		\n\t"\
			"cmp "#dstw", "#index"		\n\t"\
			" jb 1b				\n\t"
#define WRITEBGR15(dst, dstw, index)  REAL_WRITEBGR15(dst, dstw, index)

#define WRITEBGR24OLD(dst, dstw, index) \
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */\
			"movq %%mm2, %%mm1		\n\t" /* B */\
			"movq %%mm5, %%mm6		\n\t" /* R */\
			"punpcklbw %%mm4, %%mm2		\n\t" /* GBGBGBGB 0 */\
			"punpcklbw %%mm7, %%mm5		\n\t" /* 0R0R0R0R 0 */\
			"punpckhbw %%mm4, %%mm1		\n\t" /* GBGBGBGB 2 */\
			"punpckhbw %%mm7, %%mm6		\n\t" /* 0R0R0R0R 2 */\
			"movq %%mm2, %%mm0		\n\t" /* GBGBGBGB 0 */\
			"movq %%mm1, %%mm3		\n\t" /* GBGBGBGB 2 */\
			"punpcklwd %%mm5, %%mm0		\n\t" /* 0RGB0RGB 0 */\
			"punpckhwd %%mm5, %%mm2		\n\t" /* 0RGB0RGB 1 */\
			"punpcklwd %%mm6, %%mm1		\n\t" /* 0RGB0RGB 2 */\
			"punpckhwd %%mm6, %%mm3		\n\t" /* 0RGB0RGB 3 */\
\
			"movq %%mm0, %%mm4		\n\t" /* 0RGB0RGB 0 */\
			"psrlq $8, %%mm0		\n\t" /* 00RGB0RG 0 */\
			"pand "MANGLE(bm00000111)", %%mm4\n\t" /* 00000RGB 0 */\
			"pand "MANGLE(bm11111000)", %%mm0\n\t" /* 00RGB000 0.5 */\
			"por %%mm4, %%mm0		\n\t" /* 00RGBRGB 0 */\
			"movq %%mm2, %%mm4		\n\t" /* 0RGB0RGB 1 */\
			"psllq $48, %%mm2		\n\t" /* GB000000 1 */\
			"por %%mm2, %%mm0		\n\t" /* GBRGBRGB 0 */\
\
			"movq %%mm4, %%mm2		\n\t" /* 0RGB0RGB 1 */\
			"psrld $16, %%mm4		\n\t" /* 000R000R 1 */\
			"psrlq $24, %%mm2		\n\t" /* 0000RGB0 1.5 */\
			"por %%mm4, %%mm2		\n\t" /* 000RRGBR 1 */\
			"pand "MANGLE(bm00001111)", %%mm2\n\t" /* 0000RGBR 1 */\
			"movq %%mm1, %%mm4		\n\t" /* 0RGB0RGB 2 */\
			"psrlq $8, %%mm1		\n\t" /* 00RGB0RG 2 */\
			"pand "MANGLE(bm00000111)", %%mm4\n\t" /* 00000RGB 2 */\
			"pand "MANGLE(bm11111000)", %%mm1\n\t" /* 00RGB000 2.5 */\
			"por %%mm4, %%mm1		\n\t" /* 00RGBRGB 2 */\
			"movq %%mm1, %%mm4		\n\t" /* 00RGBRGB 2 */\
			"psllq $32, %%mm1		\n\t" /* BRGB0000 2 */\
			"por %%mm1, %%mm2		\n\t" /* BRGBRGBR 1 */\
\
			"psrlq $32, %%mm4		\n\t" /* 000000RG 2.5 */\
			"movq %%mm3, %%mm5		\n\t" /* 0RGB0RGB 3 */\
			"psrlq $8, %%mm3		\n\t" /* 00RGB0RG 3 */\
			"pand "MANGLE(bm00000111)", %%mm5\n\t" /* 00000RGB 3 */\
			"pand "MANGLE(bm11111000)", %%mm3\n\t" /* 00RGB000 3.5 */\
			"por %%mm5, %%mm3		\n\t" /* 00RGBRGB 3 */\
			"psllq $16, %%mm3		\n\t" /* RGBRGB00 3 */\
			"por %%mm4, %%mm3		\n\t" /* RGBRGBRG 2.5 */\
\
			MOVNTQ(%%mm0, (dst))\
			MOVNTQ(%%mm2, 8(dst))\
			MOVNTQ(%%mm3, 16(dst))\
			"add $24, "#dst"		\n\t"\
\
			"add $8, "#index"		\n\t"\
			"cmp "#dstw", "#index"		\n\t"\
			" jb 1b				\n\t"

#define WRITEBGR24MMX(dst, dstw, index) \
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */\
			"movq %%mm2, %%mm1		\n\t" /* B */\
			"movq %%mm5, %%mm6		\n\t" /* R */\
			"punpcklbw %%mm4, %%mm2		\n\t" /* GBGBGBGB 0 */\
			"punpcklbw %%mm7, %%mm5		\n\t" /* 0R0R0R0R 0 */\
			"punpckhbw %%mm4, %%mm1		\n\t" /* GBGBGBGB 2 */\
			"punpckhbw %%mm7, %%mm6		\n\t" /* 0R0R0R0R 2 */\
			"movq %%mm2, %%mm0		\n\t" /* GBGBGBGB 0 */\
			"movq %%mm1, %%mm3		\n\t" /* GBGBGBGB 2 */\
			"punpcklwd %%mm5, %%mm0		\n\t" /* 0RGB0RGB 0 */\
			"punpckhwd %%mm5, %%mm2		\n\t" /* 0RGB0RGB 1 */\
			"punpcklwd %%mm6, %%mm1		\n\t" /* 0RGB0RGB 2 */\
			"punpckhwd %%mm6, %%mm3		\n\t" /* 0RGB0RGB 3 */\
\
			"movq %%mm0, %%mm4		\n\t" /* 0RGB0RGB 0 */\
			"movq %%mm2, %%mm6		\n\t" /* 0RGB0RGB 1 */\
			"movq %%mm1, %%mm5		\n\t" /* 0RGB0RGB 2 */\
			"movq %%mm3, %%mm7		\n\t" /* 0RGB0RGB 3 */\
\
			"psllq $40, %%mm0		\n\t" /* RGB00000 0 */\
			"psllq $40, %%mm2		\n\t" /* RGB00000 1 */\
			"psllq $40, %%mm1		\n\t" /* RGB00000 2 */\
			"psllq $40, %%mm3		\n\t" /* RGB00000 3 */\
\
			"punpckhdq %%mm4, %%mm0		\n\t" /* 0RGBRGB0 0 */\
			"punpckhdq %%mm6, %%mm2		\n\t" /* 0RGBRGB0 1 */\
			"punpckhdq %%mm5, %%mm1		\n\t" /* 0RGBRGB0 2 */\
			"punpckhdq %%mm7, %%mm3		\n\t" /* 0RGBRGB0 3 */\
\
			"psrlq $8, %%mm0		\n\t" /* 00RGBRGB 0 */\
			"movq %%mm2, %%mm6		\n\t" /* 0RGBRGB0 1 */\
			"psllq $40, %%mm2		\n\t" /* GB000000 1 */\
			"por %%mm2, %%mm0		\n\t" /* GBRGBRGB 0 */\
			MOVNTQ(%%mm0, (dst))\
\
			"psrlq $24, %%mm6		\n\t" /* 0000RGBR 1 */\
			"movq %%mm1, %%mm5		\n\t" /* 0RGBRGB0 2 */\
			"psllq $24, %%mm1		\n\t" /* BRGB0000 2 */\
			"por %%mm1, %%mm6		\n\t" /* BRGBRGBR 1 */\
			MOVNTQ(%%mm6, 8(dst))\
\
			"psrlq $40, %%mm5		\n\t" /* 000000RG 2 */\
			"psllq $8, %%mm3		\n\t" /* RGBRGB00 3 */\
			"por %%mm3, %%mm5		\n\t" /* RGBRGBRG 2 */\
			MOVNTQ(%%mm5, 16(dst))\
\
			"add $24, "#dst"		\n\t"\
\
			"add $8, "#index"			\n\t"\
			"cmp "#dstw", "#index"			\n\t"\
			" jb 1b				\n\t"

#define WRITEBGR24MMX2(dst, dstw, index) \
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */\
			"movq "MANGLE(M24A)", %%mm0	\n\t"\
			"movq "MANGLE(M24C)", %%mm7	\n\t"\
			"pshufw $0x50, %%mm2, %%mm1	\n\t" /* B3 B2 B3 B2  B1 B0 B1 B0 */\
			"pshufw $0x50, %%mm4, %%mm3	\n\t" /* G3 G2 G3 G2  G1 G0 G1 G0 */\
			"pshufw $0x00, %%mm5, %%mm6	\n\t" /* R1 R0 R1 R0  R1 R0 R1 R0 */\
\
			"pand %%mm0, %%mm1		\n\t" /*    B2        B1       B0 */\
			"pand %%mm0, %%mm3		\n\t" /*    G2        G1       G0 */\
			"pand %%mm7, %%mm6		\n\t" /*       R1        R0       */\
\
			"psllq $8, %%mm3		\n\t" /* G2        G1       G0    */\
			"por %%mm1, %%mm6		\n\t"\
			"por %%mm3, %%mm6		\n\t"\
			MOVNTQ(%%mm6, (dst))\
\
			"psrlq $8, %%mm4		\n\t" /* 00 G7 G6 G5  G4 G3 G2 G1 */\
			"pshufw $0xA5, %%mm2, %%mm1	\n\t" /* B5 B4 B5 B4  B3 B2 B3 B2 */\
			"pshufw $0x55, %%mm4, %%mm3	\n\t" /* G4 G3 G4 G3  G4 G3 G4 G3 */\
			"pshufw $0xA5, %%mm5, %%mm6	\n\t" /* R5 R4 R5 R4  R3 R2 R3 R2 */\
\
			"pand "MANGLE(M24B)", %%mm1	\n\t" /* B5       B4        B3    */\
			"pand %%mm7, %%mm3		\n\t" /*       G4        G3       */\
			"pand %%mm0, %%mm6		\n\t" /*    R4        R3       R2 */\
\
			"por %%mm1, %%mm3		\n\t" /* B5    G4 B4     G3 B3    */\
			"por %%mm3, %%mm6		\n\t"\
			MOVNTQ(%%mm6, 8(dst))\
\
			"pshufw $0xFF, %%mm2, %%mm1	\n\t" /* B7 B6 B7 B6  B7 B6 B6 B7 */\
			"pshufw $0xFA, %%mm4, %%mm3	\n\t" /* 00 G7 00 G7  G6 G5 G6 G5 */\
			"pshufw $0xFA, %%mm5, %%mm6	\n\t" /* R7 R6 R7 R6  R5 R4 R5 R4 */\
\
			"pand %%mm7, %%mm1		\n\t" /*       B7        B6       */\
			"pand %%mm0, %%mm3		\n\t" /*    G7        G6       G5 */\
			"pand "MANGLE(M24B)", %%mm6	\n\t" /* R7       R6        R5    */\
\
			"por %%mm1, %%mm3		\n\t"\
			"por %%mm3, %%mm6		\n\t"\
			MOVNTQ(%%mm6, 16(dst))\
\
			"add $24, "#dst"		\n\t"\
\
			"add $8, "#index"		\n\t"\
			"cmp "#dstw", "#index"		\n\t"\
			" jb 1b				\n\t"

#ifdef HAVE_MMX2
#undef WRITEBGR24
#define WRITEBGR24(dst, dstw, index)  WRITEBGR24MMX2(dst, dstw, index)
#else
#undef WRITEBGR24
#define WRITEBGR24(dst, dstw, index)  WRITEBGR24MMX(dst, dstw, index)
#endif

#define REAL_WRITEYUY2(dst, dstw, index) \
			"packuswb %%mm3, %%mm3		\n\t"\
			"packuswb %%mm4, %%mm4		\n\t"\
			"packuswb %%mm7, %%mm1		\n\t"\
			"punpcklbw %%mm4, %%mm3		\n\t"\
			"movq %%mm1, %%mm7		\n\t"\
			"punpcklbw %%mm3, %%mm1		\n\t"\
			"punpckhbw %%mm3, %%mm7		\n\t"\
\
			MOVNTQ(%%mm1, (dst, index, 2))\
			MOVNTQ(%%mm7, 8(dst, index, 2))\
\
			"add $8, "#index"		\n\t"\
			"cmp "#dstw", "#index"		\n\t"\
			" jb 1b				\n\t"
#define WRITEYUY2(dst, dstw, index)  REAL_WRITEYUY2(dst, dstw, index)


static inline void RENAME(yuv2yuvX)(SwsContext *c, int16_t *lumFilter, int16_t **lumSrc, int lumFilterSize,
				    int16_t *chrFilter, int16_t **chrSrc, int chrFilterSize,
				    uint8_t *dest, uint8_t *uDest, uint8_t *vDest, int dstW, int chrDstW)
{
#ifdef HAVE_MMX
	if(uDest != NULL)
	{
		asm volatile(
				YSCALEYUV2YV12X(0, CHR_MMX_FILTER_OFFSET)
				:: "r" (&c->redDither),
				"r" (uDest), "m" ((long)chrDstW)
				: "%"REG_a, "%"REG_d, "%"REG_S
			);

		asm volatile(
				YSCALEYUV2YV12X(4096, CHR_MMX_FILTER_OFFSET)
				:: "r" (&c->redDither),
				"r" (vDest), "m" ((long)chrDstW)
				: "%"REG_a, "%"REG_d, "%"REG_S
			);
	}

	asm volatile(
			YSCALEYUV2YV12X(0, LUM_MMX_FILTER_OFFSET)
			:: "r" (&c->redDither),
			   "r" (dest), "m" ((long)dstW)
			: "%"REG_a, "%"REG_d, "%"REG_S
		);
#else
#ifdef HAVE_ALTIVEC
yuv2yuvX_altivec_real(lumFilter, lumSrc, lumFilterSize,
		      chrFilter, chrSrc, chrFilterSize,
		      dest, uDest, vDest, dstW, chrDstW);
#else //HAVE_ALTIVEC
yuv2yuvXinC(lumFilter, lumSrc, lumFilterSize,
	    chrFilter, chrSrc, chrFilterSize,
	    dest, uDest, vDest, dstW, chrDstW);
#endif //!HAVE_ALTIVEC
#endif
}

static inline void RENAME(yuv2nv12X)(SwsContext *c, int16_t *lumFilter, int16_t **lumSrc, int lumFilterSize,
				     int16_t *chrFilter, int16_t **chrSrc, int chrFilterSize,
				     uint8_t *dest, uint8_t *uDest, int dstW, int chrDstW, int dstFormat)
{
yuv2nv12XinC(lumFilter, lumSrc, lumFilterSize,
	     chrFilter, chrSrc, chrFilterSize,
	     dest, uDest, dstW, chrDstW, dstFormat);
}

static inline void RENAME(yuv2yuv1)(int16_t *lumSrc, int16_t *chrSrc,
				    uint8_t *dest, uint8_t *uDest, uint8_t *vDest, int dstW, int chrDstW)
{
#ifdef HAVE_MMX
	if(uDest != NULL)
	{
		asm volatile(
				YSCALEYUV2YV121
				:: "r" (chrSrc + chrDstW), "r" (uDest + chrDstW),
				"g" ((long)-chrDstW)
				: "%"REG_a
			);

		asm volatile(
				YSCALEYUV2YV121
				:: "r" (chrSrc + 2048 + chrDstW), "r" (vDest + chrDstW),
				"g" ((long)-chrDstW)
				: "%"REG_a
			);
	}

	asm volatile(
		YSCALEYUV2YV121
		:: "r" (lumSrc + dstW), "r" (dest + dstW),
		"g" ((long)-dstW)
		: "%"REG_a
	);
#else
	int i;
	for(i=0; i<dstW; i++)
	{
		int val= lumSrc[i]>>7;
		
		if(val&256){
			if(val<0) val=0;
			else      val=255;
		}

		dest[i]= val;
	}

	if(uDest != NULL)
		for(i=0; i<chrDstW; i++)
		{
			int u=chrSrc[i]>>7;
			int v=chrSrc[i + 2048]>>7;

			if((u|v)&256){
				if(u<0)         u=0;
				else if (u>255) u=255;
				if(v<0)         v=0;
				else if (v>255) v=255;
			}

			uDest[i]= u;
			vDest[i]= v;
		}
#endif
}


/**
 * vertical scale YV12 to RGB
 */
static inline void RENAME(yuv2packedX)(SwsContext *c, int16_t *lumFilter, int16_t **lumSrc, int lumFilterSize,
				    int16_t *chrFilter, int16_t **chrSrc, int chrFilterSize,
			    uint8_t *dest, int dstW, int dstY)
{
	int dummy=0;
	switch(c->dstFormat)
	{
#ifdef HAVE_MMX
	case IMGFMT_BGR32:
		{
			asm volatile(
				YSCALEYUV2RGBX
				WRITEBGR32(%4, %5, %%REGa)

			:: "r" (&c->redDither), 
			   "m" (dummy), "m" (dummy), "m" (dummy),
			   "r" (dest), "m" (dstW)
			: "%"REG_a, "%"REG_d, "%"REG_S
			);
		}
		break;
	case IMGFMT_BGR24:
		{
			asm volatile(
				YSCALEYUV2RGBX
				"lea (%%"REG_a", %%"REG_a", 2), %%"REG_b"\n\t" //FIXME optimize
				"add %4, %%"REG_b"			\n\t"
				WRITEBGR24(%%REGb, %5, %%REGa)

			:: "r" (&c->redDither), 
			   "m" (dummy), "m" (dummy), "m" (dummy),
			   "r" (dest), "m" (dstW)
			: "%"REG_a, "%"REG_b, "%"REG_d, "%"REG_S //FIXME ebx
			);
		}
		break;
	case IMGFMT_BGR15:
		{
			asm volatile(
				YSCALEYUV2RGBX
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */
#ifdef DITHER1XBPP
				"paddusb "MANGLE(b5Dither)", %%mm2\n\t"
				"paddusb "MANGLE(g5Dither)", %%mm4\n\t"
				"paddusb "MANGLE(r5Dither)", %%mm5\n\t"
#endif

				WRITEBGR15(%4, %5, %%REGa)

			:: "r" (&c->redDither), 
			   "m" (dummy), "m" (dummy), "m" (dummy),
			   "r" (dest), "m" (dstW)
			: "%"REG_a, "%"REG_d, "%"REG_S
			);
		}
		break;
	case IMGFMT_BGR16:
		{
			asm volatile(
				YSCALEYUV2RGBX
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */
#ifdef DITHER1XBPP
				"paddusb "MANGLE(b5Dither)", %%mm2\n\t"
				"paddusb "MANGLE(g6Dither)", %%mm4\n\t"
				"paddusb "MANGLE(r5Dither)", %%mm5\n\t"
#endif

				WRITEBGR16(%4, %5, %%REGa)

			:: "r" (&c->redDither), 
			   "m" (dummy), "m" (dummy), "m" (dummy),
			   "r" (dest), "m" (dstW)
			: "%"REG_a, "%"REG_d, "%"REG_S
			);
		}
		break;
	case IMGFMT_YUY2:
		{
			asm volatile(
				YSCALEYUV2PACKEDX
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */

				"psraw $3, %%mm3		\n\t"
				"psraw $3, %%mm4		\n\t"
				"psraw $3, %%mm1		\n\t"
				"psraw $3, %%mm7		\n\t"
				WRITEYUY2(%4, %5, %%REGa)

			:: "r" (&c->redDither), 
			   "m" (dummy), "m" (dummy), "m" (dummy),
			   "r" (dest), "m" (dstW)
			: "%"REG_a, "%"REG_d, "%"REG_S
			);
		}
		break;
#endif
	default:
#ifdef HAVE_ALTIVEC
		altivec_yuv2packedX (c, lumFilter, lumSrc, lumFilterSize,
			    chrFilter, chrSrc, chrFilterSize,
			    dest, dstW, dstY);
#else
		yuv2packedXinC(c, lumFilter, lumSrc, lumFilterSize,
			    chrFilter, chrSrc, chrFilterSize,
			    dest, dstW, dstY);
#endif
		break;
	}
}

/**
 * vertical bilinear scale YV12 to RGB
 */
static inline void RENAME(yuv2packed2)(SwsContext *c, uint16_t *buf0, uint16_t *buf1, uint16_t *uvbuf0, uint16_t *uvbuf1,
			    uint8_t *dest, int dstW, int yalpha, int uvalpha, int y)
{
	int yalpha1=yalpha^4095;
	int uvalpha1=uvalpha^4095;
	int i;

#if 0 //isn't used
	if(flags&SWS_FULL_CHR_H_INT)
	{
		switch(dstFormat)
		{
#ifdef HAVE_MMX
		case IMGFMT_BGR32:
			asm volatile(


FULL_YSCALEYUV2RGB
			"punpcklbw %%mm1, %%mm3		\n\t" // BGBGBGBG
			"punpcklbw %%mm7, %%mm0		\n\t" // R0R0R0R0

			"movq %%mm3, %%mm1		\n\t"
			"punpcklwd %%mm0, %%mm3		\n\t" // BGR0BGR0
			"punpckhwd %%mm0, %%mm1		\n\t" // BGR0BGR0

			MOVNTQ(%%mm3, (%4, %%REGa, 4))
			MOVNTQ(%%mm1, 8(%4, %%REGa, 4))

			"add $4, %%"REG_a"		\n\t"
			"cmp %5, %%"REG_a"		\n\t"
			" jb 1b				\n\t"


			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "r" (dest), "m" ((long)dstW),
			"m" (yalpha1), "m" (uvalpha1)
			: "%"REG_a
			);
			break;
		case IMGFMT_BGR24:
			asm volatile(

FULL_YSCALEYUV2RGB

								// lsb ... msb
			"punpcklbw %%mm1, %%mm3		\n\t" // BGBGBGBG
			"punpcklbw %%mm7, %%mm0		\n\t" // R0R0R0R0

			"movq %%mm3, %%mm1		\n\t"
			"punpcklwd %%mm0, %%mm3		\n\t" // BGR0BGR0
			"punpckhwd %%mm0, %%mm1		\n\t" // BGR0BGR0

			"movq %%mm3, %%mm2		\n\t" // BGR0BGR0
			"psrlq $8, %%mm3		\n\t" // GR0BGR00
			"pand "MANGLE(bm00000111)", %%mm2\n\t" // BGR00000
			"pand "MANGLE(bm11111000)", %%mm3\n\t" // 000BGR00
			"por %%mm2, %%mm3		\n\t" // BGRBGR00
			"movq %%mm1, %%mm2		\n\t"
			"psllq $48, %%mm1		\n\t" // 000000BG
			"por %%mm1, %%mm3		\n\t" // BGRBGRBG

			"movq %%mm2, %%mm1		\n\t" // BGR0BGR0
			"psrld $16, %%mm2		\n\t" // R000R000
			"psrlq $24, %%mm1		\n\t" // 0BGR0000
			"por %%mm2, %%mm1		\n\t" // RBGRR000

			"mov %4, %%"REG_b"		\n\t"
			"add %%"REG_a", %%"REG_b"	\n\t"

#ifdef HAVE_MMX2
			//FIXME Alignment
			"movntq %%mm3, (%%"REG_b", %%"REG_a", 2)\n\t"
			"movntq %%mm1, 8(%%"REG_b", %%"REG_a", 2)\n\t"
#else
			"movd %%mm3, (%%"REG_b", %%"REG_a", 2)	\n\t"
			"psrlq $32, %%mm3		\n\t"
			"movd %%mm3, 4(%%"REG_b", %%"REG_a", 2)	\n\t"
			"movd %%mm1, 8(%%"REG_b", %%"REG_a", 2)	\n\t"
#endif
			"add $4, %%"REG_a"		\n\t"
			"cmp %5, %%"REG_a"		\n\t"
			" jb 1b				\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest), "m" (dstW),
			"m" (yalpha1), "m" (uvalpha1)
			: "%"REG_a, "%"REG_b
			);
			break;
		case IMGFMT_BGR15:
			asm volatile(

FULL_YSCALEYUV2RGB
#ifdef DITHER1XBPP
			"paddusb "MANGLE(g5Dither)", %%mm1\n\t"
			"paddusb "MANGLE(r5Dither)", %%mm0\n\t"
			"paddusb "MANGLE(b5Dither)", %%mm3\n\t"
#endif
			"punpcklbw %%mm7, %%mm1		\n\t" // 0G0G0G0G
			"punpcklbw %%mm7, %%mm3		\n\t" // 0B0B0B0B
			"punpcklbw %%mm7, %%mm0		\n\t" // 0R0R0R0R

			"psrlw $3, %%mm3		\n\t"
			"psllw $2, %%mm1		\n\t"
			"psllw $7, %%mm0		\n\t"
			"pand "MANGLE(g15Mask)", %%mm1	\n\t"
			"pand "MANGLE(r15Mask)", %%mm0	\n\t"

			"por %%mm3, %%mm1		\n\t"
			"por %%mm1, %%mm0		\n\t"

			MOVNTQ(%%mm0, (%4, %%REGa, 2))

			"add $4, %%"REG_a"		\n\t"
			"cmp %5, %%"REG_a"		\n\t"
			" jb 1b				\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "r" (dest), "m" (dstW),
			"m" (yalpha1), "m" (uvalpha1)
			: "%"REG_a
			);
			break;
		case IMGFMT_BGR16:
			asm volatile(

FULL_YSCALEYUV2RGB
#ifdef DITHER1XBPP
			"paddusb "MANGLE(g6Dither)", %%mm1\n\t"
			"paddusb "MANGLE(r5Dither)", %%mm0\n\t"
			"paddusb "MANGLE(b5Dither)", %%mm3\n\t"
#endif
			"punpcklbw %%mm7, %%mm1		\n\t" // 0G0G0G0G
			"punpcklbw %%mm7, %%mm3		\n\t" // 0B0B0B0B
			"punpcklbw %%mm7, %%mm0		\n\t" // 0R0R0R0R

			"psrlw $3, %%mm3		\n\t"
			"psllw $3, %%mm1		\n\t"
			"psllw $8, %%mm0		\n\t"
			"pand "MANGLE(g16Mask)", %%mm1	\n\t"
			"pand "MANGLE(r16Mask)", %%mm0	\n\t"

			"por %%mm3, %%mm1		\n\t"
			"por %%mm1, %%mm0		\n\t"

			MOVNTQ(%%mm0, (%4, %%REGa, 2))

			"add $4, %%"REG_a"		\n\t"
			"cmp %5, %%"REG_a"		\n\t"
			" jb 1b				\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "r" (dest), "m" (dstW),
			"m" (yalpha1), "m" (uvalpha1)
			: "%"REG_a
			);
		break;
#endif
		case IMGFMT_RGB32:
#ifndef HAVE_MMX
		case IMGFMT_BGR32:
#endif
		if(dstFormat==IMGFMT_BGR32)
		{
			int i;
#ifdef WORDS_BIGENDIAN
			dest++;
#endif
			for(i=0;i<dstW;i++){
				// vertical linear interpolation && yuv2rgb in a single step:
				int Y=yuvtab_2568[((buf0[i]*yalpha1+buf1[i]*yalpha)>>19)];
				int U=((uvbuf0[i]*uvalpha1+uvbuf1[i]*uvalpha)>>19);
				int V=((uvbuf0[i+2048]*uvalpha1+uvbuf1[i+2048]*uvalpha)>>19);
				dest[0]=clip_table[((Y + yuvtab_40cf[U]) >>13)];
				dest[1]=clip_table[((Y + yuvtab_1a1e[V] + yuvtab_0c92[U]) >>13)];
				dest[2]=clip_table[((Y + yuvtab_3343[V]) >>13)];
				dest+= 4;
			}
		}
		else if(dstFormat==IMGFMT_BGR24)
		{
			int i;
			for(i=0;i<dstW;i++){
				// vertical linear interpolation && yuv2rgb in a single step:
				int Y=yuvtab_2568[((buf0[i]*yalpha1+buf1[i]*yalpha)>>19)];
				int U=((uvbuf0[i]*uvalpha1+uvbuf1[i]*uvalpha)>>19);
				int V=((uvbuf0[i+2048]*uvalpha1+uvbuf1[i+2048]*uvalpha)>>19);
				dest[0]=clip_table[((Y + yuvtab_40cf[U]) >>13)];
				dest[1]=clip_table[((Y + yuvtab_1a1e[V] + yuvtab_0c92[U]) >>13)];
				dest[2]=clip_table[((Y + yuvtab_3343[V]) >>13)];
				dest+= 3;
			}
		}
		else if(dstFormat==IMGFMT_BGR16)
		{
			int i;
			for(i=0;i<dstW;i++){
				// vertical linear interpolation && yuv2rgb in a single step:
				int Y=yuvtab_2568[((buf0[i]*yalpha1+buf1[i]*yalpha)>>19)];
				int U=((uvbuf0[i]*uvalpha1+uvbuf1[i]*uvalpha)>>19);
				int V=((uvbuf0[i+2048]*uvalpha1+uvbuf1[i+2048]*uvalpha)>>19);

				((uint16_t*)dest)[i] =
					clip_table16b[(Y + yuvtab_40cf[U]) >>13] |
					clip_table16g[(Y + yuvtab_1a1e[V] + yuvtab_0c92[U]) >>13] |
					clip_table16r[(Y + yuvtab_3343[V]) >>13];
			}
		}
		else if(dstFormat==IMGFMT_BGR15)
		{
			int i;
			for(i=0;i<dstW;i++){
				// vertical linear interpolation && yuv2rgb in a single step:
				int Y=yuvtab_2568[((buf0[i]*yalpha1+buf1[i]*yalpha)>>19)];
				int U=((uvbuf0[i]*uvalpha1+uvbuf1[i]*uvalpha)>>19);
				int V=((uvbuf0[i+2048]*uvalpha1+uvbuf1[i+2048]*uvalpha)>>19);

				((uint16_t*)dest)[i] =
					clip_table15b[(Y + yuvtab_40cf[U]) >>13] |
					clip_table15g[(Y + yuvtab_1a1e[V] + yuvtab_0c92[U]) >>13] |
					clip_table15r[(Y + yuvtab_3343[V]) >>13];
			}
		}
	}//FULL_UV_IPOL
	else
	{
#endif // if 0
#ifdef HAVE_MMX
	switch(c->dstFormat)
	{
//Note 8280 == DSTW_OFFSET but the preprocessor can't handle that there :(
	case IMGFMT_BGR32:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB(%%REGa, %5)
				WRITEBGR32(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
	case IMGFMT_BGR24:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB(%%REGa, %5)
				WRITEBGR24(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"
			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
	case IMGFMT_BGR15:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB(%%REGa, %5)
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */
#ifdef DITHER1XBPP
				"paddusb "MANGLE(b5Dither)", %%mm2\n\t"
				"paddusb "MANGLE(g5Dither)", %%mm4\n\t"
				"paddusb "MANGLE(r5Dither)", %%mm5\n\t"
#endif

				WRITEBGR15(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
	case IMGFMT_BGR16:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB(%%REGa, %5)
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */
#ifdef DITHER1XBPP
				"paddusb "MANGLE(b5Dither)", %%mm2\n\t"
				"paddusb "MANGLE(g6Dither)", %%mm4\n\t"
				"paddusb "MANGLE(r5Dither)", %%mm5\n\t"
#endif

				WRITEBGR16(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"
			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
	case IMGFMT_YUY2:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2PACKED(%%REGa, %5)
				WRITEYUY2(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"
			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
	default: break;
	}
#endif //HAVE_MMX
YSCALE_YUV_2_ANYRGB_C(YSCALE_YUV_2_RGB2_C, YSCALE_YUV_2_PACKED2_C)
}

/**
 * YV12 to RGB without scaling or interpolating
 */
static inline void RENAME(yuv2packed1)(SwsContext *c, uint16_t *buf0, uint16_t *uvbuf0, uint16_t *uvbuf1,
			    uint8_t *dest, int dstW, int uvalpha, int dstFormat, int flags, int y)
{
	const int yalpha1=0;
	int i;
	
	uint16_t *buf1= buf0; //FIXME needed for the rgb1/bgr1
	const int yalpha= 4096; //FIXME ...

	if(flags&SWS_FULL_CHR_H_INT)
	{
		RENAME(yuv2packed2)(c, buf0, buf0, uvbuf0, uvbuf1, dest, dstW, 0, uvalpha, y);
		return;
	}

#ifdef HAVE_MMX
	if( uvalpha < 2048 ) // note this is not correct (shifts chrominance by 0.5 pixels) but its a bit faster
	{
		switch(dstFormat)
		{
		case IMGFMT_BGR32:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB1(%%REGa, %5)
				WRITEBGR32(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
		case IMGFMT_BGR24:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB1(%%REGa, %5)
				WRITEBGR24(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
		case IMGFMT_BGR15:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB1(%%REGa, %5)
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */
#ifdef DITHER1XBPP
				"paddusb "MANGLE(b5Dither)", %%mm2\n\t"
				"paddusb "MANGLE(g5Dither)", %%mm4\n\t"
				"paddusb "MANGLE(r5Dither)", %%mm5\n\t"
#endif
				WRITEBGR15(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
		case IMGFMT_BGR16:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB1(%%REGa, %5)
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */
#ifdef DITHER1XBPP
				"paddusb "MANGLE(b5Dither)", %%mm2\n\t"
				"paddusb "MANGLE(g6Dither)", %%mm4\n\t"
				"paddusb "MANGLE(r5Dither)", %%mm5\n\t"
#endif

				WRITEBGR16(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
		case IMGFMT_YUY2:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2PACKED1(%%REGa, %5)
				WRITEYUY2(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
		}
	}
	else
	{
		switch(dstFormat)
		{
		case IMGFMT_BGR32:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB1b(%%REGa, %5)
				WRITEBGR32(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
		case IMGFMT_BGR24:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB1b(%%REGa, %5)
				WRITEBGR24(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
		case IMGFMT_BGR15:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB1b(%%REGa, %5)
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */
#ifdef DITHER1XBPP
				"paddusb "MANGLE(b5Dither)", %%mm2\n\t"
				"paddusb "MANGLE(g5Dither)", %%mm4\n\t"
				"paddusb "MANGLE(r5Dither)", %%mm5\n\t"
#endif
				WRITEBGR15(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
		case IMGFMT_BGR16:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2RGB1b(%%REGa, %5)
		/* mm2=B, %%mm4=G, %%mm5=R, %%mm7=0 */
#ifdef DITHER1XBPP
				"paddusb "MANGLE(b5Dither)", %%mm2\n\t"
				"paddusb "MANGLE(g6Dither)", %%mm4\n\t"
				"paddusb "MANGLE(r5Dither)", %%mm5\n\t"
#endif

				WRITEBGR16(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
		case IMGFMT_YUY2:
			asm volatile(
				"mov %%"REG_SP", "ESP_OFFSET"(%5)	\n\t"
				"mov %4, %%"REG_SP"			\n\t"
				YSCALEYUV2PACKED1b(%%REGa, %5)
				WRITEYUY2(%%REGSP, 8280(%5), %%REGa)
				"mov "ESP_OFFSET"(%5), %%"REG_SP"	\n\t"

			:: "r" (buf0), "r" (buf1), "r" (uvbuf0), "r" (uvbuf1), "m" (dest),
			"r" (&c->redDither)
			: "%"REG_a
			);
			return;
		}
	}
#endif
	if( uvalpha < 2048 )
	{
		YSCALE_YUV_2_ANYRGB_C(YSCALE_YUV_2_RGB1_C, YSCALE_YUV_2_PACKED1_C)
	}else{
		YSCALE_YUV_2_ANYRGB_C(YSCALE_YUV_2_RGB1B_C, YSCALE_YUV_2_PACKED1B_C)
	}
}

//FIXME yuy2* can read upto 7 samples to much

static inline void RENAME(yuy2ToY)(uint8_t *dst, uint8_t *src, int width)
{
#ifdef HAVE_MMX
	asm volatile(
		"movq "MANGLE(bm01010101)", %%mm2\n\t"
		"mov %0, %%"REG_a"		\n\t"
		"1:				\n\t"
		"movq (%1, %%"REG_a",2), %%mm0	\n\t"
		"movq 8(%1, %%"REG_a",2), %%mm1	\n\t"
		"pand %%mm2, %%mm0		\n\t"
		"pand %%mm2, %%mm1		\n\t"
		"packuswb %%mm1, %%mm0		\n\t"
		"movq %%mm0, (%2, %%"REG_a")	\n\t"
		"add $8, %%"REG_a"		\n\t"
		" js 1b				\n\t"
		: : "g" ((long)-width), "r" (src+width*2), "r" (dst+width)
		: "%"REG_a
	);
#else
	int i;
	for(i=0; i<width; i++)
		dst[i]= src[2*i];
#endif
}

static inline void RENAME(yuy2ToUV)(uint8_t *dstU, uint8_t *dstV, uint8_t *src1, uint8_t *src2, int width)
{
#if defined (HAVE_MMX2) || defined (HAVE_3DNOW)
	asm volatile(
		"movq "MANGLE(bm01010101)", %%mm4\n\t"
		"mov %0, %%"REG_a"		\n\t"
		"1:				\n\t"
		"movq (%1, %%"REG_a",4), %%mm0	\n\t"
		"movq 8(%1, %%"REG_a",4), %%mm1	\n\t"
		"movq (%2, %%"REG_a",4), %%mm2	\n\t"
		"movq 8(%2, %%"REG_a",4), %%mm3	\n\t"
		PAVGB(%%mm2, %%mm0)
		PAVGB(%%mm3, %%mm1)
		"psrlw $8, %%mm0		\n\t"
		"psrlw $8, %%mm1		\n\t"
		"packuswb %%mm1, %%mm0		\n\t"
		"movq %%mm0, %%mm1		\n\t"
		"psrlw $8, %%mm0		\n\t"
		"pand %%mm4, %%mm1		\n\t"
		"packuswb %%mm0, %%mm0		\n\t"
		"packuswb %%mm1, %%mm1		\n\t"
		"movd %%mm0, (%4, %%"REG_a")	\n\t"
		"movd %%mm1, (%3, %%"REG_a")	\n\t"
		"add $4, %%"REG_a"		\n\t"
		" js 1b				\n\t"
		: : "g" ((long)-width), "r" (src1+width*4), "r" (src2+width*4), "r" (dstU+width), "r" (dstV+width)
		: "%"REG_a
	);
#else
	int i;
	for(i=0; i<width; i++)
	{
		dstU[i]= (src1[4*i + 1] + src2[4*i + 1])>>1;
		dstV[i]= (src1[4*i + 3] + src2[4*i + 3])>>1;
	}
#endif
}

//this is allmost identical to the previous, end exists only cuz yuy2ToY/UV)(dst, src+1, ...) would have 100% unaligned accesses
static inline void RENAME(uyvyToY)(uint8_t *dst, uint8_t *src, int width)
{
#ifdef HAVE_MMX
	asm volatile(
		"mov %0, %%"REG_a"		\n\t"
		"1:				\n\t"
		"movq (%1, %%"REG_a",2), %%mm0	\n\t"
		"movq 8(%1, %%"REG_a",2), %%mm1	\n\t"
		"psrlw $8, %%mm0		\n\t"
		"psrlw $8, %%mm1		\n\t"
		"packuswb %%mm1, %%mm0		\n\t"
		"movq %%mm0, (%2, %%"REG_a")	\n\t"
		"add $8, %%"REG_a"		\n\t"
		" js 1b				\n\t"
		: : "g" ((long)-width), "r" (src+width*2), "r" (dst+width)
		: "%"REG_a
	);
#else
	int i;
	for(i=0; i<width; i++)
		dst[i]= src[2*i+1];
#endif
}

static inline void RENAME(uyvyToUV)(uint8_t *dstU, uint8_t *dstV, uint8_t *src1, uint8_t *src2, int width)
{
#if defined (HAVE_MMX2) || defined (HAVE_3DNOW)
	asm volatile(
		"movq "MANGLE(bm01010101)", %%mm4\n\t"
		"mov %0, %%"REG_a"		\n\t"
		"1:				\n\t"
		"movq (%1, %%"REG_a",4), %%mm0	\n\t"
		"movq 8(%1, %%"REG_a",4), %%mm1	\n\t"
		"movq (%2, %%"REG_a",4), %%mm2	\n\t"
		"movq 8(%2, %%"REG_a",4), %%mm3	\n\t"
		PAVGB(%%mm2, %%mm0)
		PAVGB(%%mm3, %%mm1)
		"pand %%mm4, %%mm0		\n\t"
		"pand %%mm4, %%mm1		\n\t"
		"packuswb %%mm1, %%mm0		\n\t"
		"movq %%mm0, %%mm1		\n\t"
		"psrlw $8, %%mm0		\n\t"
		"pand %%mm4, %%mm1		\n\t"
		"packuswb %%mm0, %%mm0		\n\t"
		"packuswb %%mm1, %%mm1		\n\t"
		"movd %%mm0, (%4, %%"REG_a")	\n\t"
		"movd %%mm1, (%3, %%"REG_a")	\n\t"
		"add $4, %%"REG_a"		\n\t"
		" js 1b				\n\t"
		: : "g" ((long)-width), "r" (src1+width*4), "r" (src2+width*4), "r" (dstU+width), "r" (dstV+width)
		: "%"REG_a
	);
#else
	int i;
	for(i=0; i<width; i++)
	{
		dstU[i]= (src1[4*i + 0] + src2[4*i + 0])>>1;
		dstV[i]= (src1[4*i + 2] + src2[4*i + 2])>>1;
	}
#endif
}

static inline void RENAME(bgr32ToY)(uint8_t *dst, uint8_t *src, int width)
{
	int i;
	for(i=0; i<width; i++)
	{
		int b=  ((uint32_t*)src)[i]&0xFF;
		int g= (((uint32_t*)src)[i]>>8)&0xFF;
		int r= (((uint32_t*)src)[i]>>16)&0xFF;

		dst[i]= ((RY*r + GY*g + BY*b + (33<<(RGB2YUV_SHIFT-1)) )>>RGB2YUV_SHIFT);
	}
}

static inline void RENAME(bgr32ToUV)(uint8_t *dstU, uint8_t *dstV, uint8_t *src1, uint8_t *src2, int width)
{
	int i;
	for(i=0; i<width; i++)
	{
		const int a= ((uint32_t*)src1)[2*i+0];
		const int e= ((uint32_t*)src1)[2*i+1];
		const int c= ((uint32_t*)src2)[2*i+0];
		const int d= ((uint32_t*)src2)[2*i+1];
		const int l= (a&0xFF00FF) + (e&0xFF00FF) + (c&0xFF00FF) + (d&0xFF00FF);
		const int h= (a&0x00FF00) + (e&0x00FF00) + (c&0x00FF00) + (d&0x00FF00);
 		const int b=  l&0x3FF;
		const int g=  h>>8;
		const int r=  l>>16;

		dstU[i]= ((RU*r + GU*g + BU*b)>>(RGB2YUV_SHIFT+2)) + 128;
		dstV[i]= ((RV*r + GV*g + BV*b)>>(RGB2YUV_SHIFT+2)) + 128;
	}
}

static inline void RENAME(bgr24ToY)(uint8_t *dst, uint8_t *src, int width)
{
#ifdef HAVE_MMX
	asm volatile(
		"mov %2, %%"REG_a"		\n\t"
		"movq "MANGLE(bgr2YCoeff)", %%mm6		\n\t"
		"movq "MANGLE(w1111)", %%mm5		\n\t"
		"pxor %%mm7, %%mm7		\n\t"
		"lea (%%"REG_a", %%"REG_a", 2), %%"REG_b"\n\t"
		".balign 16			\n\t"
		"1:				\n\t"
		PREFETCH" 64(%0, %%"REG_b")	\n\t"
		"movd (%0, %%"REG_b"), %%mm0	\n\t"
		"movd 3(%0, %%"REG_b"), %%mm1	\n\t"
		"punpcklbw %%mm7, %%mm0		\n\t"
		"punpcklbw %%mm7, %%mm1		\n\t"
		"movd 6(%0, %%"REG_b"), %%mm2	\n\t"
		"movd 9(%0, %%"REG_b"), %%mm3	\n\t"
		"punpcklbw %%mm7, %%mm2		\n\t"
		"punpcklbw %%mm7, %%mm3		\n\t"
		"pmaddwd %%mm6, %%mm0		\n\t"
		"pmaddwd %%mm6, %%mm1		\n\t"
		"pmaddwd %%mm6, %%mm2		\n\t"
		"pmaddwd %%mm6, %%mm3		\n\t"
#ifndef FAST_BGR2YV12
		"psrad $8, %%mm0		\n\t"
		"psrad $8, %%mm1		\n\t"
		"psrad $8, %%mm2		\n\t"
		"psrad $8, %%mm3		\n\t"
#endif
		"packssdw %%mm1, %%mm0		\n\t"
		"packssdw %%mm3, %%mm2		\n\t"
		"pmaddwd %%mm5, %%mm0		\n\t"
		"pmaddwd %%mm5, %%mm2		\n\t"
		"packssdw %%mm2, %%mm0		\n\t"
		"psraw $7, %%mm0		\n\t"

		"movd 12(%0, %%"REG_b"), %%mm4	\n\t"
		"movd 15(%0, %%"REG_b"), %%mm1	\n\t"
		"punpcklbw %%mm7, %%mm4		\n\t"
		"punpcklbw %%mm7, %%mm1		\n\t"
		"movd 18(%0, %%"REG_b"), %%mm2	\n\t"
		"movd 21(%0, %%"REG_b"), %%mm3	\n\t"
		"punpcklbw %%mm7, %%mm2		\n\t"
		"punpcklbw %%mm7, %%mm3		\n\t"
		"pmaddwd %%mm6, %%mm4		\n\t"
		"pmaddwd %%mm6, %%mm1		\n\t"
		"pmaddwd %%mm6, %%mm2		\n\t"
		"pmaddwd %%mm6, %%mm3		\n\t"
#ifndef FAST_BGR2YV12
		"psrad $8, %%mm4		\n\t"
		"psrad $8, %%mm1		\n\t"
		"psrad $8, %%mm2		\n\t"
		"psrad $8, %%mm3		\n\t"
#endif
		"packssdw %%mm1, %%mm4		\n\t"
		"packssdw %%mm3, %%mm2		\n\t"
		"pmaddwd %%mm5, %%mm4		\n\t"
		"pmaddwd %%mm5, %%mm2		\n\t"
		"add $24, %%"REG_b"		\n\t"
		"packssdw %%mm2, %%mm4		\n\t"
		"psraw $7, %%mm4		\n\t"

		"packuswb %%mm4, %%mm0		\n\t"
		"paddusb "MANGLE(bgr2YOffset)", %%mm0	\n\t"

		"movq %%mm0, (%1, %%"REG_a")	\n\t"
		"add $8, %%"REG_a"		\n\t"
		" js 1b				\n\t"
		: : "r" (src+width*3), "r" (dst+width), "g" ((long)-width)
		: "%"REG_a, "%"REG_b
	);
#else
	int i;
	for(i=0; i<width; i++)
	{
		int b= src[i*3+0];
		int g= src[i*3+1];
		int r= src[i*3+2];

		dst[i]= ((RY*r + GY*g + BY*b + (33<<(RGB2YUV_SHIFT-1)) )>>RGB2YUV_SHIFT);
	}
#endif
}

static inline void RENAME(bgr24ToUV)(uint8_t *dstU, uint8_t *dstV, uint8_t *src1, uint8_t *src2, int width)
{
#ifdef HAVE_MMX
	asm volatile(
		"mov %4, %%"REG_a"		\n\t"
		"movq "MANGLE(w1111)", %%mm5		\n\t"
		"movq "MANGLE(bgr2UCoeff)", %%mm6		\n\t"
		"pxor %%mm7, %%mm7		\n\t"
		"lea (%%"REG_a", %%"REG_a", 2), %%"REG_b"	\n\t"
		"add %%"REG_b", %%"REG_b"	\n\t"
		".balign 16			\n\t"
		"1:				\n\t"
		PREFETCH" 64(%0, %%"REG_b")	\n\t"
		PREFETCH" 64(%1, %%"REG_b")	\n\t"
#if defined (HAVE_MMX2) || defined (HAVE_3DNOW)
		"movq (%0, %%"REG_b"), %%mm0	\n\t"
		"movq (%1, %%"REG_b"), %%mm1	\n\t"
		"movq 6(%0, %%"REG_b"), %%mm2	\n\t"
		"movq 6(%1, %%"REG_b"), %%mm3	\n\t"
		PAVGB(%%mm1, %%mm0)
		PAVGB(%%mm3, %%mm2)
		"movq %%mm0, %%mm1		\n\t"
		"movq %%mm2, %%mm3		\n\t"
		"psrlq $24, %%mm0		\n\t"
		"psrlq $24, %%mm2		\n\t"
		PAVGB(%%mm1, %%mm0)
		PAVGB(%%mm3, %%mm2)
		"punpcklbw %%mm7, %%mm0		\n\t"
		"punpcklbw %%mm7, %%mm2		\n\t"
#else
		"movd (%0, %%"REG_b"), %%mm0	\n\t"
		"movd (%1, %%"REG_b"), %%mm1	\n\t"
		"movd 3(%0, %%"REG_b"), %%mm2	\n\t"
		"movd 3(%1, %%"REG_b"), %%mm3	\n\t"
		"punpcklbw %%mm7, %%mm0		\n\t"
		"punpcklbw %%mm7, %%mm1		\n\t"
		"punpcklbw %%mm7, %%mm2		\n\t"
		"punpcklbw %%mm7, %%mm3		\n\t"
		"paddw %%mm1, %%mm0		\n\t"
		"paddw %%mm3, %%mm2		\n\t"
		"paddw %%mm2, %%mm0		\n\t"
		"movd 6(%0, %%"REG_b"), %%mm4	\n\t"
		"movd 6(%1, %%"REG_b"), %%mm1	\n\t"
		"movd 9(%0, %%"REG_b"), %%mm2	\n\t"
		"movd 9(%1, %%"REG_b"), %%mm3	\n\t"
		"punpcklbw %%mm7, %%mm4		\n\t"
		"punpcklbw %%mm7, %%mm1		\n\t"
		"punpcklbw %%mm7, %%mm2		\n\t"
		"punpcklbw %%mm7, %%mm3		\n\t"
		"paddw %%mm1, %%mm4		\n\t"
		"paddw %%mm3, %%mm2		\n\t"
		"paddw %%mm4, %%mm2		\n\t"
		"psrlw $2, %%mm0		\n\t"
		"psrlw $2, %%mm2		\n\t"
#endif
		"movq "MANGLE(bgr2VCoeff)", %%mm1		\n\t"
		"movq "MANGLE(bgr2VCoeff)", %%mm3		\n\t"
		
		"pmaddwd %%mm0, %%mm1		\n\t"
		"pmaddwd %%mm2, %%mm3		\n\t"
		"pmaddwd %%mm6, %%mm0		\n\t"
		"pmaddwd %%mm6, %%mm2		\n\t"
#ifndef FAST_BGR2YV12
		"psrad $8, %%mm0		\n\t"
		"psrad $8, %%mm1		\n\t"
		"psrad $8, %%mm2		\n\t"
		"psrad $8, %%mm3		\n\t"
#endif
		"packssdw %%mm2, %%mm0		\n\t"
		"packssdw %%mm3, %%mm1		\n\t"
		"pmaddwd %%mm5, %%mm0		\n\t"
		"pmaddwd %%mm5, %%mm1		\n\t"
		"packssdw %%mm1, %%mm0		\n\t" // V1 V0 U1 U0
		"psraw $7, %%mm0		\n\t"

#if defined (HAVE_MMX2) || defined (HAVE_3DNOW)
		"movq 12(%0, %%"REG_b"), %%mm4	\n\t"
		"movq 12(%1, %%"REG_b"), %%mm1	\n\t"
		"movq 18(%0, %%"REG_b"), %%mm2	\n\t"
		"movq 18(%1, %%"REG_b"), %%mm3	\n\t"
		PAVGB(%%mm1, %%mm4)
		PAVGB(%%mm3, %%mm2)
		"movq %%mm4, %%mm1		\n\t"
		"movq %%mm2, %%mm3		\n\t"
		"psrlq $24, %%mm4		\n\t"
		"psrlq $24, %%mm2		\n\t"
		PAVGB(%%mm1, %%mm4)
		PAVGB(%%mm3, %%mm2)
		"punpcklbw %%mm7, %%mm4		\n\t"
		"punpcklbw %%mm7, %%mm2		\n\t"
#else
		"movd 12(%0, %%"REG_b"), %%mm4	\n\t"
		"movd 12(%1, %%"REG_b"), %%mm1	\n\t"
		"movd 15(%0, %%"REG_b"), %%mm2	\n\t"
		"movd 15(%1, %%"REG_b"), %%mm3	\n\t"
		"punpcklbw %%mm7, %%mm4		\n\t"
		"punpcklbw %%mm7, %%mm1		\n\t"
		"punpcklbw %%mm7, %%mm2		\n\t"
		"punpcklbw %%mm7, %%mm3		\n\t"
		"paddw %%mm1, %%mm4		\n\t"
		"paddw %%mm3, %%mm2		\n\t"
		"paddw %%mm2, %%mm4		\n\t"
		"movd 18(%0, %%"REG_b"), %%mm5	\n\t"
		"movd 18(%1, %%"REG_b"), %%mm1	\n\t"
		"movd 21(%0, %%"REG_b"), %%mm2	\n\t"
		"movd 21(%1, %%"REG_b"), %%mm3	\n\t"
		"punpcklbw %%mm7, %%mm5		\n\t"
		"punpcklbw %%mm7, %%mm1		\n\t"
		"punpcklbw %%mm7, %%mm2		\n\t"
		"punpcklbw %%mm7, %%mm3		\n\t"
		"paddw %%mm1, %%mm5		\n\t"
		"paddw %%mm3, %%mm2		\n\t"
		"paddw %%mm5, %%mm2		\n\t"
		"movq "MANGLE(w1111)", %%mm5		\n\t"
		"psrlw $2, %%mm4		\n\t"
		"psrlw $2, %%mm2		\n\t"
#endif
		"movq "MANGLE(bgr2VCoeff)", %%mm1		\n\t"
		"movq "MANGLE(bgr2VCoeff)", %%mm3		\n\t"
		
		"pmaddwd %%mm4, %%mm1		\n\t"
		"pmaddwd %%mm2, %%mm3		\n\t"
		"pmaddwd %%mm6, %%mm4		\n\t"
		"pmaddwd %%mm6, %%mm2		\n\t"
#ifndef FAST_BGR2YV12
		"psrad $8, %%mm4		\n\t"
		"psrad $8, %%mm1		\n\t"
		"psrad $8, %%mm2		\n\t"
		"psrad $8, %%mm3		\n\t"
#endif
		"packssdw %%mm2, %%mm4		\n\t"
		"packssdw %%mm3, %%mm1		\n\t"
		"pmaddwd %%mm5, %%mm4		\n\t"
		"pmaddwd %%mm5, %%mm1		\n\t"
		"add $24, %%"REG_b"		\n\t"
		"packssdw %%mm1, %%mm4		\n\t" // V3 V2 U3 U2
		"psraw $7, %%mm4		\n\t"
		
		"movq %%mm0, %%mm1		\n\t"
		"punpckldq %%mm4, %%mm0		\n\t"
		"punpckhdq %%mm4, %%mm1		\n\t"
		"packsswb %%mm1, %%mm0		\n\t"
		"paddb "MANGLE(bgr2UVOffset)", %%mm0	\n\t"

		"movd %%mm0, (%2, %%"REG_a")	\n\t"
		"punpckhdq %%mm0, %%mm0		\n\t"
		"movd %%mm0, (%3, %%"REG_a")	\n\t"
		"add $4, %%"REG_a"		\n\t"
		" js 1b				\n\t"
		: : "r" (src1+width*6), "r" (src2+width*6), "r" (dstU+width), "r" (dstV+width), "g" ((long)-width)
		: "%"REG_a, "%"REG_b
	);
#else
	int i;
	for(i=0; i<width; i++)
	{
		int b= src1[6*i + 0] + src1[6*i + 3] + src2[6*i + 0] + src2[6*i + 3];
		int g= src1[6*i + 1] + src1[6*i + 4] + src2[6*i + 1] + src2[6*i + 4];
		int r= src1[6*i + 2] + src1[6*i + 5] + src2[6*i + 2] + src2[6*i + 5];

		dstU[i]= ((RU*r + GU*g + BU*b)>>(RGB2YUV_SHIFT+2)) + 128;
		dstV[i]= ((RV*r + GV*g + BV*b)>>(RGB2YUV_SHIFT+2)) + 128;
	}
#endif
}

static inline void RENAME(bgr16ToY)(uint8_t *dst, uint8_t *src, int width)
{
	int i;
	for(i=0; i<width; i++)
	{
		int d= ((uint16_t*)src)[i];
		int b= d&0x1F;
		int g= (d>>5)&0x3F;
		int r= (d>>11)&0x1F;

		dst[i]= ((2*RY*r + GY*g + 2*BY*b)>>(RGB2YUV_SHIFT-2)) + 16;
	}
}

static inline void RENAME(bgr16ToUV)(uint8_t *dstU, uint8_t *dstV, uint8_t *src1, uint8_t *src2, int width)
{
	int i;
	for(i=0; i<width; i++)
	{
		int d0= ((uint32_t*)src1)[i];
		int d1= ((uint32_t*)src2)[i];
		
		int dl= (d0&0x07E0F81F) + (d1&0x07E0F81F);
		int dh= ((d0>>5)&0x07C0F83F) + ((d1>>5)&0x07C0F83F);

		int dh2= (dh>>11) + (dh<<21);
		int d= dh2 + dl;

		int b= d&0x7F;
		int r= (d>>11)&0x7F;
		int g= d>>21;
		dstU[i]= ((2*RU*r + GU*g + 2*BU*b)>>(RGB2YUV_SHIFT+2-2)) + 128;
		dstV[i]= ((2*RV*r + GV*g + 2*BV*b)>>(RGB2YUV_SHIFT+2-2)) + 128;
	}
}

static inline void RENAME(bgr15ToY)(uint8_t *dst, uint8_t *src, int width)
{
	int i;
	for(i=0; i<width; i++)
	{
		int d= ((uint16_t*)src)[i];
		int b= d&0x1F;
		int g= (d>>5)&0x1F;
		int r= (d>>10)&0x1F;

		dst[i]= ((RY*r + GY*g + BY*b)>>(RGB2YUV_SHIFT-3)) + 16;
	}
}

static inline void RENAME(bgr15ToUV)(uint8_t *dstU, uint8_t *dstV, uint8_t *src1, uint8_t *src2, int width)
{
	int i;
	for(i=0; i<width; i++)
	{
		int d0= ((uint32_t*)src1)[i];
		int d1= ((uint32_t*)src2)[i];
		
		int dl= (d0&0x03E07C1F) + (d1&0x03E07C1F);
		int dh= ((d0>>5)&0x03E0F81F) + ((d1>>5)&0x03E0F81F);

		int dh2= (dh>>11) + (dh<<21);
		int d= dh2 + dl;

		int b= d&0x7F;
		int r= (d>>10)&0x7F;
		int g= d>>21;
		dstU[i]= ((RU*r + GU*g + BU*b)>>(RGB2YUV_SHIFT+2-3)) + 128;
		dstV[i]= ((RV*r + GV*g + BV*b)>>(RGB2YUV_SHIFT+2-3)) + 128;
	}
}


static inline void RENAME(rgb32ToY)(uint8_t *dst, uint8_t *src, int width)
{
	int i;
	for(i=0; i<width; i++)
	{
		int r=  ((uint32_t*)src)[i]&0xFF;
		int g= (((uint32_t*)src)[i]>>8)&0xFF;
		int b= (((uint32_t*)src)[i]>>16)&0xFF;

		dst[i]= ((RY*r + GY*g + BY*b + (33<<(RGB2YUV_SHIFT-1)) )>>RGB2YUV_SHIFT);
	}
}

static inline void RENAME(rgb32ToUV)(uint8_t *dstU, uint8_t *dstV, uint8_t *src1, uint8_t *src2, int width)
{
	int i;
	for(i=0; i<width; i++)
	{
		const int a= ((uint32_t*)src1)[2*i+0];
		const int e= ((uint32_t*)src1)[2*i+1];
		const int c= ((uint32_t*)src2)[2*i+0];
		const int d= ((uint32_t*)src2)[2*i+1];
		const int l= (a&0xFF00FF) + (e&0xFF00FF) + (c&0xFF00FF) + (d&0xFF00FF);
		const int h= (a&0x00FF00) + (e&0x00FF00) + (c&0x00FF00) + (d&0x00FF00);
 		const int r=  l&0x3FF;
		const int g=  h>>8;
		const int b=  l>>16;

		dstU[i]= ((RU*r + GU*g + BU*b)>>(RGB2YUV_SHIFT+2)) + 128;
		dstV[i]= ((RV*r + GV*g + BV*b)>>(RGB2YUV_SHIFT+2)) + 128;
	}
}

static inline void RENAME(rgb24ToY)(uint8_t *dst, uint8_t *src, int width)
{
	int i;
	for(i=0; i<width; i++)
	{
		int r= src[i*3+0];
		int g= src[i*3+1];
		int b= src[i*3+2];

		dst[i]= ((RY*r + GY*g + BY*b + (33<<(RGB2YUV_SHIFT-1)) )>>RGB2YUV_SHIFT);
	}
}

static inline void RENAME(rgb24ToUV)(uint8_t *dstU, uint8_t *dstV, uint8_t *src1, uint8_t *src2, int width)
{
	int i;
	for(i=0; i<width; i++)
	{
		int r= src1[6*i + 0] + src1[6*i + 3] + src2[6*i + 0] + src2[6*i + 3];
		int g= src1[6*i + 1] + src1[6*i + 4] + src2[6*i + 1] + src2[6*i + 4];
		int b= src1[6*i + 2] + src1[6*i + 5] + src2[6*i + 2] + src2[6*i + 5];

		dstU[i]= ((RU*r + GU*g + BU*b)>>(RGB2YUV_SHIFT+2)) + 128;
		dstV[i]= ((RV*r + GV*g + BV*b)>>(RGB2YUV_SHIFT+2)) + 128;
	}
}


// Bilinear / Bicubic scaling
static inline void RENAME(hScale)(int16_t *dst, int dstW, uint8_t *src, int srcW, int xInc,
				  int16_t *filter, int16_t *filterPos, int filterSize)
{
#ifdef HAVE_MMX
	assert(filterSize % 4 == 0 && filterSize>0);
	if(filterSize==4) // allways true for upscaling, sometimes for down too
	{
		long counter= -2*dstW;
		filter-= counter*2;
		filterPos-= counter/2;
		dst-= counter/2;
		asm volatile(
			"pxor %%mm7, %%mm7		\n\t"
			"movq "MANGLE(w02)", %%mm6	\n\t"
			"push %%"REG_BP"		\n\t" // we use 7 regs here ...
			"mov %%"REG_a", %%"REG_BP"	\n\t"
			".balign 16			\n\t"
			"1:				\n\t"
			"movzwl (%2, %%"REG_BP"), %%eax	\n\t"
			"movzwl 2(%2, %%"REG_BP"), %%ebx\n\t"
			"movq (%1, %%"REG_BP", 4), %%mm1\n\t"
			"movq 8(%1, %%"REG_BP", 4), %%mm3\n\t"
			"movd (%3, %%"REG_a"), %%mm0	\n\t"
			"movd (%3, %%"REG_b"), %%mm2	\n\t"
			"punpcklbw %%mm7, %%mm0		\n\t"
			"punpcklbw %%mm7, %%mm2		\n\t"
			"pmaddwd %%mm1, %%mm0		\n\t"
			"pmaddwd %%mm2, %%mm3		\n\t"
			"psrad $8, %%mm0		\n\t"
			"psrad $8, %%mm3		\n\t"
			"packssdw %%mm3, %%mm0		\n\t"
			"pmaddwd %%mm6, %%mm0		\n\t"
			"packssdw %%mm0, %%mm0		\n\t"
			"movd %%mm0, (%4, %%"REG_BP")	\n\t"
			"add $4, %%"REG_BP"		\n\t"
			" jnc 1b			\n\t"

			"pop %%"REG_BP"			\n\t"
			: "+a" (counter)
			: "c" (filter), "d" (filterPos), "S" (src), "D" (dst)
			: "%"REG_b
		);
	}
	else if(filterSize==8)
	{
		long counter= -2*dstW;
		filter-= counter*4;
		filterPos-= counter/2;
		dst-= counter/2;
		asm volatile(
			"pxor %%mm7, %%mm7		\n\t"
			"movq "MANGLE(w02)", %%mm6	\n\t"
			"push %%"REG_BP"		\n\t" // we use 7 regs here ...
			"mov %%"REG_a", %%"REG_BP"	\n\t"
			".balign 16			\n\t"
			"1:				\n\t"
			"movzwl (%2, %%"REG_BP"), %%eax	\n\t"
			"movzwl 2(%2, %%"REG_BP"), %%ebx\n\t"
			"movq (%1, %%"REG_BP", 8), %%mm1\n\t"
			"movq 16(%1, %%"REG_BP", 8), %%mm3\n\t"
			"movd (%3, %%"REG_a"), %%mm0	\n\t"
			"movd (%3, %%"REG_b"), %%mm2	\n\t"
			"punpcklbw %%mm7, %%mm0		\n\t"
			"punpcklbw %%mm7, %%mm2		\n\t"
			"pmaddwd %%mm1, %%mm0		\n\t"
			"pmaddwd %%mm2, %%mm3		\n\t"

			"movq 8(%1, %%"REG_BP", 8), %%mm1\n\t"
			"movq 24(%1, %%"REG_BP", 8), %%mm5\n\t"
			"movd 4(%3, %%"REG_a"), %%mm4	\n\t"
			"movd 4(%3, %%"REG_b"), %%mm2	\n\t"
			"punpcklbw %%mm7, %%mm4		\n\t"
			"punpcklbw %%mm7, %%mm2		\n\t"
			"pmaddwd %%mm1, %%mm4		\n\t"
			"pmaddwd %%mm2, %%mm5		\n\t"
			"paddd %%mm4, %%mm0		\n\t"
			"paddd %%mm5, %%mm3		\n\t"
						
			"psrad $8, %%mm0		\n\t"
			"psrad $8, %%mm3		\n\t"
			"packssdw %%mm3, %%mm0		\n\t"
			"pmaddwd %%mm6, %%mm0		\n\t"
			"packssdw %%mm0, %%mm0		\n\t"
			"movd %%mm0, (%4, %%"REG_BP")	\n\t"
			"add $4, %%"REG_BP"		\n\t"
			" jnc 1b			\n\t"

			"pop %%"REG_BP"			\n\t"
			: "+a" (counter)
			: "c" (filter), "d" (filterPos), "S" (src), "D" (dst)
			: "%"REG_b
		);
	}
	else
	{
		long counter= -2*dstW;
		uint8_t * src_fsize=src+filterSize;
//		filter-= counter*filterSize/2;
		filterPos-= counter/2;
		dst-= counter/2;
		asm volatile(
			"pxor %%mm7, %%mm7		\n\t"
			"movq "MANGLE(w02)", %%mm6	\n\t"
			".balign 16			\n\t"
			"1:				\n\t"
			"mov %2, %%"REG_c"		\n\t"
			"movzwl (%%"REG_c", %0), %%eax	\n\t"
			"movzwl 2(%%"REG_c", %0), %%ebx	\n\t"
			"mov %5, %%"REG_c"		\n\t"
			"pxor %%mm4, %%mm4		\n\t"
			"pxor %%mm5, %%mm5		\n\t"
			"2:				\n\t"
			"movq (%1), %%mm1		\n\t"
			"movq (%1, %6), %%mm3		\n\t"
			"movd (%%"REG_c", %%"REG_a"), %%mm0\n\t"
			"movd (%%"REG_c", %%"REG_b"), %%mm2\n\t"
			"punpcklbw %%mm7, %%mm0		\n\t"
			"punpcklbw %%mm7, %%mm2		\n\t"
			"pmaddwd %%mm1, %%mm0		\n\t"
			"pmaddwd %%mm2, %%mm3		\n\t"
			"paddd %%mm3, %%mm5		\n\t"
			"paddd %%mm0, %%mm4		\n\t"
			"add $8, %1			\n\t"
			"add $4, %%"REG_c"		\n\t"
			"cmp %4, %%"REG_c"		\n\t"
			" jb 2b				\n\t"
			"add %6, %1			\n\t"
			"psrad $8, %%mm4		\n\t"
			"psrad $8, %%mm5		\n\t"
			"packssdw %%mm5, %%mm4		\n\t"
			"pmaddwd %%mm6, %%mm4		\n\t"
			"packssdw %%mm4, %%mm4		\n\t"
			"mov %3, %%"REG_a"		\n\t"
			"movd %%mm4, (%%"REG_a", %0)	\n\t"
			"add $4, %0			\n\t"
			" jnc 1b			\n\t"

			: "+r" (counter), "+r" (filter)
			: "m" (filterPos), "m" (dst), "m"(src_fsize),
			  "m" (src), "r" ((long)filterSize*2)
			: "%"REG_b, "%"REG_a, "%"REG_c
		);
	}
#else
#ifdef HAVE_ALTIVEC
	hScale_altivec_real(dst, dstW, src, srcW, xInc, filter, filterPos, filterSize);
#else
	int i;
	for(i=0; i<dstW; i++)
	{
		int j;
		int srcPos= filterPos[i];
		int val=0;
//		printf("filterPos: %d\n", filterPos[i]);
		for(j=0; j<filterSize; j++)
		{
//			printf("filter: %d, src: %d\n", filter[i], src[srcPos + j]);
			val += ((int)src[srcPos + j])*filter[filterSize*i + j];
		}
//		filter += hFilterSize;
		dst[i] = MIN(MAX(0, val>>7), (1<<15)-1); // the cubic equation does overflow ...
//		dst[i] = val>>7;
	}
#endif
#endif
}
      // *** horizontal scale Y line to temp buffer
static inline void RENAME(hyscale)(uint16_t *dst, int dstWidth, uint8_t *src, int srcW, int xInc,
				   int flags, int canMMX2BeUsed, int16_t *hLumFilter,
				   int16_t *hLumFilterPos, int hLumFilterSize, void *funnyYCode, 
				   int srcFormat, uint8_t *formatConvBuffer, int16_t *mmx2Filter,
				   int32_t *mmx2FilterPos)
{
	int xIncLow=xInc&0xffff,xIncHigh=xInc>>16;
    if(srcFormat==IMGFMT_YUY2)
    {
	RENAME(yuy2ToY)(formatConvBuffer, src, srcW);
	src= formatConvBuffer;
    }
    else if(srcFormat==IMGFMT_UYVY)
    {
	RENAME(uyvyToY)(formatConvBuffer, src, srcW);
	src= formatConvBuffer;
    }
    else if(srcFormat==IMGFMT_BGR32)
    {
	RENAME(bgr32ToY)(formatConvBuffer, src, srcW);
	src= formatConvBuffer;
    }
    else if(srcFormat==IMGFMT_BGR24)
    {
	RENAME(bgr24ToY)(formatConvBuffer, src, srcW);
	src= formatConvBuffer;
    }
    else if(srcFormat==IMGFMT_BGR16)
    {
	RENAME(bgr16ToY)(formatConvBuffer, src, srcW);
	src= formatConvBuffer;
    }
    else if(srcFormat==IMGFMT_BGR15)
    {
	RENAME(bgr15ToY)(formatConvBuffer, src, srcW);
	src= formatConvBuffer;
    }
    else if(srcFormat==IMGFMT_RGB32)
    {
	RENAME(rgb32ToY)(formatConvBuffer, src, srcW);
	src= formatConvBuffer;
    }
    else if(srcFormat==IMGFMT_RGB24)
    {
	RENAME(rgb24ToY)(formatConvBuffer, src, srcW);
	src= formatConvBuffer;
    }

#ifdef HAVE_MMX
	// use the new MMX scaler if the mmx2 can't be used (its faster than the x86asm one)
    if(!(flags&SWS_FAST_BILINEAR) || (!canMMX2BeUsed))
#else
    if(!(flags&SWS_FAST_BILINEAR))
#endif
    {
    	RENAME(hScale)(dst, dstWidth, src, srcW, xInc, hLumFilter, hLumFilterPos, hLumFilterSize);
    }
    else // Fast Bilinear upscale / crap downscale
    {
#if defined(ARCH_X86) || defined(ARCH_X86_64)
#ifdef HAVE_MMX2
	int i;
	if(canMMX2BeUsed)
	{
		asm volatile(
			"pxor %%mm7, %%mm7		\n\t"
			"mov %0, %%"REG_c"		\n\t"
			"mov %1, %%"REG_D"		\n\t"
			"mov %2, %%"REG_d"		\n\t"
			"mov %3, %%"REG_b"		\n\t"
			"xor %%"REG_a", %%"REG_a"	\n\t" // i
			PREFETCH" (%%"REG_c")		\n\t"
			PREFETCH" 32(%%"REG_c")		\n\t"
			PREFETCH" 64(%%"REG_c")		\n\t"

#ifdef ARCH_X86_64

#define FUNNY_Y_CODE \
			"movl (%%"REG_b"), %%esi	\n\t"\
			"call *%4			\n\t"\
			"movl (%%"REG_b", %%"REG_a"), %%esi\n\t"\
			"add %%"REG_S", %%"REG_c"	\n\t"\
			"add %%"REG_a", %%"REG_D"	\n\t"\
			"xor %%"REG_a", %%"REG_a"	\n\t"\

#else

#define FUNNY_Y_CODE \
			"movl (%%"REG_b"), %%esi	\n\t"\
			"call *%4			\n\t"\
			"addl (%%"REG_b", %%"REG_a"), %%"REG_c"\n\t"\
			"add %%"REG_a", %%"REG_D"	\n\t"\
			"xor %%"REG_a", %%"REG_a"	\n\t"\

#endif

FUNNY_Y_CODE
FUNNY_Y_CODE
FUNNY_Y_CODE
FUNNY_Y_CODE
FUNNY_Y_CODE
FUNNY_Y_CODE
FUNNY_Y_CODE
FUNNY_Y_CODE

			:: "m" (src), "m" (dst), "m" (mmx2Filter), "m" (mmx2FilterPos),
			"m" (funnyYCode)
			: "%"REG_a, "%"REG_b, "%"REG_c, "%"REG_d, "%"REG_S, "%"REG_D
		);
		for(i=dstWidth-1; (i*xInc)>>16 >=srcW-1; i--) dst[i] = src[srcW-1]*128;
	}
	else
	{
#endif
	//NO MMX just normal asm ...
	asm volatile(
		"xor %%"REG_a", %%"REG_a"	\n\t" // i
		"xor %%"REG_b", %%"REG_b"	\n\t" // xx
		"xorl %%ecx, %%ecx		\n\t" // 2*xalpha
		".balign 16			\n\t"
		"1:				\n\t"
		"movzbl  (%0, %%"REG_b"), %%edi	\n\t" //src[xx]
		"movzbl 1(%0, %%"REG_b"), %%esi	\n\t" //src[xx+1]
		"subl %%edi, %%esi		\n\t" //src[xx+1] - src[xx]
		"imull %%ecx, %%esi		\n\t" //(src[xx+1] - src[xx])*2*xalpha
		"shll $16, %%edi		\n\t"
		"addl %%edi, %%esi		\n\t" //src[xx+1]*2*xalpha + src[xx]*(1-2*xalpha)
		"mov %1, %%"REG_D"		\n\t"
		"shrl $9, %%esi			\n\t"
		"movw %%si, (%%"REG_D", %%"REG_a", 2)\n\t"
		"addw %4, %%cx			\n\t" //2*xalpha += xInc&0xFF
		"adc %3, %%"REG_b"		\n\t" //xx+= xInc>>8 + carry

		"movzbl (%0, %%"REG_b"), %%edi	\n\t" //src[xx]
		"movzbl 1(%0, %%"REG_b"), %%esi	\n\t" //src[xx+1]
		"subl %%edi, %%esi		\n\t" //src[xx+1] - src[xx]
		"imull %%ecx, %%esi		\n\t" //(src[xx+1] - src[xx])*2*xalpha
		"shll $16, %%edi		\n\t"
		"addl %%edi, %%esi		\n\t" //src[xx+1]*2*xalpha + src[xx]*(1-2*xalpha)
		"mov %1, %%"REG_D"		\n\t"
		"shrl $9, %%esi			\n\t"
		"movw %%si, 2(%%"REG_D", %%"REG_a", 2)\n\t"
		"addw %4, %%cx			\n\t" //2*xalpha += xInc&0xFF
		"adc %3, %%"REG_b"		\n\t" //xx+= xInc>>8 + carry


		"add $2, %%"REG_a"		\n\t"
		"cmp %2, %%"REG_a"		\n\t"
		" jb 1b				\n\t"


		:: "r" (src), "m" (dst), "m" (dstWidth), "m" (xIncHigh), "m" (xIncLow)
		: "%"REG_a, "%"REG_b, "%ecx", "%"REG_D, "%esi"
		);
#ifdef HAVE_MMX2
	} //if MMX2 can't be used
#endif
#else
	int i;
	unsigned int xpos=0;
	for(i=0;i<dstWidth;i++)
	{
		register unsigned int xx=xpos>>16;
		register unsigned int xalpha=(xpos&0xFFFF)>>9;
		dst[i]= (src[xx]<<7) + (src[xx+1] - src[xx])*xalpha;
		xpos+=xInc;
	}
#endif
    }
}

inline static void RENAME(hcscale)(uint16_t *dst, int dstWidth, uint8_t *src1, uint8_t *src2,
				   int srcW, int xInc, int flags, int canMMX2BeUsed, int16_t *hChrFilter,
				   int16_t *hChrFilterPos, int hChrFilterSize, void *funnyUVCode,
				   int srcFormat, uint8_t *formatConvBuffer, int16_t *mmx2Filter,
				   int32_t *mmx2FilterPos)
{
	int xIncLow=xInc&0xffff,xIncHigh=xInc>>16;
    if(srcFormat==IMGFMT_YUY2)
    {
	RENAME(yuy2ToUV)(formatConvBuffer, formatConvBuffer+2048, src1, src2, srcW);
	src1= formatConvBuffer;
	src2= formatConvBuffer+2048;
    }
    else if(srcFormat==IMGFMT_UYVY)
    {
	RENAME(uyvyToUV)(formatConvBuffer, formatConvBuffer+2048, src1, src2, srcW);
	src1= formatConvBuffer;
	src2= formatConvBuffer+2048;
    }
    else if(srcFormat==IMGFMT_BGR32)
    {
	RENAME(bgr32ToUV)(formatConvBuffer, formatConvBuffer+2048, src1, src2, srcW);
	src1= formatConvBuffer;
	src2= formatConvBuffer+2048;
    }
    else if(srcFormat==IMGFMT_BGR24)
    {
	RENAME(bgr24ToUV)(formatConvBuffer, formatConvBuffer+2048, src1, src2, srcW);
	src1= formatConvBuffer;
	src2= formatConvBuffer+2048;
    }
    else if(srcFormat==IMGFMT_BGR16)
    {
	RENAME(bgr16ToUV)(formatConvBuffer, formatConvBuffer+2048, src1, src2, srcW);
	src1= formatConvBuffer;
	src2= formatConvBuffer+2048;
    }
    else if(srcFormat==IMGFMT_BGR15)
    {
	RENAME(bgr15ToUV)(formatConvBuffer, formatConvBuffer+2048, src1, src2, srcW);
	src1= formatConvBuffer;
	src2= formatConvBuffer+2048;
    }
    else if(srcFormat==IMGFMT_RGB32)
    {
	RENAME(rgb32ToUV)(formatConvBuffer, formatConvBuffer+2048, src1, src2, srcW);
	src1= formatConvBuffer;
	src2= formatConvBuffer+2048;
    }
    else if(srcFormat==IMGFMT_RGB24)
    {
	RENAME(rgb24ToUV)(formatConvBuffer, formatConvBuffer+2048, src1, src2, srcW);
	src1= formatConvBuffer;
	src2= formatConvBuffer+2048;
    }
    else if(isGray(srcFormat))
    {
    	return;
    }

#ifdef HAVE_MMX
	// use the new MMX scaler if the mmx2 can't be used (its faster than the x86asm one)
    if(!(flags&SWS_FAST_BILINEAR) || (!canMMX2BeUsed))
#else
    if(!(flags&SWS_FAST_BILINEAR))
#endif
    {
    	RENAME(hScale)(dst     , dstWidth, src1, srcW, xInc, hChrFilter, hChrFilterPos, hChrFilterSize);
    	RENAME(hScale)(dst+2048, dstWidth, src2, srcW, xInc, hChrFilter, hChrFilterPos, hChrFilterSize);
    }
    else // Fast Bilinear upscale / crap downscale
    {
#if defined(ARCH_X86) || defined(ARCH_X86_64)
#ifdef HAVE_MMX2
	int i;
	if(canMMX2BeUsed)
	{
		asm volatile(
			"pxor %%mm7, %%mm7		\n\t"
			"mov %0, %%"REG_c"		\n\t"
			"mov %1, %%"REG_D"		\n\t"
			"mov %2, %%"REG_d"		\n\t"
			"mov %3, %%"REG_b"		\n\t"
			"xor %%"REG_a", %%"REG_a"	\n\t" // i
			PREFETCH" (%%"REG_c")		\n\t"
			PREFETCH" 32(%%"REG_c")		\n\t"
			PREFETCH" 64(%%"REG_c")		\n\t"

#ifdef ARCH_X86_64

#define FUNNY_UV_CODE \
			"movl (%%"REG_b"), %%esi	\n\t"\
			"call *%4			\n\t"\
			"movl (%%"REG_b", %%"REG_a"), %%esi\n\t"\
			"add %%"REG_S", %%"REG_c"	\n\t"\
			"add %%"REG_a", %%"REG_D"	\n\t"\
			"xor %%"REG_a", %%"REG_a"	\n\t"\

#else

#define FUNNY_UV_CODE \
			"movl (%%"REG_b"), %%esi	\n\t"\
			"call *%4			\n\t"\
			"addl (%%"REG_b", %%"REG_a"), %%"REG_c"\n\t"\
			"add %%"REG_a", %%"REG_D"	\n\t"\
			"xor %%"REG_a", %%"REG_a"	\n\t"\

#endif

FUNNY_UV_CODE
FUNNY_UV_CODE
FUNNY_UV_CODE
FUNNY_UV_CODE
			"xor %%"REG_a", %%"REG_a"	\n\t" // i
			"mov %5, %%"REG_c"		\n\t" // src
			"mov %1, %%"REG_D"		\n\t" // buf1
			"add $4096, %%"REG_D"		\n\t"
			PREFETCH" (%%"REG_c")		\n\t"
			PREFETCH" 32(%%"REG_c")		\n\t"
			PREFETCH" 64(%%"REG_c")		\n\t"

FUNNY_UV_CODE
FUNNY_UV_CODE
FUNNY_UV_CODE
FUNNY_UV_CODE

			:: "m" (src1), "m" (dst), "m" (mmx2Filter), "m" (mmx2FilterPos),
			"m" (funnyUVCode), "m" (src2)
			: "%"REG_a, "%"REG_b, "%"REG_c, "%"REG_d, "%"REG_S, "%"REG_D
		);
		for(i=dstWidth-1; (i*xInc)>>16 >=srcW-1; i--)
		{
//			printf("%d %d %d\n", dstWidth, i, srcW);
			dst[i] = src1[srcW-1]*128;
			dst[i+2048] = src2[srcW-1]*128;
		}
	}
	else
	{
#endif
	asm volatile(
		"xor %%"REG_a", %%"REG_a"	\n\t" // i
		"xor %%"REG_b", %%"REG_b"		\n\t" // xx
		"xorl %%ecx, %%ecx		\n\t" // 2*xalpha
		".balign 16			\n\t"
		"1:				\n\t"
		"mov %0, %%"REG_S"		\n\t"
		"movzbl  (%%"REG_S", %%"REG_b"), %%edi	\n\t" //src[xx]
		"movzbl 1(%%"REG_S", %%"REG_b"), %%esi	\n\t" //src[xx+1]
		"subl %%edi, %%esi		\n\t" //src[xx+1] - src[xx]
		"imull %%ecx, %%esi		\n\t" //(src[xx+1] - src[xx])*2*xalpha
		"shll $16, %%edi		\n\t"
		"addl %%edi, %%esi		\n\t" //src[xx+1]*2*xalpha + src[xx]*(1-2*xalpha)
		"mov %1, %%"REG_D"		\n\t"
		"shrl $9, %%esi			\n\t"
		"movw %%si, (%%"REG_d", %%"REG_a", 2)\n\t"

		"movzbl  (%5, %%"REG_b"), %%edi	\n\t" //src[xx]
		"movzbl 1(%5, %%"REG_b"), %%esi	\n\t" //src[xx+1]
		"subl %%edi, %%esi		\n\t" //src[xx+1] - src[xx]
		"imull %%ecx, %%esi		\n\t" //(src[xx+1] - src[xx])*2*xalpha
		"shll $16, %%edi		\n\t"
		"addl %%edi, %%esi		\n\t" //src[xx+1]*2*xalpha + src[xx]*(1-2*xalpha)
		"mov %1, %%"REG_D"		\n\t"
		"shrl $9, %%esi			\n\t"
		"movw %%si, 4096(%%"REG_D", %%"REG_a", 2)\n\t"

		"addw %4, %%cx			\n\t" //2*xalpha += xInc&0xFF
		"adc %3, %%"REG_b"		\n\t" //xx+= xInc>>8 + carry
		"add $1, %%"REG_a"		\n\t"
		"cmp %2, %%"REG_a"		\n\t"
		" jb 1b				\n\t"

		:: "m" (src1), "m" (dst), "m" ((long)dstWidth), "m" ((long)(xIncHigh)), "m" ((xIncLow)),
		"r" (src2)
		: "%"REG_a, "%"REG_b, "%ecx", "%"REG_D, "%esi"
		);
#ifdef HAVE_MMX2
	} //if MMX2 can't be used
#endif
#else
	int i;
	unsigned int xpos=0;
	for(i=0;i<dstWidth;i++)
	{
		register unsigned int xx=xpos>>16;
		register unsigned int xalpha=(xpos&0xFFFF)>>9;
		dst[i]=(src1[xx]*(xalpha^127)+src1[xx+1]*xalpha);
		dst[i+2048]=(src2[xx]*(xalpha^127)+src2[xx+1]*xalpha);
/* slower
	  dst[i]= (src1[xx]<<7) + (src1[xx+1] - src1[xx])*xalpha;
	  dst[i+2048]=(src2[xx]<<7) + (src2[xx+1] - src2[xx])*xalpha;
*/
		xpos+=xInc;
	}
#endif
   }
}

static int RENAME(swScale)(SwsContext *c, uint8_t* src[], int srcStride[], int srcSliceY,
             int srcSliceH, uint8_t* dst[], int dstStride[]){

	/* load a few things into local vars to make the code more readable? and faster */
	const int srcW= c->srcW;
	const int dstW= c->dstW;
	const int dstH= c->dstH;
	const int chrDstW= c->chrDstW;
	const int chrSrcW= c->chrSrcW;
	const int lumXInc= c->lumXInc;
	const int chrXInc= c->chrXInc;
	const int dstFormat= c->dstFormat;
	const int srcFormat= c->srcFormat;
	const int flags= c->flags;
	const int canMMX2BeUsed= c->canMMX2BeUsed;
	int16_t *vLumFilterPos= c->vLumFilterPos;
	int16_t *vChrFilterPos= c->vChrFilterPos;
	int16_t *hLumFilterPos= c->hLumFilterPos;
	int16_t *hChrFilterPos= c->hChrFilterPos;
	int16_t *vLumFilter= c->vLumFilter;
	int16_t *vChrFilter= c->vChrFilter;
	int16_t *hLumFilter= c->hLumFilter;
	int16_t *hChrFilter= c->hChrFilter;
	int32_t *lumMmxFilter= c->lumMmxFilter;
	int32_t *chrMmxFilter= c->chrMmxFilter;
	const int vLumFilterSize= c->vLumFilterSize;
	const int vChrFilterSize= c->vChrFilterSize;
	const int hLumFilterSize= c->hLumFilterSize;
	const int hChrFilterSize= c->hChrFilterSize;
	int16_t **lumPixBuf= c->lumPixBuf;
	int16_t **chrPixBuf= c->chrPixBuf;
	const int vLumBufSize= c->vLumBufSize;
	const int vChrBufSize= c->vChrBufSize;
	uint8_t *funnyYCode= c->funnyYCode;
	uint8_t *funnyUVCode= c->funnyUVCode;
	uint8_t *formatConvBuffer= c->formatConvBuffer;
	const int chrSrcSliceY= srcSliceY >> c->chrSrcVSubSample;
	const int chrSrcSliceH= -((-srcSliceH) >> c->chrSrcVSubSample);
	int lastDstY;

	/* vars whch will change and which we need to storw back in the context */
	int dstY= c->dstY;
	int lumBufIndex= c->lumBufIndex;
	int chrBufIndex= c->chrBufIndex;
	int lastInLumBuf= c->lastInLumBuf;
	int lastInChrBuf= c->lastInChrBuf;
	
	if(isPacked(c->srcFormat)){
		src[0]=
		src[1]=
		src[2]= src[0];
		srcStride[0]=
		srcStride[1]=
		srcStride[2]= srcStride[0];
	}
	srcStride[1]<<= c->vChrDrop;
	srcStride[2]<<= c->vChrDrop;

//	printf("swscale %X %X %X -> %X %X %X\n", (int)src[0], (int)src[1], (int)src[2],
//		(int)dst[0], (int)dst[1], (int)dst[2]);

#if 0 //self test FIXME move to a vfilter or something
{
static volatile int i=0;
i++;
if(srcFormat==IMGFMT_YV12 && i==1 && srcSliceH>= c->srcH)
	selfTest(src, srcStride, c->srcW, c->srcH);
i--;
}
#endif

//printf("sws Strides:%d %d %d -> %d %d %d\n", srcStride[0],srcStride[1],srcStride[2],
//dstStride[0],dstStride[1],dstStride[2]);

	if(dstStride[0]%8 !=0 || dstStride[1]%8 !=0 || dstStride[2]%8 !=0)
	{
		static int firstTime=1; //FIXME move this into the context perhaps
		if(flags & SWS_PRINT_INFO && firstTime)
		{
			MSG_WARN("SwScaler: Warning: dstStride is not aligned!\n"
					"SwScaler:          ->cannot do aligned memory acesses anymore\n");
			firstTime=0;
		}
	}

	/* Note the user might start scaling the picture in the middle so this will not get executed
	   this is not really intended but works currently, so ppl might do it */
	if(srcSliceY ==0){
		lumBufIndex=0;
		chrBufIndex=0;
		dstY=0;	
		lastInLumBuf= -1;
		lastInChrBuf= -1;
	}

	lastDstY= dstY;

	for(;dstY < dstH; dstY++){
		unsigned char *dest =dst[0]+dstStride[0]*dstY;
		const int chrDstY= dstY>>c->chrDstVSubSample;
		unsigned char *uDest=dst[1]+dstStride[1]*chrDstY;
		unsigned char *vDest=dst[2]+dstStride[2]*chrDstY;

		const int firstLumSrcY= vLumFilterPos[dstY]; //First line needed as input
		const int firstChrSrcY= vChrFilterPos[chrDstY]; //First line needed as input
		const int lastLumSrcY= firstLumSrcY + vLumFilterSize -1; // Last line needed as input
		const int lastChrSrcY= firstChrSrcY + vChrFilterSize -1; // Last line needed as input

//printf("dstY:%d dstH:%d firstLumSrcY:%d lastInLumBuf:%d vLumBufSize: %d vChrBufSize: %d slice: %d %d vLumFilterSize: %d firstChrSrcY: %d vChrFilterSize: %d c->chrSrcVSubSample: %d\n",
// dstY, dstH, firstLumSrcY, lastInLumBuf, vLumBufSize, vChrBufSize, srcSliceY, srcSliceH, vLumFilterSize, firstChrSrcY, vChrFilterSize,  c->chrSrcVSubSample);
		//handle holes (FAST_BILINEAR & weird filters)
		if(firstLumSrcY > lastInLumBuf) lastInLumBuf= firstLumSrcY-1;
		if(firstChrSrcY > lastInChrBuf) lastInChrBuf= firstChrSrcY-1;
//printf("%d %d %d\n", firstChrSrcY, lastInChrBuf, vChrBufSize);
		ASSERT(firstLumSrcY >= lastInLumBuf - vLumBufSize + 1)
		ASSERT(firstChrSrcY >= lastInChrBuf - vChrBufSize + 1)

		// Do we have enough lines in this slice to output the dstY line
		if(lastLumSrcY < srcSliceY + srcSliceH && lastChrSrcY < -((-srcSliceY - srcSliceH)>>c->chrSrcVSubSample))
		{
			//Do horizontal scaling
			while(lastInLumBuf < lastLumSrcY)
			{
				uint8_t *s= src[0]+(lastInLumBuf + 1 - srcSliceY)*srcStride[0];
				lumBufIndex++;
//				printf("%d %d %d %d\n", lumBufIndex, vLumBufSize, lastInLumBuf,  lastLumSrcY);
				ASSERT(lumBufIndex < 2*vLumBufSize)
				ASSERT(lastInLumBuf + 1 - srcSliceY < srcSliceH)
				ASSERT(lastInLumBuf + 1 - srcSliceY >= 0)
//				printf("%d %d\n", lumBufIndex, vLumBufSize);
				RENAME(hyscale)(lumPixBuf[ lumBufIndex ], dstW, s, srcW, lumXInc,
						flags, canMMX2BeUsed, hLumFilter, hLumFilterPos, hLumFilterSize,
						funnyYCode, c->srcFormat, formatConvBuffer, 
						c->lumMmx2Filter, c->lumMmx2FilterPos);
				lastInLumBuf++;
			}
			while(lastInChrBuf < lastChrSrcY)
			{
				uint8_t *src1= src[1]+(lastInChrBuf + 1 - chrSrcSliceY)*srcStride[1];
				uint8_t *src2= src[2]+(lastInChrBuf + 1 - chrSrcSliceY)*srcStride[2];
				chrBufIndex++;
				ASSERT(chrBufIndex < 2*vChrBufSize)
				ASSERT(lastInChrBuf + 1 - chrSrcSliceY < (chrSrcSliceH))
				ASSERT(lastInChrBuf + 1 - chrSrcSliceY >= 0)
				//FIXME replace parameters through context struct (some at least)

				if(!(isGray(srcFormat) || isGray(dstFormat)))
					RENAME(hcscale)(chrPixBuf[ chrBufIndex ], chrDstW, src1, src2, chrSrcW, chrXInc,
						flags, canMMX2BeUsed, hChrFilter, hChrFilterPos, hChrFilterSize,
						funnyUVCode, c->srcFormat, formatConvBuffer, 
						c->chrMmx2Filter, c->chrMmx2FilterPos);
				lastInChrBuf++;
			}
			//wrap buf index around to stay inside the ring buffer
			if(lumBufIndex >= vLumBufSize ) lumBufIndex-= vLumBufSize;
			if(chrBufIndex >= vChrBufSize ) chrBufIndex-= vChrBufSize;
		}
		else // not enough lines left in this slice -> load the rest in the buffer
		{
/*		printf("%d %d Last:%d %d LastInBuf:%d %d Index:%d %d Y:%d FSize: %d %d BSize: %d %d\n",
			firstChrSrcY,firstLumSrcY,lastChrSrcY,lastLumSrcY,
			lastInChrBuf,lastInLumBuf,chrBufIndex,lumBufIndex,dstY,vChrFilterSize,vLumFilterSize,
			vChrBufSize, vLumBufSize);*/

			//Do horizontal scaling
			while(lastInLumBuf+1 < srcSliceY + srcSliceH)
			{
				uint8_t *s= src[0]+(lastInLumBuf + 1 - srcSliceY)*srcStride[0];
				lumBufIndex++;
				ASSERT(lumBufIndex < 2*vLumBufSize)
				ASSERT(lastInLumBuf + 1 - srcSliceY < srcSliceH)
				ASSERT(lastInLumBuf + 1 - srcSliceY >= 0)
				RENAME(hyscale)(lumPixBuf[ lumBufIndex ], dstW, s, srcW, lumXInc,
						flags, canMMX2BeUsed, hLumFilter, hLumFilterPos, hLumFilterSize,
						funnyYCode, c->srcFormat, formatConvBuffer, 
						c->lumMmx2Filter, c->lumMmx2FilterPos);
				lastInLumBuf++;
			}
			while(lastInChrBuf+1 < (chrSrcSliceY + chrSrcSliceH))
			{
				uint8_t *src1= src[1]+(lastInChrBuf + 1 - chrSrcSliceY)*srcStride[1];
				uint8_t *src2= src[2]+(lastInChrBuf + 1 - chrSrcSliceY)*srcStride[2];
				chrBufIndex++;
				ASSERT(chrBufIndex < 2*vChrBufSize)
				ASSERT(lastInChrBuf + 1 - chrSrcSliceY < chrSrcSliceH)
				ASSERT(lastInChrBuf + 1 - chrSrcSliceY >= 0)

				if(!(isGray(srcFormat) || isGray(dstFormat)))
					RENAME(hcscale)(chrPixBuf[ chrBufIndex ], chrDstW, src1, src2, chrSrcW, chrXInc,
						flags, canMMX2BeUsed, hChrFilter, hChrFilterPos, hChrFilterSize,
						funnyUVCode, c->srcFormat, formatConvBuffer, 
						c->chrMmx2Filter, c->chrMmx2FilterPos);
				lastInChrBuf++;
			}
			//wrap buf index around to stay inside the ring buffer
			if(lumBufIndex >= vLumBufSize ) lumBufIndex-= vLumBufSize;
			if(chrBufIndex >= vChrBufSize ) chrBufIndex-= vChrBufSize;
			break; //we can't output a dstY line so let's try with the next slice
		}

#ifdef HAVE_MMX
		b5Dither= dither8[dstY&1];
		g6Dither= dither4[dstY&1];
		g5Dither= dither8[dstY&1];
		r5Dither= dither8[(dstY+1)&1];
#endif
	    if(dstY < dstH-2)
	    {
		int16_t **lumSrcPtr= lumPixBuf + lumBufIndex + firstLumSrcY - lastInLumBuf + vLumBufSize;
		int16_t **chrSrcPtr= chrPixBuf + chrBufIndex + firstChrSrcY - lastInChrBuf + vChrBufSize;
#ifdef HAVE_MMX
		int i;
		for(i=0; i<vLumFilterSize; i++)
		{
			lumMmxFilter[4*i+0]= (int32_t)lumSrcPtr[i];
			lumMmxFilter[4*i+2]= 
			lumMmxFilter[4*i+3]= 
				((uint16_t)vLumFilter[dstY*vLumFilterSize + i])*0x10001;
		}
		for(i=0; i<vChrFilterSize; i++)
		{
			chrMmxFilter[4*i+0]= (int32_t)chrSrcPtr[i];
			chrMmxFilter[4*i+2]= 
			chrMmxFilter[4*i+3]= 
				((uint16_t)vChrFilter[chrDstY*vChrFilterSize + i])*0x10001;
		}
#endif
		if(dstFormat == IMGFMT_NV12 || dstFormat == IMGFMT_NV21){
			const int chrSkipMask= (1<<c->chrDstVSubSample)-1;
			if(dstY&chrSkipMask) uDest= NULL; //FIXME split functions in lumi / chromi
			RENAME(yuv2nv12X)(c,
				vLumFilter+dstY*vLumFilterSize   , lumSrcPtr, vLumFilterSize,
				vChrFilter+chrDstY*vChrFilterSize, chrSrcPtr, vChrFilterSize,
				dest, uDest, dstW, chrDstW, dstFormat);
		}
		else if(isPlanarYUV(dstFormat) || isGray(dstFormat)) //YV12 like
		{
			const int chrSkipMask= (1<<c->chrDstVSubSample)-1;
			if((dstY&chrSkipMask) || isGray(dstFormat)) uDest=vDest= NULL; //FIXME split functions in lumi / chromi
			if(vLumFilterSize == 1 && vChrFilterSize == 1) // Unscaled YV12
			{
				int16_t *lumBuf = lumPixBuf[0];
				int16_t *chrBuf= chrPixBuf[0];
				RENAME(yuv2yuv1)(lumBuf, chrBuf, dest, uDest, vDest, dstW, chrDstW);
			}
			else //General YV12
			{
				RENAME(yuv2yuvX)(c,
					vLumFilter+dstY*vLumFilterSize   , lumSrcPtr, vLumFilterSize,
					vChrFilter+chrDstY*vChrFilterSize, chrSrcPtr, vChrFilterSize,
					dest, uDest, vDest, dstW, chrDstW);
			}
		}
		else
		{
			ASSERT(lumSrcPtr + vLumFilterSize - 1 < lumPixBuf + vLumBufSize*2);
			ASSERT(chrSrcPtr + vChrFilterSize - 1 < chrPixBuf + vChrBufSize*2);
			if(vLumFilterSize == 1 && vChrFilterSize == 2) //Unscaled RGB
			{
				int chrAlpha= vChrFilter[2*dstY+1];
				RENAME(yuv2packed1)(c, *lumSrcPtr, *chrSrcPtr, *(chrSrcPtr+1),
						 dest, dstW, chrAlpha, dstFormat, flags, dstY);
			}
			else if(vLumFilterSize == 2 && vChrFilterSize == 2) //BiLinear Upscale RGB
			{
				int lumAlpha= vLumFilter[2*dstY+1];
				int chrAlpha= vChrFilter[2*dstY+1];
				RENAME(yuv2packed2)(c, *lumSrcPtr, *(lumSrcPtr+1), *chrSrcPtr, *(chrSrcPtr+1),
						 dest, dstW, lumAlpha, chrAlpha, dstY);
			}
			else //General RGB
			{
				RENAME(yuv2packedX)(c,
					vLumFilter+dstY*vLumFilterSize, lumSrcPtr, vLumFilterSize,
					vChrFilter+dstY*vChrFilterSize, chrSrcPtr, vChrFilterSize,
					dest, dstW, dstY);
			}
		}
            }
	    else // hmm looks like we can't use MMX here without overwriting this array's tail
	    {
		int16_t **lumSrcPtr= lumPixBuf + lumBufIndex + firstLumSrcY - lastInLumBuf + vLumBufSize;
		int16_t **chrSrcPtr= chrPixBuf + chrBufIndex + firstChrSrcY - lastInChrBuf + vChrBufSize;
		if(dstFormat == IMGFMT_NV12 || dstFormat == IMGFMT_NV21){
			const int chrSkipMask= (1<<c->chrDstVSubSample)-1;
			if(dstY&chrSkipMask) uDest= NULL; //FIXME split functions in lumi / chromi
			yuv2nv12XinC(
				vLumFilter+dstY*vLumFilterSize   , lumSrcPtr, vLumFilterSize,
				vChrFilter+chrDstY*vChrFilterSize, chrSrcPtr, vChrFilterSize,
				dest, uDest, dstW, chrDstW, dstFormat);
		}
		else if(isPlanarYUV(dstFormat) || isGray(dstFormat)) //YV12
		{
			const int chrSkipMask= (1<<c->chrDstVSubSample)-1;
			if((dstY&chrSkipMask) || isGray(dstFormat)) uDest=vDest= NULL; //FIXME split functions in lumi / chromi
			yuv2yuvXinC(
				vLumFilter+dstY*vLumFilterSize   , lumSrcPtr, vLumFilterSize,
				vChrFilter+chrDstY*vChrFilterSize, chrSrcPtr, vChrFilterSize,
				dest, uDest, vDest, dstW, chrDstW);
		}
		else
		{
			ASSERT(lumSrcPtr + vLumFilterSize - 1 < lumPixBuf + vLumBufSize*2);
			ASSERT(chrSrcPtr + vChrFilterSize - 1 < chrPixBuf + vChrBufSize*2);
			yuv2packedXinC(c, 
				vLumFilter+dstY*vLumFilterSize, lumSrcPtr, vLumFilterSize,
				vChrFilter+dstY*vChrFilterSize, chrSrcPtr, vChrFilterSize,
				dest, dstW, dstY);
		}
	    }
	}

#ifdef HAVE_MMX
	__asm __volatile(SFENCE:::"memory");
	__asm __volatile(EMMS:::"memory");
#endif
	/* store changed local vars back in the context */
	c->dstY= dstY;
	c->lumBufIndex= lumBufIndex;
	c->chrBufIndex= chrBufIndex;
	c->lastInLumBuf= lastInLumBuf;
	c->lastInChrBuf= lastInChrBuf;

	return dstY - lastDstY;
}
