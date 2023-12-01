#include "consts_types.h"

void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input, ap_uint<1> &move_cw, ap_uint<1> &move_ccw);
