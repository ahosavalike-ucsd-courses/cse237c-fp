#ifndef __CONWAY__
#define __CONWAY__

#include "gfx_engine.h"

typedef struct ConwayGame {
    GfxEngine g;
    ap_uint<11> score;
    ap_uint<8> random_reg;
    ap_uint<3> random_reg_index;
    ap_uint<2> world[256];  // 16 * 16
    bool done;
    ConwayGame() {
        reset();
    }
    void reset();
    void update(ap_uint<2> move);
    void draw_world(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output);
    void run(hls::stream<pixel> &input, hls::stream<pixel> &output, ap_uint<2> move);
} ConwayGame;

#endif
