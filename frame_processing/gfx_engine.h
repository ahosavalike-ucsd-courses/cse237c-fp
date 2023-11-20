#ifndef __GFX_ENGINE__
#define __GFX_ENGINE__
#include "consts_types.h"
//#include <common/xf_headers.hpp>

#define XFMT CV_8UC3, FRAME_HEIGHT, FRAME_WIDTH, XF_NPPC1

//typedef xf::cv::Mat<XFMT> xfcvmat;

typedef struct GfxEngine {
	ap_uint<11> x;
	ap_uint<11> y;
	ap_uint<11> fc;
	pixel p;
	struct GfxEngine& read(hls::stream<pixel> &input);
	struct GfxEngine& draw_diag();
	struct GfxEngine& draw_rect(rgb c, ap_uint<11> xi, ap_uint<11> yi, ap_uint<11> xj, ap_uint<11> yj);
	void write(hls::stream<pixel> &output);
} GfxEngine;

typedef struct Game {
	GfxEngine g;
	void run(hls::stream<pixel> &output, hls::stream<pixel> &input);
} Game;

#endif
