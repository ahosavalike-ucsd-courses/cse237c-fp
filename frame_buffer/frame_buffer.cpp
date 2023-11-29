#include "frame_buffer.h"

void frame_buffer(hls::stream<pixel> &in, hls::stream<pixel> &out) {
#pragma HLS INTERFACE mode=axis register_mode=both port=in,out register
	static pixel input[FRAME_HEIGHT * FRAME_WIDTH] = {};
	static pixel main[FRAME_HEIGHT * FRAME_WIDTH] = {};
	static pixel output[FRAME_HEIGHT * FRAME_WIDTH] = {};
	static bool iv = false, mv = false, ov = true;
	static ap_uint<11> ii, iim, imo, io;
	pixel i, o;
	if (!iv && in.read_nb(i)) {
		input[ii] = i;
		ii++;
		if (ii == FRAME_WIDTH * FRAME_HEIGHT) {
			ii = 0;
			iv = true;
		}
	}
	if (!mv && iv) {
		main[iim] = input[iim];
		iim++;
		if (iim == FRAME_WIDTH * FRAME_HEIGHT) {
			iim = 0;
			iv = false;
			mv = true;
		}
	}
	if (mv && !ov) {
		output[imo] = main[imo];
		imo++;
		if (imo == FRAME_WIDTH * FRAME_HEIGHT) {
			imo = 0;
			iv = false;
			mv = true;
		}
	}
	if (ov) {
		out.write(output[io]);
		io++;
		if (io == FRAME_WIDTH * FRAME_HEIGHT){
			io = 0;
			ov = false;
		}
	}
}
