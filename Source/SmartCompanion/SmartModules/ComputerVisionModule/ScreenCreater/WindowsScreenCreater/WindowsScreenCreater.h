#pragma once

#include "../IScreenCreater.h"

class WindowsScreenCreater : IScreenCreater
{
    public:
        cv::Mat Run() override;
};