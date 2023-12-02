#include "frame_buffer.h"
#include <cassert>

#define SAMPLES (128 * 72)
#define OUTPUT_WIDTH 1280
#define OUTPUT_HEIGHT 720

int main() {
	hls::stream<pixel> in, out;
	pixel p;
	p.data = 0xffffff;
	for (int i = 0; i < SAMPLES; i++) {
		in.write(p);
	}
	for (int i = 0; i < OUTPUT_HEIGHT; i++) {
		for (int j = 0; j < OUTPUT_WIDTH - 1; j++) {
			frame_buffer(in, out);
			p = out.read();
			if (i == 0 && j == 0)
				assert(p.user == 1);
			else assert(p.user == 0 && p.last == 0);
		}
		frame_buffer(in, out);
		p = out.read();
		assert(p.data == 0xffffff);
		assert(p.user == 0);
		assert(p.last == 1);
	}
	frame_buffer(in, out);
	p = out.read();
	assert(p.data == 0xffffff);
	assert(p.user == 0);
	assert(p.last == 1);
}
