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
#pragma HLS UNROLL
        for (int j = 0; j < CONWAY_FRAME_HEIGHT; j++) {
#pragma HLS UNROLL
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
        ap_uint<CONWAY_BIT_WIDTH> y = random_reg.range(CONWAY_BIT_WIDTH * 2 - 2, CONWAY_BIT_WIDTH);
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

    ap_uint<4> neighbors[CONWAY_FRAME_WIDTH][CONWAY_FRAME_HEIGHT] = {0};
#pragma HLS ARRAY_PARTITION type = complete variable = neighbors
    for (ap_uint<CONWAY_BIT_WIDTH + 1> i = 0; i < CONWAY_FRAME_WIDTH; i++) {
#pragma HLS UNROLL
        for (ap_uint<CONWAY_BIT_WIDTH> j = 0; j < CONWAY_FRAME_HEIGHT; j++) {
#pragma HLS UNROLL
            // Wrap around edges
            for (ap_int<3> k = -1; k < 2; k++) {
                for (ap_int<3> l = -1; l < 2; l++) {
                    neighbors[i][j] += world[i + k][j + l].live;
                }
            }
        }
    }
    for (ap_uint<CONWAY_BIT_WIDTH + 1> i = 0; i < CONWAY_FRAME_WIDTH; i++) {
#pragma HLS UNROLL
        for (ap_uint<CONWAY_BIT_WIDTH> j = 0; j < CONWAY_FRAME_HEIGHT; j++) {
#pragma HLS UNROLL
            world[i][j].chaos = 0;
            world[i][j].live = (neighbors[i][j] + (world[i][j].live & 1)) == 3 || ((neighbors[i][j] == 3) && (world[i][j].live & 1));
        }
    }
    // Apply rules, clear chaos bit
    ap_uint<CONWAY_BIT_WIDTH> y = chaos.range(CONWAY_BIT_WIDTH * 2 - 2, CONWAY_BIT_WIDTH);
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
    static hls::stream<streaming_data> temps[CONWAY_FRAME_WIDTH * CONWAY_FRAME_HEIGHT - 1];
    int counter = 0;
    g.draw(RECTANGLE_FILLED, colors[world[0][0].live][world[0][0].chaos], Point(0, 0), Point(0, 0) + CONWAY_FACTOR, input, temps[0]);
    for (ap_uint<CONWAY_BIT_WIDTH + 1> i = 0; i < CONWAY_FRAME_WIDTH; i++) {
#pragma HLS UNROLL
        for (ap_uint<CONWAY_BIT_WIDTH> j = 0; j < CONWAY_FRAME_HEIGHT; j++) {
#pragma HLS UNROLL
            if (i == 0 && j == 0) continue;
            if (i == (CONWAY_FRAME_WIDTH - 1) && j == (CONWAY_FRAME_HEIGHT - 1)) break;
            // dbg("%d %d pixel %x\n", (Point(i, j) * CONWAY_FACTOR).x, (Point(i, j) * CONWAY_FACTOR).y, colors[world[i][j].live][world[i][j].chaos]);
            g.draw(RECTANGLE_FILLED, colors[world[i][j].live][world[i][j].chaos], Point(i, j) * CONWAY_FACTOR, Point(i, j) * CONWAY_FACTOR + CONWAY_FACTOR, temps[counter], temps[counter + 1]);
            counter++;
        }
    }
    ap_uint<CONWAY_BIT_WIDTH> x = CONWAY_FRAME_WIDTH - 1, y = CONWAY_FRAME_HEIGHT - 1;
    g.draw(RECTANGLE_FILLED, colors[world[x][y].live][world[x][y].chaos], Point(x, y) * CONWAY_FACTOR, Point(x, y) * CONWAY_FACTOR + CONWAY_FACTOR, temps[counter], output);
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
