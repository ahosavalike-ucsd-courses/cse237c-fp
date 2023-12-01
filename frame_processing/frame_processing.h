#include "consts_types.h"

void debounce(bool &move_cw, bool &move_ccw);
void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input, bool move_cw, bool move_ccw);
