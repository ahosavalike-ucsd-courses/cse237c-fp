#include "frame_processing.h"

void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input) {
#pragma HLS INTERFACE axis port=output,input depth=1024
	static ap_uint<11> x, y, fc;
	pixel p;
	p = input.read();
	if (p.user) {
		x = y = 0;
		fc++;	// Will overflow, only track till meaningful
		if (fc > FRAME_HEIGHT)
			fc = 1;
	}
	if (x == y)
		if (x < fc)
			p.data = 0x0000ff;
		else if (x > fc)
			p.data = 0xff0000;
		else
			p.data = 0xffffff;
	x++;
	if (p.last) {
		y++;
		x = 0;
	}
	output.write(p);
}
