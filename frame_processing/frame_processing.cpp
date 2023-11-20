#include "frame_processing.h"
#include "gfx_engine.h"

void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input) {
#pragma HLS INTERFACE axis port=output,input
#pragma HLS PIPELINE II=1
	static Game g;
	g.run(output, input);
}
