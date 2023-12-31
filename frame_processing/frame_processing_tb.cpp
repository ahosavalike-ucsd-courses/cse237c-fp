#include <stdio.h>

#include <cassert>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "frame_processing.h"

typedef struct pix {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    pix() : r(0), g(0), b(0) {}
    pix(ap_uint<24> val) : r(val.range(23, 16)),
                           g(val.range(15, 8)),
                           b(val.range(7, 0)) {}
} fbtype;

void printfb(fbtype fb[FRAME_HEIGHT][FRAME_WIDTH]) {
    std::cout << "Framebuffer:" << std::endl;
    for (int i = 0; i < FRAME_HEIGHT; i++) {
        for (int j = 0; j < FRAME_WIDTH; j++) {
            printf("(%02x, %02x, %02x) ", fb[i][j].r, fb[i][j].g, fb[i][j].b);
        }
        std::cout << std::endl;
    }
}

char showfb(fbtype fb[FRAME_HEIGHT][FRAME_WIDTH]) {
    using namespace cv;
    Mat img(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3, (void*)fb);
    resize(img, img, Size(), 10, 10);
    cvtColor(img, img, COLOR_RGB2BGR);
    imshow("Image", img);
    return waitKey(1);
}

int main() {
    auto fb = new fbtype[FRAME_HEIGHT][FRAME_WIDTH]();
    hls::stream<pixel> in("main.in"), out("main.out");
    pixel inpix, pix;
    inpix.data = 0x0;
    std::cout << "Starting Frame Gen:" << std::endl;
    bool ok = false;
    bool b1 = 0, b2 = 0, b3 = 0, b4 = 0;
    // Fill fb
    char b = 0;
    while (true) {
        for (int i = 0; i < FRAME_HEIGHT; i++) {
            for (int j = 0; j < FRAME_WIDTH; j++) {
                inpix.user = i == 0 && j == 0;
                inpix.last = j == FRAME_WIDTH - 1;
                in << inpix;
                b1 = b == 'd';
                b2 = b == 'a';
                b3 = b == 'w';
                b4 = b == 'r';
                frame_processing(out, in, b1, b2, b3, b4);
                out >> pix;
                fb[i][j] = pix.data.toRGB();
                assert((pix.user == 0) != (i == 0 && j == 0));
                assert((pix.last == 0) != (j == FRAME_WIDTH - 1));
            }
        }
        //		printfb(fb);
        //		break;
        ok = true;
        b = showfb(fb);
        if (b == 'q') {
            break;
        }
    }

    std::cout << "Completed Frame Gen" << std::endl;
    cv::destroyAllWindows();
    delete[] fb;
}
