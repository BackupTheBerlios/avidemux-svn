/***************************************************************************
 *   Copyright (C) 2005 by mean                                            *
 *   fx@dave64                                                             *
 *                                                                         *
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


#include <config.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <signal.h>
#include "../ADM_library/ADM_assert.h"

static void onexit( void );
static void sig_segfault_handler(int signo);

extern void ADM_memStat( void );

int main(int argc, char **argv)
{
  printf("\n*******************\n");
  printf("  Xsub2srt, v  " VERSION "\n");
  printf("*******************\n");
  

        signal(11, sig_segfault_handler); // show stacktrace on default
        atexit(onexit);

        
        gtk_set_locale();
        gtk_init(&argc, &argv);

        gtk_main();

  
  
}
//*****************************
void onexit( void )
{
  ADM_memStat();
  printf("\n Goodbye...\n\n");
}
void sig_segfault_handler(int signo)
{
  g_on_error_stack_trace ("avidemux");
  printf("Memory stat:\n");
  signo=0; // will keep GCC happy
  exit(1); // _exit(1) ???
}
