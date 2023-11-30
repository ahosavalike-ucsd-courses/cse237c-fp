#include "frame_buffer.h"

void frame_buffer(hls::stream<pixel> &in, hls::stream<pixel> &out) {
#pragma HLS INTERFACE axis port=in,out
#pragma HLS PIPELINE II=1
	static pixel main[FRAME_WIDTH][FRAME_HEIGHT];
	static ap_uint<11> ix, iy, ox, oy, ocx, ocy;
	if (in.read_nb(main[ix][iy])) {
		ix++;
		if (ix == FRAME_WIDTH) {
			ix = 0;
			iy++;
		}
		if (iy == FRAME_HEIGHT) {
			iy = 0;
		}
	}
	pixel p = main[ox][oy];
	p.user = 0;
	p.last = 0;
	if (ocx == 0 && ocy == 0 && ox == 0 && oy == 0)
		p.user = 1;
	else if (ocx == SCALE - 1 && ox == FRAME_WIDTH - 1)
		p.last = 1;
	out.write(p);
	ocx++;
	if (ocx == SCALE) {
		ocx = 0;
		ox++;
	}
	if (ox == FRAME_WIDTH) {
		ox = 0;
		ocy++;
	}
	if (ocy == SCALE) {
		ocy = 0;
		oy++;
	}
	if (oy == FRAME_HEIGHT) {
		oy = 0;
	}
}
