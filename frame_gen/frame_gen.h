#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_int.h>

#define FRAME_HEIGHT 720
#define FRAME_WIDTH 1280

typedef ap_uint<24> rgb;
typedef hls::axis<rgb,1,1,1> pixel;

void frame_gen(hls::stream<pixel> &out);
