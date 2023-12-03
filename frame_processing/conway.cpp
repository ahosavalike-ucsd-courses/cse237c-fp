#include "conway.h"

void ConwayGame::reset() {
    done = false;
    if (!random_reg) random_reg = 1;
    for (int i = 0; i < 256; i++) {
        ap_uint<1> bit = ((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) & 1;
        random_reg = (ap_uint<8>(bit) << 7) | (random_reg >> 1);
        world[i] = random_reg & 1;
    }
}

void ConwayGame::update(ap_uint<2> move) {
    static bool updated = false;
    // 5th bit, 16th frame, ~0.5s @30fps
    // Move can be -1, 0, 1 for turn counter clockwise, no change, turn clockwise

    static ap_uint<8> chaos = 0;
    static bool chaos_set = false;
    // Pseudo random number generator. Credit - https://github.com/taylorza/go-lfsr
    random_reg ^= ap_uint<8>(move & 1) << random_reg_index;
    random_reg_index++;
    ap_uint<1> bit = ((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) & 1;
    random_reg = (ap_uint<8>(bit) << 7) | (random_reg >> 1);

    if (move.get_bit(0)) {
        // Flip liveness, mark chaotic
        chaos = random_reg;
        chaos_set = true;
        world[random_reg] = (~world[random_reg]) | 2;
    }
    if (move.get_bit(1)) {
        // Reset world to some random config
        random_reg = g.tick & 255;
        reset();
    }

    if (g.tick) {
        updated = false;
        return;
    } else {
        if (updated) {
            return;
        }
        updated = true;
    }

    ap_uint<3> neighbors[256] = {0};
    for (ap_uint<9> i = 0; i < 256; i++) {
        // Wrap around edges
        ap_uint<4> x = i & 15;
        ap_uint<4> y = (i >> 4) & 15;
        for (int j = -1; j < 2; j++) {
            for (int k = -1; k < 2; k++) {
                neighbors[i] += world[((y + j) << 4) | (x + k)] & 1;
            }
        }
    }
    for (ap_uint<9> i = 0; i < 256; i++) {
        // Apply rules, clear chaos bit
        world[i] = (neighbors[i] + (world[i] & 1)) == 3 || ((neighbors[i] == 3) && (world[i] & 1));
        if (chaos_set && chaos == i) {
            world[i] |= 2;
        }
    }

    chaos_set = false;
}

void ConwayGame::draw_world(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output) {
    int colors[] = {0x0, 0xffffff, 0xff0000, 0x00ff00};  // ideally black, white, red, green
    static hls::stream<streaming_data> temps[255];
    g.draw(RECTANGLE_FILLED, colors[world[0]], Point(0, 0), Point(0, 0), input, temps[0]);
    for (int i = 1; i < 255; i++) {
        ap_uint<8> x = (i & 15);
        ap_uint<8> y = ((i >> 4) & 15);
        g.draw(RECTANGLE_FILLED, colors[world[i]], Point(x, y), Point(x, y), temps[i - 1], temps[i]);
    }
    g.draw(RECTANGLE_FILLED, colors[world[255]], Point(15, 15), Point(15, 15), temps[254], output);
}

void ConwayGame::run(hls::stream<pixel> &input, hls::stream<pixel> &output, ap_uint<2> move) {
    static hls::stream<streaming_data> i("ConwayGame::run.i"), j("ConwayGame::run.j"), k("ConwayGame::run.k");
    g.read(input, i);
    update(move);
    streaming_data c = i.read();
    j.write(c);
    draw_world(j, k);
    g.write(k, output);
}
