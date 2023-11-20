#include "game.h"

void Game::run(hls::stream<pixel> &output, hls::stream<pixel> &input) {
	g
		.read(input)
		.draw(RECTANGLE_FILLED, 0x00f0f0, Point(g.fc, g.fc), Point(g.fc+100, g.fc+100))
		.draw(LINE, 0xffffff, Point(0, 0), Point(720, 720))
		.draw(RECTANGLE, 0x0fff00, Point(600, 10), Point(700, 20))
		.write(output);
}
