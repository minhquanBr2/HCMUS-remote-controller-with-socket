#include "pch.h"

#include "UtilBrowseDirectory.h"

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

void BrowseDisk(std::vector<CStringW>& diskList)
{
    TCHAR szDrives[MAX_PATH] = { 0 };
    if (!GetLogicalDriveStrings(MAX_PATH, szDrives))
    {
        // Error handling
        return;
    }

    TCHAR* szDrive = szDrives;
    FILE* f = fopen("test_output.txt", "w+");
    while (*szDrive != '\0')
    {
        // Print out the drive
        CString strDrive(szDrive);
        strDrive = strDrive.Left(strDrive.GetLength() - 1);
        //_tprintf(_T("%s\n"), (LPCTSTR)strDrive);
        fprintf(f, "%s\n", (LPCTSTR)strDrive);
        diskList.push_back(CStringW(strDrive));
        // Move to the next drive
        szDrive += _tcslen(szDrive) + 1;
    }
    fclose(f);
}

BOOL BrowseDir(CStringW lpPath, std::vector<CStringW>& dirList)
{
    WIN32_FIND_DATAW ffd;
    WCHAR szDir[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    // Create the search path
    wcscpy(szDir, lpPath);
    wcscat(szDir, L"\\*");

    // Find the first file in the directory
    hFind = FindFirstFileW(szDir, &ffd);


    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // Ignore . and .. directories
            if (wcscmp(ffd.cFileName, L".") == 0 || wcscmp(ffd.cFileName, L"..") == 0)
            {
                continue;
            }

            CStringW strFileNameW = ffd.cFileName;
            CStringW strFilePathW = lpPath;
            strFilePathW += L"\\";
            strFilePathW += strFileNameW;

            /*wchar_t wstrFileName[MAX_PATH];
            int nCharsConverted = MultiByteToWideChar(CP_ACP, 0, ffd.cFileName, -1, wstrFileName, MAX_PATH);
            CStringW strFileNameW(wstrFileName, nCharsConverted);

            wchar_t wstrFilePath[MAX_PATH];
            nCharsConverted = MultiByteToWideChar(CP_ACP, 0, lpPath, -1, wstrFilePath, MAX_PATH);
            CStringW strFilePathW(wstrFilePath, nCharsConverted);*/

            wchar_t wstrDateModified[MAX_PATH];
            CString dateModified = CTime(ffd.ftLastWriteTime).Format(_T("%m/%d/%Y %H:%M:%S"));
            int nCharsConverted = MultiByteToWideChar(CP_ACP, 0, dateModified, -1, wstrDateModified, MAX_PATH);
            CStringW dateModifiedW(wstrDateModified, nCharsConverted);

            // Add folder info to dirList
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                CStringW msg = strFileNameW + L"\t" + dateModifiedW + L"\t---";
                dirList.push_back(msg);
            }
            // Add file info to dirList
            else
            {
                CString size = FormatSizeUnits(ffd.nFileSizeHigh * (MAXDWORD + 1) + ffd.nFileSizeLow);
                wchar_t wstrSize[MAX_PATH];
                nCharsConverted = MultiByteToWideChar(CP_ACP, 0, size, -1, wstrSize, MAX_PATH);
                CStringW sizeW(wstrSize, nCharsConverted);
                CStringW msg = strFileNameW + _T("\t") + dateModifiedW + _T("\t") + sizeW;
                dirList.push_back(msg);
            }

            // Write to file for testing
            std::wofstream ofs("test_output.txt");
            for (auto dir : dirList) ofs << dir << std::endl;
            ofs.close();

        } while (::FindNextFileW(hFind, &ffd));

        ::FindClose(hFind);

        return TRUE;
    }

    return FALSE;
}
