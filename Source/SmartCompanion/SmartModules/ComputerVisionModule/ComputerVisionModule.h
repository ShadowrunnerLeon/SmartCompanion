#pragma once

#include "../../ConfigData.h"
#include "../../../../ThirdParty/OpenCV/include/opencv2/opencv.hpp"
#include "../ISmartModule.h"

#include <map>
#include <string>

#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"

//#pragma comment(lib, "D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_world452.lib")

class ComputerVisionModule : public ISmartModule
{

	private:
		const int rows = 8400;

		cv::Mat img;
		cv::Mat outputs;

		std::vector<float> confidences;
		std::vector<cv::Rect> boxes;
		std::vector<int> indices;

		int boxLeft;
		int boxTop;
		int boxWidth;
		int boxHeight;

		std::map<std::string, std::string> nets;
		std::string primaryModelName;

	private:
		void				preProcess();
		std::pair<int, int>	postProcess();

		void EnemyDetection();
		void NonMaximumSuppression();
		void DrawBoundingBox();
		void ClearVectors();
		void DisplayImage();

		float				getRotateAngle(int x0, int y0);

		BITMAPINFOHEADER	createBitmapHeader(int width, int height);
		cv::Mat				captureScreenMat(HWND hwnd);

		void ActivateFirstPersonView();
		void CreateScreen();
		void ResizeScreen();

	public:
		ComputerVisionModule();
		ComputerVisionModule(UWorld* _worldContext);

		void	Initialize()	override;
		void	Shutdown()		override;

		float	Run();

		void SetPrimaryModel(const std::string& modelName);
		UWorld* GetWorldContext();
};