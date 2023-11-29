#include "frame_buffer.h"

int main() {
	hls::stream<pixel> in, out;
	pixel p, q;
	p.data = 0xffffff;
	in << p;
	frame_buffer(in, out);
	if (!out.read_nb(q)) {
		return 1;
	}
	return q.data != 0x000000;
}
