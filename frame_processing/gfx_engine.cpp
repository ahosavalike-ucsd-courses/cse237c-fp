#include "gfx_engine.h"

ap_uint<1> GfxEngine::last = 0;
Point GfxEngine::loc = Point(0, 0);
ap_uint<FCB> GfxEngine::tick = 0;

void GfxEngine::read(hls::stream<pixel> &input, hls::stream<streaming_data> &output) {
	loc.x++;
	pixel p = input.read();
    if (GfxEngine::last) {
    	loc.y++;
    	loc.x = 0;
    }
    last = p.last;
    if (p.user) {
    	loc.x = loc.y = 0;
    	tick++;  // Will overflow, only track till meaningful
    }
    streaming_data c = {color : p.data, user : p.user, last : p.last, loc: loc, tick: tick};
    output << c;
}

void GfxEngine::draw(Shape s, rgb color, Point i, Point j, hls::stream<streaming_data> &input, hls::stream<streaming_data> &output) {
    streaming_data p = input.read();
    if(s == LINE) {
    	if (p.loc.x >= i.x && p.loc.x <= j.x && p.loc.y >= i.y && p.loc.y <= j.y && (p.loc.y - i.y) * (j.x - i.x) == (p.loc.x - i.x) * (j.y - i.y))
			p.color = color;
    }
	else if (s == RECTANGLE) {
		if ((p.loc.x >= i.x && p.loc.x <= j.x && (p.loc.y == i.y || p.loc.y == j.y)) ||
			(p.loc.y >= i.y && p.loc.y <= j.y && (p.loc.x == i.x || p.loc.x == j.x)))
			p.color = color;
	} else if (s == RECTANGLE_FILLED) {
		if (p.loc.x >= i.x && p.loc.x <= j.x && p.loc.y >= i.y && p.loc.y <= j.y)
			p.color = color;
	}
    output << p;
}

#define FONTX 2
#define FONTY 3
void GfxEngine::draw_num(ap_uint<4> n, rgb color, Point x, hls::stream<streaming_data> &input, hls::stream<streaming_data> &output) {
    if (n == 0) draw(RECTANGLE, color, x, x + Point(FONTX, FONTY), input, output);
    else if (n == 1) draw(LINE, color, x + Point(FONTX, 0), x + Point(FONTX, FONTY), input, output);
    else if (n == 2) {
    	hls::stream<streaming_data> i("GfxEngine::draw_num.i"), j("GfxEngine::draw_num.j"), k("GfxEngine::draw_num.k"), l("GfxEngine::draw_num.l");
    	draw(LINE, color, x, x + Point(FONTX, 0), input, i);
		draw(LINE, color, x + Point(0, FONTY / 2), x + Point(FONTX, FONTY / 2), i, j);
		draw(LINE, color, x + Point(0, FONTY), x + Point(FONTX, FONTY), j, k);
		draw(LINE, color, x + Point(FONTX, 0), x + Point(FONTX, FONTY / 2), k, l);
		draw(LINE, color, x + Point(0, FONTY / 2), x + Point(0, FONTY), l, output);
    } else if (n == 3) {
    	hls::stream<streaming_data> i("GfxEngine::draw_num.i"), j("GfxEngine::draw_num.j"), k("GfxEngine::draw_num.k");
    	draw(LINE, color, x, x + Point(FONTX, 0), input, i);
		draw(LINE, color, x + Point(0, FONTY / 2), x + Point(FONTX, FONTY / 2), i, j);
		draw(LINE, color, x + Point(0, FONTY), x + Point(FONTX, FONTY), j, k);
		draw(LINE, color, x + Point(FONTX, 0), x + Point(FONTX, FONTY), k, output);
    }
	else if (n == 4) {
		hls::stream<streaming_data> i("GfxEngine::draw_num.i"), j("GfxEngine::draw_num.j");
		draw(LINE, color, x, x + Point(0, FONTY / 2), input, i);
		draw(LINE, color, x + Point(0, FONTY / 2), x + Point(FONTX, FONTY / 2), i, j);
		draw(LINE, color, x + Point(FONTX, 0), x + Point(FONTX, FONTY), j, output);
	}
    else if (n == 5) {
    	hls::stream<streaming_data> i("GfxEngine::draw_num.i"), j("GfxEngine::draw_num.j"), k("GfxEngine::draw_num.k"), l("GfxEngine::draw_num.l");
    	draw(LINE, color, x, x + Point(FONTX, 0), input, i);
		draw(LINE, color, x + Point(0, FONTY / 2), x + Point(FONTX, FONTY / 2), i, j);
		draw(LINE, color, x + Point(0, FONTY), x + Point(FONTX, FONTY), j, k);
		draw(LINE, color, x, x + Point(0, FONTY / 2), k, l);
		draw(LINE, color, x + Point(FONTX, FONTY / 2), x + Point(FONTX, FONTY), l, output);
    }
    else if (n == 6) {
    	hls::stream<streaming_data> i("GfxEngine::draw_num.i"), j("GfxEngine::draw_num.j");
    	draw(LINE, color, x, x + Point(FONTX, 0), input, i);
		draw(LINE, color, x, x + Point(0, FONTY / 2), i, j);
		draw(RECTANGLE, color, x + Point(0, FONTY / 2), x + Point(FONTX, FONTY), j, output);
    }
	else if (n == 7) {
		hls::stream<streaming_data> i("GfxEngine::draw_num.i");
		draw(LINE, color, x, x + Point(FONTX, 0), input, i);
		draw(LINE, color, x + Point(FONTX, 0), x + Point(FONTX, FONTY), i, output);
	}
    else if (n == 8) {
    	hls::stream<streaming_data> i("GfxEngine::draw_num.i");
    	draw(LINE, color, x + Point(0, FONTY / 2), x + Point(FONTX, FONTY / 2), input, i);
		draw(RECTANGLE, color, x, x + Point(FONTX, FONTY), i, output);
    } else if (n == 9) {
    	hls::stream<streaming_data> i("GfxEngine::draw_num.i"), j("GfxEngine::draw_num.j");
    	draw(RECTANGLE, color, x, x + Point(FONTX, FONTY / 2), input, i);
		draw(LINE, color, x + Point(FONTX, FONTY / 2), x + Point(FONTX, FONTY), i, j);
		draw(LINE, color, x + Point(0, FONTY), x + Point(FONTX, FONTY), j, output);
    } else {
    	output.write(input.read());
    }
}

void GfxEngine::write(hls::stream<streaming_data> &input, hls::stream<pixel> &output) {
    streaming_data c = input.read();
    pixel p;
    p.data = c.color;
    p.user = c.user;
    p.last = c.last;
    p.strb = p.keep = 0x7;
    output << p;
}
