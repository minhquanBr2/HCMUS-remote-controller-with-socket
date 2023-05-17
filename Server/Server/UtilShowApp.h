#pragma once

#include <iostream>
#include <afx.h>
#include <afxwin.h>
#include <afxstr.h>
#include <Windows.h>
#include <string>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
CString GetAllWindowTitles();
BOOL CALLBACK EnumWindowsProc1(HWND hwnd, LPARAM lParam);
bool TerminateApplication(const std::string& windowTitle);
bool startApp(std::string nameApp);