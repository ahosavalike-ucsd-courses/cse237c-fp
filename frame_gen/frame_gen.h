#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_int.h>

#define FRAME_HEIGHT 72
#define FRAME_WIDTH 128

typedef ap_uint<24> rgb;
typedef hls::axis<rgb,1,0,0> pixel;

void frame_gen(hls::stream<pixel> &out);
