#ifndef __GFX_ENGINE__
#define __GFX_ENGINE__
#include "consts_types.h"
//#include <common/xf_headers.hpp>

#define XFMT CV_8UC3, FRAME_HEIGHT, FRAME_WIDTH, XF_NPPC1

//typedef xf::cv::Mat<XFMT> xfcvmat;

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
};

typedef struct PointSize<11> Point;

typedef struct GfxEngine {
	Point c;
	ap_uint<11> fc;
	pixel p;
	struct GfxEngine& read(hls::stream<pixel> &input);
	struct GfxEngine& draw_diag();
	struct GfxEngine& draw(Shape s, rgb c, Point i, Point j);
	void write(hls::stream<pixel> &output);
} GfxEngine;

#endif
