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
#include <math.h>
#include <assert.h>

#include "common.h"
#include "toolame.h"
#include "toolame_global_flags.h"
#include "bitbuffer.h"
#include "mem.h"
#include "crc.h"
#include "dab.h"
#include "psycho_n1.h"
#include "psycho_0.h"
#include "psycho_1.h"
#include "psycho_2.h"
#include "psycho_3.h"
#include "psycho_4.h"
#include "availbits.h"
#include "subband.h"
#include "encode.h"
#include "energy.h"
#include "tables.h"


static void init_header_info( toolame_options *glopts ) {
  frame_header *header = &glopts->header;

  /* use the options to create header info structure */
  header->lay = 2;
  header->error_protection = glopts->error_protection;

  if ((header->bitrate_index = toolame_get_bitrate_index(glopts->version, glopts->bitrate)) < 0) {
    fprintf(stdout,"Not a valid bitrate for this version: %i\n",glopts->bitrate);
    exit(99);
  }
  
  // Convert the sampling frequency to the required index
  header->samplerate_idx = toolame_get_samplerate_index ((long) glopts->samplerate_out, &glopts->version); 
  //  fprintf(stdout,"Not a valid samplerate for this version: %i\n",glopts->bitrate);
  //  exit(99);
  //}


  glopts->vbr_upper_index = toolame_get_bitrate_index(glopts->version, glopts->vbr_max_bitrate);
  //  fprintf(stdout,"Not a valid bitrate for this version: %i\n",glopts->vbr_max_bitrate);
  //  exit(99);
  //}


  /* if the sampling frequency is >=32000, set to MPEG1
     else set to MPEG2 */
  if (glopts->samplerate_out < 32000)
    header->version = glopts->version = 0;
  else
    header->version = glopts->version = 1;


  header->padding = glopts->padding;
  header->private_bit = glopts->private_bit;
  header->mode = glopts->mode;
  header->mode_ext = 0;
  header->copyright = glopts->copyright;
  header->original = glopts->original;
  header->emphasis = glopts->emphasis;  
}


static void init_frame_info(toolame_options *glopts)
/* interpret data in hdr str to fields in frame */
{
  frame_info *frame = &glopts->frame;
  frame_header *header = &glopts->header;

  frame->actual_mode = header->mode;
  frame->nch = (header->mode == MONO) ? 1 : 2;

  frame->sblimit = pick_table ( glopts );
  /* MFC FIX this up */
  encode_init( glopts );

  if (glopts->mode == JOINT_STEREO)
    frame->jsbound = js_bound(header->mode_ext);
  else
    frame->jsbound = frame->sblimit;
  /* alloc, tab_num set in pick_table */
}


/*
  toolame_init
  Create a set of encoding options and return
  a pointer to this structure
*/
toolame_options *toolame_init(void) {
	toolame_options *newoptions = NULL;  
	
	newoptions = (toolame_options *)toolame_malloc(sizeof(toolame_options), "toolame_options");
	
	newoptions->version = MPEG1; /* default to 32/44.1/48 kHz input. i.e. mpeg-1 */
	newoptions->samplerate_in = 44100;
	newoptions->samplerate_out = 0;
	
	newoptions->mode = STEREO;
	newoptions->psymodel = 3;
	newoptions->bitrate = 192;
	newoptions->vbr = FALSE;
	newoptions->vbrlevel = 0;
	newoptions->athlevel = 0;
	
	newoptions->quickmode = FALSE;
	newoptions->quickcount = 10;
	newoptions->emphasis = EMPHASIS_N;
	newoptions->private_bit = 0;
	newoptions->copyright = FALSE;
	newoptions->original = FALSE;
	newoptions->error_protection = FALSE;
	newoptions->padding = PAD_NO;
	newoptions->do_dab = FALSE;
	newoptions->dab_crc_len = 2;
	newoptions->dab_xpad_len = 0;
	newoptions->verbosity = 2;
	newoptions->vbr_upper_index = 0;
	
	newoptions->do_energy_levels = FALSE;
	newoptions->num_ancillary_bits = 0;
	
	
	newoptions->vbr_frame_count = 0;	/* only used for debugging */
    newoptions->alloc_tab_num = -1;		/* no table loaded */
	newoptions->alloc_tab = NULL;
	

	newoptions->toolame_init = 0;
	newoptions->subband = NULL;
	newoptions->j_sample = NULL;
	newoptions->sb_sample = NULL;
	newoptions->psycount = 0;
	newoptions->crc = 0;


	newoptions->p0mem = NULL;
	newoptions->p1mem = NULL;
	newoptions->p2mem = NULL;
	newoptions->p3mem = NULL;
	newoptions->p4mem = NULL;
	
	return(newoptions);
}


