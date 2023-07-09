#pragma once

#include "../ScreenCreater.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"

class WindowsScreenCreater : ScreenCreater
{
    private:
        int screenx;
        int screeny;
        int width;
        int height;

        BITMAPINFOHEADER bi;

    private:
        void InitializeDesktopParametrs();
        std::pair<HDC, HDC> GetHandles(HWND hwnd);
        void CreateBitmapHeader();
        void CopyWindowToBitmap(HDC hwindowCompatibleDC, HDC hwindowDC, HBITMAP hbwindow, cv::Mat& src);
        void Clear(HBITMAP hbwindow, HDC hwindowCompatibleDC, HWND hwnd, HDC hwindowDC);

    public:
        WindowsScreenCreater();
        cv::Mat Run() override;
};