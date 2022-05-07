#include "WindowsApi.h"

HBITMAP WindowsApi::convertCVMat2BMP(cv::Mat frame) {
    // https://gist.github.com/AhiyaHiya/6e455a3a6c846766f1017044131bfab7
    auto convertOpenCVBitDepthToBits = [](const int32_t value) {
        auto regular = 0u;

        switch (value) {
        case CV_8U:
        case CV_8S:
            regular = 8u;
            break;

        case CV_16U:
        case CV_16S:
            regular = 16u;
            break;

        case CV_32S:
        case CV_32F:
            regular = 32u;
            break;

        case CV_64F:
            regular = 64u;
            break;

        default:
            regular = 0u;
            break;
        }

        return regular;
    };

    auto imageSize = frame.size();
    assert(imageSize.width && "invalid size provided by frame");
    assert(imageSize.height && "invalid size provided by frame");

    if (imageSize.width && imageSize.height) {
        auto headerInfo = BITMAPINFOHEADER{};
        ZeroMemory(&headerInfo, sizeof(headerInfo));

        headerInfo.biSize = sizeof(headerInfo);
        headerInfo.biWidth = imageSize.width;
        headerInfo.biHeight = -(imageSize.height); // negative otherwise it will be upsidedown
        headerInfo.biPlanes = 1;                   // must be set to 1 as per documentation frame.channels();

        const auto bits = convertOpenCVBitDepthToBits(frame.depth());
        headerInfo.biBitCount = frame.channels() * bits;

        auto bitmapInfo = BITMAPINFO{};
        ZeroMemory(&bitmapInfo, sizeof(bitmapInfo));

        bitmapInfo.bmiHeader = headerInfo;
        bitmapInfo.bmiColors->rgbBlue = 0;
        bitmapInfo.bmiColors->rgbGreen = 0;
        bitmapInfo.bmiColors->rgbRed = 0;
        bitmapInfo.bmiColors->rgbReserved = 0;

        auto dc = GetDC(nullptr);
        assert(dc != nullptr && "Failure to get DC");
        auto bmp = CreateDIBitmap(dc,
            &headerInfo,
            CBM_INIT,
            frame.data,
            &bitmapInfo,
            DIB_RGB_COLORS);
        assert(bmp != nullptr && "Failure creating bitmap from captured frame");

        return bmp;
    } else {
        return nullptr;
    }
}

void WindowsApi::getScreenshot2CVMat(Mat& img) {
    // основано на https://stackoverflow.com/questions/34466993/opencv-desktop-capture/36696070#36696070
    // и основано на https://stackoverflow.com/questions/3291167/how-can-i-take-a-screenshot-in-a-windows-application

    //-------- getting screenshot --------
    int x1, y1, x2, y2, w, h;

    // get screen dimensions
    x1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
    y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);
    x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    w = x2 - x1;
    h = y2 - y1;

    w *= DISPLAY_SCALE;
    h *= DISPLAY_SCALE;

    // copy screen to bitmap
    HDC hScreen = GetDC(NULL);
    HDC hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BOOL bRet = BitBlt(hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY);

    BITMAPINFOHEADER bi;

    Mat src;
    if (src.empty() || src.type() != CV_8UC4 || src.cols != w || src.rows != h) {
        src.create(h, w, CV_8UC4);
    }
    if (img.empty() || img.type() != CV_8UC3 || img.cols != w || img.rows != h) {
        img.create(h, w, CV_8UC3);
    }

    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER); // http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
    bi.biWidth = w;
    bi.biHeight = -h; // this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hDC, hBitmap);
    // copy from the window device context to the bitmap device context
    StretchBlt(hDC, 0, 0, w, h, hScreen, 0, 0, w, h, SRCCOPY);                 // change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hDC, hBitmap, 0, h, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS); // copy from hwindowCompatibleDC to hbwindow

    // clean up
    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    DeleteObject(hBitmap);

    cvtColor(src, img, COLOR_BGRA2BGR);
}