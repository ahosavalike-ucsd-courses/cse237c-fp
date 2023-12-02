#include "frame_buffer.h"
#include <cassert>

#define SAMPLES (128*72)

int main() {
	hls::stream<pixel> in, out;
	pixel p;
	p.data = 0xffffff;
	in.write(p);
	frame_buffer(in, out);
	assert(out.read_nb(p));
	assert(p.data == 0x000000);
	p.data = 0xffffff;
	for (int i = 0; i < SAMPLES; i++) {
		in.write(p);
	}
	for (int i = 0; i < SAMPLES - 2; i++) {
		frame_buffer(in, out);
		out.read();
	}
	frame_buffer(in, out);
	assert(out.read().data == 0x000000);
	frame_buffer(in, out);
	assert(out.read().data == 0xffffff);
}
