#include "ComputerVisionModule.h"
#include "../../SmartCompanionCharacter.h"

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"

#include <vector>
#include <fstream>
#include <cmath>

ComputerVisionModule::ComputerVisionModule()
{
}

ComputerVisionModule::ComputerVisionModule(UWorld* _worldContext)
{
    worldContext = _worldContext;
}

void ComputerVisionModule::preProcess()
{
	cv::Mat blob;
	cv::dnn::blobFromImage(img, blob, 1. / 255., cv::Size(640, 640), cv::Scalar(), true, false);

    auto net = cv::dnn::readNet(nets[primaryModelName]);
    net.setInput(blob);
    outputs = net.forward();
}

std::pair<int, int> ComputerVisionModule::postProcess()
{
    float xFactor = 1;
    float yFactor = 359.0 / 640.0;

    const int rows = 8400;

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

    // non-maximum suppression
    cv::dnn::NMSBoxes(boxes, confidences, 0.25, 0.45, indices, 0.5);

    if (indices.empty()) return { 0, 0 };

    int idx = indices[0];
    cv::Rect box = boxes[idx];
    int left = box.x;
    int top = box.y;
    int width = box.width;
    int height = box.height;

    // draw bounding box.
    cv::rectangle(img, cv::Point(left, top), cv::Point(left + width, top + height), cv::Scalar(0, 255, 0), 3);

    // clear vectors
    confidences.clear();
    boxes.clear();
    indices.clear();

    // display image
    cv::imshow("Detected Enemy", img);
    cv::waitKey(0);

    return { int((left + width) / 2), int((top + height) / 2) };
}

float ComputerVisionModule::getRotateAngle(int x0, int y0)
{
    if (!y0) return 0;
    float alpha = atan((xLength / 2 - x0) / y0);
    if (x0 < xLength) alpha *= -1;
    return alpha / PI;
}

BITMAPINFOHEADER ComputerVisionModule::createBitmapHeader(int width, int height)
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

cv::Mat ComputerVisionModule::captureScreenMat(HWND hwnd)
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
    BITMAPINFOHEADER bi = createBitmapHeader(width, height);

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

void ComputerVisionModule::Initialize()
{
    nets["red"] = baseDir + "\\Models\\OpenCV\\red\\best.onnx";
    nets["blue"] = baseDir + "\\Models\\OpenCV\\red\\best.onnx";
}

float ComputerVisionModule::Run()
{
    auto controller = UGameplayStatics::GetPlayerController(worldContext, 0);
    auto character = (ASmartCompanionCharacter*)(controller->GetPawn());
    character->ActivateFirstPersonView();

    for (int i = 0; i < 5; ++i)
    {
        img = captureScreenMat(GetDesktopWindow());
        cv::imwrite(baseDir + "\\Screenshots\\screen.png", img);
    }

    img = cv::imread(baseDir + "\\Screenshots\\screen.png");

    cv::Mat resizedImg;
    cv::resize(img, resizedImg, cv::Size(640, 359));
    img = resizedImg;

    preProcess();
    const auto [x, y] = postProcess();

    //character->DeactivateFirstPersonView();

    return (!x && !y) ? 0 : getRotateAngle(x, y);
}

void ComputerVisionModule::SetPrimaryModel(const std::string& modelName)
{
    primaryModelName = modelName;
    UE_LOG(LogTemp, Display, TEXT("SetPrimaryModel"));
}

UWorld* ComputerVisionModule::GetWorldContext()
{
    return worldContext;
}

void ComputerVisionModule::Shutdown()
{
}
