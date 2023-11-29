#include "frame_buffer.h"

void frame_buffer(hls::stream<pixel> &in, hls::stream<pixel> &out) {
#pragma HLS INTERFACE axis port=in,out
#ifdef __SYNTHESIS__
	static pixel input[SAMPLES] = {};
	static pixel main[SAMPLES] = {};
	static pixel output[SAMPLES] = {};
#else
	static pixel *input = new pixel[SAMPLES]();
	static pixel *main = new pixel[SAMPLES]();
	static pixel *output = new pixel[SAMPLES]();
#endif
	static bool iv = false, mv = false, ov = true;
	static ap_uint<20> ii, iim, imo, io;
	if (!iv && in.read_nb(input[ii])) {
		ii++;
		if (ii == SAMPLES) {
			ii = 0;
			iv = true;
		}
	}
	if (!mv && iv) {
		main[iim] = input[iim];
		iim++;
		if (iim == SAMPLES) {
			iim = 0;
			iv = false;
			mv = true;
		}
	}
	if (mv && !ov) {
		output[imo] = main[imo];
		imo++;
		if (imo == SAMPLES) {
			imo = 0;
			iv = false;
			mv = true;
		}
	}
	pixel p = ov ? output[io] : main[io];
	if (ov || mv) {
		out.write(p);
		io++;
		if (io == SAMPLES){
			io = 0;
			ov = false;
		}
	}
}