/**
 * This function should actually *check* the parameters to see if they
 * make sense. 
 */

int toolame_init_params (toolame_options *glopts) {

	if (glopts->toolame_init) {
		fprintf(stderr,"Already called toolame_init_params() once.\n");
		return 1;
	}


	/* Can't do DAB and energylevel extensions at the same time
	   Because both of them think they're the only ones inserting
	   information into the ancillary section of the frame */
	if (glopts->do_dab && glopts->do_energy_levels) {
		fprintf(stderr,"Can't do DAB and Energy Levels at the same time\n");
		exit(99);
	}

	/* Check that if we're doing energy levels, that there's enough space 
	   to put the information */
	if (glopts->do_energy_levels) {
		if ((glopts->mode==MONO)&&(glopts->num_ancillary_bits<16)) {
			fprintf(stderr,"Too few ancillary bits: %i<16\n",glopts->num_ancillary_bits);
			glopts->num_ancillary_bits = 16;
		}    
		if ((glopts->mode!=MONO)&&(glopts->num_ancillary_bits<40)) {
			fprintf(stderr,"Too few ancillary bits: %i<40\n",glopts->num_ancillary_bits);
		glopts->num_ancillary_bits = 40;
		}
	}

	
	/* If not output samplerate has been set, then set it to the input sample rate */
	if (glopts->samplerate_out < 1) {
		glopts->samplerate_out = glopts->samplerate_in;
	}
	


	/* build header from parameters */
	init_header_info( glopts );
	
	/* this will load the alloc tables and do some other stuff */
	init_frame_info( glopts );
	
	/* initialises bitrate allocation */
	init_bit_allocation( glopts );



	/* Initialise */
	glopts->samples_left = 0;
	glopts->psycount = 0;
	glopts->crc = 0;

  
  	/* Allocate memory to larger buffers */
    glopts->subband = (subband_t *) toolame_malloc (sizeof (subband_t), "subband");
	glopts->j_sample = (jsb_sample_t *) toolame_malloc (sizeof (jsb_sample_t), "j_sample");
    glopts->sb_sample = (sb_sample_t *) toolame_malloc (sizeof (sb_sample_t), "sb_sample");


	/* clear buffers */
    memset ((char *) glopts->buffer, 0, sizeof(glopts->buffer));
    memset ((char *) glopts->bit_alloc, 0, sizeof (glopts->bit_alloc));
    memset ((char *) glopts->scfsi, 0, sizeof (glopts->scfsi));
    memset ((char *) glopts->scalar, 0, sizeof (glopts->scalar));
    memset ((char *) glopts->j_scale, 0, sizeof (glopts->j_scale));
    memset ((char *) glopts->smrdef, 0, sizeof (glopts->smrdef));
    memset ((char *) glopts->smr, 0, sizeof (glopts->smr));
    memset ((char *) glopts->max_sc, 0, sizeof (glopts->max_sc));


	// All initalised now :)
	glopts->toolame_init++;

	return(0);
}


