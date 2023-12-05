#ifndef __CONWAY__
#define __CONWAY__

#include "gfx_engine.h"

#define CONWAY_FACTOR 8
#define CONWAY_FACTOR_WIDTH 3
#define CONWAY_FRAME_WIDTH (FRAME_WIDTH / CONWAY_FACTOR)
#define CONWAY_FRAME_HEIGHT (FRAME_HEIGHT / CONWAY_FACTOR)
#define CONWAY_BIT_WIDTH (7 - CONWAY_FACTOR_WIDTH)
#define RANDOM_BIT 6

typedef struct ConwayPixel {
    ap_uint<1> live;
    ap_uint<1> chaos;
} ConwayPixel;

typedef struct ConwayGame {
    GfxEngine g;
    ap_uint<1 << RANDOM_BIT> random_reg;
    ap_uint<RANDOM_BIT> random_reg_index;
    ConwayPixel world[CONWAY_FRAME_WIDTH][CONWAY_FRAME_HEIGHT];  // 16 * 16
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
