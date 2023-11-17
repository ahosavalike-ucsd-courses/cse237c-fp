#include "frame_gen.h"

void frame_gen(hls::stream<pixel> &output) {
#pragma HLS INTERFACE mode=axis register_mode=both port=output register
#pragma HLS PIPELINE II=1
	static ap_uint<11> lc;
	static ap_uint<20> pc;

	pixel p;
	p.last = p.user = p.data = 0x0;
	p.keep = 0b111;
	if (pc == 0)
		p.user = 1;
	if (lc == FRAME_WIDTH - 1)
		p.last = 1;

	pc++;
	lc++;
	if (lc >= FRAME_WIDTH) {
		lc = 0;
		if (pc >= FRAME_WIDTH*FRAME_HEIGHT)
			pc = 0;
	}
	output << p;
}
