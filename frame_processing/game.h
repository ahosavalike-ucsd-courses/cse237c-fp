#ifndef __GAME__
#define __GAME__
#include "gfx_engine.h"

typedef struct Game {
    GfxEngine g;

    void run(hls::stream<pixel> &output, hls::stream<pixel> &input);
} Game;

#endif
