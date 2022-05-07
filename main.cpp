#include "BrickStacker.h"
#include "WindowsApi.h"

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    cout << "qua" << endl;

    BrickStacker game;

    Mat info(100, 100, CV_8UC3, Scalar(0, 0, 0));

    int key = 0;
    while (key != 27) {
        circle(info, Point(game.getHandPosition().x * 100 / 2880,
            game.getHandPosition().y * 100 / 1800), 5, Scalar(0, 0, 0));
        game.play();
        circle(info, Point(game.getHandPosition().x * 100 / 2880,
            game.getHandPosition().y * 100 / 1800), 5, Scalar(255, 255, 255));

        imshow("info", info);

        key = waitKey(1);
    }

    cout << "Program returned 0" << endl;
    return 0;
}