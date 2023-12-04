#include "conway.h"

void ConwayGame::reset() {
    done = false;
    if (!random_reg) random_reg = 1;
    for (int i = 0; i < FRAME_WIDTH; i++) {
        for (int j = 0; j < FRAME_HEIGHT; j++) {
            random_reg = (((random_reg >> 0) ^ (random_reg >> 1) ^ (random_reg >> 3) ^ (random_reg >> 4)) << 63) | (random_reg >> 1);
            world[i][j].live = random_reg & 1;
            world[i][j].chaos = 0;
        }
    }
}

void ConwayGame::update(ap_uint<2> move) {
    static bool updated = false;
    // 5th bit, 16th frame, ~0.5s @30fps
    // Move can be -1, 0, 1 for turn counter clockwise, no change, turn clockwise

    static ap_uint<64> chaos = 0;
    static bool chaos_set = false;
    // Pseudo random number generator. Credit - https://github.com/taylorza/go-lfsr
    random_reg ^= ap_uint<64>(move & 1) << random_reg_index;
    random_reg_index++;
    random_reg = (((random_reg >> 0) ^ (random_reg >> 1) ^ (random_reg >> 3) ^ (random_reg >> 4)) << 63) | (random_reg >> 1);

    if (move.get_bit(0)) {
        // Flip liveness, mark chaotic
        chaos = random_reg;
        chaos_set = true;
        ap_uint<7> y = random_reg.range(14, 7);
        world[random_reg.range(7, 0)][y > FRAME_HEIGHT ? ap_uint<7>(FRAME_HEIGHT - y) : y].chaos = 1;
    }
    if (move.get_bit(1)) {
        // Reset world to some random config
        random_reg = g.tick & 255;
        reset();
    }

    if (!g.tick) {
        updated = false;
        return;
    } else {
        if (updated) {
            return;
        }
        updated = true;
    }

    ap_uint<4> neighbors[FRAME_WIDTH][FRAME_HEIGHT] = {0};
    for (ap_uint<8> i = 0; i < FRAME_WIDTH; i++) {
        for (ap_uint<7> j = 0; j < FRAME_HEIGHT; j++) {
            // Wrap around edges
            for (int k = -1; k < 2; k++) {
                for (int l = -1; l < 2; l++) {
                    neighbors[i][j] += world[i + k][j + l].live;
                }
            }
        }
    }
    for (ap_uint<8> i = 0; i < FRAME_WIDTH; i++) {
        for (ap_uint<7> j = 0; j < FRAME_HEIGHT; j++) {
            world[i][j].chaos = 0;
            world[i][j].live = (neighbors[i][j] + (world[i][j].live & 1)) == 3 || ((neighbors[i][j] == 3) && (world[i][j].live & 1));
            // Apply rules, clear chaos bit
            ap_uint<7> y = chaos.range(14, 7);
            if (chaos_set && (y >= FRAME_HEIGHT ? ap_uint<7>(ap_uint<7>(FRAME_HEIGHT) - y) : y) == j && chaos.range(7, 0) == i) {
                world[i][j].live ^= 1;
                world[i][j].chaos = 1;
            }
        }
    }

    chaos_set = false;
}

void ConwayGame::draw_world(hls::stream<streaming_data>& input, hls::stream<streaming_data>& output) {
    ap_uint<24> colors[2][2] = {{0x0, 0xff0000}, {0xffffff, 0x00ff00}};  // ideally black, red, white, green
    static hls::stream<streaming_data> temps[FRAME_WIDTH * FRAME_HEIGHT - 1];
    int counter = 0;
    g.draw(POINT, colors[world[0][0].live][world[0][0].chaos], Point(0, 0), Point(0, 0), input, temps[0]);
    for (ap_uint<8> i = 0; i < FRAME_WIDTH; i++) {
        for (ap_uint<7> j = 0; j < FRAME_HEIGHT; j++) {
            if (i == 0 && j == 0) continue;
            if (i == (FRAME_WIDTH - 1) && j == (FRAME_HEIGHT - 1)) break;
            g.draw(POINT, colors[world[i][j].live][world[i][j].chaos], Point(i, j), Point(i, j), temps[counter], temps[counter + 1]);
            counter++;
        }
    }
    ap_uint<7> x = FRAME_WIDTH - 1, y = FRAME_HEIGHT - 1;
    g.draw(POINT, colors[world[x][y].live][world[x][y].chaos], Point(x, y), Point(x, y), temps[counter], output);
}

void ConwayGame::run(hls::stream<pixel>& input, hls::stream<pixel>& output, ap_uint<2> move) {
    static hls::stream<streaming_data> i("ConwayGame::run.i"), j("ConwayGame::run.j"), k("ConwayGame::run.k");
    g.read(input, i);
    update(move);
    streaming_data c = i.read();
    j.write(c);
    draw_world(j, k);
    g.write(k, output);
}
