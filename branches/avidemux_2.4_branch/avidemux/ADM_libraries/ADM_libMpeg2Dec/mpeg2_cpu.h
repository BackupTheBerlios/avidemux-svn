/*
 * mpeg2.h
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

#ifndef MPEG2_CPU_H
#define MPEG2_CPU_H

#define MPEG2_ACCEL_X86_MMX 1
#define MPEG2_ACCEL_X86_3DNOW 2
#define MPEG2_ACCEL_X86_MMXEXT 4
#define MPEG2_ACCEL_PPC_ALTIVEC 1
#define MPEG2_ACCEL_ALPHA 1
#define MPEG2_ACCEL_ALPHA_MVI 2
#define MPEG2_ACCEL_SPARC_VIS 1
#define MPEG2_ACCEL_SPARC_VIS2 2
#define MPEG2_ACCEL_DETECT 0x80000000

#endif
