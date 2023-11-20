#include "game.h"

void Game::run(hls::stream<pixel> &output, hls::stream<pixel> &input) {
	g
		.read(input)
		.draw(RECTANGLE_FILLED, 0x00f0f0, g.fc, g.fc, g.fc+100, g.fc+100)
		.draw(LINE, 0xffffff, 0, 0, 720, 720)
		.draw(RECTANGLE, 0x0fff00, 600, 10, 700, 20)
		.write(output);
}