/*
	Encode a single frame of audio from 1152 samples
	Audio samples are taken from glopts->buffer
	Encoded bit stream is placed in to parameter bs
	(not intended for use outside the library)
	
	Returns the size of the frame
*/
static int toolame_encode_frame(toolame_options *glopts, bit_stream *bs)
{
	int nch = glopts->frame.nch;
	int sb, ch, adb, i;
	unsigned long frameBits, initial_bits;

	/* This is the saved audio buffer? */
	short sam[2][1152];		/* was [1056]; 
				   MFC MAY03 - 
				   so why the bloody hell did you change it? 
				   explain yourself son.
				   I think this was changed to 1344 for mp3 encoding
				   I think it can be changed back to 1056?
				*/


	if (!glopts->toolame_init) {
		fprintf (stderr, "Please call toolame_init_params() before starting encoding.\n");
		exit (0);
	}
	
	
	/* FIXME: do downmixing/upmixing of samples here */
	

    // Clear the saved audio buffer
    memset ((char *) sam, 0, sizeof (sam));

	/* Store the number of bits initially in the bit buffer */
	initial_bits = buffer_sstell(bs);

	adb = available_bits ( glopts );

    /* allow the user to reserve some space at the end of the frame
       This will however leave fewer bits for the audio.
       Need to do a sanity check here to see that there are *some* 
       bits left. */
	if (glopts->num_ancillary_bits > 0.6*adb) {
		/* Trying to reserve more than 60% of the frame.
		   0.6 is arbitrary. but since most applications
		   probably only want to reserve a few bytes, this seems fine.
		   Typical frame size is about 800bytes */
		fprintf(stderr,"You're trying to reserve more than 60%% of the mpeg frame for ancillary data\n");
		fprintf(stderr,"This is probably an error. But I'll keep going anyway...\n");
    } 

	adb -= glopts->num_ancillary_bits;


    /* MFC 26 July 2003
       Doing DAB became a bit harder in the reorganisation of the code.
       Now there is no guarantee that there is more than one frame in the bitbuffer.
       But DAB requires that the CRC for the *current* frame be written at the end 
       of the *previous* frame.
       Workaround: Users (Nicholas?) wanting to implement DAB will have to do some work
       in the frontend.
            First: Reserve some bits for yourself (options->num_ancillary_bits)
	    Second: Put the encoder into "single frame mode" i.e. only read 1152 samples
	            per channel. (frontendoptions->singleFrameMode)
	    Third: When you receive each mp2 frame back from the library, you'll have to insert
	           the options->dabCrc[i] values into the end of the frame yourself. (DAB crc calc is 
		   done below)
       The frontend will have to keep the previous frame in memory. 
       As of 26July all that needs to be done is for the frontend to buffer one frame in memory, such that
       the CRC for the next frame can be written in at the end of it.
    */

    {
		int gr, bl, ch;
		/* New polyphase filter
		 Combines windowing and filtering. Ricardo Feb'03 */
		for( gr = 0; gr < 3; gr++ )
		for ( bl = 0; bl < 12; bl++ )
		for ( ch = 0; ch < nch; ch++ )
			window_filter_subband( &glopts->buffer[ch][gr * 12 * 32 + 32 * bl], ch,
				 &(*glopts->sb_sample)[ch][gr][bl][0] );
    }

    scalefactor_calc(*glopts->sb_sample, glopts->scalar, nch, glopts->frame.sblimit);
    find_sf_max (glopts, glopts->scalar, glopts->max_sc);
    if (glopts->frame.actual_mode == JOINT_STEREO) {
		/* this way we calculate more mono than we need */
		/* but it is cheap */
		combine_lr (*glopts->sb_sample, *glopts->j_sample, glopts->frame.sblimit);
		scalefactor_calc (glopts->j_sample, &glopts->j_scale, 1, glopts->frame.sblimit);
    }

    if ((glopts->quickmode == TRUE) && (++glopts->psycount % glopts->quickcount != 0)) {
		/* We're using quick mode, so we're only calculating the model every
		 'quickcount' frames. Otherwise, just copy the old ones across */
		for (ch = 0; ch < nch; ch++) {
			for (sb = 0; sb < SBLIMIT; sb++) {
				glopts->smr[ch][sb] = glopts->smrdef[ch][sb];
			}
		}
	} else {
		/* calculate the psymodel */
		switch (glopts->psymodel) {
			case -1:
				psycho_n1 (glopts, glopts->smr, nch);
			break;
			case 0:	/* Psy Model A */
				psycho_0 (glopts, glopts->smr, glopts->scalar);	
			break;
			case 1:
				psycho_1 (glopts, glopts->buffer, glopts->max_sc, glopts->smr);
			break;
			case 2:
				psycho_2 (glopts, glopts->buffer, sam, glopts->smr );	
			break;
			case 3:
				/* Modified psy model 1 */
				psycho_3 (glopts, glopts->buffer, glopts->max_sc, glopts->smr);
			break;
			case 4:
				psycho_4 (glopts, glopts->buffer, sam, glopts->smr );
			break;	
			default:
				fprintf (stderr, "Invalid psy model specification: %i\n", glopts->psymodel);
				exit (0);
		}

		if (glopts->quickmode == TRUE) {
			/* copy the smr values and reuse them later */
			for (ch = 0; ch < nch; ch++) {
				for (sb = 0; sb < SBLIMIT; sb++) glopts->smrdef[ch][sb] = glopts->smr[ch][sb];
			}
		}
	}


    sf_transmission_pattern (glopts, glopts->scalar, glopts->scfsi);
    main_bit_allocation (glopts, glopts->smr, glopts->scfsi, glopts->bit_alloc, &adb);

    if (glopts->error_protection)
      crc_calc (glopts, glopts->bit_alloc, glopts->scfsi, &glopts->crc);

    write_header (glopts, bs);
	if (glopts->error_protection)
		buffer_putbits (bs, glopts->crc, 16);
    write_bit_alloc (glopts, glopts->bit_alloc, bs);
    write_scalefactors(glopts, glopts->bit_alloc, glopts->scfsi, glopts->scalar, bs);
    subband_quantization (glopts, glopts->scalar, *glopts->sb_sample, glopts->j_scale, *glopts->j_sample, glopts->bit_alloc,
    			  *glopts->subband);
    write_samples(glopts, *glopts->subband, glopts->bit_alloc, bs);

    /* If not all the bits were used, write out a stack of zeros */
    for (i = 0; i < adb; i++)	buffer_put1bit (bs, 0);


    /* MFC July 03 
       Write an extra byte for 16/24/32/48 input when padding is on.
       Something must be going astray with the frame size calcs.
       This fudge works fine for the moment */
    if ((glopts->header.samplerate_idx != 0) && (glopts->padding)) // i.e. not a 44.1 or 22kHz input file
		buffer_putbits (bs, 0, 8);

    if (glopts->do_dab) {
		// Do the CRC calc for DAB stuff if required.
		// It will be up to the frontend to insert it into the end of the 
		// previous frame.
		for (i=glopts->dab_crc_len-1; i>=0; i--) {
			dab_crc_calc (glopts, glopts->bit_alloc, glopts->scfsi, glopts->scalar, &glopts->dab_crc[i], i);
		}
    }


	// Allocate space for the reserved ancillary bits
	for (i=0; i<glopts->num_ancillary_bits;i++) 
		buffer_put1bit(bs, 0);


	// Calulate the number of bits in this frame
    frameBits = buffer_sstell(bs) - initial_bits;
    if (frameBits % 8) {	/* a program failure */
		fprintf (stderr, "Sent %ld bits = %ld slots plus %ld\n", frameBits, frameBits/8, frameBits%8);
		fprintf (stderr, "If you are reading this, the program is broken\n");
		fprintf (stderr, "email [mfc at NOTplanckenerg.com] without the NOT\n");
		fprintf (stderr, "with the command line arguments and other info\n");
		exit (0);
    }

    //fprintf(stdout,"Frame size: %li\n\n",frameBits/8);

	// *** FIXME currently broken njh Sept 04
    if (glopts->do_energy_levels)
    	do_energy_levels(glopts, bs);

	return frameBits/8;
}



