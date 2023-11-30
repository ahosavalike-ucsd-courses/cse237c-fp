#ifndef __CONST_TYPES__
#define __CONST_TYPES__
#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>

#define FRAME_HEIGHT 72
#define FRAME_WIDTH 128
#define SNAKE_LEN 10
#define SNAKE_SIZE 1
#define SNAKE_HEIGHT (FRAME_HEIGHT / SNAKE_SIZE)
#define SNAKE_WIDTH (FRAME_WIDTH / SNAKE_SIZE)

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

template <int SIZE>
struct PointSize {
    ap_uint<SIZE> x;
    ap_uint<SIZE> y;
    PointSize() {}
    PointSize(const PointSize<SIZE> &o) : x(o.x), y(o.y) {}
    PointSize(int i, int j) : x(i), y(j) {}
    PointSize<SIZE> operator+(PointSize<SIZE> o) {
        return PointSize<SIZE>(x + o.x, y + o.y);
    }
    PointSize<SIZE> operator+(int o) {
        return PointSize<SIZE>(x + o, y + o);
    }
    PointSize<SIZE> operator-(PointSize<SIZE> o) {
        return PointSize<SIZE>(x - o.x, y - o.y);
    }
    PointSize<SIZE> operator-(int o) {
        return PointSize<SIZE>(x - o, y - o);
    }
    bool operator==(PointSize<SIZE> o) {
        return x == o.x && y == o.y;
    }
};

typedef struct PointSize<11> Point;

typedef struct streaming_data {
	// Pixel
    rgb color;
    ap_uint<1> user, last;
    // Current pixel location
	Point loc;
	// Current Tick
	ap_uint<11> tick, score;
} streaming_data;

typedef hls::axis<rgb, 1, 0, 0> pixel;
#endif
