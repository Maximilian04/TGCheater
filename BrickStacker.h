#pragma once

#include "WindowsApi.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define TURBOSCALE (0.2)

class BrickStacker {
public:
    BrickStacker();

    void findTemplate(Mat& templ, Point& pos);
    void findTemplate(Mat& templ, Point& pos, double scale);

    void play();
    const Point& getHandPosition();
private:
    Mat screen, screenScaled;
    Mat handTemplate, handTemplateScaled;
    Point handPosition;
};