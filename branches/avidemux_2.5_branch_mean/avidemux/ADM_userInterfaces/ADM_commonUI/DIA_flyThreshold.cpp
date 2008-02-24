/***************************************************************************
                          DIA_flyThreshold.cpp  -  configuration dialog for
						   threshold filter
                              -------------------
                         Chris MacGregor, September 2007
                         chris-avidemux@bouncingdog.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "config.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "ADM_default.h"
#include "ADM_colorspace/ADM_rgb.h"

#include "ADM_image.h"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_videoFilter/ADM_vidThreshold.h"
#include "DIA_flyDialog.h"
#include "DIA_flyThreshold.h"

/************* COMMON PART *********************/

uint8_t flyThreshold::sliderChanged ()
{
    ADM_assert(_yuvBuffer);
    ADM_assert(_rgbBufferOut);
    ADM_assert(_in);
    
    if (preview_mode == PREVIEWMODE_THIS_FILTER)
    {
        uint32_t frame = sliderGet();
        uint32_t len, flags;

        if (!_in->getFrameNumberNoAlloc (frame, &len, _yuvBuffer, &flags))
        {
            printf ("[FlyDialog] Cannot get frame %u\n", frame);
            return 0;
        }
    }
    else if (preview_mode == PREVIEWMODE_LATER_FILTER)
    {
        // No need to call getFrameNumberNoAlloc() here, because our process()
        // (see below) will do it (indirectly, by calling the later filter's
        // getFrameNumberNoAlloc(), which will call the preceding filter's
        // getFrameNumberNoAlloc(), which in turn will call it for the filter
        // before that, etc.):
    }
    else // if (preview_mode == PREVIEWMODE_EARLIER_FILTER)
    {
        // In this case, we just call the earlier filter's
        // getFrameNumberNoAlloc() and send the result straight into the
        // output buffer - we needn't do anything to it ourselves.

        uint32_t frame = sliderGet();
        uint32_t len, flags;

        if (!source->getFrameNumberNoAlloc (frame, &len, _yuvBufferOut,
                                            &flags))
        {
            printf ("[FlyDialog] Cannot get frame %u\n", frame);
            return 0;
        }
    }

    // Process...    
    if (_isYuvProcessing)
    {
        process();
        copyYuvFinalToRgb();
    }
    else // RGB Processing      
    {
        // HERE: we are NOT set up for this to work...
        ADM_assert(0);
        ADM_assert(_rgbBuffer);
        copyYuvScratchToRgb();
        process();
    }

    return display();
}

uint8_t  flyThreshold::update ()
{
    download();
    pushParam();
    process();
    copyYuvFinalToRgb();
    return display();
}

uint8_t flyThreshold::process ()
{
    if (preview_mode == PREVIEWMODE_THIS_FILTER)
    {
        if (thresholdp->computeLookupTable (&param))
            upload();

        ADMVideoThreshold::doThreshold (_yuvBuffer, _yuvBufferOut, thresholdp,
                                        (_h * _w * 3) >> 1);
        return 1;
    }
    else if (preview_mode == PREVIEWMODE_LATER_FILTER)
    {
        if (thresholdp->computeLookupTable (&param))
            upload();

        uint32_t len = 0;
        uint32_t flags = 0;
        uint32_t frame = sliderGet();
        return source->getFrameNumberNoAlloc (frame, &len, _yuvBufferOut,
                                              &flags);
    }
    else // if (preview_mode == PREVIEWMODE_EARLIER_FILTER)
    {
        // nothing to do here - we've already arranged for the earlier filter
        // to write the result into the output buffer, and changes in our
        // parameters can't affect the result of an earlier filter.

        return 1;
    }
}

/************* COMMON PART *********************/
