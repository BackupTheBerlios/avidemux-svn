/*
                        ADM_vidCurveEditor.cpp
                        ----------------------
    This program is creating spline curves that can be used for colour
    adjustment. You can edit three curves in YUV colour space.
    email: george.janec@gmail.com
    
    Copyright (C) 2009 Jiri Janecek

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ADM_plugin_translate.h"
#include "ADM_Utils.h"
#include "ADM_PointArrayList.h"
#include "ADM_vidCurveEditor.h"

#ifdef SETS
#undef SETS
#endif
#define SETS(x,y) (*couples)->setCouple(x,y)

#ifdef GETS
#undef GETS
#endif
#define GETS(x,y) ADM_assert(couples->getCouple(x,&y))


static FILTER_PARAM curveParam = {6,
    {"pointsY", "pointsU", "pointsV", "tableY", "tableU", "tableV"}
};

VF_DEFINE_FILTER_UI(
    CurveEditor,                            // class name
    curveParam,                             // filter parameters
    curveEditor,                            // library name
    QT_TR_NOOP("Color Curve Editor"),       // string name
    1,                                      // version
    VF_COLORS,                              // family
    QT_TR_NOOP("Color adjustment by color curves in YUV color space."));


extern uint8_t DIA_RunCurveDialog(p_ColorCurveParam param,
    AVDMGenericVideoStream *in);


/*
 * Serializes transformation table into string representation. Created string
 * is saved to couples.
 */
void CurveEditor::serializeTable(const uint8_t *table, const char *name,
    CONFcouple **couples)
{
    ADM_assert(table);
    ADM_assert(name);
    char *tempStr = new char[256 * 4 + 1];
    ADM_assert(tempStr);
    char *p = tempStr;
    for (int i = 0; i < 256; i++)
    {
        p += sprintf(p, "%d,", table[i]);
    }
    SETS(name, tempStr);
    delete [] tempStr;
}

/*
 * Restores transformation table from input string to table address table.
 */
void CurveEditor::deserializeTable(uint8_t *table, const char *name,
    CONFcouple *couples)
{
    char *token = NULL;
    ADM_assert(table);
    ADM_assert(name);
    GETS(name, token);
    ADM_assert(token);
    token = strtok(token, ",");
    for (int i = 0; i < 256; i++)
    {
        table[i] = (uint8_t) atoi(token);
        token = strtok(NULL, ",");
    }
    ADM_dealloc(token);
}

/*
 * Creates temporary image buffer _uncompressed for uncompressed frames.
 * If couples is not NULL then filter parameters are restored from couples
 * else default values are adjusted.
 */
CurveEditor::CurveEditor(AVDMGenericVideoStream *in, CONFcouple *couples)
{
    ADM_assert(in);
    _in = in;
    memcpy(&_info, _in->getInfo(), sizeof(_info));
    _info.encoding = 1;
    _uncompressed = new ADMImage(_in->getInfo()->width, _in->getInfo()->height);
    ADM_assert(_uncompressed);

    if (couples != NULL)
    {
        char *tempStr = NULL;
        GETS("pointsY",tempStr);
        _param.points[0].deserialize(tempStr);
        if (tempStr) ADM_dealloc(tempStr);
        GETS("pointsU",tempStr);
        _param.points[1].deserialize(tempStr);
        if (tempStr) ADM_dealloc(tempStr);
        GETS("pointsV",tempStr);
        _param.points[2].deserialize(tempStr);
        if (tempStr) ADM_dealloc(tempStr);
        deserializeTable(_param.table[0], "tableY", couples);
        deserializeTable(_param.table[1], "tableU", couples);
        deserializeTable(_param.table[2], "tableV", couples);
    }
    else
    {
        for (int i = 0; i < 256; i++)
        {
            _param.table[0][i] = _param.table[1][i] = _param.table[2][i] = i;
        }
    }
}

/*
 * Frees uncompressed image buffer.
 */
CurveEditor::~CurveEditor()
{
    delete _uncompressed;
    _uncompressed = NULL;
}

/*
 * Returns filter configuration in string.
 */
char *CurveEditor::printConf(void)
{
    static char buf[0xFF];
    sprintf((char *) buf, QT_TR_NOOP("Control points count: Y:%d, U:%d, V:%d"),
        _param.points[0].count(),
        _param.points[1].count(),
        _param.points[2].count());
    return buf;
}

/*
 * Creates couples of current parameters.
 */
uint8_t CurveEditor::getCoupledConf(CONFcouple **couples)
{
    *couples = new CONFcouple(6); // Number of param in your structure
    char *tempStr = _param.points[0].serialize();
    SETS("pointsY", tempStr);
    if (tempStr) delete [] tempStr;
    tempStr = _param.points[1].serialize();
    SETS("pointsU", tempStr);
    if (tempStr) delete [] tempStr;
    tempStr = _param.points[2].serialize();
    SETS("pointsV", tempStr);
    if (tempStr) delete [] tempStr;
    serializeTable(_param.table[0], "tableY", couples);
    serializeTable(_param.table[1], "tableU", couples);
    serializeTable(_param.table[2], "tableV", couples);
    return 1;
}

/*
 * Opens configuration dialog for adjusting parameters.
 */
uint8_t CurveEditor::configure(AVDMGenericVideoStream *in)
{
    _in=in;
    return DIA_RunCurveDialog(&_param, in);
}

/*
 * Core method for applying filter at specified frame number.
 */
uint8_t CurveEditor::getFrameNumberNoAlloc(uint32_t frame,
    uint32_t *len,
    ADMImage *data,
    uint32_t *flags)
{
    if(frame >= _info.nb_frames)
        return 0;
    // read uncompressed frame
    if(!_in->getFrameNumberNoAlloc(frame, len, _uncompressed, flags))
        return 0;

    uint8_t *in, *out;
    uint8_t *currTable;

    uint32_t stride = _info.width;
    uint32_t hstride = stride / 2;
    uint32_t h = _info.height;
    uint32_t hh = h / 2;

    // edit luma
    in = YPLANE(_uncompressed);
    out = YPLANE(data);
    currTable = _param.table[0];
    for (uint32_t y = 0; y < h; y++)
    {
        for (uint32_t x = 0; x < stride; x++)
        {
            *(out + x) = currTable[*(in + x)];  // colour transformation
        }
        in += stride;
        out += stride;
    }

    // edit chroma (U)
    in = UPLANE(_uncompressed);
    out = UPLANE(data);
    currTable = _param.table[1];
    for(uint32_t y = 0; y < hh; y++)
    {
        for (uint32_t x = 0; x < hstride; x++)
        {
            *(out + x) = currTable[*(in + x)];
        }
        in += hstride;
        out += hstride;
    }

    // edit chroma (V)
    in = VPLANE(_uncompressed);
    out = VPLANE(data);
    currTable = _param.table[2];
    for(uint32_t y = 0; y < hh; y++)
    {
        for (uint32_t x = 0; x < hstride; x++)
        {
            *(out + x) = currTable[*(in + x)];
        }
        in += hstride;
        out += hstride;
    }

    data->copyInfo(_uncompressed);

    return 1;
}

