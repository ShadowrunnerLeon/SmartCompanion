#pragma once

class IScreenCreater
{
    protected:
        cv::Mat screen;
    
    protected:
        virtual void Prepare() = 0;
        virtual void Create() = 0;

    public:
        virtual cv::Mat Run() = 0;
        virtual ~IScreenCreater() {};
};