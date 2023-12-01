#include "frame_processing.h"

#include "game.h"

void debounce(bool &move_cw, bool &move_ccw) {
    static bool cw_pressed = 0;
    static bool ccw_pressed = 0;

    if (!move_cw && cw_pressed) cw_pressed = 0;
    if (move_cw && !cw_pressed)
        cw_pressed = 1;
    else
        move_cw = 0;

    if (!move_ccw && ccw_pressed) ccw_pressed = 0;
    if (move_ccw && !ccw_pressed)
        ccw_pressed = 1;
    else
        move_ccw = 0;
}

void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input, bool move_cw, bool move_ccw) {
#pragma HLS INTERFACE axis port = output, input
#pragma HLS PIPELINE
    static Game g;
    static ap_uint<4> s = 0;
    debounce(move_cw, move_ccw);
    g.run(input, output, (move_ccw << 1) | move_cw | move_ccw);
}
