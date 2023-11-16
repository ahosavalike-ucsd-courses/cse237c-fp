#include "frame_processing.h"
#include <cassert>
#include <iostream>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#pragma pack(1)
typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} pixelrgb;

#pragma pack(1)
union pixconvert {
	pixelrgb p;
	unsigned int i : 24;
};


void printfb(pixconvert fb[FRAME_HEIGHT][FRAME_WIDTH]) {
	std::cout << "Framebuffer:" << std::endl;
	for (int i = 0; i < FRAME_HEIGHT; i++) {
		for (int j = 0; j < FRAME_WIDTH; j++) {
			printf("(%02x, %02x, %02x) ", fb[i][j].p.r, fb[i][j].p.g, fb[i][j].p.b);
		}
		std::cout << std::endl;
	}
}

char showfb(pixconvert fb[FRAME_HEIGHT][FRAME_WIDTH]) {
	using namespace cv;
	Mat img(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3, (void*)fb);
	cvtColor(img, img, COLOR_RGB2BGR);
	imshow("Image", img);
	return waitKey(1);
}

int main() {
	auto fb = new pixconvert[FRAME_HEIGHT][FRAME_WIDTH]();
	hls::stream<pixel> in, out;
	pixel inpix, pix;
	inpix.data = 0x0;
	std::cout << "Starting Frame Gen:" << std::endl;
	// Fill fb
	while (true) {
		for (int i = 0; i < FRAME_HEIGHT; i++) {
			for (int j = 0; j < FRAME_WIDTH; j++) {
				inpix.user = i == 0 && j == 0;
				inpix.last = j == FRAME_WIDTH-1;
				in << inpix;
				frame_processing(out, in);
				out >> pix;
				fb[i][j].i = pix.data;
				assert((pix.user == 0) != (i == 0 && j == 0));
				assert((pix.last == 0) != (j == FRAME_WIDTH - 1));
			}
		}
		if(showfb(fb) == 'c') {
			break;
		}
	}

	std::cout << "Completed Frame Gen" << std::endl;
	// Somehow print fb?
//	printfb(fb);
	cv::destroyAllWindows();
	delete[] fb;
}
