#pragma once
#include <iostream>
#include <filesystem>
#include <strsafe.h>

typedef std::tuple<CString, CString> tuple2;
typedef std::tuple<CString, CString, CString> tuple3;

using std::string;
using std::cout;
namespace fs = std::filesystem;

std::string BrowseDirectory();

CString FormatSizeUnits(ULONGLONG bytes);
BOOL BrowseDirectory(LPCTSTR lpPath, std::vector<tuple2>& folderList, std::vector<tuple3>& fileList);
