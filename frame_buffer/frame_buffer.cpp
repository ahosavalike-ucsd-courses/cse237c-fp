#include "frame_buffer.h"

void frame_buffer(hls::stream<pixel> &in, hls::stream<pixel> &out) {
#pragma HLS INTERFACE axis port=in,out
#pragma HLS PIPELINE II=1
	static pixel main[FRAME_WIDTH][FRAME_HEIGHT];
	static ap_uint<11> ix, iy, ox, oy, ocx, ocy;
	static bool outputDone = true;

	// Input
	const ap_uint<11> tix = ix, tiy = iy;
	if (outputDone && in.read_nb(main[tix][tiy])) {
		if (tix == FRAME_WIDTH - 1) {
			ix = 0;
			if (tiy == FRAME_HEIGHT - 1) {
				iy = 0;
//				outputDone = false;
			}
			else iy = tiy + 1;
		}
		else ix = tix + 1;
	}

	// Output
	pixel p = main[ox][oy];
	p.user = 0;
	p.last = 0;
	const ap_uint<11> tocx = ocx, tocy = ocy, tox = ox, toy = oy;
	const bool ocxend = tocx == SCALE-1;
	const bool ocyend = tocy == SCALE-1;
	const bool oxend = tox == FRAME_WIDTH-1;
	const bool oyend = toy == FRAME_HEIGHT-1;
	if (tocx == 0 && tocy == 0 && tox == 0 && toy == 0)
		p.user = 1;
	else if (ocxend && oxend)
		p.last = 1;
	out.write(p);
	if (ocxend) {
		ocx = 0;
		if (oxend) ox = 0;
		else ox = tox + 1;
	} else {
		ocx = tocx + 1;
	}
	if (oxend && ocxend) {
		if (ocyend) {
			ocy = 0;
			if (oyend) oy = 0;
			else oy = toy + 1;
		}
		else ocy = tocy + 1;
	}
	if (ocxend && ocyend && oxend && oyend) outputDone = true;
}
