#include "BrickStacker.h"

BrickStacker::BrickStacker() {
    handTemplate = imread("../imageTemplates/handTemplate.png");
    if (handTemplate.empty()) {
        cout << "ERROR NO hand template" << endl;
    }
    resize(handTemplate, handTemplateScaled, Size(handTemplate.cols * TURBOSCALE, handTemplate.rows * TURBOSCALE));
}

void BrickStacker::findTemplate(Mat& templ, Point& pos) {
    Mat result(screen.rows, screen.cols, CV_32FC1);
    matchTemplate(screen, templ, result, TM_CCOEFF_NORMED);
    Point resultPos;
    double resultDouble;

    minMaxLoc(result, nullptr, &resultDouble, nullptr, &resultPos);
    pos = resultPos;
}

void BrickStacker::findTemplateTurbo(Mat& templ, Point& pos) {
    Mat result(screenScaled.rows, screenScaled.cols, CV_32FC1);
    matchTemplate(screenScaled, templ, result, TM_CCOEFF_NORMED);
    Point resultPos;
    double resultDouble;

    minMaxLoc(result, nullptr, &resultDouble, nullptr, &resultPos);
    pos = resultPos / TURBOSCALE;
}

void BrickStacker::play() {
    WindowsApi::getScreenshot2CVMat(screen);
    resize(screen, screenScaled, Size(screen.cols * TURBOSCALE, screen.rows * TURBOSCALE));

    findTemplateTurbo(handTemplateScaled, handPosition);
}

const Point& BrickStacker::getHandPosition() {
    return handPosition;
}