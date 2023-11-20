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
};

template <int SIZE>
struct PointSize {
    ap_uint<SIZE> x;
    ap_uint<SIZE> y;
    PointSize() {}
    PointSize(int i, int j) : x(i), y(j) {}
    PointSize<SIZE> operator+ (PointSize<SIZE> o) {
    	return PointSize<SIZE>(x+o.x, y+o.y);
    }
    PointSize<SIZE> operator+ (int o) {
		return PointSize<SIZE>(x+o, y+o);
	}
    PointSize<SIZE> operator- (PointSize<SIZE> o) {
		return PointSize<SIZE>(x-o.x, y-o.y);
	}
    PointSize<SIZE> operator- (int o) {
		return PointSize<SIZE>(x-o, y-o);
	}
    bool operator== (PointSize<SIZE> o) {
    	return x == o.x && y == o.y;
    }
};

typedef struct PointSize<11> Point;

typedef struct GfxEngine {
    // Current pixel location
    Point current_location;
    // Current Tick
    ap_uint<11> tick;
    // Pixel value to send
    pixel p;
    struct GfxEngine &read(hls::stream<pixel> &input);
    struct GfxEngine &draw(Shape s, rgb color, Point i, Point j);
    struct GfxEngine &draw_num(ap_uint<4> n, rgb color, Point i);
    void write(hls::stream<pixel> &output);
} GfxEngine;

#endif
