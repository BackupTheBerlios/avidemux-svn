//
// C++ Implementation: ADM_cpuCap
//
// Description: 
//
//
// Author: mean <fixounet@free.fr>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "config.h"
#include <stdio.h>

#if defined(ADM_WIN32)
#include <pthread.h>
#elif !defined(__APPLE__) && !defined(ADM_BSD_FAMILY)
#include <string.h>
#include <sched.h>
#endif

#include "default.h"
#include "admmangle.h"
#include "ADM_osSupport/ADM_cpuCap.h"
#include "prefs.h"

/* Cpu caps interface to other libs */
extern "C"{ 
	int ADM_lavcodec_mm_support(void);
	int ADM_mpeg2dec_mm_support(void);
	
}


#if defined(ARCH_X86) || defined(ARCH_X86_64)
extern "C" 
{
#include "ADM_lavcodec/dsputil_cpu.h"
}
#endif

#define cpuid(index,eax,ebx,ecx,edx)\
    __asm __volatile\
        ("mov %%"REG_b", %%"REG_S"\n\t"\
         "cpuid\n\t"\
         "xchg %%"REG_b", %%"REG_S\
         : "=a" (eax), "=S" (ebx),\
           "=c" (ecx), "=d" (edx)\
         : "0" (index));


/**
 * 		\fn CpuCaps::init
 * 		\brief Detect the SIM capabilities of CPU, borrowed from lavcodec
 */
  void 	CpuCaps::init( void)
{
	printf("[cpuCaps]Checking CPU capabilities\n");
	myCpuCaps=0;
	myCpuMask=0xffffffff;
	prefs->get(FEATURE_CPU_CAPS,&myCpuMask);

#if defined(ARCH_X86) || defined(ARCH_X86_64)
int rval = 0;
 int eax, ebx, ecx, edx;
 int max_std_level, max_ext_level, std_caps=0, ext_caps=0;
 long a, c;

 __asm__ __volatile__ (
                       /* See if CPUID instruction is supported ... */
                       /* ... Get copies of EFLAGS into eax and ecx */
                       "pushf\n\t"
                       "pop %0\n\t"
                       "mov %0, %1\n\t"

                       /* ... Toggle the ID bit in one copy and store */
                       /*     to the EFLAGS reg */
                       "xor $0x200000, %0\n\t"
                       "push %0\n\t"
                       "popf\n\t"

                       /* ... Get the (hopefully modified) EFLAGS */
                       "pushf\n\t"
                       "pop %0\n\t"
                       : "=a" (a), "=c" (c)
                       :
                       : "cc"
                       );

 if (a == c)
     return ; /* CPUID not supported */

 cpuid(0, max_std_level, ebx, ecx, edx);

 if(max_std_level >= 1)
 {
     cpuid(1, eax, ebx, ecx, std_caps);
     if (std_caps & (1<<23))
    	 myCpuCaps |= ADM_CPU_MMX;
     if (std_caps & (1<<25))
    	 myCpuCaps |= ADM_CPU_MMXEXT | ADM_CPU_SSE;
     if (std_caps & (1<<26))
    	 myCpuCaps |= ADM_CPU_SSE2;
     if (ecx & 1)
    	 myCpuCaps |= ADM_CPU_SSE3;
     if (ecx & 0x00000200 )
    	 myCpuCaps |= ADM_CPU_SSSE3;
     
     
     
 }

 cpuid(0x80000000, max_ext_level, ebx, ecx, edx);

 if(max_ext_level >= 0x80000001)
 {
     cpuid(0x80000001, eax, ebx, ecx, ext_caps);
     if (ext_caps & (1<<31))
    	 myCpuCaps |= ADM_CPU_3DNOW;
     if (ext_caps & (1<<30))
    	 myCpuCaps |= ADM_CPU_3DNOWEXT;
     if (ext_caps & (1<<23))
    	 myCpuCaps |= ADM_CPU_MMX;
     if (ext_caps & (1<<22))
    	 myCpuCaps |= ADM_CPU_MMXEXT;
     
 }
#define CHECK(x) if(myCpuCaps & ADM_CPU_##x) { printf("\t\t"#x" detected ");\
											if(!(myCpuMask&ADM_CPU_##x)) printf("  but disabled");printf("\n");}
	CHECK(MMX);
	CHECK(3DNOW);
	CHECK(MMXEXT);
	CHECK(SSE);
	CHECK(SSE2);
	CHECK(SSE3);
	CHECK(SSSE3);

#endif // X86
	printf("[cpuCaps]End of CPU capabilities check (cpuMask :%x)\n",myCpuMask);
	return ;
}

/************************************************************************/
#if defined(ARCH_X86) || defined(ARCH_X86_64)
#include "ADM_libraries/ADM_libMpeg2Dec/mpeg2_cpu.h"
#endif

int ADM_mpeg2dec_mm_support(void)
{
int rval=0;

#if defined(ARCH_X86) || defined(ARCH_X86_64)
#undef MATCH
#define MATCH(x,y) if(CpuCaps::myCpuCaps & CpuCaps::myCpuMask & ADM_CPU_##x) rval|=MPEG2_ACCEL_X86_##x;
	
	MATCH(MMX,MMX);
	MATCH(MMXEXT,MMXEXT);
	MATCH(3DNOW,3DNOW);
#endif

	return rval;
}
//******************************************************
#if defined(ARCH_X86) || defined(ARCH_X86_64)
#include "ADM_lavcodec.h"
#endif
/**
 * 		\fn lavcodec_mm_support
 * 		\brief Give lavcodec CPU supported ( FF_MM_MMX)
 */
int ADM_lavcodec_mm_support(void)
{
int rval=0;

#if defined(ARCH_X86) || defined(ARCH_X86_64)
#undef MATCH
#define MATCH(x,y) if(CpuCaps::myCpuCaps &  CpuCaps::myCpuMask & ADM_CPU_##x) rval|=MM_##x;

	MATCH(MMX,MMX);
	MATCH(MMXEXT,MMXEXT);
	MATCH(SSE,SSE);
	MATCH(SSE2,SSE2);
	MATCH(SSE3,SSE3);
	MATCH(SSSE3,SSSE3);
	MATCH(3DNOW,3DNOW);
	MATCH(3DNOWEXT,3DNOWEXT);
#endif

	return rval;
}
// EOF

// Stolen from x264
int ADM_cpu_num_processors(void)
{
#if defined(ADM_WIN32)
    return pthread_num_processors_np();
#elif !defined(__APPLE__) && !defined(ADM_BSD_FAMILY)
    unsigned int bit;
    int np;

    cpu_set_t p_aff;
    memset( &p_aff, 0, sizeof(p_aff) );
    sched_getaffinity( 0, sizeof(p_aff), &p_aff );

    for( np = 0, bit = 0; bit < sizeof(p_aff); bit++ )
        np += (((uint8_t *)&p_aff)[bit / 8] >> (bit % 8)) & 1;

    return np;
#else
    return 1;
#endif
}
