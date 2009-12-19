#ifndef CURVE_EDITOR_H
#define CURVE_EDITOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ADM_default.h"
#include "ADM_videoFilterDynamic.h"

#include "ADM_vidCurveEditor_param.h"


class CurveEditor: public AVDMGenericVideoStream {
    private:
        void serializeTable(const uint8_t *table, const char *name, CONFcouple **couples);
        void deserializeTable(uint8_t *table, const char *name, CONFcouple *couples);
    protected:
        AVDMGenericVideoStream *_in;
        ColorCurveParam _param;
        virtual char *printConf(void);
    public:
        CurveEditor(AVDMGenericVideoStream *in, CONFcouple *setup);
        virtual ~CurveEditor();
        virtual uint8_t getCoupledConf(CONFcouple **couples);
        virtual uint8_t configure(AVDMGenericVideoStream *in);
        virtual uint8_t getFrameNumberNoAlloc(uint32_t frame, uint32_t *len,
            ADMImage *data, uint32_t *flags);
};

#endif // CURVE_EDITOR_H
