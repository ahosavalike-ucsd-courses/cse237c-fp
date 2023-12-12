#include "conway.h"

void printout(ConwayPixel w[CONWAY_FRAME_WIDTH][CONWAY_FRAME_HEIGHT]) {
    for (int i = 0; i < CONWAY_FRAME_WIDTH; i++) {
        for (int j = 0; j < CONWAY_FRAME_HEIGHT; j++)
            dbg("%d.%d ", w[i][j].live, w[i][j].chaos);
        dbg("\n");
    }
}

void ConwayGame::reset() {
    done = false;
    if (!random_reg) random_reg = 1;
    for (int i = 0; i < CONWAY_FRAME_WIDTH; i++) {
        for (int j = 0; j < CONWAY_FRAME_HEIGHT; j++) {
            random_reg = (((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) << 7) | (random_reg >> 1);
            world[i][j].live = random_reg & 1;
            world[i][j].chaos = 0;
        }
    }
}

void ConwayGame::update(ap_uint<2> move) {
    static bool updated = false;
    // 5th bit, 16th frame, ~0.5s @30fps
    // Move can be -1, 0, 1 for turn counter clockwise, no change, turn clockwise

    static ap_uint<1 << RANDOM_BIT> chaos = 0;
    static bool chaos_set = false;
    // Pseudo random number generator. Credit - https://github.com/taylorza/go-lfsr
    random_reg ^= ap_uint<1 << RANDOM_BIT>(move & 1) << random_reg_index;
    random_reg_index++;
    random_reg = (((random_reg >> 0) ^ (random_reg >> 1) ^ (random_reg >> 3) ^ (random_reg >> 4)) << ((1 << RANDOM_BIT) - 1)) | (random_reg >> 1);

    if (move.get_bit(0)) {
        // Flip liveness, mark chaotic
        chaos = random_reg;
        chaos_set = true;
        ap_uint<CONWAY_BIT_WIDTH> y = random_reg.range(CONWAY_BIT_WIDTH * 2 - 1, CONWAY_BIT_WIDTH);
        world[random_reg.range(CONWAY_BIT_WIDTH - 1, 0)][y > CONWAY_FRAME_HEIGHT ? ap_uint<CONWAY_BIT_WIDTH>(CONWAY_FRAME_HEIGHT - y) : y].chaos = 1;
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

    ap_uint<4> neighbors[CONWAY_FRAME_WIDTH][CONWAY_FRAME_HEIGHT];
#pragma HLS ARRAY_PARTITION type = complete variable = neighbors
    for (ap_uint<CONWAY_BIT_WIDTH + 1> j = 0; j < CONWAY_FRAME_HEIGHT; j++) {
#pragma HLS UNROLL
        for (ap_uint<CONWAY_BIT_WIDTH + 1> i = 0; i < CONWAY_FRAME_WIDTH; i++) {
#pragma HLS UNROLL
            // Dont wrap around edges
            ap_uint<4> val = 0;
            for (ap_int<4> k = -1; k < 2; k++) {
#pragma HLS UNROLL
                for (ap_int<4> l = -1; l < 2; l++) {
#pragma HLS UNROLL
                    ap_uint<CONWAY_BIT_WIDTH + 1> x = i + k, y = j + l;
                    if (x >= CONWAY_FRAME_WIDTH || y >= CONWAY_FRAME_HEIGHT) continue;
                    val += world[x][y].live;
                }
            }
            neighbors[i][j] = val;
        }
    }
    for (ap_uint<CONWAY_BIT_WIDTH + 1> j = 0; j < CONWAY_FRAME_HEIGHT; j++) {
#pragma HLS UNROLL
        for (ap_uint<CONWAY_BIT_WIDTH + 1> i = 0; i < CONWAY_FRAME_WIDTH; i++) {
#pragma HLS UNROLL
            world[i][j].chaos = 0;
            world[i][j].live = neighbors[i][j] == 3 || (world[i][j].live && neighbors[i][j] == 4);
        }
    }
    // Apply rules, clear chaos bit
    ap_uint<CONWAY_BIT_WIDTH> y = chaos.range(CONWAY_BIT_WIDTH * 2 - 1, CONWAY_BIT_WIDTH);
    y = (y >= CONWAY_FRAME_HEIGHT ? ap_uint<CONWAY_BIT_WIDTH>(ap_uint<CONWAY_BIT_WIDTH>(CONWAY_FRAME_HEIGHT) - y) : y);
    if (chaos_set) {
        world[chaos.range(CONWAY_BIT_WIDTH - 1, 0)][y].live ^= 1;
        world[chaos.range(CONWAY_BIT_WIDTH - 1, 0)][y].chaos = 1;
    }

    chaos_set = false;
}

void ConwayGame::draw_world(hls::stream<streaming_data>& input, hls::stream<streaming_data>& output) {
    ap_uint<24> colors[2][2] = {{0x0, 0xff0000}, {0xffffff, 0x00ff00}};  // ideally black, red, white, green
#pragma HLS ARRAY_PARTITION type = complete variable = colors
    streaming_data in = input.read();
    in.color = colors[world[in.loc.x >> CONWAY_FACTOR_WIDTH][in.loc.y >> CONWAY_FACTOR_WIDTH].live][world[in.loc.x >> CONWAY_FACTOR_WIDTH][in.loc.y >> CONWAY_FACTOR_WIDTH].chaos];
    output << in;
}

void ConwayGame::run(hls::stream<pixel>& input, hls::stream<pixel>& output, ap_uint<2> move) {
#pragma HLS ARRAY_PARTITION type = complete variable = world
    static hls::stream<streaming_data> i("ConwayGame::run.i"), j("ConwayGame::run.j"), k("ConwayGame::run.k");
    g.read(input, i);
    update(move);
    streaming_data c = i.read();
    j.write(c);
    draw_world(j, k);
    g.write(k, output);
}
