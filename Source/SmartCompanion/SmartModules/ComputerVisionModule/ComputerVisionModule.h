#pragma once

#include "../../../../ThirdParty/OpenCV/include/opencv2/opencv.hpp"
#include "../ISmartModule.h"

#include <string>

//#pragma comment(lib, "D:\\SmartCompanion\\SmartCompanion\\ThirdParty\\OpenCV\\lib\\opencv_world452.lib")

class ComputerVisionModule : public ISmartModule
{

	private:
		cv::Mat img;
		cv::dnn::Net redModel, blueModel;
		cv::dnn::Net primaryModel;
		cv::Mat outputs;
		const int xLength = 640;

	private:
		void				preProcess();
		std::pair<int, int>	postProcess();
		float				getRotateAngle(int x0, int y0);

	public:
		ComputerVisionModule();
		ComputerVisionModule(UWorld* _worldContext);

		void	Initialize()	override;
		void	Shutdown()		override;

		float	Run();

		void SetPrimaryModel(const std::string& modelName);
		UWorld* GetWorldContext();
};