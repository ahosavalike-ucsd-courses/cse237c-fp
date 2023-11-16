#include "frame_processing.h"
#include "gfx_engine.h"

void frame_processing(hls::stream<pixel> &output, hls::stream<pixel> &input) {
#pragma HLS INTERFACE axis port=output,input
	static GfxEngine g;
	g.read(input);
	g.draw_diag();
	g.write(output);
}
