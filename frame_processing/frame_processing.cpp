#include "frame_processing.h"

void debounce(bool &move_cw, bool &move_ccw, bool &game_select) {
    static bool cw_pressed = 0;
    static bool ccw_pressed = 0;
    static bool game_selected = 0;
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

    if (!game_select && game_selected) game_selected = 0;
    if (game_select && !game_selected)
        game_selected = 1;
    else
        game_select = 0;
}

void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input, bool move_cw, bool move_ccw, bool game_select) {
#pragma HLS INTERFACE axis port = output, input
#pragma HLS PIPELINE
    static Game g;
    static ConwayGame cg;
    // static TetrisGame tg;
    static GameSelect select = CONWAY;
    debounce(move_cw, move_ccw, game_select);
    if (game_select) {
        select++;
        g.reset();
        cg.reset();
        // tg.reset();
    }
    switch (select) {
        case SNAKE:
        case DUMMY:
            g.run(input, output, (move_ccw << 1) | move_cw | move_ccw);
            break;
        case CONWAY:
            cg.run(input, output, move_ccw << 1 | move_cw);
            break;
        case TETRIS:
            // tg.run(input, output, (move_ccw << 1) | move_cw | move_ccw);
            break;
    }
}
