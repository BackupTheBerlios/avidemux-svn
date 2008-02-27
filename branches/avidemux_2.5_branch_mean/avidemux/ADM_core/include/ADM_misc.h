/***************************************************************************
                          ADM_misc.h  -  description
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

#ifndef __ADM_MISC__
#define __ADM_MISC__
//#include "time.h"

typedef enum 
{
        ADM_LOG_NONE=0,
        ADM_LOG_IMPORTANT=1,
        ADM_LOG_INFO=2,
        ADM_LOG_DEBUG=3
  
} ADM_LOG_LEVEL;

typedef struct 
{
  uint32_t hours,minutes,seconds; 
} ADM_date;

void            TLK_getDate(ADM_date *date);
void            PathSplit(char *str, char **root, char **ext);
void	        PathStripName(char *str);
const char      *GetFileName(const char *str);
char            *PathCanonize(const char *tmpname);
void            LowerCase(char *string);
uint32_t        getTime( int called );;
uint32_t 	getTimeOfTheDay(void);
extern char *slashToBackSlash(char *in);
#ifdef HAVE_GETTIMEOFDAY
	#define TIMZ struct timezone
#else
	#ifndef HAVE_STRUCT_TIMESPEC
	#define HAVE_STRUCT_TIMESPEC
	extern "C"
	{
		typedef struct timespec
		{
			time_t tv_sec;
			long int tv_nsec;
		};

		void gettimeofday(struct timeval *p, void *tz);
		};
		#define timezone int
		#define TIMZ int
	#endif
#endif

#ifdef ADM_WIN32
	#define PRIO_MIN -20
	#define PRIO_MAX 20
	#define PRIO_PROCESS 0

	int getpriority(int which, int who);
	int setpriority(int which, int who, int value);

	#define RB_POWER_OFF 0x4321fedc
	#define SHTDN_REASON_FLAG_PLANNED 0x80000000

	int shutdown_win32(void);
	void getWorkingArea(uint32_t *width, uint32_t *height);
#else
	#include <sys/resource.h>
#endif

bool shutdown(void);

#ifdef ADM_BIG_ENDIAN	
	#define R64 ADM_swap64
	#define R32 ADM_swap32
	#define R16 ADM_swap16
#else
	#define R64(x) (x)
	#define R32(x) (x) 
	#define R16(x) (x) 
#endif
#endif
