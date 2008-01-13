/***************************************************************************
                          DIA_flyEraser.cpp  -  configuration dialog for
						   Eraser filter
                              -------------------
                         Chris MacGregor, December 2007
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

#ifndef FLY_ERASER_H
#define FLY_ERASER_H

class flyEraser : public ADM_flyDialog
{
  
//  protected:
  public:
    ADMVideoEraser * eraserp;
  private:
    const MenuMapping * menu_mapping;
    uint32_t menu_mapping_count;

  public:
    ERASER_PARAM param;
    Eraser::MaskVec::iterator current_mask;

    uint8_t    process();
    uint8_t    update();

    flyEraser (uint32_t width, uint32_t height,
               AVDMGenericVideoStream * in,
               void * canvas, void * slider, void * dialog,
               ADMVideoEraser * eraserp, const ERASER_PARAM * in_param,
               const MenuMapping * menu_mapping, uint32_t menu_mapping_count)
        : ADM_flyDialog (width, height, in, canvas, slider, 1, RESIZE_AUTO),
          eraserp (eraserp),
          param (*in_param),
          menu_mapping (menu_mapping),
          menu_mapping_count (menu_mapping_count),
          current_mask (eraserp->getMasks().begin())
    {
        _cookie = dialog;
        setupMenus (in_param, menu_mapping, menu_mapping_count);
        // printf ("flyEraser::flyEraser: "
        //         "in_param %p, &param %p\n", in_param, &param);
    };

    uint8_t sliderChanged (void);

    void getParam (ERASER_PARAM * outputParam)
    {
        *outputParam = param;
    }

    ADMImage * getInputImage ()
    {
        return _yuvBuffer;
    }

    ADMImage * getOutputImage ()
    {
        return _yuvBufferOut;
    }

    const MenuMapping * lookupMenu (const char * widgetName)
    {
        return ADM_flyDialog::lookupMenu (widgetName, menu_mapping,
                                          menu_mapping_count);
    }

    void getMenuValues ()
    {
        ADM_flyDialog::getMenuValues (&param, menu_mapping,
                                      menu_mapping_count);
    }
};

#endif
