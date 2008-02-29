/** *************************************************************************
    \fn ADM_cpuCap.h
    \brief Handle cpu capabilities (MMX/SSE/...)
                      
    copyright            : (C) 2008 by mean
    
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#if defined(ARCH_X86_64) ||defined(ARCH_X86)

#define CHECK_Z(x) {if(CpuCaps::myCpuCaps & ADM_CPU_##x & CpuCaps::myCpuMask) return 1; else return 0;} 

#else
#define         CHECK_Z(x) {return 0;}
#endif

typedef enum 
{
        ADM_CPU_NONE   =1,
        ADM_CPU_MMX    =1<<1,
        ADM_CPU_MMXEXT =1<<2,
        ADM_CPU_3DNOW  =1<<3,
        ADM_CPU_3DNOWEXT  =1<<4,
        ADM_CPU_SSE    =1<<5,
        ADM_CPU_SSE2   =1<<6,
		ADM_CPU_SSE3   =1<<7,
		ADM_CPU_SSSE3  =1<<8,
        ADM_CPU_ALTIVEC=1<<9,
        
        ADM_CPU_ALL=0xffffffff
} ADM_CPU_CAPS;

class CpuCaps
{
public:
        static uint32_t      myCpuCaps;
        static uint32_t      myCpuMask;
public:
	static void 	init( void);
	static uint8_t 	hasMMX (void) {CHECK_Z(MMX)};
	static uint8_t 	hasMMXEXT (void){CHECK_Z(MMXEXT)};
	static uint8_t 	has3DNOW (void) {CHECK_Z(3DNOW)};
	static uint8_t 	hasSSE (void) {CHECK_Z(SSE)};
	static uint8_t 	hasSSE2 (void){CHECK_Z(SSE2)};
	static uint8_t 	hasSSE3 (void){CHECK_Z(SSE3)};
	static uint8_t 	hasSSSE3 (void){CHECK_Z(SSSE3)};
	static uint8_t 	has3DNOWEXT(void){CHECK_Z(3DNOWEXT)};


};

#if (defined( ARCH_X86)  || defined(ARCH_X86_64)) && defined(HAVE_BUILTIN_VECTOR)
#define USE_SSE 1
#endif
