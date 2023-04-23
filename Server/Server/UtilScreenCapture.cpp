#include "pch.h"

#include "UtilScreenCapture.h"

#include <Windows.h>
#include <iostream>
#include <fstream>

void screenCapture()
{
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMemDC, hBitmap);

    BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    std::ofstream file("screenshot.bmp", std::ios::binary);
    BITMAPINFOHEADER bmpInfoHeader;
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biHeight = height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24;
    bmpInfoHeader.biCompression = BI_RGB;
    bmpInfoHeader.biSizeImage = 0;
    bmpInfoHeader.biXPelsPerMeter = 0;
    bmpInfoHeader.biYPelsPerMeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;

    BITMAPFILEHEADER bmfHeader;
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + bmpInfoHeader.biSize;
    bmfHeader.bfSize = bmfHeader.bfOffBits + (width * height * 3);
    bmfHeader.bfType = 0x4d42;

    file.write((char*)&bmfHeader, sizeof(BITMAPFILEHEADER));
    file.write((char*)&bmpInfoHeader, sizeof(BITMAPINFOHEADER));

    int bytesPerRow = ((width * 3 + 3) & ~3);
    char* buffer = new char[bytesPerRow];
    for (int y = height - 1; y >= 0; y--)
    {
        char* row = (char*)hBitmap + y * bytesPerRow;
        for (int x = 0; x < width; x++)
        {
            char* pixel = row + x * 3;
            file.write(pixel + 2, 1); // blue
            file.write(pixel + 1, 1); // green
            file.write(pixel + 0, 1); // red
        }
    }

    file.close();

    SelectObject(hdcMemDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);
}
    


