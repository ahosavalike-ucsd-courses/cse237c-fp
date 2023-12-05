#include "tetris.h"

#define TETRIS_I 1
#define TETRIS_T 2
#define TETRIS_J 3
#define TETRIS_L 4
#define TETRIS_O 5
#define TETRIS_S 6
#define TETRIS_Z 7

void printout(ap_uint<3> w[256]) {
    for (int i = 0; i < 256; i++) dbg("%d ", w[i]);
    dbg("\n");
}

void TetrisGame::reset() {
    done = false;
    if (!random_reg) random_reg = 1;
    for (int i = 0; i < 256; i++) {
#pragma HLS UNROLL
        world[i] = 0;
    }
    ap_uint<1> bit = ((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) & 1;
    random_reg = (bit << 7) | (random_reg >> 1);
    active.type = (random_reg % 7) + 1;
    bit = ((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) & 1;
    random_reg = (bit << 7) | (random_reg >> 1);
    active.x = random_reg & 15;
    active.y = 2;
    bit = ((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) & 1;
    random_reg = (bit << 7) | (random_reg >> 1);
    active.direction = random_reg % 4;
    for (int i = 0; i < 16; i++) {
#pragma HLS UNROLL
        filled_counts[i] = 0;
    }
}

void TetrisGame::get_subsquare(ap_uint<4> &x, ap_uint<4> &y, ap_uint<2> direction, ap_uint<2> id) {
    ap_int<2> subx, suby;
    if (active.type == TETRIS_I) {
        if (id == 0) {
            subx = 0;
            suby = 1;
        }
        if (id == 1) {
            subx = 0;
            suby = 0;
        }
        if (id == 2) {
            subx = 0;
            suby = -1;
        }
        if (id == 3) {
            subx = 0;
            suby = -2;
        }
    }
    if (active.type == TETRIS_T) {
        if (id == 0) {
            subx = -1;
            suby = 0;
        }
        if (id == 1) {
            subx = 0;
            suby = 0;
        }
        if (id == 2) {
            subx = 1;
            suby = 0;
        }
        if (id == 3) {
            subx = 0;
            suby = -1;
        }
    }
    if (active.type == TETRIS_J) {
        if (id == 0) {
            subx = -1;
            suby = 0;
        }
        if (id == 1) {
            subx = 0;
            suby = 0;
        }
        if (id == 2) {
            subx = 0;
            suby = -1;
        }
        if (id == 3) {
            subx = 0;
            suby = -2;
        }
    }
    if (active.type == TETRIS_L) {
        if (id == 0) {
            subx = 1;
            suby = 0;
        }
        if (id == 1) {
            subx = 0;
            suby = 0;
        }
        if (id == 2) {
            subx = 0;
            suby = -1;
        }
        if (id == 3) {
            subx = 0;
            suby = -2;
        }
    }
    if (active.type == TETRIS_O) {
        if (id == 0) {
            subx = -1;
            suby = 0;
        }
        if (id == 1) {
            subx = 0;
            suby = 0;
        }
        if (id == 2) {
            subx = 0;
            suby = -1;
        }
        if (id == 3) {
            subx = -1;
            suby = -1;
        }
    }
    if (active.type == TETRIS_S) {
        if (id == 0) {
            subx = -1;
            suby = 0;
        }
        if (id == 1) {
            subx = 0;
            suby = 0;
        }
        if (id == 2) {
            subx = 0;
            suby = -1;
        }
        if (id == 3) {
            subx = 1;
            suby = -1;
        }
    }
    if (active.type == TETRIS_Z) {
        if (id == 0) {
            subx = 1;
            suby = 0;
        }
        if (id == 1) {
            subx = 0;
            suby = 0;
        }
        if (id == 2) {
            subx = 0;
            suby = -1;
        }
        if (id == 3) {
            subx = -1;
            suby = -1;
        }
    }
    if (direction == 0) {
        x = active.x + subx;
        y = active.y + suby;
    }
    if (direction == 1) {
        x = active.x - suby;
        y = active.y + subx;
    }
    if (direction == 2) {
        x = active.x - subx;
        y = active.y - suby;
    }
    if (direction == 3) {
        x = active.x + suby;
        y = active.y - subx;
    }
}

void TetrisGame::rotate(ap_uint<2> direction) {
    // Direction can be -1, 0, 1 for turn counter clockwise, no change, turn clockwise
    ap_uint<2> new_direction = direction + active.direction;
    ap_uint<1> valid = 1;
    for (int i = 0; i < 4; i++) {
#pragma HLS UNROLL
        ap_uint<4> x, y;
        get_subsquare(x, y, active.direction, i);
        world[(ap_uint<8>(y) << 4) | (x)] = 0;
    }
    for (int i = 0; i < 4; i++) {
#pragma HLS UNROLL
        ap_uint<4> x, y;
        get_subsquare(x, y, new_direction, i);
        if (world[(ap_uint<8>(y) << 4) | (x)] != 0) valid = 0;
    }
    if (valid) {
        active.direction = new_direction;
    }
    for (int i = 0; i < 4; i++) {
#pragma HLS UNROLL
        ap_uint<4> x, y;
        get_subsquare(x, y, active.direction, i);
        world[(ap_uint<8>(y) << 4) | (x)] = active.type;
    }
}

void TetrisGame::move(ap_int<2> direction) {
    // Direction can be -1, 0, 1 for turn left, no change, right
    ap_uint<1> valid = 1;
    if (direction)
        for (int i = 0; i < 4; i++) {
#pragma HLS UNROLL
            ap_uint<4> x, y;
            get_subsquare(x, y, active.direction, i);
            world[(ap_uint<8>(y) << 4) | (x)] = 0;
        }
    for (int i = 0; i < 4; i++) {
#pragma HLS UNROLL
        ap_uint<4> x, y;
        get_subsquare(x, y, active.direction, i);
        x += direction;
        if (world[(ap_uint<8>(y) << 4) | (x)] != 0) valid = 0;
    }
    if (valid) active.x += direction;
    for (int i = 0; i < 4; i++) {
#pragma HLS UNROLL
        ap_uint<4> x, y;
        get_subsquare(x, y, active.direction, i);
        world[(ap_uint<8>(y) << 4) | (x)] = active.type;
    }
}

void TetrisGame::update(ap_int<2> move, ap_int<2> turn) {
    // 5th bit, 16th frame, ~0.5s @30fps
    // turn can be -1, 0, 1 for turn counter clockwise, no change, turn clockwise
    // move can be -1, 0, 1 for move left, no change, right

    static bool updated = false;

    // Pseudo random number generator. Credit - https://github.com/taylorza/go-lfsr
    random_reg ^= ap_uint<1 << RANDOM_BIT>(move & 1) << random_reg_index;
    random_reg_index++;
    random_reg = (((random_reg >> 0) ^ (random_reg >> 1) ^ (random_reg >> 3) ^ (random_reg >> 4)) << ((1 << RANDOM_BIT) - 1)) | (random_reg >> 1);

    rotate(turn);
    this->move(move);

    if (!g.tick) {
        updated = false;
        return;
    } else {
        if (updated) {
            return;
        }
        updated = true;
    }

    // dbg("x,y: %d, %d\n", active.x, active.y);
    ap_uint<1> valid = 1;
    for (int i = 0; i < 4; i++) {
#pragma HLS UNROLL
        ap_uint<4> x, y;
        get_subsquare(x, y, active.direction, i);
        world[(ap_uint<8>(y) << 4) | (x)] = 0;
    }
    for (int i = 0; i < 4; i++) {
#pragma HLS UNROLL
        ap_uint<4> x, y;
        get_subsquare(x, y, active.direction, i);
        y += 1;
        // dbg("x,y: %d, %d\n", x, y);
        // printout(world);
        if (world[(ap_uint<8>(y) << 4) | (x)] != 0 || y == 15) valid = 0;
    }
    if (valid) active.y += 1;
    for (int i = 0; i < 4; i++) {
#pragma HLS UNROLL
        ap_uint<4> x, y;
        get_subsquare(x, y, active.direction, i);
        world[(ap_uint<8>(y) << 4) | (x)] = active.type;
        if (!valid) filled_counts[y]++;
    }

    if (!valid) {
        // Reached top
        if (active.y == 2) {
            done = true;
            reset();
        }
    }

    if (!valid) {
        // New active block
        ap_uint<1> bit = ((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) & 1;
        random_reg = (bit << 7) | (random_reg >> 1);
        active.type = (random_reg % 7) + 1;
        bit = ((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) & 1;
        random_reg = (bit << 7) | (random_reg >> 1);
        active.x = random_reg & 15;
        active.y = 2;
        bit = ((random_reg >> 0) ^ (random_reg >> 2) ^ (random_reg >> 3) ^ (random_reg >> 4)) & 1;
        random_reg = (bit << 7) | (random_reg >> 1);
        active.direction = random_reg % 4;
    }

    if (!valid) {
        // Clear full lines
        // for (int i = 15; i >= 0; i--) {
        if (filled_counts[0] == 16) {
            for (int j = 0; j >= 2; j--) {
                for (int k = 0; k < 16; k++) {
#pragma HLS UNROLL
                    world[(j << 4) | k] = world[((j - 1) << 4) | k];
                }
                filled_counts[0] = filled_counts[1];
            }
        }
        // }
    }
}

void TetrisGame::draw_world(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output) {
    int colors[] = {0x0, 0xffffff, 0xff0000, 0x00ff00, 0x0000ff, 0x00ffff, 0xff00ff, 0xffff00};
    streaming_data in = input.read();
    if (in.loc.x < 64 && in.loc.y < 64)
        in.color = colors[world[((in.loc.y & 0x3c) << 2) | (in.loc.x >> 2)]];
    output << in;
}

void TetrisGame::run(hls::stream<pixel> &input, hls::stream<pixel> &output, ap_uint<2> move) {
    // #pragma HLS ARRAY_PARTITION type = complete variable = world
    static hls::stream<streaming_data> i("TetrisGame::run.i"), j("TetrisGame::run.j"), k("TetrisGame::run.k");
    g.read(input, i);
    update(move.get_bit(0), move.get_bit(1));
    streaming_data c = i.read();
    j.write(c);
    draw_world(j, k);
    g.write(k, output);
}
