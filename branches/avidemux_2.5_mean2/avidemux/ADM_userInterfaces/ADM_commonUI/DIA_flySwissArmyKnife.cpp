/***************************************************************************
                          DIA_flySwissArmyKnife.cpp  -  configuration dialog for
						   Swiss Army Knife filter
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

#include "default.h"

#include "ADM_colorspace/ADM_rgb.h"

#include "ADM_image.h"
#include "ADM_video/ADM_genvideo.hxx"
#include "ADM_videoFilter/ADM_vidSwissArmyKnife.h"
#include "DIA_flyDialog.h"
#include "DIA_flySwissArmyKnife.h"

/************* COMMON PART *********************/

uint8_t flySwissArmyKnife::sliderChanged ()
{
    // HERE: perhaps we should replace the preview enable button with an
    // additional selection on the preview filter selection list ("Disable" or
    // some such), with a corresponding PREVIEWMODE_NO_PREVIEW.

    if (!param.enable_preview)
        return 0;

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

uint8_t  flySwissArmyKnife::update ()
{
    download();
    if (!param.enable_preview)
        return 0;

    pushParam();
    process();
    copyYuvFinalToRgb();
    return display();
}

uint8_t flySwissArmyKnife::process ()
{
    if (!param.enable_preview)
        return 0;

    if (preview_mode == PREVIEWMODE_THIS_FILTER)
    {
        ADMImage * lai;
        if (param.lookahead_n_frames)
        {
            if (lookaheadimage == NULL)
                lookaheadimage = new ADMImage (_in->getInfo()->width,
                                               _in->getInfo()->height);

            uint32_t len = 0;
            uint32_t flags = 0;
            uint32_t frame = sliderGet();
            if (frame + param.lookahead_n_frames >= _in->getInfo()->nb_frames)
                lai = 0;
            else if (!_in->getFrameNumberNoAlloc (frame
                                                  + param.lookahead_n_frames,
                                                  &len, lookaheadimage,
                                                  &flags))
                return 0;
            else
                lai = lookaheadimage;
        }
        else
            lai = 0;

        uint8_t ret =
            ADMVideoSwissArmyKnife::doSwissArmyKnife (_yuvBuffer, lai,
                                                      _yuvBufferOut, _in,
                                                      sakp, &param, _w, _h);
        return ret;
    }
    else if (preview_mode == PREVIEWMODE_LATER_FILTER)
    {
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
