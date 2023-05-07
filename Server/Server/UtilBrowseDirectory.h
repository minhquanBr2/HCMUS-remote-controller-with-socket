#pragma once
#include <iostream>
#include <filesystem>
#include <strsafe.h>
#include <fstream>

using std::string;
using std::cout;
namespace fs = std::filesystem;

CString FormatSizeUnits(ULONGLONG bytes);
void BrowseDisk(std::vector<CStringW>& diskList);
BOOL BrowseDir(CStringW lpPath, std::vector<CStringW>& dirList);