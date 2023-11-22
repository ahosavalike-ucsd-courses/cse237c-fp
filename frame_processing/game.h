#ifndef __GAME__
#define __GAME__
#include "gfx_engine.h"

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

typedef ap_uint<2> Direction;

typedef struct SnakePiece {
    Point p;
    Direction d;
    SnakePiece() {}
    SnakePiece(const SnakePiece &o) : p(o.p), d(o.d) {}
    SnakePiece(Point px, Direction dx) : p(px), d(dx) {}
    SnakePiece(Point px, int dx) : p(px), d(dx) {}
} SnakePiece;

typedef struct Game {
	GfxEngine g;
    Point food;
    ap_uint<11> score;
    SnakePiece snake[SNAKE_LEN];
    bool done;
    Game() {
        reset();
    }
    void reset();
    void update(ap_int<2> move);
    void draw(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output);
    void draw_head(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output);
    void draw_body(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output);
    void draw_food(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output);
    void run(hls::stream<pixel> &input, hls::stream<pixel> &output);
} Game;

#endif
