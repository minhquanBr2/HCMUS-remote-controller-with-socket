#include "UtilShowProcess.h"
#include "pch.h"
#include <string>

bool killProcess(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        return false;
    }
    bool result = TerminateProcess(hProcess, 0) != 0;
    CloseHandle(hProcess);
    return result;
}

bool startProcess(std::string namePro) {
    STARTUPINFOA info = { sizeof(info) };
    PROCESS_INFORMATION processInfo;

    BOOL success = CreateProcessA(
        NULL,                                           // Application name (use NULL to use command line)
        const_cast<LPSTR>(namePro.c_str()),          // Command line (as narrow string)
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