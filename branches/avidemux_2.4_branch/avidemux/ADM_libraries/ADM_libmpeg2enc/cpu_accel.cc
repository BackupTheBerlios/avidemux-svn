/*
 * cpu_accel.c
 * Copyright (C) 1999-2000 Aaron Holtzman <aholtzma@ess.engr.uvic.ca>
 *
 * This file is part of mpeg2dec, a free MPEG-2 video stream decoder.
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

#include <config.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include "cpu_accel.h"

#include "default.h"
#include "ADM_osSupport/ADM_cpuCap.h"

#if defined( HAVE_ALTIVEC ) && defined(USE_ALTIVEC)
int detect_altivec(){};
#endif

//#include "mjpeg_types.h"

/* Some miscelaneous stuff to allow checking whether SSE instructions cause
   illegal instruction errors.
*/



#if defined(ARCH_X86) || defined(ARCH_X86_64)

static int x86_accel (void)
{
   int32_t eax, ebx, ecx, edx;
   int32_t AMD;
   int32_t caps=0;

   if(CpuCaps::hasMMX()) caps |=ACCEL_X86_MMX;
   if(CpuCaps::hasMMXEXT()) caps |=ACCEL_X86_MMXEXT;
   if(CpuCaps::has3DNOW()) caps |=ACCEL_X86_3DNOW;
   if(CpuCaps::hasSSE()) caps |=ACCEL_X86_SSE;
   return caps;
} 
#endif   
int cpu_accel (void)
{
#if defined(ARCH_X86) || defined(ARCH_X86_64)
    static int got_accel = 0;
    static int accel;

    if (!got_accel) {
	got_accel = 1;
	accel = x86_accel ();
    }

    return accel;
#endif
#if defined( HAVE_ALTIVEC) && defined(USE_ALTIVEC)
    return detect_altivec();
#else
    return 0;
#endif
}
