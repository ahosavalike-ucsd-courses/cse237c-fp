#include "conway.h"

void ConwayGame::reset() {
    done = false;
    for (int i = 0; i < 256; i++) {
        ap_uint<1> bit = ((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) & 1;
        random_reg = (bit << 7) | (random_reg >> 1);
        world[i] = random_reg & 1;
    }
}

void ConwayGame::update(ap_int<2> move) {
    // 5th bit, 16th frame, ~0.5s @30fps
    // Move can be -1, 0, 1 for turn counter clockwise, no change, turn clockwise

    // Pseudo random number generator. Credit - https://github.com/taylorza/go-lfsr
    random_reg ^= (move & 1) << random_reg_index;
    random_reg_index++;
    ap_uint<1> bit = ((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) & 1;
    random_reg = (bit << 7) | (random_reg >> 1);

    ap_uint<3> neighbors[256] = {0};
#pragma HLS ARRAY_PARTITION type = complete variable = world
#pragma HLS ARRAY_PARTITION type = complete variable = neighbors
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
        world[i] = ((neighbors[i] + (world[i] & 1)) == 3 || ((neighbors[i] == 3) && (world[i] & 1)));
    }

    // if button pressed, introduce random chaos
    // world value is 2 bits - lsb is state (live/dead), msb is special visual marker for chaos
    if (move == 1) {
        // Flip liveness, mark chaotic
        world[random_reg] = (~world[random_reg]) | 2;
    } else if (move == -1) {
        // Reset world to some random config
        random_reg = g.tick & 255;
        reset();
    }
}

void ConwayGame::draw_world(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output) {
    int colors[] = {0x0, 0xffffff, 0xff0000, 0x00ff00};  // ideally black, white, red, green
    static hls::stream<streaming_data> temps[257];
    g.draw(RECTANGLE_FILLED, 0x000000, Point(0, 0), Point(0, 0), input, temps[0]);
    for (int i = 0; i < 256; i++) {
        ap_uint<8> x = (i & 15) * 10;
        ap_uint<8> y = ((i >> 4) & 15) * 10;
        g.draw(RECTANGLE_FILLED, colors[world[i]], Point(x, y), Point(x + 10, y + 10), temps[i], temps[i + 1]);
    }
    g.draw(RECTANGLE_FILLED, 0x000000, Point(0, 0), Point(0, 0), temps[256], output);
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
