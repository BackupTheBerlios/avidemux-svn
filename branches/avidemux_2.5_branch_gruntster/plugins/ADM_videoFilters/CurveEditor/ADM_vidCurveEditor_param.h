#ifndef CURVE_EDITOR_PARAM_H
#define CURVE_EDITOR_PARAM_H

#include "ADM_PointArrayList.h"

/*
 * Filter parameters.
 */
typedef struct
{
    // Lists of control points.
    PointArrayList points[3];
    // Transformation colour tables with bounds 0..255.
    unsigned char table[3][256];
} ColorCurveParam, *p_ColorCurveParam;

#endif

