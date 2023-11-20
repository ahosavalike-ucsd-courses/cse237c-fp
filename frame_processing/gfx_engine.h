#ifndef __GFX_ENGINE__
#define __GFX_ENGINE__
#include "consts_types.h"
//#include <common/xf_headers.hpp>

#define XFMT CV_8UC3, FRAME_HEIGHT, FRAME_WIDTH, XF_NPPC1

//typedef xf::cv::Mat<XFMT> xfcvmat;

enum shape {
	LINE,
	RECTANGLE,
	RECTANGLE_FILLED,
};

typedef struct GfxEngine {
	ap_uint<11> x;
	ap_uint<11> y;
	ap_uint<11> fc;
	pixel p;
	struct GfxEngine& read(hls::stream<pixel> &input);
	struct GfxEngine& draw_diag();
	struct GfxEngine& draw(shape s, rgb c, ap_uint<11> xi, ap_uint<11> yi, ap_uint<11> xj, ap_uint<11> yj);
	void write(hls::stream<pixel> &output);
} GfxEngine;

#endif
