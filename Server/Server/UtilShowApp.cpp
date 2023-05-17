#include "UtilShowApp.h"
#include "pch.h"
#include <string>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    if (IsWindowVisible(hwnd))
    {
        constexpr int maxTitleLength = 256;
        wchar_t windowTitle[maxTitleLength];
        if (GetWindowTextW(hwnd, windowTitle, maxTitleLength) > 0)
        {
            CString* windowTitles = reinterpret_cast<CString*>(lParam);
            *windowTitles += CString(windowTitle) + _T("/");
        }
    }
    return TRUE;
}

CString GetAllWindowTitles()
{
    CString windowTitles;
    if (!EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windowTitles)))
    {
        CString Error = "Failed to enumerate windows.";
        return Error;
    }

    return windowTitles;
}

BOOL CALLBACK EnumWindowsProc1(HWND hwnd, LPARAM lParam)
{
    if (IsWindowVisible(hwnd))
    {
        constexpr int maxTitleLength = 256;
        wchar_t windowTitle[maxTitleLength];
        if (GetWindowTextW(hwnd, windowTitle, maxTitleLength) > 0)
        {
            const std::wstring& targetWindowTitle = *reinterpret_cast<const std::wstring*>(lParam);
            if (wcscmp(windowTitle, targetWindowTitle.c_str()) == 0)
            {
                DWORD processId;
                GetWindowThreadProcessId(hwnd, &processId);

                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
                if (hProcess)
                {
                    if (TerminateProcess(hProcess, 0))
                    {
                        CloseHandle(hProcess);
                        return TRUE; // Termination successful
                    }

                    CloseHandle(hProcess);
                }
            }
        }
    }

    return TRUE;
}

bool TerminateApplication(const std::string& windowTitle)
{
    std::wstring wideWindowTitle(windowTitle.begin(), windowTitle.end());
    return EnumWindows(EnumWindowsProc1, reinterpret_cast<LPARAM>(&wideWindowTitle));
}


bool startApp(std::string nameApp) {
    STARTUPINFOA info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;

    BOOL success = CreateProcessA(
        NULL,                                           // Application name (use NULL to use command line)
        const_cast<LPSTR>(nameApp.c_str()),          // Command line (as narrow string)
        NULL,                                           // Process attributes
        NULL,                                           // Thread attributes
        FALSE,                                          // Inherit handles
        0,                                              // Creation flags
        NULL,                                           // Environment variables
        NULL,                                           // Current directory
        &info,                                          // Startup info
        &processInfo                                    // Process information
    );

    if (success) {
        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
    }

    return success != 0;
}