#include "ComputerVisionModule.h"

static const int rows = 8400;

COMPUTERVISIONMODULE_API float Run(const std::string& baseDir, const std::string& netPath, const int IMG_HEIGHT, const int IMG_WIDTH)
{
    cv::Mat img;
    CreateScreen(baseDir, img);
    ResizeImage(img);

    cv::Mat outputs = PreProcess(netPath, img, IMG_HEIGHT, IMG_WIDTH);
    const auto [x, y] = PostProcess(img, outputs, IMG_HEIGHT, IMG_WIDTH);
    float angle = GetRotateAngle(IMG_HEIGHT, IMG_WIDTH, x, y);

    return angle;
}

cv::Mat PreProcess(const std::string& netPath, cv::Mat& img, const int IMG_HEIGHT, const int IMG_WIDTH)
{
    cv::Mat blob;
    cv::dnn::blobFromImage(img, blob, 1. / 255., cv::Size(640, 640), cv::Scalar(), true, false);
    cv::Mat outputs;

    try
    {
        auto net = cv::dnn::readNet(netPath);
        net.setInput(blob);
        outputs = net.forward();
    }
    catch (cv::Exception& exception)
    {
        outputs = 0;
        std::cerr << "ComputerVisionModule: PreProcess: " << exception.what() << std::endl;
    }

    return outputs;
}

std::pair<int, int>	PostProcess(cv::Mat& img, cv::Mat& outputs, const int IMG_HEIGHT, const int IMG_WIDTH)
{
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    std::vector<int> indicies;

    EnemyDetection(outputs, boxes, confidences, IMG_HEIGHT, IMG_WIDTH);
    NMS(boxes, confidences, indicies);
    cv::Rect rect = DrawRectangle(img, boxes, indicies);
    DisplayImage(img);

    int boxLeft = rect.x;
    int boxTop = rect.y;
    int boxWidth = rect.width;
    int boxHeight = rect.height;

    return { int(boxLeft + boxWidth / 2), int(boxTop + boxHeight / 2) };
}

float GetRotateAngle(const int IMG_HEIGHT, const int IMG_WIDTH, int x, int y)
{
    if (y == IMG_HEIGHT) return 0;
    float tg = abs(IMG_WIDTH / 2 - x) / (IMG_HEIGHT - y);
    float alpha = atan(tg) * 180 / PI;
    alpha = (x > IMG_WIDTH / 2) ? alpha / 2 : -alpha / 2;

    return alpha;
}

BITMAPINFOHEADER CreateBitmapHeader(int width, int height)
{
    BITMAPINFOHEADER  bi;

    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    return bi;
}

void CreateScreen(const std::string& baseDir, cv::Mat& img)
{
    for (int i = 0; i < 5; ++i)
    {
        img = CaptureScreenMat(GetDesktopWindow());
        cv::imwrite(baseDir + "\\Screenshots\\screen.png", img);
    }

    img = cv::imread(baseDir + "\\Screenshots\\screen.png");
}

void ResizeImage(cv::Mat& img)
{
    cv::Mat resizedImg;
    cv::resize(img, resizedImg, cv::Size(640, 359));
    img = resizedImg;
}

void EnemyDetection(cv::Mat & outputs, std::vector<cv::Rect>& boxes, std::vector<float>& confidences, const int IMG_HEIGHT, const int IMG_WIDTH)
{
    float xFactor = IMG_WIDTH / IMG_WIDTH;
    float yFactor = IMG_HEIGHT / IMG_WIDTH;

    for (int i = 0; i < rows; ++i)
    {
        float confidence = outputs.at<float>(0, 4, i);

        if (confidence < 0.25) continue;

        boxes.push_back({
            int((outputs.at<float>(0, 0, i) - (0.5 * outputs.at<float>(0, 2, i))) * xFactor),
            int((outputs.at<float>(0, 1, i) - (0.5 * outputs.at<float>(0, 3, i))) * yFactor),
            int(outputs.at<float>(0, 2, i) * xFactor),
            int(outputs.at<float>(0, 3, i) * yFactor) });

        confidences.push_back(confidence);
    }
}

void NMS(std::vector<cv::Rect>& boxes, std::vector<float>& confidences, std::vector<int>& indicies)
{
    cv::dnn::NMSBoxes(boxes, confidences, 0.25, 0.45, indicies, 0.5);
}

cv::Rect DrawRectangle(cv::Mat& img, std::vector<cv::Rect>& boxes, std::vector<int>& indicies)
{
    if (indicies.empty()) return cv::Rect(0, 0, 0, 0);

    int idx = indicies[0];
    cv::Rect box = boxes[idx];
    int boxLeft = box.x;
    int boxTop = box.y;
    int boxWidth = box.width;
    int boxHeight = box.height;

    cv::rectangle(img, cv::Point(boxLeft, boxTop), cv::Point(boxLeft + boxWidth, boxTop + boxHeight), cv::Scalar(0, 255, 0), 3);
}

void DisplayImage(cv::Mat& img)
{
    cv::imshow("Detected Enemy", img);
    cv::waitKey(0);
    cv::destroyWindow("Detected Enemy");
}

cv::Mat CaptureScreenMat(HWND hwnd)
{
    cv::Mat src;

    // get handles to a device context (DC)
    HDC hwindowDC = GetDC(hwnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // define scale, height and width
    int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    // create mat object
    src.create(height, width, CV_8UC4);

    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    BITMAPINFOHEADER bi = CreateBitmapHeader(width, height);

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);

    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}