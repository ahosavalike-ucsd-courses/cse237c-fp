#ifndef __CONST_TYPES__
#define __CONST_TYPES__
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>

#define FRAME_HEIGHT 720
#define FRAME_WIDTH 1280

// struct rgb uses the internal ordering for rbg but accepts rgb.
typedef struct rgb {
    ap_uint<8> r;
    ap_uint<8> b;
    ap_uint<8> g;
    rgb() : r(0), b(0), g(0) {}
    rgb(ap_uint<24> val) : r(val.range(23, 16)),
                           b(val.range(7, 0)),
                           g(val.range(15, 8)) {}
    rgb(int v) {
        ap_uint<24> val = v;
        r = val.range(23, 16);
        g = val.range(15, 8);
        b = val.range(7, 0);
    }
    void operator=(ap_uint<24> val) {
        r = val.range(23, 16);
        g = val.range(15, 8);
        b = val.range(7, 0);
    }
    void operator=(int v) {
        ap_uint<24> val = v;
        r = val.range(23, 16);
        g = val.range(15, 8);
        b = val.range(7, 0);
    }
    void rbg(ap_uint<24> val) {
        r = val.range(23, 16);
        b = val.range(15, 8);
        g = val.range(7, 0);
    }
    ap_uint<24> toRGB() {
        ap_uint<24> rgb;
        rgb.range(23, 16) = r;
        rgb.range(15, 8) = g;
        rgb.range(7, 0) = b;
        return rgb;
    }
    ap_uint<24> toBGR() {
        ap_uint<24> bgr;
        bgr.range(23, 16) = b;
        bgr.range(15, 8) = g;
        bgr.range(7, 0) = r;
        return bgr;
    }
} rgb;
typedef hls::axis<rgb, 1, 1, 1> pixel;
#endif
