#include "gfx_engine.h"

struct GfxEngine& GfxEngine::read(hls::stream<pixel> &input) {
	input >> p;
	if (p.user) {
		c.x = c.y = 0;
		fc++;	// Will overflow, only track till meaningful
		if (fc > FRAME_HEIGHT)
			fc = 1;
	}
	return *this;
}

struct GfxEngine& GfxEngine::draw_diag() {
	if (c.x == c.y)
		if (c.x < fc)
			p.data = 0x0000ff;
		else if (c.x > fc)
			p.data = 0xff0000;
		else
			p.data = 0xffffff;
	return *this;
}

struct GfxEngine& GfxEngine::draw(Shape s, rgb color, Point i, Point j) {
	switch (s) {
	case LINE:
		if ((c.y - i.y) * (j.x - i.x) == (c.x - i.x) * (j.y - i.y))
			p.data = color;
		break;
	case RECTANGLE:
		if (
				(c.x >= i.x && c.x <= j.x && (c.y == i.y || c.y == j.y)) ||
				(c.y >= i.y && c.y <= j.y && (c.x == i.x || c.x == j.x))
		)
			p.data = color;
		break;
	case RECTANGLE_FILLED:
		if (c.x >= i.x && c.x <= j.x && c.y >= i.y && c.y <= j.y)
			p.data = color;
		break;
	}
	return *this;
}

void GfxEngine::write(hls::stream<pixel> &output) {
	c.x++;
	if (p.last) {
		c.y++;
		c.x = 0;
	}
	output << p;
}
