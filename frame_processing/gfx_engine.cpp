#include "gfx_engine.h"

struct GfxEngine& GfxEngine::read(hls::stream<pixel> &input) {
	input >> p;
	if (p.user) {
		x = y = 0;
		fc++;	// Will overflow, only track till meaningful
		if (fc > FRAME_HEIGHT)
			fc = 1;
	}
	return *this;
}

struct GfxEngine& GfxEngine::draw_diag() {
	if (x == y)
		if (x < fc)
			p.data = 0x0000ff;
		else if (x > fc)
			p.data = 0xff0000;
		else
			p.data = 0xffffff;
	return *this;
}

struct GfxEngine& GfxEngine::draw(shape s, rgb color, ap_uint<11> xi, ap_uint<11> yi, ap_uint<11> xj, ap_uint<11> yj) {
	switch (s) {
	case LINE:
		if ((y - yi) * (xj - xi) == (x - xi) * (yj - yi))
			p.data = color;
		break;
	case RECTANGLE:
		if ((x >= xi && x <= xj && (y == yi || y == yj)) || (y >= yi && y <= yj && (x == xi || x == xj)))
			p.data = color;
		break;
	case RECTANGLE_FILLED:
		if (x >= xi && x <= xj && y >= yi && y <= yj)
			p.data = color;
		break;
	}
	return *this;
}

void GfxEngine::write(hls::stream<pixel> &output) {
	x++;
	if (p.last) {
		y++;
		x = 0;
	}
	output << p;
}
