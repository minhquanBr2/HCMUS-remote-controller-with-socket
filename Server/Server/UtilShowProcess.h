#pragma once
#include <Windows.h>
#include <string>
#include <tchar.h>

bool killProcess(DWORD pid);
bool startProcess(std::string namePro);