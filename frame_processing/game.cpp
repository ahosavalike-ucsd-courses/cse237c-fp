#include "game.h"

Point food_loc[SNAKE_LEN] = {
    Point(140, 80),
    Point(530, 580),
    Point(230, 660),
    Point(90, 630),
    Point(80, 290),
    Point(200, 620),
    Point(690, 480),
    Point(470, 210),
    Point(290, 110),
    Point(300, 610),
    Point(920, 30),
    Point(780, 690),
    Point(560, 30),
    Point(1270, 470),
    Point(590, 370),
    Point(40, 460),
    Point(540, 180),
    Point(460, 250),
    Point(400, 70),
    Point(600, 20),
    Point(1240, 410),
    Point(600, 200),
    Point(300, 60),
    Point(70, 400),
    Point(670, 530),
    Point(240, 170),
    Point(390, 620),
    Point(250, 590),
    Point(230, 320),
    Point(620, 440),
    Point(390, 440),
    Point(820, 310),
    Point(980, 580),
    Point(440, 380),
    Point(710, 20),
    Point(500, 710),
    Point(1260, 90),
    Point(50, 610),
    Point(820, 680),
    Point(720, 710),
    Point(240, 440),
    Point(950, 260),
    Point(1150, 670),
    Point(660, 660),
    Point(570, 550),
    Point(510, 570),
    Point(760, 290),
    Point(1270, 520),
    Point(680, 90),
    Point(90, 160),
    Point(820, 440),
    Point(490, 30),
    Point(480, 410),
    Point(390, 650),
    Point(480, 380),
    Point(940, 190),
    Point(140, 20),
    Point(180, 370),
    Point(740, 240),
    Point(450, 380),
    Point(940, 640),
    Point(550, 610),
    Point(900, 100),
    Point(110, 520),
    Point(770, 230),
    Point(650, 320),
    Point(200, 220),
    Point(790, 680),
    Point(1220, 170),
    Point(760, 630),
    Point(180, 450),
    Point(660, 370),
    Point(40, 390),
    Point(1140, 60),
    Point(680, 150),
    Point(160, 660),
    Point(1040, 310),
    Point(510, 230),
    Point(880, 380),
    Point(830, 510),
    Point(240, 640),
    Point(1050, 670),
    Point(10, 280),
    Point(110, 30),
    Point(10, 340),
    Point(770, 270),
    Point(500, 550),
    Point(340, 690),
    Point(210, 510),
    Point(180, 150),
    Point(560, 210),
    Point(680, 620),
    Point(1080, 60),
    Point(830, 350),
    Point(930, 230),
    Point(1110, 520),
    Point(350, 10),
    Point(190, 680),
    Point(410, 200),
    Point(1000, 470),
};

void Game::reset() {
    for (ap_uint<7> i = 0; i < SNAKE_LEN; i++)
        snake[i] = SnakePiece(Point(-1, -1), RIGHT);
    snake[0] = SnakePiece(Point(SNAKE_WIDTH * SNAKE_SIZE / 2, SNAKE_HEIGHT * SNAKE_SIZE / 2), RIGHT);
    score = 0;
    food = food_loc[0];
    done = false;
}

void Game::update(ap_int<2> move) {
    // 5th bit, 16th frame, ~0.5s @30fps
    // Move can be -1, 0, 1 for turn counter clockwise, no change, turn clockwise
    snake[0].d += move;
    switch (snake[0].d) {
        case LEFT:
            snake[0].p.x -= SNAKE_SIZE;
            if (snake[0].p.x < 0)
                snake[0].p.x = SNAKE_WIDTH * SNAKE_SIZE - SNAKE_SIZE;
            break;
        case RIGHT:
            snake[0].p.x += SNAKE_SIZE;
            if (snake[0].p.x >= SNAKE_WIDTH)
                snake[0].p.x = 0;
            break;
        case UP:
            snake[0].p.y -= SNAKE_SIZE;
            if (snake[0].p.y < 0)
                snake[0].p.y = SNAKE_HEIGHT * SNAKE_SIZE - SNAKE_SIZE;
            break;
        case DOWN:
            snake[0].p.y += SNAKE_SIZE;
            if (snake[0].p.y >= 0)
                snake[0].p.y = 0;
            break;
    }
    if (snake[0].p.x <= food.x && snake[0].p.x + SNAKE_SIZE > food.x && snake[0].p.y <= food.y && snake[0].p.y + SNAKE_SIZE > food.y) {
        score++;
        if (score == SNAKE_LEN) {
            done = true;
        } else {
            // New food
            food = food_loc[score];
        }
    }
    if (g.tick.get_bit(5)) {
        for (ap_uint<7> i = 1; i < SNAKE_LEN; i++) {
            if (i < score) {
                snake[i] = snake[i - 1];
                if (snake[i].p == snake[0].p)
                    done = true;
            }
        }
    }
}

void Game::draw_head() {
    g.draw(RECTANGLE_FILLED, 0xffffff, snake[0].p, snake[0].p + 10);
}

void Game::draw_body() {
    for (ap_uint<7> i = 1; i < SNAKE_LEN; i++) {
        g.draw(RECTANGLE, 0xf0f0f0, snake[i].p, snake[i].p + 10);
    }
}

void Game::draw_food() {
    // Flash the food
    if (!g.tick.get_bit(2))
        g.draw(RECTANGLE_FILLED, 0xff0000, food - 2, food + 2);
}

void Game::draw() {
    if (done) {
        // Show score?
        g.draw_num(score, 0x00ff00, Point(1200, 10));
    } else {
        draw_head();
        // Draw snake body
        draw_body();
        // Draw food
        draw_food();
    }
}

void Game::run(hls::stream<pixel> &output, hls::stream<pixel> &input) {
    g.read(input);
    // Check key press and change state
    if (!done)
        update(0);
    if (g.tick.get_bit(5)) score++;
    if (score >= 10) score = 0;
    draw();
    g.write(output);
}
