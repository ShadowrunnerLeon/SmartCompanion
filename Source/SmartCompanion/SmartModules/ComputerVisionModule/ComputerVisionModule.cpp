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

    primaryModel.setInput(blob);

    try
    {
        outputs = primaryModel.forward();
    }
    catch (cv::Exception& e)
    {
        e.what();
    }
}

std::pair<int, int> ComputerVisionModule::postProcess()
{
    // Initialize vectors to hold respective outputs while unwrapping detections.
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

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

    // Perform Non-Maximum Suppression and draw predictions.
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, 0.25, 0.45, indices, 0.5);

    int idx = indices[0];
    cv::Rect box = boxes[idx];
    int left = box.x;
    int top = box.y;
    int width = box.width;
    int height = box.height;

    // Draw bounding box.
    cv::rectangle(img, cv::Point(left, top), cv::Point(left + width, top + height), cv::Scalar(0, 255, 0), 3);
    return std::make_pair<int, int>(int((left + width) / 2), int((top + height) / 2));
}

float ComputerVisionModule::getRotateAngle(int x0, int y0)
{
    float alpha = atan((xLength / 2 - x0) / y0);
    if (x0 < xLength) alpha *= -1;
    return alpha / PI;
}

void ComputerVisionModule::Initialize()
{
    redModel = cv::dnn::readNet("C:\\Users\\sorok\\Downloads\\yolov8\\runs\\detect\\train\\weights\\best.onnx");
	//blueModel = cv::dnn::readNet("D:\\SmartCompanion\\SmartComapnion\\Models\\OpenCV\\redModel.onnx");
}

float ComputerVisionModule::Run()
{
    auto controller = UGameplayStatics::GetPlayerController(worldContext, 0);
    auto character = (ASmartCompanionCharacter*)(controller->GetPawn());
    character->ActivateFirstPersonView();

    system("python D:\\SmartCompanion\\SmartCompanion\\Script\\screen.py");

    img = cv::imread("D:\\SmartCompanion\\SmartCompanion\\Screenshots\\screen.png");

    preProcess();
    const auto [x, y] = postProcess();

	return (!x && !y) ? 0 : getRotateAngle(x, y);

    /*
    system("python D:\\SmartCompanion\\SmartCompanion\\Script\\yolov8.py");

    std::ifstream coordsFile("D:\\SmartCompanion\\SmartCompanion\\Script\\coords.txt");
    int x, y;
    coordsFile >> x >> y;
    coordsFile.close();
    */

    character->DeactivateFirstPersonView();

    return (!x && !y) ? 0 : getRotateAngle(x, y);
}

void ComputerVisionModule::SetPrimaryModel(const std::string& modelName)
{
    modelName == "red" ? primaryModel = redModel : primaryModel = blueModel;
    UE_LOG(LogTemp, Display, TEXT("SetPrimaryModel"));
}

UWorld* ComputerVisionModule::GetWorldContext()
{
    return worldContext;
}

void ComputerVisionModule::Shutdown()
{
}
