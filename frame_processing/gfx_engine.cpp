#include "gfx_engine.h"

struct GfxEngine &GfxEngine::read(hls::stream<pixel> &input) {
    input >> p;
    if (p.user) {
        current_location.x = current_location.y = 0;
        fc++;  // Will overflow, only track till meaningful
        if (fc > FRAME_HEIGHT)
            fc = 1;
    }
    return *this;
}

struct GfxEngine &GfxEngine::draw_diag() {
    if (current_location.x == current_location.y)
        if (current_location.x < fc)
            p.data = 0x0000ff;
        else if (current_location.x > fc)
            p.data = 0xff0000;
        else
            p.data = 0xffffff;
    return *this;
}

struct GfxEngine &GfxEngine::draw(Shape s, rgb color, Point i, Point j) {
    switch (s) {
        case LINE:
            if ((current_location.y - i.y) * (j.x - i.x) == (current_location.x - i.x) * (j.y - i.y))
                p.data = color;
            break;
        case RECTANGLE:
            if (
                (current_location.x >= i.x && current_location.x <= j.x && (current_location.y == i.y || current_location.y == j.y)) ||
                (current_location.y >= i.y && current_location.y <= j.y && (current_location.x == i.x || current_location.x == j.x)))
                p.data = color;
            break;
        case RECTANGLE_FILLED:
            if (current_location.x >= i.x && current_location.x <= j.x && current_location.y >= i.y && current_location.y <= j.y)
                p.data = color;
            break;
    }
    return *this;
}

void GfxEngine::write(hls::stream<pixel> &output) {
    current_location.x++;
    if (p.last) {
        current_location.y++;
        current_location.x = 0;
    }
    output << p;
}
