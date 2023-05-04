#include "pch.h"

#include "UtilBrowseDirectory.h"


std::string BrowseDirectory() {
    std::string folder_path = fs::current_path().string(); // Replace with your folder path

    if (!fs::exists(folder_path))
    {
        std::cerr << "Error: Directory does not exist.\n";
        return "";
    }

    string output = "";

    for (const auto& entry : fs::directory_iterator(folder_path))
    {
        if (fs::is_directory(entry))
        {
            std::string name = entry.path().filename().string();
            if (name != "." && name != "..")
            {
                output = name + "\n";
            }
        }
        else if (fs::is_regular_file(entry))
        {
            std::string name = entry.path().filename().string();
            if (name != "." && name != "..")
            {
                output += name + "\n";
            }
        }
    }

    return output;
}

CString FormatSizeUnits(ULONGLONG bytes)
{
    static const char* suffix[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB" };

    ULONGLONG temp = bytes;
    int index = 0;

    while (temp >= 1024 && index < (sizeof(suffix) / sizeof(suffix[0]) - 1))
    {
        temp /= 1024;
        index++;
    }

    CString str;
    str.Format(_T("%I64d %s"), temp, suffix[index]);
    return str;
}

BOOL BrowseDirectory(LPCTSTR lpPath, std::vector<tuple2>& folderList, std::vector<tuple3>& fileList)
{
    WIN32_FIND_DATA ffd;
    TCHAR szDir[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    // Create the search path
    StringCchCopy(szDir, MAX_PATH, lpPath);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory
    hFind = FindFirstFile(szDir, &ffd);


    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // Ignore . and .. directories
            if (_tcscmp(ffd.cFileName, _T(".")) == 0 || _tcscmp(ffd.cFileName, _T("..")) == 0)
            {
                continue;
            }

            CString strFileName = ffd.cFileName;
            CString strFilePath = lpPath;
            strFilePath += _T("\\");
            strFilePath += strFileName;



            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // Add directory name to folderList
                CString fileName = ffd.cFileName;
                CString dateModified = CTime(ffd.ftLastWriteTime).Format(_T("%m/%d/%Y %H:%M:%S"));
                tuple2 fileInfo = std::make_tuple(fileName, dateModified);
                folderList.push_back(fileInfo);

                // Recursively call BrowseDirectory for subdirectories
                // BrowseDirectory(strFilePath, folderList, fileList);
            }
            else
            {
                // Add file name to fileList
                // Get the directory name, date modified, and size
                CString fileName = ffd.cFileName;
                CString dateModified = CTime(ffd.ftLastWriteTime).Format(_T("%m/%d/%Y %H:%M:%S"));
                CString size = FormatSizeUnits(ffd.nFileSizeHigh * (MAXDWORD + 1) + ffd.nFileSizeLow);

                // Add the directory info to the list
                tuple3 fileInfo = std::make_tuple(fileName, dateModified, size);
                //CString dirInfo = dirName + _T("\t") + dateModified + _T("\t") + size;
                fileList.push_back(fileInfo);
            }

            // Write to file for testing
            FILE* f = fopen("test_output.txt", "w+");
            for (auto folder : folderList) fprintf(f, "%s\n", (LPCTSTR)std::get<0>(folder));
            for (auto file : fileList) fprintf(f, "%s\n", (LPCTSTR)std::get<0>(file));
            fclose(f);

        } while (::FindNextFile(hFind, &ffd));

        ::FindClose(hFind);

        return TRUE;
    }

    return FALSE;
}

BOOL BrowseDirectory(LPCTSTR lpszFolder, CStringArray& folderList, CStringArray& fileList)
{
    CString strWildcard(lpszFolder);
    strWildcard += _T("\\*.*");

    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(strWildcard, &fd);

    

    return FALSE;
}
