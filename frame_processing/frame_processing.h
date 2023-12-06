#include "consts_types.h"
#include "conway.h"
#include "game.h"
#include "tetris.h"

#define SNAKE 0
#define CONWAY 1
#define DUMMY 2
#define TETRIS 3

typedef ap_uint<2> GameSelect;

void debounce(bool &move_cw, bool &move_ccw);
void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input, bool move_cw, bool move_ccw, bool rotate, bool game_select);
