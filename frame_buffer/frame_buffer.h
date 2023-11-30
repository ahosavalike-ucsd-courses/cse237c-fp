#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>

#define FRAME_WIDTH 128
#define FRAME_HEIGHT 72
#define SCALE 10

typedef hls::axis<ap_uint<24>, 1, 0, 0> pixel;

void frame_buffer(hls::stream<pixel> &in, hls::stream<pixel> &out);
