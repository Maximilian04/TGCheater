#pragma once

#include <opencv2/opencv.hpp>
#include <windows.h>
#include <winuser.h>

#define DISPLAY_SCALE (1.0)

using namespace std;
using namespace cv;

class WindowsApi {
public:
    static HBITMAP convertCVMat2BMP(Mat frame);
    static void getScreenshot2CVMat(Mat& src);

    static void pressSpace();
    static void unpressSpace();
    static bool isCtrlPressed();
private:
};