#include "ComputerVisionModule.h"
#include "../../SmartCompanionCharacter.h"

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

ComputerVisionModule::~ComputerVisionModule()
{
}

bool ComputerVisionModule::PreProcess()
{
	cv::Mat blob;
	cv::dnn::blobFromImage(img, blob, 1. / 255., cv::Size(640, 640), cv::Scalar(), true, false);

    try
    {
        auto net = cv::dnn::readNet(nets[primaryModelName]);
        net.setInput(blob);
        outputs = net.forward();
    }
    catch (cv::Exception& exception)
    {
        UE_LOG(LogTemp, Display, TEXT("PreProcess: %s"), exception.what());
        return false;
    }

    return true;
}

std::pair<int, int> ComputerVisionModule::PostProcess()
{
    EnemyDetection();
    NMS();
    DrawRectangle();
    ClearVectors();
    DisplayImage();

    return { int(boxLeft + boxWidth / 2), int(boxTop + boxHeight / 2) };
}

void ComputerVisionModule::EnemyDetection()
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

void ComputerVisionModule::NMS()
{
    cv::dnn::NMSBoxes(boxes, confidences, 0.25, 0.45, indicies, 0.5);
}

void ComputerVisionModule::DrawRectangle()
{
    if (indicies.empty())
    {
        boxLeft = 0;
        boxTop = 0;
        boxWidth = 0;
        boxHeight = 0;

        return;
    }

    int idx = indicies[0];
    cv::Rect box = boxes[idx];
    boxLeft = box.x;
    boxTop = box.y;
    boxWidth = box.width;
    boxHeight = box.height;

    cv::rectangle(img, cv::Point(boxLeft, boxTop), cv::Point(boxLeft + boxWidth, boxTop + boxHeight), cv::Scalar(0, 255, 0), 3);
}

void ComputerVisionModule::ClearVectors()
{
    confidences.clear();
    boxes.clear();
    indicies.clear();
}

void ComputerVisionModule::DisplayImage()
{
    cv::imshow("Detected Enemy", img);
    cv::waitKey(0);
    cv::destroyWindow("Detected Enemy");
}

float ComputerVisionModule::GetRotateAngle(int x, int y)
{
    if (y == IMG_HEIGHT) return 0;
    float tg = abs(IMG_WIDTH / 2 - x) / (IMG_HEIGHT - y);
    float alpha = atan(tg) * 180 / PI;
    alpha = (x > IMG_WIDTH / 2) ? alpha / 2 : -alpha / 2;

    return alpha;
}

BITMAPINFOHEADER ComputerVisionModule::CreateBitmapHeader(int width, int height)
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

cv::Mat ComputerVisionModule::CaptureScreenMat(HWND hwnd)
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

bool ComputerVisionModule::Initialize()
{
    nets["red"] = baseDir + "\\Models\\OpenCV\\red\\best.onnx";
    nets["blue"] = baseDir + "\\Models\\OpenCV\\blue\\best.onnx";

    return true;
}

float ComputerVisionModule::Run()
{
    ActivateFirstPersonView();
    CreateScreen();
    ResizeImage();

    if (!PreProcess()) return 0.f;
    const auto [x, y] = PostProcess();
    float angle = GetRotateAngle(x, y);

    return angle;
}

void ComputerVisionModule::ActivateFirstPersonView()
{
    auto controller = UGameplayStatics::GetPlayerController(worldContext, 0);
    auto character = (ASmartCompanionCharacter*)(controller->GetPawn());
    character->ActivateFirstPersonView();
}

void ComputerVisionModule::CreateScreen()
{
    for (int i = 0; i < 5; ++i)
    {
        img = CaptureScreenMat(GetDesktopWindow());
        cv::imwrite(baseDir + "\\Screenshots\\screen.png", img);
    }

    img = cv::imread(baseDir + "\\Screenshots\\screen.png");
}

void ComputerVisionModule::ResizeImage()
{
    cv::Mat resizedImg;
    cv::resize(img, resizedImg, cv::Size(640, 359));
    img = resizedImg;
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
