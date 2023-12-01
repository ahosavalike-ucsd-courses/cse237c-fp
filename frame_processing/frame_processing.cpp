#include "frame_processing.h"

#include "game.h"

void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input, ap_uint<1> &move_cw, ap_uint<1> &move_ccw) {
#pragma HLS INTERFACE axis port=output,input
#pragma HLS PIPELINE
    static Game g;
    g.run(input, output, (move_ccw << 1) | move_cw | move_ccw);
}
