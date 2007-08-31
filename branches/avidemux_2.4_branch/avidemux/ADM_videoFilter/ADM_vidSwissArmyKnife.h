/***************************************************************************
                          ADM_vidSwissArmyKnife.h  -  Perform one of many
                                                      possible operations
                             -------------------
                          Chris MacGregor, 2005, 2007
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
 
#ifndef __SWISSARMYKNIFE__
#define __SWISSARMYKNIFE__   

#include <map>

#include "ADM_video/ADM_cache.h"

#include <vector>
#include <string>

#include <sys/time.h>

struct SWISSARMYKNIFE_PARAM
{
    uint32_t tool; // ADMVideoSwissArmyKnife::Tool
    uint32_t input_type; // ADMVideoSwissArmyKnife::Input

    const char * input_file;
    float load_bias;
    float load_multiplier;

    float input_constant;

    uint32_t memory_constant_denom;
    uint32_t init_start_frame;
    uint32_t init_end_frame;
    uint32_t init_by_rolling;

    int32_t  bias;
    int32_t  scale_from_min;
    int32_t  scale_from_max;
    uint32_t histogram_frame_interval;
    uint32_t debug;

};

class ADMVideoSwissArmyKnife : public AVDMGenericVideoStream
{
protected:
    	
    // This is a hack to work around the fact that the ctor & dtor get called
    // too often.  The right solution would be to arrange for the filter
    // objects to be constructed and destructed only when really necessary:
    // when a new instance of a filter is added to the list (by the user), it
    // is constructed, and when it is removed from the list (by the user), it
    // is destructed, and anything else is handled by a separate init() or
    // configure() method.  This would allow the objects to maintain a
    // persistent state in a more straightforward way.

    typedef std::vector <float> FloatVector;

    class PersistentInfo
    {
    public:
        CONFcouple *  conf;
        CONFcouple *  oldConf;
        uint32_t      refCount;

        FloatVector   kernel;
        uint32_t      kernel_w;
        uint32_t      kernel_h;

        uint32_t      image_w;
        uint32_t      image_h;
        uint8_t *     image_int;
        float *       image_float;

        std::string   input_file_name;
        time_t        input_file_mtime;

        float *       bg;
        uint32_t      bg_mc;
        uint32_t      bg_isf;
        uint32_t      bg_ief;
        uint32_t      bg_x;
        uint32_t      bg_y;

        uint32_t      histogram_frame_interval;
        uint32_t *    histogram_input_data;
        uint32_t *    histogram_output_data;
        uint32_t      histogram_frame_count;

        bool          bg_ibr;
        bool          histogram_data_invalid;
        bool          image_data_invalid;

        PersistentInfo ()
            : conf (0),
              oldConf (0),
              refCount (0),

              kernel_w (0),
              kernel_h (0),

              image_w (0),
              image_h (0),
              image_int (0),
              image_float (0),

              input_file_mtime (0),

              bg (0),
              bg_mc (0),
              bg_isf (0),
              bg_ief (0),
              bg_x (0),
              bg_y (0),

              histogram_frame_interval (0),
              histogram_input_data (0),
              histogram_output_data (0),
              histogram_frame_count (0),

              bg_ibr (false),
              histogram_data_invalid (false),
              image_data_invalid (false)
        {
        }

        ~PersistentInfo ()
        {
            delete image_int;
            delete image_float;
            delete bg;
            delete histogram_input_data;
            delete histogram_output_data;
        }
    };

    typedef std::map <CONFcouple *, PersistentInfo *> PImap;
    static PImap pimap;

    PersistentInfo * myInfo;

    enum Input
    {
        INPUT_INVALID = 0,

        INPUT_FILE_IMAGE_FLOAT,
        INPUT_FILE_IMAGE_INTEGER,
        INPUT_CUSTOM_CONVOLUTION,
        INPUT_CONSTANT_VALUE,
        INPUT_ROLLING_AVERAGE,

        INPUT_COUNT
    };

    enum Tool
    {
        TOOL_INVALID = 0,

        TOOL_A,          // P' = A
        TOOL_P,          // P' = P
        TOOL_P_MINUS_A,  // P' = P - A
        TOOL_A_MINUS_P,  // P' = A - P
        TOOL_P_PLUS_A,   // P' = P + A
        TOOL_P_TIMES_A,  // P' = P * A
        TOOL_P_DIVBY_A,  // P' = P / A
        TOOL_A_DIVBY_P,  // P' = A / P
        TOOL_MIN_P_A,    // P' = min (P, A)
        TOOL_MAX_P_A,    // P' = max (P, A)

        TOOL_A_WITH_HISTOGRAM,
        TOOL_P_WITH_HISTOGRAM,
        TOOL_P_MINUS_A_WITH_HISTOGRAM,
        TOOL_A_MINUS_P_WITH_HISTOGRAM,
        TOOL_P_PLUS_A_WITH_HISTOGRAM,
        TOOL_P_TIMES_A_WITH_HISTOGRAM,
        TOOL_P_DIVBY_A_WITH_HISTOGRAM,
        TOOL_A_DIVBY_P_WITH_HISTOGRAM,
        TOOL_MIN_P_A_WITH_HISTOGRAM,
        TOOL_MAX_P_A_WITH_HISTOGRAM,

        TOOL_A_SCALED,
        TOOL_P_SCALED,
        TOOL_P_MINUS_A_SCALED,
        TOOL_A_MINUS_P_SCALED,
        TOOL_P_PLUS_A_SCALED,
        TOOL_P_TIMES_A_SCALED,
        TOOL_P_DIVBY_A_SCALED,
        TOOL_A_DIVBY_P_SCALED,
        TOOL_MIN_P_A_SCALED,
        TOOL_MAX_P_A_SCALED,

        TOOL_A_SCALED_WITH_HISTOGRAM,
        TOOL_P_SCALED_WITH_HISTOGRAM,
        TOOL_P_MINUS_A_SCALED_WITH_HISTOGRAM,
        TOOL_A_MINUS_P_SCALED_WITH_HISTOGRAM,
        TOOL_P_PLUS_A_SCALED_WITH_HISTOGRAM,
        TOOL_P_TIMES_A_SCALED_WITH_HISTOGRAM,
        TOOL_P_DIVBY_A_SCALED_WITH_HISTOGRAM,
        TOOL_A_DIVBY_P_SCALED_WITH_HISTOGRAM,
        TOOL_MIN_P_A_SCALED_WITH_HISTOGRAM,
        TOOL_MAX_P_A_SCALED_WITH_HISTOGRAM,

        TOOL_TOTAL_COUNT,

        TOOL_ADD_HISTOGRAM = TOOL_A_WITH_HISTOGRAM - TOOL_A,
        TOOL_ADD_SCALING = TOOL_A_SCALED - TOOL_A,

    };

    struct ToolMap
    {
        Tool toolid;
        const char * outputName; // displayed name
        const char * format;     // printf format (%s is A)
    };

    static ToolMap tool_map [];

    SWISSARMYKNIFE_PARAM *  _param;

public:

    ADMVideoSwissArmyKnife (AVDMGenericVideoStream *in,CONFcouple *setup);
    ~ADMVideoSwissArmyKnife();

    virtual uint8_t getFrameNumberNoAlloc (uint32_t frame, uint32_t *len,
                                           ADMImage *data, uint32_t *flags);

    virtual uint8_t configure (AVDMGenericVideoStream *instream);
    virtual char * printConf(void);
    virtual uint8_t getCoupledConf (CONFcouple **couples);

private:

    template <typename Oper, typename Histo>
    void computeRollingAverage (ADMImage * image, ADMImage * data,
                                uint32_t planesize, int32_t bias,
                                const Oper & op_in,
                                const Histo & histogram_in);

    template <typename InputImageType, typename Oper, typename Histo>
    void applyImage (ADMImage * image, ADMImage * data,
                     uint32_t planesize, int32_t bias,
                     InputImageType * input_image,
                     const Oper & op_in, const Histo & histogram_in);

    template <typename Oper, typename Histo>
    void applyConstant (ADMImage * image, ADMImage * data,
                        uint32_t planesize, int32_t bias,
                        const Oper & op_in,
                        const Histo & histogram_in);

};
#endif
