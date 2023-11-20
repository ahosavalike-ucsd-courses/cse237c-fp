#include "gfx_engine.h"

struct GfxEngine &GfxEngine::read(hls::stream<pixel> &input) {
    input >> p;
    if (p.user) {
        current_location.x = current_location.y = 0;
        tick++;  // Will overflow, only track till meaningful
    }
    return *this;
}

struct GfxEngine &GfxEngine::draw(Shape s, rgb color, Point i, Point j) {
    switch (s) {
        case LINE:
            if (current_location.x >= i.x && current_location.x <= j.x && current_location.y >= i.y && current_location.y <= j.y && (current_location.y - i.y) * (j.x - i.x) == (current_location.x - i.x) * (j.y - i.y))
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

#define FONTX 10
#define FONTY 16
struct GfxEngine &GfxEngine::draw_num(ap_uint<4> n, rgb color, Point i) {
    switch (n) {
        case 0:
            draw(RECTANGLE, color, i, i + Point(FONTX, FONTY));
            break;
        case 1:
            draw(LINE, color, i + Point(FONTX, 0), i + Point(FONTX, FONTY));
            break;
        case 2:
            draw(LINE, color, i, i + Point(FONTX, 0));
            draw(LINE, color, i + Point(0, FONTY / 2), i + Point(FONTX, FONTY / 2));
            draw(LINE, color, i + Point(0, FONTY), i + Point(FONTX, FONTY));
            draw(LINE, color, i + Point(FONTX, 0), i + Point(FONTX, FONTY / 2));
            draw(LINE, color, i + Point(0, FONTY / 2), i + Point(0, FONTY));
            break;
        case 3:
            draw(LINE, color, i, i + Point(FONTX, 0));
            draw(LINE, color, i + Point(0, FONTY / 2), i + Point(FONTX, FONTY / 2));
            draw(LINE, color, i + Point(0, FONTY), i + Point(FONTX, FONTY));
            draw(LINE, color, i + Point(FONTX, 0), i + Point(FONTX, FONTY));
            break;
        case 4:
            draw(LINE, color, i, i + Point(0, FONTY / 2));
            draw(LINE, color, i + Point(0, FONTY / 2), i + Point(FONTX, FONTY / 2));
            draw(LINE, color, i + Point(FONTX, 0), i + Point(FONTX, FONTY));
            break;
        case 5:
            draw(LINE, color, i, i + Point(FONTX, 0));
            draw(LINE, color, i + Point(0, FONTY / 2), i + Point(FONTX, FONTY / 2));
            draw(LINE, color, i + Point(0, FONTY), i + Point(FONTX, FONTY));
            draw(LINE, color, i, i + Point(0, FONTY / 2));
            draw(LINE, color, i + Point(FONTX, FONTY / 2), i + Point(FONTX, FONTY));
            break;
        case 6:
            draw(LINE, color, i, i + Point(FONTX, 0));
            draw(LINE, color, i, i + Point(0, FONTY / 2));
            draw(RECTANGLE, color, i + Point(0, FONTY / 2), i + Point(FONTX, FONTY));
            break;
        case 7:
            draw(LINE, color, i, i + Point(FONTX, 0));
            draw(LINE, color, i + Point(FONTX, 0), i + Point(FONTX, FONTY));
            break;
        case 8:
            draw(LINE, color, i + Point(0, FONTY / 2), i + Point(FONTX, FONTY / 2));
            draw(RECTANGLE, color, i, i + Point(FONTX, FONTY));
            break;
        case 9:
            draw(RECTANGLE, color, i, i + Point(FONTX, FONTY / 2));
            draw(LINE, color, i + Point(FONTX, FONTY / 2), i + Point(FONTX, FONTY));
            draw(LINE, color, i + Point(0, FONTY), i + Point(FONTX, FONTY));
            break;
        case 0xA:
            break;
        case 0xB:
            break;
        case 0xC:
            break;
        case 0xD:
            break;
        case 0xE:
            break;
        case 0xF:
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