/*
  glopts
  leftpcm - holds left channel (or mono channel)
  rightpcm - d'uh
  num_samples - the number of samples in each channel
  mp2buffer - a pointer to the place where we want the mpeg data to be written
  mp2buffer_size - how much space the user allocated for this buffer
  mp2fill_size - how much mpeg data the library has put into the mp2buffer 
*/

int toolame_encode_buffer(
		toolame_options *glopts,
		const short int leftpcm[],
		const short int rightpcm[],
		int num_samples,
		unsigned char *mp2buffer,
		int mp2buffer_size )
{
	int mp2_size = 0;
	int i;
	
	/* now would be a great time to validate the size of the buffer.
	 samples/1152 * sizeof(frame) < mp2buffer_size */
	bit_stream *mybs = buffer_init( mp2buffer, mp2buffer_size );

 
	//fprintf(stdout, "Asked for %i samples now. %i from before\n",samples, glopts->samples_left);

	while (num_samples+glopts->samples_left >= 1152) { // Check that there's enough PCM data for one frame
		// Copy a set of samples into the buffers
		for (i=glopts->samples_left;i<1152;i++) {
			glopts->buffer[0][i] = *leftpcm++;
			if (glopts->frame.nch==2)
			glopts->buffer[1][i] = *rightpcm++;
		}
		
		// Encode the samples in gltops->buffer to a single frame
		mp2_size += toolame_encode_frame( glopts, mybs );
		
		num_samples = num_samples - 1152 + glopts->samples_left;
		glopts->samples_left = 0;
	} // end of one frame


	/* Cope with non-integral numbers of samples */
	if (num_samples != 0) {
		// Remember samples left
		glopts->samples_left = num_samples;

		// Copy these remaining samples into the buffer.
		for (i=0;i<num_samples;i++) {
			glopts->buffer[0][i] = *leftpcm++;
			if (glopts->frame.nch==2)
			glopts->buffer[1][i] = *rightpcm++;
		}
	}
	
	//fprintf(stderr, "samples_left=%d\n", glopts->samples_left);
  
	// free up the bit stream buffer structure
	buffer_deinit( &mybs );

	return(mp2_size);
}

