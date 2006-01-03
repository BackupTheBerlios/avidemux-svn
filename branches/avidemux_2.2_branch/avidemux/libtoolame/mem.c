/*
 *  tooLAME: an optimized mpeg 1/2 layer 2 audio encoder
 *
 *  Copyright (C) 2001-2004 Michael Cheng
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"

/*******************************************************************************
*
*  Allocate number of bytes of memory equal to "block".
*
*******************************************************************************/

void *toolame_malloc (unsigned long block, char *item)
{

  void *ptr;

  ptr = (void *) malloc (block);

  if (ptr != NULL) {
    memset (ptr, 0, block);
  } else {
    fprintf (stderr, "Unable to allocate %s\n", item);
    exit (0);
  }
  
  return (ptr);
}


/****************************************************************************
*
*  Free memory pointed to by "*ptr_addr".
*
*****************************************************************************/

void toolame_free (void **ptr_addr)
{

  if (*ptr_addr != NULL) {
    free (*ptr_addr);
    *ptr_addr = NULL;
  }
}
