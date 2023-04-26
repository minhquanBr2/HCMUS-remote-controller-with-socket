#include "pch.h"

#include "UtilScreenCapture.h"

#include <Windows.h>
#include <iostream>
#include <fstream>


int ScreenCapture() 
{
    // Get the dimensions of the primary display
    DEVMODE dm = { 0 };
    dm.dmSize = sizeof(dm);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
    int width = dm.dmPelsWidth;
    int height = dm.dmPelsHeight;

    // Create a device context for the primary display
    HDC hScreen = GetDC(NULL);
    HDC hDC = CreateCompatibleDC(hScreen);

    // Create a bitmap compatible with the primary display
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);

    // Copy the contents of the primary display to the bitmap
    BitBlt(hDC, 0, 0, width, height, hScreen, 0, 0, SRCCOPY);

    // Save the bitmap to a BMP file
    const char* filename = "screenshot.bmp";
    BITMAPINFOHEADER bmpInfoHeader = { 0 };
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biHeight = -height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24;
    bmpInfoHeader.biCompression = BI_RGB;
    bmpInfoHeader.biSizeImage = ((width * bmpInfoHeader.biBitCount + 31) / 32) * 4 * height;

    BITMAPFILEHEADER bmpFileHeader = { 0 };
    bmpFileHeader.bfType = 'MB';
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmpInfoHeader.biSizeImage;

    std::ofstream outputFile(filename, std::ios::out | std::ios::binary);
    outputFile.write(reinterpret_cast<const char*>(&bmpFileHeader), sizeof(BITMAPFILEHEADER));
    outputFile.write(reinterpret_cast<const char*>(&bmpInfoHeader), sizeof(BITMAPINFOHEADER));

    BYTE* pixels = new BYTE[bmpInfoHeader.biSizeImage];
    GetDIBits(hDC, hBitmap, 0, height, pixels, reinterpret_cast<BITMAPINFO*>(&bmpInfoHeader), DIB_RGB_COLORS);
    outputFile.write(reinterpret_cast<const char*>(pixels), bmpInfoHeader.biSizeImage);

    // Clean up
    outputFile.close();
    delete[] pixels;
    SelectObject(hDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);

    return 0;
}
    