int toolame_encode_buffer_interleaved(
		toolame_options *glopts,
		const short int pcm[],
		int num_samples,
		unsigned char *mp2buffer,
		int mp2buffer_size )
{
	int mp2_size = 0;
	int i;
	
	/* now would be a great time to validate the size of the buffer.
	 samples/1152 * sizeof(frame) < mp2buffer_size */
	bit_stream *mybs = buffer_init( mp2buffer, mp2buffer_size );


	while (num_samples+glopts->samples_left >= 1152) { // Check that there's enough PCM data for one frame
		// Copy a set of samples into the buffers
		for (i=glopts->samples_left;i<1152;i++) {
			glopts->buffer[0][i] = *pcm++;
			if (glopts->frame.nch==2)
			glopts->buffer[1][i] = *pcm++;
		}
		
		// Encode the samples in gltops->buffer to a single frame
		mp2_size += toolame_encode_frame( glopts, mybs );
		
		num_samples = num_samples - 1152 + glopts->samples_left;
		glopts->samples_left = 0;
	} // end of one frame


	/* Cope with non-integral numbers of samples */
	if (num_samples != 0) {
		// Remember samples left
		glopts->samples_left = num_samples;

		// Copy these remaining samples into the buffer.
		for (i=0;i<num_samples;i++) {
			glopts->buffer[0][i] = *pcm++;
			if (glopts->frame.nch==2)
			glopts->buffer[1][i] = *pcm++;
		}
	}
	
	// free up the bit stream buffer structure
	buffer_deinit( &mybs );

	return(mp2_size);
}


