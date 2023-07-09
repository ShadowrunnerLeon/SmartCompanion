#pragma once

#include "../../../../../ThirdParty/OpenCV/include/opencv2/opencv.hpp"

enum OS
{
    WINDOWS,
    LINUX,
    MAC
};

class ScreenCreater
{
    public:
        virtual cv::Mat Run() = 0;
        virtual ~ScreenCreater() {};

        static ScreenCreater* ScreenCreaterFabric(OS os);
};