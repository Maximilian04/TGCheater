#pragma once

#include <opencv2/opencv.hpp>
#include <windows.h>

#define DISPLAY_SCALE (1.75)

using namespace std;
using namespace cv;

class WindowsApi {
public:
    static HBITMAP convertCVMat2BMP(Mat frame);
    static void getScreenshot2CVMat(Mat& src);
private:
};