void toolame_close(toolame_options **glopts) {
	toolame_options *opts = *glopts;

	// free mem
	if (opts->p4mem) psycho_4_deinit( &opts->p4mem );
	if (opts->p3mem) psycho_3_deinit( &opts->p3mem );
	if (opts->p2mem) psycho_2_deinit( &opts->p2mem );
	if (opts->p1mem) psycho_1_deinit( &opts->p1mem );
	if (opts->p0mem) psycho_0_deinit( &opts->p0mem );
	
	if (opts->alloc_tab) toolame_free( (void **) &opts->alloc_tab );
	if (opts->subband) toolame_free( (void **) &opts->subband );
	if (opts->j_sample) toolame_free( (void **) &opts->j_sample );
	if (opts->sb_sample) toolame_free( (void **) &opts->sb_sample );
	
	toolame_free ( (void **)glopts );
}

int toolame_encode_flush(toolame_options *glopts, unsigned char *mp2buffer, int mp2buffer_size) {
	bit_stream *mybs = NULL;
	int mp2_size = 0;
	int i;
	
	//fprintf(stderr, "toolame_encode_flush: samples_left=%d\n", glopts->samples_left );

	if (glopts->samples_left==0) {
		// No samples left over
		return 0;
	}

	// Create bit stream structure
	mybs = buffer_init( mp2buffer, mp2buffer_size );

	/* Pad out the PCM buffers with 0 and encode the frame */
	for (i=glopts->samples_left; i< 1152; i++) {
		glopts->buffer[0][i] = glopts->buffer[1][i] = 0;
	}

	/* Encode the frame */
	mp2_size = toolame_encode_frame( glopts, mybs );


	// free up the bit stream buffer structure
	buffer_deinit( &mybs );

	return mp2_size;
}


/* 1: MPEG-1, 0: MPEG-2 LSF, 1995-07-11 shn */
static const int bitrate[2][15] = {
{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},
{0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384}
};


int toolame_get_bitrate_index (MPEG_version version, int bRate) {
/* legal bitrates rates from 32 to 448 
   version is MPEG-1 or MPEG-2 LSF */ 
/* convert bitrate in kbps to index */
  int index = 0;
  int found = 0;

  // MFC sanity check.
  if (version!=0 && version!=1) {
    fprintf(stderr,"toolame_get_bitrate_index error %i\n",version);
    exit(99);
  }

  while (!found && index < 15) {
    if (bitrate[version][index] == bRate)
      found = 1;
    else
      ++index;
  }
  if (found)
    return (index);
  else {
    fprintf (stderr,
	     "toolame_get_bitrate_index: %d is not a legal bitrate for version %i\n",
	     bRate, version);
    exit (-1);			/* Error! */
  }
}

int toolame_get_samplerate_index (long sRate, MPEG_version *version)
/* convert samp frq in Hz to index */
/* legal rates 16000, 22050, 24000, 32000, 44100, 48000 */
{
  /* 1: MPEG-1, 0: MPEG-2 LSF, 1995-07-11 shn */
  //static FLOAT s_freq[2][4] = { {22.05, 24, 16, 0}, {44.1, 48, 32, 0} };


  if (sRate == 44100L) {
    *version = MPEG1;
    return (0);
  } else if (sRate == 48000L) {
    *version = MPEG1;
    return (1);
  } else if (sRate == 32000L) {
    *version = MPEG1;
    return (2);
  } else if (sRate == 24000L) {
    *version = MPEG2_LSF;
    return (1);
  } else if (sRate == 22050L) {
    *version = MPEG2_LSF;
    return (0);
  } else if (sRate == 16000L) {
    *version = MPEG2_LSF;
    return (2);
  } else {
    fprintf (stderr, "SmpFrqIndex: %ld is not a legal sample rate\n", sRate);
    exit(99);
  }
}

