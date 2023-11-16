#include "frame_gen.h"

void frame_gen(hls::stream<pixel> &output) {
#pragma HLS INTERFACE mode=axis register_mode=both port=output register
	pixel f;
	f.data = 0x0;
	f.keep = 0b111;
	f.strb = 0b111;
	// Start of Frame
	f.user = 1;
	send_frame_height:
	for (int i = 0; i < FRAME_HEIGHT; i++) {
		f.last = 0;
		send_frame_width:
		for (int j = 0; j < FRAME_WIDTH - 1; j++) {
			output << f;
			f.user = 0;
		}
		// End of Line
		f.last = 1;
		output << f;
	}
}
