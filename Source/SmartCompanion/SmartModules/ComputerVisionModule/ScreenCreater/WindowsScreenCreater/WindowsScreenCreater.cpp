#include "WindowsScreenCreater.h"

WindowsScreenCreater::WindowsScreenCreater()
{
    InitializeDesktopParametrs();
    CreateBitmapHeader();
}

void WindowsScreenCreater::InitializeDesktopParametrs()
{
    screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
    width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
}

cv::Mat WindowsScreenCreater::Run()
{
    HWND hwnd = GetDesktopWindow();

    auto [hwindowDC, hwindowCompatibleDC] = GetHandles(hwnd);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    cv::Mat src(height, width, CV_8UC4);

    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);

    SelectObject(hwindowCompatibleDC, hbwindow);

    CopyWindowToBitmap(hwindowCompatibleDC, hwindowDC, hbwindow, src);
    Clear(hbwindow, hwindowCompatibleDC, hwnd, hwindowDC);

    return src;
}

std::pair<HDC, HDC> WindowsScreenCreater::GetHandles(HWND hwnd)
{
    HDC hwindowDC = GetDC(hwnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);

    return {hwindowDC, hwindowCompatibleDC};
}

void WindowsScreenCreater::CreateBitmapHeader()
{
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
}

void WindowsScreenCreater::CopyWindowToBitmap(HDC hwindowCompatibleDC, HDC hwindowDC, HBITMAP hbwindow, cv::Mat& src)
{
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
}

void WindowsScreenCreater::Clear(HBITMAP hbwindow, HDC hwindowCompatibleDC, HWND hwnd, HDC hwindowDC)
{
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);
}