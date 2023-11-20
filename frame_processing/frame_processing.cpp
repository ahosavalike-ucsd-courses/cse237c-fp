#include "frame_processing.h"

#include "game.h"

void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input) {
#pragma HLS INTERFACE axis port = output, input
#pragma HLS PIPELINE II = 1
    static Game g;
    g.done = true;
    g.run(output, input);
}
