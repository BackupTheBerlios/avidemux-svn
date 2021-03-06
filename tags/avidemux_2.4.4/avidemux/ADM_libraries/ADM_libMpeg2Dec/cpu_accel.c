/*
 * cpu_accel.c
 * Copyright (C) 2000-2003 Michel Lespinasse <walken@zoy.org>
 * Copyright (C) 1999-2000 Aaron Holtzman <aholtzma@ess.engr.uvic.ca>
 *
 * This file is part of mpeg2dec, a free MPEG-2 video stream decoder.
 * See http://libmpeg2.sourceforge.net/ for updates.
 *
 * mpeg2dec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * mpeg2dec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"
#include <stdio.h>
#include <inttypes.h>

#include "mpeg2.h"
#include "attributes.h"
#include "mpeg2_internal.h"

typedef void (*sighandler_t)(int); // MEANX RETSIGTYPE
 

extern int ADM_mpeg2dec_mm_support(void); // MEANX

#ifdef ACCEL_DETECT
#ifdef ARCH_X86
static inline uint32_t arch_accel (void)
{
   
    static uint32_t caps;
    static int done=0;

#define CHECK_CAPS(y) if (caps & MPEG2_ACCEL_X86_##y ) printf("[LibMpeg2Dec] "#y" supported\n");
    
    if(!done)
    {
    	caps=ADM_mpeg2dec_mm_support();
    	done=1;
    }
    return caps;
}
#endif /* ARCH_X86 */

#if defined(ARCH_PPC) || defined(ARCH_SPARC)
#include <signal.h>
#include <setjmp.h>

static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump = 0;

static sighandler_t sigill_handler (int sig)
{
    if (!canjump) {
	signal (sig, SIG_DFL);
	raise (sig);
    }

    canjump = 0;
    siglongjmp (jmpbuf, 1);
}

#ifdef ARCH_PPC
static inline uint32_t arch_accel (void)
{
    static sighandler_t oldsig;

    oldsig = signal (SIGILL, sigill_handler);
    if (sigsetjmp (jmpbuf, 1)) {
	signal (SIGILL, oldsig);
	return 0;
    }

    canjump = 1;

#ifdef HAVE_ALTIVEC_H	/* gnu */
#define VAND(a,b,c) "vand " #a "," #b "," #c "\n\t"
#else			/* apple */
#define VAND(a,b,c) "vand v" #a ",v" #b ",v" #c "\n\t"
#endif
    asm volatile ("mtspr 256, %0\n\t"
		  VAND (0, 0, 0)
		  :
		  : "r" (-1));

    canjump = 0;

    signal (SIGILL, oldsig);
    //return 0 ; // Meanx
    return MPEG2_ACCEL_PPC_ALTIVEC;
}
#endif /* ARCH_PPC */

#ifdef ARCH_SPARC
static inline uint32_t arch_accel (void)
{
    static sighandler_t (* oldsig) (int);

    oldsig = signal (SIGILL, sigill_handler);
    if (sigsetjmp (jmpbuf, 1)) {
	signal (SIGILL, oldsig);
	return 0;
    }

    canjump = 1;

    /* pdist %f0, %f0, %f0 */
    __asm__ __volatile__(".word\t0x81b007c0");

    canjump = 0;

    if (sigsetjmp (jmpbuf, 1)) {
	signal (SIGILL, oldsig);
	return MPEG2_ACCEL_SPARC_VIS;
    }

    canjump = 1;

    /* edge8n %g0, %g0, %g0 */
    __asm__ __volatile__(".word\t0x81b00020");

    canjump = 0;

    signal (SIGILL, oldsig);
    return MPEG2_ACCEL_SPARC_VIS | MPEG2_ACCEL_SPARC_VIS2;
}
#endif /* ARCH_SPARC */
#endif /* ARCH_PPC || ARCH_SPARC */

#ifdef ARCH_ALPHA
static inline uint32_t arch_accel (void)
{
    uint64_t no_mvi;

    asm volatile ("amask %1, %0"
		  : "=r" (no_mvi)
		  : "rI" (256));	/* AMASK_MVI */
    return no_mvi ? MPEG2_ACCEL_ALPHA : (MPEG2_ACCEL_ALPHA |
					 MPEG2_ACCEL_ALPHA_MVI);
}
#endif /* ARCH_ALPHA */
#endif /* ACCEL_DETECT */

uint32_t mpeg2_detect_accel (void)
{
    uint32_t accel;

    accel = 0;
#ifdef ACCEL_DETECT
#if defined (ARCH_X86) || defined (ARCH_PPC) || defined (ARCH_ALPHA) || defined (ARCH_SPARC)
    accel = arch_accel ();
#define PRT(x) if(accel & x) printf("[libMpeg2dec] accel :"#x"\n");
#ifdef ARCH_X86
	printf("[libMpeg2dec] X86 CPU accel in use:\n");
	PRT(MPEG2_ACCEL_X86_MMX);
	PRT(MPEG2_ACCEL_X86_3DNOW);
	PRT(MPEG2_ACCEL_X86_MMXEXT);
#endif	
#ifdef 	ARCH_PPC
	printf("Mpeg2dec: PPC CPU accel in use:\n");
	PRT(MPEG2_ACCEL_PPC_ALTIVEC);
#endif	
	
    
#endif
#endif
    return accel;
}
