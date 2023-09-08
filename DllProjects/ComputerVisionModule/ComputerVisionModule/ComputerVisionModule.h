#pragma once

#ifdef COMPUTERVISIONMODULE_EXPORTS
#define COMPUTERVISIONMODULE_API __declspec(dllexport)
#else
#define COMPUTERVISIONMODULE_API __declspec(dllimport)
#endif

#include "E:/SmartCompanion/ThirdParty/OpenCV/include/opencv2/opencv.hpp"

#include <string>
#include <vector>
#include <map>

#include <windows.h>

#define PI 3.14159265358979323846

extern "C" COMPUTERVISIONMODULE_API float Run(const std::string& baseDir, const std::string& netPath, const int IMG_HEIGHT, const int IMG_WIDTH);

cv::Mat				PreProcess(const std::string& netPath, cv::Mat& img, const int IMG_HEIGHT, const int IMG_WIDTH);
std::pair<int, int>	PostProcess(cv::Mat& img, cv::Mat &outputs, const int IMG_HEIGHT, const int IMG_WIDTH);
float				GetRotateAngle(const int IMG_HEIGHT, const int IMG_WIDTH, int x, int y);
BITMAPINFOHEADER	CreateBitmapHeader(int width, int height);
void				CreateScreen(const std::string& baseDir, cv::Mat& img);
void				ResizeImage(cv::Mat& img);
void				EnemyDetection(cv::Mat& outputs, std::vector<cv::Rect>& boxes, std::vector<float>& confidences, const int IMG_HEIGHT, const int IMG_WIDTH);
void				NMS(std::vector<cv::Rect>& boxes, std::vector<float>& confidences, std::vector<int>& indicies);
cv::Rect			DrawRectangle(cv::Mat& img, std::vector<cv::Rect>& boxes, std::vector<int>& indicies);
void				DisplayImage(cv::Mat& img);
cv::Mat				CaptureScreenMat(HWND hwnd);
