#ifndef __TETRIS__
#define __TETRIS__
#include "gfx_engine.h"

#define RANDOM_BIT 6

typedef struct TetrisBlock {
    ap_uint<4> x, y;
    ap_uint<2> direction;  // 0 up, 1 right, 2 down, 3 left
    ap_uint<3> type;       // defined under TETRIS_ hash defs
} TetrisBlock;

typedef struct TetrisGame {
    GfxEngine g;
    ap_uint<11> score;
    ap_uint<1 << RANDOM_BIT> random_reg;
    ap_uint<RANDOM_BIT> random_reg_index;
    ap_uint<3> world[256];  // 16 * 16
    ap_uint<5> filled_counts[16];
    TetrisBlock active;
    bool done;
    TetrisGame() {
        reset();
    }
    void reset();
    void update(ap_int<2> move, ap_int<2> turn);
    void draw_world(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output);
    void get_subsquare(ap_uint<4> &x, ap_uint<4> &y, ap_uint<2> direction, ap_uint<2> id);
    void rotate(ap_uint<2> turn);
    void move(ap_int<2> direction);
    void run(hls::stream<pixel> &input, hls::stream<pixel> &output, ap_uint<2> move);
} TetrisGame;

#endif
