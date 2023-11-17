#include "consts_types.h"
//#include <common/xf_headers.hpp>

#define XFMT CV_8UC3, FRAME_HEIGHT, FRAME_WIDTH, XF_NPPC1

//typedef xf::cv::Mat<XFMT> xfcvmat;

typedef struct GfxEngine {
	ap_uint<11> x;
	ap_uint<11> y;
	ap_uint<11> fc;
	pixel p;
	struct GfxEngine& read(hls::stream<pixel> &input) {
		input >> p;
		if (p.user) {
			x = y = 0;
			fc++;	// Will overflow, only track till meaningful
			if (fc > FRAME_HEIGHT)
				fc = 1;
		}
		return *this;
	}
	struct GfxEngine& draw_diag() {
		if (x == y)
			if (x < fc)
				p.data = 0x0000ff;
			else if (x > fc)
				p.data = 0xff0000;
			else
				p.data = 0xffffff;
		return *this;
	}
	void write(hls::stream<pixel> &output) {
		x++;
		if (p.last) {
			y++;
			x = 0;
		}
		output << p;
	}
} GfxEngine;
