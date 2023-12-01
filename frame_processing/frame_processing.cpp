#include "frame_processing.h"

#include "game.h"

void debounce(bool &move_cw, bool &move_ccw) {
	static bool cw_pressed = 0;
	static bool ccw_pressed = 0;

	if (!move_cw && cw_pressed) cw_pressed = 0;
	if (move_cw && !cw_pressed) cw_pressed = 1;
	else move_cw = 0;

	if (!move_ccw && ccw_pressed) ccw_pressed = 0;
	if (move_ccw && !ccw_pressed) ccw_pressed = 1;
	else move_ccw = 0;
}

void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input, bool move_cw, bool move_ccw) {
#pragma HLS INTERFACE axis port=output,input
#pragma HLS PIPELINE
    static GfxEngine g;
    static ap_uint<4> s = 0;
    debounce(move_cw, move_ccw);
    if (move_cw) s++;
    else if (move_ccw) s--;
    hls::stream<streaming_data> i, j;
    g.read(input, i);
    g.draw_num(s, 0x00ff00, Point(120, 1), i, j);
    g.write(j, output);
}
