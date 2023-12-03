#include "game.h"

/*
```py
from random import sample
from itertools import product
SNAKE_LEN = 10
SNAKE_SIZE = 4
FRAME_HEIGHT = 72
FRAME_WIDTH = 128

SNAKE_HEIGHT = int(FRAME_HEIGHT / SNAKE_SIZE)
SNAKE_WIDTH = int(FRAME_WIDTH / SNAKE_SIZE)

valid_food = [(i * SNAKE_SIZE + int(SNAKE_SIZE/2), j * SNAKE_SIZE + int(SNAKE_SIZE/2)) for (i, j) in product(range(SNAKE_WIDTH), range(SNAKE_HEIGHT))]
for (i,j) in sample(valid_food, SNAKE_LEN):
    print(f"Point({i}, {j}),")
```
*/
Point food_loc[SNAKE_LEN] = {
    Point(114, 50),
    Point(90, 58),
    Point(10, 58),
    Point(62, 34),
    Point(54, 30),
    Point(102, 26),
    Point(126, 14),
    Point(102, 50),
    Point(110, 46),
    Point(46, 66),
};

void Game::reset() {
#pragma HLS PIPELINE
reset_snake:
    for (ap_uint<7> i = 0; i < SNAKE_LEN; i++)
#pragma HLS UNROLL
        snake[i] = SnakePiece(Point(-1, -1), RIGHT);
    snake[0] = SnakePiece(Point(0, 0), RIGHT);
    score = 0;
    food = food_loc[0];
    done = false;
}

void Game::update(ap_int<2> move) {
    static bool updated = false;
    SnakePiece new_head = snake[0];
    // Move can be -1, 0, 1 for turn counter clockwise, no change, turn clockwise
    new_head.d += move;

    if (!g.tick) {
        updated = false;
        snake[0] = new_head;
        return;
    } else {
        if (updated) {
            snake[0] = new_head;
            return;
        }
        updated = true;
    }

    switch (new_head.d) {
        case LEFT:
            new_head.p.x -= SNAKE_SIZE;
            if (new_head.p.x.get_bit(10))
                new_head.p.x = FRAME_WIDTH - SNAKE_SIZE;
            break;
        case RIGHT:
            new_head.p.x += SNAKE_SIZE;
            if (new_head.p.x == FRAME_WIDTH)
                new_head.p.x = 0;
            break;
        case UP:
            new_head.p.y -= SNAKE_SIZE;
            if (new_head.p.y.get_bit(10))
                new_head.p.y = FRAME_HEIGHT - SNAKE_SIZE;
            break;
        case DOWN:
            new_head.p.y += SNAKE_SIZE;
            if (new_head.p.y == FRAME_HEIGHT)
                new_head.p.y = 0;
            break;
    }

    for (ap_uint<7> i = SNAKE_LEN - 1; i >= 1; i--) {
#pragma HLS UNROLL
        if (score >= i) {
            snake[i] = snake[i - 1];
            if (snake[i].p == new_head.p)
                done = true;
        }
    }

    if (new_head.p.x <= food.x && new_head.p.x + SNAKE_SIZE > food.x && new_head.p.y <= food.y && new_head.p.y + SNAKE_SIZE > food.y) {
        score++;
        if (score == SNAKE_LEN) {
            done = true;
        } else {
            // New food
            food = food_loc[score];
        }
    }
    snake[0] = new_head;
    dbg("Snake[0]: %d,%d %d\n", snake[0].p.x, snake[0].p.y, snake[0].d);
}

void Game::draw_snake(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output) {
    hls::stream<streaming_data> temps[SNAKE_LEN - 1];
    g.draw(RECTANGLE_FILLED, 0xf0f0f0, snake[0].p, snake[0].p + SNAKE_SIZE, input, temps[0]);
draw_snake:
    for (ap_uint<7> i = 1; i < SNAKE_LEN - 1; i++) {
        g.draw(RECTANGLE, 0xf0f0f0, snake[i].p, snake[i].p + SNAKE_SIZE, temps[i - 1], temps[i]);
    }
    g.draw(RECTANGLE, 0xf0f0f0, snake[SNAKE_LEN - 1].p, snake[SNAKE_LEN - 1].p + SNAKE_SIZE, temps[SNAKE_LEN - 2], output);
}

void Game::draw_food(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output) {
    // Flash the food
    if (g.tick.get_bit(FCB - 2))
        output.write(input.read());
    else
        g.draw(RECTANGLE_FILLED, 0xff0000, food, food, input, output);
}

void Game::draw(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output, ap_uint<11> score) {
    if (!done) {
        hls::stream<streaming_data> i("Game::draw.i"), j("Game::draw.j");
        // Draw Snake
        draw_snake(input, i);
        // Draw food
        draw_food(i, j);
        // Show score
        g.draw_num(score, 0x00ff00, Point(FRAME_WIDTH * 90 / 100, FRAME_HEIGHT * 2 / 100), j, output);
    } else {
        // Show score
        g.draw_num(score, 0xff0000, Point(FRAME_WIDTH / 2 - FONTX / 2, FRAME_HEIGHT / 2 - FONTY / 2), input, output);
    }
}

// void Game::update_score(hls::stream<streaming_data> &input, hls::stream<streaming_data> &output, ap_uint<11> &score) {
//	streaming_data c = input.read();
//	c.score = this->score;
//	score = this->score;
//	output.write(c);
// }

void Game::run(hls::stream<pixel> &input, hls::stream<pixel> &output, ap_int<2> move) {
    hls::stream<streaming_data> i("Game::run.i"), j("Game::run.j"), k("Game::run.k");
    g.read(input, j);
    // Check key press and change state
    if (!done)
        update(move);
    draw(j, k, score);
    g.write(k, output);
}
