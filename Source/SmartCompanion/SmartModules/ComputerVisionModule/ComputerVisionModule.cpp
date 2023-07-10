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

ComputerVisionModule::ComputerVisionModule(UWorld* _worldContext, ScreenCreater* _screenCreater)
{
    worldContext = _worldContext;
    screenCreater = _screenCreater;
}

void ComputerVisionModule::PreProcess()
{
	cv::Mat blob;
	cv::dnn::blobFromImage(img, blob, 1. / 255., cv::Size(IMG_WIDTH, IMG_WIDTH), cv::Scalar(), true, false);

    auto net = cv::dnn::readNet(nets[primaryModelName]);
    net.setInput(blob);
    outputs = net.forward();
}

std::pair<int, int> ComputerVisionModule::PostProcess()
{
    EnemyDetection();
    NMS();
    DrawRectangle();
    ClearVectors();
    DisplayImage();

    return { int((boxLeft + boxLeft + boxWidth) / 2), int((boxTop + boxTop + boxHeight) / 2) };
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
    cv::dnn::NMSBoxes(boxes, confidences, 0.25, 0.45, indices, 0.5);
}

void ComputerVisionModule::DrawRectangle()
{
    if (indices.empty())
    {
        boxLeft = 0;
        boxTop = 0;
        boxWidth = 0;
        boxHeight = 0;

        return;
    }

    int idx = indices[0];
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
    indices.clear();
}

void ComputerVisionModule::DisplayImage()
{
    cv::imshow("Detected Enemy", img);
    cv::waitKey(0);
}

float ComputerVisionModule::GetRotateAngle(int x, int y)
{
    if (y == IMG_HEIGHT) return 0;
    float tg = abs(IMG_WIDTH / 2 - x) / (IMG_HEIGHT - y);
    float alpha = atan(tg) * 180 / PI;
    alpha = (x > IMG_WIDTH / 2) ? alpha + 90 : alpha;
    return alpha;
}

void ComputerVisionModule::Initialize()
{
    nets["red"] = baseDir + "\\Models\\OpenCV\\red\\best.onnx";
    nets["blue"] = baseDir + "\\Models\\OpenCV\\red\\best.onnx";
}

float ComputerVisionModule::Run()
{
    ActivateFirstPersonView();
    CreateScreen();
    ResizeScreen();

    PreProcess();
    const auto [x, y] = PostProcess();
    float rotateAngle = GetRotateAngle(x, y);

    return rotateAngle;
}

void ComputerVisionModule::ActivateFirstPersonView()
{
    ActivateFirstPersonView();
    CreateScreen();
    ResizeImage();

    preProcess();
    const auto [x, y] = postProcess();

    //character->DeactivateFirstPersonView();
    return getRotateAngle(x, y);
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
        img = screenCreater->Run();
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
