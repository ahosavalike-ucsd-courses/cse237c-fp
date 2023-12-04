#ifndef __GFX_ENGINE__
#define __GFX_ENGINE__
#include "consts_types.h"
// #include <common/xf_headers.hpp>

#define XFMT CV_8UC3, FRAME_HEIGHT, FRAME_WIDTH, XF_NPPC1

// typedef xf::cv::Mat<XFMT> xfcvmat;

enum Shape {
    LINE,
    RECTANGLE,
    RECTANGLE_FILLED,
    POINT,
};

typedef struct GfxEngine {
    // Remember if prev pixel was last in that line
    static ap_uint<1> last;
    // Current pixel location
    static Point loc;
    // Current Tick
    static ap_uint<FCB> tick;
    static void read(hls::stream<pixel> &input, hls::stream<streaming_data> &output);
    static void draw(Shape s, rgb color, Point i, Point j, hls::stream<streaming_data> &input, hls::stream<streaming_data> &output);
    static void draw_num(ap_uint<4> n, rgb color, Point i, hls::stream<streaming_data> &input, hls::stream<streaming_data> &output);
    static void write(hls::stream<streaming_data> &input, hls::stream<pixel> &output);
} GfxEngine;

#endif
