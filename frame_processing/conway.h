#ifndef __CONWAY__
#define __CONWAY__

#include "gfx_engine.h"

typedef struct ConwayPixel {
    ap_uint<1> live;
    ap_uint<1> chaos;
} ConwayPixel;

typedef struct ConwayGame {
    GfxEngine g;
    ap_uint<64> random_reg;
    ap_uint<6> random_reg_index;
    ConwayPixel world[FRAME_WIDTH][FRAME_HEIGHT];  // 16 * 16
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
