#include "consts_types.h"
//#include <common/xf_headers.hpp>

#define XFMT CV_8UC3, FRAME_HEIGHT, FRAME_WIDTH, XF_NPPC1

//typedef xf::cv::Mat<XFMT> xfcvmat;

typedef struct GfxEngine {
	ap_uint<11> x;
	ap_uint<11> y;
	ap_uint<11> fc;
	pixel p;
	void read(pixel input) {
		p = input;
		if (p.user) {
			x = y = 0;
			fc++;	// Will overflow, only track till meaningful
		}
	}
	void draw_diag() {
		if (x == y)
			if (x < fc % FRAME_HEIGHT)
				p.data = 0x0000ff;
			else if (x > fc % FRAME_HEIGHT)
				p.data = 0xff0000;
			else
				p.data = 0xffffff;
	}
	pixel write() {
		x++;
		if (p.last) {
			y++;
			x = 0;
		}
		return p;
	}
} GfxEngine;
